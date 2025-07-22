
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

/*! \file linkpoly.h
 *  \brief Provides a tab for viewing link polynomials.
 */

#ifndef __LINKPOLY_H
#define __LINKPOLY_H

#include "packettabui.h"

class QAbstractButton;
class QLabel;
class QRadioButton;

namespace regina {
    class Link;
    class Packet;
};

/**
 * A packet viewer tab for viewing graphs related to a link.
 */
class LinkPolynomialUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * The link itself
         */
        regina::PacketOf<regina::Link>* link;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* jones;
        QLabel* homfly;
        QLabel* bracket;
        QLabel* arrow;
        QAbstractButton* btnJones;
        QAbstractButton* btnHomfly;
        QAbstractButton* btnBracket;
        QAbstractButton* btnArrow;
        QRadioButton* btnAZ;
        QRadioButton* btnLM;

    public:
        /**
         * Constructor and destructor.
         */
        LinkPolynomialUI(regina::PacketOf<regina::Link>* useLink,
            PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        void refresh() override;

    public slots:
        /**
         * Calculate properties.
         */
        void calculateJones();
        void calculateHomfly();
        void calculateBracket();
        void calculateArrow();

        /**
         * Change the display style.
         */
        void homflyTypeChanged(bool checked);

        /**
         * Notify that preferences have changed.
         */
        void updatePreferences();

        /**
         * Support clipboard actions.
         */
        void contextJones(const QPoint& pos);
        void contextHomfly(const QPoint& pos);
        void contextBracket(const QPoint& pos);
        void contextArrow(const QPoint& pos);
        void copyJones();
        void copyHomfly();
        void copyBracket();
        void copyArrow();
        void copyJonesPlain();
        void copyHomflyPlain();
        void copyBracketPlain();
        void copyArrowPlain();

    private:
        /**
         * Update the UI to reflect the current unicode setting.
         */
        void updateLabels();
};

#endif
