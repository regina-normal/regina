
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// Regina core includes:
#include "packet/nscript.h"

// UI includes:
#include "nscriptui.h"
#include "nscriptvaritems.h"
#include "../reginapart.h"

#include <cstring>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktexteditor/document.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/highlightinginterface.h>
#include <ktexteditor/undointerface.h>
#include <ktexteditor/view.h>
#include <qhbox.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qtable.h>
#include <qvbox.h>
#include <set>

#define SCRIPT_TABLE_WEIGHT 1
#define SCRIPT_EDITOR_WEIGHT 3
#define SCRIPT_TOTAL_WEIGHT 4

using regina::NPacket;
using regina::NScript;

NScriptUI::NScriptUI(NScript* packet, PacketPane* enclosingPane,
        KTextEditor::Document* doc, bool readWrite) :
        PacketUI(enclosingPane), script(packet), document(doc),
        isCommitting(false) {
    ui = new QSplitter(Qt::Vertical);

    // --- Variable Table ---

    QHBox* varBox = new QHBox(ui);

    varTable = new QTable(0, 2, varBox);
    varTable->setReadOnly(! readWrite);
    varBox->setStretchFactor(varTable, 1);

    QHeader* hdr = varTable->verticalHeader();
    hdr->hide();
    varTable->setLeftMargin(0);

    hdr = varTable->horizontalHeader();
    hdr->setLabel(0, i18n("Variable"));
    hdr->setLabel(1, i18n("Value"));

    varTable->setColumnStretchable(0, true);
    varTable->setColumnStretchable(1, true);

    varTable->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,
        QSizePolicy::Expanding, SCRIPT_TABLE_WEIGHT, SCRIPT_TABLE_WEIGHT));

    QVBox* varActions = new QVBox(varBox);
    varBox->setStretchFactor(varActions, 0);

    QButton* varAdd = new QPushButton(SmallIconSet("insert_table_row", 0,
        ReginaPart::factoryInstance()), i18n("&Add"), varActions);
    connect(varAdd, SIGNAL(clicked()), this, SLOT(addVariable()));

    varRemove = new QPushButton(SmallIconSet("delete_table_row", 0,
        ReginaPart::factoryInstance()), i18n("&Remove"), varActions);
    varRemove->setEnabled(false);
    connect(varRemove, SIGNAL(clicked()), this,
        SLOT(removeSelectedVariables()));
    connect(varTable, SIGNAL(selectionChanged()), this,
        SLOT(updateRemoveState()));

    QWidget* stretch = new QWidget(varActions);
    varActions->setStretchFactor(stretch, 1);

    ui->setResizeMode(varBox, QSplitter::Stretch);

    // --- Text Editor ---

    // Create a view before we do anything else.
    // Otherwise the Vim component crashes.
    view = document->createView(ui);
    editInterface = KTextEditor::editInterface(document);

    // Prepare the components.
    document->setReadWrite(readWrite);
    setPythonMode();

    view->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
        QSizePolicy::MinimumExpanding, SCRIPT_EDITOR_WEIGHT,
        SCRIPT_EDITOR_WEIGHT));
    ui->setResizeMode(view, QSplitter::Stretch);

    // --- Finalising ---

    // Resize the components within the splitter so that the editor has most
    // of the space.
    QValueList<int> sizes = ui->sizes();
    int totalSize = sizes[0] + sizes[1];
    sizes[0] = totalSize * SCRIPT_TABLE_WEIGHT / SCRIPT_TOTAL_WEIGHT;
    if (sizes[0] < varBox->minimumHeight())
        sizes[0] = varBox->minimumHeight();
    sizes[1] = totalSize - sizes[0];
    ui->setSizes(sizes);

    // Fill the components with data.
    refresh();

    // Clear the undo stack.
    // TODO: Can we work around this in a nicer way (Vim component problem)?
    if (strcmp(document->className(), "Vim::Document") == 0)
        std::cerr << "Not flushing the undo list since this has strange "
            "side-effects with the Vim component." << std::endl;
    else
        KTextEditor::undoInterface(document)->clearUndo();

    // Notify us of any changes.
    connect(varTable, SIGNAL(valueChanged(int, int)),
        this, SLOT(notifyScriptChanged()));
    connect(document, SIGNAL(textChanged()),
        this, SLOT(notifyScriptChanged()));
}

NScriptUI::~NScriptUI() {
    delete document;
}

NPacket* NScriptUI::getPacket() {
    return script;
}

QWidget* NScriptUI::getInterface() {
    return ui;
}

KTextEditor::Document* NScriptUI::getTextComponent() {
    return document;
}

