
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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

/*! \file linkmovedialog.h
 *  \brief Provides a dialog through which the user can perform
 *  Reidemeister moves on a knot or link.
 */

#ifndef __LINKMOVEDIALOG_H
#define __LINKMOVEDIALOG_H

#include "link/link.h"
#include "packet/packetlistener.h"

#include <QDialog>
#include <vector>

class QAbstractButton;
class QButtonGroup;
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QRadioButton;

/**
 * Identifies a potential type 1 move to add a new crossing.
 */
struct R1UpArg {
    regina::StrandRef strand;
    int side;
    int sign;

    R1UpArg() = default;
    R1UpArg(const regina::StrandRef& s, int useSide, int useSign);
    QString display() const;
};

/**
 * Identifies a potential type 1 move to remove a crossing.
 */
struct R1DownArg {
    regina::Crossing* crossing;

    R1DownArg() = default;
    R1DownArg(regina::Crossing* c);
    QString display() const;
};

/**
 * A dialog used to select and perform Reidemeister moves on a knot or link.
 */
class LinkMoveDialog : public QDialog, public regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        QLabel* name;
        QLabel* overview;

        QComboBox* box1up;
        QComboBox* box1down;
        QComboBox* box2upOver;
        QComboBox* box2upUnder;
        QComboBox* box2down;
        QComboBox* box3;
        QRadioButton* use1up;
        QRadioButton* use1down;
        QRadioButton* use2up;
        QRadioButton* use2down;
        QRadioButton* use3;
        QButtonGroup* moveTypes;
        QDialogButtonBox* buttons;

        /**
         * Allowable moves:
         */
        std::vector<R1UpArg> options1up;
        std::vector<R1DownArg> options1down;

        /**
         * Packet tree structure:
         */
        regina::Link* link;

    public:
        /**
         * Constructor and destructor.
         */
        LinkMoveDialog(QWidget* parent, regina::Link* useLink);
        ~LinkMoveDialog();

        /**
         * Update the overview information if the link changes.
         */
        void packetWasRenamed(regina::Packet*);
        void packetWasChanged(regina::Packet*);
        void packetToBeDestroyed(regina::Packet*);

    protected slots:
        /**
         * A button at the bottom of the dialog has been clicked.
         */
        virtual void clicked(QAbstractButton*);

        /**
         * One of the move types has been selected.
         */
        void updateApply();

    private:
        void fill();
        void updateStates(QComboBox* chooser, QRadioButton* button);
};

inline R1UpArg::R1UpArg(const regina::StrandRef& s, int useSide, int useSign) :
        strand(s), side(useSide), sign(useSign) {
}

inline R1DownArg::R1DownArg(regina::Crossing* c) : crossing(c) {
}

#endif
