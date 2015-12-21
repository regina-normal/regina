
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
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
#include "triangulation/ntriangulation.h"

// UI includes:
#include "cycledrawingtab.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../messagelayer.h"

#include <fstream>
#include <QGraphicsView>
#include <QLayout>
#include <QGraphicsScene>
#include <QStackedWidget>
#include <QMouseEvent>


CycleDrawingTab::CycleDrawingTab(CycleDrawingData* useData,
        PacketTabbedViewerTab* useParentUI) :
        PacketViewerTab(useParentUI), data(useData) {
    ui = new QWidget();
    baseLayout = new QVBoxLayout(ui);
    stack = new QStackedWidget(ui);

    // Information and error layers.
    layerInfo = new MessageLayer("dialog-information", tr("Initialising..."));
    layerError = new MessageLayer("dialog-warning", tr("Initialising..."));
    stack->addWidget(layerInfo);
    stack->addWidget(layerError);

    scene = new QGraphicsScene();
    view = new CycleView(scene, this);
    view->setWhatsThis(data->overview());
    stack->addWidget(view);

    // Finish off.
    baseLayout->addWidget(stack);
    refresh();
}

regina::NPacket* CycleDrawingTab::getPacket() {
    return data->getPacket();
}

QWidget* CycleDrawingTab::getInterface() {
    return ui;
}

void CycleDrawingTab::refresh() {
    unsigned long n = data->numberOfSimplices();
    if (n == 0) {
        showInfo(tr("<qt>This triangulation is empty.</qt>"));
        return;
    } else if (n > 50) {
        showInfo(tr("<qt>This triangulation contains over 50 %1.<p>"
            "<p>Regina does not display graphs "
            "for such large triangulations.</qt>")
            .arg(data->simplicesName()));
        return;
    }
    view->drawHighlight();
    view->drawArcs(data);
    scene->setSceneRect(scene->itemsBoundingRect());
    view->show();
    view->resize(stack->size());
    stack->setCurrentWidget(view);
}

void CycleDrawingTab::showInfo(const QString& msg) {
    layerInfo->setText(msg);
    stack->setCurrentWidget(layerInfo);
}

// Currently not ever called, but could be useful at some point in the future.
void CycleDrawingTab::showError(const QString& msg) {
    layerError->setText(msg);
    stack->setCurrentWidget(layerError);
}

CycleDrawingData::Arc::Arc() : added(false), needsRefresh(true) {
}

void CycleDrawingData::Arc::draw(QGraphicsScene *s) {
    if (added)
        s->removeItem(this);
    added = true;
    needsRefresh = false;
    updatePath();
    setColour();
    s->addItem(this);
}

void CycleDrawingData::Arc::setEdgeIndex(int i) {
    edgeIndex = i;
}

QColor CycleDrawingData::Arc::colours[numColours] = { Qt::red,
    Qt::green, Qt::blue, Qt::magenta, Qt::darkYellow, Qt::darkGreen, Qt::cyan,
    Qt::yellow };


void CycleDrawingData::Arc::setColour() {
    p.setColor(getColour(edgeIndex));
    p.setWidth(3);
    setPen(p);
}

Dim3CycleDrawingData::Dim3CycleDrawingData(regina::NTriangulation* tri) :
            tri_(tri), simps(0) {
    refresh();
}

Dim3CycleDrawingData::~Dim3CycleDrawingData() {
    // Delete arcs
    for(auto v: arcs) {
        for(Arc *a: v) {
            delete a;
        }
    }
    // Then the simplex objects themselves
    for(int s=0; s < numberOfSimplices(); s++) {
        delete getSimplex(s);
    }
}