void NScriptUI::commit() {
    isCommitting = true;

    // Update the lines.
    script->removeAllLines();
    unsigned nLines = editInterface->numLines();
    for (unsigned i = 0; i < nLines; i++) {
        QString s = editInterface->textLine(i);
        script->addLast(s.isNull() ? "" : s.ascii());
    }

    // Update the variables.
    script->removeAllVariables();
    unsigned nVars = varTable->numRows();
    regina::NPacket* value;
    for (unsigned i = 0; i < nVars; i++) {
        value = dynamic_cast<ScriptVarValueItem*>(varTable->item(i, 1))->
            getPacket();
        script->addVariable(varTable->text(i, 0).ascii(),
            value ? value->getPacketLabel() : std::string());
    }

    isCommitting = false;
    setDirty(false);
}

void NScriptUI::refresh() {
    // Refresh the variables.
    unsigned long nVars = script->getNumberOfVariables();
    varTable->setNumRows(nVars);
    for (unsigned long i = 0; i < nVars; i++) {
        varTable->setItem(i, 0, new ScriptVarNameItem(varTable,
            script->getVariableName(i).c_str()));
        varTable->setItem(i, 1, new ScriptVarValueItem(varTable,
            script->getTreeMatriarch(), script->getVariableValue(i).c_str()));
    }

    // Refresh the lines.
    // The first line is handled separately to avoid an additional blank
    // line from being appended.
    unsigned long nLines = script->getNumberOfLines();
    if (nLines == 0)
        editInterface->clear();
    else {
        editInterface->setText(script->getLine(0).c_str());
        for (unsigned long i = 1; i < nLines; i++)
            editInterface->insertLine(i, script->getLine(i).c_str());
    }

    setDirty(false);
}

void NScriptUI::setReadWrite(bool readWrite) {
    varTable->setReadOnly(! readWrite);
    document->setReadWrite(readWrite);
}

void NScriptUI::addVariable() {
    // Find a suitable variable name.
    QString varName;

    unsigned rows = varTable->numRows();
    unsigned which = 0;
    unsigned i;

    while (true) {
        varName = QString("var") + QString::number(which);
        for (i = 0; i < rows; i++)
            if (varTable->text(i, 0) == varName)
                break;
        if (i == rows)
            break;
        which++;
    }

    // Add the new variable.
    varTable->insertRows(rows);
    varTable->setItem(rows, 0, new ScriptVarNameItem(varTable, varName));
    varTable->setItem(rows, 1, new ScriptVarValueItem(varTable,
        script->getTreeMatriarch(), (regina::NPacket*)0));

    // Done!
    setDirty(true);
}

void NScriptUI::removeSelectedVariables() {
    // Gather together all the rows to be deleted.
    std::set<int> rows;

    int nSel = varTable->numSelections();
    QTableSelection sel;
    int i, j;
    for (i = 0; i < nSel; i++) {
        sel = varTable->selection(i);
        if (sel.isActive())
            for (j = sel.topRow(); j <= sel.bottomRow(); j++)
                rows.insert(j);
    }

    // Has anything been selected at all?
    if (rows.empty()) {
        KMessageBox::error(ui, i18n(
            "No variables are currently selected for removal."));
        return;
    }

    // Notify the user that variables will be removed.
    QString message;
    if (rows.size() == 1)
        message = i18n("The variable %1 will be removed.  Are you sure?").
            arg(varTable->text(*rows.begin(), 0));
    else if (rows.size() == 2)
        message = i18n("The variables %1 and %2 will be removed.  "
            "Are you sure?").arg(varTable->text(*rows.begin(), 0)).
            arg(varTable->text(*rows.rbegin(), 0));
    else
        message = i18n("%1 variables from %1 to %2 will be removed.  "
            "Are you sure?").arg(rows.size()).
            arg(varTable->text(*rows.begin(), 0)).
            arg(varTable->text(*rows.rbegin(), 0));

    if (KMessageBox::warningContinueCancel(ui, message) ==
            KMessageBox::Cancel)
        return;

    // Remove the variables!
    QMemArray<int> arr(rows.size());
    i = 0;
    std::set<int>::const_iterator it = rows.begin();
    while (it != rows.end()) {
        arr[i] = *it;
        it++;
        i++;
    }

    varTable->removeRows(arr);
    setDirty(true);
}

void NScriptUI::updateRemoveState() {
    varRemove->setEnabled(varTable->numSelections() > 0);
}

void NScriptUI::notifyScriptChanged() {
    if (! isCommitting)
        setDirty(true);
}

void NScriptUI::setPythonMode() {
    KTextEditor::HighlightingInterface* hi =
        KTextEditor::highlightingInterface(document);

    unsigned nModes = hi->hlModeCount();
    for (unsigned i = 0; i < nModes; i++)
        if (hi->hlModeName(i).lower() == "python") {
            hi->setHlMode(i);
            break;
        }
}

#include "nscriptui.moc"
