
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include "regina-config.h"
#include "packet/container.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

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
#include <QMimeData>
#include <QPushButton>
#include <QScreen>
#include <QSize>
#include <QSplitter>
#include <QTextDocument>
#include <QTreeView>
#include <QTreeWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QWhatsThis>

QMenu* ReginaMain::windowMenu = nullptr;

ReginaMain::ReginaMain(ReginaManager* useManager, bool starterWindow) :
        QMainWindow(),
        manager(useManager),
        packetTree(nullptr),
        starterWindow_(starterWindow),
        fakeRoot_(false),
        packetMenu(nullptr),
        aboutApp(nullptr),
        dirty(false) {
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
    std::list<PacketPane*> panes = allPanes;
    for (auto pane : panes)
        delete pane;

    // Delete the visual tree before the underlying packets so that
    // we don't get a flood of change events.
    delete treeView;

    // Finish cleaning up.
    // The shared pointer should delete the packet tree automatically.
}

void ReginaMain::plugPacketMenu() {
    if (packetMenu) {
        menuBar()->removeAction(packetMenu->menuAction());
        delete packetMenu;
        packetMenu = nullptr;
    }
}

void ReginaMain::unplugPacketMenu() {
    if (packetMenu) {
        menuBar()->removeAction(packetMenu->menuAction());
        delete packetMenu;
        packetMenu = nullptr;
    }
}

void ReginaMain::registerWindow(QAction* windowAction) {
    docMenu->addAction(windowAction);
}

std::shared_ptr<regina::Packet> ReginaMain::selectedPacket() {
    return treeView->selectedPacket();
}

void ReginaMain::setModified(bool modified) {
    dirty = modified;
    actSave->setEnabled(modified);

    if (starterWindow_ && modified) {
        starterWindow_ = false;
        delete advice;
        advice = nullptr;
    }
}

