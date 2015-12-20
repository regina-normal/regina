
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

#include "dim2/dim2triangulation.h"
#include "dim4/dim4triangulation.h"
#include "triangulation/ntriangulation.h"

#include "skeletonwindow.h"
#include "../packetui.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLayout>
#include <QPainter>
#include <QScrollBar>
#include <QStyle>

using regina::NBoundaryComponent;
using regina::NComponent;
using regina::NEdge;
using regina::NTriangle;
using regina::NTriangleEmbedding;
using regina::NVertex;
using regina::Dim2BoundaryComponent;
using regina::Dim2Component;
using regina::Dim2Edge;
using regina::Dim2EdgeEmbedding;
using regina::Dim2Vertex;
using regina::Dim4BoundaryComponent;
using regina::Dim4Component;
using regina::Dim4Edge;
using regina::Dim4Tetrahedron;
using regina::Dim4TetrahedronEmbedding;
using regina::Dim4Triangle;
using regina::Dim4Vertex;

#define SKELETON_MAX_ROWS_DEFAULT 10
namespace {
    inline QString& appendToList(QString& list,
            const QString& item) {
        return (list.isEmpty() ? (list = item) :
            (list.append(", ").append(item)));
    }
}

QSize SkeletonTreeView::sizeHint() const {
    int preferredWidth = header()->length();

    // Wide, but not insanely wide.
    int desktopWidth = QApplication::desktop()->availableGeometry().width();
    if (preferredWidth > desktopWidth / 2)
        preferredWidth = desktopWidth / 2;

    // Can we fit all the rows?
    int preferredHeight;

    int rows = model()->rowCount();
    int headerHeight = header()->sizeHint().height();
    int rowHeight = sizeHintForRow(0);
    if (rows <= SKELETON_MAX_ROWS_DEFAULT) {
        preferredHeight = rowHeight * rows + headerHeight;
    } else {
        preferredHeight = rowHeight * SKELETON_MAX_ROWS_DEFAULT +
            headerHeight + horizontalScrollBar()->height();

        preferredWidth += verticalScrollBar()->width();
    }

    preferredWidth += 2 * frameWidth();
    preferredHeight += 2 * frameWidth();

    return QSize(preferredWidth, preferredHeight);
}

SkeletonWindow::SkeletonWindow(PacketUI* packetUI,
        SkeletalModel* useModel) :
        QDialog(packetUI->getInterface()), model(useModel) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // Set up the table of data.
    table = new SkeletonTreeView();
    table->setItemsExpandable(false);
    table->setRootIsDecorated(false);
    table->setAlternatingRowColors(true);
    table->header()->setStretchLastSection(false);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setWhatsThis(model->overview());
    // Add grid lines:
    table->setStyleSheet("QTreeView::item { "
                             "border: 1px solid #d9d9d9; "
                             "border-top-color: transparent;"
                             "border-left-color: transparent;"
                         "}");
    /*
    table->header()->setStyleSheet("QHeaderView::section:last { "
                                       "border: 1px solid #d9d9d9; "
                                   "}");
    */
    table->setModel(model);
    layout->addWidget(table, 1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    layout->addWidget(buttonBox);

    refresh();

    // Resize columns now that the table is full of data.
    table->header()->resizeSections(QHeaderView::ResizeToContents);

    packetUI->getPacket()->listen(this);

    // Only one button to press (Close).
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(accept()));
}

void SkeletonWindow::refresh() {
    updateCaption();
    model->rebuild();
    table->header()->resizeSections(QHeaderView::ResizeToContents);
}

void SkeletonWindow::packetWasChanged(regina::NPacket*) {
    refresh();
}

void SkeletonWindow::packetWasRenamed(regina::NPacket*) {
    updateCaption();
}

void SkeletonWindow::packetToBeDestroyed(regina::NPacket*) {
    accepted();
}

QString VertexModel::caption() const {
    return tr("Vertices (%1)").arg(tri->getPacketLabel().c_str());
}