void Dim3CycleDrawingData::refresh() {
    if (simps > 0) {
        // delete any existing stuff.
        for(auto v: arcs) {
            for(Arc *a: v) {
                delete a;
            }
        }
        arcs.clear();
        // Only simplices left;
        for(int simp = 0; simp < simps; simp++) {
            delete simplices[simp];
        }
        delete simplices;
    }
    simps = tri_->getNumberOfSimplices();
    // Create new simplices. Note that this also creates the internal arcs
    simplices = new Dim3CycleDrawingData::Dim3SimplexRep*[tri_->getNumberOfSimplices()];
    for(int simp = 0; simp < tri_->getNumberOfSimplices(); simp++)
        simplices[simp] = new Dim3CycleDrawingData::Dim3SimplexRep(simp);
    arcs.resize(tri_->getNumberOfEdges());
    // Now add each of the edges as a series of arcs
    for(int edgeID = 0; edgeID < tri_->getNumberOfEdges(); edgeID++) {
        regina::NEdge* edge = tri_->getEdge(edgeID);
        std::deque<Arc*> & arcSet = arcs[edgeID];
        std::deque<regina::NEdgeEmbedding>::const_iterator it;
        int lastTet, lastEdge;
        // If the edge is on the boundary, we don't want to join the start+end
        // together
        if (edge->isBoundary()) {
            lastTet = -1;
            lastEdge = -1;
        } else {
            lastTet = edge->getEmbeddings().back().getTetrahedron()->index();
            lastEdge = edge->getEmbeddings().back().getEdge();
        }
        for (it = edge->getEmbeddings().begin();
                it != edge->getEmbeddings().end(); ++it) {
            int tet = it->getTetrahedron()->index();
            int edgeIdx = it->getEdge();
            // Add internal arc
            simplices[tet]->getInternal(edgeIdx)->setEdgeIndex(edgeID);
            arcSet.push_back( simplices[tet]->getInternal(edgeIdx) );
            // Now need to set external arc.
            // But not if this is the first edge in a boundary component
            if (lastTet == -1) {
                lastTet = tet;
                lastEdge = edgeIdx;
                continue;
            }
            // Work out which face leads "back" to the previous edge
            // embedding.
            int face = it->getVertices()[3];
            ExternalArc *ea = new Dim3CycleDrawingData::ExternalArc;
            ea->setEdgeIndex(edgeID);
            ea->setInternal(0, simplices[tet]->getInternal(edgeIdx));
            ea->setInternal(1, simplices[lastTet]->getInternal(lastEdge));
            arcSet.push_back(ea);
            // Each InternalArc has two external arcs, one from each
            // face. The lower-numbered face is external[0] and the
            // higher is external[1]
            if (regina::NEdge::edgeVertex[5-edgeIdx][0] == face)
                simplices[tet]->getInternal(edgeIdx)->setExternal(0, ea);
            else
                simplices[tet]->getInternal(edgeIdx)->setExternal(1, ea);
            int otherFace =
                tri_->getTetrahedron(tet)->adjacentFace(face);
            // Do the same for the other face.
            if (regina::NEdge::edgeVertex[5-lastEdge][0] == otherFace)
                simplices[lastTet]->getInternal(lastEdge)->setExternal(0, ea);
            else
                simplices[lastTet]->getInternal(lastEdge)->setExternal(1, ea);
            lastTet = tet;
            lastEdge = edgeIdx;
        }
    }
}

std::vector<std::deque<Dim3CycleDrawingData::Arc*>>& Dim3CycleDrawingData::getArcs() {
    return arcs;
}

QString Dim3CycleDrawingData::simplicesName() {
    return QObject::tr("tetrahedra");
}

QString Dim3CycleDrawingData::overview() {
    return QObject::tr("<qt>This shows a graphical representation of the "
            "triangulation, as a decomposition of a graph. For details on this "
            "representation, see TODO</qt>");
}

regina::NPacket* Dim3CycleDrawingData::getPacket() {
    return tri_;
}

qreal Dim3CycleDrawingData::Dim3SimplexRep::cubic = 3;
qreal Dim3CycleDrawingData::Dim3SimplexRep::scale = 25;

