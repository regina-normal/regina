
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// Regina core includes:
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntriskeleton.h"
#include "skeletonwindow.h"

#include <kiconloader.h>
#include <klocale.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qwhatsthis.h>

using regina::NPacket;
using regina::NTriangulation;

NTriSkeletonUI::NTriSkeletonUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);
    layout->addStretch(1);

    QGridLayout* grid = new QGridLayout(layout, 3, 13, 5 /* spacing */);
    grid->setColStretch(0, 1);
    grid->setColSpacing(2, 5);
    grid->setColSpacing(4, 10);
    grid->setColSpacing(6, 10);
    grid->setColSpacing(8, 5);
    grid->setColSpacing(10, 10);
    grid->setColStretch(12, 1);

    QLabel* label;
    QPushButton* btn;
    QString msg;

    label = new QLabel(i18n("Vertices:"), ui);
    grid->addWidget(label, 0, 1);
    nVertices = new QLabel(ui);
    nVertices->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nVertices, 0, 3);
    msg = i18n("The total number of vertices in this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nVertices, msg);

    label = new QLabel(i18n("Edges:"), ui);
    grid->addWidget(label, 1, 1);
    nEdges = new QLabel(ui);
    nEdges ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nEdges, 1, 3);
    msg = i18n("The total number of edges in this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nEdges, msg);

    label = new QLabel(i18n("Faces:"), ui);
    grid->addWidget(label, 2, 1);
    nFaces = new QLabel(ui);
    nFaces->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nFaces, 2, 3);
    msg = i18n("The total number of faces in this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nFaces, msg);

    label = new QLabel(i18n("Components:"), ui);
    grid->addWidget(label, 0, 7);
    nComps = new QLabel(ui);
    nComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nComps, 0, 9);
    msg = i18n("The total number of disconnected components in this "
        "triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nComps, msg);

    label = new QLabel(i18n("Bdry Components:"), ui);
    grid->addWidget(label, 1, 7);
    nBdryComps = new QLabel(ui);
    nBdryComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nBdryComps, 1, 9);
    msg = i18n("The total number of boundary components in this "
        "triangulation.  Boundary components can either be ideal vertices "
        "or collections of adjacent boundary faces.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nBdryComps, msg);

    label = new QLabel(i18n("Tetrahedra:"), ui);
    grid->addWidget(label, 2, 7);
    nTets = new QLabel(ui);
    nTets->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nTets, 2, 9);
    msg = i18n("The total number of tetrahedra in this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nTets, msg);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    // btn->setFlat(true);
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual vertices."));
    connect(btn, SIGNAL(clicked()), this, SLOT(viewVertices()));
    grid->addWidget(btn, 0, 5);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual edges."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewEdges()));
    grid->addWidget(btn, 1, 5);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual faces."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewFaces()));
    grid->addWidget(btn, 2, 5);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual components."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewComponents()));
    grid->addWidget(btn, 0, 11);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual boundary components.  Note that boundary components "
        "can either be ideal vertices of collections of adjacent "
        "boundary faces."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewBoundaryComponents()));
    grid->addWidget(btn, 1, 11);

    layout->addStretch(1);

    viewers.setAutoDelete(true);
}

regina::NPacket* NTriSkeletonUI::getPacket() {
    return tri;
}

QWidget* NTriSkeletonUI::getInterface() {
    return ui;
}

void NTriSkeletonUI::refresh() {
    nVertices->setText(QString::number(tri->getNumberOfVertices()));
    nEdges->setText(QString::number(tri->getNumberOfEdges()));
    nFaces->setText(QString::number(tri->getNumberOfFaces()));
    nTets->setText(QString::number(tri->getNumberOfTetrahedra()));
    nComps->setText(QString::number(tri->getNumberOfComponents()));
    nBdryComps->setText(QString::number(tri->getNumberOfBoundaryComponents()));

    for (SkeletonWindow* win = viewers.first(); win; win = viewers.next())
        win->refresh();
}

void NTriSkeletonUI::editingElsewhere() {
    nVertices->setText(i18n("Editing..."));
    nEdges->setText(i18n("Editing..."));
    nFaces->setText(i18n("Editing..."));
    nTets->setText(i18n("Editing..."));
    nComps->setText(i18n("Editing..."));
    nBdryComps->setText(i18n("Editing..."));

    for (SkeletonWindow* win = viewers.first(); win; win = viewers.next())
        win->editingElsewhere();
}

void NTriSkeletonUI::viewVertices() {
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Vertices);
    win->show();
    viewers.append(win);
}

void NTriSkeletonUI::viewEdges() {
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Edges);
    win->show();
    viewers.append(win);
}

void NTriSkeletonUI::viewFaces() {
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Faces);
    win->show();
    viewers.append(win);
}

void NTriSkeletonUI::viewComponents() {
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Components);
    win->show();
    viewers.append(win);
}

void NTriSkeletonUI::viewBoundaryComponents() {
    SkeletonWindow* win = new SkeletonWindow(this,
        SkeletonWindow::BoundaryComponents);
    win->show();
    viewers.append(win);
}

#include "ntriskeleton.moc"
