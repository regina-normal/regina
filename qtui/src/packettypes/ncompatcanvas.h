
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/*! \file ncompatcanvas.h
 *  \brief Provides a canvas for displaying a surface/hypersurface
 *  compatibility matrix.
 */

#ifndef __NCOMPATCANVAS_H
#define __NCOMPATCANVAS_H

#include <QGraphicsScene>

namespace regina {
    class NNormalHypersurfaceList;
    class NNormalSurfaceList;
};

/**
 * A canvas for displaying a compatibility matrix for a list of
 * normal surfaces or hypersurfaces.
 */
class NCompatCanvas : public QGraphicsScene {
    Q_OBJECT

    private:
        /**
         * Matrix size and state
         */
        unsigned nSurfaces;
        bool filled;

        /**
         * Internal components
         */
        unsigned cellSize;
        unsigned gridX;
        unsigned gridY;
        unsigned gridSize;

    public:
        /**
         * Constructor and destructor.
         */
        NCompatCanvas(unsigned useNumSurfaces);
        ~NCompatCanvas();

        /**
         * Fill the canvas with data.
         *
         * These routines will do nothing if the canvas has already been
         * filled.
         *
         * \pre The given list is non-empty and contains only embedded
         * surfaces/hypersurfaces.
         */
        void fillLocal(const regina::NNormalSurfaceList& surfaces);
        void fillLocal(const regina::NNormalHypersurfaceList& surfaces);
        void fillGlobal(const regina::NNormalSurfaceList& surfaces);
};

#endif
