
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include "triangulation/ntriangulation.h"

#include "skeletonwindow.h"
#include "../packetui.h"

#include <klistview.h>
#include <klocale.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qwhatsthis.h>

using regina::NBoundaryComponent;
using regina::NComponent;
using regina::NEdge;
using regina::NFace;
using regina::NVertex;

SkeletonWindow::SkeletonWindow(PacketUI* packetUI,
        SkeletalObject viewObjectType) : KDialogBase(Plain,
        QString::null, Close, Close, packetUI->getInterface(), 0, false),
        objectType(viewObjectType) {
    tri = dynamic_cast<regina::NTriangulation*>(packetUI->getPacket());

    QFrame* page = plainPage();
    QBoxLayout* layout = new QVBoxLayout(page);

    table = new KListView(page);
    table->addColumn(columnLabel(objectType, 0));
    table->addColumn(columnLabel(objectType, 1));
    table->addColumn(columnLabel(objectType, 2));
    table->addColumn(columnLabel(objectType, 3));
    table->setSelectionMode(QListView::NoSelection);
    table->setSorting(-1);
    QWhatsThis::add(table, overview(objectType));
    layout->addWidget(table);

    refresh();
}

void SkeletonWindow::refresh() {
    table->clear();

    /**
     * Add the items in reverse order so they come out correctly in the
     * list view.  *sigh*
     */
    switch (objectType) {
        case Vertices:
            for (long i = tri->getNumberOfVertices() - 1; i >= 0; i--)
                new VertexItem(table, tri, i);
            break;
        case Edges:
            for (long i = tri->getNumberOfEdges() - 1; i >= 0; i--)
                new EdgeItem(table, tri, i);
            break;
        case Faces:
            for (long i = tri->getNumberOfFaces() - 1; i >= 0; i--)
                new FaceItem(table, tri, i);
            break;
        case Components:
            for (long i = tri->getNumberOfComponents() - 1; i >= 0; i--)
                new ComponentItem(table, tri, i);
            break;
        case BoundaryComponents:
            for (long i = tri->getNumberOfBoundaryComponents() - 1; i >= 0; i--)
                new BoundaryComponentItem(table, tri, i);
            break;
    }

    updateCaption();
    tri->listen(this);
}

void SkeletonWindow::editingElsewhere() {
    table->clear();
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
    slotClose();
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

QString SkeletonWindow::columnLabel(SkeletalObject type, int column) {
    switch (column) {
        case 0:
            switch (type) {
                case Vertices: return i18n("Vertex #");
                case Edges: return i18n("Edge #");
                case Faces: return i18n("Face #");
                case Components: return i18n("Cmpt #");
                case BoundaryComponents: return i18n("Cmpt #");
            }
            return QString::null;
        case 1:
            return i18n("Type");
        case 2:
            switch (type) {
                case Vertices:
                case Edges:
                case Faces: return i18n("Degree");
                case Components:
                case BoundaryComponents: return i18n("Size");
            }
            return QString::null;
        case 3:
            switch (type) {
                case Vertices:
                case Edges:
                case Faces: return i18n("Tetrahedra (Tet vertices)");
                case Components: return i18n("Tetrahedra");
                case BoundaryComponents: return i18n("Faces / Vertex");
            }
            return QString::null;
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
            "See the reference manual for further details on what each "
            "column of the table means.</qt>");
        case Edges: return i18n("<qt>Displays details of each edge of "
            "this triangulation.<p>"
            "The different edges are numbered from 0 upwards.  "
            "Each row describes properties of the edge as well as "
            "listing precisely which vertices of which tetrahedra it "
            "corresponds to.<p>"
            "See the reference manual for further details on what each "
            "column of the table means.</qt>");
        case Faces: return i18n("<qt>Displays details of each "
            "face of this triangulation.<p>"
            "The different faces are numbered from 0 upwards.  "
            "Each row describes the shape of the face as well as "
            "listing precisely which vertices of which tetrahedra it "
            "corresponds to.<p>"
            "See the reference manual for further details on what each "
            "column of the table means.</qt>");
        case Components: return i18n("<qt>Displays details of each "
            "connected component of this triangulation.<p>"
            "The different components are numbered from 0 upwards.  "
            "Each row describes properties of the component as well as "
            "listing precisely which tetrahedra the component contains.<p>"
            "See the reference manual for further details on what each "
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
            "See the reference manual for further details on what each "
            "column of the table means.</qt>");
    }
    return QString::null;
}

VertexItem::VertexItem(QListView* parent,
                regina::NTriangulation* useTri, unsigned long useItemIndex) :
        SkeletalItem(parent, useTri, useItemIndex),
        item(useTri->getVertex(useItemIndex)) {
}

