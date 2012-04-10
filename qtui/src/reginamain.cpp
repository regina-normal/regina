
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

#include "regina-config.h"
#include "file/nxmlfile.h"
#include "packet/ncontainer.h"
#include "surfaces/nnormalsurfacelist.h"

#include "eventids.h"
#include "examplesaction.h"
#include "introdialog.h"
#include "messagelayer.h"
#include "packetmanager.h"
#include "packettreeview.h"
#include "packetui.h"
#include "recentfilesaction.h"
#include "reginaabout.h"
#include "reginafilter.h"
#include "reginamain.h"
#include "reginamanager.h"
#include "reginapref.h"
#include "reginasupport.h"

#include <QApplication>
#include <QBoxLayout>
#include <QColor>
#include <QDesktopWidget>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSize>
#include <QSplitter>
#include <QTextDocument>
#include <QTreeView>
#include <QTreeWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QWhatsThis>

ReginaMain::ReginaMain(ReginaManager* useManager, bool starterWindow) :
        QMainWindow(),
        manager(useManager),
        packetTree(0),
        starterWindow_(starterWindow),
        packetMenu(0),
        aboutApp(0),
        dockedPane(0),
        dirty(false),
        supportingDock(ReginaPrefSet::global().useDock) {
    setAttribute(Qt::WA_DeleteOnClose);

    // Accept drag and drop.
    setAcceptDrops(true);

    // Set up our widgets and actions.
    setupWidgets();
    setupActions();

    // Initialise the packet tree.
    initPacketTree();

    // Other tidying up.
    setModified(false);
    updateTreeActions();

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

ReginaMain::~ReginaMain() {
    if (aboutApp)
        delete aboutApp;

    // Make an emergency closure of any remaining packet panes.
    QLinkedList<PacketPane*> panes = allPanes;
    QLinkedList<PacketPane*>::iterator i;
    for (i=panes.begin() ; i!=panes.end(); i++)
        delete *i;

    // Delete the visual tree before the underlying packets so that
    // we don't get a flood of change events.
    delete treeView;

    // Finish cleaning up.
    delete packetTree;
}

void ReginaMain::plugPacketMenu() {
    if (packetMenu) {
        menuBar()->removeAction(packetMenu->menuAction());
        delete packetMenu;
        packetMenu = 0;
    }
    if (dockedPane) {
        packetMenu = new QMenu(tr("Docked %1").arg(dockedPane->getUI()->
            getPacketMenuText()));
        menuBar()->insertMenu(toolMenu->menuAction(), packetMenu);
        dockedPane->fillPacketTypeMenu(packetMenu);
    }
}

void ReginaMain::unplugPacketMenu() {
    if (packetMenu) {
        menuBar()->removeAction(packetMenu->menuAction());
        delete packetMenu;
        packetMenu = 0;
    }
}

void ReginaMain::setModified(bool modified) {
    dirty = modified;
    actSave->setEnabled(modified);

    if (starterWindow_ && modified) {
        starterWindow_ = false;
        delete advice;
        advice = 0;
    }
}

void ReginaMain::packetView(regina::NPacket* packet, bool makeVisibleInTree,
        bool selectInTree) {
    PacketExternalViewer ext = PacketManager::externalViewer(packet);
    if (ext) {
        (*ext)(packet, this);
    } else
        view(new PacketPane(this, packet));

    if (makeVisibleInTree || selectInTree) {
        PacketTreeItem* item = treeView->find(packet);
        if (! item) {
            // We cannot find the item in the tree.
            // Perhaps this is because the packet was just created and
            // the tree has not been refreshed yet?
            // Force a refresh now and try again.
            if (packet->getTreeParent()) {
                PacketTreeItem* parent =
                    treeView->find(packet->getTreeParent());
                if (parent) {
                    parent->refreshSubtree();
                    item = treeView->find(packet);
                }
            }
        }
        if (item) {
            if (makeVisibleInTree)
                treeView->scrollToItem(item);
            if (selectInTree)
                treeView->setCurrentItem(item);
        }
    }
}

void ReginaMain::ensureVisibleInTree(regina::NPacket* packet) {
    PacketTreeItem* item = treeView->find(packet);
    if (item)
        treeView->scrollToItem(item);
}

void ReginaMain::dock(PacketPane* newPane) {
    // Get rid of the currently docked pane by whatever means necessary.
    if (! closeDockedPane())
        dockedPane->floatPane();

    newPane->setParent(dockArea);
    static_cast<QBoxLayout*>(dockArea->layout())->addWidget(newPane, 1);
    dockedPane = newPane;

    newPane->registerEditOperations(actCut, actCopy, actPaste);
    
    // Postpone plugPacketMenu() until after this run through the event
    // loop, in the hope of avoiding the strange situation where menus
    // end up in the wrong order even though QMenuBar is trying to
    // insert them exactly where they should go...
    QApplication::postEvent(this,
        new QEvent((QEvent::Type)EVT_PLUG_PACKET_MENU));
}

void ReginaMain::aboutToUndock(PacketPane* undockedPane) {
    unplugPacketMenu();
    undockedPane->deregisterEditOperations();

    if (dockedPane == undockedPane) {
        dockedPane = 0;
    }
}

void ReginaMain::isClosing(PacketPane* closingPane) {
    allPanes.removeAll(closingPane);
}

void ReginaMain::dragEnterEvent(QDragEnterEvent *event) {
    if( event->mimeData()->hasUrls() )
      event->acceptProposedAction();
}

void ReginaMain::dropEvent(QDropEvent *event) {
    // Accept a dropped URL (or URLs).

    // See if we can decode a URI.  If not, just ignore it.
    if (event->mimeData()->hasUrls()) {
        // We have a URI, so process it.
        // Load in each file.
        const QList<QUrl>& urls(event->mimeData()->urls());
        for (QList<QUrl>::const_iterator it = urls.begin();
                it != urls.end(); ++it)
            fileOpenUrl(*it);
    }
}

void ReginaMain::closeEvent(QCloseEvent *event) {
    if (! closeAllPanes()) {
        event->ignore();
        return;
    }

    // Do we really want to close?
    while (dirty) {
        QMessageBox msgBox(QMessageBox::Information, tr("Information"),
            tr("The data file has been modified."),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            this);
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Cancel) {
            event->ignore();
            return;
        }
        if (ret == QMessageBox::Discard)
            break;

        fileSave();
    }

    consoles.closeAllConsoles();

    ReginaPrefSet::global().windowMainSize = size();
    ReginaPrefSet::save();

    manager->aboutToClose(this);
    event->accept();
}

