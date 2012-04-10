
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "file/nglobaldirs.h"
#include "packet/nscript.h"

// UI includes:
#include "bigwidget.h"
#include "nscriptui.h"
#include "packetchooser.h"
#include "packeteditiface.h"
#include "packetmanager.h"
#include "reginamain.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#if ! (NO_SRCHILITE)
#include "../srchiliteqt/Qt4SyntaxHighlighter.h"
#endif

#include <cstring>
#include <QAction>
#include <QBoxLayout>
#include <QFontMetrics>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QTableWidget>
#include <QToolBar>
#include <set>

using regina::NPacket;
using regina::NScript;

namespace {
    QRegExp rePythonIdentifier("^[A-Za-z_][A-Za-z0-9_]*$");
}

ScriptVarValueItem::ScriptVarValueItem(regina::NPacket* packet) :
        packet_(packet) {
    if (packet_)
        packet_->listen(this);

    updateData();
}

void ScriptVarValueItem::setPacket(regina::NPacket* p) {
    if (packet_)
        packet_->unlisten(this);

    packet_ = p;

    if (p)
        p->listen(this);

    updateData();
}

void ScriptVarValueItem::packetWasRenamed(regina::NPacket* p) {
    if (p == packet_)
        updateData();
}

void ScriptVarValueItem::packetToBeDestroyed(regina::NPacket* p) {
    if (p == packet_) {
        packet_->unlisten(this);
        packet_ = 0;
        updateData();
    }
}

void ScriptVarValueItem::updateData() {
    if (packet_ && ! packet_->getPacketLabel().empty()) {
        setText(packet_->getPacketLabel().c_str());
        setIcon(PacketManager::icon(packet_));
    } else {
        setText("<None>");
        setIcon(QIcon());
    }
}

QWidget* ScriptNameDelegate::createEditor(QWidget* parent,
        const QStyleOptionViewItem&, const QModelIndex&) const {
    QLineEdit* e = new QLineEdit(parent);
    e->setValidator(new QRegExpValidator(rePythonIdentifier, e));
    return e;
}

void ScriptNameDelegate::setEditorData(QWidget* editor,
        const QModelIndex& index) const {
    QString data = index.model()->data(index, Qt::EditRole).toString();

    QLineEdit* e = static_cast<QLineEdit*>(editor);
    e->setText(data);
}

void ScriptNameDelegate::setModelData(QWidget* editor,
        QAbstractItemModel* model, const QModelIndex& index) const {
    QLineEdit* e = static_cast<QLineEdit*>(editor);
    QString data = e->text().trimmed();

    if (data.isEmpty()) {
        ReginaSupport::info(e,
            tr("Variable names cannot be empty."));
        return;
    }
    if (! rePythonIdentifier.exactMatch(data)) {
        QString oldData(data);

        // Construct a better variable name.
        data.replace(QRegExp("[^A-Za-z0-9_]"), "");
        if (data.isEmpty())
            return;
        if (! rePythonIdentifier.exactMatch(data))
            data.prepend('_');

        ReginaSupport::info(e,
            tr("<qt><tt>%1</tt> is not a valid Python variable name.</qt>").
                arg(Qt::escape(oldData)),
            tr("<qt>I have changed it to <tt>%1</tt> instead.</qt>").
                arg(Qt::escape(data)));

    }
    if (nameUsedElsewhere(data, index.row(), model)) {
        QString oldData(data);

        // Construct a unique variable name.
        int which;
        for (which = 0; nameUsedElsewhere(data + QString::number(which),
                index.row(), model); which++)
            ;
        data.append(QString::number(which));

        ReginaSupport::info(e,
            tr("<qt>Another variable is already using the "
                "name <tt>%1</tt>.</qt>").arg(Qt::escape(oldData)),
            tr("<qt>I will use <tt>%1</tt> instead.</qt>").
                arg(Qt::escape(data)));
    }

    model->setData(index, data, Qt::EditRole);
}

void ScriptNameDelegate::updateEditorGeometry(QWidget* editor,
        const QStyleOptionViewItem& option, const QModelIndex&) const {
    editor->setGeometry(option.rect);
}

bool ScriptNameDelegate::nameUsedElsewhere(const QString& name, int currRow,
        QAbstractItemModel* model) {
    int rows = model->rowCount();
    for (int i = 0; i < rows; i++) {
        if (i == currRow)
            continue;
        if (model->data(model->index(i, 0)).toString() == name)
            return true;
    }
    return false;
}

