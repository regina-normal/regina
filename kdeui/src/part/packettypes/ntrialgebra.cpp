
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
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntrialgebra.h"

#include <klocale.h>
#include <qlabel.h>

using regina::NPacket;
using regina::NTriangulation;

NTriAlgebraUI::NTriAlgebraUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI) {
    // TODO
    ui = new QLabel(QString("ALGEBRA"), 0);
}

regina::NPacket* NTriAlgebraUI::getPacket() {
    return tri;
}

QWidget* NTriAlgebraUI::getInterface() {
    return ui;
}

void NTriAlgebraUI::refresh() {
    std::cerr << "Algebra refresh\n";
    // TODO
}

void NTriAlgebraUI::editingElsewhere() {
    // TODO
}

