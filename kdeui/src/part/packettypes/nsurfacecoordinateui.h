
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file nsurfacecoordinateui.h
 *  \brief Provides a normal surface coordinate viewer.
 */

#ifndef __NSURFACECOODINATEUI_H
#define __NSURFACECOODINATEUI_H

#include "packet/npacketlistener.h"

#include "../packettabui.h"

#include <memory>
#include <qtooltip.h>

class CoordinateChooser;
class KAction;
class KActionCollection;
class PacketChooser;
class QBoxLayout;
class QHeader;
class QListView;
class QListViewItem;
class SurfaceHeaderToolTip;

namespace regina {
    class NPacket;
    class NNormalSurfaceList;
    class NSurfaceFilter;
};

/**
 * A normal surface page for viewing surface coordinates.
 */
class NSurfaceCoordinateUI : public QObject, public PacketEditorTab,
        public regina::NPacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NNormalSurfaceList* surfaces;
        regina::NSurfaceFilter* appliedFilter;

        /**
         * Local modifications
         */
        QString* newName;

        /**
         * Internal components
         */
        QWidget* ui;
        QBoxLayout* uiLayout;
        CoordinateChooser* coords;
        PacketChooser* filter;
        std::auto_ptr<QListView> table;
        std::auto_ptr<SurfaceHeaderToolTip> headerTips;
        QString tableWhatsThis;

        /**
         * Surface list actions
         */
        KAction* actCrush;
        KActionCollection* surfaceActions;
        QPtrList<KAction> surfaceActionList;

        /**
         * Internal status
         */
        bool isReadWrite;
        bool currentlyResizing;

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

        /**
         * NPacketListener overrides.
         */
        void packetToBeDestroyed(regina::NPacket* packet);

    public slots:
        /**
         * Refill the table of surfaces whilst preserving local
         * modifications.
         */
        void refreshLocal();

        /**
         * Surface list actions.
         */
        void crush();

        /**
         * Update the states of internal components.
         */
        void updateCrushState();

        /**
         * Provides auto-resizing of columns.
         */
        void columnResized(int section, int oldSize, int newSize);

        /**
         * Notify us that a surface has been renamed.
         */
        void notifySurfaceRenamed();
};

/**
 * A utility class for displaying tooltips for table headers.
 */
class SurfaceHeaderToolTip : public QToolTip {
    private:
        /**
         * Surface information
         */
        regina::NNormalSurfaceList* surfaces;
        int coordSystem;

    public:
        /**
         * Constructor.
         */
        SurfaceHeaderToolTip(regina::NNormalSurfaceList* useSurfaces,
            int useCoordSystem, QHeader* header, QToolTipGroup* group = 0);

    protected:
        /**
         * QToolTip overrides.
         */
        void maybeTip(const QPoint& p);
};

#endif
