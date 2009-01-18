
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

/*! \file skeletonwindow.h
 *  \brief Provides a viewer for all skeletal objects of a particular
 *  type.
 */

#ifndef __SKELETONWINDOW_H
#define __SKELETONWINDOW_H

#include "packet/npacketlistener.h"

#include "../gridlistview.h"

#include <kdialogbase.h>

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
 * Skeleton windows automatically listen for changes on the underlying
 * triangulation and update themselves when necessary.
 */
class SkeletonWindow : public KDialogBase, public regina::NPacketListener {
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
         * NPacketListener overrides.
         */
        void packetWasChanged(regina::NPacket* packet);
        void packetWasRenamed(regina::NPacket* packet);
        void packetToBeDestroyed(regina::NPacket* packet);

        /**
         * Return information specific to different skeletal object types.
         */
        static QString typeLabel(SkeletalObject type);
        static QString columnLabel(SkeletalObject type, int column);
        static QString overview(SkeletalObject type);
};

/**
 * A list view item describing a single skeletal object.  This class is
 * not used on its own, but is instead subclassed for each type of
 * skeletal item.
 */
class SkeletalItem : public GridListViewItem {
    protected:
        /**
         * Properties of the underlying skeletal object.
         */
        regina::NTriangulation* tri;
        unsigned long itemIndex;

    public:
        /**
         * Constructor.
         */
        SkeletalItem(QListView* parent, regina::NTriangulation* useTri,
            unsigned long useItemIndex);

    protected:
        /**
         * Aids the construction of a comma-separated string list.
         */
        static QString& appendToList(QString& list, const QString& item);
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

    public:
        /**
         * Constructor.
         */
        VertexItem(QListView* parent, regina::NTriangulation* useTri,
            unsigned long useItemIndex);

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

    public:
        /**
         * Constructor.
         */
        EdgeItem(QListView* parent, regina::NTriangulation* useTri,
            unsigned long useItemIndex);

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

    public:
        /**
         * Constructor.
         */
        FaceItem(QListView* parent, regina::NTriangulation* useTri,
            unsigned long useItemIndex);

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

    public:
        /**
         * Constructor.
         */
        ComponentItem(QListView* parent, regina::NTriangulation* useTri,
            unsigned long useItemIndex);

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

    public:
        /**
         * Constructor.
         */
        BoundaryComponentItem(QListView* parent, regina::NTriangulation* useTri,
            unsigned long useItemIndex);

        /**
         * QListViewItem overrides.
         */
        QString text(int column) const;
};

inline SkeletalItem::SkeletalItem(QListView* parent,
        regina::NTriangulation* useTri, unsigned long useItemIndex) :
        GridListViewItem(parent), tri(useTri), itemIndex(useItemIndex) {
}

inline QString& SkeletalItem::appendToList(QString& list,
        const QString& item) {
    return (list.isEmpty() ? (list = item) : (list.append(", ").append(item)));
}

#endif