QSize ReginaMain::sizeHint() const {
    if (ReginaPrefSet::global().windowMainSize.isValid())
        return ReginaPrefSet::global().windowMainSize;

    // Use the suggested width, but expand to 2/3 the screen height.
    QSize ans = QMainWindow::sizeHint();

    int ht = QApplication::desktop()->availableGeometry().height();
    ht *= 2;
    ht /= 3;
    if (ht > ans.height())
        ans.setHeight(ht);

    return ans;
}

void ReginaMain::customEvent(QEvent* evt) {
    switch (evt->type()) {
        case EVT_PLUG_PACKET_MENU:
            plugPacketMenu(); break;
        default:
            break;
    }
}

void ReginaMain::fileNew() {
    manager->newWindow(false);
}

void ReginaMain::fileOpen() {
    QString file = QFileDialog::getOpenFileName(this, tr("Open Data File"),
        QString(), tr(FILTER_SUPPORTED));
    if (! file.isNull())
        fileOpenUrl(QUrl::fromLocalFile(file));
}

void ReginaMain::fileOpenUrl(const QUrl& url) {
    // Can we read data from the file?
    QString localFile = url.toLocalFile();
    if (localFile.isEmpty()) {
        ReginaSupport::warn(this,
            tr("The filename is empty."),
            tr("<qt>There may be a miscommunication between Regina and "
            "your operating system.  "
            "Please report this to the developers at <i>%1</i>.</qt>").
            arg(PACKAGE_BUGREPORT));
        return;
    }

    regina::NPacket* packetTree = regina::readFileMagic(
        static_cast<const char*>(QFile::encodeName(localFile)));

    if (! packetTree) {
        ReginaSupport::sorry(this,
            tr("I could not open the selected file."),
            tr("<qt>Please check that the file <tt>%1</tt> "
            "is readable and in Regina format.</qt>").
            arg(Qt::escape(localFile)));
        return;
    }

    // All good, we have some real data.  Let's go.
    // If we already have a document open, make a new window.
    ReginaMain* useWindow = (starterWindow_ ? this : manager->newWindow(false));
    useWindow->initData(packetTree, localFile, QString());
}

