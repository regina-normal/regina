
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// Regina core includes:
#include "regina-config.h"
#include "packet/nscript.h"

// UI includes:
#include "nscriptui.h"
#include "nscriptvaritems.h"
#include "../reginapart.h"

#include <cstring>
#include <kaction.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktexteditor/document.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/highlightinginterface.h>
#include <ktexteditor/undointerface.h>
#include <ktexteditor/view.h>
#include <ktexteditor/wordwrapinterface.h>
#include <ktoolbar.h>
#include <qhbox.h>
#include <qsplitter.h>
#include <qtable.h>
#include <qvbox.h>
#include <qwhatsthis.h>
#include <set>

#define SCRIPT_TABLE_WEIGHT 1
#define SCRIPT_EDITOR_WEIGHT 3
#define SCRIPT_TOTAL_WEIGHT 4

using regina::NPacket;
using regina::NScript;

NScriptUI::NScriptUI(NScript* packet, PacketPane* enclosingPane,
        KTextEditor::Document* doc) :
        PacketUI(enclosingPane), script(packet), document(doc) {
    bool readWrite = enclosingPane->isReadWrite();

    ui = new QVBox(enclosingPane);

    // --- Action Toolbar ---

    KToolBar* actionBar = new KToolBar(ui, "scriptActionBar", false, false);
    actionBar->setFullSize(true);
    actionBar->setIconText(KToolBar::IconTextRight);

    // --- Variable Table ---

    // Prepare a splitter for the remaining components.
    QSplitter* splitter = new QSplitter(Qt::Vertical, ui);

    varTable = new QTable(0, 2, splitter);
    varTable->setReadOnly(! readWrite);

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
    splitter->setResizeMode(varTable, QSplitter::Stretch);
    QWhatsThis::add(varTable, i18n("<qt>A list of variables that will be "
        "set before the script is run.  Each variable may refer to a "
        "single packet.<p>"
        "This allows your script to easily access the other packets in "
        "this data file.</qt>"));

    // --- Text Editor ---

    // Create a view (which must be parented) before we do anything else.
    // Otherwise the Vim component crashes.
    view = document->createView(splitter);
    editInterface = KTextEditor::editInterface(document);
    if (strcmp(document->className(), "Vim::Document") == 0)
        enclosingPane->setDirtinessBroken();

    // Prepare the components.
    document->setReadWrite(readWrite);
    KTextEditor::wordWrapInterface(document)->setWordWrap(false);
    setPythonMode();

    view->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
        QSizePolicy::MinimumExpanding, SCRIPT_EDITOR_WEIGHT,
        SCRIPT_EDITOR_WEIGHT));
    splitter->setResizeMode(view, QSplitter::Stretch);
    view->setFocus();
    QWhatsThis::add(view, i18n("Type the Python script into this "
        "area.  Any variables listed in the table above will be "
        "set before the script is run."));

    splitter->setTabOrder(view, varTable);
    ui->setFocusProxy(view);

    // --- Script Actions ---

    scriptActions = new KActionCollection(0, 0, 0,
        ReginaPart::factoryInstance());
    scriptActionList.setAutoDelete(true);

    actAdd = new KAction(i18n("&Add Var"), "insert_table_row",
        0 /* shortcut */, this, SLOT(addVariable()), scriptActions,
        "script_add_var");
    actAdd->setToolTip(i18n("Add a new script variable"));
    actAdd->setEnabled(readWrite);
    actAdd->setWhatsThis(i18n("Add a new variable to this script.<p>"
        "A script may come with any number of variables, each of which "
        "refers to a single packet.  "
        "This allows your script to easily access the other packets in "
        "this data file."));
    actAdd->plug(actionBar);
    scriptActionList.append(actAdd);

    actRemove = new KAction(i18n("Re&move Var"), "delete_table_row",
        0 /* shortcut */, this, SLOT(removeSelectedVariables()),
        scriptActions, "script_remove_var");
    actRemove->setToolTip(i18n(
        "Remove the currently selected script variable(s)"));
    actRemove->setEnabled(false);
    actRemove->setWhatsThis(i18n("Remove the selected variable(s) from "
        "this script.<p>"
        "A script may come with any number of variables, each of which "
        "refers to a single packet.  "
        "This allows your script to easily access the other packets in "
        "this data file."));
    connect(varTable, SIGNAL(selectionChanged()), this,
        SLOT(updateRemoveState()));
    actRemove->plug(actionBar);
    scriptActionList.append(actRemove);

    actionBar->insertLineSeparator();
    scriptActionList.append(new KActionSeparator());

    KAction* actCompile = new KAction(i18n("&Compile"), "compfile",
        0 /* shortcut */, this, SLOT(compile()), scriptActions,
        "script_compile");
    actCompile->setToolTip(i18n("Compile the Python script"));
    actCompile->setWhatsThis(i18n("Test whether this Python script "
        "actually compiles.  Any errors will be shown in a separate "
        "Python console."));
    actCompile->plug(actionBar);
    scriptActionList.append(actCompile);

    KAction* actRun = new KAction(i18n("&Run"), "run", 0 /* shortcut */,
        this, SLOT(execute()), scriptActions,
        "script_run");
    actRun->setToolTip(i18n("Execute the Python script"));
    actRun->setWhatsThis(i18n("Execute this Python script.  The "
        "script will be run in a separate Python console."));
    actRun->plug(actionBar);
    scriptActionList.append(actRun);

    // --- Finalising ---

    // Resize the components within the splitter so that the editor has most
    // of the space.
    QValueList<int> sizes = splitter->sizes();
    int totalSize = sizes[0] + sizes[1];
    sizes[0] = totalSize * SCRIPT_TABLE_WEIGHT / SCRIPT_TOTAL_WEIGHT;
    if (sizes[0] < varTable->minimumHeight())
        sizes[0] = varTable->minimumHeight();
    sizes[1] = totalSize - sizes[0];
    splitter->setSizes(sizes);

    // Fill the components with data.
    refresh();

    // Clear the undo stack.
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
    // Make sure the actions, including separators, are all deleted.
    scriptActionList.clear();

    // Clean up.
    delete scriptActions;
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

