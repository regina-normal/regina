
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

// UI includes:
#include "ntrialgebra.h"
#include "ntricomposition.h"
#include "ntriangulationui.h"
#include "ntrigluings.h"
#include "ntriskeleton.h"
#include "ntrisurfaces.h"

#include <klocale.h>

using regina::NPacket;
using regina::NTriangulation;

NTriangulationUI::NTriangulationUI(regina::NTriangulation* packet,
        PacketPane* newEnclosingPane, bool readWrite) :
        PacketTabbedUI(newEnclosingPane) {
    addTab(new NTriGluingsUI(packet, this, readWrite), i18n("&Gluings"));
    addTab(new NTriSkeletonUI(packet, this), i18n("&Skeleton"));
    addTab(new NTriAlgebraUI(packet, this), i18n("&Algebra"));
    addTab(new NTriCompositionUI(packet, this), i18n("&Composition"));
    addTab(new NTriSurfacesUI(packet, this), i18n("Sur&faces"));
}

