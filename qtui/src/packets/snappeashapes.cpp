
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
#include "maths/numbertheory.h"
#include "snappea/snappeatriangulation.h"
#include "triangulation/dim2.h"

// UI includes:
#include "edittreeview.h"
#include "snappeashapes.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "choosers/cuspchooser.h"
#include "choosers/facechooser.h"

#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QRegExp>
#include <QToolBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>

using regina::Packet;
using regina::SnapPeaTriangulation;

namespace {
    QRegExp reIntPair("^[^0-9\\-]*(-?\\d+)[^0-9\\-]+(-?\\d+)[^0-9\\-]*$");
}

void CuspModel::rebuild() {
    beginResetModel();
    endResetModel();
}

QModelIndex CuspModel::index(int row, int column,
        const QModelIndex& parent) const {
    return createIndex(row, column, quint32(3 * row + column));
}

int CuspModel::rowCount(const QModelIndex& /* unused parent*/) const {
    return tri_->countCusps();
}

int CuspModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 3;
}

QVariant CuspModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        const regina::Cusp* cusp = tri_->cusp(index.row());
        switch (index.column()) {
            case 0:
                return QString::number(index.row());
            case 1:
                return QString::number(cusp->vertex()->markedIndex());
            case 2:
                if (cusp->complete())
                    return QString(QChar(0x2014 /* emdash */));
                else
                    return tr("%1, %2").arg(cusp->m()).arg(cusp->l());
            default:
                return QVariant();
        }
    } else if (role == Qt::EditRole) {
        if (index.column() == 2) {
            const regina::Cusp* cusp = tri_->cusp(index.row());
            if (cusp->complete())
                return QString();
            else
                return tr("%1, %2").arg(cusp->m()).arg(cusp->l());
        } else
            return QVariant();
    } else if (role == Qt::ToolTipRole) {
        return headerData(index.column(), Qt::Horizontal, Qt::ToolTipRole);
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

QVariant CuspModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Cusp #");
            case 1: return tr("Vertex #");
            case 2: return tr("Filling");
        }
    } else if (role == Qt::ToolTipRole) {
        switch (section) {
            case 0: return tr("The cusp number according to SnapPea");
            case 1: return tr("<qt>The vertex number according to Regina, "
                "as seen in the <i>Skeleton</i> tab</qt>");
            case 2: return tr("The current filling coefficients on this cusp");
        }
    } else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

Qt::ItemFlags CuspModel::flags(const QModelIndex& index) const {
    if (index.column() == 2)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool CuspModel::setData(const QModelIndex& index, const QVariant& value,
        int role) {
    if (index.column() == 2) {
        QString data = value.toString().trimmed();
        if (data.isEmpty() || data == "-") {
            tri_->unfill(index.row());
            return true;
        }
        if (! reIntPair.exactMatch(data)) {
            ReginaSupport::info(0,
                tr("Please enter a pair of filling coefficients."),
                tr("<qt>This should be a pair of integers, such as "
                    "<i>3, 2</i>."));
            return false;
        }

        bool mOk, lOk;
        int m = reIntPair.cap(1).toInt(&mOk);
        int l = reIntPair.cap(2).toInt(&lOk);
        if (! (mOk && lOk)) {
            ReginaSupport::sorry(0,
                tr("The filling coefficients are too large."),
                tr("The coefficients you provided are too large for "
                    "this machine to store using its native integer type."));
            return false;
        }

        if (m == 0 && l == 0) {
            tri_->unfill(index.row());
            return true;
        }

        if (regina::gcd(m, l) != 1) {
            ReginaSupport::sorry(0,
                tr("The filling coefficients must be coprime."),
                tr("Although SnapPea can handle more general filling "
                    "coefficients, Regina insists that the filling "
                    "coefficients be relatively prime."));
            return false;
        }
        if ((! tri_->cusp(index.row())->vertex()->isLinkOrientable()) &&
                l != 0) {
            ReginaSupport::sorry(0,
                trUtf8("For non-orientable cusps, the filling coefficients "
                    "must be (±1, 0)."),
                trUtf8("Although SnapPea can handle more general filling "
                    "coefficients, Regina insists on (±1, 0) for "
                    "non-orientable cusps."));
            return false;
        }
        if (tri_->fill(m, l, index.row()))
            return true;
        ReginaSupport::info(0,
            tr("I could not use these filling coefficients."),
            tr("<qt>SnapPea rejected them, and I'm not sure why.  "
                "This could happen (for instance) if the coefficients "
                "are too large for SnapPea to represent exactly "
                "using its internal floating point data type.<p>"
                "If you are not sure what is happening, please feel "
                "free to contact the Regina developers.</qt>"));
        return false;
    } else
        return false;
}

SnapPeaShapesUI::SnapPeaShapesUI(regina::SnapPeaTriangulation* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QLabel* label = new QLabel(tr("Cusps:"));
    layout->addWidget(label);

    model = new CuspModel(packet);
    cusps = new EditTreeView();
    cusps->setItemsExpandable(false);
    cusps->setRootIsDecorated(false);
    cusps->setAlternatingRowColors(true);
    cusps->header()->setStretchLastSection(false);
    cusps->setSelectionMode(QTreeView::NoSelection);
    cusps->setWhatsThis(tr("<qt>Shows information on each cusp.<p>"
        "Cusps are numbered according to SnapPea's internal numbering "
        "(see the <i>Cusp #</i> column).  This table also gives the "
        "corresponding vertex number (using Regina's numbering, as seen "
        "in the <i>Skeleton</i> tab).<p>"
        "You can change the filling on each cusp by typing new "
        "filling coefficients directly into the table.</qt>"));
    cusps->setModel(model);
    cusps->header()->resizeSections(QHeaderView::ResizeToContents);
    layout->addWidget(cusps, 1);

    label = new QLabel(tr("Tetrahedron shapes:"));
    layout->addWidget(label);

    shapes = new QTreeWidget();
    shapes->setRootIsDecorated(false);
    shapes->setAlternatingRowColors(true);
    shapes->header()->setStretchLastSection(false);
    shapes->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
    actCanonise->setIcon(ReginaSupport::regIcon("canonical"));
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

SnapPeaShapesUI::~SnapPeaShapesUI() {
    delete model;
}

const QLinkedList<QAction*>& SnapPeaShapesUI::getPacketTypeActions() {
    return triActionList;
}

void SnapPeaShapesUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actRandomise);
    bar->addAction(actFill);
    bar->addAction(actToRegina);
}

