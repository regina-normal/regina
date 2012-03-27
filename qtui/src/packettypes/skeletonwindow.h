
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

/*! \file skeletonwindow.h
 *  \brief Provides a viewer for all skeletal objects of a particular
 *  type.
 */

#ifndef __SKELETONWINDOW_H
#define __SKELETONWINDOW_H

#include "packet/npacketlistener.h"

#include <QDialog>
#include <QAbstractItemModel>
#include <QTreeView>

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

class SkeletalModel : public QAbstractItemModel {
    protected:
        /**
         * The triangulation being displayed
         */
        regina::NTriangulation* tri;

        /**
         * Is the model temporarily empty (e.g., when the triangulation
         * is being edited)?
         */
        bool forceEmpty;

    public:
        /**
         * Constructor and destructor.
         */
        SkeletalModel(regina::NTriangulation* tri_);
        virtual ~SkeletalModel();

        /**
         * Make the model temporarily empty.
         */
        void makeEmpty();

        /**
         * Rebuild the model from scratch.
         * If the model was temporarily emptied, it will be refilled.
         */
        void rebuild();

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
        VertexModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;

        /**
         * Helper routine for generating tooltips.
         */
        static QString toolTipForCol(int column);
};

class EdgeModel : public SkeletalModel {
    public:
        /**
         * Constructor.
         */
        EdgeModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;

        /**
         * Helper routine for generating tooltips.
         */
        static QString toolTipForCol(int column);
};

class FaceModel : public SkeletalModel {
    public:
        /**
         * Constructor.
         */
        FaceModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;

        /**
         * Helper routine for generating tooltips.
         */
        static QString toolTipForCol(int column);
};

class ComponentModel : public SkeletalModel {
    public:
        /**
         * Constructor.
         */
        ComponentModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;

        /**
         * Helper routine for generating tooltips.
         */
        static QString toolTipForCol(int column);
};

class BoundaryComponentModel : public SkeletalModel {
    public:
        /**
         * Constructor.
         */
        BoundaryComponentModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing data in the model.
         */
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;

        /**
         * Helper routine for generating tooltips.
         */
        static QString toolTipForCol(int column);
};

/**
 * A tree view with a larger size hint than normal.
 */
class SkeletonTreeView : public QTreeView {
    Q_OBJECT

    public:
        SkeletonTreeView();

    protected:
        virtual QSize sizeHint() const;
};

/**
 * A modeless dialog for viewing all skeletal objects of a particular
 * type in a triangulation.
 *
 * Skeleton windows automatically listen for changes on the underlying
 * triangulation and update themselves when necessary.
 */
class SkeletonWindow : public QDialog, public regina::NPacketListener {
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
        SkeletalModel* model;
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
        ~SkeletonWindow();

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
        static QString overview(SkeletalObject type);
};

inline SkeletalModel::SkeletalModel(regina::NTriangulation* tri_) :
        tri(tri_), forceEmpty(false) {
}

inline SkeletalModel::~SkeletalModel() {
}

inline void SkeletalModel::makeEmpty() {
    beginResetModel();
    forceEmpty = true;
    endResetModel();
}

inline void SkeletalModel::rebuild() {
    beginResetModel();
    forceEmpty = false;
    endResetModel();
}

inline QModelIndex SkeletalModel::index(int row, int column,
        const QModelIndex& /* unused parent */) const {
    return createIndex(row, column, quint32(4 * row + column));
}

inline QModelIndex SkeletalModel::parent(const QModelIndex&) const {
    // All items are top-level.
    return QModelIndex();
}

inline VertexModel::VertexModel(regina::NTriangulation* tri_) :
        SkeletalModel(tri_) {}

inline EdgeModel::EdgeModel(regina::NTriangulation* tri_) :
        SkeletalModel(tri_) {}

inline FaceModel::FaceModel(regina::NTriangulation* tri_) :
        SkeletalModel(tri_) {}

inline ComponentModel::ComponentModel(regina::NTriangulation* tri_) :
        SkeletalModel(tri_) {}

inline BoundaryComponentModel::BoundaryComponentModel(
        regina::NTriangulation* tri_) :
        SkeletalModel(tri_) {}

inline SkeletonTreeView::SkeletonTreeView() : QTreeView() {
}

inline SkeletonWindow::~SkeletonWindow() {
    delete model;
}

#endif
