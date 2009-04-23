
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file ncompatcanvas.h
 *  \brief Provides a canvas for displaying a surface compatibility matrix.
 */

#ifndef __NCOMPATCANVAS_H
#define __NCOMPATCANVAS_H

#include <qcanvas.h>

namespace regina {
    class NNormalSurfaceList;
};

/**
 * A canvas for displaying a compatibility matrix for a list of
 * normal surfaces.
 */
class NCompatCanvas : public QCanvas {
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
         * \pre The given list is non-empty and contains only embedded surfaces.
         */
        void fillLocal(const regina::NNormalSurfaceList& surfaces);
        void fillGlobal(const regina::NNormalSurfaceList& surfaces);
};

#endif
