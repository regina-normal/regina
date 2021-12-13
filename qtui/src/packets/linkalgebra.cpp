
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

// Regina core includes:
#include "algebra/grouppresentation.h"
#include "link/link.h"

// UI includes:
#include "groupwidget.h"
#include "linkalgebra.h"
#include "reginaprefset.h"

#include <QLabel>
#include <QLayout>

using regina::Link;
using regina::Packet;

#define GROUP_SIMP_THRESHOLD 50

LinkAlgebraUI::LinkAlgebraUI(regina::PacketOf<regina::Link>* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), link(packet) {
    ui = new QWidget();

    auto* fundLayout = new QVBoxLayout(ui);

    fgTitle = new QLabel();
    fgTitle->setAlignment(Qt::AlignCenter);
    fundLayout->addWidget(fgTitle);

    fgGroup = new GroupWidget(true, true);
    fgGroup->setWhatsThis(tr("A full set of generators and relations "
        "for the link group."));
    connect(fgGroup, SIGNAL(simplified()), this, SLOT(fundGroupSimplified()));
    fundLayout->addWidget(fgGroup, 1);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

regina::Packet* LinkAlgebraUI::getPacket() {
    return link;
}

QWidget* LinkAlgebraUI::getInterface() {
    return ui;
}

void LinkAlgebraUI::refresh() {
    if (link->countComponents() == 1)
        fgTitle->setText(tr("<qt><b>Knot Group</b></qt>"));
    else
        fgTitle->setText(tr("<qt><b>Link Group</b></qt>"));

    fgGroup->setGroup(link->group(link->size() < GROUP_SIMP_THRESHOLD));
}

void LinkAlgebraUI::updatePreferences() {
    // If we've changed the unicode setting, then we may need some redrawing.
    refresh();
}

