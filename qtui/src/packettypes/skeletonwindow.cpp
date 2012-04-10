
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

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
using regina::NFace;
using regina::NVertex;

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
        SkeletalObject viewObjectType) : 
        QDialog(packetUI->getInterface()), objectType(viewObjectType) {
    tri = dynamic_cast<regina::NTriangulation*>(packetUI->getPacket());
    QBoxLayout* layout = new QVBoxLayout(this);

    // Set up the table of data.
    switch (objectType) {
        case Vertices:
            model = new VertexModel(tri);
            break;
        case Edges:
            model = new EdgeModel(tri);
            break;
        case Faces:
            model = new FaceModel(tri);
            break;
        case Components:
            model = new ComponentModel(tri);
            break;
        case BoundaryComponents:
            model = new BoundaryComponentModel(tri);
            break;
    }

    table = new SkeletonTreeView();
    table->setItemsExpandable(false);
    table->setRootIsDecorated(false);
    table->setAlternatingRowColors(true);
    table->header()->setStretchLastSection(false);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setWhatsThis(overview(objectType));
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

    tri->listen(this);

    // Only one button to press (Close).
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(accept()));
}

void SkeletonWindow::refresh() {
    updateCaption();
    model->rebuild();
    table->header()->resizeSections(QHeaderView::ResizeToContents);
}

void SkeletonWindow::editingElsewhere() {
    setWindowTitle(tr("Editing... (") + tri->getPacketLabel().c_str() + ')');
    model->makeEmpty();
}

void SkeletonWindow::updateCaption() {
    QString label;
    switch (objectType) {
        case Vertices: label = tr("Vertices (%1)"); break;
        case Edges: label = tr("Edges (%1)"); break;
        case Faces: label = tr("Faces (%1)"); break;
        case Components: label = tr("Components (%1)"); break;
        case BoundaryComponents: label =
            tr("Boundary Components (%1)"); break;
        default: label = tr("Unknown skeletal items (%1)"); break;
    }

    setWindowTitle(label.arg(tri->getPacketLabel().c_str()));
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

QString SkeletonWindow::overview(SkeletalObject type) {
    switch (type) {
        case Vertices: return tr("<qt>Displays details of each "
            "vertex of this triangulation.<p>"
            "The different vertices are numbered from 0 upwards.  "
            "Each row describes properties of the vertex as well as "
            "listing precisely which vertices of which tetrahedra it "
            "corresponds to.<p>"
            "See the users' handbook for further details on what each "
            "column of the table means.</qt>");
        case Edges: return tr("<qt>Displays details of each edge of "
            "this triangulation.<p>"
            "The different edges are numbered from 0 upwards.  "
            "Each row describes properties of the edge as well as "
            "listing precisely which vertices of which tetrahedra it "
            "corresponds to.<p>"
            "See the users' handbook for further details on what each "
            "column of the table means.</qt>");
        case Faces: return tr("<qt>Displays details of each "
            "face of this triangulation.<p>"
            "The different faces are numbered from 0 upwards.  "
            "Each row describes the shape of the face as well as "
            "listing precisely which vertices of which tetrahedra it "
            "corresponds to.<p>"
            "See the users' handbook for further details on what each "
            "column of the table means.</qt>");
        case Components: return tr("<qt>Displays details of each "
            "connected component of this triangulation.<p>"
            "The different components are numbered from 0 upwards.  "
            "Each row describes properties of the component as well as "
            "listing precisely which tetrahedra the component contains.<p>"
            "See the users' handbook for further details on what each "
            "column of the table means.</qt>");
        case BoundaryComponents: return tr("<qt>Displays details of each "
            "boundary component of this triangulation.  A boundary "
            "component may be a collection of adjacent boundary faces, "
            "or it may be a single ideal vertex, whose link is closed but "
            "not a 2-sphere.<p>"
            "The different boundary components are numbered from 0 upwards.  "
            "Each row describes properties of the boundary component, as "
            "well as which tetrahedron faces (for a real boundary component) "
            "or which tetrahedron vertex (for an ideal boundary component) "
            "it is formed from.<p>"
            "See the users' handbook for further details on what each "
            "column of the table means.</qt>");
    }
    return QString::null;
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
                    return tr("Cusp (klein bottle)");
                if (link == NVertex::NON_STANDARD_CUSP) {
                    if (item->isLinkOrientable())
                        return tr("Cusp (orbl, genus %1)").arg(
                            1 - (item->getLinkEulerCharacteristic() / 2));
                    else
                        return tr("Cusp (non-or, genus %1)").arg(
                            2 - item->getLinkEulerCharacteristic());
                }
                if (link == NVertex::NON_STANDARD_BDRY)
                    return tr("Non-std bdry");
                return QString();
            }
            case 2:
                return static_cast<unsigned>(item->getNumberOfEmbeddings());
            case 3:
                QString ans;
                std::vector<regina::NVertexEmbedding>::const_iterator it;
                for (it = item->getEmbeddings().begin();
                        it != item->getEmbeddings().end(); it++)
                    appendToList(ans, QString("%1 (%2)").
                        arg(tri->tetrahedronIndex((*it).getTetrahedron())).
                        arg((*it).getVertex()));
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
                    return tr("INVALID");
                else if (item->isBoundary())
                    return tr("Bdry");
                else
                    return QString();
            case 2:
                return static_cast<unsigned>(item->getNumberOfEmbeddings());
            case 3:
                QString ans;
                std::deque<regina::NEdgeEmbedding>::const_iterator it;
                for (it = item->getEmbeddings().begin();
                        it != item->getEmbeddings().end(); it++)
                    appendToList(ans, QString("%1 (%2)").
                        arg(tri->tetrahedronIndex((*it).getTetrahedron())).
                        arg((*it).getVertices().trunc2().c_str()));
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

int FaceModel::rowCount(const QModelIndex& parent) const {
    if (forceEmpty)
        return 0;
    return (parent.isValid() ? 0 : tri->getNumberOfFaces());
}

int FaceModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant FaceModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        NFace* item = tri->getFace(index.row());
        switch (index.column()) {
            case 0:
                return index.row();
            case 1: {
                QString prefix;
                if (item->isBoundary())
                    prefix = tr("(Bdry) ");

                int type = item->getType();
                if (type == NFace::TRIANGLE)
                    return prefix + tr("Triangle");
                if (type == NFace::SCARF)
                    return prefix + tr("Scarf");
                if (type == NFace::PARACHUTE)
                    return prefix + tr("Parachute");
                if (type == NFace::MOBIUS)
                    return prefix + tr("Mobius band");
                if (type == NFace::CONE)
                    return prefix + tr("Cone");
                if (type == NFace::HORN)
                    return prefix + tr("Horn");
                if (type == NFace::DUNCEHAT)
                    return prefix + tr("Dunce hat");
                if (type == NFace::L31)
                    return prefix + tr("L(3,1)");
                return prefix + tr("UNKNOWN");
            }
            case 2:
                return static_cast<unsigned>(item->getNumberOfEmbeddings());
            case 3:
                QString ans;
                for (unsigned i = 0; i < item->getNumberOfEmbeddings(); i++)
                    appendToList(ans, QString("%1 (%2)").
                        arg(tri->tetrahedronIndex(
                            item->getEmbedding(i).getTetrahedron())).
                        arg(item->getEmbedding(i).getVertices().
                            trunc3().c_str()));
                return ans;
        }
        return QString();
    } else if (role == Qt::ToolTipRole) {
        return toolTipForCol(index.column());
    } else
        return QVariant();
}

