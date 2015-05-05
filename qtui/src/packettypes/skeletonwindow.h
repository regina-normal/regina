
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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
    template <int> class Triangulation;
    typedef Triangulation<2> Dim2Triangulation;
    typedef Triangulation<3> NTriangulation;
};

/**
 * A base class for models for viewing skeletal data of a particular type
 * for triangulations of a particular dimension.  Each type of skeletal
 * data for each type of triangulation requires its own separate subclass.
 *
 * \pre Subclasses may use at most \e four columns.  This number is
 * currently hard-coded into the SkeletalModel code.
 *
 * See SkeletonWindow for further details.
 */
class SkeletalModel : public QAbstractItemModel {
    protected:
        /**
         * Is the model temporarily empty (e.g., when the triangulation
         * is being edited)?
         */
        bool forceEmpty;

    public:
        /**
         * Constructor and destructor.
         */
        SkeletalModel();
        virtual ~SkeletalModel();

        /**
         * General information about the subclass of model.
         */
        virtual QString caption() const = 0;
        virtual QString overview() const = 0;

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

/**
 * A modeless dialog for viewing all skeletal objects of a particular
 * type in a triangulation.
 *
 * Skeleton windows automatically listen for changes on the underlying
 * triangulation and update themselves when necessary.
 *
 * This class can work with any dimensional face of any dimensional
 * triangulation, as long as an appropriate SkeletalModel subclass is
 * available.
 */
class SkeletonWindow : public QDialog, public regina::NPacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        SkeletalModel* model;

        /**
         * Internal components
         */
        QTreeView* table;

    public:
        /**
         * Constructor and destructor.
         */
        SkeletonWindow(PacketUI* parentUI, SkeletalModel* useModel);
        ~SkeletonWindow();

        /**
         * Update the display.
         */
        void refresh();
        void updateCaption();

        /**
         * NPacketListener overrides.
         */
        void packetWasChanged(regina::NPacket* packet);
        void packetWasRenamed(regina::NPacket* packet);
        void packetToBeDestroyed(regina::NPacket* packet);
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

class VertexModel : public SkeletalModel {
    private:
        /**
         * The triangulation being displayed
         */
        regina::NTriangulation* tri;

    public:
        /**
         * Constructor.
         */
        VertexModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing this subclass of model.
         */
        virtual QString caption() const;
        virtual QString overview() const;

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
    private:
        /**
         * The triangulation being displayed
         */
        regina::NTriangulation* tri;

    public:
        /**
         * Constructor.
         */
        EdgeModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing this subclass of model.
         */
        virtual QString caption() const;
        virtual QString overview() const;

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

class TriangleModel : public SkeletalModel {
    private:
        /**
         * The triangulation being displayed
         */
        regina::NTriangulation* tri;

    public:
        /**
         * Constructor.
         */
        TriangleModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing this subclass of model.
         */
        virtual QString caption() const;
        virtual QString overview() const;

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
    private:
        /**
         * The triangulation being displayed
         */
        regina::NTriangulation* tri;

    public:
        /**
         * Constructor.
         */
        ComponentModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing this subclass of model.
         */
        virtual QString caption() const;
        virtual QString overview() const;

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
    private:
        /**
         * The triangulation being displayed
         */
        regina::NTriangulation* tri;

    public:
        /**
         * Constructor.
         */
        BoundaryComponentModel(regina::NTriangulation* tri_);

        /**
         * Overrides for describing this subclass of model.
         */
        virtual QString caption() const;
        virtual QString overview() const;

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

class Dim2VertexModel : public SkeletalModel {
    private:
        /**
         * The triangulation being displayed
         */
        regina::Dim2Triangulation* tri;

    public:
        /**
         * Constructor.
         */
        Dim2VertexModel(regina::Dim2Triangulation* tri_);

        /**
         * Overrides for describing this subclass of model.
         */
        virtual QString caption() const;
        virtual QString overview() const;

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

class Dim2EdgeModel : public SkeletalModel {
    private:
        /**
         * The triangulation being displayed
         */
        regina::Dim2Triangulation* tri;

    public:
        /**
         * Constructor.
         */
        Dim2EdgeModel(regina::Dim2Triangulation* tri_);

        /**
         * Overrides for describing this subclass of model.
         */
        virtual QString caption() const;
        virtual QString overview() const;

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

class Dim2ComponentModel : public SkeletalModel {
    private:
        /**
         * The triangulation being displayed
         */
        regina::Dim2Triangulation* tri;

    public:
        /**
         * Constructor.
         */
        Dim2ComponentModel(regina::Dim2Triangulation* tri_);

        /**
         * Overrides for describing this subclass of model.
         */
        virtual QString caption() const;
        virtual QString overview() const;

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

class Dim2BoundaryComponentModel : public SkeletalModel {
    private:
        /**
         * The triangulation being displayed
         */
        regina::Dim2Triangulation* tri;

    public:
        /**
         * Constructor.
         */
        Dim2BoundaryComponentModel(regina::Dim2Triangulation* tri_);

        /**
         * Overrides for describing this subclass of model.
         */
        virtual QString caption() const;
        virtual QString overview() const;

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

inline SkeletalModel::SkeletalModel() : forceEmpty(false) {
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

inline SkeletonTreeView::SkeletonTreeView() : QTreeView() {
}

inline SkeletonWindow::~SkeletonWindow() {
    delete model;
}

inline void SkeletonWindow::updateCaption() {
    setWindowTitle(model->caption());
}

inline VertexModel::VertexModel(regina::NTriangulation* tri_) :
        tri(tri_) {}

inline EdgeModel::EdgeModel(regina::NTriangulation* tri_) :
        tri(tri_) {}

inline TriangleModel::TriangleModel(regina::NTriangulation* tri_) :
        tri(tri_) {}

inline ComponentModel::ComponentModel(regina::NTriangulation* tri_) :
        tri(tri_) {}

inline BoundaryComponentModel::BoundaryComponentModel(
        regina::NTriangulation* tri_) : tri(tri_) {}

inline Dim2VertexModel::Dim2VertexModel(regina::Dim2Triangulation* tri_) :
        tri(tri_) {}

inline Dim2EdgeModel::Dim2EdgeModel(regina::Dim2Triangulation* tri_) :
        tri(tri_) {}

inline Dim2ComponentModel::Dim2ComponentModel(regina::Dim2Triangulation* tri_) :
        tri(tri_) {}

inline Dim2BoundaryComponentModel::Dim2BoundaryComponentModel(
        regina::Dim2Triangulation* tri_) : tri(tri_) {}

#endif
