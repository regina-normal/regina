
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
    layout->addWidget(table);

    refresh();
}

void SkeletonWindow::refresh() {
    table->clear();

    switch (objectType) {
        case Vertices:
            for (unsigned i = 0; i < tri->getNumberOfVertices(); i++)
                new VertexItem(table, tri->getVertex(i), i);
            break;
        case Edges:
            for (unsigned i = 0; i < tri->getNumberOfEdges(); i++)
                new EdgeItem(table, tri->getEdge(i), i);
            break;
        case Faces:
            for (unsigned i = 0; i < tri->getNumberOfFaces(); i++)
                new FaceItem(table, tri->getFace(i), i);
            break;
        case Components:
            for (unsigned i = 0; i < tri->getNumberOfComponents(); i++)
                new ComponentItem(table, tri->getComponent(i), i);
            break;
        case BoundaryComponents:
            for (unsigned i = 0; i < tri->getNumberOfBoundaryComponents(); i++)
                new BoundaryComponentItem(table, tri->getBoundaryComponent(i),
                    i);
            break;
    }

    updateCaption();
}

void SkeletonWindow::editingElsewhere() {
    table->clear();
    setCaption(i18n("Editing... (") + tri->getPacketLabel().c_str() + ')');
}

void SkeletonWindow::updateCaption() {
    setCaption(typeLabel(objectType) + " (" +
        tri->getPacketLabel().c_str() + ')');
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

int SkeletalItem::width(const QFontMetrics& fm, const QListView* lv, int c)
        const {
    /**
     * Add a bit of space so items aren't pressed right against the
     * grid.
     */
    return KListViewItem::width(fm, lv, c) + 2;
}
void SkeletalItem::paintCell(QPainter* p, const QColorGroup& cg,
        int column, int width, int align) {
    // Do the standard painting.
    KListViewItem::paintCell(p, cg, column, width, align);

    // Draw a box around the cell.
    p->setPen((QRgb)listView()->style().styleHint(
        QStyle::SH_Table_GridLineColor, listView()));
    p->drawLine(0, height() - 1, width - 1, height() - 1);
    p->lineTo(width - 1, 0);
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
            /* TODO */
            return QString::null;
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
            /* TODO */
            return QString::null;
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
                prefix = i18n("(Bdry )");

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
            /* TODO */
            return QString::null;
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
            /* TODO */
            return QString::null;
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
            /* TODO */
            return QString::null;
    }
    return QString::null;
}

#include "skeletonwindow.moc"
