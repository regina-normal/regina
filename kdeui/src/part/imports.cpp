
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
#include "foreign/importdialog.h"
#include "foreign/snappea.h"
#include "../reginafilter.h"

#include <kfiledialog.h>
#include <klocale.h>

// TODO: Check that there are actually potential parents.

void ReginaPart::importDehydration() {
    unimplemented();
}

void ReginaPart::importPython() {
    unimplemented();
}

void ReginaPart::importRegina() {
    unimplemented();
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
            if (dlg.exec() == QDialog::Accepted) {
                QListViewItem* item = treeView->find(newTree);
                if (item)
                    treeView->ensureItemVisible(item);
                packetView(newTree);

                setModified(true);
            } else
                delete newTree;
        }
    }
}

