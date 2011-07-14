
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include <klocale.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qstyle.h>
#include <QTreeView>

using regina::NBoundaryComponent;
using regina::NComponent;
using regina::NEdge;
using regina::NFace;
using regina::NVertex;

namespace {
    inline QString& appendToList(QString& list,
            const QString& item) {
        return (list.isEmpty() ? (list = item) :
            (list.append(", ").append(item)));
    }
}

SkeletonWindow::SkeletonWindow(PacketUI* packetUI,
        SkeletalObject viewObjectType) : 
        KDialog(packetUI->getInterface()), objectType(viewObjectType) {

    setButtons(KDialog::Close);
    tri = dynamic_cast<regina::NTriangulation*>(packetUI->getPacket());

    QWidget* page = new QWidget();
    setMainWidget(page);
    QBoxLayout* layout = new QVBoxLayout(page);

    table = new QTreeView(page);
    switch (objectType) {
        case Vertices:
            table->setModel(new VertexModel(this, tri));
            break;
        case Edges:
            table->setModel(new EdgeModel(this, tri));
            break;
        case Faces:
            table->setModel(new FaceModel(this, tri));
            break;
        case Components:
            table->setModel(new ComponentModel(this, tri));
            break;
        case BoundaryComponents:
            table->setModel(new BoundaryComponentModel(this, tri));
            break;
    }
    table->setRootIsDecorated(false);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    //table->setSorting(-1);
    table->setToolTip(overview(objectType)); // TODO
    layout->addWidget(table);

    refresh();
}

void SkeletonWindow::refresh() {
    // TODO
    // table->clear();

    updateCaption();
    tri->listen(this);
}

void SkeletonWindow::editingElsewhere() {
    // table->clear(); // TODO
    setCaption(i18n("Editing... (") + tri->getPacketLabel().c_str() + ')');
}

void SkeletonWindow::updateCaption() {
    setCaption(typeLabel(objectType) + " (" +
        tri->getPacketLabel().c_str() + ')');
}

void SkeletonWindow::packetWasChanged(regina::NPacket*) {
    refresh();
}

void SkeletonWindow::packetWasRenamed(regina::NPacket*) {
    updateCaption();
}

void SkeletonWindow::packetToBeDestroyed(regina::NPacket*) {
    slotButtonClicked(KDialog::Close);
}

QString SkeletonWindow::typeLabel(SkeletalObject type) {
    switch (type) {
        case Vertices: return i18n("Vertices");
        case Edges: return i18n("Edges");
        case Faces: return i18n("Faces");
        case Components: return i18n("Components");
        case BoundaryComponents: return i18n("Boundary Components");
    }
    return QString::null;
}

QString SkeletonWindow::overview(SkeletalObject type) {
    switch (type) {
        case Vertices: return i18n("<qt>Displays details of each "
            "vertex of this triangulation.<p>"
            "The different vertices are numbered from 0 upwards.  "
            "Each row describes properties of the vertex as well as "
            "listing precisely which vertices of which tetrahedra it "
            "corresponds to.<p>"
            "See the users' handbook for further details on what each "
            "column of the table means.</qt>");
        case Edges: return i18n("<qt>Displays details of each edge of "
            "this triangulation.<p>"
            "The different edges are numbered from 0 upwards.  "
            "Each row describes properties of the edge as well as "
            "listing precisely which vertices of which tetrahedra it "
            "corresponds to.<p>"
            "See the users' handbook for further details on what each "
            "column of the table means.</qt>");
        case Faces: return i18n("<qt>Displays details of each "
            "face of this triangulation.<p>"
            "The different faces are numbered from 0 upwards.  "
            "Each row describes the shape of the face as well as "
            "listing precisely which vertices of which tetrahedra it "
            "corresponds to.<p>"
            "See the users' handbook for further details on what each "
            "column of the table means.</qt>");
        case Components: return i18n("<qt>Displays details of each "
            "connected component of this triangulation.<p>"
            "The different components are numbered from 0 upwards.  "
            "Each row describes properties of the component as well as "
            "listing precisely which tetrahedra the component contains.<p>"
            "See the users' handbook for further details on what each "
            "column of the table means.</qt>");
        case BoundaryComponents: return i18n("<qt>Displays details of each "
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
    return (parent.isValid() ? 0 : tri->getNumberOfVertices());
}

int VertexModel::columnCount(const QModelIndex& parent) const {
    return 4;
}

QVariant VertexModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    NVertex* item = tri->getVertex(index.row());
    switch (index.column()) {
        case 0:
            return index.row();
        case 1: {
            int link = item->getLink();
            if (link == NVertex::SPHERE)
                return QString();
            if (link == NVertex::DISC)
                return i18n("Bdry");
            if (link == NVertex::TORUS)
                return i18n("Cusp (torus)");
            if (link == NVertex::KLEIN_BOTTLE)
                return i18n("Cusp (klein bottle)");
            if (link == NVertex::NON_STANDARD_CUSP) {
                if (item->isLinkOrientable())
                    return i18n("Cusp (orbl, genus %1)").arg(
                        1 - (item->getLinkEulerCharacteristic() / 2));
                else
                    return i18n("Cusp (non-or, genus %1)").arg(
                        2 - item->getLinkEulerCharacteristic());
            }
            if (link == NVertex::NON_STANDARD_BDRY)
                return i18n("Non-std bdry");
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
}

QVariant VertexModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return i18n("Vertex #");
            case 1: return i18n("Type");
            case 2: return i18n("Degree");
            case 3: return i18n("Tetrahedra (Tet vertices)");
            default: return QString();
        }
    } else
        return QString();
}

