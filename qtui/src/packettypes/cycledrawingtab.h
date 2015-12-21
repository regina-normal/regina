
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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

/*! \file cycledrawingtab.h
 *  \brief Provides a tab for viewing the cycle drawing of triangulations of
 *  arbitrary dimension.
 */

#ifndef __CYCLEDRAWINGTAB_H
#define __CYCLEDRAWINGTAB_H

#include <deque>

#include <QGraphicsPathItem>
#include <QGraphicsView>

#include "packettabui.h"
#include "skeletonwindow.h"

class CycleView;

class MessageLayer;
class QBoxLayout;
class QGraphicsScene;
class QStackedWidget;

namespace regina {
    class Dim2Triangulation;
    class NPacket;
    class NTriangulation;
};

/**
 * Contains dimension-agnostic information for viewing a cycle drawing.  Each
 * dimension requires its own separate subclass.
 */
class CycleDrawingData {
    protected:
        /**
         * A dimension-agnostic representation of a simplex.
         */
        class SimplexRep {
            public:
                /**
                 * Adjusts the rotation of the representation. Note that this
                 * adds to any current rotation, it doesn't set the rotation
                 * value to parameter value.
                 */
                virtual void addAngle(qreal) = 0;

                /**
                 * Returns the centre point of the representation.
                 */
                virtual QPointF getCentre() = 0;

                /**
                 * Sets the centre point of the representation.
                 */
                virtual void setCentre(QPointF) = 0;

                /**
                 * To be called if the simplex changes. This will often require
                 * calling markChanged on any arcs incident to this simplex.
                 */
                virtual void markChanged() = 0;
        };

        /**
         * A simple arc to be drawn. Should be subclassed by dimension-specific
         * subclasses of CycleDrawingData.
         */
        class Arc : public QGraphicsPathItem {
            private:
                int edgeIndex;

                /**
                 * The pen to use to draw this arc. Stores colour and width
                 * information, as well as stroke type.
                 */
                QPen p;

                /**
                 * Has this been added to a scene.
                 */
                bool added;

                /**
                 * Does this arc need to be refreshed on the screen.
                 */
                bool needsRefresh;

                /**
                 * An array of 8 nicely-visible colours to draw with, as
                 * determined by me. Also an accessor function.
                 */
                static constexpr int numColours = 8;
                static QColor colours[numColours];
                static inline QColor getColour(int);

            public:
                /**
                 * Basic constructor.
                 */
                Arc();

                /**
                 * Sets the edge index of the arc, i.e. which edge of the
                 * triangulation this arc partially represents.
                 */
                void setEdgeIndex(int);

                /**
                 * Sets the colour of this arc.
                 */
                void setColour();

                /**
                 * Gets the simplex corresponding to this arc. If no such
                 * simplex exists, return NULL.
                 */
                virtual CycleDrawingData::SimplexRep* getSimplex() = 0;

                /**
                 * Draws this arc onto the given scene.
                 */
                void draw(QGraphicsScene *);

                /**
                 * We only re-draw arcs if they have changed since the last
                 * time it was drawn. needsRedrawing() returns true if and only
                 * if markChanged() has been called since the last draw() call.
                 */
                bool needsRedrawing();
                void markChanged();

                /**
                 * This function updates the path that this arc takes. Note
                 * that this does not call markChanged() as there's no
                 * guarantee that the new path actually differs from the old.
                 * If the new path might potentially differ, call markChanged()
                 * as well.
                 */
                virtual void updatePath() = 0;
        };

    public:
        virtual ~CycleDrawingData() {}

        virtual regina::NPacket* getPacket() = 0;
        virtual unsigned long numberOfSimplices() = 0;
        virtual QString simplicesName() = 0;
        virtual QString overview() = 0;

        /**
         * Called when the underlying packet changes.
         */
        virtual void refresh() = 0;

        /**
         * Returns all arcs in the drawing.
         */
        virtual std::vector<std::deque<Arc*>> & getArcs() = 0;

    /**
     * The CycleView object needs to interact with simplex representations to
     * move and rotate them.
     */
    friend class CycleView;

    /**
     * Dimension 3 specific class.
     */
    friend class Dim3CycleDrawingData;
};

class Dim3CycleDrawingData : public CycleDrawingData {
    protected:
        class InternalArc;
        class ExternalArc;

        /**
         * A SimplexRep class specific to dimension 3.
         */
        class Dim3SimplexRep : public SimplexRep {
            private:
                /**
                 * The centre of the object.
                 */
                QPointF centre;

                /**
                 * The size of the object. Note that the simplex extends for
                 * about (2*scale) in each direction. The units for this object
                 * are arbitrary. They can be thought of as pixels, but as
                 * zooming is allowed this is not quite correct.
                 */
                static qreal scale;

                /**
                 * How far out the control points for the cubic bezier curves
                 * should be. Note that this value is multiplied by scale (see
                 * above) to allow for proper scaling.
                 */
                static qreal cubic;

