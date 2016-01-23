
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file dim4trigluings.h
 *  \brief Provides a facet gluing editor for 4-manifold triangulations.
 */

#ifndef __DIM4TRIGLUINGS_H
#define __DIM4TRIGLUINGS_H

#include "../packettabui.h"
#include "reginaprefset.h"
#include "maths/nperm5.h"

#include <QAbstractItemModel>

class EditTableView;
class QToolBar;

namespace regina {
    class NPacket;

    template <int> class Simplex;
    template <int> class Triangulation;
    typedef Simplex<4> Dim4Pentachoron;
    typedef Triangulation<4> Dim4Triangulation;
};

class Dim4GluingsModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the triangulation
         */
        regina::Dim4Triangulation* tri_;

        /**
         * Internal status
         */
        bool isReadWrite_;

    public:
        /**
         * Constructor.
         */
        Dim4GluingsModel(regina::Dim4Triangulation* tri, bool readWrite);

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
         * Determine whether the given destination pentachoron and facet
         * string are valid.  If so, a null string is returned; if not,
         * an appropriate error message is returned.
         *
         * If the given permutation pointer is not null, the resulting
         * gluing permutation will be returned in this variable.
         */
        QString isFacetStringValid(unsigned long srcPent, int srcFacet,
            unsigned long destPent, const QString& destFacet,
            regina::NPerm5* gluing);

        /**
         * Return a short string describing the destination of a
         * facet gluing.  This routine handles both boundary and
         * non-boundary facets.
         */
        static QString destString(int srcFacet,
            regina::Dim4Pentachoron* destPent,
            const regina::NPerm5& gluing);

        /**
         * Convert a facet string (e.g., "1304") to a facet permutation.
         *
         * The given facet string must be valid; otherwise the results
         * could be unpredictable (and indeed a crash could result).
         */
        static regina::NPerm5 facetStringToPerm(int srcFacet,
            const QString& str);

        /**
         * Display the given error to the user.
         */
        void showError(const QString& message);
};

/**
 * A 4-manifold triangulation page for editing facet gluings.
 */
class Dim4TriGluingsUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Dim4Triangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        EditTableView* facetTable;
        Dim4GluingsModel* model;

        /**
         * Gluing actions
         */
        QAction* actAddPent;
        QAction* actRemovePent;
        QAction* actSimplify;
        QAction* actOrient;
        QAction* actBoundaryComponents;
        QLinkedList<QAction*> triActionList;
        QLinkedList<QAction*> enableWhenWritable;

    public:
        /**
         * Constructor and destructor.
         */
        Dim4TriGluingsUI(regina::Dim4Triangulation* packet,
                PacketTabbedUI* useParentUI, bool readWrite);
        ~Dim4TriGluingsUI();

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
        regina::NPacket* getPacket();
        QWidget* getInterface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        void refresh();
        void endEdit();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Gluing edit actions.
         */
        void addPent();
        void removeSelectedPents();

        /**
         * Triangulation actions.
         */
        void simplify();
        void orient();
        void barycentricSubdivide();
        void idealToFinite();
        void finiteToIdeal();
        void elementaryMove();
        void doubleCover();
        void boundaryComponents();
        void vertexLinks();
        void splitIntoComponents();

        /**
         * Update the states of internal components.
         */
        void updateRemoveState();
        void updateActionStates();
};

inline bool Dim4GluingsModel::isReadWrite() const {
    return isReadWrite_;
}

inline void Dim4GluingsModel::setReadWrite(bool readWrite) {
    if (isReadWrite_ != readWrite) {
        // Edit flags will all change.
        // A full model reset is probably too severe, but.. *shrug*
        beginResetModel();
        isReadWrite_ = readWrite;
        endResetModel();
    }
}

inline QModelIndex Dim4GluingsModel::parent(const QModelIndex&) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
