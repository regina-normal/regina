
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

#include "choosers/trisigchooser.h"

TriSigChooser::TriSigChooser(QWidget* parent) : QComboBox(parent) {
    addItem("2nd-gen");
    addItem("2nd-gen oriented");
    addItem("1st-gen (Regina ≤ 7.x)");

    select(ReginaPrefSet::global().triSigVariant);

    connect(this, &TriSigChooser::activated, this, [this]() {
        ReginaPrefSet::global().triSigVariant = selected();
    });
}

ReginaPrefSet::TriSigVariant TriSigChooser::selected() {
    switch (currentIndex()) {
        case 1: return ReginaPrefSet::TriSigVariant::Gen2Oriented;
        case 2: return ReginaPrefSet::TriSigVariant::Gen1;
        default: return ReginaPrefSet::TriSigVariant::Gen2;
    }
}

void TriSigChooser::select(ReginaPrefSet::TriSigVariant variant) {
    switch (variant) {
        case ReginaPrefSet::TriSigVariant::Gen2Oriented:
            setCurrentIndex(1);
            break;
        case ReginaPrefSet::TriSigVariant::Gen1:
            setCurrentIndex(2);
            break;
        default: // Gen2
            setCurrentIndex(0);
            break;
    }
}

