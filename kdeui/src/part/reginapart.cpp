
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
#include "file/nxmlfile.h"
#include "packet/ncontainer.h"

// UI includes:
#include "../reginaabout.h"
#include "../reginafilter.h"
#include "packettreeview.h"
#include "packetui.h"
#include "reginapart.h"

#include <qcolor.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qvbox.h>
#include <kaction.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kinstance.h>
#include <klineeditdlg.h>
#include <kmainwindow.h>
#include <kmessagebox.h>
#include <kparts/genericfactory.h>

typedef KParts::GenericFactory<ReginaPart> ReginaPartFactory;
K_EXPORT_COMPONENT_FACTORY(libreginapart, ReginaPartFactory);

ReginaPart::ReginaPart(QWidget *parentWidget, const char *widgetName,
        QObject *parent, const char *name, const QStringList& /*args*/) :
        KParts::ReadWritePart(parent, name), packetTree(0),
        dockedPane(0), autoDock(true) {
    // Get the instance.
    setInstance(factoryInstance());

    // Set up our widgets and actions.
    setXMLFile("reginapart.rc");
    setupWidgets(parentWidget, widgetName);
    setupActions();

    // Initialise the packet tree.
    initPacketTree();

    // Other tidying up.
    dockChanged();
    setReadWrite(true);
    setModified(false);
    updateTreeEditActions();
    updateTreePacketActions();
}

ReginaPart::~ReginaPart() {
    // Make an emergency closure of any remaining packet panes.
    QPtrList<PacketPane> panes = allPanes;
    for (PacketPane* p = panes.first(); p; p = panes.next())
        delete p;

    // Finish cleaning up.
    if (packetTree)
        delete packetTree;
}

void ReginaPart::setReadWrite(bool rw) {
    // Update each packet pane.
    QPtrList<PacketPane> panes = allPanes;
    for (PacketPane* p = panes.first(); p; p = panes.next())
        p->setReadWrite(rw);

    // TODO: Update r/w status for console actions.
    updateTreeEditActions();

    // Finally call the parent implementation.
    ReadWritePart::setReadWrite(rw);
}

void ReginaPart::setModified(bool modified) {
    // Internal changes.
    actSave->setEnabled(modified);

    // Finally call the parent implementation.
    ReadWritePart::setModified(modified);
}

bool ReginaPart::closeURL() {
    if (! closeAllPanes())
        return false;
    return ReadWritePart::closeURL();
}

KAboutData* ReginaPart::createAboutData() {
    return new ReginaAbout<ReginaPart>("reginapart");
}

KInstance* ReginaPart::factoryInstance() {
    return ReginaPartFactory::instance();
}

void ReginaPart::view(PacketPane* newPane) {
    // Decide whether to dock or float.
    bool shouldDock;

    if (autoDock) {
        if (dockedPane) {
            shouldDock = ! dockedPane->isDirty();
        } else
            shouldDock = true;
    } else
        shouldDock = false;

    // Display the new pane.
    if (shouldDock)
        dock(newPane);
    else
        newPane->floatPane();

    // Add it to the list of currently managed panes.
    allPanes.append(newPane);
}

void ReginaPart::dock(PacketPane* newPane) {
    // Get rid of the currently docked pane by whatever means necessary.
    if (! closeDockedPane())
        dockedPane->floatPane();

    newPane->reparent(dockArea, QPoint(0, 0));
    dockedPane = newPane;
    plugActionList("packet_tracking_actions", newPane->getTrackingActions());
    plugActionList("packet_tracking_separator", separatorList);
    newPane->show();

    if (newPane->hasTextComponent()) {
        newPane->registerEditOperation(actCut, PacketPane::editCut);
        newPane->registerEditOperation(actCopy, PacketPane::editCopy);
        newPane->registerEditOperation(actPaste, PacketPane::editPaste);
        newPane->registerEditOperation(actUndo, PacketPane::editUndo);
        newPane->registerEditOperation(actRedo, PacketPane::editRedo);
    }

    dockChanged();
}

void ReginaPart::isClosing(PacketPane* closingPane) {
    allPanes.removeRef(closingPane);
}

