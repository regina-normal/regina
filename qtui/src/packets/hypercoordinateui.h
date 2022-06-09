
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file hypercoordinateui.h
 *  \brief Provides a normal hypersurface coordinate viewer.
 */

#ifndef __HYPERCOODINATEUI_H
#define __HYPERCOODINATEUI_H

#include "hypersurface/normalhypersurfaces.h"
#include "triangulation/dim4.h" // To avoid generic Triangulation<4>.

#include "../packettabui.h"
#include "./coordinates.h" // Use ./ to avoid picking up the iOS header.

#include <QAbstractItemModel>
#include <QAction>

class HyperCoordinateChooser;
class PacketChooser;
class QTreeView;

namespace regina {
    class Packet;
    class NormalHypersurfaces;
};

class HyperModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the normal hypersurfaces being displayed
         */
        regina::NormalHypersurfaces* surfaces_;
        regina::HyperCoords coordSystem_;

    public:
        /**
         * Constructor and destructor.
         */
        HyperModel(regina::NormalHypersurfaces* surfaces);

        /**
         * Data retrieval.
         */
        regina::NormalHypersurfaces* surfaces() const;
        regina::HyperCoords coordSystem() const;

        /**
         * Rebuild all or some of the model from scratch.
         */
        void rebuild(regina::HyperCoords coordSystem_);
        void rebuildUnicode();

        /**
         * Overrides for describing and editing data in the model.
         */
        QModelIndex index(int row, int column,
                const QModelIndex& parent) const override;
        QModelIndex parent(const QModelIndex& index) const override;
        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex& parent) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;
        bool setData(const QModelIndex& index, const QVariant& value,
            int role) override;

        /**
         * Information on the property (non-coordinate) columns.
         */
        int propertyColCount() const;
        QString propertyColName(int whichCol) const;
        QString propertyColDesc(int whichCol) const;
};

/**
 * A normal surface page for viewing surface coordinates.
 */
class HyperCoordinateUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        HyperModel* model;
        regina::PacketOf<regina::NormalHypersurfaces>* surfaces;

        /**
         * Internal components
         */
        QWidget* ui;
        HyperCoordinateChooser* coords;
        QTreeView* table;

        /**
         * Hypersurface list actions
         */
        QAction* actTriangulate;
        std::vector<QAction*> surfaceActionList;

        /**
         * Internal status
         */
        bool currentlyResizing;

    public:
        /**
         * Constructor and destructor.
         */
        HyperCoordinateUI(regina::PacketOf<regina::NormalHypersurfaces>* packet,
            PacketTabbedUI* useParentUI);
        ~HyperCoordinateUI() override;

        /**
         * PacketEditorTab overrides.
         * Note that refresh() is a slot now.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        const std::vector<QAction*>& getPacketTypeActions() override;

    public slots:
        /**
         * More PacketEditorTab overrides.
         */
        void refresh() override;

        /**
         * Hypersurface list actions.
         */
        void triangulate();

        /**
         * Update the states of internal components.
         */
        void updateActionStates();

        /**
         * Provides auto-resizing of columns.
         */
        void columnResized(int section, int oldSize, int newSize);

        /**
         * Note that preferences have changed.
         */
        void updatePreferences();
};

inline HyperModel::HyperModel(regina::NormalHypersurfaces* surfaces) :
        surfaces_(surfaces), coordSystem_(surfaces->coords()) {
}

inline regina::NormalHypersurfaces* HyperModel::surfaces() const {
    return surfaces_;
}

inline regina::HyperCoords HyperModel::coordSystem() const {
    return coordSystem_;
}

inline QModelIndex HyperModel::parent(const QModelIndex& /* unused index*/) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
