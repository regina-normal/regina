
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nsnappea.h"
#else
    #include "engine/imports/nsnappea.h"
#endif

#include "EngineI.h"
#include "NTetrahedronI.h"
#include "NTriangulationI.h"

Regina::Triangulation::NTetrahedron_ptr Engine_i::newNTetrahedron_() {
    return NTetrahedron_i::newWrapper(new ::NTetrahedron());
}
Regina::Triangulation::NTetrahedron_ptr Engine_i::newNTetrahedron_string(
        const char* desc) {
    return NTetrahedron_i::newWrapper(new ::NTetrahedron(desc));
}
Regina::Triangulation::NTriangulation_ptr Engine_i::newNTriangulation_() {
    return NTriangulation_i::newWrapper(new ::NTriangulation());
}
Regina::Triangulation::NTriangulation_ptr
        Engine_i::newNTriangulation_NTriangulation(
        Regina::Triangulation::NTriangulation_ptr cloneMe) {
    return NTriangulation_i::newWrapper(new ::NTriangulation(
        *GET_ENGINE_OBJECT(NTriangulation, cloneMe)));
}

Regina::Triangulation::NTriangulation_ptr Engine_i::readSnapPea(
        const char* fileName) {
    return NTriangulation_i::newWrapper(::readSnapPea(fileName));
}

