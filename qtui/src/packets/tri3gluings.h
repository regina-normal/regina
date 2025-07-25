
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file tri3gluings.h
 *  \brief Provides a face gluing editor for 3-manifold triangulations.
 */

#ifndef __TRI3GLUINGS_H
#define __TRI3GLUINGS_H

#include "triangulation/forward.h"
#include "../packettabui.h"
#include "reginaprefset.h"
#include "maths/perm.h"

#include <QAbstractItemModel>

class EditTableView;
class QToolBar;

namespace regina {
    class Packet;
};

class GluingsModel3 : public QAbstractItemModel {
    protected:
        /**
         * Details of the triangulation
         */
        regina::Triangulation<3>* tri_;

        /**
         * Internal status
         */
        bool isReadWrite_;

    public:
        /**
         * Constructor.
         */
        GluingsModel3(regina::Triangulation<3>* tri, bool readWrite);

        /**
         * Force a complete refresh.
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

        /**
         * Return a short string describing the destination of a
         * face gluing.  This routine handles both boundary and
         * non-boundary faces.  The destination is deduced by looking at
         * whatever the given source face is glued to.
         */
        static QString destString(regina::Simplex<3>* srcTet, int srcFace);

    private:
        /**
         * Determine whether the given destination tetrahedron and face
         * string are valid.  If so, a null string is returned; if not,
         * an appropriate error message is returned.
         *
         * If the given permutation pointer is not null, the resulting
         * gluing permutation will be returned in this variable.
         */
        QString isFaceStringValid(unsigned long srcTet, int srcFace,
            unsigned long destTet, const QString& destFace,
            regina::Perm<4>* gluing);

        /**
         * Convert a face string (e.g., "130") to a face permutation.
         *
         * The given face string must be valid; otherwise the results
         * could be unpredictable (and indeed a crash could result).
         */
        static regina::Perm<4> faceStringToPerm(int srcFace, const QString& str);

        /**
         * Display the given error to the user.
         */
        void showError(const QString& message);
};

/**
 * A 3-manifold triangulation page for editing face gluings.
 */
class Tri3GluingsUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::Triangulation<3>>* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        EditTableView* faceTable;
        GluingsModel3* model;

        /**
         * Pop-up menus
         */
        ssize_t lockSimplex { -1 };
        int lockFacet { -1 }; // -1 for simplex, 0..3 for facet
        bool lockAdd { false }; // true to lock, false to unlock

        /**
         * Gluing actions
         */
        QAction* actAddTet;
        QAction* actRemoveTet;
        QAction* actSimplify;
        QAction* actOrient;
        QAction* actBoundaryComponents;
        std::vector<QAction*> triActionList;

    public:
        /**
         * Constructor and destructor.
         */
        Tri3GluingsUI(regina::PacketOf<regina::Triangulation<3>>* packet,
                PacketTabbedUI* useParentUI);
        ~Tri3GluingsUI() override;

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
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        const std::vector<QAction*>& getPacketTypeActions() override;
        void refresh() override;
        void endEdit() override;

    public slots:
        /**
         * Gluing edit actions.
         */
        void addTet();
        void removeSelectedTets();

        /**
         * Lock menu actions.
         */
        void lockMenu(const QPoint&);
        void changeLock();

        /**
         * Triangulation actions.
         */
        void simplify();
        void simplifyExhaustive(int height);
        void orient();
        void reflect();
        void barycentricSubdivide();
        void idealToFinite();
        void truncateVertex();
        void finiteToIdeal();
        void elementaryMove();
        void doubleCover();
        void puncture();
        void drillEdge();
        void connectedSumWith();
        void insertTriangulation();
        void boundaryComponents();
        void vertexLinks();
        void splitIntoComponents();
        void connectedSumDecomposition();
        void makeZeroEfficient();
        void toSnapPea();

        /**
         * Update the states of internal components.
         */
        void updateRemoveState();
        void updateActionStates();
};

inline QModelIndex GluingsModel3::parent(const QModelIndex&) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
