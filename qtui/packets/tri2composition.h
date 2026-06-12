
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file tri2composition.h
 *  \brief Provides a combinatorial composition viewer for
 *  2-manifold triangulations.
 */

#ifndef __TRI2COMPOSITION_H
#define __TRI2COMPOSITION_H

#include "packet/packet.h"
#include "triangulation/dim2.h"

#include "../packettabui.h"

#include <memory>

class ElidedLabel;
class TriSigChooser;
class PacketChooser;
class QMenu;
class QPushButton;

/**
 * A triangulation page for viewing the combinatorial composition.
 */
class Tri2CompositionUI : public QObject, public PacketViewerTab,
        public regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * Describes the type of isomorphism relationship that has been
         * discovered, if any.
         */
        enum class IsomorphismType
            { NoRelationship, IsIsomorphic, IsSubcomplex, IsSupercomplex };

        /**
         * Packet details
         */
        regina::PacketOf<regina::Triangulation<2>>* tri_;
        regina::PacketOf<regina::Triangulation<2>>* compare_;
        std::string sig_;
        std::optional<regina::Isomorphism<2>> isomorphism;
        IsomorphismType isoType;

        /**
         * Internal components
         */
        QWidget* ui;
        PacketChooser* isoTest;
        QLabel* isoResult;
        QPushButton* isoView;
        ElidedLabel* isoSig;
        TriSigChooser* isoSigVariant;
        QLabel* copyHint;

    public:
        /**
         * Constructor.
         */
        Tri2CompositionUI(regina::PacketOf<regina::Triangulation<2>>* tri,
            PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        void refresh() override;

        /**
         * PacketListener overrides.
         */
        void packetBeingDestroyed(regina::PacketShell packet) override;

        /**
         * Context menu actions that are not slots because they take
         * more arguments than the corresponding signal provides.
         */
        void contextIsoSig(const QPoint& pos, QWidget* fromWidget);

    public slots:
        /**
         * Update the isomorphism signature.
         */
        void updateIsoSig();

        /**
         * Update the isomorphism test panel.
         */
        void updateIsoPanel();

        /**
         * View the isomorphism details.
         */
        void viewIsomorphism();

        /**
         * Support clipboard actions.
         */
        void copyIsoSig();
};

#endif
