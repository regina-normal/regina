
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

/*! \file surfacescoordinateui.h
 *  \brief Provides a normal surface coordinate viewer.
 */

#ifndef __SURFACESCOORDINATEUI_H
#define __SURFACESCOORDINATEUI_H

#include "packet/packetlistener.h"
#include "surfaces/normalsurfaces.h"

#include "../packettabui.h"
#include "./coordinates.h" // Use ./ to avoid picking up the iOS header.

#include <QAbstractItemModel>
#include <QAction>

class CoordinateChooser;
class PacketChooser;
class QTreeView;

namespace regina {
    class Packet;
    class NormalSurfaces;
    class SurfaceFilter;
};

class SurfaceModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the normal surfaces being displayed
         */
        regina::NormalSurfaces* surfaces_;
        regina::NormalCoords coordSystem_;

        /**
         * A mapping from table indices to surface indices, for use with
         * filtered lists.
         */
        unsigned* realIndex;
        unsigned nFiltered;

        /**
         * Internal status
         */
        bool isReadWrite;

    public:
        /**
         * Constructor and destructor.
         */
        SurfaceModel(regina::NormalSurfaces* surfaces, bool readWrite);
        ~SurfaceModel();

        /**
         * Data retrieval.
         */
        regina::NormalSurfaces* surfaces() const;
        const regina::NormalSurface* surface(const QModelIndex& index) const;
        size_t surfaceIndex(const QModelIndex& index) const;
        regina::NormalCoords coordSystem() const;

        /**
         * Rebuild all or some of the model from scratch.
         */
        void rebuild(regina::NormalCoords coordSystem_);
        void rebuild(regina::NormalCoords coordSystem_,
            regina::SurfaceFilter* filter);
        void rebuildUnicode();

        /**
         * Updating read/write status.
         */
        void setReadWrite(bool readWrite);

        /**
         * Overrides for describing and editing data in the model.
         */
        QModelIndex index(int row, int column,
                const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& index) const;
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;
        Qt::ItemFlags flags(const QModelIndex& index) const;
        bool setData(const QModelIndex& index, const QVariant& value, int role);

        /**
         * Information on the property (non-coordinate) columns.
         */
        unsigned propertyColCount() const;
        QString propertyColName(int whichCol) const;
        QString propertyColDesc(int whichCol) const;
};

/**
 * A normal surface page for viewing surface coordinates.
 */
class SurfacesCoordinateUI : public QObject, public PacketEditorTab,
        public regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        SurfaceModel* model;
        regina::NormalSurfaces* surfaces;
        regina::SurfaceFilter* appliedFilter;

        /**
         * Internal components
         */
        QWidget* ui;
        CoordinateChooser* coords;
        PacketChooser* filter;
        QTreeView* table;

        /**
         * Surface list actions
         */
        QAction* actCutAlong;
        QAction* actCrush;
        QLinkedList<QAction*> surfaceActionList;

        /**
         * Internal status
         */
        bool isReadWrite;
        bool currentlyResizing;

    public:
        /**
         * Constructor and destructor.
         */
        SurfacesCoordinateUI(regina::NormalSurfaces* packet,
            PacketTabbedUI* useParentUI, bool readWrite);
        ~SurfacesCoordinateUI();

        /**
         * PacketEditorTab overrides.
         * Note that refresh() is a slot now.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        void setReadWrite(bool readWrite);

        /**
         * PacketListener overrides.
         */
        void packetToBeDestroyed(regina::Packet* packet);

    public slots:
        /**
         * More PacketEditorTab overrides.
         */
        void refresh();

        /**
         * Surface list actions.
         */
        void cutAlong();
        void crush();

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

inline SurfaceModel::~SurfaceModel() {
    delete[] realIndex;
}

inline regina::NormalSurfaces* SurfaceModel::surfaces() const {
    return surfaces_;
}

inline const regina::NormalSurface* SurfaceModel::surface(
        const QModelIndex& index) const {
    return surfaces_->surface(realIndex[index.row()]);
}

inline size_t SurfaceModel::surfaceIndex(const QModelIndex& index) const {
    return realIndex[index.row()];
}

inline regina::NormalCoords SurfaceModel::coordSystem() const {
    return coordSystem_;
}

inline QModelIndex SurfaceModel::parent(const QModelIndex& /* unused index*/) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