// TODO - initial placement in general position instead of in a line
#define OFFSET 40.0
Dim3CycleDrawingData::Dim3SimplexRep::Dim3SimplexRep(int simp) :
        centre(OFFSET*simp, OFFSET*simp),  angle(0) {
    // Make all 6 internal arcs.
    internal[0] = new Dim3CycleDrawingData::InternalArc(this,QPointF(-1.0*scale, scale/5),
                                  QPointF(-cubic*scale, scale/5),
                                  QPointF(-1.0*scale/5, scale/5),
                                  QPointF(-1.0*scale/5, scale*cubic),
                                  QPointF(-1.0*scale/5, scale));
    internal[1] = new Dim3CycleDrawingData::InternalArc(this,QPointF(0, -1.0*scale),
                                  QPointF(0, -cubic*scale),
                                  QPointF(0,0),
                                  QPointF(0, scale*cubic),
                                  QPointF(0, scale));
    internal[2] = new Dim3CycleDrawingData::InternalArc(this,QPointF(-scale/5, -scale),
                                  QPointF(-scale/5, -scale*cubic),
                                  QPointF(-scale/5, -scale/5),
                                  QPointF(-scale*cubic, -scale/5),
                                  QPointF(-scale,-scale/5));
    internal[3] = new Dim3CycleDrawingData::InternalArc(this,QPointF(scale, scale/5),
                                  QPointF(scale*cubic, scale/5),
                                  QPointF(scale/5, scale/5),
                                  QPointF(scale/5, scale*cubic),
                                  QPointF(scale/5, scale));
    internal[4] = new Dim3CycleDrawingData::InternalArc(this,QPointF(scale, 0),
                                  QPointF(scale*cubic, 0),
                                  QPointF(0, 0),
                                  QPointF(-scale*cubic, 0),
                                  QPointF(-scale, 0));
    internal[5] = new Dim3CycleDrawingData::InternalArc(this,QPointF(scale, -scale/5),
                                  QPointF(scale*cubic, -scale/5),
                                  QPointF(scale/5, -scale/5),
                                  QPointF(scale/5, -scale*cubic),
                                  QPointF(scale/5, -scale));
}

void Dim3CycleDrawingData::Dim3SimplexRep::addAngle(qreal a) {
    angle = std::fmod(angle + a, 360);
}

void Dim3CycleDrawingData::Dim3SimplexRep::markChanged() {
    for (int i=0; i < 6; i++) {
        internal[i]->markChanged();
        for (int j=0; j < 2; j++)
            if (internal[i]->getExternal(j))
                internal[i]->getExternal(j)->markChanged();
    }
}

Dim3CycleDrawingData::InternalArc::InternalArc(Dim3SimplexRep* simp_, QPointF
        endA_, QPointF cubicA_, QPointF mid_, QPointF cubicB_, QPointF endB_) :
    simp(simp_), endA(endA_), cubicA(cubicA_), mid(mid_), cubicB(cubicB_),
    endB(endB_) {
    externals[0] = NULL;
    externals[1] = NULL;
}

QPointF Dim3CycleDrawingData::InternalArc::getEnd(int end) {
    QTransform r;
    if (end == 0)
        return simp->getCentre() + r.rotate(simp->getAngle()).map(endA);
    else
        return simp->getCentre() + r.rotate(simp->getAngle()).map(endB);
}

QPointF Dim3CycleDrawingData::InternalArc::getMid() {
    QTransform r;
    return simp->getCentre() + r.rotate(simp->getAngle()).map(mid);
}

QPointF Dim3CycleDrawingData::InternalArc::getCubic(int end) {
    QTransform r;
    if (end == 0)
        return simp->getCentre() + r.rotate(simp->getAngle()).map(cubicA);
    else
        return simp->getCentre() + r.rotate(simp->getAngle()).map(cubicB);
}

void Dim3CycleDrawingData::InternalArc::updatePath() {
    QPainterPath path;
    path.moveTo(getEnd(0));
    path.cubicTo(getMid(),getMid(),getEnd(1));
    setPath(path);
}

void Dim3CycleDrawingData::ExternalArc::updatePath() {
    InternalArc *ia = internals[0];
    QPointF start, c1, c2, end;
    // Note that if this arc is a loop, it won't matter which end we get, as
    // long as we get both ends.
    if (ia->getExternal(0) == this) {
        start = ia->getEnd(0);
        c1 = ia->getCubic(0);
    } else {
        start = ia->getEnd(1);
        c1 = ia->getCubic(1);
    }
    ia = internals[1];
    // If this is a loop, we'll now use the "other" end here.
    if (ia->getExternal(1) == this) {
        end = ia->getEnd(1);
        c2 = ia->getCubic(1);
    } else {
        end = ia->getEnd(0);
        c2 = ia->getCubic(0);
    }
    QPainterPath path;
    path.moveTo(start);
    path.cubicTo(c1,c2,end);
    setPath(path);
}


CycleView::CycleView(QGraphicsScene *scene, CycleDrawingTab *tab_) :
    QGraphicsView(scene),
    selectedSimplex(NULL),/* selectedArc(NULL),*/ highlighted(NULL),
    currScale(1.0), minScale(0.1), maxScale(10.0), scaleFactor(0.1),
    tab(tab_) {

}

void CycleView::drawArcs(CycleDrawingData *data) {
    for(auto v: data->getArcs()) {
        for(CycleDrawingData::Arc* arc: v) {
            if (arc->needsRedrawing()) {
                arc->draw(scene());
            }
        }
    }
}

