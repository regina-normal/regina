
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

/*! \file ntricomposition.h
 *  \brief Provides a combinatorial composition viewer for triangulations.
 */

#ifndef __NTRICOMPOSITION_H
#define __NTRICOMPOSITION_H

#include "packet/packetlistener.h"

#include "../packettabui.h"

#include <memory>

class PacketChooser;
class PacketEditIface;
class QMenu;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;

namespace regina {
    class Matrix2;
    class Packet;
    class NSatRegion;
    class NStandardTriangulation;
    template <int> class Isomorphism;
    template <int> class NPerm;
    template <int> class Triangulation;
    typedef Triangulation<3> NTriangulation;
};

/**
 * A triangulation page for viewing the combinatorial composition.
 */
class NTriCompositionUI : public QObject, public PacketViewerTab,
        public regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * Describes the type of isomorphism relationship that has been
         * discovered, if any.
         */
        enum IsomorphismType
            { NoRelationship, IsIsomorphic, IsSubcomplex, IsSupercomplex };

        /**
         * Packet details
         */
        regina::NTriangulation* tri;
        regina::NTriangulation* comparingTri;
        std::unique_ptr<regina::Isomorphism<3>> isomorphism;
        IsomorphismType isoType;

        /**
         * Internal components
         */
        QWidget* ui;
        PacketChooser* isoTest;
        QLabel* isoResult;
        QPushButton* isoView;
        QTreeWidget* details;
        QTreeWidgetItem* components;
        QTreeWidgetItem* lastComponent;
        PacketEditIface* editIface;

    public:
        /**
         * Constructor.
         */
        NTriCompositionUI(regina::NTriangulation* packet,
                PacketTabbedUI* useParentUI);
        ~NTriCompositionUI();

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        PacketEditIface* getEditIface();
        void refresh();

        /**
         * PacketListener overrides.
         */
        void packetToBeDestroyed(regina::Packet* packet);

    public slots:
        /**
         * Update the isomorphism test panel.
         */
        void updateIsoPanel();

        /**
         * View the isomorphism details.
         */
        void viewIsomorphism();

    private:
        /**
         * Add new items to the list view.
         */
        QTreeWidgetItem* addTopLevelSection(const QString& text);
        QTreeWidgetItem* addComponentSection(const QString& text);

        /**
         * Fill the list view with information.
         */
        void findAugTriSolidTori();
        void findBlockedTriangulations();
        void findL31Pillows();
        void findLayeredChainPairs();
        void findLayeredLensSpaces();
        void findLayeredLoops();
        void findLayeredSolidTori();
        void findPillowSpheres();
        void findPlugTriSolidTori();
        void findSnappedBalls();
        void findSnappedSpheres();
        void findSpiralSolidTori();
        void describeSatRegion(const regina::NSatRegion& region,
            QTreeWidgetItem* parent);

        /**
         * Return string representations of various items.
         */
        static QString edgeString(unsigned long tetIndex, int edge1,
            int edge2);
        static QString edgeString(unsigned long tetIndex,
            const regina::NPerm<4>& roles, int startPreimage, int endPreimage);
        static QString matrixString(const regina::Matrix2& matrix);
};

inline PacketEditIface* NTriCompositionUI::getEditIface() {
    return editIface;
}

#endif
