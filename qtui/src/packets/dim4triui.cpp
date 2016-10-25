
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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
#include "triangulation/dim4.h"

// UI includes:
#include "clickablelabel.h"
#include "eventids.h"
#include "iconcache.h"
#include "dim4trialgebra.h"
#include "dim4trigluings.h"
#include "dim4triskeleton.h"
#include "dim4triui.h"
#include "packeteditiface.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QApplication>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::Packet;
using regina::Triangulation;

Dim4TriangulationUI::Dim4TriangulationUI(regina::Triangulation<4>* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane, ReginaPrefSet::global().tabDim4Tri) {
    Dim4TriHeaderUI* header = new Dim4TriHeaderUI(packet, this);
    gluings = new Dim4TriGluingsUI(packet, this,
        newEnclosingPane->isReadWrite());
    skeleton = new Dim4TriSkeletonUI(packet, this);
    algebra = new Dim4TriAlgebraUI(packet, this);

    gluings->fillToolBar(header->getToolBar());

    addHeader(header);
    addTab(gluings, QObject::tr("&Gluings"));
    addTab(skeleton, QObject::tr("&Skeleton"));
    addTab(algebra, QObject::tr("&Algebra"));

    editIface = new PacketEditTabbedUI(this);
}

Dim4TriangulationUI::~Dim4TriangulationUI() {
    delete editIface;
}

const QLinkedList<QAction*>& Dim4TriangulationUI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString Dim4TriangulationUI::getPacketMenuText() const {
    return QObject::tr("&4-D Triangulation");
}

Dim4TriHeaderUI::Dim4TriHeaderUI(regina::Triangulation<4>* packet,
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
        "normal hypersurfaces that refer to it.<p>"
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

regina::Packet* Dim4TriHeaderUI::getPacket() {
    return tri;
}

QWidget* Dim4TriHeaderUI::getInterface() {
    return ui;
}

void Dim4TriHeaderUI::refresh() {
    header->setText(summaryInfo(tri));
    refreshLock();
}

QString Dim4TriHeaderUI::summaryInfo(regina::Triangulation<4>* tri) {
    if (tri->isEmpty())
        return QObject::tr("Empty");

    if (! tri->isValid())
        return QObject::tr("INVALID TRIANGULATION!");

    QString msg;

    if (tri->isClosed())
        msg += QObject::tr("Closed, ");
    else {
        if (tri->isIdeal() && tri->hasBoundaryTetrahedra())
            msg += QObject::tr("Ideal & real bdry, ");
        else if (tri->isIdeal())
            msg += QObject::tr("Ideal bdry, ");
        else if (tri->hasBoundaryTetrahedra())
            msg += QObject::tr("Real Bdry, ");
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

void Dim4TriHeaderUI::lockedExplanation() {
    if (tri->isPacketEditable())
        return;

    ReginaSupport::info(ui,
        tr("This triangulation cannot be changed."),
        tr("<qt>There are normal hypersurfaces "
            "that refer to it, and so you cannot change its "
            "pentachoron gluings.<p>"
            "You may clone the triangulation (through the "
            "<i>Packet Tree</i> menu in the main window), and then "
            "edit the clone instead.</qt>"));
}

void Dim4TriHeaderUI::childWasAdded(regina::Packet* packet,
        regina::Packet* child) {
    // Be careful - we may not be in the GUI thread.
    QApplication::postEvent(this, new QEvent(
        (QEvent::Type)EVT_HEADER_CHILD_ADDED));
}

void Dim4TriHeaderUI::childWasRemoved(regina::Packet* packet,
        regina::Packet* child, bool inParentDestructor) {
    if (! inParentDestructor)
        refreshLock();
}

void Dim4TriHeaderUI::refreshLock() {
    if (tri->isPacketEditable())
        locked->hide();
    else
        locked->show();
}

void Dim4TriHeaderUI::customEvent(QEvent* event) {
    if (event->type() == EVT_HEADER_CHILD_ADDED)
        refreshLock();
}

