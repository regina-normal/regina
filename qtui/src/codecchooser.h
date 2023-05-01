
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file codec.h
 *  \brief Provides a widget for selecting a codec to use for text encodings.
 */

#ifndef __CODECCHOOSER_H
#define __CODECCHOOSER_H

#include "reginaqt.h"
#include <QComboBox>
#include <vector>

/**
 * A widget through which a codec can be selected.
 */
class CodecChooser : public QComboBox {
    public:
        /**
         * Constructor that fills the chooser with all available codecs.
         * The default UTF-8 codec will be selected.
         */
        CodecChooser();

        /**
         * Get the currently selected codec name.  This may return NULL.
         */
        QByteArray selectedCodecName();

        /**
         * Selects the given codec name, if it is available.  If not, the
         * selection will not be changed.
         */
        void setCodecName(const QByteArray& codec);
};

#endif
