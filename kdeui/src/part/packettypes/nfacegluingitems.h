
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nfacegluingitems.h
 *  \brief Provides table items for editing triangulation face gluings.
 */

#ifndef __NFACEGLUINGITEMS_H
#define __NFACEGLUINGITEMS_H

#include "triangulation/nperm.h"

#include "../../reginaprefset.h"

#include <qtable.h>

/**
 * A table item for tetrahedron names.
 */
class TetNameItem : public QTableItem {
    private:
        QString name;
            /**< The current tetrahedron name. */

    public:
        /**
         * Constructor.
         */
        TetNameItem(QTable* table, unsigned long tetNum,
                const QString& tetName);

        /**
         * Query properties.
         */
        const QString& getName() const;

        /**
         * Notify the table that this tetrahedron's number is about to
         * change.  The table cell will be updated accordingly.
         */
        void tetNumToChange(long newTetNum);

        /**
         * QTableItem overrides.
         */
        int alignment() const;
        virtual QWidget* createEditor() const;
        virtual void setContentFromEditor(QWidget* editor);
};

/**
 * A table item for an individual face gluing.
 */
class FaceGluingItem : public QTableItem {
    private:
        long adjTet;
            /**< The adjacent tetrahedron, or -1 if this is a boundary face. */
        regina::NPerm adjPerm;
            /**< The adjacent tetrahedron gluing. */

        const ReginaPrefSet::TriEditMode& editMode;
            /**< Determines the style of cell editor that is created. */

        bool error;
            /**< Are we currently displaying an error message? */
    public:
        /**
         * Constructors.  The first constructor is for a boundary face,
         * the second for a face that is glued elsewhere.
         */
        FaceGluingItem(QTable* table,
            const ReginaPrefSet::TriEditMode& useEditMode);
        FaceGluingItem(QTable* table,
            const ReginaPrefSet::TriEditMode& useEditMode, int myFace,
            unsigned long destTet, const regina::NPerm& gluingPerm);

        /**
         * Query properties.
         */
        bool isBoundary() const;
        long getAdjacentTetrahedron() const;
        int getMyFace() const;
        int getAdjacentFace() const;
        const regina::NPerm& getAdjacentTetrahedronGluing() const;

        /**
         * Find the table entry corresponding to the partner of this
         * face, if any.
         */
        FaceGluingItem* getPartner();

        /**
         * Break any existing face pairing involving this tetrahedron
         * face.
         *
         * Note that the table cell for the partner will repainted, but
         * this table cell will not (under the assumption that this
         * table cell is still being worked upon).
         */
        void unjoin();

        /**
         * Called when one or more tetrahedron numbers are about to
         * change.  This can happen for instance when tetrahedra are
         * removed from the table.
         *
         * A map for converting old tetrahedron numbers to new
         * is passed.  The adjacent tetrahedron number registered for
         * this face will be modified if necessary and the table cell
         * updated accordingly.
         */
        void tetNumsToChange(const long newTetNums[]);

        /**
         * QTableItem overrides.
         */
        virtual QWidget* createEditor() const;
        virtual void setContentFromEditor(QWidget* editor);

    private:
        /**
         * Return a short string describing the destination of a
         * (non-boundary) face gluing.
         */
        static QString destString(int srcFace, int destTet,
                const regina::NPerm& gluing);

        /**
         * Display the given error to the user if no error is already
         * being displayed.
         */
        void showError(const QString& message);
};

inline const QString& TetNameItem::getName() const {
    return name;
}

inline bool FaceGluingItem::isBoundary() const {
    return (adjTet < 0);
}

inline long FaceGluingItem::getAdjacentTetrahedron() const {
    return adjTet;
}

inline int FaceGluingItem::getMyFace() const {
    return 4 - col();
}

inline int FaceGluingItem::getAdjacentFace() const {
    return (adjTet < 0 ? -1 : adjPerm[getMyFace()]);
}

inline const regina::NPerm& FaceGluingItem::getAdjacentTetrahedronGluing()
        const {
    return adjPerm;
}

#endif
