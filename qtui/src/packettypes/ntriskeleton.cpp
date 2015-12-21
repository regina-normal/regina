
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

// Regina core includes:
#include "census/nfacepairing.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "cycledrawingtab.h"
#include "facetgraphtab.h"
#include "ntriskeleton.h"
#include "skeletonwindow.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../messagelayer.h"

#include <QLabel>
#include <QLayout>
#include <QPushButton>

using regina::NPacket;
using regina::NTriangulation;

NTriSkeletonUI::NTriSkeletonUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketTabbedViewerTab(useParentUI,
            ReginaPrefSet::global().tabDim3TriSkeleton) {
    addTab(new NTriSkelCompUI(packet, this), tr("&Skeletal Components"));
    addTab(new FacetGraphTab(new Dim3FaceGraphData(packet), this),
        tr("&Face Pairing Graph"));
    addTab(new CycleDrawingTab(new Dim3CycleDrawingData(packet), this),
            tr("&Cycle Drawing"));
}

NTriSkelCompUI::NTriSkelCompUI(regina::NTriangulation* packet,
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
    msg = tr("The total number of boundary components in this "
        "triangulation.  Boundary components can either be ideal vertices "
        "or collections of adjacent boundary triangles.");
    label->setWhatsThis(msg);
    nBdryComps->setWhatsThis(msg);

    label = new QLabel(tr("Tetrahedra:"), ui);
    grid->addWidget(label, 2, 7);
    nTets = new QLabel(ui);
    nTets->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nTets, 2, 9);
    msg = tr("The total number of tetrahedra in this triangulation.");
    label->setWhatsThis(msg);
    nTets->setWhatsThis(msg);

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
        "individual boundary components in a separate window.  Note that "
        "boundary components can either be ideal vertices of collections "
        "of adjacent boundary triangles."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewBoundaryComponents()));
    grid->addWidget(btn, 1, 11);

    layout->addStretch(1);
}

regina::NPacket* NTriSkelCompUI::getPacket() {
    return tri;
}

QWidget* NTriSkelCompUI::getInterface() {
    return ui;
}

void NTriSkelCompUI::refresh() {
    nVertices->setText(QString::number(tri->getNumberOfVertices()));
    nEdges->setText(QString::number(tri->getNumberOfEdges()));
    nTriangles->setText(QString::number(tri->getNumberOfTriangles()));
    nTets->setText(QString::number(tri->getNumberOfTetrahedra()));
    nComps->setText(QString::number(tri->getNumberOfComponents()));
    nBdryComps->setText(QString::number(tri->getNumberOfBoundaryComponents()));
    
    QLinkedListIterator<SkeletonWindow*> it(viewers);
    while( it.hasNext())
        (it.next())->refresh();
}

void NTriSkelCompUI::viewVertices() {
    // Because we pass this as parent to the new window, we are
    // guaranteed that the window will be closed and deleted
    // automatically if the packet pane is closed.
    // Similarly for edges, triangles, etc.
    SkeletonWindow* win = new SkeletonWindow(this, new VertexModel(tri));
    win->show();
    viewers.append(win);
}

void NTriSkelCompUI::viewEdges() {
    SkeletonWindow* win = new SkeletonWindow(this, new EdgeModel(tri));
    win->show();
    viewers.append(win);
}

void NTriSkelCompUI::viewTriangles() {
    SkeletonWindow* win = new SkeletonWindow(this, new TriangleModel(tri));
    win->show();
    viewers.append(win);
}

void NTriSkelCompUI::viewComponents() {
    SkeletonWindow* win = new SkeletonWindow(this, new ComponentModel(tri));
    win->show();
    viewers.append(win);
}

void NTriSkelCompUI::viewBoundaryComponents() {
    SkeletonWindow* win = new SkeletonWindow(this,
        new BoundaryComponentModel(tri));
    win->show();
    viewers.append(win);
}

