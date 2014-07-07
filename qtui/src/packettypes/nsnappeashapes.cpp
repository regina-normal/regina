
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

// Regina core includes:
#include "snappea/nsnappeatriangulation.h"

// UI includes:
#include "nsnappeashapes.h"

#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>

using regina::NPacket;
using regina::NSnapPeaTriangulation;

NSnapPeaShapesUI::NSnapPeaShapesUI(regina::NSnapPeaTriangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QLabel* label = new QLabel(tr("Cusps:"));
    layout->addWidget(label);

    cusps = new QTreeWidget();
    cusps->setRootIsDecorated(false);
    cusps->setAlternatingRowColors(true);
    cusps->header()->setStretchLastSection(false);
    cusps->header()->setResizeMode(QHeaderView::ResizeToContents);
    cusps->setSelectionMode(QAbstractItemView::NoSelection);
    cusps->setWhatsThis(tr("<qt>Shows information on each cusp.  "
        "Cusps are numbered according to SnapPea's internal numbering "
        "(see the <i>Cusp #</i> column).  This table gives the "
        "corresponding vertex number (using Regina's numbering, as seen "
        "in the <i>Skeleton</i> tab), and shows the filling on each cusp "
        "(if any).</qt>"));
    layout->addWidget(cusps, 1);

    label = new QLabel(tr("Tetrahedron shapes:"));
    layout->addWidget(label);

    shapes = new QTreeWidget();
    shapes->setRootIsDecorated(false);
    shapes->setAlternatingRowColors(true);
    shapes->header()->setStretchLastSection(false);
    shapes->header()->setResizeMode(QHeaderView::ResizeToContents);
    shapes->setSelectionMode(QAbstractItemView::NoSelection);
    shapes->setWhatsThis(tr("Shows the shape of each tetrahedron, "
        "with respect to the Dehn filled hyperbolic structure.  "
        "Tetrahedron shapes are given in rectangular form, and using "
        "a fixed coordinate system (fixed alignment, in SnapPea's "
        "terminology)."));
    layout->addWidget(shapes, 3);
}

regina::NPacket* NSnapPeaShapesUI::getPacket() {
    return tri;
}

QWidget* NSnapPeaShapesUI::getInterface() {
    return ui;
}

void NSnapPeaShapesUI::refresh() {
    cusps->clear();
    shapes->clear();

    QTreeWidgetItem *row, *header;

    cusps->setColumnCount(3);
    header = new QTreeWidgetItem();
    header->setText(0, tr("Cusp #"));
    header->setText(1, tr("Vertex #"));
    header->setText(2, tr("Filling"));
    header->setTextAlignment(0, Qt::AlignCenter);
    header->setTextAlignment(1, Qt::AlignCenter);
    header->setTextAlignment(2, Qt::AlignCenter);
    cusps->setHeaderItem(header);

    shapes->setColumnCount(3);
    header = new QTreeWidgetItem();
    header->setText(0, tr("Tet #"));
    header->setText(1, tr("Real"));
    header->setText(2, tr("Imag"));
    header->setTextAlignment(0, Qt::AlignCenter);
    header->setTextAlignment(1, Qt::AlignCenter);
    header->setTextAlignment(2, Qt::AlignCenter);
    shapes->setHeaderItem(header);

    if (tri->isNull())
        return;

    unsigned i;
    std::complex<double> s;
    for (i = 0; i < tri->getNumberOfBoundaryComponents(); ++i) {
        row = new QTreeWidgetItem();
        row->setText(0, QString::number(i));
        row->setText(1, QString::number(tri->cusp(i)->vertex()->markedIndex()));
        if (tri->cusp(i)->complete())
            row->setText(2, QString(QChar(0x2014 /* emdash */)));
        else
            row->setText(2, tr("%1, %2")
                .arg(tri->cusp(i)->m())
                .arg(tri->cusp(i)->l()));
        row->setTextAlignment(0, Qt::AlignCenter);
        row->setTextAlignment(1, Qt::AlignCenter);
        row->setTextAlignment(2, Qt::AlignCenter);
        cusps->addTopLevelItem(row);
    }

    if (tri->solutionType() == NSnapPeaTriangulation::not_attempted ||
            tri->solutionType() == NSnapPeaTriangulation::no_solution)
        return;

    for (i = 0; i < tri->getNumberOfTetrahedra(); ++i) {
        s = tri->shape(i);
        row = new QTreeWidgetItem();
        row->setText(0, QString::number(i));
        row->setText(1, QString::number(s.real()));
        row->setText(2, QString::number(s.imag()));
        row->setTextAlignment(0, Qt::AlignRight);
        row->setTextAlignment(1, Qt::AlignRight);
        row->setTextAlignment(2, Qt::AlignRight);
        shapes->addTopLevelItem(row);
    }
}

void NSnapPeaShapesUI::editingElsewhere() {
    cusps->clear();
    shapes->clear();
}