regina::Packet* SnapPeaShapesUI::getPacket() {
    return tri;
}

QWidget* SnapPeaShapesUI::getInterface() {
    return ui;
}

void SnapPeaShapesUI::refresh() {
    // Rebuild the cusps table.
    model->rebuild();

    // Rebuild the shapes table.
    shapes->clear();

    QTreeWidgetItem *row, *header;

    shapes->setColumnCount(3);
    header = new QTreeWidgetItem();
    header->setText(0, tr("Tet #"));
    header->setText(1, tr("Real"));
    header->setText(2, tr("Imag"));
    header->setToolTip(0, tr("The tetrahedron number"));
    header->setToolTip(1, tr("The real part of the complex shape parameter"));
    header->setToolTip(2, tr("The imaginary part of the complex shape parameter"));
    header->setTextAlignment(0, Qt::AlignCenter);
    header->setTextAlignment(1, Qt::AlignCenter);
    header->setTextAlignment(2, Qt::AlignCenter);
    shapes->setHeaderItem(header);

    if (tri->isNull()) {
        updateNonNullActions();
        return;
    }
    if (tri->solutionType() == SnapPeaTriangulation::not_attempted ||
            tri->solutionType() == SnapPeaTriangulation::no_solution) {
        updateNonNullActions();
        return;
    }

    std::complex<double> s;
    for (unsigned i = 0; i < tri->size(); ++i) {
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
}

void SnapPeaShapesUI::endEdit() {
    cusps->endEdit();
}

void SnapPeaShapesUI::setReadWrite(bool readWrite) {
    // Regardless of whether we allow edits, we can do nothing with a
    // null triangulation.
    if (tri->isNull())
        readWrite = false;

    QLinkedListIterator<QAction*> it(enableWhenWritable);
    while (it.hasNext())
        (it.next())->setEnabled(readWrite);

    updateNonNullActions();
}

void SnapPeaShapesUI::vertexLinks() {
    endEdit();

    if (tri->countVertices() == 0)
        ReginaSupport::sorry(ui,
            tr("This triangulation does not have any vertices."));
    else {
        regina::Vertex<3>* chosen =
            FaceDialog<3, 0>::choose(ui, tri, 0 /* filter */,
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
            regina::Triangulation<2>* ans = new regina::Triangulation<2>(
                *chosen->buildLink());
            ans->setLabel(tr("Link of vertex %1").arg(chosen->index()).
                toUtf8().constData());
            tri->insertChildLast(ans);
            enclosingPane->getMainWindow()->packetView(ans, true, true);
        }
    }
}

void SnapPeaShapesUI::toRegina() {
    endEdit();

    if (tri->isNull())
        ReginaSupport::sorry(ui,
            tr("This is a null triangulation: there is no SnapPea "
            "triangulation for me to convert."));
    else {
        regina::Triangulation<3>* ans = new regina::Triangulation<3>(*tri);
        ans->setLabel(tri->label());
        tri->insertChildLast(ans);
        enclosingPane->getMainWindow()->packetView(ans, true, true);
    }
}

void SnapPeaShapesUI::fill() {
    endEdit();

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
        regina::Triangulation<3>* ans;
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
            ans->setLabel(tri->adornedLabel("Filled"));
            tri->insertChildLast(ans);
            enclosingPane->getMainWindow()->packetView(ans, true, true);
        }
    }
}

void SnapPeaShapesUI::randomise() {
    endEdit();

    tri->randomize();
}

void SnapPeaShapesUI::canonise() {
    endEdit();

    if (tri->isNull())
        ReginaSupport::sorry(ui,
            tr("This is a null triangulation: there is no SnapPea "
            "triangulation for me to canonise."));
    else {
        regina::Triangulation<3>* ans = tri->canonise();
        if (! ans) {
            ReginaSupport::sorry(ui,
                tr("The SnapPea kernel was not able to build the "
                "canonical retriangulation of the "
                "canonical cell decomposition."));
        } else {
            ans->setLabel(tr("Canonical retriangulation").
                toUtf8().constData());
            tri->insertChildLast(ans);
            enclosingPane->getMainWindow()->packetView(ans, true, true);
        }
    }
}

void SnapPeaShapesUI::updateNonNullActions() {
    if (tri->isNull()) {
        QLinkedListIterator<QAction*> it(requiresNonNull);
        while (it.hasNext())
            (it.next())->setEnabled(false);
    }
}