const QPtrList<KAction>& NScriptUI::getPacketTypeActions() {
    return scriptActionList;
}

QString NScriptUI::getPacketMenuText() const {
    return i18n("S&cript");
}

void NScriptUI::commit() {
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

    // A kate part needs to be in read-write mode before we can alter its
    // contents.
    bool wasReadWrite = document->isReadWrite();
    if (! wasReadWrite)
        document->setReadWrite(true);

    // Refresh the lines.
    // The first line is handled separately to avoid an additional blank
    // line from being appended.
    unsigned long nLines = script->getNumberOfLines();
    if (nLines == 0)
        editInterface->clear();
    else {
        // Bloody hell.
        // Trying to support both kate and vimpart with line-by-line
        // insertion is just too much drama, especially with vimpart's
        // continually changing behaviour.
        // Just use setText() and be done with it.
        QString allLines;
        for (unsigned long i = 0; i < nLines; i++) {
            allLines += script->getLine(i).c_str();
            if (i + 1 < nLines)
                allLines += '\n';
        }
        editInterface->setText(allLines);
    }

    if (! wasReadWrite)
        document->setReadWrite(false);

    setDirty(false);
}

void NScriptUI::setReadWrite(bool readWrite) {
    varTable->setReadOnly(! readWrite);
    document->setReadWrite(readWrite);

    actAdd->setEnabled(readWrite);
    updateRemoveState();
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
        message = i18n("%1 variables from %2 to %3 will be removed.  "
            "Are you sure?").arg(rows.size()).
            arg(varTable->text(*rows.begin(), 0)).
            arg(varTable->text(*rows.rbegin(), 0));

    if (KMessageBox::warningContinueCancel(ui, message) ==
            KMessageBox::Cancel)
        return;

    // Remove the variables!
    QMemArray<int> arr(rows.size());
    i = 0;
    for (std::set<int>::const_iterator it = rows.begin();
            it != rows.end(); it++)
        arr[i++] = *it;

    varTable->removeRows(arr);
    setDirty(true);
}

void NScriptUI::updateRemoveState() {
    // Are we read-write?
    if (actAdd->isEnabled())
        actRemove->setEnabled(varTable->numSelections() > 0);
    else
        actRemove->setEnabled(false);
}

void NScriptUI::compile() {
    ReginaPart* part = enclosingPane->getPart();
    if (part->getPythonManager().compileScript(ui, &part->getPreferences(),
            editInterface->text() + "\n\n") == 0) {
        #ifdef HAVE_BOOST_PYTHON
        KMessageBox::information(ui,
            i18n("The script compiles successfully."), i18n("Success"));
        #endif
    } else
        KMessageBox::error(ui, i18n("The script does not compile.\n"
            "See the Python console for details.  You may interact with "
            "this console to further investigate the problem."),
            i18n("Compile Failure"));
}

void NScriptUI::execute() {
    // Set up the variable list.
    PythonVariableList vars;

    unsigned nVars = varTable->numRows();
    for (unsigned i = 0; i < nVars; i++)
        vars.push_back(PythonVariable(varTable->text(i, 0),
            dynamic_cast<ScriptVarValueItem*>(varTable->item(i, 1))->
                getPacket()));

    // Run the script.
    ReginaPart* part = enclosingPane->getPart();
    part->getPythonManager().launchPythonConsole(ui, &part->getPreferences(),
            editInterface->text() + "\n\n", vars);
}

void NScriptUI::notifyScriptChanged() {
    setDirty(true);
}

void NScriptUI::setPythonMode() {
    KTextEditor::HighlightingInterface* hi =
        KTextEditor::highlightingInterface(document);
    if (! hi)
        return;

    unsigned nModes = hi->hlModeCount();
    for (unsigned i = 0; i < nModes; i++)
        if (hi->hlModeName(i).lower() == "python") {
            hi->setHlMode(i);
            break;
        }
}

#include "nscriptui.moc"
