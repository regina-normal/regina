
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

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
class KLineEdit;

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
        KLineEdit* eulerList;
        QLabel* eulerExpln1;
        QLabel* eulerExpln2;

        /**
         * Current state
         */
        bool allowReadWrite;

    public:
        /**
         * Constructor and destructor.
         */
        NSurfaceFilterPropUI(regina::NSurfaceFilterProperties* packet,
                PacketPane* newEnclosingPane, bool readWrite);

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void commit();
        void refresh();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Notification of various actions.
         */
        void notifyFilterChanged();

        /**
         * Update the enabled/disable state of particular components.
         */
        void enableDisableOrient();
        void enableDisableCompact();
        void enableDisableBdry();
        void enableDisableEuler();

    private:
        /**
         * Translate between UI elements and underlying boolean options.
         */
        regina::NBoolSet getBoolSet(QCheckBox* use, QComboBox* opt);
        void setBoolSet(QCheckBox* use, QComboBox* opt, regina::NBoolSet set);

        /**
         * Refresh the Euler characteristic UI elements only.
         */
        void refreshECList();
};

#endif
