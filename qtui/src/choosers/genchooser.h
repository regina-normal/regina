
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

/*! \file genchooser.h
 *  \brief Provides a widget for selecting a generation of isomorphism
 *  signature or knot/link signature.
 */

#ifndef __GENCHOOSER_H
#define __GENCHOOSER_H

#include <QComboBox>

/**
 * A widget through which the user can select a generation of isomorphism
 * signature or knot/link signature.
 *
 * The connection with ReginaPrefSet::sigGeneration will be managed
 * automatically by this class.
 */
class GenChooser : public QComboBox {
    Q_OBJECT

    public:
        /**
         * Constructor that fills the chooser with all available generations
         * of signature.
         *
         * The initial selection will be determined by
         * `ReginaPrefSet::sigGeneration`.
         */
        GenChooser(QWidget* parent);

        /**
         * Returns the currently selected generation.
         */
        int selected();

        /**
         * Changes the selection to the given generation.
         *
         * If the given generation is out of range, then the last entry
         * in the chooser (i.e., the newest generation) will be selected.
         *
         * The activated() signal will _not_ be emitted.
         */
        void select(int generation);
};

inline int GenChooser::selected() {
    int curr = currentIndex();
    return (curr < 0 ? count() /* newest generation */ : curr + 1);
}

#endif
