
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file anglesui.h
 *  \brief Provides an interface for viewing angle structure lists.
 */

#ifndef __ANGLESUI_H
#define __ANGLESUI_H

#include "angle/anglestructures.h"
#include "triangulation/dim3.h"
#include "../packetui.h"

#include "reginaqt.h"
#include <QAbstractItemModel>

class QLabel;
class QTreeView;

namespace regina {
    class AngleStructures;
    class Packet;
};

class AngleModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the angle structures being displayed
         */
        const regina::AngleStructures* structures_;
        unsigned nCoords;

    public:
        /**
         * Constructor.
         */
        AngleModel(regina::AngleStructures* structures);

        /**
         * Rebuild the model from scratch.
         */
        void rebuild();

        /**
         * Overrides for describing data in the model.
         */
        QModelIndex index(int row, int column,
                const QModelIndex& parent) const override;
        QModelIndex parent(const QModelIndex& index) const override;
        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex& parent) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const override;
};

/**
 * A packet interface for viewing angle structure lists.
 */
class AngleStructureUI : public QObject, public PacketReadOnlyUI,
        regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::AngleStructures>* structures_;
        AngleModel* model;
        bool triDestroyed { false };

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
        AngleStructureUI(regina::PacketOf<regina::AngleStructures>* packet,
                PacketPane* newEnclosingPane);
        ~AngleStructureUI() override;

        /**
         * PacketUI overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        QString getPacketMenuText() const override;
        void refresh() override;

        /**
         * Refresh just the text header above the table.
         */
        void refreshHeader();

        /**
         * PacketListener overrides.
         */
        void packetWasRenamed(regina::Packet&) override;
        void packetWasChanged(regina::Packet&) override;
        void packetBeingDestroyed(regina::PacketShell) override;

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

inline AngleModel::AngleModel(regina::AngleStructures* structures) :
        structures_(structures),
        nCoords(3 * structures_->triangulation().size()) {
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
