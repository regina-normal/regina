
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
#include "snappea/snappeatriangulation.h"

// UI includes:
#include "snappeagluings.h"

#include <memory>
#include <QHeaderView>
#include <QTableView>

using regina::Packet;
using regina::SnapPeaTriangulation;

SnapPeaGluingsUI::SnapPeaGluingsUI(regina::SnapPeaTriangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), tri(packet) {
    // Set up the table of face gluings.
    model = new GluingsModel3(packet, false /* read-only */);
    faceTable = new QTableView();
    faceTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
    faceTable->setModel(model);
    faceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    faceTable->setWhatsThis(tr("<qt>A table specifying which tetrahedron "
        "faces are identified with which others.<p>"
        "Tetrahedra are numbered upwards from 0, and the four vertices of "
        "each tetrahedron are numbered 0, 1, 2 and 3.  Each row of the table "
        "represents a single tetrahedron, and shows the identifications "
        "for each of its four faces.<p>"
        "As an example, if we are looking at the table cell for face 012 of "
        "tetrahedron 7, a gluing of <i>5 (031)</i> shows that "
        "that this face is identified with face 031 of tetrahedron 5, in "
        "such a way that vertices 0, 1 and 2 of tetrahedron "
        "7 are mapped to vertices 0, 3 and 1 respectively of tetrahedron 5.<p>"
        "To change these identifications, simply type your own gluings into "
        "the table.</qt>"));

    faceTable->verticalHeader()->hide();

    //faceTable->setColumnStretchable(0, true);
    //faceTable->setColumnStretchable(1, true);
    //faceTable->setColumnStretchable(2, true);
    //faceTable->setColumnStretchable(3, true);
    //faceTable->setColumnStretchable(4, true);

    ui = faceTable;
}

SnapPeaGluingsUI::~SnapPeaGluingsUI() {
    // Make sure the actions, including separators, are all deleted.

    delete model;
}

regina::Packet* SnapPeaGluingsUI::getPacket() {
    return tri;
}

QWidget* SnapPeaGluingsUI::getInterface() {
    return ui;
}

void SnapPeaGluingsUI::refresh() {
    model->rebuild();
}

