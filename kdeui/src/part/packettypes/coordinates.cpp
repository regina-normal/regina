
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

#include "surfaces/nnormalsurfacelist.h"

#include <klocale.h>

using regina::NNormalSurfaceList;

namespace Coordinates {
    QString name(int coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == NNormalSurfaceList::STANDARD)
                return i18n("Standard normal (tri-quad)");
            if (coordSystem == NNormalSurfaceList::AN_STANDARD)
                return i18n("Standard almost normal (tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::QUAD)
                return i18n("Quad normal");
            if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT)
                return i18n("Edge weight");
            if (coordSystem == NNormalSurfaceList::FACE_ARCS)
                return i18n("Face arc");
            return i18n("Unknown");
        } else {
            if (coordSystem == NNormalSurfaceList::STANDARD)
                return i18n("standard normal (tri-quad)");
            if (coordSystem == NNormalSurfaceList::AN_STANDARD)
                return i18n("standard almost normal (tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::QUAD)
                return i18n("quad normal");
            if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT)
                return i18n("edge weight");
            if (coordSystem == NNormalSurfaceList::FACE_ARCS)
                return i18n("face arc");
            return i18n("unknown");
        }
    }
};

