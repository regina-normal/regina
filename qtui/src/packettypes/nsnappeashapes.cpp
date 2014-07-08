
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
#include "dim2/dim2triangulation.h"
#include "snappea/nsnappeatriangulation.h"

// UI includes:
#include "nsnappeashapes.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "choosers/cuspchooser.h"
#include "choosers/vertexchooser.h"

#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QToolBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>

using regina::NPacket;
using regina::NSnapPeaTriangulation;

NSnapPeaShapesUI::NSnapPeaShapesUI(regina::NSnapPeaTriangulation* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), tri(packet) {
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

    // Set up the actions.
    QAction* sep;

    actRandomise = new QAction(this);
    actRandomise->setText(tr("&Randomise"));
    actRandomise->setIcon(ReginaSupport::regIcon("randomise"));
    actRandomise->setToolTip(tr(
        "Randomise this triangulation"));
    actRandomise->setEnabled(readWrite);
    actRandomise->setWhatsThis(tr("Randomise this triangulation.  "
        "The manifold will be randomly retriangulated using local moves "
        "that preserve the topology."));
    enableWhenWritable.append(actRandomise);
    triActionList.append(actRandomise);
    requiresNonNull.append(actRandomise);
    connect(actRandomise, SIGNAL(triggered()), this, SLOT(randomise()));

    QAction* actCanonise = new QAction(this);
    actCanonise->setText(tr("&Canonical Retriangulation"));
    actCanonise->setToolTip(tr(
        "Build the canonical retriangulation of the canonical "
        "cell decomposition"));
    actCanonise->setWhatsThis(tr("<qt>Build the canonical retriangulation "
        "of the canonical cell decomposition.<p>"
        "The canonical cell decomposition is described in "
        "<i>Convex hulls and isometries of cusped hyperbolic 3-manifolds</i>, "
        "Jeffrey R. Weeks, Topology Appl. 52 (1993), 127-149.<p>"
        "If this canonical cell decomposition contains non-tetrahedron "
        "cells, then SnapPea will canonically retriangulate it by introducing "
        "internal vertices.  See the user handbook for details.<p>"
        "<b>Warning:</b> SnapPea might not compute the canonical "
        "cell decomposition correctly.  However, it does guarantee "
        "that the resulting manifold is homeomorphic to the original.</qt>"));
    triActionList.append(actCanonise);
    requiresNonNull.append(actCanonise);
    connect(actCanonise, SIGNAL(triggered()), this, SLOT(canonise()));

    QAction* actVertexLinks = new QAction(this);
    actVertexLinks->setText(tr("&Vertex Links..."));
    actVertexLinks->setIcon(ReginaSupport::regIcon("vtxlinks"));
    actVertexLinks->setToolTip(tr(
        "Build a 2-manifold triangulation from a vertex link"));
    actVertexLinks->setWhatsThis(tr("<qt>Build a 2-manifold triangulation "
        "from the link of a vertex of this triangulation.<p>"
        "If <i>V</i> is a vertex, then the <i>link</i> of <i>V</i> is the "
        "frontier of a small regular neighbourhood of <i>V</i>.  "
        "The triangles that make up this link sit inside "
        "the tetrahedron corners that meet together at <i>V</i>.</qt>"));
    triActionList.append(actVertexLinks);
    requiresNonNull.append(actVertexLinks);
    connect(actVertexLinks, SIGNAL(triggered()), this, SLOT(vertexLinks()));

    actFill = new QAction(this);
    actFill->setText(tr("Permanently &Fill Cusps..."));
    actFill->setIcon(ReginaSupport::regIcon("fill"));
    actFill->setToolTip(tr(
        "Permanently fill one cusp or all cusps of this manifold"));
    actFill->setWhatsThis(tr("<qt>Retriangulate to permanently "
        "fill either one cusp or all cusps of this manifold.  "
        "The original triangulation will be left untouched.</qt>"));
    triActionList.append(actFill);
    requiresNonNull.append(actFill);
    connect(actFill, SIGNAL(triggered()), this, SLOT(fill()));

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.append(sep);

    actToRegina = new QAction(this);
    actToRegina->setText(tr("&Convert to Regina"));
    actToRegina->setIcon(ReginaSupport::regIcon("packet_triangulation"));
    actToRegina->setToolTip(tr("Convert this to a Regina triangulation"));
    actToRegina->setWhatsThis(tr("<qt>Convert this to one of Regina's native "
        "3-manifold triangulations.  The original SnapPea triangulation "
        "will be kept and left untouched.<p>"
        "A native Regina triangulation will allow you to use Regina's "
        "full suite of tools to edit and analyse the triangulation.  "
        "However, the native Regina "
        "triangulation will lose any SnapPea-specific "
        "information (such as peripheral curves on cusps).</qt>"));
    triActionList.append(actToRegina);
    requiresNonNull.append(actToRegina);
    connect(actToRegina, SIGNAL(triggered()), this, SLOT(toRegina()));

    // Tidy up.
    refresh();
}

const QLinkedList<QAction*>& NSnapPeaShapesUI::getPacketTypeActions() {
    return triActionList;
}

void NSnapPeaShapesUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actRandomise);
    bar->addAction(actFill);
    bar->addAction(actToRegina);
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

    if (tri->isNull()) {
        updateNonNullActions();
        setDirty(false);
        return;
    }

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
            tri->solutionType() == NSnapPeaTriangulation::no_solution) {
        updateNonNullActions();
        setDirty(false);
        return;
    }

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

    updateNonNullActions();
    setDirty(false);
}

void NSnapPeaShapesUI::commit() {
    // Nothing to commit, since commits happen automatically now.
    setDirty(false);
}

void NSnapPeaShapesUI::setReadWrite(bool readWrite) {
    // Regardless of whether we allow edits, we can do nothing with a
    // null triangulation.
    if (tri->isNull())
        readWrite = false;

    QLinkedListIterator<QAction*> it(enableWhenWritable);
    while (it.hasNext())
        (it.next())->setEnabled(readWrite);

    updateNonNullActions();
}

void NSnapPeaShapesUI::vertexLinks() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    if (tri->getVertices().empty())
        ReginaSupport::sorry(ui,
            tr("This triangulation does not have any vertices."));
    else {
        regina::NVertex* chosen =
            VertexDialog::choose(ui, tri, 0 /* filter */,
            tr("Vertex Links"),
            tr("Triangulate the link of which vertex?"),
            tr("<qt>Regina will triangulate the link of whichever "
                "vertex you choose.<p>"
                "If <i>V</i> is a vertex, then the <i>link</i> of "
                "<i>V</i> is the "
                "frontier of a small regular neighbourhood of <i>V</i>.  "
                "The triangles that make up this link sit inside "
                "the tetrahedron corners that meet together at "
                "<i>V</i>.</qt>"));
        if (chosen) {
            regina::Dim2Triangulation* ans = new regina::Dim2Triangulation(
                *chosen->buildLink());
            ans->setPacketLabel(tr("Link of vertex %1").arg(
                tri->vertexIndex(chosen)).toAscii().constData());
            tri->insertChildLast(ans);
            enclosingPane->getMainWindow()->packetView(ans, true, true);
        }
    }
}

void NSnapPeaShapesUI::toRegina() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    if (tri->isNull())
        ReginaSupport::sorry(ui,
            tr("This is a null triangulation: there is no SnapPea "
            "triangulation for me to convert."));
    else {
        regina::NTriangulation* ans = new regina::NTriangulation(*tri);
        ans->setPacketLabel(tri->getPacketLabel());
        tri->insertChildLast(ans);
        enclosingPane->getMainWindow()->packetView(ans, true, true);
    }
}

void NSnapPeaShapesUI::fill() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    if (tri->isNull())
        ReginaSupport::sorry(ui,
            tr("This is a null triangulation: there is no SnapPea "
            "triangulation for me to fill."));
    else if (tri->countFilledCusps() == 0) {
        ReginaSupport::sorry(ui,
            tr("There are no filling coefficients on any of the cusps."),
            tr("You can enter filling coefficients on the "
                "<i>Shapes & Cusps</i> tab."));
    } else {
        regina::NTriangulation* ans;
        if (tri->countFilledCusps() == 1)
            ans = tri->filledTriangulation();
        else {
            int chosen = CuspDialog::choose(ui, tri, CuspChooser::filterFilled,
                tr("Permanently Fill Cusps"),
                tr("Permanently fill which cusp(s)?"),
                tr("SnapPea will retriangulate to permanently fill "
                    "whatever cusp you choose."));
            if (chosen == CuspChooser::CUSP_NO_SELECTION)
                return;
            else if (chosen == CuspChooser::CUSP_ALL)
                ans = tri->filledTriangulation();
            else
                ans = tri->filledTriangulation(chosen);
        }
        if (! ans) {
            ReginaSupport::sorry(ui,
                tr("SnapPea was not able to contruct the filled "
                    "triangulation."),
                tr("Please report this to the Regina developers."));
        } else {
            ans->setPacketLabel(tri->getPacketLabel() + " (Filled)");
            tri->insertChildLast(ans);
            enclosingPane->getMainWindow()->packetView(ans, true, true);
        }
    }
}

void NSnapPeaShapesUI::randomise() {
    if (! enclosingPane->commitToModify())
        return;

    tri->randomize();
}

void NSnapPeaShapesUI::canonise() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    if (tri->isNull())
        ReginaSupport::sorry(ui,
            tr("This is a null triangulation: there is no SnapPea "
            "triangulation for me to canonise."));
    else {
        regina::NTriangulation* ans = tri->canonise();
        if (! ans) {
            ReginaSupport::sorry(ui,
                tr("The SnapPea kernel was not able to build the "
                "canonical retriangulation of the "
                "canonical cell decomposition."));
        } else {
            ans->setPacketLabel(tr("Canonical retriangulation").
                toAscii().constData());
            tri->insertChildLast(ans);
            enclosingPane->getMainWindow()->packetView(ans, true, true);
        }
    }
}

void NSnapPeaShapesUI::updateNonNullActions() {
    if (tri->isNull()) {
        QLinkedListIterator<QAction*> it(requiresNonNull);
        while (it.hasNext())
            (it.next())->setEnabled(false);
    }
}