QWidget* ScriptValueDelegate::createEditor(QWidget* parent,
        const QStyleOptionViewItem&, const QModelIndex&) const {
    PacketChooser* e = new PacketChooser(matriarch_,
        0 /* filter */, true /* allow "none" */,
        0 /* initial selection */, parent);
    e->setAutoUpdate(true);
    return e;
}

void ScriptValueDelegate::setEditorData(QWidget* editor,
        const QModelIndex& index) const {
    PacketChooser* e = static_cast<PacketChooser*>(editor);
    e->selectPacket(static_cast<ScriptVarValueItem*>(
        table_->item(index.row(), index.column()))->getPacket());
}

void ScriptValueDelegate::setModelData(QWidget* editor,
        QAbstractItemModel*, const QModelIndex& index) const {
    PacketChooser* e = static_cast<PacketChooser*>(editor);
    NPacket* p = e->selectedPacket();
    ScriptVarValueItem* item = static_cast<ScriptVarValueItem*>(table_->item(
        index.row(), index.column()));

    if (item->getPacket() != p)
        item->setPacket(p);
}

void ScriptValueDelegate::updateEditorGeometry(QWidget* editor,
        const QStyleOptionViewItem& option, const QModelIndex&) const {
    editor->setGeometry(option.rect);
}

