
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

/*! \file ntrigluings.h
 *  \brief Provides a face gluing editor for triangulations.
 */

#ifndef __NTRIGLUINGS_H
#define __NTRIGLUINGS_H

#include "../packettabui.h"

class KAction;
class KActionCollection;
class KToolBar;
class QTable;

namespace regina {
    class NPacket;
    class NTriangulation;
};

/**
 * A triangulation page for editing face gluings.
 */
class NTriGluingsUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NTriangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QTable* faceTable;

        /**
         * Gluing actions
         */
        KAction* actAddTet;
        KAction* actRemoveTet;
        KAction* actSimplify;
        KActionCollection* triActions;
        QPtrList<KAction> triActionList;
        QPtrList<KAction> enableWhenWritable;

    public:
        /**
         * Constructor and destructor.
         */
        NTriGluingsUI(regina::NTriangulation* packet,
                PacketTabbedUI* useParentUI, bool readWrite);
        ~NTriGluingsUI();

        /**
         * Fill the given toolbar with triangulation actions.
         *
         * This is necessary since the toolbar will not be a part of
         * this page, but this page (as the editor) keeps track of the
         * available actions.
         */
        void fillToolBar(KToolBar* bar);

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
         * Gluing edit actions.
         */
        void addTet();
        void removeSelectedTets();

        /**
         * Triangulation actions.
         */
        void simplify();
        void barycentricSubdivide();
        void idealToFinite();
        void elementaryMove();
        void doubleCover();

        /**
         * Update the states of internal components.
         */
        void updateRemoveState();

        /**
         * Notify us of the fact that an edit has been made.
         */
        void notifyGluingsChanged();
};

#endif
