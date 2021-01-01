
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
#include "link/link.h"

// UI includes:
#include "linkcodes.h"
#include "linkcrossings.h"
#include "linkgraph.h"
#include "linkpoly.h"
#include "linkui.h"
#include "packeteditiface.h"
#include "reginamain.h"

#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::Packet;
using regina::Link;

#define NEG_COLOUR "#b8860b"
#define POS_COLOUR "#0071bc"
#define LEFT_COLOUR "#9a0000"
#define RIGHT_COLOUR "#008000"

LinkUI::LinkUI(regina::Link* packet, PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane, ReginaPrefSet::global().tabLink) {
    LinkHeaderUI* header = new LinkHeaderUI(packet, this);
    crossings = new LinkCrossingsUI(packet, this,
        newEnclosingPane->isReadWrite());

    crossings->fillToolBar(header->getToolBar());

    addHeader(header);
    addTab(crossings, QObject::tr("&Crossings"));
    addTab(new LinkPolynomialUI(packet, this), QObject::tr("&Polynomials"));
    addTab(new LinkCodesUI(packet, this), QObject::tr("C&odes"));
    addTab(new LinkGraphUI(packet, this), QObject::tr("&Graphs"));

    editIface = new PacketEditTabbedUI(this);
}

LinkUI::~LinkUI() {
    delete editIface;
}

const QLinkedList<QAction*>& LinkUI::getPacketTypeActions() {
    return crossings->getPacketTypeActions();
}

QString LinkUI::getPacketMenuText() const {
    return QObject::tr("&Knot / Link");
}

LinkHeaderUI::LinkHeaderUI(regina::Link* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        link(packet) {
    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);
    uiLayout->setContentsMargins(0, 0, 0, 0);

    bar = new QToolBar(ui);
    bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    uiLayout->addWidget(bar);

    header = new QLabel();
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    header->setWhatsThis(QObject::tr("Displays basic information about "
        "the knot or link."));
    uiLayout->addWidget(header);
}

regina::Packet* LinkHeaderUI::getPacket() {
    return link;
}

QWidget* LinkHeaderUI::getInterface() {
    return ui;
}

void LinkHeaderUI::refresh() {
    header->setText(summaryInfo(link));
}

QString LinkHeaderUI::summaryInfo(regina::Link* link) {
    if (link->isEmpty())
        return QObject::tr("Empty");

    QString signs;
    if (link->size() > 1) {
        size_t plus = 0, minus = 0;
        for (size_t i = 0; i < link->size(); ++i)
            if (link->crossing(i)->sign() > 0)
                ++plus;
            else
                ++minus;

        if (minus == 0)
            signs = QObject::tr("<span style='color:" POS_COLOUR ";'>"
                "all +ve</span>");
        else if (plus == 0)
            signs = QObject::trUtf8("<span style='color:" NEG_COLOUR ";'>"
                "all −ve</span>");
        else
            signs = QObject::trUtf8("<span style='color:" POS_COLOUR ";'>"
                "%1 +ve</span>, <span style='color:" NEG_COLOUR ";'>"
                "%2 −ve</span>").arg(plus).arg(minus);
    } else if (link->size() == 1) {
        if (link->crossing(0)->sign() > 0)
            signs = QObject::tr("<span style='color:" POS_COLOUR ";'>"
                "+ve</span>");
        else
            signs = QObject::trUtf8("<span style='color:" NEG_COLOUR ";'>"
                "−ve</span>");
    }

    if (link->countComponents() == 1) {
        // Knot:
        if (link->size() == 0) {
            return QObject::tr("Unknot with no crossings");
        } else if (link->size() == 1) {
            // Must be alternating.
            return QObject::tr("Alternating knot with 1 crossing (%1)")
                .arg(signs);
        } else if (link->isAlternating()) {
            return QObject::tr("Alternating knot with %1 crossings (%2)")
                .arg(link->size()).arg(signs);
        } else {
            return QObject::tr("Non-alternating knot with %1 crossings (%2)")
                .arg(link->size()).arg(signs);
        }
    } else {
        // Multiple component link:
        if (link->size() == 0) {
            return QObject::tr("Unlink with %1 components, no crossings")
                .arg(link->countComponents());
        } else if (link->size() == 1) {
            // Must be alternating.
            return QObject::tr(
                "Alternating link with %1 components, 1 crossing (%2)")
                .arg(link->countComponents()).arg(signs);
        } else if (link->isAlternating()) {
            return QObject::tr(
                "Alternating link with %1 components, %2 crossings (%3)")
                .arg(link->countComponents()).arg(link->size()).arg(signs);
        } else {
            return QObject::tr(
                "Non-alternating link with %1 components, %2 crossings (%3)")
                .arg(link->countComponents()).arg(link->size()).arg(signs);
        }
    }
}

