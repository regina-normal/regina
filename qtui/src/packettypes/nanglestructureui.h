
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

/*! \file nanglestructureui.h
 *  \brief Provides an interface for viewing angle structure lists.
 */

#ifndef __NANGLESTRUCTUREUI_H
#define __NANGLESTRUCTUREUI_H

#include "angle/nanglestructurelist.h"
#include "triangulation/ntriangulation.h"
#include "../packetui.h"

#include <QAbstractItemModel>

class QLabel;
class QTreeView;

namespace regina {
    class NAngleStructureList;
    class NPacket;
};

class AngleModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the angle structures being displayed
         */
        regina::NAngleStructureList* structures_;
        unsigned nCoords;

    public:
        /**
         * Constructor.
         */
        AngleModel(regina::NAngleStructureList* structures);

        /**
         * Data retrieval.
         */
        regina::NAngleStructureList* structures() const;

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
 * A packet interface for viewing angle structure lists.
 */
class NAngleStructureUI : public QObject, public PacketReadOnlyUI,
        regina::NPacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        AngleModel* model;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* stats;
        QTreeView* table;

        /**
         * Status of any ongoing actions.
         */
        bool currentlyAutoResizing;

    public:
        /**
         * Constructor and destructor.
         */
        NAngleStructureUI(regina::NAngleStructureList* packet,
                PacketPane* newEnclosingPane);
        ~NAngleStructureUI();

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void refresh();

        /**
         * Refresh just the text header above the table.
         */
        void refreshHeader();

        /**
         * NPacketListener overrides.
         */
        void packetWasRenamed(regina::NPacket* packet);
        
    public slots:
        /**
         * View the underlying triangulation.
         */
        void viewTriangulation();

        /**
         * Provides auto-resizing of columns.
         */
        void columnResized(int section, int oldSize, int newSize);
};

inline AngleModel::AngleModel(regina::NAngleStructureList* structures) :
        structures_(structures),
        nCoords(3 * structures_->getTriangulation()->getNumberOfTetrahedra()) {
}

inline regina::NAngleStructureList* AngleModel::structures() const {
    return structures_;
}

inline QModelIndex AngleModel::index(int row, int column,
        const QModelIndex& /* unused parent */) const {
    return createIndex(row, column, quint32((nCoords + 1) * row + column));
}

inline QModelIndex AngleModel::parent(const QModelIndex& /* unused index */) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
