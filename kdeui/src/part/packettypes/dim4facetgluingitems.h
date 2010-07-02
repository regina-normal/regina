
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file dim4facetgluingitems.h
 *  \brief Provides table items for editing 4-manifold triangulation
 *  facet gluings.
 */

#ifndef __DIM4FACETGLUINGITEMS_H
#define __DIM4FACETGLUINGITEMS_H

#include "maths/nperm5.h"

#include "reginaprefset.h"

#include <qtable.h>

/**
 * A table item for pentachoron names.
 */
class PentNameItem : public QTableItem {
    private:
        QString name;
            /**< The current pentachoron name. */

    public:
        /**
         * Constructor.
         */
        PentNameItem(QTable* table, unsigned long pentNum,
                const QString& pentName);

        /**
         * Query properties.
         */
        const QString& getName() const;

        /**
         * Notify the table that this pentachoron's number is about to
         * change.  The table cell will be updated accordingly.
         */
        void pentNumToChange(long newPentNum);

        /**
         * QTableItem overrides.
         */
        int alignment() const;
        virtual QWidget* createEditor() const;
        virtual void setContentFromEditor(QWidget* editor);
};

/**
 * A table item for an individual facet gluing.
 */
class FacetGluingItem : public QObject, public QTableItem {
    Q_OBJECT

    private:
        long adjPent;
            /**< The adjacent pentachoron, or -1 if this is a boundary facet. */
        regina::NPerm5 adjPerm;
            /**< The adjacent pentachoron gluing. */

        const ReginaPrefSet::TriEditMode& editMode;
            /**< Determines the style of cell editor that is created. */

        bool error;
            /**< Are we currently displaying an error message? */
    public:
        /**
         * Constructors.  The first constructor is for a boundary facet,
         * the second for a facet that is glued elsewhere.
         */
        FacetGluingItem(QTable* table,
            const ReginaPrefSet::TriEditMode& useEditMode);
        FacetGluingItem(QTable* table,
            const ReginaPrefSet::TriEditMode& useEditMode, int myFacet,
            unsigned long destPent, const regina::NPerm5& gluingPerm);

        /**
         * Query properties.
         */
        bool isBoundary() const;
        long adjacentPentachoron() const;
        int myFacet() const;
        int adjacentFacet() const;
        const regina::NPerm5& adjacentGluing() const;

        /**
         * Change the destination for this facet gluing.  Related facet
         * gluings will also be updated if necessary.
         *
         * It is assumed that the given destination is valid.
         *
         * Related table cells will always be repainted.  This table
         * cell will be repainted by default, but this can be suppressed
         * by passing \c false as the final boolean parameter.
         *
         * This routine can handle both boundary facets and real facet
         * gluings.
         */
        void setDestination(long newDestPent,
            const regina::NPerm5& newGluingPerm,
            bool shouldRepaintThisTableCell = true);

        /**
         * Find the table entry corresponding to the partner of this
         * facet, if any.
         */
        FacetGluingItem* getPartner();

        /**
         * Break any existing facet pairing involving this pentachoron
         * facet.
         *
         * Note that the table cell for the partner will repainted, but
         * this table cell will not (under the assumption that this
         * table cell is still being worked upon).
         */
        void unjoin();

        /**
         * Called when one or more pentachoron numbers are about to
         * change.  This can happen for instance when pentachora are
         * removed from the table.
         *
         * A map for converting old pentachoron numbers to new
         * is passed.  The adjacent pentachoron number registered for
         * this facet will be modified if necessary and the table cell
         * updated accordingly.
         */
        void pentNumsToChange(const long newPentNums[]);

        /**
         * QTableItem overrides.
         */
        virtual QWidget* createEditor() const;
        virtual void setContentFromEditor(QWidget* editor);

        /**
         * Determine whether the given destination pentachoron and facet
         * string are valid.  If so, a null string is returned; if not,
         * an appropriate error message is returned.
         *
         * If the given permutation pointer is not null, the resulting
         * gluing permutation will be returned in this variable.
         */
        static QString isFacetStringValid(unsigned long nPents,
            unsigned long srcPent, int srcFacet,
            unsigned long destPent, const QString& destFacet,
            regina::NPerm5* gluing);

    signals:
        /**
         * Emitted when a table item is explicitly changed by the user
         * through either a call to setDestination() or through direct
         * interaction.
         *
         * Note that generally one change will imply others (e.g., if
         * facet A is glued to facet B then facet B will as a result be glued
         * to facet A).  In such cases, this signal will be emitted only
         * for the facet that was explicitly changed by the user.
         */
        void destinationChanged();

    private:
        /**
         * Return a short string describing the destination of a
         * facet gluing.  This routine handles both boundary and
         * non-boundary facets.
         */
        static QString destString(int srcFacet, int destPent,
            const regina::NPerm5& gluing);

        /**
         * Convert a facet string (e.g., "1420") to a facet permutation.
         *
         * The given facet string must be valid; otherwise the results
         * could be unpredictable (and indeed a crash could result).
         */
        static regina::NPerm5 facetStringToPerm(int srcFacet,
            const QString& str);

        /**
         * Display the given error to the user if no error is already
         * being displayed.
         */
        void showError(const QString& message);
};

inline const QString& PentNameItem::getName() const {
    return name;
}

inline bool FacetGluingItem::isBoundary() const {
    return (adjPent < 0);
}

inline long FacetGluingItem::adjacentPentachoron() const {
    return adjPent;
}

inline int FacetGluingItem::myFacet() const {
    return 5 - col();
}

inline int FacetGluingItem::adjacentFacet() const {
    return (adjPent < 0 ? -1 : adjPerm[myFacet()]);
}

inline const regina::NPerm5& FacetGluingItem::adjacentGluing()
        const {
    return adjPerm;
}

#endif
