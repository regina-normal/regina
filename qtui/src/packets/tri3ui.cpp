
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
#include "triangulation/dim3.h"

// UI includes:
#include "clickablelabel.h"
#include "eventids.h"
#include "iconcache.h"
#include "tri3algebra.h"
#include "tri3composition.h"
#include "tri3ui.h"
#include "tri3gluings.h"
#include "tri3skeleton.h"
#include "tri3snappea.h"
#include "tri3surfaces.h"
#include "packeteditiface.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QApplication>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::Packet;
using regina::Triangulation;

Tri3UI::Tri3UI(regina::Triangulation<3>* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane, ReginaPrefSet::global().tabDim3Tri) {
    Tri3HeaderUI* header = new Tri3HeaderUI(packet, this);
    gluings = new Tri3GluingsUI(packet, this, newEnclosingPane->isReadWrite());
    skeleton = new Tri3SkeletonUI(packet, this);
    algebra = new Tri3AlgebraUI(packet, this);
    surfaces = new Tri3SurfacesUI(packet, this);
    snapPea = new Tri3SnapPeaUI(packet, this);

    gluings->fillToolBar(header->getToolBar());

    addHeader(header);
    addTab(gluings, QObject::tr("&Gluings"));
    addTab(skeleton, QObject::tr("&Skeleton"));
    addTab(algebra, QObject::tr("&Algebra"));
    addTab(new Tri3CompositionUI(packet, this), QObject::tr("&Composition"));
    addTab(surfaces, QObject::tr("&Recognition"));
    addTab(snapPea, QObject::tr("Snap&Pea"));

    editIface = new PacketEditTabbedUI(this);
}

Tri3UI::~Tri3UI() {
    delete editIface;
}

const QLinkedList<QAction*>& Tri3UI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString Tri3UI::getPacketMenuText() const {
    return QObject::tr("3-D T&riangulation");
}

Tri3HeaderUI::Tri3HeaderUI(regina::Triangulation<3>* packet,
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
        "triangulation, such as boundary and orientability."));
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

regina::Packet* Tri3HeaderUI::getPacket() {
    return tri;
}

QWidget* Tri3HeaderUI::getInterface() {
    return ui;
}

void Tri3HeaderUI::refresh() {
    header->setText(summaryInfo(tri));
    refreshLock();
}

QString Tri3HeaderUI::summaryInfo(regina::Triangulation<3>* tri) {
    if (tri->isEmpty())
        return QObject::tr("Empty");

    if (! tri->isValid())
        return QObject::tr("INVALID TRIANGULATION!");

    QString msg;

    if (tri->isClosed())
        msg += QObject::tr("Closed, ");
    else {
        if (tri->isIdeal() && tri->hasBoundaryTriangles())
            msg += QObject::tr("Ideal & real bdry, ");
        else if (tri->isIdeal())
            msg += QObject::tr("Ideal bdry, ");
        else if (tri->hasBoundaryTriangles())
            msg += QObject::tr("Real bdry, ");
    }

    if (tri->isOrientable()) {
        if (tri->isOriented())
            msg += QObject::tr("orientable and oriented, ");
        else
            msg += QObject::tr("orientable but not oriented, ");
    } else
        msg += QObject::tr("non-orientable, ");

    msg += (tri->isConnected() ? QObject::tr("connected") :
        QObject::tr("disconnected"));

    return msg;
}

void Tri3HeaderUI::lockedExplanation() {
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

void Tri3HeaderUI::childWasAdded(regina::Packet* packet,
        regina::Packet* child) {
    // Be careful - we may not be in the GUI thread.
    QApplication::postEvent(this, new QEvent(
        (QEvent::Type)EVT_HEADER_CHILD_ADDED));
}

void Tri3HeaderUI::childWasRemoved(regina::Packet* packet,
        regina::Packet* child, bool inParentDestructor) {
    if (! inParentDestructor)
        refreshLock();
}

void Tri3HeaderUI::refreshLock() {
    if (tri->isPacketEditable())
        locked->hide();
    else
        locked->show();
}

void Tri3HeaderUI::customEvent(QEvent* event) {
    if (event->type() == EVT_HEADER_CHILD_ADDED)
        refreshLock();
}