void CycleView::drawHighlight() {
    // If something has been highlighted, remove it.
    if (highlighted) {
        scene()->removeItem(highlighted);
        delete highlighted;
        highlighted = NULL;
    }
    // Rehighlight if necessary
    if (selectedSimplex) {
        const qreal ellipseSize = 50.0;
        QPointF offset(-ellipseSize/2,-ellipseSize/2);
        highlighted = new QGraphicsEllipseItem(QRectF(
                    selectedSimplex->getCentre()+offset, QSize(ellipseSize,
                        ellipseSize)));
        highlighted->setBrush(Qt::NoBrush);
        QPen h(Qt::DashDotLine);
        h.setWidth(4);
        highlighted->setPen(h);
        scene()->addItem(highlighted);
    }
}

void CycleView::mouseMoveEvent(QMouseEvent * event) {
    if (event->buttons() == Qt::MidButton) {
        setTransformationAnchor(QGraphicsView::NoAnchor);
        QPoint diff = (event->globalPos() - lastPos);
        translate(diff.x()/2, diff.y()/2);
        lastPos = event->globalPos();
        event->accept();
    }
    if ((event->buttons() == Qt::RightButton) && selectedSimplex) {
        QPointF diff;
        // Calculate angle between start of drag and simplex
        diff = (mapToScene(lastPos) - selectedSimplex->getCentre());
        qreal oldAngle = std::atan2(diff.y(), diff.x()) * 180 / M_PI;
        // Calculate angle between endof drag and simplex
        diff = (mapToScene(event->pos()) - selectedSimplex->getCentre());
        qreal newAngle = std::atan2(diff.y(), diff.x()) * 180 / M_PI;
        // Apply the difference between these two angles to simplex
        selectedSimplex->addAngle(newAngle - oldAngle);

        lastPos = event->pos();
        selectedSimplex->markChanged();
        tab->refresh();
        event->accept();
    }
    if ((event->buttons() == Qt::LeftButton) && selectedSimplex) {
        selectedSimplex->setCentre(mapToScene(event->pos())+offset);
        selectedSimplex->markChanged();
        tab->refresh();
        event->accept();
    }
    event->ignore();
}

void CycleView::mousePressEvent(QMouseEvent * event) {
    if (event->buttons() == Qt::MidButton) {
        setTransformationAnchor(QGraphicsView::NoAnchor);
        lastPos = event->globalPos();
        event->accept();
    }
    if (event->buttons() == Qt::RightButton) {
        lastPos = event->pos();
        event->accept();
    }
    if (event->buttons() == Qt::LeftButton) {
        selectedSimplex = NULL;
        //selectedArc = NULL;
        const int fudgeFactor = 5;
        QPoint fudge(-fudgeFactor, -fudgeFactor);
        QList<QGraphicsItem *> itemList = items(
                QRect(event->pos()-fudge, QSize(2*fudgeFactor, 2*fudgeFactor))
                );
        for(auto item: itemList) {
            CycleDrawingData::SimplexRep* r =
                dynamic_cast<CycleDrawingData::SimplexRep*>(item);
            if (r != NULL) {
                selectedSimplex = r;
                offset = mapToScene(event->pos()) - r->getCentre();
                break;
            }
            CycleDrawingData::Arc* a =
                dynamic_cast<CycleDrawingData::Arc*>(item);
            if ((a != NULL) && (a->getSimplex() != NULL)) {
                selectedSimplex = a->getSimplex();
                //selectedArc = a;
                offset = mapToScene(event->pos()) - selectedSimplex->getCentre();
                break;
            }
        }
        setSelected(selectedSimplex);
        tab->refresh();
        event->accept();
    }
    event->ignore();
}


void CycleView::mouseReleaseEvent(QMouseEvent * event) {
    event->ignore();
}

void CycleView::wheelEvent(QWheelEvent * event) {
    qreal newScale;
    // Scale appropriately, but don't get too crazy with scaling
    if (event->delta() > 0)
        newScale = currScale + scaleFactor;
    else
        newScale = currScale - scaleFactor;
    if (newScale < minScale)
        newScale = minScale;
    if (newScale > maxScale)
        newScale = maxScale;
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setTransform(QTransform(newScale, 0, 0, newScale, 0, 0));
    currScale = newScale;
    event->accept();
}

unsigned long Dim3CycleDrawingData::numberOfSimplices() {
    return simps;
}
