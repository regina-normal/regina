
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

// Regina core includes:
#include "regina-config.h"
#include "file/globaldirs.h"
#include "packet/script.h"

// UI includes:
#include "bigwidget.h"
#include "docwidget.h"
#include "edittableview.h"
#include "scriptui.h"
#include "packetchooser.h"
#include "packeteditiface.h"
#include "packetmanager.h"
#include "reginamain.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include "syntax/definition.h"
#include "syntax/repository.h"
#include "syntax/syntaxhighlighter.h"
#include "syntax/theme.h"

#include <cstring>
#include <QAction>
#include <QBoxLayout>
#include <QFontMetrics>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QToolBar>
#include <set>

using regina::Packet;
using regina::Script;

namespace {
    QRegExp rePythonIdentifier("^[A-Za-z_][A-Za-z0-9_]*$");

    // The syntax highlighting repository of definitions and themes is
    // a singleton: it is created on demand, and never deleted.
    regina::syntax::Repository* repository;
}

QWidget* ScriptValueDelegate::createEditor(QWidget* parent,
        const QStyleOptionViewItem&, const QModelIndex&) const {
    PacketChooser* e = new PacketChooser(script_->root(),
        0 /* filter */, PacketChooser::ROOT_AS_SUBTREE,
        true /* allow "none" */, 0 /* initial selection */, parent);
    e->setAutoUpdate(true);
    return e;
}

void ScriptValueDelegate::setEditorData(QWidget* editor,
        const QModelIndex& index) const {
    PacketChooser* e = static_cast<PacketChooser*>(editor);
    e->selectPacket(script_->variableValue(index.row()));
}

void ScriptValueDelegate::setModelData(QWidget* editor,
        QAbstractItemModel*, const QModelIndex& index) const {
    PacketChooser* e = static_cast<PacketChooser*>(editor);
    script_->setVariableValue(index.row(), e->selectedPacket());
}

void ScriptValueDelegate::updateEditorGeometry(QWidget* editor,
        const QStyleOptionViewItem& option, const QModelIndex&) const {
    editor->setGeometry(option.rect);
}

ScriptVarModel::ScriptVarModel(Script* script, bool readWrite) :
        script_(script), isReadWrite_(readWrite) {
}

void ScriptVarModel::rebuild() {
    beginResetModel();
    endResetModel();
}

QModelIndex ScriptVarModel::index(int row, int column,
        const QModelIndex& /* unused parent */) const {
    return createIndex(row, column, quint32(2 * row + column));
}

int ScriptVarModel::rowCount(const QModelIndex&) const {
    return script_->countVariables();
}

int ScriptVarModel::columnCount(const QModelIndex&) const {
    return 2;
}

QVariant ScriptVarModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return script_->variableName(index.row()).c_str();
        else if (index.column() == 1) {
            Packet* p = script_->variableValue(index.row());
            if (! p)
                return tr("<None>");
            else if (p->label().empty())
                return tr("(no label)");
            else
                return p->label().c_str();
        } else
            return QVariant();
    } else if (role == Qt::DecorationRole) {
        if (index.column() == 1) {
            Packet* p = script_->variableValue(index.row());
            if (! p)
                return QIcon();
            else
                return PacketManager::icon(p);
        } else
            return QVariant();
    } else if (role == Qt::EditRole) {
        if (index.column() == 0)
            return script_->variableName(index.row()).c_str();
        else if (index.column() == 1) {
            // This is handled by the delegate class.
            return QVariant();
        } else
            return QVariant();
    } else
        return QVariant();
}

QVariant ScriptVarModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
        case 0: return tr("Variable");
        case 1: return tr("Value");
    }
    return QVariant();
}

