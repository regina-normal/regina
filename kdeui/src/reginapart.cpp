
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
#include "packettreeview.h"
#include "reginaabout.h"
#include "reginafilter.h"
#include "reginapart.h"

#include <qcolor.h>
#include <qlabel.h>
#include <qsplitter.h>
#include <qvbox.h>
#include <kaction.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kinstance.h>
#include <kmessagebox.h>
#include <kparts/genericfactory.h>
#include <kstdaction.h>

typedef KParts::GenericFactory<ReginaPart> ReginaPartFactory;
K_EXPORT_COMPONENT_FACTORY(libreginapart, ReginaPartFactory);

ReginaPart::ReginaPart(QWidget *parentWidget, const char *widgetName,
        QObject *parent, const char *name, const QStringList& /*args*/) :
        KParts::ReadWritePart(parent, name), packetTree(0) {
    // Get the instance.
    setInstance(factoryInstance());

    // Set up our widgets and actions.
    setXMLFile("reginapart.rc");
    setupWidgets(parentWidget, widgetName);
    setupActions();

    // Initialise the packet tree.
    initPacketTree();

    // Other tidying up.
    setReadWrite(true);
    setModified(false);
}

ReginaPart::~ReginaPart() {
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

KAboutData* ReginaPart::createAboutData() {
    return new ReginaAbout<ReginaPart>("reginapart");
}

KInstance* ReginaPart::factoryInstance() {
    return ReginaPartFactory::instance();
}

bool ReginaPart::openFile() {
    if (packetTree)
        delete packetTree;
    packetTree = regina::readFileMagic(m_file);

    if (packetTree) {
        treeView->fill(packetTree);
        // Expand the first level.
        if (treeView->firstChild()->firstChild())
            treeView->ensureItemVisible(treeView->firstChild()->firstChild());
        return true;
    } else {
        KMessageBox::error(widget(), QString(i18n(
            "Topology data file %1 could not be opened.  Perhaps"
            "it is not a Regina data file?")).arg(m_file));
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
        KMessageBox::error(widget(), QString(i18n(
            "Topology data file %1 could not be saved.")).arg(m_file));
        return false;
    }
}

void ReginaPart::displayIcon(bool shouldDisplay) {
    if (shouldDisplay)
        reginaIcon->show();
    else
        reginaIcon->hide();
}

void ReginaPart::fileSaveAs() {
    QString file = KFileDialog::getSaveFileName(QString::null,
        i18n(FILTER_REGINA), widget(), i18n("Save Data File"));
    if (! file.isEmpty())
        saveAs(file);
}

void ReginaPart::unimplemented() {
    KMessageBox::sorry(widget(), QString(i18n(
        "This feature is not yet implemented.")));
}

void ReginaPart::setupWidgets(QWidget* parentWidget, const char* widgetName) {
    QSplitter* splitter = new QSplitter(parentWidget, widgetName);

    // Set up the packet tree viewer.
    QVBox* treeBox = new QVBox(splitter);

    treeView = new PacketTreeView(treeBox);
    treeBox->setStretchFactor(treeView, 1);

    reginaIcon = new QLabel(treeBox);
    reginaIcon->setPixmap(UserIcon("reginatrans", instance()));
    reginaIcon->setPaletteBackgroundPixmap(UserIcon("stars", instance()));
    reginaIcon->setAlignment(AlignCenter);
    reginaIcon->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    reginaIcon->hide();

    // Set up the docking area.
    QVBox* dockArea = new QVBox(splitter);

    // TODO: Adjust the divider position.
    setWidget(splitter);
}

void ReginaPart::setupActions() {
    actSave = KStdAction::save(this, SLOT(save()), actionCollection());
    KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());

    new KAction(i18n("&Angle Structure List"), "packet_angles", ALT+Key_A,
        this, SLOT(unimplemented()), actionCollection(), "packet_angles");
    new KAction(i18n("&Container"), "packet_container", ALT+Key_C,
        this, SLOT(unimplemented()), actionCollection(), "packet_container");
    new KAction(i18n("&Filter"), "packet_filter", ALT+Key_F,
        this, SLOT(unimplemented()), actionCollection(), "packet_filter");
    new KAction(i18n("&Normal Surface List"), "packet_surfaces", ALT+Key_N,
        this, SLOT(unimplemented()), actionCollection(), "packet_surfaces");
    new KAction(i18n("&Script"), "packet_script", ALT+Key_S,
        this, SLOT(unimplemented()), actionCollection(), "packet_script");
    new KAction(i18n("Te&xt"), "packet_text", ALT+Key_X,
        this, SLOT(unimplemented()), actionCollection(), "packet_text");
    new KAction(i18n("&Triangulation"), "packet_triangulation", ALT+Key_T,
        this, SLOT(unimplemented()), actionCollection(),
        "packet_triangulation");
}

void ReginaPart::initPacketTree() {
    if (packetTree)
        delete packetTree;
    packetTree = new regina::NContainer();
    packetTree->setPacketLabel(i18n("Container"));

    // Update the visual representation.
    treeView->fill(packetTree);
}

#include "reginapart.moc"