int EdgeModel::rowCount(const QModelIndex& parent) const {
    return (parent.isValid() ? 0 : tri->getNumberOfEdges());
}

int EdgeModel::columnCount(const QModelIndex& parent) const {
    return 4;
}

QVariant EdgeModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    NEdge* item = tri->getEdge(index.row());
    switch (index.column()) {
        case 0:
            return index.row();
        case 1:
            if (! item->isValid())
                return i18n("INVALID");
            else if (item->isBoundary())
                return i18n("Bdry");
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
}

QVariant EdgeModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return i18n("Edge #");
            case 1: return i18n("Type");
            case 2: return i18n("Degree");
            case 3: return i18n("Tetrahedra (Tet vertices)");
            default: return QString();
        }
    } else
        return QString();
}

int FaceModel::rowCount(const QModelIndex& parent) const {
    return (parent.isValid() ? 0 : tri->getNumberOfFaces());
}

int FaceModel::columnCount(const QModelIndex& parent) const {
    return 4;
}

QVariant FaceModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    NFace* item = tri->getFace(index.row());
    switch (index.column()) {
        case 0:
            return index.row();
        case 1: {
            QString prefix;
            if (item->isBoundary())
                prefix = i18n("(Bdry) ");

            int type = item->getType();
            if (type == NFace::TRIANGLE)
                return prefix + i18n("Triangle");
            if (type == NFace::SCARF)
                return prefix + i18n("Scarf");
            if (type == NFace::PARACHUTE)
                return prefix + i18n("Parachute");
            if (type == NFace::MOBIUS)
                return prefix + i18n("Mobius band");
            if (type == NFace::CONE)
                return prefix + i18n("Cone");
            if (type == NFace::HORN)
                return prefix + i18n("Horn");
            if (type == NFace::DUNCEHAT)
                return prefix + i18n("Dunce hat");
            if (type == NFace::L31)
                return prefix + i18n("L(3,1)");
            return prefix + i18n("UNKNOWN");
        }
        case 2:
            return static_cast<unsigned>(item->getNumberOfEmbeddings());
        case 3:
            QString ans;
            for (unsigned i = 0; i < item->getNumberOfEmbeddings(); i++)
                appendToList(ans, QString("%1 (%2)").
                    arg(tri->tetrahedronIndex(
                        item->getEmbedding(i).getTetrahedron())).
                    arg(item->getEmbedding(i).getVertices().trunc3().c_str()));
            return ans;
    }
    return QString();
}

QVariant FaceModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return i18n("Face #");
            case 1: return i18n("Type");
            case 2: return i18n("Degree");
            case 3: return i18n("Tetrahedra (Tet vertices)");
            default: return QString();
        }
    } else
        return QString();
}

int ComponentModel::rowCount(const QModelIndex& parent) const {
    return (parent.isValid() ? 0 : tri->getNumberOfComponents());
}

int ComponentModel::columnCount(const QModelIndex& parent) const {
    return 4;
}

QVariant ComponentModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    NComponent* item = tri->getComponent(index.row());
    switch (index.column()) {
        case 0:
            return index.row();
        case 1:
            return (item->isIdeal() ? i18n("Ideal, ") : i18n("Real, ")) +
                (item->isOrientable() ? i18n("Orbl") : i18n("Non-orbl"));
        case 2:
            return static_cast<unsigned>(item->getNumberOfTetrahedra());
        case 3:
            QString ans;
            for (unsigned long i = 0; i < item->getNumberOfTetrahedra(); i++)
                appendToList(ans, QString::number(tri->tetrahedronIndex(
                    item->getTetrahedron(i))));
            return ans;
    }
    return QString();
}

QVariant ComponentModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return i18n("Cmpt #");
            case 1: return i18n("Type");
            case 2: return i18n("Size");
            case 3: return i18n("Tetrahedra");
            default: return QString();
        }
    } else
        return QString();
}

int BoundaryComponentModel::rowCount(const QModelIndex& parent) const {
    return (parent.isValid() ? 0 : tri->getNumberOfBoundaryComponents());
}

int BoundaryComponentModel::columnCount(const QModelIndex& parent) const {
    return 4;
}

QVariant BoundaryComponentModel::data(const QModelIndex& index,
        int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    NBoundaryComponent* item = tri->getBoundaryComponent(index.row());
    switch (index.column()) {
        case 0:
            return index.row();
        case 1:
            return (item->isIdeal() ? i18n("Ideal") : i18n("Real"));
        case 2:
            // Note that we can't have just one face (by a parity argument).
            return (item->isIdeal() ? i18n("1 vertex") :
                i18n("%1 faces").arg(item->getNumberOfFaces()));
        case 3:
            if (item->isIdeal())
                return i18n("Vertex %1").arg(tri->vertexIndex(
                    item->getVertex(0)));
            else {
                QString ans;
                for (unsigned long i = 0; i < item->getNumberOfFaces(); i++)
                    appendToList(ans, QString::number(tri->faceIndex(
                        item->getFace(i))));
                return i18n("Faces ") + ans;
            }
    }
    return QString();
}

QVariant BoundaryComponentModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return i18n("Cmpt #");
            case 1: return i18n("Type");
            case 2: return i18n("Size");
            case 3: return i18n("Faces / Vertex");
            default: return QString();
        }
    } else
        return QString();
}

