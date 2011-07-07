
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include <KActionCollection>
#include <KComponentData>
#include <KHBox>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktexteditor/document.h>
//#include <ktexteditor/editinterface.h>
//#include <ktexteditor/highlightinginterface.h>
//#include <ktexteditor/undointerface.h>
#include <ktexteditor/view.h>
//#include <ktexteditor/viewcursorinterface.h>
//#include <ktexteditor/wordwrapinterface.h>
#include <ktoolbar.h>
#include <KVBox>
#include <qsplitter.h>
#include <QTableWidget>
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

    ui = new KVBox(enclosingPane);

    // --- Action Toolbar ---

    KToolBar* actionBar = new KToolBar(ui, false, true);
    actionBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // --- Variable Table ---

    // Prepare a splitter for the remaining components.
    QSplitter* splitter = new QSplitter(Qt::Vertical, ui);

    varTable = new QTableWidget(0, 2, splitter);
    if (! readWrite )
        varTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // TODO: If needed, uncomment below
    //varTable->setContentsMargins(varTable->contentsMargins()->setLeft(0));
    //
    //QHeader* hdr = varTable->verticalHeader();
    //hdr->hide();
    //varTable->setLeftMargin(0);

    QStringList hdr;
    hdr << i18n("Variable") << i18n("Value");
    varTable->setHorizontalHeaderLabels(hdr);

    //varTable->setColumnStretchable(0, true);
    //varTable->setColumnStretchable(1, true);

    QSizePolicy pol = QSizePolicy(QSizePolicy::Expanding,
        QSizePolicy::Expanding);
    pol.setHorizontalStretch(SCRIPT_TABLE_WEIGHT);
    pol.setVerticalStretch(SCRIPT_TABLE_WEIGHT);
    varTable->setSizePolicy(pol); 
   
    // TODO: Below seems unsupported.
    //splitter->setResizeMode(varTable, QSplitter::Stretch);
    varTable->setWhatsThis(i18n("<qt>A list of variables that will be "
        "set before the script is run.  Each variable may refer to a "
        "single packet.<p>"
        "This allows your script to easily access the other packets in "
        "this data file.</qt>"));

    // --- Text Editor ---

    // Create a view (which must be parented) before we do anything else.
    // Otherwise the Vim component crashes.
    view = document->createView(splitter);
    //editInterface = KTextEditor::editInterface(document);
    if (strcmp(document->metaObject()->className(), "Vim::Document") == 0)
        enclosingPane->setDirtinessBroken();

    // Prepare the components.
    document->setReadWrite(readWrite);
    //KTextEditor::wordWrapInterface(document)->setWordWrap(false);
    setPythonMode();

    pol = QSizePolicy(QSizePolicy::MinimumExpanding,
        QSizePolicy::MinimumExpanding);
    pol.setHorizontalStretch(SCRIPT_EDITOR_WEIGHT);
    pol.setVerticalStretch(SCRIPT_EDITOR_WEIGHT);
    view->setSizePolicy(pol); 
    // TODO: Below seems unsupported.
    //splitter->setResizeMode(view, QSplitter::Stretch);
    view->setFocus();
    view->setWhatsThis(i18n("Type the Python script into this "
        "area.  Any variables listed in the table above will be "
        "set before the script is run."));

    splitter->setTabOrder(view, varTable);
    ui->setFocusProxy(view);

    // --- Script Actions ---

    scriptActions = new KActionCollection((QObject*)0);
    
    // scriptActionList.setAutoDelete(true); TODO: This is manual now
    // for pointers.

    actAdd = scriptActions->addAction("script_add_var");
    actAdd->setText(i18n("&Add Var"));
    actAdd->setIcon(KIcon("edit-table-insert-row-below"));
    actAdd->setToolTip(i18n("Add a new script variable"));
    actAdd->setEnabled(readWrite);
    actAdd->setWhatsThis(i18n("Add a new variable to this script.<p>"
        "A script may come with any number of variables, each of which "
        "refers to a single packet.  "
        "This allows your script to easily access the other packets in "
        "this data file."));
    connect(actAdd, SIGNAL(triggered()), this, SLOT(addVariable()));
    actionBar->addAction(actAdd);

    actRemove = scriptActions->addAction("script_remove_var");
    actRemove->setText(i18n("Re&move Var"));
    actRemove->setIcon(KIcon("edit-table-delete-row"));
    actRemove->setToolTip(i18n(
        "Remove the currently selected script variable(s)"));
    actRemove->setEnabled(false);
    actRemove->setWhatsThis(i18n("Remove the selected variable(s) from "
        "this script.<p>"
        "A script may come with any number of variables, each of which "
        "refers to a single packet.  "
        "This allows your script to easily access the other packets in "
        "this data file."));
    connect(actRemove, SIGNAL(triggered()), this, 
        SLOT(removeSelectedVariable()));
    connect(varTable, SIGNAL(selectionChanged()), this,
        SLOT(updateRemoveState()));
    actionBar->addAction(actRemove);

    KAction* actSep = scriptActions->addAction("script_separator");
    actSep->setSeparator(true);
    actionBar->addAction(actSep);
    scriptActionList.append(actSep);

    KAction* actCompile = scriptActions->addAction("script_compile");;
    actCompile->setText(i18n("&Compile"));
    actCompile->setIcon(KIcon("run-build-file"));
    actCompile->setToolTip(i18n("Compile the Python script"));
    actCompile->setWhatsThis(i18n("Test whether this Python script "
        "actually compiles.  Any errors will be shown in a separate "
        "Python console."));
    connect(actCompile, SIGNAL(triggered()), this, SLOT(compile()));
    actionBar->addAction(actCompile);
    scriptActionList.append(actCompile);

    KAction* actRun = scriptActions->addAction("script_run");;
    actRun->setText(i18n("&Run"));
    actRun->setIcon(KIcon("system-run"));
    actRun->setToolTip(i18n("Execute the Python script"));
    actRun->setWhatsThis(i18n("Execute this Python script.  The "
        "script will be run in a separate Python console."));
    connect(actRun, SIGNAL(triggered()), this, SLOT(execute()));
    actionBar->addAction(actRun);
    scriptActionList.append(actRun);

    // --- Finalising ---

    // Resize the components within the splitter so that the editor has most
    // of the space.
    QList<int> sizes = splitter->sizes();
    int totalSize = sizes[0] + sizes[1];
    sizes[0] = totalSize * SCRIPT_TABLE_WEIGHT / SCRIPT_TOTAL_WEIGHT;
    if (sizes[0] < varTable->minimumHeight())
        sizes[0] = varTable->minimumHeight();
    sizes[1] = totalSize - sizes[0];
    splitter->setSizes(sizes);

    // Fill the components with data.
    refresh();

    // Clear the undo stack.

    // undoInterface has been removed.
    /*if (strcmp(document->metaObject()->className(), "Vim::Document") == 0)
        std::cerr << "Not flushing the undo list since this has strange "
            "side-effects with the Vim component." << std::endl;
    else
        KTextEditor::undoInterface(document)->clearUndo();*/

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