                /**
                 * The rotation of this object.
                 */
                qreal angle;

                /**
                 * An array of the 6 internal arcs corresponding to this
                 * simplex.
                 */
                InternalArc *internal[6];

            public:
                /**
                 * Basic constructor.
                 */
                Dim3SimplexRep(int simp);

                /**
                 * Get the appropriate internal arc, given an index. This index
                 * is the same as that used in NEdge.
                 */
                InternalArc* getInternal(int);

                /**
                 * Get the rotation of this object.
                 */
                qreal getAngle();

                /**
                 * Increase (or decrease) the rotation of this object. Note
                 * that this function does not set the absolute value of the
                 * rotation, it merely adds to the current rotation.
                 */
                void addAngle(qreal);

                /**
                 * Get the centre of the object.
                 */
                QPointF getCentre();

                /**
                 * Set the centre of the object.
                 */
                void setCentre(QPointF);

                /**
                 * Mark as having been changed.
                 */
                void markChanged();
        };


        /**
         * A representation of one of the internal arcs of a simplex. Note that
         * an internal arc has two ends, each of which leaves through a
         * different face of the simplex. We always use 0 to denote the arc end
         * which leaves this simplex in the lower face, and 1 to denote the arc
         * end which leaves this simplex in the upper face.
         */
        class InternalArc : public Arc {
            private:

                /**
                 * The simplex to which this internal arc belongs.
                 */
                Dim3SimplexRep *simp;

                /**
                 * The two external arcs hanging off this internal arc. Note
                 * that either value (or both) could be NULL if there is no
                 * arc. externals[0] will always be the arc leaving from the
                 * lower face.
                 */
                ExternalArc *externals[2];
            public:

                /**
                 * Basic constructor. Sets the simplex, and the five points
                 * described below.
                 */
                InternalArc(Dim3SimplexRep*, QPointF, QPointF, QPointF, QPointF, QPointF);

                /**
                 * These five points are used to draw the arcs. endA and endB
                 * are the two ends of this internal arc. mid is the midway
                 * point of this internal arc. cubicA and cubicB are both
                 * control points when drawing the cubic bezier curve extending
                 * from either end of this arc.
                 */
                QPointF endA, cubicA, mid, endB, cubicB;

                /**
                 * Gets the cubic control point for a given "end" of an
                 * internal arc.
                 */
                QPointF getCubic(int);

                /**
                 * Gets the appropriate end point of an internal arc.
                 */
                QPointF getEnd(int);

                /**
                 * Gets the midway point of an internal arc.
                 */
                QPointF getMid();

                /**
                 * Gets the corresponding external arc which continues this
                 * internal arc. Note that this can (and will) return NULL if
                 * no such arc exists.
                 */
                ExternalArc* getExternal(int);

                /**
                 * Sets the external arc which continues this internal arc out
                 * a given end.
                 */
                void setExternal(int, ExternalArc*);

                /**
                 * Updates the path drawing of this arc.
                 */
                void updatePath();

                /**
                 * Gets the simplex to which this arc belongs.
                 */
                Dim3SimplexRep* getSimplex();
        };

        /**
         * A representation of one of the external arcs of a simplex.
         */
        class ExternalArc : public Arc {
            private:

                /**
                 * The two internal arcs to either side of this external arc.
                 * These are not ordered in any useful way.
                 */
                InternalArc* internals[2];
            public:

                /**
                 * Sets one of the two internal arcs to either side of this
                 * external arc.
                 */
                void setInternal(int, InternalArc*);

                /**
                 * Updates the path drawing of this arc.
                 */
                void updatePath();

                /**
                 * Gets the simplex to which this path belongs. This is a
                 * required inheritance from Arc, and is needed as CycleView
                 * needs to know what to do if this arc is clicked on. Note
                 * that this function always returns NULL, so CycleView knows
                 * to do nothing.
                 */
                Dim3SimplexRep* getSimplex();
        };
    private:

        /**
         * The triangulation.
         */
        regina::NTriangulation* tri_;

        /**
         * The number of simplices
         */
        int simps;

        /**
         * An array of the simplices.
         */
        Dim3SimplexRep** simplices;

        /**
         * All of the arcs used to represent this triangulation.
         */
        std::vector<std::deque<CycleDrawingData::Arc*>> arcs;

    public:

        /**
         * Basic constructor.
         */
        Dim3CycleDrawingData(regina::NTriangulation* tri);

        /**
         * Basic destructor.
         */
        ~Dim3CycleDrawingData();

        /**
         * Retrieve a collection of all the arcs of this drawing.
         */
        std::vector<std::deque<Dim3CycleDrawingData::Arc*>> & getArcs();

        /**
         * Get the packet.
         */
        regina::NPacket* getPacket();

        /**
         * Return the number of simplices this drawing represents.
         */
        unsigned long numberOfSimplices();

        /**
         * Get a simplex representation from a given index.
         */
        Dim3SimplexRep* getSimplex(int);

