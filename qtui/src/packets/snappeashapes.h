
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

/*! \file snappeashapes.h
 *  \brief Provides access to tetrahedron shapes and cusp data for
 *  SnapPea triangulations.
 */

#ifndef __SNAPPEASHAPES_H
#define __SNAPPEASHAPES_H

#include "../packettabui.h"

#include <QAbstractItemModel>

class EditTreeView;
class QAction;
class QToolBar;
class QTreeWidget;
class QTreeWidgetItem;

namespace regina {
    class SnapPeaTriangulation;
};

/**
 * Note: We always allow editing the fillings in the cusp table.
 */
class CuspModel : public QAbstractItemModel {
    private:
        /**
         * Details of the cusps being displayed.
         */
        regina::SnapPeaTriangulation* tri_;

    public:
        /**
         * Constructor and destructor.
         */
        CuspModel(regina::SnapPeaTriangulation* tri);

        /**
         * Rebuild the model from scratch.
         */
        void rebuild();

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
};

/**
 * A triangulation page for viewing normal surface properties.
 */
class SnapPeaShapesUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        CuspModel* model;
        regina::PacketOf<regina::SnapPeaTriangulation>* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        EditTreeView* cusps;
        QTreeWidget* shapes;

        /**
         * Actions
         */
        QAction* actRandomise;
        QAction* actFill;
        QAction* actToRegina;
        std::vector<QAction*> triActionList;
        std::vector<QAction*> requiresNonNull;

    public:
        /**
         * Constructor and destructor.
         */
        SnapPeaShapesUI(regina::PacketOf<regina::SnapPeaTriangulation>* packet,
            PacketTabbedUI* useParentUI);
        ~SnapPeaShapesUI() override;

        /**
         * Fill the given toolbar with actions.
         *
         * This is necessary since the toolbar will not be a part of
         * this page, but this page (as the editor) keeps track of the
         * available actions.
         */
        void fillToolBar(QToolBar* bar);

        /**
         * PacketEditorTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        const std::vector<QAction*>& getPacketTypeActions() override;
        void refresh() override;
        void endEdit() override;

    public slots:
        /**
         * Actions.
         */
        void randomise();
        void vertexLinks();
        void canonise();
        void toRegina();
        void fill();

        /**
         * Update the states of internal components.
         */
        void updateNonNullActions();
};

inline CuspModel::CuspModel(regina::SnapPeaTriangulation* tri) : tri_(tri) {
}

inline QModelIndex CuspModel::parent(const QModelIndex&) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
