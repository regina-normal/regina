
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

/*! \file dim4trigluings.h
 *  \brief Provides a facet gluing editor for 4-manifold triangulations.
 */

#ifndef __DIM4TRIGLUINGS_H
#define __DIM4TRIGLUINGS_H

#include "../packettabui.h"
#include "reginaprefset.h"

class KAction;
class KActionCollection;
class KToolBar;
class QTable;

namespace regina {
    class NPacket;
    class Dim4Triangulation;
};

/**
 * A 4-manifold triangulation page for editing facet gluings.
 */
class Dim4TriGluingsUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Dim4Triangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QTable* facetTable;

        /**
         * Gluing actions
         */
        KAction* actAddPent;
        KAction* actRemovePent;
        KAction* actSimplify;
        KActionCollection* triActions;
        QPtrList<KAction> triActionList;
        QPtrList<KAction> enableWhenWritable;

        /**
         * Preferences
         */
        ReginaPrefSet::TriEditMode editMode;

    public:
        /**
         * Constructor and destructor.
         */
        Dim4TriGluingsUI(regina::Dim4Triangulation* packet,
                PacketTabbedUI* useParentUI,
                const ReginaPrefSet& initialPrefs, bool readWrite);
        ~Dim4TriGluingsUI();

        /**
         * Fill the given toolbar with triangulation actions.
         *
         * This is necessary since the toolbar will not be a part of
         * this page, but this page (as the editor) keeps track of the
         * available actions.
         */
        void fillToolBar(KToolBar* bar);

        /**
         * Update the preferences.
         */
        void updatePreferences(const ReginaPrefSet& newPrefs);

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
        void addPent();
        void removeSelectedPents();

        /**
         * Triangulation actions.
         */
        void simplify();

        /**
         * Update the states of internal components.
         */
        void updateRemoveState();

        /**
         * Notify us of the fact that an edit has been made.
         */
        void notifyGluingsChanged();
};

inline void Dim4TriGluingsUI::updatePreferences(const ReginaPrefSet& newPrefs) {
    editMode = newPrefs.triEditMode;
}

#endif