Qt::ItemFlags ScriptVarModel::flags(const QModelIndex&) const {
    if (isReadWrite_)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool ScriptVarModel::setData(const QModelIndex& index, const QVariant& value,
        int) {
    if (index.column() == 0) {
        QString data = value.toString().trimmed();

        if (data.isEmpty()) {
            ReginaSupport::info(0,
                tr("Variable names cannot be empty."));
            return false;
        }
        if (! rePythonIdentifier.exactMatch(data)) {
            QString oldData(data);

            // Construct a better variable name.
            data.replace(QRegExp("[^A-Za-z0-9_]"), "");
            if (data.isEmpty()) {
                ReginaSupport::info(0,
                    tr("<qt><tt>%1</tt> is not a valid Python "
                        "variable name.</qt>").arg(oldData.toHtmlEscaped()));
                return false;
            }
            if (! rePythonIdentifier.exactMatch(data))
                data.prepend('_');

            ReginaSupport::info(0,
                tr("<qt><tt>%1</tt> is not a valid Python variable name.</qt>").
                    arg(oldData.toHtmlEscaped()),
                tr("<qt>I have changed it to <tt>%1</tt> instead.</qt>").
                    arg(data.toHtmlEscaped()));
        }
        if (nameUsedElsewhere(data, index.row())) {
            QString oldData(data);

            // Construct a unique variable name.
            int which;
            for (which = 0; nameUsedElsewhere(data + QString::number(which),
                    index.row()); ++which)
                ;
            data.append(QString::number(which));

            ReginaSupport::info(0,
                tr("<qt>Another variable is already using the "
                    "name <tt>%1</tt>.</qt>").arg(oldData.toHtmlEscaped()),
                tr("<qt>I will use <tt>%1</tt> instead.</qt>").
                    arg(data.toHtmlEscaped()));
        }

        script_->setVariableName(index.row(), data.toUtf8().constData());
        return true;
    } else if (index.column() == 1) {
        // This is handled by the delegate class.
        return false;
    } else
        return false;
}

bool ScriptVarModel::nameUsedElsewhere(const QString& name, int exclude) const {
    int n = script_->countVariables();
    for (int i = 0; i < n; ++i) {
        if (i == exclude)
            continue;
        if (name == script_->variableName(i).c_str())
            return true;
    }
    return false;
}

ScriptUI::ScriptUI(Script* packet, PacketPane* enclosingPane) :
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

    model = new ScriptVarModel(packet, readWrite);
    varTable = new EditTableView();
    varTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
    varTable->setModel(model);

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
    varTable->horizontalHeader()->setStretchLastSection(true);

    valueDelegate = new ScriptValueDelegate(packet);
    varTable->setItemDelegateForColumn(1, valueDelegate);

    splitter->addWidget(varTable);

    // --- Text Editor ---

    editWidget = new DocWidget<Script, DocWidgetFinalNewline>(
        packet, splitter);
    editWidget->setReadOnly(!readWrite);
    editWidget->setLineWrapMode(QPlainTextEdit::NoWrap);
    editWidget->setFont(ReginaPrefSet::fixedWidthFont());
    updateTabWidth();

    if (! repository)
        repository = new regina::syntax::Repository;

    auto h = new SyntaxHighlighter(editWidget->document());
    h->setDefinition(repository->definitionForName("Python"));
    h->setTheme(repository->theme("Default"));

    editWidget->setFocus();
    editWidget->setWhatsThis(tr("Type the Python script into this "
        "area.  Any variables listed in the table above will be "
        "set before the script is run."));

    editIface = new PacketEditPlainTextEditor(editWidget);

    splitter->addWidget(editWidget);

    splitter->setTabOrder(editWidget, varTable);
    ui->setFocusProxy(editWidget);

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

    QAction* actRun = new QAction(this);
    //scriptActions->addAction("script_run");;
    actRun->setText(tr("&Run"));
    actRun->setIcon(ReginaSupport::regIcon("run"));
    actRun->setToolTip(tr("Execute the Python script"));
    actRun->setWhatsThis(tr("Execute this Python script.  The "
        "script will be run in a separate Python console."));
    connect(actRun, SIGNAL(triggered()), this, SLOT(execute()));
    actionBar->addAction(actRun);
    scriptActionList.append(actRun);

    // --- Finalising ---

    // Make the editor get most of the space.
    splitter->setStretchFactor(0 /* index */, 1 /* weight */);
    splitter->setStretchFactor(1 /* index */, 2 /* weight */);

    // Fill the components with data.
    refresh();
    // varTable->horizontalHeader()->resizeSections(
    //     QHeaderView::ResizeToContents);

    // Notify us if the preferences (e.g., the default fixed-width font)
    // change.
    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

ScriptUI::~ScriptUI() {
    // Make sure the actions, including separators, are all deleted.
    for (QLinkedList<QAction*>::iterator it = scriptActionList.begin() ;
            it != scriptActionList.end(); it++ )
        delete *it;

    // Clean up.
    delete valueDelegate;
    delete model;
    delete editIface;
    delete editWidget;
}

Packet* ScriptUI::getPacket() {
    return script;
}

QWidget* ScriptUI::getInterface() {
    return ui;
}

const QLinkedList<QAction*>& ScriptUI::getPacketTypeActions() {
    return scriptActionList;
}

QString ScriptUI::getPacketMenuText() const {
    return tr("S&cript");
}

void ScriptUI::refresh() {
    // Refresh the variables.
    model->rebuild();

    // Refresh the text.
    editWidget->refresh();

    // Update any actions as necessary.
    updateRemoveState();
}

void ScriptUI::endEdit() {
    varTable->endEdit();
    editWidget->commit();
}

void ScriptUI::setReadWrite(bool readWrite) {
    model->setReadWrite(readWrite);

    if (readWrite)
        varTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    else
        varTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    editWidget->setReadOnly(!readWrite);
    actAdd->setEnabled(readWrite);
    updateRemoveState();
}

void ScriptUI::addVariable() {
    endEdit();

    // Find a suitable variable name.
    QString varName;

    unsigned n = script->countVariables();
    unsigned which = 0;
    unsigned i;

    while (true) {
        varName = QString("var") + QString::number(which);
        for (i = 0; i < n; ++i)
            if (varName == script->variableName(i).c_str())
                break;
        if (i == n)
            break;
        ++which;
    }

    // Add the new variable.
    // TODO: Alter addVariable() to return the index immediately, so we
    // don't need to fetch it again.
    script->addVariable(varName.toUtf8().constData(), 0);
    varTable->scrollTo(model->index(
        script->variableIndex(varName.toUtf8().constData()), 0, QModelIndex()));
}

void ScriptUI::removeSelectedVariables() {
    endEdit();

    // Note that selections are contiguous.
    if (! varTable->selectionModel()->hasSelection()) {
        ReginaSupport::info(ui,
            tr("No variables are selected."),
            tr("Please select one or more variables to remove, "
            "then press <i>Remove Var</i> again."));
        return;
    }

    // Gather together all the rows to be deleted.
    std::set<int> rows;
    QModelIndexList indices = varTable->selectionModel()->selectedIndexes();
    for (QModelIndexList::Iterator it = indices.begin(); it != indices.end();
            ++it)
        rows.insert(it->row());

    // Notify the user that variables will be removed.
    QMessageBox msgBox(ui);
    msgBox.setWindowTitle(tr("Question"));
    msgBox.setIcon(QMessageBox::Question);
    if (rows.size() == 1) {
        msgBox.setText(tr("<qt>The variable <tt>%1</tt> will be removed.</qt>").
            arg(QString(script->variableName(*rows.begin()).c_str()).
                toHtmlEscaped()));
        msgBox.setInformativeText(tr("Are you sure?"));
    } else {
        msgBox.setText(tr("%1 variables will be removed.").arg(rows.size()));
        msgBox.setInformativeText(tr("Are you sure?"));
    }

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() == QMessageBox::Cancel)
        return;

    // Remove the variables!
    // Since std::set uses sorted order, we can delete from the bottom
    // up without affecting the indices of the rows yet to be removed.
    for (std::set<int>::reverse_iterator rit = rows.rbegin();
            rit != rows.rend(); ++rit)
        script->removeVariable(*rit);
}

void ScriptUI::updateRemoveState() {
    // Are we read-write?
    if (actAdd->isEnabled())
        actRemove->setEnabled(script->countVariables() > 0);
    else
        actRemove->setEnabled(false);
}

void ScriptUI::execute() {
    endEdit();

    // Set up the variable list.
    PythonVariableList vars;

    unsigned nVars = script->countVariables();
    for (unsigned i = 0; i < nVars; i++)
        vars.push_back(PythonVariable(script->variableName(i).c_str(),
            script->variableValue(i)));

    // Run the script.
    enclosingPane->getMainWindow()->getPythonManager().launchPythonConsole(ui,
            editWidget->toPlainText() + "\n\n", vars);
}

void ScriptUI::updatePreferences() {
    editWidget->setFont(ReginaPrefSet::fixedWidthFont());
    updateTabWidth();
}

void ScriptUI::updateTabWidth() {
    editWidget->setTabStopWidth(
        QFontMetrics(editWidget->font()).width('x') *
        ReginaPrefSet::global().pythonSpacesPerTab);
}
