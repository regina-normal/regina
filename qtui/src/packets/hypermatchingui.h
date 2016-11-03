
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file hypermatchingui.h
 *  \brief Provides a matching equation viewer for normal hypersurface lists.
 */

#ifndef __HYPERMATCHINGUI_H
#define __HYPERMATCHINGUI_H

#include "maths/matrix.h"
#include "triangulation/forward.h"
#include "../packettabui.h"

#include <memory>
#include <qabstractitemmodel.h>

class QTreeView;

namespace regina {
    class NormalHypersurfaces;
    class Packet;
};

class HyperMatchingModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the matching equations being displayed
         */
        std::unique_ptr<regina::MatrixInt> eqns_;
        regina::NormalHypersurfaces* surfaces_;

    public:
        /**
         * Constructor.
         */
        HyperMatchingModel(regina::NormalHypersurfaces* surfaces);

        /**
         * Data retrieval.
         */
        regina::NormalHypersurfaces* surfaces() const;

        /**
         * Rebuild the model from scratch.
         */
        void rebuild();

        /**
         * Overrides for describing data in the model.
         */
        QModelIndex index(int row, int column,
                const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& index) const;
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;
};

/**
 * A hypersurface list page for viewing matching equations.
 */
class HyperMatchingUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Matrix details
         */
        HyperMatchingModel* model;

        /**
         * Internal components
         */
        QWidget* ui;
        QTreeView* table;

        /**
         * Status of any ongoing actions.
         */
        bool currentlyAutoResizing;
        bool everRefreshed;

    public:
        /**
         * Constructor and destructor.
         */
        HyperMatchingUI(regina::NormalHypersurfaces* packet,
                PacketTabbedUI* useParentUI);
        ~HyperMatchingUI();

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

    protected slots:
        /**
         * Provides auto-resizing of columns.
         */
        void columnResized(int section, int oldSize, int newSize);
};

inline HyperMatchingModel::HyperMatchingModel(
        regina::NormalHypersurfaces* surfaces) :
        surfaces_(surfaces) {
}

inline regina::NormalHypersurfaces* HyperMatchingModel::surfaces() const {
    return surfaces_;
}

inline QModelIndex HyperMatchingModel::index(int row, int column,
        const QModelIndex& /* unused parent */) const {
    if (eqns_.get())
        return createIndex(row, column,
            quintptr(eqns_->columns() * row + column));
    else
        return createIndex(row, column, quintptr(0));
}

inline QModelIndex HyperMatchingModel::parent(
        const QModelIndex& /* unused index */) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
