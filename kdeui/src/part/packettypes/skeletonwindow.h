
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

/*! \file skeletonwindow.h
 *  \brief Provides a viewer for all skeletal objects of a particular
 *  type.
 */

#ifndef __SKELETONWINDOW_H
#define __SKELETONWINDOW_H

#include <kdialogbase.h>
#include <klistview.h>

class PacketUI;
class QListView;

namespace regina {
    class NBoundaryComponent;
    class NComponent;
    class NEdge;
    class NFace;
    class NTriangulation;
    class NVertex;
};

/**
 * A modeless dialog for viewing all skeletal objects of a particular
 * type in a triangulation.
 *
 * Note that skeleton windows must be manually refreshed when necessary;
 * they do not by default listen for changes on the underlying
 * triangulation.
 */
class SkeletonWindow : public KDialogBase {
    Q_OBJECT

    public:
        /**
         * Types of skeletal objects that can be viewed.
         */
        enum SkeletalObject
            { Vertices, Edges, Faces, Components, BoundaryComponents };

    private:
        /**
         * Packet details
         */
        regina::NTriangulation* tri;
        SkeletalObject objectType;

        /**
         * Internal components
         */
        QListView* table;

    public:
        /**
         * Constructor and destructor.
         */
        SkeletonWindow(PacketUI* parentUI, SkeletalObject viewObjectType);

        /**
         * Update the display.
         */
        void refresh();
        void editingElsewhere();
        void updateCaption();

        /**
         * Return information specific to different skeletal object types.
         */
        static QString typeLabel(SkeletalObject type);
        static QString columnLabel(SkeletalObject type, int column);
};

/**
 * A list view item describing a single skeletal object.  This class is
 * not used on its own, but is instead subclassed for each type of
 * skeletal item.
 */
class SkeletalItem : public KListViewItem {
    public:
        /**
         * Constructor.
         */
        SkeletalItem(QListView* parent);

        /**
         * QListViewItem overrides.
         */
        int width(const QFontMetrics& fm, const QListView* lv, int c) const;
        void paintCell(QPainter* p, const QColorGroup& cg, int column,
            int width, int align);
};

/**
 * A list view item describing a single vertex.
 */
class VertexItem : public SkeletalItem {
    private:
        /**
         * The underlying skeletal item.
         */
        regina::NVertex* item;
        unsigned long itemIndex;

    public:
        /**
         * Constructor.
         */
        VertexItem(QListView* parent, regina::NVertex* vertex,
            unsigned long newItemIndex);

        /**
         * QListViewItem overrides.
         */
        QString text(int column) const;
};

/**
 * A list view item describing a single edge.
 */
class EdgeItem : public SkeletalItem {
    private:
        /**
         * The underlying skeletal item.
         */
        regina::NEdge* item;
        unsigned long itemIndex;

    public:
        /**
         * Constructor.
         */
        EdgeItem(QListView* parent, regina::NEdge* edge,
            unsigned long newItemIndex);

        /**
         * QListViewItem overrides.
         */
        QString text(int column) const;
};

/**
 * A list view item describing a single face.
 */
class FaceItem : public SkeletalItem {
    private:
        /**
         * The underlying skeletal item.
         */
        regina::NFace* item;
        unsigned long itemIndex;

    public:
        /**
         * Constructor.
         */
        FaceItem(QListView* parent, regina::NFace* face,
            unsigned long newItemIndex);

        /**
         * QListViewItem overrides.
         */
        QString text(int column) const;
};

/**
 * A list view item describing a single component.
 */
class ComponentItem : public SkeletalItem {
    private:
        /**
         * The underlying skeletal item.
         */
        regina::NComponent* item;
        unsigned long itemIndex;

    public:
        /**
         * Constructor.
         */
        ComponentItem(QListView* parent, regina::NComponent* component,
            unsigned long newItemIndex);

        /**
         * QListViewItem overrides.
         */
        QString text(int column) const;
};

/**
 * A list view item describing a single boundary component.
 */
class BoundaryComponentItem : public SkeletalItem {
    private:
        /**
         * The underlying skeletal item.
         */
        regina::NBoundaryComponent* item;
        unsigned long itemIndex;

    public:
        /**
         * Constructor.
         */
        BoundaryComponentItem(QListView* parent,
            regina::NBoundaryComponent* bdryCmpt, unsigned long newItemIndex);

        /**
         * QListViewItem overrides.
         */
        QString text(int column) const;
};

inline SkeletalItem::SkeletalItem(QListView* parent) : KListViewItem(parent) {
}

inline VertexItem::VertexItem(QListView* parent, regina::NVertex* vertex,
        unsigned long newItemIndex) :
        SkeletalItem(parent), item(vertex), itemIndex(newItemIndex) {
}

inline EdgeItem::EdgeItem(QListView* parent, regina::NEdge* edge,
        unsigned long newItemIndex) :
        SkeletalItem(parent), item(edge), itemIndex(newItemIndex) {
}

inline FaceItem::FaceItem(QListView* parent, regina::NFace* face,
        unsigned long newItemIndex) :
        SkeletalItem(parent), item(face), itemIndex(newItemIndex) {
}

inline ComponentItem::ComponentItem(QListView* parent,
        regina::NComponent* component, unsigned long newItemIndex) :
        SkeletalItem(parent), item(component), itemIndex(newItemIndex) {
}

inline BoundaryComponentItem::BoundaryComponentItem(QListView* parent,
        regina::NBoundaryComponent* bdryCmpt, unsigned long newItemIndex) :
        SkeletalItem(parent), item(bdryCmpt), itemIndex(newItemIndex) {
}

#endif