QString VertexModel::overview() const {
    return tr("<qt>Displays details of each "
        "vertex of this triangulation.<p>"
        "The different vertices are numbered from 0 upwards.  "
        "Each row describes properties of the vertex as well as "
        "listing precisely which vertices of which tetrahedra it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int VertexModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfVertices());
}

int VertexModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant VertexModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        NVertex* item = tri->getVertex(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1: {
                int link = item->getLink();
                if (link == NVertex::SPHERE)
                    return QString();
                if (link == NVertex::DISC)
                    return tr("Bdry");
                if (link == NVertex::TORUS)
                    return tr("Cusp (torus)");
                if (link == NVertex::KLEIN_BOTTLE)
                    return tr("Cusp (Klein bottle)");
                if (link == NVertex::NON_STANDARD_CUSP) {
                    if (item->isLinkOrientable())
                        return tr("Cusp (orbl, genus %1)").arg(
                            1 - (item->getLinkEulerChar() / 2));
                    else
                        return tr("Cusp (non-or, genus %1)").arg(
                            2 - item->getLinkEulerChar());
                }
                if (link == NVertex::NON_STANDARD_BDRY)
                    return tr("Non-std bdry");
                return QString();
            }
            case 2:
                return static_cast<unsigned>(item->getDegree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.getTetrahedron()->index()).
                        arg(emb.getVertex()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant VertexModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Vertex #");
            case 1: return tr("Type");
            case 2: return tr("Degree");
            case 3: return tr("Tetrahedra (Tet vertices)");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString VertexModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual vertex.  "
            "Vertices are numbered 0,1,2,...,<i>v</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the vertex, "
            "such as whether this is a cusp or a boundary vertex.</qt>");
        case 2: return tr("<qt>Gives the degree of this vertex, i.e., "
            "the number of individual tetrahedron vertices that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual tetrahedron vertices "
            "that come together to form this vertex of the "
            "triangulation.</qt>");
        default: return QString();
    }
}

QString EdgeModel::caption() const {
    return tr("Edges (%1)").arg(tri->getPacketLabel().c_str());
}

QString EdgeModel::overview() const {
    return tr("<qt>Displays details of each edge of "
        "this triangulation.<p>"
        "The different edges are numbered from 0 upwards.  "
        "Each row describes properties of the edge as well as "
        "listing precisely which vertices of which tetrahedra it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int EdgeModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfEdges());
}

int EdgeModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant EdgeModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        NEdge* item = tri->getEdge(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                if (! item->isValid())
                    return tr("Invalid");
                else if (item->isBoundary())
                    return tr("Bdry");
                else
                    return QString();
            case 2:
                return static_cast<unsigned>(item->getDegree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.getTetrahedron()->index()).
                        arg(emb.getVertices().trunc2().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant EdgeModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Edge #");
            case 1: return tr("Type");
            case 2: return tr("Degree");
            case 3: return tr("Tetrahedra (Tet vertices)");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString EdgeModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual edge.  "
            "Edges are numbered 0,1,2,...,<i>e</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the edge, "
            "such as whether it lies on the boundary or is invalid.</qt>");
        case 2: return tr("<qt>Gives the degree of this edge, i.e., "
            "the number of individual tetrahedron edges that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual tetrahedron edges "
            "that come together to form this edge of the triangulation.</qt>");
        default: return QString();
    }
}

QString TriangleModel::caption() const {
    return tr("Triangles (%1)").arg(tri->getPacketLabel().c_str());
}