void ReginaPart::hasUndocked(PacketPane* undockedPane) {
    if (undockedPane->hasTextComponent()) {
        undockedPane->deregisterEditOperation(actCut, PacketPane::editCut);
        undockedPane->deregisterEditOperation(actCopy, PacketPane::editCopy);
        undockedPane->deregisterEditOperation(actPaste, PacketPane::editPaste);
        undockedPane->deregisterEditOperation(actUndo, PacketPane::editUndo);
        undockedPane->deregisterEditOperation(actRedo, PacketPane::editRedo);
    }

    if (dockedPane == undockedPane) {
        unplugActionList("packet_tracking_actions");
        unplugActionList("packet_tracking_separator");
        dockedPane = 0;
    }

    dockChanged();
}

bool ReginaPart::openFile() {
    if (packetTree)
        delete packetTree;
    packetTree = regina::readFileMagic(m_file.ascii());

    if (packetTree) {
        treeView->fill(packetTree);
        // Expand the first level.
        if (treeView->firstChild()->firstChild())
            treeView->ensureItemVisible(treeView->firstChild()->firstChild());
        return true;
    } else {
        KMessageBox::error(widget(), i18n(
            "Topology data file %1 could not be opened.  Perhaps "
            "it is not a Regina data file?").arg(m_file));
        initPacketTree();
        return false;
    }
}

bool ReginaPart::saveFile() {
    // If we aren't read-write, return immediately.
    if (! isReadWrite())
        return false;

    if (regina::writeXMLFile(m_file, packetTree))
        return true;
    else {
        KMessageBox::error(widget(), i18n(
            "Topology data file %1 could not be saved.").arg(m_file));
        return false;
    }
}

void ReginaPart::packetView(regina::NPacket* packet) {
    view(new PacketPane(this, packet));
}

void ReginaPart::packetView() {
    regina::NPacket* packet = checkPacketSelected();
    if (packet)
        packetView(packet);
}

void ReginaPart::packetRename() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    bool ok;
    QString suggest = packet->getPacketLabel().c_str();
    while (true) {
        QString newLabel = KLineEditDlg::getText(i18n("Rename Packet"),
            i18n("New label:"), suggest, &ok).stripWhiteSpace();
        if ((! ok) || (newLabel == packet->getPacketLabel().c_str()))
            return;

        // Has this label already been used?
        if (packetTree->findPacketLabel(newLabel.ascii())) {
            KMessageBox::error(widget(), i18n(
                "Another packet is already using this label."));
            suggest = packetTree->makeUniqueLabel(newLabel.ascii()).c_str();
        } else {
            // It's a unique label; we can rename it!
            packet->setPacketLabel(newLabel.ascii());
            setModified(true);
            return;
        }
    }
}

void ReginaPart::packetDelete() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (KMessageBox::warningContinueCancel(widget(), i18n(
            "You are about to delete the packet %1 and all its children.  "
            "Are you sure?").arg(packet->getPacketLabel().c_str()),
            i18n("Delete Packet")) == KMessageBox::Cancel)
        return;

    delete packet;
    setModified(true);
}

void ReginaPart::subtreeRefresh() {
    if (! checkSubtreeSelected())
        return;

    // Refresh the tree itself.
    PacketTreeItem* item = dynamic_cast<PacketTreeItem*>(
        treeView->selectedItem());
    item->refreshSubtree();

    // Refresh any relevant packet panes.
    regina::NPacket* subtree = item->getPacket();
    for (PacketPane* pane = allPanes.first(); pane; pane = allPanes.next())
        if (subtree->isGrandparentOf(pane->getPacket()))
            pane->refresh();
}

void ReginaPart::clonePacket() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    packet->clone(false, false);
    setModified(true);
}

void ReginaPart::cloneSubtree() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkSubtreeSelected();
    if (! packet)
        return;

    packet->clone(true, false);
    setModified(true);
}

void ReginaPart::floatDockedPane() {
    // Delegate the entire procedure to PacketPane::floatPane().
    // Processing will return to this class when PacketPane calls
    // ReginaPart::hasUndocked().
    if (dockedPane)
        dockedPane->floatPane();
}

bool ReginaPart::closeDockedPane() {
    // Is there anything to close?
    if (! dockedPane)
        return true;

    // Are we allowed to close it?
    if (! dockedPane->queryClose())
        return false;

    // Close it.  Note that queryClose() has already done the
    // deregistration for us.
    PacketPane* closedPane = dockedPane;
    hasUndocked(dockedPane);

    // At this point dockedPane is already 0.
    delete closedPane;
    return true;
}

