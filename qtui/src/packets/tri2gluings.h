
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

/*! \file tri2gluings.h
 *  \brief Provides an edge gluing editor for 2-manifold triangulations.
 */

#ifndef __TRI2GLUINGS_H
#define __TRI2GLUINGS_H

#include "triangulation/forward.h"
#include "../packettabui.h"
#include "reginaprefset.h"

#include <QAbstractItemModel>

class EditTableView;
class QToolBar;

namespace regina {
    class Packet;
    template <int> class Perm;
};

class GluingsModel2 : public QAbstractItemModel {
    protected:
        /**
         * Details of the triangulation
         */
        regina::Triangulation<2>* tri_;

        /**
         * Internal status
         */
        bool isReadWrite_;

    public:
        /**
         * Constructor and destructor.
         */
        GluingsModel2(regina::Triangulation<2>* tri, bool readWrite);

        /**
         * Read-write state.
         */
        bool isReadWrite() const;
        void setReadWrite(bool readWrite);

        /**
         * Force a complete refresh.
         */
        void rebuild();

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

    private:
        /**
         * Determine whether the given destination triangle and edge
         * string are valid.  If so, a null string is returned; if not,
         * an appropriate error message is returned.
         *
         * If the given permutation pointer is not null, the resulting
         * gluing permutation will be returned in this variable.
         */
        QString isEdgeStringValid(unsigned long srcTri, int srcEdge,
            unsigned long destTri, const QString& destEdge,
            regina::Perm<3>* gluing);

        /**
         * Return a short string describing the destination of an
         * edge gluing.  This routine handles both boundary and
         * non-boundary edges.
         */
        static QString destString(int srcEdge, regina::Simplex<2>* destTri,
            const regina::Perm<3>& gluing);

        /**
         * Convert an edge string (e.g., "20") to an edge permutation.
         *
         * The given edge string must be valid; otherwise the results
         * could be unpredictable (and indeed a crash could result).
         */
        static regina::Perm<3> edgeStringToPerm(int srcEdge, const QString& str);

        /**
         * Display the given error to the user.
         */
        void showError(const QString& message);
};

/**
 * A 2-manifold triangulation page for editing edge gluings.
 */
class Tri2GluingsUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Triangulation<2>* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        EditTableView* edgeTable;
        GluingsModel2* model;

        /**
         * Gluing actions
         */
        QAction* actAddTri;
        QAction* actRemoveTri;
        QAction* actOrient;
        QLinkedList<QAction*> triActionList;
        QLinkedList<QAction*> enableWhenWritable;

    public:
        /**
         * Constructor and destructor.
         */
        Tri2GluingsUI(regina::Triangulation<2>* packet,
                PacketTabbedUI* useParentUI, bool readWrite);
        ~Tri2GluingsUI();

        /**
         * Fill the given toolbar with triangulation actions.
         *
         * This is necessary since the toolbar will not be a part of
         * this page, but this page (as the editor) keeps track of the
         * available actions.
         */
        void fillToolBar(QToolBar* bar);

        /**
         * PacketEditorTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        void refresh();
        void endEdit();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Gluing edit actions.
         */
        void addTri();
        void removeSelectedTris();

        /**
         * Triangulation actions.
         */
        void orient();
        void barycentricSubdivide();
        void doubleCover();
        void splitIntoComponents();

        /**
         * Update the states of internal components.
         */
        void updateRemoveState();
        void updateActionStates();
};

inline bool GluingsModel2::isReadWrite() const {
    return isReadWrite_;
}

inline void GluingsModel2::setReadWrite(bool readWrite) {
    if (isReadWrite_ != readWrite) {
        // Edit flags will all change.
        // A full model reset is probably too severe, but.. *shrug*
        beginResetModel();
        isReadWrite_ = readWrite;
        endResetModel();
    }
}

inline QModelIndex GluingsModel2::parent(const QModelIndex&) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
