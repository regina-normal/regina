
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

/*! \file nsurfacesubset.h
 *  \brief Provides subsets of normal surface sets.
 */

#ifndef __NSURFACESUBSET_H
#ifndef __DOXYGEN
#define __NSURFACESUBSET_H
#endif

#include <vector>
#include "shareableobject.h"
#include "surfaces/nsurfaceset.h"

namespace regina {

class NSurfaceFilter;

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * Represents a subset of a normal surface set.
 * This subset merely contains pointers to some of the normal surfaces
 * stored in the original set.
 *
 * If the surfaces in the original set
 * should change, the surfaces in this subset will thus change also.  If
 * the original set is deleted, this subset will become invalid.
 *
 * \pre As long as this subset is in use, the surface set upon which this
 * subset is based will never be deleted.
 */
class NSurfaceSubset : public ShareableObject, public NSurfaceSet {
    private:
        std::vector<NNormalSurface*> surfaces;
            /**< Contains the surfaces contained in this subset.
                 These will all be pointers to surfaces stored in
                 \a source. */
        const NSurfaceSet& source;
            /**< The surface set for which this is a subset. */

    public:
        /**
         * Creates a new normal surface subset.
         * The surfaces included in the subset will be those from the
         * given set that are accepted by the given filter.
         *
         * @param set the surface set for which this will be a subset.
         * @param filter the filter that defines which surfaces in \a set
         * will be included in this subset.
         */
        NSurfaceSubset(const NSurfaceSet& set, const NSurfaceFilter& filter);
        /**
         * Destroys this normal surface subset.
         */
        virtual ~NSurfaceSubset();

        virtual int getFlavour() const;
        virtual bool allowsAlmostNormal() const;
        virtual bool isEmbeddedOnly() const;
        virtual NTriangulation* getTriangulation() const;
        virtual unsigned long getNumberOfSurfaces() const;
        virtual const NNormalSurface* getSurface(unsigned long index) const;
        virtual ShareableObject* getShareableObject();

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
};

/*@}*/

// Inline functions for NSurfaceSubset

inline NSurfaceSubset::~NSurfaceSubset() {
}

inline int NSurfaceSubset::getFlavour() const {
    return source.getFlavour();
}
inline bool NSurfaceSubset::allowsAlmostNormal() const {
    return source.allowsAlmostNormal();
}
inline bool NSurfaceSubset::isEmbeddedOnly() const {
    return source.isEmbeddedOnly();
}
inline NTriangulation* NSurfaceSubset::getTriangulation() const {
    return source.getTriangulation();
}
inline unsigned long NSurfaceSubset::getNumberOfSurfaces() const {
    return surfaces.size();
}
inline const NNormalSurface* NSurfaceSubset::getSurface(unsigned long index)
        const {
    return surfaces[index];
}
inline ShareableObject* NSurfaceSubset::getShareableObject() {
    return this;
}

inline void NSurfaceSubset::writeTextShort(std::ostream& out) const {
    out << "Subset containing " << surfaces.size() << " normal surface";
    if (surfaces.size() != 1)
        out << 's';
}
inline void NSurfaceSubset::writeTextLong(std::ostream& out) const {
    writeAllSurfaces(out);
}

} // namespace regina

#endif

