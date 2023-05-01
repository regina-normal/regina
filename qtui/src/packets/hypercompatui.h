
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

/*! \file hypercompatui.h
 *  \brief Provides a viewer for pairwise compatibility of normal hypersurfaces.
 */

#ifndef __HYPERCOMPATUI_H
#define __HYPERCOMPATUI_H

#include "../packettabui.h"

class MessageLayer;
class CompatCanvas;
class QPushButton;
class QGraphicsView;
class QComboBox;
class QStackedWidget;

namespace regina {
    class Packet;
    class NormalHypersurfaces;
};

/**
 * A normal surface page for viewing surface coordinates.
 */
class HyperCompatibilityUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Constants for the various "computer says no" messages that can be
         * displayed.
         */
        enum MessageIndex { TOO_LARGE, NON_EMBEDDED, EMPTY_LIST };

        /**
         * Packet details
         */
        regina::PacketOf<regina::NormalHypersurfaces>* surfaces;

        /**
         * Compatibility matrices
         *
         * These are null if there are too many surfaces, or real objects
         * if we aim to display the matrices.  Note that, even if these
         * are real objects, we do not _fill_ the canvases with data
         * points until the user actually tries to display them.
         */
        CompatCanvas* matrixLocal;
        QGraphicsView* layerLocal;

        /**
         * Internal components
         */
        QWidget* ui;
        QStackedWidget* stack;
        MessageLayer* layerNone;
        QComboBox* chooseMatrix;
        QPushButton* btnCalculate;

        /**
         * Properties
         */
        bool requestedCalculation;

    public:
        /**
         * Constructor and destructor.
         */
        HyperCompatibilityUI(
            regina::PacketOf<regina::NormalHypersurfaces>* packet,
            PacketTabbedUI* useParentUI);
        ~HyperCompatibilityUI() override;

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        void refresh() override;

    public slots:
        /**
         * Notify that preferences have changed.
         */
        void updatePreferences();

    private:
        /**
         * Change the display to show the given message.
         */
        void setMessage(MessageIndex msg);

    private slots:
        /**
         * Compute or change matrices.
         */
        void changeLayer(int index);
        void calculate();
};

#endif