void ReginaMain::fileOpenExample(const QUrl& url, const QString& description) {
    // Same as fileOpenUrl(), but give a pleasant message if the file
    // doesn't seem to exist.
    QString localFile = url.toLocalFile();
    if (! QFile(localFile).exists()) {
        ReginaSupport::warn(this,
            tr("I could not locate the example that you requested."),
            tr("<qt>The example \"%1\" may not have been installed properly.  "
            "Please contact <i>%2</i> for assistance.").
            arg(Qt::escape(description)).arg(PACKAGE_BUGREPORT));
        return;
    }

    regina::NPacket* packetTree = regina::readXMLFile(
        static_cast<const char*>(QFile::encodeName(localFile)));

    if (! packetTree) {
        ReginaSupport::warn(this,
            tr("I could not open the example that you requested."),
            tr("<qt>The example \"%1\" may not have been installed properly.  "
            "Please contact <i>%2</i> for assistance.").
            arg(Qt::escape(description)).arg(PACKAGE_BUGREPORT));
        return;
    }

    // All good, we have some real data.  Let's go.
    // If we already have a document open, make a new window.
    ReginaMain* useWindow = (starterWindow_ ? this : manager->newWindow(false));
    useWindow->initData(packetTree, QString(), description);
}

void ReginaMain::fileSave() {
    if (localFile.isEmpty())
        fileSaveAs();
    else
        saveFile();
}

void ReginaMain::fileSaveAs() {
    QString file = QFileDialog::getSaveFileName(this,
        tr("Save Data File"), QString(), tr(FILTER_REGINA));

    if (file.isEmpty())
        return;

    // Do we need to add an extension?
    // Qt seems to handle this for us automatically.
    /*
    if (QFileInfo(file).suffix().isEmpty())
        file += ReginaAbout::regDataExt;
    */

    // Does this file already exist?
    // Don't warn the user; Qt seems to do this for us.
    /*
    if (QFileInfo(file).exists()) {
        QMessageBox msgBox(QMessageBox::Information,
            tr("Information"),
            tr("A file with this name already exists."),
            QMessageBox::Save | QMessageBox::Cancel,
            this);
        msgBox.setInformativeText(tr("Are you sure you wish to "
            "overwrite it?"));
        msgBox.setDefaultButton(QMessageBox::Cancel);
        if (msgBox.exec() != QMessageBox::Save)
            return;
    }
    */

    // Go ahead and save it.
    if (localFile != file) {
        localFile = file;
        setWindowTitle(localFile);
    }

    saveFile();
    ReginaPrefSet::addRecentFile(QUrl::fromLocalFile(localFile));
}

void ReginaMain::packetView() {
    regina::NPacket* packet = checkPacketSelected();
    if (packet)
        packetView(packet, false);
}

void ReginaMain::packetRename() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    bool ok;
    QString suggest = packet->getPacketLabel().c_str();
    while (true) {
        QString newLabel = QInputDialog::getText(this,
            tr("Rename Packet"), tr("New label:"), QLineEdit::Normal,
                suggest, &ok).trimmed();
        if ((! ok) || (newLabel == packet->getPacketLabel().c_str()))
            return;

        // Has this label already been used?
        if (packetTree->findPacketLabel(newLabel.toAscii().constData())) {
            ReginaSupport::info(this,
                tr("Another packet is already using this label."),
                tr("Each packet in your data file must have its own "
                    "unique label.  I will suggest a different label "
                    "that is not in use."));
            suggest = packetTree->makeUniqueLabel(
                newLabel.toAscii().constData()).c_str();
        } else {
            // It's a unique label; we can rename it!
            packet->setPacketLabel(newLabel.toAscii().constData());
            return;
        }
    }
}

void ReginaMain::packetDelete() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;
    if (! packet->getTreeParent()) {
        ReginaSupport::info(this,
            tr("You cannot delete the root of the packet tree."),
            tr("You may delete any other packet except for this one."));
        return;
    }

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("Warning"));
    QPushButton* delBtn;
    if (packet->getFirstTreeChild()) {
        msgBox.setText(tr("<qt>You are about to delete the packet <i>%1</i> "
            "and all of its children.</qt>").
            arg(Qt::escape(packet->getPacketLabel().c_str())));
        delBtn = msgBox.addButton(tr("Delete All"),
            QMessageBox::AcceptRole);
    } else {
        msgBox.setText(tr("<qt>You are about to delete the packet "
            "<i>%1</i>.</qt>").
            arg(Qt::escape(packet->getPacketLabel().c_str())));
        delBtn = msgBox.addButton(tr("Delete"),
            QMessageBox::AcceptRole);
    }
    msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
    msgBox.setDefaultButton(delBtn);
    msgBox.setInformativeText("Are you sure?");
    msgBox.exec();
    if (msgBox.clickedButton() != delBtn)
        return;

    delete packet;
}

