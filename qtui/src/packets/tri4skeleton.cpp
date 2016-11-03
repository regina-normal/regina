
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
#include "triangulation/facetpairing.h"
#include "triangulation/dim4.h"

// UI includes:
#include "tri4skeleton.h"
#include "facetgraphtab.h"
#include "skeletonwindow.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../messagelayer.h"

#include <QLabel>
#include <QLayout>
#include <QPushButton>

using regina::Packet;
using regina::Triangulation;

Tri4SkeletonUI::Tri4SkeletonUI(regina::Triangulation<4>* packet,
        PacketTabbedUI* useParentUI) :
        PacketTabbedViewerTab(useParentUI,
            ReginaPrefSet::global().tabDim4TriSkeleton) {
    addTab(new Tri4SkelCompUI(packet, this), tr("&Skeletal Components"));
    addTab(new FacetGraphTab(new Dim4FacetGraphData(packet), this),
        tr("&Graphs"));
}

Tri4SkelCompUI::Tri4SkelCompUI(regina::Triangulation<4>* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);
    layout->addStretch(1);

    QGridLayout* grid = new QGridLayout();
    layout->addLayout(grid);
    grid->setColumnStretch(0, 1);
    grid->setColumnMinimumWidth(2, 5);
    grid->setColumnMinimumWidth(4, 10);
    grid->setColumnMinimumWidth(6, 10);
    grid->setColumnMinimumWidth(8, 5);
    grid->setColumnMinimumWidth(10, 10);
    grid->setColumnStretch(12, 1);

    QLabel* label;
    QPushButton* btn;
    QString msg;

    label = new QLabel(tr("Vertices:"), ui);
    grid->addWidget(label, 0, 1);
    nVertices = new QLabel(ui);
    nVertices->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nVertices, 0, 3);
    msg = tr("The total number of vertices in this triangulation.");
    label->setWhatsThis(msg);
    nVertices->setWhatsThis(msg);

    label = new QLabel(tr("Edges:"), ui);
    grid->addWidget(label, 1, 1);
    nEdges = new QLabel(ui);
    nEdges ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nEdges, 1, 3);
    msg = tr("The total number of edges in this triangulation.");
    label->setWhatsThis(msg);
    nEdges->setWhatsThis(msg);

    label = new QLabel(tr("Triangles:"), ui);
    grid->addWidget(label, 2, 1);
    nTriangles = new QLabel(ui);
    nTriangles->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nTriangles, 2, 3);
    msg = tr("The total number of triangles in this triangulation.");
    label->setWhatsThis(msg);
    nTriangles->setWhatsThis(msg);

    label = new QLabel(tr("Tetrahedra:"), ui);
    grid->addWidget(label, 3, 1);
    nTetrahedra = new QLabel(ui);
    nTetrahedra->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nTetrahedra, 3, 3);
    msg = tr("The total number of tetrahedra in this triangulation.");
    label->setWhatsThis(msg);
    nTetrahedra->setWhatsThis(msg);

    label = new QLabel(tr("Pentachora:"), ui);
    grid->addWidget(label, 4, 1);
    nPentachora = new QLabel(ui);
    nPentachora->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nPentachora, 4, 3);
    msg = tr("The total number of pentachora in this triangulation.");
    label->setWhatsThis(msg);
    nPentachora->setWhatsThis(msg);

    label = new QLabel(tr("Components:"), ui);
    grid->addWidget(label, 0, 7);
    nComps = new QLabel(ui);
    nComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nComps, 0, 9);
    msg = tr("The total number of connected components in this "
        "triangulation.");
    label->setWhatsThis(msg);
    nComps->setWhatsThis(msg);

    label = new QLabel(tr("Bdry Components:"), ui);
    grid->addWidget(label, 1, 7);
    nBdryComps = new QLabel(ui);
    nBdryComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nBdryComps, 1, 9);
    msg = tr("The total number of boundary components in this triangulation.");
    label->setWhatsThis(msg);
    nBdryComps->setWhatsThis(msg);

    label = new QLabel(trUtf8("χ (Triangulation):"), ui);
    grid->addWidget(label, 2, 7);
    eulerTri = new QLabel(ui);
    eulerTri->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(eulerTri, 2, 9);
    msg = tr("The Euler characteristic of the triangulation.  "
        "This is computed precisely as "
        "(vertices - edges + triangles - tetrahedra + pentachora).");
    label->setWhatsThis(msg);
    eulerTri->setWhatsThis(msg);

    eulerManifoldLabel = new QLabel(trUtf8("χ (Manifold):"), ui);
    grid->addWidget(eulerManifoldLabel, 3, 7);
    eulerManifold = new QLabel(ui);
    eulerManifold->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(eulerManifold, 3, 9);
    msg = tr("The Euler characteristic of the underlying compact 3-manifold.  "
        "This is computed as though we had truncated all ideal vertices.");
    eulerManifoldLabel->setWhatsThis(msg);
    eulerManifold->setWhatsThis(msg);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    // btn->setFlat(true);
    btn->setToolTip(tr("View details of individual vertices"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual vertices in a separate window."));
    connect(btn, SIGNAL(clicked()), this, SLOT(viewVertices()));
    grid->addWidget(btn, 0, 5);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    btn->setToolTip(tr("View details of individual edges"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual edges in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewEdges()));
    grid->addWidget(btn, 1, 5);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    btn->setToolTip(tr("View details of individual triangles"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual triangles in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewTriangles()));
    grid->addWidget(btn, 2, 5);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    btn->setToolTip(tr("View details of individual tetrahedra"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual tetrahedra in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewTetrahedra()));
    grid->addWidget(btn, 3, 5);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    btn->setToolTip(tr("View details of individual components"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual connected components in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewComponents()));
    grid->addWidget(btn, 0, 11);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    btn->setToolTip(tr("View details of individual boundary components"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual boundary components in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewBoundaryComponents()));
    grid->addWidget(btn, 1, 11);

    layout->addStretch(1);
}

regina::Packet* Tri4SkelCompUI::getPacket() {
    return tri;
}

QWidget* Tri4SkelCompUI::getInterface() {
    return ui;
}

void Tri4SkelCompUI::refresh() {
    nVertices->setText(QString::number(tri->countVertices()));
    nEdges->setText(QString::number(tri->countEdges()));
    nTriangles->setText(QString::number(tri->countTriangles()));
    nTetrahedra->setText(QString::number(tri->countTetrahedra()));
    nPentachora->setText(QString::number(tri->size()));
    nComps->setText(QString::number(tri->countComponents()));
    nBdryComps->setText(QString::number(tri->countBoundaryComponents()));

    eulerTri->setText(QString::number(tri->eulerCharTri()));
    if (tri->isValid()) {
        eulerManifoldLabel->show();
        eulerManifold->show();
        eulerManifold->setText(QString::number(tri->eulerCharManifold()));
    } else {
        eulerManifoldLabel->hide();
        eulerManifold->hide();
    }

    QLinkedListIterator<SkeletonWindow*> it(viewers);
    while( it.hasNext())
        (it.next())->refresh();
}

void Tri4SkelCompUI::viewVertices() {
    // Because we pass this as parent to the new window, we are
    // guaranteed that the window will be closed and deleted
    // automatically if the packet pane is closed.
    // Similarly for edges, triangles, etc.
    SkeletonWindow* win = new SkeletonWindow(this, new Vertex4Model(tri));
    win->show();
    viewers.append(win);
}

void Tri4SkelCompUI::viewEdges() {
    SkeletonWindow* win = new SkeletonWindow(this, new Edge4Model(tri));
    win->show();
    viewers.append(win);
}

void Tri4SkelCompUI::viewTriangles() {
    SkeletonWindow* win = new SkeletonWindow(this, new Triangle4Model(tri));
    win->show();
    viewers.append(win);
}

void Tri4SkelCompUI::viewTetrahedra() {
    SkeletonWindow* win = new SkeletonWindow(this,
        new Tetrahedron4Model(tri));
    win->show();
    viewers.append(win);
}

void Tri4SkelCompUI::viewComponents() {
    SkeletonWindow* win = new SkeletonWindow(this, new Component4Model(tri));
    win->show();
    viewers.append(win);
}

void Tri4SkelCompUI::viewBoundaryComponents() {
    SkeletonWindow* win = new SkeletonWindow(this,
        new BoundaryComponent4Model(tri));
    win->show();
    viewers.append(win);
}