QString TriangleModel::overview() const {
    return tr("<qt>Displays details of each "
        "triangle of this triangulation.<p>"
        "The different triangles are numbered from 0 upwards.  "
        "Each row describes the shape of the triangle as well as "
        "listing precisely which vertices of which tetrahedra it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int TriangleModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfTriangles());
}

int TriangleModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant TriangleModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        NTriangle* item = tri->getTriangle(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1: {
                QString prefix;
                if (item->isBoundary())
                    prefix = tr("(Bdry) ");

                int type = item->getType();
                if (type == NTriangle::TRIANGLE)
                    return prefix + tr("Triangle");
                if (type == NTriangle::SCARF)
                    return prefix + tr("Scarf");
                if (type == NTriangle::PARACHUTE)
                    return prefix + tr("Parachute");
                if (type == NTriangle::MOBIUS)
                    return prefix + trUtf8("Möbius band");
                if (type == NTriangle::CONE)
                    return prefix + tr("Cone");
                if (type == NTriangle::HORN)
                    return prefix + tr("Horn");
                if (type == NTriangle::DUNCEHAT)
                    return prefix + tr("Dunce hat");
                if (type == NTriangle::L31)
                    return prefix + tr("L(3,1)");
                return prefix + tr("UNKNOWN");
            }
            case 2:
                return static_cast<unsigned>(item->getDegree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.getTetrahedron()->index()).
                        arg(emb.getVertices().trunc3().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant TriangleModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Triangle #");
            case 1: return tr("Type");
            case 2: return tr("Degree");
            case 3: return tr("Tetrahedra (Tet vertices)");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString TriangleModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual triangle.  "
            "Triangles are numbered 0,1,2,...,<i>t</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the triangle, "
            "such as the shape that it forms and whether it lies on the "
            "boundary.</qt>");
        case 2: return tr("<qt>Gives the degree of this triangle, i.e., "
            "the number of individual tetrahedron faces that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual tetrahedron faces "
            "that come together to form this triangle.</qt>");
        default: return QString();
    }
}

QString ComponentModel::caption() const {
    return tr("Components (%1)").arg(tri->getPacketLabel().c_str());
}

QString ComponentModel::overview() const {
    return tr("<qt>Displays details of each "
        "connected component of this triangulation.<p>"
        "The different components are numbered from 0 upwards.  "
        "Each row describes properties of the component as well as "
        "listing precisely which tetrahedra the component contains.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int ComponentModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfComponents());
}

int ComponentModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant ComponentModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        NComponent* item = tri->getComponent(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->isIdeal() ? tr("Ideal, ") : tr("Real, ")) +
                    (item->isOrientable() ? tr("Orbl") : tr("Non-orbl"));
            case 2:
                return static_cast<unsigned>(item->getNumberOfTetrahedra());
            case 3:
                QString ans;
                for (unsigned long i = 0; i < item->getNumberOfTetrahedra();
                        i++)
                    appendToList(ans, QString::number(
                        item->getTetrahedron(i)->index()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant ComponentModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Cmpt #");
            case 1: return tr("Type");
            case 2: return tr("Size");
            case 3: return tr("Tetrahedra");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString ComponentModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual component.  "
            "Components are numbered 0,1,2,...,<i>c</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the component, "
            "such as its orientability or whether it contains ideal "
            "vertices.</qt>");
        case 2: return tr("<qt>Gives the size of this component, i.e., "
            "the number of tetrahedra that it contains.</qt>");
        case 3: return tr("<qt>Identifies the individual tetrahedra "
            "that belong to this component.</qt>");
        default: return QString();
    }
}

QString BoundaryComponentModel::caption() const {
    return tr("Boundary Components (%1)").arg(tri->getPacketLabel().c_str());
}

