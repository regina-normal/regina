
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

/*! \file nsurfacecoordinateui.h
 *  \brief Provides a normal surface coordinate viewer.
 */

#ifndef __NSURFACECOODINATEUI_H
#define __NSURFACECOODINATEUI_H

#include "../packettabui.h"

class KAction;
class KActionCollection;
class QListView;

namespace regina {
    class NPacket;
    class NNormalSurfaceList;
};

/**
 * A normal surface page for viewing surface coordinates.
 */
class NSurfaceCoordinateUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NNormalSurfaceList* surfaces;

        /**
         * Internal components
         */
        QWidget* ui;
        QListView* table;

        /**
         * Surface list actions
         */
        KAction* actCrush;
        KActionCollection* surfaceActions;
        QPtrList<KAction> surfaceActionList;
        QPtrList<KAction> enableWhenWritable;

    public:
        /**
         * Constructor and destructor.
         */
        NSurfaceCoordinateUI(regina::NNormalSurfaceList* packet,
                PacketTabbedUI* useParentUI, bool readWrite);
        ~NSurfaceCoordinateUI();

        /**
         * PacketEditorTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        const QPtrList<KAction>& getPacketTypeActions();
        void commit();
        void refresh();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Surface list actions.
         */
        void crush();

        /**
         * Update the states of internal components.
         */
        void updateCrushState();

        /**
         * Notify us of the fact that an edit has been made.
         */
        void notifySurfaceRenamed();
};

#endif
