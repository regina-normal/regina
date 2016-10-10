
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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

/*! \file coordinates.h
 *  \brief Assists working in different normal surface/hypersurface
 *  coordinate systems.
 */

#ifndef __COORDINATES_H
#define __COORDINATES_H

#include "hypersurface/hypercoords.h"
#include "maths/integer.h"
#include "surfaces/normalcoords.h"

#include <QString>
#include <QObject>

namespace regina {
    class NormalSurface;
    class NormalHypersurface;
    template <int> class Triangulation;
    typedef Triangulation<3> NTriangulation;
    typedef Triangulation<4> Dim4Triangulation;
}

namespace Coordinates {
    /**
     * Return a human-readable name for the given coordinate system.
     */
    const char* name(regina::NormalCoords coordSystem,
        bool capitalise = true);

    /**
     * Return a human-readable name for the given coordinate system.
     */
    const char* name(regina::HyperCoords coordSystem,
        bool capitalise = true);

    /**
     * Does the given coordinate system generate almost normal
     * surfaces when used with NormalSurfaces::enumerate()?
     *
     * Only coordinate systems that are used for enumerating surfaces
     * (not just viewing surfaces) are relevant here.
     */
    bool generatesAlmostNormal(regina::NormalCoords coordSystem);

    /**
     * Return the number of coordinate columns in the given coordinate
     * system.
     */
    size_t numColumns(regina::NormalCoords coordSystem,
        regina::NTriangulation* tri);

    /**
     * Return the number of coordinate columns in the given coordinate
     * system.
     */
    size_t numColumns(regina::HyperCoords coordSystem,
        regina::Dim4Triangulation* tri);

    /**
     * Return a column header for the given coordinate of the given
     * coordinate system.
     *
     * The associated triangulation may be passed so that more precise
     * information can be returned, though this routine will behave
     * well without it.
     */
    QString columnName(regina::NormalCoords coordSystem,
        size_t whichCoord, regina::NTriangulation* tri = 0);

    /**
     * Return a column header for the given coordinate of the given
     * coordinate system.
     *
     * The associated triangulation may be passed so that more precise
     * information can be returned, though this routine will behave
     * well without it.
     */
    QString columnName(regina::HyperCoords coordSystem,
        size_t whichCoord, regina::Dim4Triangulation* tri = 0);

    /**
     * Return a column description for the given coordinate of the given
     * coordinate system.
     *
     * The associated triangulation may be passed so that more precise
     * information can be returned, though this routine will behave
     * well without it.
     */
    QString columnDesc(regina::NormalCoords coordSystem, size_t whichCoord,
        const QObject *context, regina::NTriangulation* tri = 0);

    /**
     * Return a column description for the given coordinate of the given
     * coordinate system.
     *
     * The associated triangulation may be passed so that more precise
     * information can be returned, though this routine will behave
     * well without it.
     */
    QString columnDesc(regina::HyperCoords coordSystem, size_t whichCoord,
        const QObject *context, regina::Dim4Triangulation* tri = 0);

    /**
     * Return a particular coordinate of a normal surface in the given
     * coordinate system.
     */
    regina::LargeInteger getCoordinate(regina::NormalCoords coordSystem,
        const regina::NormalSurface& surface, size_t whichCoord);

    /**
     * Return a particular coordinate of a normal hypersurface in the given
     * coordinate system.
     */
    regina::LargeInteger getCoordinate(regina::HyperCoords coordSystem,
        const regina::NormalHypersurface& surface, size_t whichCoord);
};

#endif