bool ReginaPart::closeAllPanes() {
    // Copy the list since the original list will be modified as panes
    // are closed.
    QPtrList<PacketPane> panes = allPanes;

    // Try to close each pane in return, returning false if a pane
    // refuses.
    for (PacketPane* p = panes.first(); p; p = panes.next())
        if (! p->close())
            return false;

    return true;
}

void ReginaPart::displayIcon(bool shouldDisplay) {
    if (shouldDisplay)
        reginaIcon->show();
    else
        reginaIcon->hide();
}

void ReginaPart::setAutoDock(bool value) {
    autoDock = value;
}

void ReginaPart::updateTreePacketActions() {
    KAction* act;

    bool enable = (treeView->selectedItem() != 0);
    for (act = treePacketViewActions.first(); act;
            act = treePacketViewActions.next())
        act->setEnabled(enable);

    enable = enable && isReadWrite();
    for (act = treePacketEditActions.first(); act;
            act = treePacketEditActions.next())
        act->setEnabled(enable);
}

void ReginaPart::updateTreeEditActions() {
    KAction* act;

    bool enable = isReadWrite();
    for (act = treeGeneralEditActions.first(); act;
            act = treeGeneralEditActions.next())
        act->setEnabled(enable);

    enable = enable && (treeView->selectedItem() != 0);
    for (act = treePacketEditActions.first(); act;
            act = treePacketEditActions.next())
        act->setEnabled(enable);
}

void ReginaPart::fileSaveAs() {
    QString file = KFileDialog::getSaveFileName(QString::null,
        i18n(FILTER_REGINA), widget(), i18n("Save Data File"));
    if (! file.isEmpty())
        saveAs(file);
}

void ReginaPart::unimplemented() {
    KMessageBox::sorry(widget(), i18n("This feature is not yet implemented."));
}

void ReginaPart::setupWidgets(QWidget* parentWidget, const char* widgetName) {
    QSplitter* splitter = new QSplitter(parentWidget, widgetName);

    // Set up the packet tree viewer.
    QVBox* treeBox = new QVBox(splitter);
    treeBox->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
        QSizePolicy::MinimumExpanding, 1, 1));

    treeView = new PacketTreeView(this, treeBox);
    treeBox->setStretchFactor(treeView, 1);
    connect(treeView, SIGNAL(selectionChanged()), this,
        SLOT(updateTreePacketActions()));

    reginaIcon = new QLabel(treeBox);
    reginaIcon->setPixmap(UserIcon("reginatrans", instance()));
    reginaIcon->setPaletteBackgroundPixmap(UserIcon("stars", instance()));
    reginaIcon->setAlignment(AlignCenter);
    reginaIcon->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    reginaIcon->hide();

    // Set up the docking area.
    dockArea = new QVBox(splitter);
    dockArea->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
        QSizePolicy::MinimumExpanding, 5, 5));

    // Make sure the docking area gets some space even when there's
    // nothing in it.
    dynamic_cast<QBoxLayout*>(dockArea->layout())->addStrut(100);

    // Make the splitter our main widget.
    setWidget(splitter);
}

void ReginaPart::initPacketTree() {
    if (packetTree)
        delete packetTree;
    packetTree = new regina::NContainer();
    packetTree->setPacketLabel(i18n("Container").ascii());

    // Update the visual representation.
    treeView->fill(packetTree);
}

void ReginaPart::dockChanged() {
    if (! dockedPane) {
        actCurrUndock->setEnabled(false);
        actCurrClose->setEnabled(false);
    } else {
        actCurrUndock->setEnabled(true);
        actCurrClose->setEnabled(true);
    }
}

bool ReginaPart::checkReadWrite() {
    if (isReadWrite())
        return true;

    KMessageBox::error(widget(), i18n(
        "This topology data file is currently in read-only mode."));
    return false;
}

regina::NPacket* ReginaPart::checkPacketSelected() {
    regina::NPacket* p = treeView->selectedPacket();
    if (p)
        return p;

    KMessageBox::error(widget(), i18n(
        "No packet is currently selected within the tree."));
    return 0;
}

regina::NPacket* ReginaPart::checkSubtreeSelected() {
    regina::NPacket* p = treeView->selectedPacket();
    if (p)
        return p;

    KMessageBox::error(widget(), i18n(
        "No subtree is currently selected.  To work with a packet subtree, "
        "select the packet at the base of the subtree."));
    return 0;
}

#include "reginapart.moc"
