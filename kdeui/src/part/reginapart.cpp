
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
#include <qtextedit.h>
#include <qvbox.h>
#include <kaction.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kinstance.h>
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
    // TODO: Internal changes.

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
    newPane->show();

    dockChanged();
}

void ReginaPart::isClosing(PacketPane* closingPane) {
    allPanes.removeRef(closingPane);
}

void ReginaPart::hasUndocked(PacketPane* undockedPane) {
    if (dockedPane == undockedPane) {
        unplugActionList("packet_tracking_actions");
        dockedPane = 0;
    }

    dockChanged();
}

bool ReginaPart::openFile() {
    if (packetTree)
        delete packetTree;
    packetTree = regina::readFileMagic(m_file.latin1());

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

void ReginaPart::packetView() {
    regina::NPacket* packet = treeView->selectedPacket();
    if (packet)
        packetView(packet);
    else
        KMessageBox::error(widget(), i18n(
            "No packet is currently selected within the tree."));
}

void ReginaPart::packetView(regina::NPacket* packet) {
    view(new PacketPane(this, packet));
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
    unplugActionList("packet_tracking_actions");
    delete dockedPane;
    dockedPane = 0;

    dockChanged();

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
    packetTree->setPacketLabel(i18n("Container").latin1());

    // Update the visual representation.
    treeView->fill(packetTree);
}

void ReginaPart::dockChanged() {
    if (! dockedPane) {
        actCurrUndock->setEnabled(false);
        actCurrClose->setEnabled(false);

        actCut->setEnabled(false);
        actCopy->setEnabled(false);
        actPaste->setEnabled(false);
    } else {
        actCurrUndock->setEnabled(true);
        actCurrClose->setEnabled(true);

        QTextEdit* edit = dockedPane->getMainUI()->getTextComponent();
        if (edit) {
            actCut->setEnabled(true);
            actCopy->setEnabled(true);
            actPaste->setEnabled(true);

            connect(actCut, SIGNAL(activated()), edit, SLOT(cut()));
            connect(actCopy, SIGNAL(activated()), edit, SLOT(copy()));
            connect(actPaste, SIGNAL(activated()), edit, SLOT(paste()));
        } else {
            actCut->setEnabled(false);
            actCopy->setEnabled(false);
            actPaste->setEnabled(false);

            disconnect(actCut, SIGNAL(activated()));
            disconnect(actCopy, SIGNAL(activated()));
            disconnect(actPaste, SIGNAL(activated()));
        }
    }
}

#include "reginapart.moc"
