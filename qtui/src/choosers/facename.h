
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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

/*! \file facename.h
 *  \brief Provides dimension-specific English names for faces in triangulations.
 */

#ifndef __FACENAME_H
#define __FACENAME_H

/**
 * Provides a dimension-specific English name for a subdim-face in a
 * triangulation.
 */
template <int subdim>
struct FaceName {
    /**
     * The name of a subdim-face, with the first letter capitalised.
     *
     * This function is specialised for dimensions <= 4, and (for the
     * time being) is left undefined for dimensions > 5.
     */
    static const char* upper();
};

template <> inline const char* FaceName<0>::upper() { return "Vertex"; }
template <> inline const char* FaceName<1>::upper() { return "Edge"; }
template <> inline const char* FaceName<2>::upper() { return "Triangle"; }
template <> inline const char* FaceName<3>::upper() { return "Tetrahedron"; }
template <> inline const char* FaceName<4>::upper() { return "Pentachoron"; }

#endif
