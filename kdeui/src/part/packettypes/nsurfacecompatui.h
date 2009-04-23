
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

/*! \file nsurfacecompatui.h
 *  \brief Provides a viewer for pairwise compatibility of normal surfaces.
 */

#ifndef __NSURFACECOMPATUI_H
#define __NSURFACECOMPATUI_H

#include "packet/npacketlistener.h"

#include "../packettabui.h"

class NCompatCanvas;
class QPushButton;
class QCanvasView;
class QComboBox;
class QWidgetStack;
class ReginaPrefSet;

namespace regina {
    class NPacket;
    class NNormalSurfaceList;
};

/**
 * A normal surface page for viewing surface coordinates.
 */
class NSurfaceCompatibilityUI : public QObject, public PacketViewerTab,
        public regina::NPacketListener {
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
        regina::NNormalSurfaceList* surfaces;

        /**
         * Compatibility matrices
         *
         * These are null if there are too many surfaces, or real objects
         * if we aim to display the matrices.  Note that, even if these
         * are real objects, we do not \e fill the canvases with data
         * points until the user actually tries to display them.
         */
        NCompatCanvas* matrixLocal;
        NCompatCanvas* matrixGlobal;
        QCanvasView* layerLocal;
        QCanvasView* layerGlobal;

        /**
         * Internal components
         */
        QWidget* ui;
        QWidgetStack* stack;
        QWidget* layerNone;
        QLabel* msgNone;
        QComboBox* chooseMatrix;
        QPushButton* btnCalculate;

        /**
         * Properties
         */
        unsigned autoCalcThreshold;
        bool requestedCalculation;

    public:
        /**
         * Constructor and destructor.
         */
        NSurfaceCompatibilityUI(regina::NNormalSurfaceList* packet,
            PacketTabbedUI* useParentUI, const ReginaPrefSet& prefs);
        ~NSurfaceCompatibilityUI();

        /**
         * Update properties.
         */
        void setAutoCalcThreshold(unsigned newThreshold);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();

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
