
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

/*! \file nfacegluingitems.h
 *  \brief Provides table items for editing triangulation face gluings.
 */

#ifndef __NFACEGLUINGITEMS_H
#define __NFACEGLUINGITEMS_H

#include "triangulation/nperm.h"

#include "reginaprefset.h"

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
class FaceGluingItem : public QObject, public QTableItem {
    Q_OBJECT

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
         * Change the destination for this face gluing.  Related face
         * gluings will also be updated if necessary.
         *
         * It is assumed that the given destination is valid.
         *
         * Related table cells will always be repainted.  This table
         * cell will be repainted by default, but this can be suppressed
         * by passing \c false as the final boolean parameter.
         *
         * This routine can handle both boundary faces and real face
         * gluings.
         */
        void setDestination(long newDestTet,
            const regina::NPerm& newGluingPerm,
            bool shouldRepaintThisTableCell = true);

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

        /**
         * Determine whether the given destination tetrahedron and face
         * string are valid.  If so, a null string is returned; if not,
         * an appropriate error message is returned.
         *
         * If the given permutation pointer is not null, the resulting
         * gluing permutation will be returned in this variable.
         */
        static QString isFaceStringValid(unsigned long nTets,
            unsigned long srcTet, int srcFace,
            unsigned long destTet, const QString& destFace,
            regina::NPerm* gluing);

    signals:
        /**
         * Emitted when a table item is explicitly changed by the user
         * through either a call to setDestination() or through direct
         * interaction.
         *
         * Note that generally one change will imply others (e.g., if
         * face A is glued to face B then face B will as a result be glued
         * to face A).  In such cases, this signal will be emitted only
         * for the face that was explicitly changed by the user.
         */
        void destinationChanged();

    private:
        /**
         * Return a short string describing the destination of a
         * face gluing.  This routine handles both boundary and
         * non-boundary faces.
         */
        static QString destString(int srcFace, int destTet,
            const regina::NPerm& gluing);

        /**
         * Convert a face string (e.g., "130") to a face permutation.
         *
         * The given face string must be valid; otherwise the results
         * could be unpredictable (and indeed a crash could result).
         */
        static regina::NPerm faceStringToPerm(int srcFace, const QString& str);

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