QString BoundaryComponentModel::overview() const {
    return tr("<qt>Displays details of each "
        "boundary component of this triangulation.  A boundary "
        "component may be a collection of adjacent boundary triangles, "
        "or it may be a single ideal vertex, whose link is closed but "
        "not a 2-sphere.<p>"
        "The different boundary components are numbered from 0 upwards.  "
        "Each row describes properties of the boundary component, as "
        "well as which tetrahedron faces (for a real boundary component) "
        "or which tetrahedron vertices (for an ideal boundary component) "
        "it is formed from.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int BoundaryComponentModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfBoundaryComponents());
}

int BoundaryComponentModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant BoundaryComponentModel::data(const QModelIndex& index,
        int role) const {
    if (role == Qt::DisplayRole) {
        NBoundaryComponent* item = tri->getBoundaryComponent(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->isIdeal() ? tr("Ideal") : tr("Real"));
            case 2:
                // Note that we can't have just one triangle
                // (by a parity argument).
                return (item->isIdeal() ?
                    tr("Degree %1").arg(item->getVertex(0)->getDegree()) :
                    tr("%1 triangles").arg(item->getNumberOfTriangles()));
            case 3:
                if (item->isIdeal()) {
                    NVertex* v = item->getVertex(0);
                    QString ans;
                    for (auto& emb : *v)
                        appendToList(ans, QString("%1 (%2)").
                            arg(emb.getTetrahedron()->index()).
                            arg(emb.getVertex()));
                    return tr("Vertex %1 = ").arg(v->index()) + ans;
                } else {
                    QString ans;
                    for (unsigned long i = 0;
                            i < item->getNumberOfTriangles(); ++i) {
                        const NTriangleEmbedding& emb =
                            item->getTriangle(i)->front();
                        appendToList(ans, QString("%1 (%2)").
                            arg(emb.getTetrahedron()->index()).
                            arg(emb.getVertices().trunc3().c_str()));
                    }
                    return ans;
                }
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant BoundaryComponentModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Cmpt #");
            case 1: return tr("Type");
            case 2: return tr("Size");
            case 3: return tr("Triangles / Vertex");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString BoundaryComponentModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual boundary "
            "component.  "
            "Boundary components are numbered 0,1,2,...,<i>b</i>-1.</qt>");
        case 1: return tr("<qt>Lists whether this is an ideal or real "
            "boundary component.</qt>");
        case 2: return tr("<qt>Gives the size of this boundary component, "
            "i.e., the number of triangles (for a real boundary component) "
            "or the degree of the vertex (for an ideal "
            "boundary component).</qt>");
        case 3: return tr("<qt>For a real boundary component, this column "
            "lists the individual triangles that it contains.<p>"
            "For an ideal boundary component, this column "
            "lists the individual tetrahedron vertices that are identified "
            "to form the overall vertex of the triangulation.</qt>");
        default: return QString();
    }
}

