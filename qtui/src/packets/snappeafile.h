
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

/*! \file snappeafile.h
 *  \brief Provides access to the SnapPea data file for SnapPea triangulations.
 */

#ifndef __SNAPPEAFILE_H
#define __SNAPPEAFILE_H

#include "../packettabui.h"

class QTextEdit;

namespace regina {
    class SnapPeaTriangulation;
};

/**
 * A triangulation page for viewing normal surface properties.
 */
class SnapPeaFileUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::SnapPeaTriangulation>* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QTextEdit* file;
        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        SnapPeaFileUI(regina::PacketOf<regina::SnapPeaTriangulation>* packet,
            PacketTabbedUI* useParentUI);
        ~SnapPeaFileUI() override;

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        void refresh() override;
        PacketEditIface* getEditIface() override;
};

inline PacketEditIface* SnapPeaFileUI::getEditIface() {
    return editIface;
}

#endif
