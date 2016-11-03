
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
         * Return a short string describing the destination of a
         * face gluing.  This routine handles both boundary and
         * non-boundary faces.
         */
        static QString destString(int srcFace, regina::Simplex<3>* destTet,
            const regina::Perm<4>& gluing);

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
        regina::Triangulation<3>* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        EditTableView* faceTable;
        GluingsModel3* model;

        /**
         * Gluing actions
         */
        QAction* actAddTet;
        QAction* actRemoveTet;
        QAction* actSimplify;
        QAction* actOrient;
        QAction* actBoundaryComponents;
        QLinkedList<QAction*> triActionList;
        QLinkedList<QAction*> enableWhenWritable;

    public:
        /**
         * Constructor and destructor.
         */
        Tri3GluingsUI(regina::Triangulation<3>* packet,
                PacketTabbedUI* useParentUI, bool readWrite);
        ~Tri3GluingsUI();

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
        void addTet();
        void removeSelectedTets();

        /**
         * Triangulation actions.
         */
        void simplify();
        void simplifyExhaustive(int height);
        void orient();
        void barycentricSubdivide();
        void idealToFinite();
        void finiteToIdeal();
        void elementaryMove();
        void doubleCover();
        void puncture();
        void drillEdge();
        void connectedSumWith();
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

inline bool GluingsModel3::isReadWrite() const {
    return isReadWrite_;
}

inline void GluingsModel3::setReadWrite(bool readWrite) {
    if (isReadWrite_ != readWrite) {
        // Edit flags will all change.
        // A full model reset is probably too severe, but.. *shrug*
        beginResetModel();
        isReadWrite_ = readWrite;
        endResetModel();
    }
}

inline QModelIndex GluingsModel3::parent(const QModelIndex&) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