const QLinkedList<KAction*>& NScriptUI::getPacketTypeActions() {
    return scriptActionList;
}

QString NScriptUI::getPacketMenuText() const {
    return i18n("S&cript");
}

void NScriptUI::commit() {
    // Update the lines.
    script->removeAllLines();
    unsigned nLines = document->lines();
    for (unsigned i = 0; i < nLines; i++) {
        QString s = document->line(i);
        script->addLast(s.isNull() ? "" : s.toAscii().constData());
    }

    // Update the variables.
    script->removeAllVariables();
    unsigned nVars = varTable->rowCount();
    regina::NPacket* value;
    for (unsigned i = 0; i < nVars; i++) {
        value = dynamic_cast<ScriptVarValueItem*>(varTable->item(i, 1))->
            getPacket();
        script->addVariable(
            varTable->itemAt(i, 0)->text().toAscii().constData(),
            value ? value->getPacketLabel() : std::string());
    }

    setDirty(false);
}

void NScriptUI::refresh() {
    // Refresh the variables.
    unsigned long nVars = script->getNumberOfVariables();
    varTable->setRowCount(nVars);
    for (unsigned long i = 0; i < nVars; i++) {
        varTable->setItem(i, 0, new ScriptVarNameItem(
            script->getVariableName(i).c_str()));
        varTable->setItem(i, 1, new ScriptVarValueItem(
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
        document->clear();
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
        document->setText(allLines);
        view->setCursorPosition(KTextEditor::Cursor(0, 0));
    }

    if (! wasReadWrite)
        document->setReadWrite(false);

    setDirty(false);
}

void NScriptUI::setReadWrite(bool readWrite) {
    if ( readWrite ) {
        varTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    } else {
        varTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    document->setReadWrite(readWrite);
    actAdd->setEnabled(readWrite);
    updateRemoveState();
}

void NScriptUI::addVariable() {
    // Find a suitable variable name.
    QString varName;

    unsigned rows = varTable->rowCount();
    unsigned which = 0;
    unsigned i;

    while (true) {
        varName = QString("var") + QString::number(which);
        for (i = 0; i < rows; i++)
            if (varTable->itemAt(i, 0)->text() == varName)
                break;
        if (i == rows)
            break;
        which++;
    }

    // Add the new variable.
    //varTable->insertRows(rows); TODO: Don't think this is needed, test.
    varTable->setItem(rows, 0, new ScriptVarNameItem(varName));
    varTable->setItem(rows, 1, new ScriptVarValueItem(
        script->getTreeMatriarch(), (regina::NPacket*)0));

    // Done!
    setDirty(true);
}

void NScriptUI::removeSelectedVariables() {
    // Gather together all the rows to be deleted.
    std::set<int> rows;

    QList<QTableWidgetItem*> sel = varTable->selectedItems() ;
    int i;
    for (i = 0; i < sel.count(); i++) {
        rows.insert(sel[i]->row());
        //sel = varTable->selection(i);
        //if (sel.isActive())
        //    for (j = sel.topRow(); j <= sel.bottomRow(); j++)
        //        rows.insert(j);
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
            arg(varTable->itemAt(*rows.begin(), 0)->text());
    else if (rows.size() == 2)
        message = i18n("The variables %1 and %2 will be removed.  "
            "Are you sure?").arg(varTable->itemAt(*rows.begin(), 0)->text()).
            arg(varTable->itemAt(*rows.rbegin(), 0)->text());
    else
        message = i18n("%1 variables from %2 to %3 will be removed.  "
            "Are you sure?").arg(rows.size()).
            arg(varTable->itemAt(*rows.begin(), 0)->text()).
            arg(varTable->itemAt(*rows.rbegin(), 0)->text());

    if (KMessageBox::warningContinueCancel(ui, message) ==
            KMessageBox::Cancel)
        return;

    // Remove the variables!
    for (std::set<int>::const_iterator it = rows.end();
            it != rows.begin(); it--)
        varTable->removeRow(*it);

    setDirty(true);
}

void NScriptUI::updateRemoveState() {
    // Are we read-write?
    if (actAdd->isEnabled())
        actRemove->setEnabled(varTable->selectedItems().count() > 0);
    else
        actRemove->setEnabled(false);
}

void NScriptUI::compile() {
    ReginaPart* part = enclosingPane->getPart();
    if (part->getPythonManager().compileScript(ui, &part->getPreferences(),
            document->text() + "\n\n") == 0) {
        #ifdef BOOST_PYTHON_FOUND
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

    unsigned nVars = varTable->rowCount();
    for (unsigned i = 0; i < nVars; i++)
        vars.push_back(PythonVariable(varTable->itemAt(i, 0)->text(),
            dynamic_cast<ScriptVarValueItem*>(varTable->item(i, 1))->
                getPacket()));

    // Run the script.
    ReginaPart* part = enclosingPane->getPart();
    part->getPythonManager().launchPythonConsole(ui, &part->getPreferences(),
            document->text() + "\n\n", vars);
}

void NScriptUI::notifyScriptChanged() {
    setDirty(true);
}

void NScriptUI::setPythonMode() {
    // KDE4 lets us do document->setHighlightingMode("python") with graceful
    // failure, possibly making code smaller.
    QStringList nModes = document->highlightingModes();
    for ( int i = 0; i < nModes.count(); i++)
        if (nModes[i].toLower() == "python") {
            document->setHighlightingMode(nModes[i]);
            break;
        } 
}

