
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include "regina-config.h" // For EXCLUDE_SNAPPEA

// Regina core includes:
#include "snappea/nsnappeatriangulation.h"

// UI includes:
#include "ntrialgebra.h"
#include "ntricomposition.h"
#include "nsnappeaui.h"
#include "nsnappeagluings.h"
#include "ntriskeleton.h"
#include "packeteditiface.h"
#include "reginamain.h"

#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::NPacket;
using regina::NSnapPeaTriangulation;

NSnapPeaUI::NSnapPeaUI(regina::NSnapPeaTriangulation* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane,
            ReginaPrefSet::global().tabSnapPeaTri) {
    NSnapPeaHeaderUI* header = new NSnapPeaHeaderUI(packet, this);
    gluings = new NSnapPeaGluingsUI(packet, this,
        newEnclosingPane->isReadWrite());
    skeleton = new NTriSkeletonUI(packet, this);
    algebra = new NTriAlgebraUI(packet, this);

    gluings->fillToolBar(header->getToolBar());

    addHeader(header);
    addTab(gluings, QObject::tr("&Gluings"));
    addTab(skeleton, QObject::tr("&Skeleton"));
    addTab(algebra, QObject::tr("&Algebra"));
    addTab(new NTriCompositionUI(packet, this), QObject::tr("&Composition"));

    editIface = new PacketEditTabbedUI(this);
}

NSnapPeaUI::~NSnapPeaUI() {
    delete editIface;
}

const QLinkedList<QAction*>& NSnapPeaUI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString NSnapPeaUI::getPacketMenuText() const {
    return QObject::tr("&SnapPea Triangulation");
}

NSnapPeaHeaderUI::NSnapPeaHeaderUI(regina::NSnapPeaTriangulation* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);
    uiLayout->setContentsMargins(0, 0, 0, 0);

    bar = new QToolBar(ui);
    bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    uiLayout->addWidget(bar);

    header = new QLabel();
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    header->setWhatsThis(QObject::tr("Displays a few basic properties of the "
        "triangulation, such as orientability and solution type."));
    uiLayout->addWidget(header);
}

regina::NPacket* NSnapPeaHeaderUI::getPacket() {
    return tri;
}

QWidget* NSnapPeaHeaderUI::getInterface() {
    return ui;
}

void NSnapPeaHeaderUI::refresh() {
    header->setText(summaryInfo(tri));
}

void NSnapPeaHeaderUI::editingElsewhere() {
    header->setText(QObject::tr("Editing..."));
}

QString NSnapPeaHeaderUI::summaryInfo(regina::NSnapPeaTriangulation* tri) {
    if (tri->isNull())
        return QObject::tr("Null triangulation (no SnapPea data)");
    if (tri->getNumberOfTetrahedra() == 0)
        return QObject::tr("Empty");

    if (! tri->isValid())
        return QObject::tr("INVALID TRIANGULATION!");

    QString msg;

    if (tri->isClosed())
        msg += QObject::tr("Closed, ");
    else
        msg += QObject::tr("Ideal, ");

    if (tri->isOrientable())
        msg += QObject::tr("orientable, ");
    else
        msg += QObject::tr("non-orientable, ");

    switch (tri->solutionType()) {
        case NSnapPeaTriangulation::not_attempted:
            msg += QObject::tr("solution not attempted");
            break;
        case NSnapPeaTriangulation::geometric_solution:
            msg += QObject::tr("tetrahedra positively oriented");
            break;
        case NSnapPeaTriangulation::nongeometric_solution:
            msg += QObject::tr("contains negatively oriented tetrahedra");
            break;
        case NSnapPeaTriangulation::flat_solution:
            msg += QObject::tr("all tetrahedra flat");
            break;
        case NSnapPeaTriangulation::degenerate_solution:
            msg += QObject::tr("contains degenerate tetrahedra");
            break;
        case NSnapPeaTriangulation::other_solution:
            msg += QObject::tr("unrecognised solution type");
            break;
        case NSnapPeaTriangulation::no_solution:
            msg += QObject::tr("no solution found");
            break;
        default:
            msg += QObject::tr("INVALID SOLUTION TYPE");
    }

    return msg;
}

