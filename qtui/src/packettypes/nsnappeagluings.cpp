
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
#include "nsnappeagluings.h"
#include "patiencedialog.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "choosers/vertexchooser.h"

#include <memory>
#include <QAction>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableView>
#include <QToolBar>

using regina::NPacket;
using regina::NSnapPeaTriangulation;

NSnapPeaGluingsUI::NSnapPeaGluingsUI(regina::NSnapPeaTriangulation* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), tri(packet) {
    // Set up the table of face gluings.
    model = new GluingsModel(false /* read-only */);
    faceTable = new QTableView();
    faceTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
    faceTable->setModel(model);
    faceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    faceTable->setWhatsThis(tr("<qt>A table specifying which tetrahedron "
        "faces are identified with which others.<p>"
        "Tetrahedra are numbered upwards from 0, and the four vertices of "
        "each tetrahedron are numbered 0, 1, 2 and 3.  Each row of the table "
        "represents a single tetrahedron, and shows the identifications "
        "for each of its four faces.<p>"
        "As an example, if we are looking at the table cell for face 012 of "
        "tetrahedron 7, a gluing of <i>5 (031)</i> shows that "
        "that this face is identified with face 031 of tetrahedron 5, in "
        "such a way that vertices 0, 1 and 2 of tetrahedron "
        "7 are mapped to vertices 0, 3 and 1 respectively of tetrahedron 5.<p>"
        "To change these identifications, simply type your own gluings into "
        "the table.</qt>"));

    faceTable->verticalHeader()->hide();

    //faceTable->setColumnStretchable(0, true);
    //faceTable->setColumnStretchable(1, true);
    //faceTable->setColumnStretchable(2, true);
    //faceTable->setColumnStretchable(3, true);
    //faceTable->setColumnStretchable(4, true);

    ui = faceTable;

    // Set up the triangulation actions.
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
    actCanonise->setText(tr("&Canonical retriangulation"));
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

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.append(sep);

    actToRegina = new QAction(this);
    actToRegina->setText(tr("&Convert to Regina"));
    actToRegina->setIcon(ReginaSupport::regIcon("packet_triangulation"));
    actToRegina->setToolTip(tr(
        "Convert this to a Regina triangulation"));
    actToRegina->setWhatsThis(tr("Convert this to one of Regina's native "
        "3-manifold triangulations.  The original SnapPea triangulation "
        "will be kept and left untouched.<p>"
        "A native Regina triangulation will allow you to use Regina's "
        "full suite of tools to edit and analyse the triangulation.  "
        "However, the native Regina "
        "triangulation will lose any SnapPea-specific "
        "information (such as peripheral curves on cusps)."));
    triActionList.append(actToRegina);
    requiresNonNull.append(actToRegina);
    connect(actToRegina, SIGNAL(triggered()), this, SLOT(toRegina()));

    // Tidy up.

    refresh();
}

NSnapPeaGluingsUI::~NSnapPeaGluingsUI() {
    // Make sure the actions, including separators, are all deleted.

    delete model;
}

const QLinkedList<QAction*>& NSnapPeaGluingsUI::getPacketTypeActions() {
    return triActionList;
}

void NSnapPeaGluingsUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actRandomise);
    bar->addAction(actToRegina);
}

regina::NPacket* NSnapPeaGluingsUI::getPacket() {
    return tri;
}

QWidget* NSnapPeaGluingsUI::getInterface() {
    return ui;
}

void NSnapPeaGluingsUI::refresh() {
    model->refreshData(tri);
    updateNonNullActions();
    setDirty(false);
}

void NSnapPeaGluingsUI::commit() {
    // Nothing to commit, since the gluings table is read-only.
    setDirty(false);
}

void NSnapPeaGluingsUI::setReadWrite(bool readWrite) {
    // Regardless of whether we allow edits, we can do nothing with a
    // null triangulation.
    if (tri->isNull())
        readWrite = false;

    QLinkedListIterator<QAction*> it(enableWhenWritable);
    while (it.hasNext())
        (it.next())->setEnabled(readWrite);

    updateNonNullActions();
}

void NSnapPeaGluingsUI::vertexLinks() {
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

void NSnapPeaGluingsUI::toRegina() {
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

void NSnapPeaGluingsUI::randomise() {
    if (! enclosingPane->commitToModify())
        return;

    tri->randomize();
}

void NSnapPeaGluingsUI::canonise() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    if (tri->isNull())
        ReginaSupport::sorry(ui,
            tr("This is a null triangulation: there is no SnapPea "
            "triangulation for me to canonise."));
    else {
        regina::NSnapPeaTriangulation* ans = tri->canonise();
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

void NSnapPeaGluingsUI::updateNonNullActions() {
    if (tri->isNull()) {
        QLinkedListIterator<QAction*> it(requiresNonNull);
        while (it.hasNext())
            (it.next())->setEnabled(false);
    }
}