EdgeItem::EdgeItem(QListView* parent,
                regina::NTriangulation* useTri, unsigned long useItemIndex) :
        SkeletalItem(parent, useTri, useItemIndex),
        item(useTri->getEdge(useItemIndex)) {
}

FaceItem::FaceItem(QListView* parent,
                regina::NTriangulation* useTri, unsigned long useItemIndex) :
        SkeletalItem(parent, useTri, useItemIndex),
        item(useTri->getFace(useItemIndex)) {
}

ComponentItem::ComponentItem(QListView* parent,
                regina::NTriangulation* useTri, unsigned long useItemIndex) :
        SkeletalItem(parent, useTri, useItemIndex),
        item(useTri->getComponent(useItemIndex)) {
}

BoundaryComponentItem::BoundaryComponentItem(QListView* parent,
                regina::NTriangulation* useTri, unsigned long useItemIndex) :
        SkeletalItem(parent, useTri, useItemIndex),
        item(useTri->getBoundaryComponent(useItemIndex)) {
}

QString VertexItem::text(int column) const {
    switch (column) {
        case 0:
            return QString::number(itemIndex);
        case 1: {
            int link = item->getLink();
            if (link == NVertex::SPHERE)
                return QString::null;
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
            return QString::null;
        }
        case 2:
            return QString::number(item->getNumberOfEmbeddings());
        case 3:
            QString ans;
            std::vector<regina::NVertexEmbedding>::const_iterator it;
            for (it = item->getEmbeddings().begin();
                    it != item->getEmbeddings().end(); it++)
                appendToList(ans, QString("%1 (%2)").
                    arg(tri->getTetrahedronIndex((*it).getTetrahedron())).
                    arg((*it).getVertex()));
            return ans;
    }
    return QString::null;
}

QString EdgeItem::text(int column) const {
    switch (column) {
        case 0:
            return QString::number(itemIndex);
        case 1:
            if (! item->isValid())
                return i18n("INVALID");
            else if (item->isBoundary())
                return i18n("Bdry");
            else
                return QString::null;
        case 2:
            return QString::number(item->getNumberOfEmbeddings());
        case 3:
            QString ans;
            std::deque<regina::NEdgeEmbedding>::const_iterator it;
            for (it = item->getEmbeddings().begin();
                    it != item->getEmbeddings().end(); it++)
                appendToList(ans, QString("%1 (%2)").
                    arg(tri->getTetrahedronIndex((*it).getTetrahedron())).
                    arg(regina::edgeDescription((*it).getVertices()).c_str()));
            return ans;
    }
    return QString::null;
}

QString FaceItem::text(int column) const {
    switch (column) {
        case 0:
            return QString::number(itemIndex);
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
            return QString::number(item->getNumberOfEmbeddings());
        case 3:
            QString ans;
            for (unsigned i = 0; i < item->getNumberOfEmbeddings(); i++)
                appendToList(ans, QString("%1 (%2)").
                    arg(tri->getTetrahedronIndex(
                        item->getEmbedding(i).getTetrahedron())).
                    arg(regina::faceDescription(
                        item->getEmbedding(i).getVertices()).c_str()));
            return ans;
    }
    return QString::null;
}

QString ComponentItem::text(int column) const {
    switch (column) {
        case 0:
            return QString::number(itemIndex);
        case 1:
            return (item->isIdeal() ? i18n("Ideal, ") : i18n("Real, ")) +
                (item->isOrientable() ? i18n("Orbl") : i18n("Non-orbl"));
        case 2:
            return QString::number(item->getNumberOfTetrahedra());
        case 3:
            QString ans;
            for (unsigned long i = 0; i < item->getNumberOfTetrahedra(); i++)
                appendToList(ans, QString::number(tri->getTetrahedronIndex(
                    item->getTetrahedron(i))));
            return ans;
    }
    return QString::null;
}

QString BoundaryComponentItem::text(int column) const {
    switch (column) {
        case 0:
            return QString::number(itemIndex);
        case 1:
            return (item->isIdeal() ? i18n("Ideal") : i18n("Real"));
        case 2:
            // Note that we can't have just one face (by a parity argument).
            return (item->isIdeal() ? i18n("1 vertex") :
                i18n("%1 faces").arg(item->getNumberOfFaces()));
        case 3:
            if (item->isIdeal())
                return i18n("Vertex %1").arg(tri->getVertexIndex(
                    item->getVertex(0)));
            else {
                QString ans;
                for (unsigned long i = 0; i < item->getNumberOfFaces(); i++)
                    appendToList(ans, QString::number(tri->getFaceIndex(
                        item->getFace(i))));
                return i18n("Faces ") + ans;
            }
    }
    return QString::null;
}

#include "skeletonwindow.moc"
