
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file trisigchooser.h
 *  \brief Provides a widget for selecting a variant of isomorphism
 *  signature for triangulations.
 */

#ifndef __TRISIGCHOOSER_H
#define __TRISIGCHOOSER_H

#include <QComboBox>
#include "reginaprefset.h"

/**
 * A widget through which the user can select a variant of isomorphism
 * signature for triangulations.
 *
 * The connection with ReginaPrefSet::triSigVariant will be managed
 * automatically by this class.
 */
class TriSigChooser : public QComboBox {
    public:
        /**
         * Constructor that fills the chooser with all available variants
         * of isomorphism signature.
         *
         * The initial selection will be determined by
         * `ReginaPrefSet::triSigVariant`.
         */
        TriSigChooser(QWidget* parent);

        /**
         * Returns the currently selected variant.
         */
        ReginaPrefSet::TriSigVariant selected();

        /**
         * Changes the selection to the given variant.
         *
         * The activated() signal will _not_ be emitted.
         */
        void select(ReginaPrefSet::TriSigVariant variant);
};

#endif