QString Dim2VertexModel::caption() const {
    return tr("Vertices (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim2VertexModel::overview() const {
    return tr("<qt>Displays details of each "
        "vertex of this triangulation.<p>"
        "The different vertices are numbered from 0 upwards.  "
        "Each row describes properties of the vertex as well as "
        "listing precisely which vertices of which triangles it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim2VertexModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfVertices());
}

int Dim2VertexModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Dim2VertexModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Dim2Vertex* item = tri->getVertex(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                if (item->isBoundary())
                    return tr("Bdry");
                else
                    return QString();
            case 2:
                return static_cast<unsigned>(item->getDegree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.getTriangle()->index()).
                        arg(emb.getVertex()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim2VertexModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Vertex #");
            case 1: return tr("Type");
            case 2: return tr("Degree");
            case 3: return tr("Triangles (Triangle vertices)");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim2VertexModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual vertex.  "
            "Vertices are numbered 0,1,2,...,<i>v</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the vertex, "
            "such as whether it lies on the boundary.</qt>");
        case 2: return tr("<qt>Gives the degree of this vertex, i.e., "
            "the number of individual triangle vertices that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual triangle vertices "
            "that come together to form this vertex of the "
            "triangulation.</qt>");
        default: return QString();
    }
}

QString Dim2EdgeModel::caption() const {
    return tr("Edges (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim2EdgeModel::overview() const {
    return tr("<qt>Displays details of each edge of "
        "this triangulation.<p>"
        "The different edges are numbered from 0 upwards.  "
        "Each row describes properties of the edge as well as "
        "listing precisely which vertices of which triangles it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim2EdgeModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfEdges());
}

int Dim2EdgeModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Dim2EdgeModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Dim2Edge* item = tri->getEdge(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                if (item->isBoundary())
                    return tr("Bdry");
                else
                    return QString();
            case 2:
                return static_cast<unsigned>(item->getDegree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.getTriangle()->index()).
                        arg(emb.getVertices().trunc2().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim2EdgeModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Edge #");
            case 1: return tr("Type");
            case 2: return tr("Degree");
            case 3: return tr("Triangles (Triangle vertices)");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim2EdgeModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual edge.  "
            "Edges are numbered 0,1,2,...,<i>e</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the edge, "
            "such as whether it lies on the boundary.</qt>");
        case 2: return tr("<qt>Gives the degree of this edge, i.e., "
            "the number of individual triangle edges that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual triangle edges "
            "that come together to form this edge of the triangulation.</qt>");
        default: return QString();
    }
}

QString Dim2ComponentModel::caption() const {
    return tr("Components (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim2ComponentModel::overview() const {
    return tr("<qt>Displays details of each "
        "connected component of this triangulation.<p>"
        "The different components are numbered from 0 upwards.  "
        "Each row describes properties of the component as well as "
        "listing precisely which triangles the component contains.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim2ComponentModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfComponents());
}

int Dim2ComponentModel::columnCount(const QModelIndex& /* unused */) const {
    return 4;
}

QVariant Dim2ComponentModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Dim2Component* item = tri->getComponent(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->isOrientable() ? tr("Orbl") : tr("Non-orbl"));
            case 2:
                return static_cast<unsigned>(item->getNumberOfTriangles());
            case 3:
                QString ans;
                for (unsigned long i = 0; i < item->getNumberOfTriangles(); ++i)
                    appendToList(ans, QString::number(
                        item->getTriangle(i)->index()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim2ComponentModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Cmpt #");
            case 1: return tr("Type");
            case 2: return tr("Size");
            case 3: return tr("Triangles");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim2ComponentModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual component.  "
            "Components are numbered 0,1,2,...,<i>c</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the component, "
            "such as its orientability.</qt>");
        case 2: return tr("<qt>Gives the size of this component, i.e., "
            "the number of triangles that it contains.</qt>");
        case 3: return tr("<qt>Identifies the individual triangles "
            "that belong to this component.</qt>");
        default: return QString();
    }
}

QString Dim2BoundaryComponentModel::caption() const {
    return tr("Boundary Components (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim2BoundaryComponentModel::overview() const {
    return tr("<qt>Displays details of each "
        "boundary component of this triangulation.<p>"
        "The different boundary components are numbered from 0 upwards.  "
        "Each row describes properties of the boundary component, as "
        "well as which triangle edges it is formed from.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim2BoundaryComponentModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfBoundaryComponents());
}

int Dim2BoundaryComponentModel::columnCount(const QModelIndex& /* unused */)
        const {
    return 3;
}

QVariant Dim2BoundaryComponentModel::data(const QModelIndex& index,
        int role) const {
    if (role == Qt::DisplayRole) {
        Dim2BoundaryComponent* item = tri->getBoundaryComponent(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->getNumberOfEdges() == 1 ? tr("1 edge") :
                    tr("%1 edges").arg(item->getNumberOfEdges()));
            case 2:
                QString ans;
                for (unsigned long i = 0; i < item->getNumberOfEdges(); ++i) {
                    const Dim2EdgeEmbedding& emb = item->getEdge(i)->front();
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.getTriangle()->index()).
                        arg(emb.getVertices().trunc2().c_str()));
                }
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim2BoundaryComponentModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Cmpt #");
            case 1: return tr("Size");
            case 2: return tr("Edges");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim2BoundaryComponentModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual boundary "
            "component.  "
            "Boundary components are numbered 0,1,2,...,<i>b</i>-1.</qt>");
        case 1: return tr("<qt>Gives the size of this boundary component, "
            "i.e., the number of edges it is formed from.</qt>");
        case 2: return tr("<qt>Identifies the individual edges that "
            "this boundary component contains.</qt>");
        default: return QString();
    }
}

QString Dim4VertexModel::caption() const {
    return tr("Vertices (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim4VertexModel::overview() const {
    return tr("<qt>Displays details of each "
        "vertex of this triangulation.<p>"
        "The different vertices are numbered from 0 upwards.  "
        "Each row describes properties of the vertex as well as "
        "listing precisely which vertices of which pentachora it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim4VertexModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfVertices());
}

int Dim4VertexModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Dim4VertexModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Dim4Vertex* item = tri->getVertex(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1: {
                if (item->isIdeal())
                    return tr("Ideal");
                else if (! item->isValid())
                    return tr("Invalid");
                else if (item->isBoundary())
                    return tr("Bdry");
                else
                    return QString();
            }
            case 2:
                return static_cast<unsigned>(item->degree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.getPentachoron()->index()).
                        arg(emb.getVertex()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim4VertexModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Vertex #");
            case 1: return tr("Type");
            case 2: return tr("Degree");
            case 3: return tr("Pentachora (Pent vertices)");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim4VertexModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual vertex.  "
            "Vertices are numbered 0,1,2,...,<i>v</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the vertex, "
            "such as whether this is an ideal or boundary vertex.</qt>");
        case 2: return tr("<qt>Gives the degree of this vertex, i.e., "
            "the number of individual pentachoron vertices that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual pentachoron vertices "
            "that come together to form this vertex of the "
            "triangulation.</qt>");
        default: return QString();
    }
}

QString Dim4EdgeModel::caption() const {
    return tr("Edges (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim4EdgeModel::overview() const {
    return tr("<qt>Displays details of each edge of "
        "this triangulation.<p>"
        "The different edges are numbered from 0 upwards.  "
        "Each row describes properties of the edge as well as "
        "listing precisely which edges of which pentachora it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim4EdgeModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfEdges());
}

int Dim4EdgeModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Dim4EdgeModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Dim4Edge* item = tri->getEdge(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                if (! item->isValid()) {
                    if (item->hasBadIdentification())
                        return tr("Invalid (reverse gluing)");
                    else if (item->hasBadLink())
                        return tr("Invalid (bad link)");
                    else // should never happen
                        return tr("Invalid (unknown reason)");
                }
                else if (item->isBoundary())
                    return tr("Bdry");
                else
                    return QString();
            case 2:
                return static_cast<unsigned>(item->degree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.getPentachoron()->index()).
                        arg(emb.getVertices().trunc2().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim4EdgeModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Edge #");
            case 1: return tr("Type");
            case 2: return tr("Degree");
            case 3: return tr("Pentachora (Pent vertices)");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim4EdgeModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual edge.  "
            "Edges are numbered 0,1,2,...,<i>e</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the edge, "
            "such as whether it lies on the boundary or is invalid.</qt>");
        case 2: return tr("<qt>Gives the degree of this edge, i.e., "
            "the number of individual pentachoron edges that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual pentachoron edges "
            "that come together to form this edge of the triangulation.</qt>");
        default: return QString();
    }
}

QString Dim4TriangleModel::caption() const {
    return tr("Triangles (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim4TriangleModel::overview() const {
    return tr("<qt>Displays details of each triangle of "
        "this triangulation.<p>"
        "The different triangles are numbered from 0 upwards.  "
        "Each row describes properties of the triangle as well as "
        "listing precisely which vertices of which pentachora it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim4TriangleModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfTriangles());
}

int Dim4TriangleModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Dim4TriangleModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Dim4Triangle* item = tri->getTriangle(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                if (! item->isValid())
                    return tr("Invalid");
                else if (item->isBoundary())
                    return tr("Bdry");
                else
                    return QString();
            case 2:
                return static_cast<unsigned>(item->degree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.getPentachoron()->index()).
                        arg(emb.getVertices().trunc3().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim4TriangleModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Triangle #");
            case 1: return tr("Type");
            case 2: return tr("Degree");
            case 3: return tr("Pentachora (Pent vertices)");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim4TriangleModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual triangle.  "
            "Triangles are numbered 0,1,2,...,<i>t</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the triangle, "
            "such as whether it lies on the boundary or is invalid.</qt>");
        case 2: return tr("<qt>Gives the degree of this triangle, i.e., "
            "the number of individual pentachoron triangles that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual pentachoron triangles "
            "that come together to form this triangle of the "
            "triangulation.</qt>");
        default: return QString();
    }
}

QString Dim4TetrahedronModel::caption() const {
    return tr("Tetrahedra (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim4TetrahedronModel::overview() const {
    return tr("<qt>Displays details of each "
        "tetrahedron of this triangulation.<p>"
        "The different tetrahedra are numbered from 0 upwards.  "
        "Each row describes properties of the tetrahedron as well as "
        "listing precisely which vertices of which pentachora it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim4TetrahedronModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfTetrahedra());
}

int Dim4TetrahedronModel::columnCount(const QModelIndex& /* unused parent*/)
        const {
    return 4;
}

QVariant Dim4TetrahedronModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Dim4Tetrahedron* item = tri->getTetrahedron(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1: {
                QString prefix;
                if (item->isBoundary())
                    return tr("Bdry");
                return QString();
            }
            case 2:
                return static_cast<unsigned>(item->degree());
            case 3:
                QString ans;
                for (unsigned i = 0; i < item->degree(); i++)
                    appendToList(ans, QString("%1 (%2)").
                        arg(item->getEmbedding(i).getPentachoron()->index()).
                        arg(item->getEmbedding(i).getVertices().
                            trunc4().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim4TetrahedronModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Tet #");
            case 1: return tr("Type");
            case 2: return tr("Degree");
            case 3: return tr("Pentachora (Pent vertices)");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim4TetrahedronModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual tetrahedron.  "
            "Tetrahedra are numbered 0,1,2,...,<i>t</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the tetrahedron, "
            "such as whether it lies on the boundary.</qt>");
        case 2: return tr("<qt>Gives the degree of this tetrahedron, i.e., "
            "the number of individual pentachoron facets that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual pentachoron facets "
            "that come together to form this tetrahedron "
            "of the triangulation.</qt>");
        default: return QString();
    }
}

QString Dim4ComponentModel::caption() const {
    return tr("Components (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim4ComponentModel::overview() const {
    return tr("<qt>Displays details of each "
        "connected component of this triangulation.<p>"
        "The different components are numbered from 0 upwards.  "
        "Each row describes properties of the component as well as "
        "listing precisely which pentachora the component contains.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim4ComponentModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfComponents());
}

int Dim4ComponentModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Dim4ComponentModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Dim4Component* item = tri->getComponent(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->isIdeal() ? tr("Ideal, ") : tr("Real, ")) +
                    (item->isOrientable() ? tr("Orbl") : tr("Non-orbl"));
            case 2:
                return static_cast<unsigned>(item->getNumberOfSimplices());
            case 3:
                QString ans;
                for (unsigned long i = 0; i < item->getNumberOfSimplices(); i++)
                    appendToList(ans, QString::number(
                        item->simplex(i)->index()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim4ComponentModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Cmpt #");
            case 1: return tr("Type");
            case 2: return tr("Size");
            case 3: return tr("Pentachora");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim4ComponentModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual component.  "
            "Components are numbered 0,1,2,...,<i>c</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the component, "
            "such as its orientability or whether it contains ideal "
            "vertices.</qt>");
        case 2: return tr("<qt>Gives the size of this component, i.e., "
            "the number of pentachora that it contains.</qt>");
        case 3: return tr("<qt>Identifies the individual pentachora "
            "that belong to this component.</qt>");
        default: return QString();
    }
}

QString Dim4BoundaryComponentModel::caption() const {
    return tr("Boundary Components (%1)").arg(tri->getPacketLabel().c_str());
}

QString Dim4BoundaryComponentModel::overview() const {
    return tr("<qt>Displays details of each "
        "boundary component of this triangulation.  A boundary "
        "component may be: (i) a collection of adjacent boundary tetrahedra; "
        "(ii) a single ideal vertex, whose link is closed but "
        "not a 3-sphere; or "
        "(iii) a single invalid vertex, which does not belong to any boundary "
        "tetrahedra but whose link is not a valid closed 3-manifold.<p>"
        "The different boundary components are numbered from 0 upwards.  "
        "Each row describes properties of the boundary component, as "
        "well as which pentachoron facets (for a real boundary component) "
        "or which pentachoron vertices (for an ideal boundary component or "
        "an invalid vertex) it is formed from.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Dim4BoundaryComponentModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfBoundaryComponents());
}

int Dim4BoundaryComponentModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Dim4BoundaryComponentModel::data(const QModelIndex& index,
        int role) const {
    if (role == Qt::DisplayRole) {
        Dim4BoundaryComponent* item = tri->getBoundaryComponent(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->isIdeal() ? tr("Ideal") :
                    item->isInvalidVertex() ? tr("Invalid vertex") :
                    tr("Real"));
            case 2:
                return ((item->isIdeal() || item->isInvalidVertex()) ?
                    tr("Degree %1").arg(item->getVertex(0)->getDegree()):
                    item->getNumberOfTetrahedra() == 1 ? tr("1 tetrahedron") :
                    tr("%1 tetrahedra").arg(item->getNumberOfTetrahedra()));
            case 3:
                if (item->isIdeal() || item->isInvalidVertex()) {
                    Dim4Vertex* v = item->getVertex(0);
                    QString ans;
                    for (auto& emb : *v)
                        appendToList(ans, QString("%1 (%2)").
                            arg(emb.getPentachoron()->index()).
                            arg(emb.getVertex()));
                    return tr("Vertex %1 = ").arg(v->index()) + ans;
                } else {
                    QString ans;
                    for (unsigned long i = 0;
                            i < item->getNumberOfTetrahedra(); i++) {
                        const Dim4TetrahedronEmbedding& emb =
                            item->getTetrahedron(i)->front();
                        appendToList(ans, QString("%1 (%2)").
                            arg(emb.getPentachoron()->index()).
                            arg(emb.getVertices().trunc4().c_str()));
                    }
                    return ans;
                }
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Dim4BoundaryComponentModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Cmpt #");
            case 1: return tr("Type");
            case 2: return tr("Size");
            case 3: return tr("Tetrahedra / Vertex");
            default: return QString();
        }
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(section);
    } else
        return QVariant();
}

QString Dim4BoundaryComponentModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual boundary "
            "component.  "
            "Boundary components are numbered 0,1,2,...,<i>b</i>-1.</qt>");
        case 1: return tr("<qt>Lists whether this is an ideal or real "
            "boundary component, or an invalid vertex.</qt>");
        case 2: return tr("<qt>Gives the size of this boundary component, "
            "i.e., the number of tetrahedra (for a real boundary component) "
            "or the degree of the vertex (for an ideal "
            "boundary component or an invalid vertex).</qt>");
        case 3: return tr("<qt>For a real boundary component, this column "
            "lists the individual tetrahedra that it contains.<p>"
            "For an ideal boundary component or an invalid vertex, this column "
            "lists the individual pentachoron vertices that are identified "
            "to form the overall vertex of the triangulation.</qt>");
        default: return QString();
    }
}

