
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

/*! \file ntrigluings.h
 *  \brief Provides a face gluing editor for triangulations.
 */

#ifndef __NTRIGLUINGS_H
#define __NTRIGLUINGS_H

#include "../packettabui.h"
#include "reginaprefset.h"

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

        /**
         * Preferences
         */
        ReginaPrefSet::TriEditMode editMode;
        ReginaFilePrefList censusFiles;

    public:
        /**
         * Constructor and destructor.
         */
        NTriGluingsUI(regina::NTriangulation* packet,
                PacketTabbedUI* useParentUI,
                const ReginaPrefSet& initialPrefs, bool readWrite);
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
        void addTet();
        void removeSelectedTets();

        /**
         * Triangulation actions.
         */
        void simplify();
        void barycentricSubdivide();
        void idealToFinite();
        void finiteToIdeal();
        void elementaryMove();
        void doubleCover();
        void splitIntoComponents();
        void connectedSumDecomposition();
        void makeZeroEfficient();
        void censusLookup();

        /**
         * Update the states of internal components.
         */
        void updateRemoveState();

        /**
         * Notify us of the fact that an edit has been made.
         */
        void notifyGluingsChanged();
};

inline void NTriGluingsUI::updatePreferences(const ReginaPrefSet& newPrefs) {
    editMode = newPrefs.triEditMode;
    censusFiles = newPrefs.censusFiles;
}

#endif
