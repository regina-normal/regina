
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

/*! \file nsurfacecoordinateui.h
 *  \brief Provides a normal surface coordinate viewer.
 */

#ifndef __NSURFACECOODINATEUI_H
#define __NSURFACECOODINATEUI_H

#include "packet/npacketlistener.h"
#include "surfaces/nnormalsurfacelist.h"

#include "../packettabui.h"
#include "coordinates.h"

#include <QAbstractItemModel>
#include <QAction>

class CoordinateChooser;
class PacketChooser;
class QTreeView;

namespace regina {
    class NPacket;
    class NNormalSurfaceList;
    class NSurfaceFilter;
};

class SurfaceModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the normal surfaces being displayed
         */
        regina::NNormalSurfaceList* surfaces_;
        int coordSystem_;

        /**
         * A mapping from table indices to surface indices, for use with
         * filtered lists.
         */
        unsigned* realIndex;
        unsigned nFiltered;

        /**
         * Local modifications
         */
        QString* localName;

        /**
         * Internal status
         */
        bool isReadWrite;

    public:
        /**
         * Constructor and destructor.
         */
        SurfaceModel(regina::NNormalSurfaceList* surfaces, bool readWrite);
        ~SurfaceModel();

        /**
         * Data retrieval.
         */
        regina::NNormalSurfaceList* surfaces() const;
        const regina::NNormalSurface* surface(const QModelIndex& index) const;
        int coordSystem() const;

        /**
         * Rebuild the model from scratch.
         */
        void rebuild(int coordSystem_);
        void rebuild(int coordSystem_, regina::NSurfaceFilter* filter);

        /**
         * Loading and saving local data from/to the packet.
         */
        void refreshNames();
        void commitNames();

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
class NSurfaceCoordinateUI : public QObject, public PacketEditorTab,
        public regina::NPacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        SurfaceModel* model;
        regina::NNormalSurfaceList* surfaces;
        regina::NSurfaceFilter* appliedFilter;

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
        NSurfaceCoordinateUI(regina::NNormalSurfaceList* packet,
            PacketTabbedUI* useParentUI, bool readWrite);
        ~NSurfaceCoordinateUI();

        /**
         * PacketEditorTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        void commit();
        void refresh();
        void setReadWrite(bool readWrite);

        /**
         * NPacketListener overrides.
         */
        void packetToBeDestroyed(regina::NPacket* packet);

    public slots:
        /**
         * Refill the table of surfaces whilst preserving local
         * modifications.
         */
        void refreshLocal();

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
         * Notify us that a surface has been renamed.
         */
        void notifySurfaceRenamed();
};

inline SurfaceModel::~SurfaceModel() {
    delete[] realIndex;
    delete[] localName;
}

inline regina::NNormalSurfaceList* SurfaceModel::surfaces() const {
    return surfaces_;
}

inline const regina::NNormalSurface* SurfaceModel::surface(
        const QModelIndex& index) const {
    return surfaces_->getSurface(realIndex[index.row()]);
}

inline int SurfaceModel::coordSystem() const {
    return coordSystem_;
}

inline QModelIndex SurfaceModel::parent(const QModelIndex& /* unused index*/) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