NScriptUI::NScriptUI(NScript* packet, PacketPane* enclosingPane) :
        PacketUI(enclosingPane), script(packet) {
    bool readWrite = enclosingPane->isReadWrite();

    ui = new BigWidget(1, 2);
    QVBoxLayout* layout = new QVBoxLayout(ui);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // --- Action Toolbar ---

    QToolBar* actionBar = new QToolBar(ui);
    actionBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    layout->addWidget(actionBar);

    // --- Variable Table ---

    // Prepare a splitter for the remaining components.
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    layout->addWidget(splitter, 1);

    varTable = new ScriptVarTable(0, 2);
    varTable->setSelectionMode(QAbstractItemView::ContiguousSelection);

    if (readWrite )
        varTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    else
        varTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    varTable->setWhatsThis(tr("<qt>A list of variables that will be "
        "set before the script is run.  Each variable may refer to a "
        "single packet.<p>"
        "This allows your script to easily access the other packets in "
        "this data file.</qt>"));

    varTable->verticalHeader()->hide();

    varTable->setHorizontalHeaderLabels(
        QStringList() << tr("Variable") << tr("Value"));
    varTable->horizontalHeader()->setStretchLastSection(true);

    nameDelegate = new ScriptNameDelegate();
    valueDelegate = new ScriptValueDelegate(varTable,
        packet->getTreeMatriarch());
    varTable->setItemDelegateForColumn(0, nameDelegate);
    varTable->setItemDelegateForColumn(1, valueDelegate);

    splitter->addWidget(varTable);
   
    // --- Text Editor ---

    document = new QPlainTextEdit(splitter);
    // Prepare the components.
    document->setReadOnly(!readWrite);
    document->setLineWrapMode(QPlainTextEdit::NoWrap);
    document->setFont(ReginaPrefSet::fixedWidthFont());
    updateTabWidth();

#if ! (NO_SRCHILITE)
    srchiliteqt::Qt4SyntaxHighlighter* highlighter =
        new srchiliteqt::Qt4SyntaxHighlighter(document->document());
    highlighter->init("python.lang", "default.style");
#endif

    document->setFocus();
    document->setWhatsThis(tr("Type the Python script into this "
        "area.  Any variables listed in the table above will be "
        "set before the script is run."));

    editIface = new PacketEditTextEditor(document);

    splitter->addWidget(document);

    splitter->setTabOrder(document, varTable);
    ui->setFocusProxy(document);

    // --- Script Actions ---

    //scriptActions = new KActionCollection((QObject*)0);
    
    actAdd = new QAction(this);
    //scriptActions->addAction("script_add_var");
    actAdd->setText(tr("&Add Var"));
    actAdd->setIcon(ReginaSupport::themeIcon("edit-table-insert-row-below"));
    actAdd->setToolTip(tr("Add a new script variable"));
    actAdd->setEnabled(readWrite);
    actAdd->setWhatsThis(tr("Add a new variable to this script.<p>"
        "A script may come with any number of variables, each of which "
        "refers to a single packet.  "
        "This allows your script to easily access the other packets in "
        "this data file."));
    connect(actAdd, SIGNAL(triggered()), this, SLOT(addVariable()));
    actionBar->addAction(actAdd);
    scriptActionList.append(actAdd);

    actRemove = new QAction(this);
    //scriptActions->addAction("script_remove_var");
    actRemove->setText(tr("Re&move Var"));
    actRemove->setIcon(ReginaSupport::themeIcon("edit-table-delete-row"));
    actRemove->setToolTip(tr(
        "Remove the currently selected script variable(s)"));
    actRemove->setEnabled(false);
    actRemove->setWhatsThis(tr("Remove the selected variable(s) from "
        "this script.<p>"
        "A script may come with any number of variables, each of which "
        "refers to a single packet.  "
        "This allows your script to easily access the other packets in "
        "this data file."));
    connect(actRemove, SIGNAL(triggered()), this, 
        SLOT(removeSelectedVariables()));
    connect(varTable, SIGNAL(itemSelectionChanged()), this,
        SLOT(updateRemoveState()));
    actionBar->addAction(actRemove);
    scriptActionList.append(actRemove);

    QAction* actSep = new QAction(this);
    //scriptActions->addAction("script_separator");
    actSep->setSeparator(true);
    actionBar->addAction(actSep);
    scriptActionList.append(actSep);

    QAction* actCompile = new QAction(this);
    //scriptActions->addAction("script_compile");;
    actCompile->setText(tr("&Compile"));
    actCompile->setIcon(ReginaSupport::themeIcon("run-build-file"));
    actCompile->setToolTip(tr("Compile the Python script"));
    actCompile->setWhatsThis(tr("Test whether this Python script "
        "actually compiles.  Any errors will be shown in a separate "
        "Python console."));
    connect(actCompile, SIGNAL(triggered()), this, SLOT(compile()));
    actionBar->addAction(actCompile);
    scriptActionList.append(actCompile);

    QAction* actRun = new QAction(this);
    //scriptActions->addAction("script_run");;
    actRun->setText(tr("&Run"));
    actRun->setIcon(ReginaSupport::themeIcon("system-run"));
    actRun->setToolTip(tr("Execute the Python script"));
    actRun->setWhatsThis(tr("Execute this Python script.  The "
        "script will be run in a separate Python console."));
    connect(actRun, SIGNAL(triggered()), this, SLOT(execute()));
    actionBar->addAction(actRun);
    scriptActionList.append(actRun);

    // --- Finalising ---

    // Make the editor get most of the space.
    splitter->setStretchFactor(0 /* index */, 0 /* weight */);
    splitter->setStretchFactor(1 /* index */, 1 /* weight */);

    // Fill the components with data.
    refresh();
    // varTable->horizontalHeader()->resizeSections(
    //     QHeaderView::ResizeToContents);

    // Notify us of any changes.
    connect(varTable, SIGNAL(itemChanged(QTableWidgetItem*)),
        this, SLOT(notifyScriptChanged()));
    connect(document, SIGNAL(textChanged()),
        this, SLOT(notifyScriptChanged()));

    // Notify us if the preferences (e.g., the default fixed-width font)
    // change.
    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

NScriptUI::~NScriptUI() {
    // Make sure the actions, including separators, are all deleted.
    for (QLinkedList<QAction*>::iterator it = scriptActionList.begin() ;
            it != scriptActionList.end(); it++ )
        delete *it;


    // Clean up.
    delete nameDelegate;
    delete valueDelegate;
    delete editIface;
    delete document;
}

NPacket* NScriptUI::getPacket() {
    return script;
}

QWidget* NScriptUI::getInterface() {
    return ui;
}

const QLinkedList<QAction*>& NScriptUI::getPacketTypeActions() {
    return scriptActionList;
}

QString NScriptUI::getPacketMenuText() const {
    return tr("S&cript");
}

void NScriptUI::commit() {
    // Finish whatever edit was going on in the table.
    varTable->endEdit();

    // Update the lines.
    script->removeAllLines();
    QStringList lines = document->toPlainText().split("\n");
    for( QStringList::iterator it = lines.begin(); it != lines.end(); ++it) 
        script->addLast((*it).isNull() ? "" : (*it).toAscii().constData());


    // Update the variables.
    script->removeAllVariables();
    unsigned nVars = varTable->rowCount();
    regina::NPacket* value;
    for (unsigned i = 0; i < nVars; i++) {
        value = dynamic_cast<ScriptVarValueItem*>(varTable->item(i, 1))->
            getPacket();
        script->addVariable(
            varTable->item(i, 0)->text().toAscii().constData(),
            value ? value->getPacketLabel() : std::string());
    }

    setDirty(false);
}

void NScriptUI::refresh() {
    // Refresh the variables.
    unsigned long nVars = script->getNumberOfVariables();
    varTable->setRowCount(nVars);

    regina::NPacket* matriarch = script->getTreeMatriarch();
    for (unsigned long i = 0; i < nVars; i++) {
        varTable->setItem(i, 0, new QTableWidgetItem(
            script->getVariableName(i).c_str()));
        varTable->setItem(i, 1, new ScriptVarValueItem(
            matriarch->findPacketLabel(script->getVariableValue(i).c_str())));
    }

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
        document->setPlainText(allLines);
        document->moveCursor(QTextCursor::Start);
    }

    setDirty(false);
}

