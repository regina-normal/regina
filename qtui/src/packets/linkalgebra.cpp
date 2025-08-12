
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

// Regina core includes:
#include "algebra/grouppresentation.h"
#include "link/link.h"

// UI includes:
#include "columnlayout.h"
#include "groupwidget.h"
#include "linkalgebra.h"
#include "reginaprefset.h"

#include <QLabel>
#include <QLayout>
#include <QStackedWidget>

using regina::Link;
using regina::Packet;

#define GROUP_SIMP_THRESHOLD 50

LinkAlgebraUI::LinkAlgebraUI(regina::PacketOf<regina::Link>* packet,
        PacketTabbedUI* parentUI) :
        PacketTabbedViewerTab(parentUI,
            ReginaPrefSet::global().tabLinkAlgebra),
        link(packet), usesKnotLabels(link->countComponents() == 1) {
    addTab(new LinkGroupUI(packet, false, this),
        usesKnotLabels ? tr("Knot &Group") : tr("Link &Group"));
    addTab(new LinkGroupUI(packet, true, this), tr("&Extended Group"));
}

void LinkAlgebraUI::refresh() {
    if (link->countComponents() == 1) {
        if (! usesKnotLabels) {
            renameTab(0, "Knot &Group");
            usesKnotLabels = true;
        }
    } else {
        if (usesKnotLabels) {
            renameTab(0, "Link &Group");
            usesKnotLabels = false;
        }
    }

    // Refresh the individual sub-tabs.
    PacketTabbedViewerTab::refresh();
}

LinkGroupUI::LinkGroupUI(regina::PacketOf<regina::Link>* packet,
        bool extended, PacketTabbedViewerTab* parentUI) :
        PacketViewerTab(parentUI), link(packet), extended(extended) {
    pages = new QStackedWidget();

    // Note: currently links do not allow simplified groups to be passed back,
    // so we do not connect the signal group.simplified() to any slot.

    // ---------- Single common group ----------

    auto* ui = new QWidget();
    auto* groupLayout = new QVBoxLayout(ui);

    group = new GroupWidget();
    group->setWhatsThis(tr("A full set of generators and relations "
        "for this group."));
    groupLayout->addWidget(group, 1);

    pages->addWidget(ui);

    // ---------- Two groups (above vs below) ----------

    ui = new QWidget();
    auto* master = new ColumnLayout(ui);

    groupLayout = new QVBoxLayout();
    groupAbove = new GroupWidget();
    groupAbove->setWhatsThis(tr("The generators and relations for the group "
        "obtained when viewing the link diagram from above the surface "
        "in which it embeds."));
    groupLayout->addWidget(groupAbove, 1);
    master->addLayout(groupLayout, tr("From above"));

    groupLayout = new QVBoxLayout();
    groupBelow = new GroupWidget();
    groupBelow->setWhatsThis(tr("The generators and relations for the group "
        "obtained when viewing the link diagram from beneath the surface "
        "in which it embeds."));
    groupLayout->addWidget(groupBelow, 1);
    master->addLayout(groupLayout, tr("From below"));

    pages->addWidget(ui);

    // ---------- Finishing up ----------

    pages->setCurrentIndex(0);
    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

regina::Packet* LinkGroupUI::getPacket() {
    return link;
}

QWidget* LinkGroupUI::getInterface() {
    return pages;
}

void LinkGroupUI::refresh() {
    bool simplify = (link->size() < GROUP_SIMP_THRESHOLD);
    if (link->isClassical()) {
        if (extended)
            group->setGroup(link->extendedGroup(simplify));
        else
            group->setGroup(link->group(simplify));
        pages->setCurrentIndex(0);
    } else {
        auto groups = (extended ? link->extendedGroups(simplify) :
            link->groups(simplify));
        groupAbove->setGroup(groups.first);
        groupBelow->setGroup(groups.second);
        pages->setCurrentIndex(1);
    }
}

void LinkGroupUI::updatePreferences() {
    // If we've changed the unicode setting, then we may need some redrawing.
    refresh();
}

