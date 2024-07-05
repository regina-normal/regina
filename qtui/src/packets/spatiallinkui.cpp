
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "regina-config.h"

// UI includes:
#include "bigwidget.h"
#include "spatiallinkui.h"

#include <QAction>
#include <QBoxLayout>
#include <QToolBar>

using regina::Packet;
using regina::SpatialLink;

SpatialLinkUI::SpatialLinkUI(regina::PacketOf<SpatialLink>* packet,
        PacketPane* enclosingPane) : PacketUI(enclosingPane), link_(packet) {
    ui = new BigWidget(1, 2);
    auto* layout = new QVBoxLayout(ui);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // --- Action Toolbar ---

    auto* actionBar = new QToolBar(ui);
    actionBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    layout->addWidget(actionBar);

    // --- 3-D Link ---

    QWidget* rendering = new QWidget();
    layout->addWidget(rendering, 1);

    // --- Actions ---

    actThinner = new QAction(this);
    actThinner->setText(tr("T&hinner"));
    // TODO: actThinner->setIcon(ReginaSupport::regIcon("insert"));
    actThinner->setToolTip(tr("Make the link appear thinner"));
    actThinner->setWhatsThis(tr("Reduces the radius that is used to render "
        "the link."));
    connect(actThinner, SIGNAL(triggered()), this, SLOT(makeThinner()));
    actionList.push_back(actThinner);
    actionBar->addAction(actThinner);

    actThicker = new QAction(this);
    actThicker->setText(tr("Thic&ker"));
    // TODO: actThicker->setIcon(ReginaSupport::regIcon("insert"));
    actThicker->setToolTip(tr("Make the link appear thicker"));
    actThicker->setWhatsThis(tr("Increases the radius that is used to render "
        "the link."));
    connect(actThicker, SIGNAL(triggered()), this, SLOT(makeThicker()));
    actionList.push_back(actThicker);
    actionBar->addAction(actThicker);

    actRefine = new QAction(this);
    actRefine->setText(tr("&Refine"));
    // TODO: actRefine->setIcon(ReginaSupport::regIcon("insert"));
    actRefine->setToolTip(tr("Makes the link appear smoother"));
    actRefine->setWhatsThis(tr("Increases the number of line segments that are "
        "used to render the link."));
    connect(actRefine, SIGNAL(triggered()), this, SLOT(refine()));
    actionList.push_back(actRefine);
    actionBar->addAction(actRefine);

    // --- Finalising ---

    refresh();
}

Packet* SpatialLinkUI::getPacket() {
    return link_;
}

QWidget* SpatialLinkUI::getInterface() {
    return ui;
}

const std::vector<QAction*>& SpatialLinkUI::getPacketTypeActions() {
    return actionList;
}

QString SpatialLinkUI::getPacketMenuText() const {
    return tr("S&patial Link");
}

void SpatialLinkUI::refresh() {
    // TODO
}

void SpatialLinkUI::makeThinner() {
    link_->setRadius(link_->radius() / 1.2);
}

void SpatialLinkUI::makeThicker() {
    link_->setRadius(link_->radius() * 1.2);
}

void SpatialLinkUI::refine() {
    if (packet_->size() * 2 > MAX_NODES) {
        ReginaSupport::sorry(this,
            tr("I am not brave enough to create a spatial link with "
                "more than %1 nodes.").arg(MAX_NODES));
        return;
    }

    link_->refine();
}

