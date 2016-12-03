
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

// Regina core includes:
#include "snappea/snappeatriangulation.h"

// UI includes:
#include "clickablelabel.h"
#include "eventids.h"
#include "iconcache.h"
#include "snappeaui.h"
#include "snappeaalgebra.h"
#include "snappeafile.h"
#include "snappeagluings.h"
#include "snappeashapes.h"
#include "tri3composition.h"
#include "tri3skeleton.h"
#include "tri3surfaces.h"
#include "packeteditiface.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QApplication>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::Packet;
using regina::SnapPeaTriangulation;

SnapPeaUI::SnapPeaUI(regina::SnapPeaTriangulation* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane,
            ReginaPrefSet::global().tabSnapPeaTri) {
    SnapPeaHeaderUI* header = new SnapPeaHeaderUI(packet, this);
    shapes = new SnapPeaShapesUI(packet, this,
        newEnclosingPane->isReadWrite());
    gluings = new SnapPeaGluingsUI(packet, this);
    skeleton = new Tri3SkeletonUI(packet, this);
    algebra = new SnapPeaAlgebraUI(packet, this);

    shapes->fillToolBar(header->getToolBar());

    addHeader(header);
    addTab(shapes, QObject::tr("S&hapes && Cusps"));
    addTab(gluings, QObject::tr("&Gluings"));
    addTab(skeleton, QObject::tr("&Skeleton"));
    addTab(algebra, QObject::tr("&Algebra"));
    addTab(new Tri3CompositionUI(packet, this), QObject::tr("&Composition"));
    addTab(new Tri3SurfacesUI(packet, this), QObject::tr("&Recognition"));
    addTab(new SnapPeaFileUI(packet, this), QObject::tr("&File"));

    editIface = new PacketEditTabbedUI(this);
}

SnapPeaUI::~SnapPeaUI() {
    delete editIface;
}

const QLinkedList<QAction*>& SnapPeaUI::getPacketTypeActions() {
    return shapes->getPacketTypeActions();
}

QString SnapPeaUI::getPacketMenuText() const {
    return QObject::tr("&SnapPea Triangulation");
}

SnapPeaHeaderUI::SnapPeaHeaderUI(regina::SnapPeaTriangulation* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);
    uiLayout->setContentsMargins(0, 0, 0, 0);

    bar = new QToolBar(ui);
    bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    uiLayout->addWidget(bar);

    QBoxLayout* headerLayout = new QHBoxLayout(ui);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    header = new QLabel();
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    header->setWhatsThis(QObject::tr("Displays a few basic properties of the "
        "triangulation, such as orientability and solution type."));
    headerLayout->addWidget(header, 1);
    locked = new ClickableLabel(IconCache::icon(IconCache::lock));
    locked->setWhatsThis(tr(
        "<qt>This triangulation cannot be changed, since it has "
        "normal surfaces and/or angle structures that refer to it.<p>"
        "Click on the padlock for more information.</qt>"));
    locked->hide();
    connect(locked, SIGNAL(clicked()), this, SLOT(lockedExplanation()));
    headerLayout->addWidget(locked);
    headerLayout->addSpacing(10);
    uiLayout->addLayout(headerLayout);

    // Register ourselves as a lister for child changes, so we can
    // update the lock icon accordingly.
    tri->listen(this);
}

regina::Packet* SnapPeaHeaderUI::getPacket() {
    return tri;
}

QWidget* SnapPeaHeaderUI::getInterface() {
    return ui;
}

void SnapPeaHeaderUI::refresh() {
    header->setText(summaryInfo(tri));
    refreshLock();
}