void ReginaMain::subtreeRefresh() {
    if (! checkSubtreeSelected())
        return;

    // Refresh the tree itself.
    PacketTreeItem* item = dynamic_cast<PacketTreeItem*>(
        treeView->selectedItems().first());
    item->refreshSubtree();

    // Refresh any relevant packet panes.
    regina::NPacket* subtree = item->getPacket();
    for (QLinkedList<PacketPane *>::iterator it = allPanes.begin();
            it != allPanes.end(); it++) {
        PacketPane * pane = (*it);
        if (subtree->isGrandparentOf(pane->getPacket()))
            pane->refresh();
    }
}

void ReginaMain::clonePacket() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;
    if (! packet->getTreeParent()) {
        ReginaSupport::info(this,
            tr("You cannot clone the top-level packet in the tree."),
            tr("Any other packet in the tree may be cloned except "
            "for this one."));
        return;
    }

    regina::NPacket* ans = packet->clone(false, false);

    treeView->selectPacket(ans, true);
    packetView(ans, false);
}

void ReginaMain::cloneSubtree() {
    regina::NPacket* packet = checkSubtreeSelected();
    if (! packet)
        return;
    if (! packet->getTreeParent()) {
        ReginaSupport::info(this,
            tr("You cannot clone the entire tree."),
            tr("Any subtree may be cloned, however."));
        return;
    }

    regina::NPacket* ans = packet->clone(true, false);

    treeView->selectPacket(ans, true);
    packetView(ans, false);
}

void ReginaMain::pythonConsole() {
    consoles.launchPythonConsole(this, packetTree,
        treeView->selectedPacket());
}

void ReginaMain::optionsPreferences() {
    ReginaPreferences dlg(this);
    dlg.exec();
}

void ReginaMain::helpAboutApp() {
    if (! aboutApp)
        aboutApp = new ReginaAbout(this);
    aboutApp->show();
}

void ReginaMain::helpHandbook() {
    ReginaPrefSet::openHandbook("index", 0, this);
}

void ReginaMain::helpXMLRef() {
    ReginaPrefSet::openHandbook("index", "regina-xml", this);
}

void ReginaMain::helpPythonReference() {
    PythonManager::openPythonReference(this);
}

void ReginaMain::helpWhatsThis() {
    QWhatsThis::enterWhatsThisMode();
}

void ReginaMain::helpTipOfDay() {
    // TODO: Tip of the day
    //KTipDialog::showTip(this, QString::null, true);
}

void ReginaMain::helpTrouble() {
    ReginaPrefSet::openHandbook("troubleshooting", 0, this);
}

void ReginaMain::helpIntro() {
    QDialog* dlg = new IntroDialog(this);
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}

void ReginaMain::floatDockedPane() {
    // Delegate the entire procedure to PacketPane::floatPane().
    // Processing will return to this class when PacketPane calls
    // ReginaMain::aboutToUndock().
    if (dockedPane)
        dockedPane->floatPane();
}

bool ReginaMain::closeDockedPane() {
    // Is there anything to close?
    if (! dockedPane)
        return true;

    // Are we allowed to close it?
    if (! dockedPane->queryClose())
        return false;

    // Close it.  Note that queryClose() has already done the
    // deregistration for us.
    PacketPane* closedPane = dockedPane;
    aboutToUndock(dockedPane);

    // At this point dockedPane is already 0.
    delete closedPane;
    return true;
}

bool ReginaMain::closeAllPanes() {
    // Copy the list since the original list will be modified as panes
    // are closed.
    QLinkedList<PacketPane *> panes = allPanes;

    // Try to close each pane in return, returning false if a pane
    // refuses.
    for (QLinkedList<PacketPane *>::iterator it = panes.begin(); 
            it != panes.end() ; it++) {
        if (! (*it)->close())
            return false;
    }

    return true;
}

bool ReginaMain::hasUncommittedChanges() {
    QLinkedList<PacketPane *> panes = allPanes;
    for (QLinkedList<PacketPane *>::iterator it = panes.begin(); 
            it != panes.end() ; it++) {
        if ((*it)->isDirty())
            return true;
    }
    return false;
}

