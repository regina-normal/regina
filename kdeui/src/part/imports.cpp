
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

#include "packet/npacket.h"

#include "packettreeview.h"
#include "reginapart.h"
#include "foreign/dehydrationhandler.h"
#include "foreign/importdialog.h"
#include "foreign/pythonhandler.h"
#include "foreign/reginahandler.h"
#include "foreign/snappeahandler.h"
#include "../reginafilter.h"

#include <kfiledialog.h>
#include <klocale.h>

void ReginaPart::importDehydration() {
    importFile(DehydrationHandler::instance, 0, i18n(FILTER_ALL),
        i18n("Import dehydrated triangulation list"));
}

void ReginaPart::importPython() {
    importFile(PythonHandler::instance, 0, i18n(FILTER_PYTHON_SCRIPTS),
        i18n("Import Python Script"));
}

void ReginaPart::importRegina() {
    importFile(ReginaHandler(), 0, i18n(FILTER_REGINA),
        i18n("Import Regina Data File"));
}

void ReginaPart::importSnapPea() {
    importFile(SnapPeaHandler::instance, 0, i18n(FILTER_SNAPPEA),
        i18n("Import SnapPea Triangulation"));
}

void ReginaPart::importFile(const PacketImporter& importer,
        PacketFilter* parentFilter, const QString& fileFilter,
        const QString& dialogTitle) {
    if (! checkReadWrite())
        return;

    QString file = KFileDialog::getOpenFileName(QString::null,
        fileFilter, widget(), dialogTitle);
    if (! file.isEmpty()) {
        regina::NPacket* newTree = importer.import(file, widget());
        if (newTree) {
            ImportDialog dlg(widget(), newTree, packetTree,
                treeView->selectedPacket(), parentFilter, dialogTitle);
            if (dlg.validate() && dlg.exec() == QDialog::Accepted)
                packetView(newTree, true);
            else
                delete newTree;
        }
    }
}

