
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

/*! \file nsurfacefilterprop.h
 *  \brief Provides an interface for working with property-based surface
 *  filters.
 */

#ifndef __NSURFACEFILTERPROP_H
#define __NSURFACEFILTERPROP_H

#include "utilities/nbooleans.h"

#include "../packetui.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;

namespace regina {
    class NSurfaceFilterProperties;
    class NPacket;
};

/**
 * A packet interface for working with property-based surface filters.
 */
class NSurfaceFilterPropUI : public QObject, public PacketUI {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NSurfaceFilterProperties* filter;

        /**
         * Internal components
         */
        QWidget* ui;
        QCheckBox* useOrient;
        QCheckBox* useCompact;
        QCheckBox* useBdry;
        QCheckBox* useEuler;
        QComboBox* optOrient;
        QComboBox* optCompact;
        QComboBox* optBdry;
        QLineEdit* eulerList;
        QLabel* eulerExpln;

        /**
         * Current state
         */
        bool allowReadWrite;
        bool inNotify;

    public:
        /**
         * Constructor and destructor.
         */
        NSurfaceFilterPropUI(regina::NSurfaceFilterProperties* packet,
                PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void refresh();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Notification of any change in the dialog.
         * Returns false if the change was not actionable (e.g., if the
         * list of Euler characteristics was invalid).
         */
        bool notifyOptionsChanged();

        /**
         * Update the enabled/disable state of particular components.
         */
        void enableDisableOrient();
        void enableDisableCompact();
        void enableDisableBdry();

    private:
        /**
         * Translate between UI elements and underlying filter options.
         */
        regina::NBoolSet getBoolSet(QCheckBox* use, QComboBox* opt);
        void setBoolSet(QCheckBox* use, QComboBox* opt, regina::NBoolSet set);
        QString filterECList();
};

#endif
