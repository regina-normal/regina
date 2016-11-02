
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

#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

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

using regina::BoundaryComponent;
using regina::Component;
using regina::Edge;
using regina::EdgeEmbedding;
using regina::Tetrahedron;
using regina::TetrahedronEmbedding;
using regina::Triangle;
using regina::TriangleEmbedding;
using regina::Vertex;

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

void SkeletonWindow::packetWasChanged(regina::Packet*) {
    refresh();
}

void SkeletonWindow::packetWasRenamed(regina::Packet*) {
    updateCaption();
}

void SkeletonWindow::packetToBeDestroyed(regina::Packet*) {
    accepted();
}

QString Vertex3Model::caption() const {
    return tr("Vertices (%1)").arg(tri->label().c_str());
}

QString Vertex3Model::overview() const {
    return tr("<qt>Displays details of each "
        "vertex of this triangulation.<p>"
        "The different vertices are numbered from 0 upwards.  "
        "Each row describes properties of the vertex as well as "
        "listing precisely which vertices of which tetrahedra it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Vertex3Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countVertices());
}

int Vertex3Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Vertex3Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Vertex<3>* item = tri->vertex(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1: {
                switch (item->link()) {
                    case Vertex<3>::SPHERE:
                        return QString();
                    case Vertex<3>::DISC:
                        return tr("Bdry");
                    case Vertex<3>::TORUS:
                        return tr("Ideal: Torus");
                    case Vertex<3>::KLEIN_BOTTLE:
                        return tr("Ideal: Klein bottle");
                    case Vertex<3>::NON_STANDARD_CUSP: {
                        if (item->isLinkOrientable())
                            return tr("Ideal: Genus %1 orbl").arg(
                                1 - (item->linkEulerChar() / 2));
                        else
                            return tr("Ideal: Genus %1 non-orbl").arg(
                                2 - item->linkEulerChar());
                    }
                    case Vertex<3>::INVALID:
                        return tr("Invalid");
                    default:
                        return tr("Unknown");
                }
            }
            case 2:
                return static_cast<unsigned>(item->degree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.tetrahedron()->index()).
                        arg(emb.vertex()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Vertex3Model::headerData(int section,
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

QString Vertex3Model::toolTipForCol(int column) {
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

QString Edge3Model::caption() const {
    return tr("Edges (%1)").arg(tri->label().c_str());
}

QString Edge3Model::overview() const {
    return tr("<qt>Displays details of each edge of "
        "this triangulation.<p>"
        "The different edges are numbered from 0 upwards.  "
        "Each row describes properties of the edge as well as "
        "listing precisely which vertices of which tetrahedra it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Edge3Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countEdges());
}

int Edge3Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Edge3Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Edge<3>* item = tri->edge(index.row());
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
                        arg(emb.tetrahedron()->index()).
                        arg(emb.vertices().trunc2().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Edge3Model::headerData(int section,
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

QString Edge3Model::toolTipForCol(int column) {
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

QString Triangle3Model::caption() const {
    return tr("Triangles (%1)").arg(tri->label().c_str());
}

QString Triangle3Model::overview() const {
    return tr("<qt>Displays details of each "
        "triangle of this triangulation.<p>"
        "The different triangles are numbered from 0 upwards.  "
        "Each row describes the shape of the triangle as well as "
        "listing precisely which vertices of which tetrahedra it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Triangle3Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countTriangles());
}

int Triangle3Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Triangle3Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Triangle<3>* item = tri->triangle(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1: {
                QString prefix;
                if (item->isBoundary())
                    prefix = tr("(Bdry) ");

                int type = item->type();
                if (type == Triangle<3>::TRIANGLE)
                    return prefix + tr("Triangle");
                if (type == Triangle<3>::SCARF)
                    return prefix + tr("Scarf");
                if (type == Triangle<3>::PARACHUTE)
                    return prefix + tr("Parachute");
                if (type == Triangle<3>::MOBIUS)
                    return prefix + trUtf8("Möbius band");
                if (type == Triangle<3>::CONE)
                    return prefix + tr("Cone");
                if (type == Triangle<3>::HORN)
                    return prefix + tr("Horn");
                if (type == Triangle<3>::DUNCEHAT)
                    return prefix + tr("Dunce hat");
                if (type == Triangle<3>::L31)
                    return prefix + tr("L(3,1)");
                return prefix + tr("UNKNOWN");
            }
            case 2:
                return static_cast<unsigned>(item->degree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.tetrahedron()->index()).
                        arg(emb.vertices().trunc3().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Triangle3Model::headerData(int section,
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

QString Triangle3Model::toolTipForCol(int column) {
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

QString Component3Model::caption() const {
    return tr("Components (%1)").arg(tri->label().c_str());
}

QString Component3Model::overview() const {
    return tr("<qt>Displays details of each "
        "connected component of this triangulation.<p>"
        "The different components are numbered from 0 upwards.  "
        "Each row describes properties of the component as well as "
        "listing precisely which tetrahedra the component contains.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Component3Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countComponents());
}

int Component3Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Component3Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Component<3>* item = tri->component(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return ((! item->isValid()) ? tr("Invalid, ") :
                        item->isIdeal() ? tr("Ideal, ") : tr("Real, ")) +
                    (item->isOrientable() ? tr("orbl") : tr("non-orbl"));
            case 2:
                return static_cast<unsigned>(item->size());
            case 3:
                QString ans;
                for (unsigned long i = 0; i < item->size(); i++)
                    appendToList(ans, QString::number(
                        item->tetrahedron(i)->index()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Component3Model::headerData(int section,
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

QString Component3Model::toolTipForCol(int column) {
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

QString BoundaryComponent3Model::caption() const {
    return tr("Boundary Components (%1)").arg(tri->label().c_str());
}

QString BoundaryComponent3Model::overview() const {
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

int BoundaryComponent3Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countBoundaryComponents());
}

int BoundaryComponent3Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant BoundaryComponent3Model::data(const QModelIndex& index,
        int role) const {
    if (role == Qt::DisplayRole) {
        BoundaryComponent<3>* item = tri->boundaryComponent(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->isIdeal() ? tr("Ideal") : tr("Real"));
            case 2:
                // Note that we can't have just one triangle
                // (by a parity argument).
                return (item->isIdeal() ?
                    tr("Degree %1").arg(item->vertex(0)->degree()) :
                    tr("%1 triangles").arg(item->countTriangles()));
            case 3:
                if (item->isIdeal()) {
                    Vertex<3>* v = item->vertex(0);
                    QString ans;
                    for (auto& emb : *v)
                        appendToList(ans, QString("%1 (%2)").
                            arg(emb.tetrahedron()->index()).
                            arg(emb.vertex()));
                    return tr("Vertex %1 = ").arg(v->index()) + ans;
                } else {
                    QString ans;
                    for (unsigned long i = 0; i < item->countTriangles(); ++i) {
                        const TriangleEmbedding<3>& emb =
                            item->triangle(i)->front();
                        appendToList(ans, QString("%1 (%2)").
                            arg(emb.tetrahedron()->index()).
                            arg(emb.vertices().trunc3().c_str()));
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

QVariant BoundaryComponent3Model::headerData(int section,
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

QString BoundaryComponent3Model::toolTipForCol(int column) {
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

QString Vertex2Model::caption() const {
    return tr("Vertices (%1)").arg(tri->label().c_str());
}

QString Vertex2Model::overview() const {
    return tr("<qt>Displays details of each "
        "vertex of this triangulation.<p>"
        "The different vertices are numbered from 0 upwards.  "
        "Each row describes properties of the vertex as well as "
        "listing precisely which vertices of which triangles it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Vertex2Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countVertices());
}

int Vertex2Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Vertex2Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Vertex<2>* item = tri->vertex(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                if (item->isBoundary())
                    return tr("Bdry");
                else
                    return QString();
            case 2:
                return static_cast<unsigned>(item->degree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.triangle()->index()).
                        arg(emb.vertex()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Vertex2Model::headerData(int section,
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

QString Vertex2Model::toolTipForCol(int column) {
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

QString Edge2Model::caption() const {
    return tr("Edges (%1)").arg(tri->label().c_str());
}

QString Edge2Model::overview() const {
    return tr("<qt>Displays details of each edge of "
        "this triangulation.<p>"
        "The different edges are numbered from 0 upwards.  "
        "Each row describes properties of the edge as well as "
        "listing precisely which vertices of which triangles it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Edge2Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countEdges());
}

int Edge2Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Edge2Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Edge<2>* item = tri->edge(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                if (item->isBoundary())
                    return tr("Bdry");
                else
                    return QString();
            case 2:
                return static_cast<unsigned>(item->degree());
            case 3:
                QString ans;
                for (auto& emb : *item)
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.triangle()->index()).
                        arg(emb.vertices().trunc2().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Edge2Model::headerData(int section,
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

QString Edge2Model::toolTipForCol(int column) {
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

QString Component2Model::caption() const {
    return tr("Components (%1)").arg(tri->label().c_str());
}

QString Component2Model::overview() const {
    return tr("<qt>Displays details of each "
        "connected component of this triangulation.<p>"
        "The different components are numbered from 0 upwards.  "
        "Each row describes properties of the component as well as "
        "listing precisely which triangles the component contains.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Component2Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countComponents());
}

int Component2Model::columnCount(const QModelIndex& /* unused */) const {
    return 4;
}

QVariant Component2Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Component<2>* item = tri->component(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->isOrientable() ? tr("Orbl") : tr("Non-orbl"));
            case 2:
                return static_cast<unsigned>(item->size());
            case 3:
                QString ans;
                for (unsigned long i = 0; i < item->size(); ++i)
                    appendToList(ans, QString::number(
                        item->triangle(i)->index()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Component2Model::headerData(int section,
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

QString Component2Model::toolTipForCol(int column) {
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

QString BoundaryComponent2Model::caption() const {
    return tr("Boundary Components (%1)").arg(tri->label().c_str());
}

QString BoundaryComponent2Model::overview() const {
    return tr("<qt>Displays details of each "
        "boundary component of this triangulation.<p>"
        "The different boundary components are numbered from 0 upwards.  "
        "Each row describes properties of the boundary component, as "
        "well as which triangle edges it is formed from.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int BoundaryComponent2Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countBoundaryComponents());
}

int BoundaryComponent2Model::columnCount(const QModelIndex& /* unused */)
        const {
    return 3;
}

QVariant BoundaryComponent2Model::data(const QModelIndex& index,
        int role) const {
    if (role == Qt::DisplayRole) {
        BoundaryComponent<2>* item = tri->boundaryComponent(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->countEdges() == 1 ? tr("1 edge") :
                    tr("%1 edges").arg(item->countEdges()));
            case 2:
                QString ans;
                for (unsigned long i = 0; i < item->countEdges(); ++i) {
                    const EdgeEmbedding<2>& emb = item->edge(i)->front();
                    appendToList(ans, QString("%1 (%2)").
                        arg(emb.triangle()->index()).
                        arg(emb.vertices().trunc2().c_str()));
                }
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant BoundaryComponent2Model::headerData(int section,
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

QString BoundaryComponent2Model::toolTipForCol(int column) {
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

QString Vertex4Model::caption() const {
    return tr("Vertices (%1)").arg(tri->label().c_str());
}

QString Vertex4Model::overview() const {
    return tr("<qt>Displays details of each "
        "vertex of this triangulation.<p>"
        "The different vertices are numbered from 0 upwards.  "
        "Each row describes properties of the vertex as well as "
        "listing precisely which vertices of which pentachora it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Vertex4Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countVertices());
}

int Vertex4Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Vertex4Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Vertex<4>* item = tri->vertex(index.row());
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
                        arg(emb.pentachoron()->index()).
                        arg(emb.vertex()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Vertex4Model::headerData(int section,
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

QString Vertex4Model::toolTipForCol(int column) {
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

QString Edge4Model::caption() const {
    return tr("Edges (%1)").arg(tri->label().c_str());
}

QString Edge4Model::overview() const {
    return tr("<qt>Displays details of each edge of "
        "this triangulation.<p>"
        "The different edges are numbered from 0 upwards.  "
        "Each row describes properties of the edge as well as "
        "listing precisely which edges of which pentachora it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Edge4Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countEdges());
}

int Edge4Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Edge4Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Edge<4>* item = tri->edge(index.row());
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
                        arg(emb.pentachoron()->index()).
                        arg(emb.vertices().trunc2().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Edge4Model::headerData(int section,
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

QString Edge4Model::toolTipForCol(int column) {
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

QString Triangle4Model::caption() const {
    return tr("Triangles (%1)").arg(tri->label().c_str());
}

QString Triangle4Model::overview() const {
    return tr("<qt>Displays details of each triangle of "
        "this triangulation.<p>"
        "The different triangles are numbered from 0 upwards.  "
        "Each row describes properties of the triangle as well as "
        "listing precisely which vertices of which pentachora it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Triangle4Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countTriangles());
}

int Triangle4Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Triangle4Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Triangle<4>* item = tri->triangle(index.row());
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
                        arg(emb.pentachoron()->index()).
                        arg(emb.vertices().trunc3().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Triangle4Model::headerData(int section,
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

QString Triangle4Model::toolTipForCol(int column) {
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

QString Tetrahedron4Model::caption() const {
    return tr("Tetrahedra (%1)").arg(tri->label().c_str());
}

QString Tetrahedron4Model::overview() const {
    return tr("<qt>Displays details of each "
        "tetrahedron of this triangulation.<p>"
        "The different tetrahedra are numbered from 0 upwards.  "
        "Each row describes properties of the tetrahedron as well as "
        "listing precisely which vertices of which pentachora it "
        "corresponds to.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Tetrahedron4Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countTetrahedra());
}

int Tetrahedron4Model::columnCount(const QModelIndex& /* unused parent*/)
        const {
    return 4;
}

QVariant Tetrahedron4Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Tetrahedron<4>* item = tri->tetrahedron(index.row());
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
                        arg(item->embedding(i).pentachoron()->index()).
                        arg(item->embedding(i).vertices().
                            trunc4().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant Tetrahedron4Model::headerData(int section,
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

QString Tetrahedron4Model::toolTipForCol(int column) {
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

QString Component4Model::caption() const {
    return tr("Components (%1)").arg(tri->label().c_str());
}

QString Component4Model::overview() const {
    return tr("<qt>Displays details of each "
        "connected component of this triangulation.<p>"
        "The different components are numbered from 0 upwards.  "
        "Each row describes properties of the component as well as "
        "listing precisely which pentachora the component contains.<p>"
        "See the users' handbook for further details on what each "
        "column of the table means.</qt>");
}

int Component4Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countComponents());
}

int Component4Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant Component4Model::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        Component<4>* item = tri->component(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return ((! item->isValid()) ? tr("Invalid, ") :
                        item->isIdeal() ? tr("Ideal, ") : tr("Real, ")) +
                    (item->isOrientable() ? tr("orbl") : tr("non-orbl"));
            case 2:
                return static_cast<unsigned>(item->size());
            case 3:
                QString ans;
                for (unsigned long i = 0; i < item->size(); i++)
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

QVariant Component4Model::headerData(int section,
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

QString Component4Model::toolTipForCol(int column) {
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

QString BoundaryComponent4Model::caption() const {
    return tr("Boundary Components (%1)").arg(tri->label().c_str());
}

QString BoundaryComponent4Model::overview() const {
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

int BoundaryComponent4Model::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->countBoundaryComponents());
}

int BoundaryComponent4Model::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant BoundaryComponent4Model::data(const QModelIndex& index,
        int role) const {
    if (role == Qt::DisplayRole) {
        BoundaryComponent<4>* item = tri->boundaryComponent(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return (item->isIdeal() ? tr("Ideal") :
                    item->isInvalidVertex() ? tr("Invalid vertex") :
                    tr("Real"));
            case 2:
                return ((item->isIdeal() || item->isInvalidVertex()) ?
                    tr("Degree %1").arg(item->vertex(0)->degree()):
                    item->countTetrahedra() == 1 ? tr("1 tetrahedron") :
                    tr("%1 tetrahedra").arg(item->countTetrahedra()));
            case 3:
                if (item->isIdeal() || item->isInvalidVertex()) {
                    Vertex<4>* v = item->vertex(0);
                    QString ans;
                    for (auto& emb : *v)
                        appendToList(ans, QString("%1 (%2)").
                            arg(emb.pentachoron()->index()).
                            arg(emb.vertex()));
                    return tr("Vertex %1 = ").arg(v->index()) + ans;
                } else {
                    QString ans;
                    for (unsigned long i = 0;
                            i < item->countTetrahedra(); i++) {
                        const TetrahedronEmbedding<4>& emb =
                            item->tetrahedron(i)->front();
                        appendToList(ans, QString("%1 (%2)").
                            arg(emb.pentachoron()->index()).
                            arg(emb.vertices().trunc4().c_str()));
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

QVariant BoundaryComponent4Model::headerData(int section,
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

QString BoundaryComponent4Model::toolTipForCol(int column) {
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