QVariant FaceModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Face #");
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

QString FaceModel::toolTipForCol(int column) {
    switch (column) {
        case 0: return tr("<qt>The number of the individual face.  "
            "Faces are numbered 0,1,2,...,<i>f</i>-1.</qt>");
        case 1: return tr("<qt>Lists additional properties of the face, "
            "such as the shape that it forms and whether it lies on the "
            "boundary.</qt>");
        case 2: return tr("<qt>Gives the degree of this face, i.e., "
            "the number of individual tetrahedron faces that are "
            "identified to it.</qt>");
        case 3: return tr("<qt>Lists the individual tetrahedron faces "
            "that come together to form this face of the triangulation.</qt>");
        default: return QString();
    }
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
                    appendToList(ans, QString::number(tri->tetrahedronIndex(
                        item->getTetrahedron(i))));
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
                // Note that we can't have just one face (by a parity argument).
                return (item->isIdeal() ? tr("1 vertex") :
                    tr("%1 faces").arg(item->getNumberOfFaces()));
            case 3:
                if (item->isIdeal())
                    return tr("Vertex %1").arg(tri->vertexIndex(
                        item->getVertex(0)));
                else {
                    QString ans;
                    for (unsigned long i = 0; i < item->getNumberOfFaces(); i++)
                        appendToList(ans, QString::number(tri->faceIndex(
                            item->getFace(i))));
                    return tr("Faces ") + ans;
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
            case 3: return tr("Faces / Vertex");
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
            "i.e., the number of faces (for a real boundary component) "
            "or the number of vertices (which is always one for an ideal "
            "boundary component).</qt>");
        case 3: return tr("<qt>Identifies the individual faces for a real "
            "boundary component, or the individual vertex for an ideal "
            "boundary component.</qt>");
        default: return QString();
    }
}

