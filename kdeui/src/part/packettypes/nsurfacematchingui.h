
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

/*! \file nsurfacematchingui.h
 *  \brief Provides a matching equation viewer for normal surface lists.
 */

#ifndef __NSURFACEMATCHINGUI_H
#define __NSURFACEMATCHINGUI_H

#include "maths/nmatrixint.h"
#include "../packettabui.h"

#include <memory>
#include <qabstractitemmodel.h>

class QTreeView;

namespace regina {
    class NMatrixInt;
    class NNormalSurfaceList;
    class NPacket;
    class NTriangulation;
};

class MatchingModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the matching equations being displayed
         */
        std::auto_ptr<regina::NMatrixInt> eqns_;
        regina::NNormalSurfaceList* surfaces_;

    public:
        /**
         * Constructor.
         */
        MatchingModel(regina::NNormalSurfaceList* surfaces);

        /**
         * Data retrieval.
         */
        regina::NNormalSurfaceList* surfaces() const;

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
 * A surface list page for viewing matching equations.
 */
class NSurfaceMatchingUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Matrix details
         */
        MatchingModel* model;

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
        NSurfaceMatchingUI(regina::NNormalSurfaceList* packet,
                PacketTabbedUI* useParentUI);
        ~NSurfaceMatchingUI();

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();

    protected slots:
        /**
         * Provides auto-resizing of columns.
         */
        void columnResized(int section, int oldSize, int newSize);
};

inline MatchingModel::MatchingModel(regina::NNormalSurfaceList* surfaces) :
        surfaces_(surfaces) {
}

inline regina::NNormalSurfaceList* MatchingModel::surfaces() const {
    return surfaces_;
}

inline QModelIndex MatchingModel::index(int row, int column,
        const QModelIndex& /* unused parent */) const {
    if (eqns_.get())
        return createIndex(row, column,
            quint32(eqns_->columns() * row + column));
    else
        return createIndex(row, column, quint32(0));
}

inline QModelIndex MatchingModel::parent(const QModelIndex& /* unused index */) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