void ReginaMain::commitAllChanges() {
    QLinkedList<PacketPane *> panes = allPanes;
    for (QLinkedList<PacketPane *>::iterator it = panes.begin(); 
            it != panes.end() ; it++) {
        if ((*it)->isDirty())
            (*it)->commit();
    }
}

void ReginaMain::discardAllChanges() {
    QLinkedList<PacketPane *> panes = allPanes;
    for (QLinkedList<PacketPane *>::iterator it = panes.begin(); 
            it != panes.end() ; it++) {
        if ((*it)->isDirty())
            (*it)->refreshForce();
    }
}

void ReginaMain::updateTreeActions() {
    QLinkedList<QAction *>::iterator it;
    for (it = treeGeneralEditActions.begin(); 
            it != treeGeneralEditActions.end(); it++)
        (*it)->setEnabled(true);

    bool enable = ! treeView->selectedItems().isEmpty(); 

    for (it = treePacketViewActions.begin(); 
            it != treePacketViewActions.end(); it++)
        (*it)->setEnabled(enable);

    for (it = treePacketEditActions.begin(); 
            it != treePacketEditActions.end(); it++)
        (*it)->setEnabled(enable);
}

void ReginaMain::setupWidgets() {
    QWidget* main = new QWidget(this);
    QBoxLayout* layout = new QVBoxLayout(main);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    splitter = new QSplitter();
    splitter->setWhatsThis(tr(
        "<qt>Each piece of information stored in a data file "
        "is a <i>packet</i>: this include triangulations, normal surface "
        "lists, text items and so on.<p>"
        "Packets within a data file are arranged in a tree-like structure, "
        "which you should see on the left-hand side of the window.  "
        "If you click on a packet in the tree, it will open up in the "
        "right-hand side of the window where you can edit it or view "
        "detailed information.</qt>"));
    layout->addWidget(splitter, 1);

    if (starterWindow_ && ReginaPrefSet::global().helpIntroOnStartup) {
        // Give the user something helpful to start with.
        advice = new QWidget();
        QBoxLayout* advLayout = new QHBoxLayout(advice);

        int iconSize = QApplication::style()->pixelMetric(
            QStyle::PM_ToolBarIconSize);
        QLabel* icon = new QLabel(advice);
        icon->setPixmap(ReginaSupport::themeIcon("help-hint").pixmap(iconSize));
        advLayout->addWidget(icon);

        advLayout->addSpacing(iconSize / 2);

        QLabel* text = new QLabel(tr(
            "<qt>New to Regina?  <a href=\"#\">Click here.</a></qt>"), advice);
        text->setWordWrap(false);
        text->setTextInteractionFlags(Qt::TextBrowserInteraction);
        advLayout->addWidget(text);
        connect(text, SIGNAL(linkActivated(QString)), this, SLOT(helpIntro()));

        advLayout->addStretch(1);

        layout->addWidget(advice);
    } else
        advice = 0;

    // Set up the packet tree viewer.
    treeView = new PacketTreeView(this, splitter);
    treeView->setSizePolicy(QSizePolicy(
        QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    // Leave the stretch factors at the default of zero.
    connect(treeView, SIGNAL(itemSelectionChanged()), this,
        SLOT(updateTreeActions()));

    // Set up the docking area.
    dockArea = new QWidget(splitter);
    QBoxLayout* dockLayout = new QVBoxLayout(dockArea);
    dockLayout->setContentsMargins(0, 0, 0, 0);
    if (! supportingDock)
        dockArea->hide();

    QSizePolicy qpol(
        QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    qpol.setHorizontalStretch(5);
    qpol.setVerticalStretch(5);
    dockArea->setSizePolicy(qpol);

    // Make sure the docking area gets some space even when there's
    // nothing in it.
    dockLayout->addStrut(100);

    // Put it all inside the main window.
    setCentralWidget(main);
}

void ReginaMain::initPacketTree() {
    if (packetTree)
        delete packetTree;
    packetTree = new regina::NContainer();
    packetTree->setPacketLabel(tr("Data").toAscii().constData());

    // Update the visual representation.
    treeView->fill(packetTree);

    setWindowTitle(tr("Untitled"));
}

void ReginaMain::view(PacketPane* newPane) {
    // Decide whether to dock or float.
    bool shouldDock;

    if (supportingDock) {
        if (dockedPane) {
            shouldDock = ! dockedPane->isDirty();
        } else
            shouldDock = true;
    } else
        shouldDock = false;

    // Display the new pane.
    if (shouldDock) {
        dock(newPane);
        newPane->setFocus();
    } else
        newPane->floatPane();

    // Add it to the list of currently managed panes.
    allPanes.append(newPane);
}

regina::NPacket* ReginaMain::checkPacketSelected() {
    regina::NPacket* p = treeView->selectedPacket();
    if (p)
        return p;
    ReginaSupport::info(this, tr("Please select a packet to work with."));
    return 0;
}

regina::NPacket* ReginaMain::checkSubtreeSelected() {
    regina::NPacket* p = treeView->selectedPacket();
    if (p)
        return p;
    ReginaSupport::info(this, tr("Please select a packet to work with."));
        // Remove all the information about subtrees; it's clear anyway.
    return 0;
}

bool ReginaMain::initData(regina::NPacket* usePacketTree,
        const QString& useLocalFilename,
        const QString& useDisplayName) {
    if (packetTree) {
        delete packetTree;
        setModified(false);
    }

    localFile = useLocalFilename;
    displayName = useDisplayName;
    packetTree = usePacketTree;

    if (packetTree) {
        treeView->fill(packetTree);
        // Expand the first level.
        if (treeView->invisibleRootItem()->child(0)->child(0))
            treeView->scrollToItem(
                treeView->invisibleRootItem()->child(0)->child(0));

        if (! displayName.isEmpty())
            setWindowTitle(displayName);
        else
            setWindowTitle(localFile);

        if (! localFile.isEmpty())
            ReginaPrefSet::addRecentFile(QUrl::fromLocalFile(localFile));

        return true;
    } else {
        initPacketTree();
        return false;
    }
}

bool ReginaMain::saveFile() {
    // Does the user have some work that still needs to be committed?
    while (hasUncommittedChanges()) {
        QMessageBox msgBox(QMessageBox::Information, tr("Information"),
            tr("Some of your packets have changes that are "
            "not yet committed."));
        msgBox.setInformativeText("<qt>Uncommitted changes will "
            "not be saved to file.<p>"
            "Do you wish to commit all of your changes now?</qt>");
        QPushButton* commit = msgBox.addButton(tr("Commit All"),
            QMessageBox::AcceptRole);
        QPushButton* discard = msgBox.addButton(tr("Discard All"),
            QMessageBox::DestructiveRole);
        QPushButton* cancel = msgBox.addButton(tr("Cancel"),
            QMessageBox::RejectRole);
        msgBox.setDefaultButton(commit);
        msgBox.exec();

        if (msgBox.clickedButton() == commit)
            commitAllChanges();
        else if (msgBox.clickedButton() == discard)
            discardAllChanges();
        else
            return false;
    }

    if (regina::writeXMLFile(static_cast<const char*>(
            QFile::encodeName(localFile)), packetTree)) {
        setModified(false);
        return true;
    } else {
        ReginaSupport::warn(this,
            tr("<qt>I could not save the data file <tt>%1</tt>.</qt>").
                arg(Qt::escape(localFile)),
            tr("Please check that you have permissions to write "
                "to this file."));
        return false;
    }
}

void ReginaMain::updatePreferences() {
    if (advice && ! ReginaPrefSet::global().helpIntroOnStartup) {
        delete advice;
        advice = 0;
    }

    if (ReginaPrefSet::global().useDock == supportingDock)
        return;

    supportingDock = ReginaPrefSet::global().useDock;

    if (! supportingDock)
        floatDockedPane();

    dockArea->setVisible(supportingDock);
    if (supportingDock)
        removeToolBarBreak(toolBarPacket);
    else
        insertToolBarBreak(toolBarPacket);

    // Resize to the suggested width, but only use the suggested height
    // if it is larger (i.e., never shrink vertically).
    // Here we use QMainWindow's sizeHint() which measures the widths of
    // toolbars, instead of ReginaMain's sizeHint() which simply uses
    // the size of the last main window.
    QSize s = QMainWindow::sizeHint();
    if (height() > s.height())
        s.setHeight(height());
    resize(s);

    for (QLinkedList<PacketPane *>::iterator it = allPanes.begin();
            it != allPanes.end(); it++)
        (*it)->supportDock(supportingDock);
}

void ReginaMain::newCensus() {
    ReginaSupport::info(this,
        tr("Census creation is only available from the command line."), 
        tr("<qt>See the command-line program <i>tricensus</i>, which "
        "supports a rich set of features and is suitable for "
        "long census calculations.</qt>"));
}