        /**
         * The human-readable name of a simplex
         */
        QString simplicesName();

        /**
         * An overview of this object.
         */
        QString overview();

        /**
         * Called when the packet is updated.
         */
        void refresh();
};


/**
 * A packet viewer tab for viewing the cycle drawing of a triangulation.
 * This can work with triangulations of arbitrary dimension, as long as
 * an appropriate subclass of CycleDrawingData is defined.
 */
class CycleDrawingTab : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet and graphing details
         */
        CycleDrawingData* data;

        /**
         * Internal components
         */
        QWidget* ui;
        QStackedWidget* stack;
        QBoxLayout* baseLayout;
        MessageLayer* layerInfo;
        MessageLayer* layerError;
        QScrollArea* layerScene;
        QGraphicsScene* scene;
        CycleView* view;


    public:
        /**
         * Constructor and destructor.
         * This object will take ownership of \a useData.
         */
        CycleDrawingTab(CycleDrawingData* useData,
                PacketTabbedViewerTab* useParentUI);
        ~CycleDrawingTab();

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();


    private:
        /**
         * Set up internal components.
         */
        void showInfo(const QString& msg);
        void showError(const QString& msg);
};

/**
 * This view class allows appropriate mouse manipulation of a cycle drawing.
 */
class CycleView : public QGraphicsView {
    public:

        /**
         * Basic constructor
         */
        CycleView(QGraphicsScene*, CycleDrawingTab *);

        /**
         * Movement events.
         */
        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
        void wheelEvent(QWheelEvent *);

        /**
         * Drawing functions.
         */
        void drawHighlight();
        void drawArcs(CycleDrawingData*);

        /**
         * Stores the selected simplex.
         */
        void setSelected(CycleDrawingData::SimplexRep*);

    private:

        /**
         * The selected simplex.
         */
        CycleDrawingData::SimplexRep* selectedSimplex;

        /**
         * An ellipse to surround the selected simplex.
         */
        QGraphicsEllipseItem *highlighted;

        /**
         * The last position the mouse was in when the user interacted with
         * this view. We use this to track movement and rotation.
         */
        QPoint lastPos;

        /**
         * Scaling limitations.
         */
        const qreal minScale, maxScale, scaleFactor;

        /**
         * The user will often select a simplex by clicking near a simplex,
         * rather than exactly on it. This offset stores the distance between
         * the click and the centre of the simplex, so we can move the simplex
         * more accurately.
         */
        QPointF offset;

        /**
         * The current scale of the view.
         */
        qreal currScale;

        /**
         * The drawing tab.
         */
        CycleDrawingTab *tab;
};

// Inline functions

// CycleDrawingData::Arc

inline QColor CycleDrawingData::Arc::getColour(int idx) {
    return colours[idx % numColours];
}


inline bool Dim3CycleDrawingData::Arc::needsRedrawing() {
    return needsRefresh;
}

inline void Dim3CycleDrawingData::Arc::markChanged() {
    needsRefresh = true;
}

// Dim3CycleDrawingData

inline Dim3CycleDrawingData::Dim3SimplexRep*
        Dim3CycleDrawingData::getSimplex(int i) {
    return simplices[i];
}

// Dim3CycleDrawingData::Dim3SimplexRep

inline qreal Dim3CycleDrawingData::Dim3SimplexRep::getAngle() {
    return angle;
}

inline QPointF Dim3CycleDrawingData::Dim3SimplexRep::getCentre() {
    return centre;
}

inline void Dim3CycleDrawingData::Dim3SimplexRep::setCentre(QPointF c) {
    centre = c;
}

inline Dim3CycleDrawingData::InternalArc*
        Dim3CycleDrawingData::Dim3SimplexRep::getInternal(int i) {
    return internal[i];
}

// Dim3CycleDrawingData::InternalArc

inline Dim3CycleDrawingData::Dim3SimplexRep*
        Dim3CycleDrawingData::InternalArc::getSimplex() {
    return simp;
}

inline Dim3CycleDrawingData::ExternalArc*
        Dim3CycleDrawingData::InternalArc::getExternal(int i) {
    return externals[i];
}

inline void Dim3CycleDrawingData::InternalArc::setExternal(int i,
        Dim3CycleDrawingData::ExternalArc *a) {
    externals[i] = a;
}

// Dim3CycleDrawingData::ExternalArc

inline void Dim3CycleDrawingData::ExternalArc::setInternal(int i,
        Dim3CycleDrawingData::InternalArc *a) {
    internals[i] = a;
}

inline Dim3CycleDrawingData::Dim3SimplexRep*
        Dim3CycleDrawingData::ExternalArc::getSimplex() {
    return NULL;
}

// CycleView

inline void CycleView::setSelected(CycleDrawingData::SimplexRep *s) {
    selectedSimplex = s;
}

// CycleDrawingTab

inline CycleDrawingTab::~CycleDrawingTab() {
    delete data;
}



#endif