void NScriptUI::setReadWrite(bool readWrite) {
    if (readWrite)
        varTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    else
        varTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    document->setReadOnly(!readWrite);
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
            if (varTable->item(i, 0)->text() == varName)
                break;
        if (i == rows)
            break;
        which++;
    }

    // Add the new variable.
    varTable->insertRow(rows);
    QTableWidgetItem* nameItem = new QTableWidgetItem(varName);
    varTable->setItem(rows, 0, nameItem);
    varTable->setItem(rows, 1, new ScriptVarValueItem(0));
    varTable->scrollToItem(nameItem);

    // Done!
    setDirty(true);
}

void NScriptUI::removeSelectedVariables() {
    // Gather together all the rows to be deleted.
    std::set<int> rows;

    // Note that selections are contiguous.
    if (varTable->selectedRanges().empty()) {
        ReginaSupport::info(ui,
            tr("No variables are selected."),
            tr("Please select one or more variables to remove, "
            "then press <i>Remove Var</i> again."));
        return;
    }
    QTableWidgetSelectionRange range = varTable->selectedRanges().front();
    std::cerr << "REMOVING: " << range.topRow() << "--" <<
        range.bottomRow() << std::endl;

    // Notify the user that variables will be removed.
    QMessageBox msgBox(ui);
    msgBox.setWindowTitle(tr("Question"));
    msgBox.setIcon(QMessageBox::Question);
    if (range.bottomRow() == range.topRow()) {
        msgBox.setText(tr("<qt>The variable <tt>%1</tt> will be removed.</qt>").
            arg(Qt::escape(varTable->item(range.topRow(), 0)->text())));
        msgBox.setInformativeText(tr("Are you sure?"));
    } else {
        msgBox.setText(tr("%1 variables will be removed.").
            arg(range.bottomRow() - range.topRow() + 1));
        msgBox.setInformativeText(tr("Are you sure?"));
    }

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() == QMessageBox::Cancel)
        return;

    // Remove the variables!
    for (int i = range.bottomRow(); i >= range.topRow(); --i)
        varTable->removeRow(i);

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
    if (enclosingPane->getMainWindow()->getPythonManager().compileScript(ui,
            document->toPlainText() + "\n\n") == 0) {
        #ifdef BOOST_PYTHON_FOUND
        ReginaSupport::success(ui,
            tr("The script compiles successfully."));
        #endif
    } else
        ReginaSupport::failure(ui,
            tr("The script does not compile."),
            tr("See the Python console for details.  You may interact with "
            "this console to further investigate the problem."));
}

void NScriptUI::execute() {
    // Finish whatever edit was going on in the table.
    varTable->endEdit();

    // Set up the variable list.
    PythonVariableList vars;

    unsigned nVars = varTable->rowCount();
    for (unsigned i = 0; i < nVars; i++)
        vars.push_back(PythonVariable(varTable->item(i, 0)->text(),
            dynamic_cast<ScriptVarValueItem*>(varTable->item(i, 1))->
                getPacket()));

    // Run the script.
    enclosingPane->getMainWindow()->getPythonManager().launchPythonConsole(ui,
            document->toPlainText() + "\n\n", vars);
}

void NScriptUI::notifyScriptChanged() {
    setDirty(true);
}

void NScriptUI::updatePreferences() {
    document->setFont(ReginaPrefSet::fixedWidthFont());
    updateTabWidth();
}

void NScriptUI::updateTabWidth() {
    document->setTabStopWidth(
        QFontMetrics(document->font()).width('x') *
        ReginaPrefSet::global().pythonSpacesPerTab);
}
