
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

#include <KDialog>
#include <QAbstractItemModel>

class PacketUI;
class QTreeView;

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
class SkeletonWindow : public KDialog, public regina::NPacketListener {
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
        QTreeView* table;

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
        static QString overview(SkeletalObject type);
};

class SkeletalModel : public QAbstractItemModel {
    protected:
        /**
         * The triangulation being displayed
         */
        regina::NTriangulation* tri;

    public:
        /**
         * Constructor.
         */
        SkeletalModel(QObject* parent, regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        QModelIndex index(int row, int column,
            const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& index) const;
};

class VertexModel : public SkeletalModel {
    public:
        /**
         * Constructor.
         */
        VertexModel(QObject* parent, regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;
};

class EdgeModel : public SkeletalModel {
    public:
        /**
         * Constructor.
         */
        EdgeModel(QObject* parent, regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;
};

class FaceModel : public SkeletalModel {
    public:
        /**
         * Constructor.
         */
        FaceModel(QObject* parent, regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;
};

class ComponentModel : public SkeletalModel {
    public:
        /**
         * Constructor.
         */
        ComponentModel(QObject* parent, regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;
};

class BoundaryComponentModel : public SkeletalModel {
    public:
        /**
         * Constructor.
         */
        BoundaryComponentModel(QObject* parent, regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;
};

inline SkeletalModel::SkeletalModel(QObject* parent,
        regina::NTriangulation* tri_) :
        QAbstractItemModel(parent), tri(tri_) {}

inline QModelIndex SkeletalModel::index(int row, int column,
        const QModelIndex& parent) const {
    if (parent.isValid())
        return QModelIndex();
    else
        return createIndex(row, column, 4 * row + column);
}

inline QModelIndex SkeletalModel::parent(const QModelIndex&) const {
    // All items are top-level.
    return QModelIndex();
}

inline VertexModel::VertexModel(QObject* parent, regina::NTriangulation* tri_) :
        SkeletalModel(parent, tri_) {}

inline EdgeModel::EdgeModel(QObject* parent, regina::NTriangulation* tri_) :
        SkeletalModel(parent, tri_) {}

inline FaceModel::FaceModel(QObject* parent, regina::NTriangulation* tri_) :
        SkeletalModel(parent, tri_) {}

inline ComponentModel::ComponentModel(QObject* parent,
        regina::NTriangulation* tri_) :
        SkeletalModel(parent, tri_) {}

inline BoundaryComponentModel::BoundaryComponentModel(QObject* parent,
        regina::NTriangulation* tri_) :
        SkeletalModel(parent, tri_) {}

#endif