QString SnapPeaHeaderUI::summaryInfo(regina::SnapPeaTriangulation* tri) {
    if (tri->isNull())
        return QObject::tr("Null triangulation (no SnapPea data)");
    if (tri->isEmpty())
        return QObject::tr("Empty");

    if (! tri->isValid())
        return QObject::tr("INVALID TRIANGULATION!");

    QString msg;

    if (tri->isOrientable())
        msg += QObject::tr("Orientable, ");
    else
        msg += QObject::tr("Non-orientable, ");

    unsigned nFilled = tri->countFilledCusps();
    unsigned nComplete = tri->countCompleteCusps();

    if (nFilled + nComplete == 1) {
        msg += QObject::tr("1 cusp, ");
        if (nFilled)
            msg += QObject::tr("filled\n");
        else
            msg += QObject::tr("complete\n");
    } else if (nFilled == 0) {
        msg += QObject::tr("%1 cusps, all complete\n").arg(nComplete);
    } else if (nComplete == 0) {
        msg += QObject::tr("%1 cusps, all filled\n").arg(nFilled);
    } else {
        msg += QObject::tr("%1 cusps, %2 filled\n")
            .arg(nFilled + nComplete).arg(nFilled);
    }

    switch (tri->solutionType()) {
        case SnapPeaTriangulation::not_attempted:
            msg += QObject::tr("Solution not attempted");
            break;
        case SnapPeaTriangulation::geometric_solution:
            if (tri->volumeZero())
                msg += QObject::tr("Volume approximately zero\n");
            else
                msg += QObject::tr("Volume: %1\n").arg(tri->volume());
            msg += QObject::tr("Tetrahedra positively oriented");
            break;
        case SnapPeaTriangulation::nongeometric_solution:
            if (tri->volumeZero())
                msg += QObject::tr("Volume approximately zero\n");
            else
                msg += QObject::tr("Volume: %1\n").arg(tri->volume());
            msg += QObject::tr("Contains flat or negative tetrahedra");
            break;
        case SnapPeaTriangulation::flat_solution:
            if (tri->volumeZero())
                msg += QObject::tr("Volume approximately zero\n");
            else
                msg += QObject::tr("Volume: %1\n").arg(tri->volume());
            msg += QObject::tr("All tetrahedra flat");
            break;
        case SnapPeaTriangulation::degenerate_solution:
            if (tri->volumeZero())
                msg += QObject::tr("Volume approximately zero\n");
            else
                msg += QObject::tr("Volume: %1\n").arg(tri->volume());
            msg += QObject::tr("Contains degenerate tetrahedra");
            break;
        case SnapPeaTriangulation::other_solution:
            msg += QObject::tr("Unrecognised solution type");
            break;
        case SnapPeaTriangulation::externally_computed:
            msg += QObject::tr("Externally computed");
            break;
        case SnapPeaTriangulation::no_solution:
            msg += QObject::tr("No solution found");
            break;
        default:
            msg += QObject::tr("INVALID SOLUTION TYPE");
    }

    return msg;
}

void SnapPeaHeaderUI::lockedExplanation() {
    if (tri->isPacketEditable())
        return;

    ReginaSupport::info(ui,
        tr("This triangulation cannot be changed."),
        tr("<qt>There are normal surfaces and/or angle structures "
            "that refer to it, and so you cannot change its "
            "tetrahedron gluings.<p>"
            "You may clone the triangulation (through the "
            "<i>Packet Tree</i> menu in the main window), and then "
            "edit the clone instead.</qt>"));
}

void SnapPeaHeaderUI::childWasAdded(regina::Packet* packet,
        regina::Packet* child) {
    // Be careful - we may not be in the GUI thread.
    QApplication::postEvent(this, new QEvent(
        (QEvent::Type)EVT_HEADER_CHILD_ADDED));
}

void SnapPeaHeaderUI::childWasRemoved(regina::Packet* packet,
        regina::Packet* child, bool inParentDestructor) {
    if (! inParentDestructor)
        refreshLock();
}

void SnapPeaHeaderUI::refreshLock() {
    if (tri->isPacketEditable())
        locked->hide();
    else
        locked->show();
}

void SnapPeaHeaderUI::customEvent(QEvent* event) {
    if (event->type() == EVT_HEADER_CHILD_ADDED)
        refreshLock();
}