void ReginaMain::packetView(regina::Packet& packet,
        bool makeVisibleInTree, bool selectInTree) {
    view(new PacketPane(this, packet));

    if (makeVisibleInTree || selectInTree) {
        PacketTreeItem* item = treeView->find(packet);
        if (! item) {
            // We cannot find the item in the tree.
            // Perhaps this is because the packet was just created and
            // the tree has not been refreshed yet?
            // Force a refresh now and try again.

            std::shared_ptr<regina::Packet> treeParent = packet.parent();
            // We refresh from treeParent.
            if (treeParent && treeParent->parent()) {
                // treeParent is not the root, which means the parent
                // is also visible in the tree.
                PacketTreeItem* parent = treeView->find(*treeParent);
                if (parent) {
                    parent->refreshSubtree();
                    item = treeView->find(packet);
                }
            } else if (treeParent) {
                // The parent is the root, and the packet we wish to
                // view is a top-level packet in the user-visible tree.
                treeView->refreshFullTree();
                item = treeView->find(packet);
            } else {
                // We are after the root packet.
                // As of Regina 4.95, this no longer appears in the tree.
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

void ReginaMain::ensureVisibleInTree(const regina::Packet& packet) {
    PacketTreeItem* item = treeView->find(packet);
    if (item)
        treeView->scrollToItem(item);
}

void ReginaMain::isClosing(PacketPane* closingPane) {
    allPanes.remove(closingPane);
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
        for (const auto& url : urls)
            fileOpenUrl(url);
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

    int ht = QGuiApplication::primaryScreen()->availableGeometry().height();
    ht *= 2;
    ht /= 3;
    if (ht > ans.height())
        ans.setHeight(ht);

    return ans;
}

void ReginaMain::customEvent(QEvent* evt) {
    if (evt->type() == EVT_PLUG_PACKET_MENU)
        plugPacketMenu();
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
    QString f = url.toLocalFile();
    if (f.isEmpty()) {
        ReginaSupport::warn(this,
            tr("The filename is empty."),
            tr("There may be a miscommunication between Regina and "
            "your operating system.  Please report this to the authors."));
        return;
    }

    std::shared_ptr<regina::Packet> data = regina::open(
        static_cast<const char*>(QFile::encodeName(f)));

    if (! data) {
        ReginaSupport::sorry(this,
            tr("I could not open the selected file."),
            tr("<qt>Please check that the file <tt>%1</tt> "
            "is readable and in Regina format.</qt>").
            arg(f.toHtmlEscaped()));
        return;
    }

    // All good, we have some real data.  Let's go.

    // As of Regina 4.95, the root packet is hidden.
    // If the root packet is not a container, create a new fake root above it.
    if (data->type() != regina::PACKET_CONTAINER) {
        auto newRoot = std::make_shared<regina::Container>();
        newRoot->append(data);
        data = newRoot;

        // We will assume the user knows what (s)he is doing here, and so
        // we remember to "undo" this fake root when we save.
        fakeRoot_ = true;
    }

    // If we already have a document open, make a new window.
    ReginaMain* useWindow = (starterWindow_ ? this : manager->newWindow(false));
    useWindow->initData(data, std::move(f), QString());
    useWindow->starterWindow_ = false;
}

void ReginaMain::fileOpenExample(const QUrl& url, const QString& description) {
    // Same as fileOpenUrl(), but give a pleasant message if the file
    // doesn't seem to exist.
    QString f = url.toLocalFile();
    if (! QFile(f).exists()) {
        ReginaSupport::warn(this,
            tr("I could not locate the example that you requested."),
            tr("<qt>The example \"%1\" may not have been installed properly.  "
            "Please mail the authors for assistance.").
            arg(description.toHtmlEscaped()));
        return;
    }

    std::shared_ptr<regina::Packet> data = regina::open(
        static_cast<const char*>(QFile::encodeName(f)));

    if (! data) {
        ReginaSupport::warn(this,
            tr("I could not open the example that you requested."),
            tr("<qt>The example \"%1\" may not have been installed properly.  "
            "Please mail the authors for assistance.").
            arg(description.toHtmlEscaped()));
        return;
    }

    // All good, we have some real data.  Let's go.
    // If we already have a document open, make a new window.
    ReginaMain* useWindow = (starterWindow_ ? this : manager->newWindow(false));
    useWindow->initData(data, QString(), description);
    useWindow->starterWindow_ = false;
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
        renameWindow(localFile);
    }

    saveFile();
    ReginaPrefSet::addRecentFile(QUrl::fromLocalFile(localFile));
}

void ReginaMain::raiseWindow() {
    raise();
    activateWindow();
    showNormal();
}

void ReginaMain::packetView() {
    if (auto packet = checkPacketSelected())
        packetView(*packet, false);
}

void ReginaMain::packetRename() {
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    bool ok;
    QString suggest = packet->label().c_str();

    QString newLabel = QInputDialog::getText(this,
        tr("Rename Packet"), tr("New label:"), QLineEdit::Normal,
            suggest, &ok).trimmed();
    if ((! ok) || (newLabel == packet->label().c_str()))
        return;

    packet->setLabel(newLabel.toUtf8().constData());
    return;
}

void ReginaMain::packetDelete() {
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("Warning"));
    QPushButton* delBtn;
    if (packet->firstChild()) {
        msgBox.setText(tr("<qt>You are about to delete the packet <i>%1</i> "
            "and all of its children.</qt>").
            arg(QString(packet->humanLabel().c_str()).toHtmlEscaped()));
        delBtn = msgBox.addButton(tr("Delete All"),
            QMessageBox::AcceptRole);
    } else {
        msgBox.setText(tr("<qt>You are about to delete the packet "
            "<i>%1</i>.</qt>").
            arg(QString(packet->humanLabel().c_str()).toHtmlEscaped()));
        delBtn = msgBox.addButton(tr("Delete"),
            QMessageBox::AcceptRole);
    }
    msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
    msgBox.setDefaultButton(delBtn);
    msgBox.setInformativeText("Are you sure?");
    msgBox.exec();
    if (msgBox.clickedButton() != delBtn)
        return;

    // Awkwardly, if a python window still holds a reference to the
    // packet then the packet will not actually be deleted until the
    // python window closes.  *Probably* this is what we want.
    //
    // Otherwise, the packet will simply be deleted as its last
    // shared_ptr drops away.
    packet->makeOrphan();
}

void ReginaMain::treeRefresh() {
    // Refresh the tree itself.
    treeView->refreshFullTree();

    // Refresh all packet panes.
    for (auto pane : allPanes)
        pane->refresh();
}

void ReginaMain::clonePacket() {
    auto packet = checkPacketSelected();
    if (! (packet && packet->parent())) {
        // Note that the root packet is not visible, and cannot be cloned.
        return;
    }

    auto ans = packet->cloneAsSibling(false, false);

    treeView->selectPacket(ans, true);
    packetView(*ans, false);
}

void ReginaMain::cloneSubtree() {
    auto packet = checkSubtreeSelected();
    if (! (packet && packet->parent())) {
        // Note that the root packet is not visible, and cannot be cloned.
        return;
    }

    auto ans = packet->cloneAsSibling(true, false);

    treeView->selectPacket(ans, true);
    packetView(*ans, false);
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
    ReginaPrefSet::openHandbook("index", nullptr, this);
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
    ReginaPrefSet::openHandbook("troubleshooting", nullptr, this);
}

void ReginaMain::helpIntro() {
    QDialog* dlg = new IntroDialog(this);
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}

bool ReginaMain::closeAllPanes() {
    // Copy the list since the original list will be modified as panes
    // are closed.
    std::list<PacketPane*> panes = allPanes;

    // Try to close each pane in return, returning false if a pane refuses.
    for (auto pane : panes)
        if (! pane->close())
            return false;

    return true;
}

void ReginaMain::endEdit() {
    std::list<PacketPane *> panes = allPanes;
    for (auto pane : panes)
        pane->getUI()->endEdit();
}

void ReginaMain::updateTreeActions() {
    for (auto action : treeGeneralEditActions)
        action->setEnabled(true);

    bool enable = ! treeView->selectedItems().isEmpty(); 

    for (auto action : treePacketViewActions)
        action->setEnabled(enable);

    for (auto action : treePacketEditActions)
        action->setEnabled(enable);
}

void ReginaMain::setupWidgets() {
    auto* main = new QWidget(this);
    QBoxLayout* layout = new QVBoxLayout(main);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Set up the packet tree viewer.
    treeView = new PacketTreeView(this);
    treeView->setWhatsThis(tr(
        "<qt>Each piece of information stored in a data file "
        "is a <i>packet</i>: this include triangulations, normal surface "
        "lists, text items and so on.<p>"
        "Packets within a data file are arranged in a tree-like structure, "
        "which you will see in this window.  "
        "You can click on a packet in the tree to "
        "edit it or view detailed information.</qt>"));
    connect(treeView, SIGNAL(itemSelectionChanged()), this,
        SLOT(updateTreeActions()));
    layout->addWidget(treeView, 1);

    if (starterWindow_ && ReginaPrefSet::global().helpIntroOnStartup) {
        // Give the user something helpful to start with.
        advice = new QWidget();
        QBoxLayout* advLayout = new QHBoxLayout(advice);

        int iconSize = QApplication::style()->pixelMetric(
            QStyle::PM_ToolBarIconSize);
        auto* icon = new QLabel(advice);
        icon->setPixmap(ReginaSupport::regIcon("welcome").pixmap(iconSize));
        advLayout->addWidget(icon);

        advLayout->addSpacing(iconSize / 2);

        auto* text = new QLabel(tr(
            "<qt>New to Regina?  <a href=\"#\">Click here.</a></qt>"), advice);
        text->setWordWrap(false);
        text->setTextInteractionFlags(Qt::TextBrowserInteraction);
        advLayout->addWidget(text);
        connect(text, SIGNAL(linkActivated(QString)), this, SLOT(helpIntro()));

        advLayout->addStretch(1);

        layout->addWidget(advice);
    } else
        advice = nullptr;

    // Put it all inside the main window.
    setCentralWidget(main);
}

void ReginaMain::initPacketTree() {
    packetTree.reset(new regina::Container());
    // No label now, since the root packet is hidden.
    // packetTree->setLabel(tr("Data").toUtf8().constData());

    // Update the visual representation.
    treeView->fill(packetTree);

    renameWindow(tr("Untitled"));
}

void ReginaMain::view(PacketPane* newPane) {
    // Display the new pane.
    newPane->floatPane();

    // Add it to the list of currently managed panes.
    allPanes.push_back(newPane);
}

std::shared_ptr<regina::Packet> ReginaMain::checkPacketSelected() {
    // We guarantee not to return the root packet.
    std::shared_ptr<regina::Packet> p = treeView->selectedPacket();
    if (p && p->parent())
        return p;
    ReginaSupport::info(this, tr("Please select a packet to work with."));
    return nullptr;
}

std::shared_ptr<regina::Packet> ReginaMain::checkSubtreeSelected() {
    // We guarantee not to return the root packet.
    std::shared_ptr<regina::Packet> p = treeView->selectedPacket();
    if (p && p->parent())
        return p;
    ReginaSupport::info(this, tr("Please select a packet to work with."));
        // Remove all the information about subtrees; it's clear anyway.
    return nullptr;
}

bool ReginaMain::initData(std::shared_ptr<regina::Packet> usePacketTree,
        QString useLocalFilename, QString useDisplayName) {
    if (packetTree)
        setModified(false);

    localFile = std::move(useLocalFilename);
    displayName = std::move(useDisplayName);
    packetTree = std::move(usePacketTree);

    if (packetTree) {
        treeView->fill(packetTree);
        // Expand the first level.
        for (int i = 0; i < treeView->topLevelItemCount(); ++i)
            treeView->expandItem(treeView->topLevelItem(i));

        if (! displayName.isEmpty())
            renameWindow(displayName);
        else
            renameWindow(localFile);

        if (! localFile.isEmpty())
            ReginaPrefSet::addRecentFile(QUrl::fromLocalFile(localFile));

        return true;
    } else {
        initPacketTree();
        return false;
    }
}

bool ReginaMain::saveFile() {
    endEdit();

    std::shared_ptr<regina::Packet> writeTree = packetTree;
    if (fakeRoot_) {
        // Save the (visible) child, but only if there is exactly one child.
        auto child = packetTree->firstChild();
        if (child && ! child->nextSibling())
            writeTree = child;
    }

    if (writeTree->save(static_cast<const char*>(
            QFile::encodeName(localFile)))) {
        setModified(false);
        return true;
    } else {
        ReginaSupport::warn(this,
            tr("<qt>I could not save the data file <tt>%1</tt>.</qt>").
                arg(localFile.toHtmlEscaped()),
            tr("Please check that you have permissions to write "
                "to this file."));
        return false;
    }
}

void ReginaMain::renameWindow(const QString& newName) {
    setWindowTitle(newName);
    docMenu->setTitle(newName);
}

void ReginaMain::updatePreferences() {
    if (advice && ! ReginaPrefSet::global().helpIntroOnStartup) {
        delete advice;
        advice = nullptr;
    }
}

