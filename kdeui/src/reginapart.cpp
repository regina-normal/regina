
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
#include "reginaabout.h"
#include "reginapart.h"

#include <qlabel.h>
#include <kaction.h>
#include <kfiledialog.h>
#include <kinstance.h>
#include <kmessagebox.h>
#include <kparts/genericfactory.h>
#include <kstdaction.h>

typedef KParts::GenericFactory<ReginaPart> ReginaPartFactory;
K_EXPORT_COMPONENT_FACTORY(libreginapart, ReginaPartFactory);

ReginaPart::ReginaPart(QWidget *parentWidget, const char *widgetName,
        QObject *parent, const char *name, const QStringList& /*args*/) :
        KParts::ReadWritePart(parent, name) {
    // Get the instance.
    setInstance( ReginaPartFactory::instance() );

    // Initialise the packet tree.
    packetTree = new regina::NContainer();
    packetTree->setPacketLabel(i18n("Container"));

    // Set up the internal widgets.
    setWidget(new QLabel(i18n("Welcome to Regina!"), parentWidget,
        widgetName));

    // Set up our actions.
    actSave = KStdAction::save(this, SLOT(save()), actionCollection());
    KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());

    // Set our XML-UI resource file.
    setXMLFile("reginapart.rc");

    // Other tidying up.
    setReadWrite(true);
    setModified(false);
}

ReginaPart::~ReginaPart() {
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

KAboutData *ReginaPart::createAboutData() {
    return new ReginaAbout<ReginaPart>("reginapart");
}

bool ReginaPart::openFile() {
    if (packetTree)
        delete packetTree;

    packetTree = regina::readFileMagic(m_file);

    if (packetTree)
        return true;
    else {
        KMessageBox::error(widget(), QString(i18n(
            "Topology data file %1 could not be opened.")).arg(m_file));
        packetTree = new regina::NContainer();
        packetTree->setPacketLabel(i18n("Container"));
        return false;
    }

    // TODO: Update the visual representation.
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

void ReginaPart::fileSaveAs() {
    // TODO: Fix
    QString file_name = KFileDialog::getSaveFileName();
    if (! file_name.isEmpty())
        saveAs(file_name);
}

#include "reginapart.moc"
