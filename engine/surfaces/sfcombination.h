
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file sfcombination.h
 *  \brief Contains a normal surface filter that simply combines other
 *  filters.
 */

#ifndef __SFCOMBINATION_H
#ifndef __DOXYGEN
#define __SFCOMBINATION_H
#endif

#include "surfaces/nsurfacefilter.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * A normal surface filter that simply combines other filters.
 * This filter will combine, using boolean \a and or \a or, all of the
 * filters that are immediate children of this packet.  This packet may
 * have children that are not normal surface filters; such children will
 * simply be ignored.
 *
 * If there are no immediate child filters, a normal surface will be
 * accepted if this is an \a and filter and rejected if this is an \a or
 * filter.
 */
class NSurfaceFilterCombination : public NSurfaceFilter {
    public:
        static const int filterID;

    private:
        bool usesAnd;
            /**< \c true if children are combined using boolean \a and, or
                 \c false if children are combined using boolean \a or. */

    public:
        /**
         * Creates a new surface filter that accepts all normal surfaces.
         * This will be an \a and filter.
         */
        NSurfaceFilterCombination();
        /**
         * Creates a new surface filter that is a clone of the given
         * surface filter.
         *
         * @param cloneMe the surface filter to clone.
         */
        NSurfaceFilterCombination(const NSurfaceFilterCombination& cloneMe);

        /**
         * Determines whether this is an \a and or an \a or combination.
         *
         * @return \c true if this is an \a and combination, or \c false
         * if this is an \a or combination.
         */
        bool getUsesAnd() const;
        /**
         * Sets whether this is an \a and or an \a or combination.
         *
         * @param value \c true if this is to be an \a and combination,
         * or \c false if this is to be an \a or combination.
         */
        void setUsesAnd(bool value);

        virtual bool accept(const NNormalSurface& surface) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLFilterReader* getXMLFilterReader(NPacket* parent);
        static NSurfaceFilter* readFilter(NFile& in, NPacket* parent);

        virtual int getFilterID() const;
        virtual std::string getFilterName() const;

    protected:
        virtual void writeXMLFilterData(std::ostream& out) const;
        virtual void writeFilter(NFile& out) const;
};

/*@}*/

// Inline functions for NSurfaceFilterCombination

inline NSurfaceFilterCombination::NSurfaceFilterCombination() : usesAnd(true) {
}
inline NSurfaceFilterCombination::NSurfaceFilterCombination(
        const NSurfaceFilterCombination& cloneMe) : NSurfaceFilter(),
        usesAnd(cloneMe.usesAnd) {
}

inline bool NSurfaceFilterCombination::getUsesAnd() const {
    return usesAnd;
}
inline void NSurfaceFilterCombination::setUsesAnd(bool value) {
    usesAnd = value;
    fireChangedEvent();
}

inline void NSurfaceFilterCombination::writeTextLong(std::ostream& o) const {
    o << (usesAnd ? "AND" : "OR") << " combination normal surface filter\n";
}

} // namespace regina

#endif

