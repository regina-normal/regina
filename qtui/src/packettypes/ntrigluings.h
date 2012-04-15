
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

/*! \file ntrigluings.h
 *  \brief Provides a face gluing editor for 3-manifold triangulations.
 */

#ifndef __NTRIGLUINGS_H
#define __NTRIGLUINGS_H

#include "../packettabui.h"
#include "reginaprefset.h"
#include "maths/nperm4.h"

#include <QAbstractItemModel>

class QTableView;
class QToolBar;

namespace regina {
    class NPacket;
    class NTriangulation;
};

class GluingsModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the working (non-committed) triangulation
         */
        int nTet;
        QString* name;
        int* adjTet;
        regina::NPerm4* adjPerm;

        /**
         * Internal status
         */
        bool isReadWrite_;

    public:
        /**
         * Constructor and destructor.
         */
        GluingsModel(bool readWrite);
        ~GluingsModel();

        /**
         * Read-write state.
         */
        bool isReadWrite() const;
        void setReadWrite(bool readWrite);

        /**
         * Loading and saving local data from/to the packet.
         */
        void refreshData(regina::NTriangulation* tri);
        void commitData(regina::NTriangulation* tri);

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
         * Gluing edit actions.
         */
        void addTet();
        void removeTet(int first, int last);

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
            regina::NPerm4* gluing);

        /**
         * Return a short string describing the destination of a
         * face gluing.  This routine handles both boundary and
         * non-boundary faces.
         */
        static QString destString(int srcFace, int destTet,
            const regina::NPerm4& gluing);

        /**
         * Convert a face string (e.g., "130") to a face permutation.
         *
         * The given face string must be valid; otherwise the results
         * could be unpredictable (and indeed a crash could result).
         */
        static regina::NPerm4 faceStringToPerm(int srcFace, const QString& str);

        /**
         * Display the given error to the user.
         */
        void showError(const QString& message);
};

/**
 * A 3-manifold triangulation page for editing face gluings.
 */
class NTriGluingsUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NTriangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QTableView* faceTable;
        GluingsModel* model;

        /**
         * Gluing actions
         */
        QAction* actAddTet;
        QAction* actRemoveTet;
        QAction* actSimplify;
        QAction* actOrient;
        QLinkedList<QAction*> triActionList;
        QLinkedList<QAction*> enableWhenWritable;

    public:
        /**
         * Constructor and destructor.
         */
        NTriGluingsUI(regina::NTriangulation* packet,
                PacketTabbedUI* useParentUI, bool readWrite);
        ~NTriGluingsUI();

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
        void commit();
        void refresh();
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
        void orient();
        void barycentricSubdivide();
        void idealToFinite();
        void finiteToIdeal();
        void elementaryMove();
        void doubleCover();
        void splitIntoComponents();
        void connectedSumDecomposition();
        void makeZeroEfficient();
        void censusLookup();

        /**
         * Update the states of internal components.
         */
        void updateRemoveState();
        void updateOrientState();

        /**
         * Notify us of the fact that an edit has been made.
         */
        void notifyDataChanged();
};

inline GluingsModel::~GluingsModel() {
    delete[] name;
    delete[] adjTet;
    delete[] adjPerm;
}

inline bool GluingsModel::isReadWrite() const {
    return isReadWrite_;
}

inline void GluingsModel::setReadWrite(bool readWrite) {
    if (isReadWrite_ != readWrite) {
        // Edit flags will all change.
        // A full model reset is probably too severe, but.. *shrug*
        beginResetModel();
        isReadWrite_ = readWrite;
        endResetModel();
    }
}

inline QModelIndex GluingsModel::parent(const QModelIndex&) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
