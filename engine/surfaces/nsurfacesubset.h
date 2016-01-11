
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/*! \file surfaces/nsurfacesubset.h
 *  \brief Provides subsets of normal surface sets.
 */

#ifndef __NSURFACESUBSET_H
#ifndef __DOXYGEN
#define __NSURFACESUBSET_H
#endif

#include <vector>
#include "regina-core.h"
#include "output.h"
#include "surfaces/nnormalsurfacelist.h"
#include <boost/noncopyable.hpp>

namespace regina {

class NSurfaceFilter;

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * Represents a subset of a normal surface list.
 * This subset merely contains pointers to some of the normal surfaces
 * stored in the original list.
 *
 * If the surfaces in the original list
 * should change, the surfaces in this subset will thus change also.  If
 * the original list is deleted, this subset will become invalid.
 *
 * \pre As long as this subset is in use, the surface list upon which this
 * subset is based must never be deleted.
 */
class REGINA_API NSurfaceSubset :
        public Output<NSurfaceSubset>,
        public boost::noncopyable {
    private:
        std::vector<NNormalSurface*> surfaces;
            /**< Contains the surfaces contained in this subset.
                 These will all be pointers to surfaces stored in
                 \a source. */
        const NNormalSurfaceList& source;
            /**< The surface list for which this is a subset. */

    public:
        /**
         * Creates a new normal surface subset.
         * The surfaces included in the subset will be those from the
         * given set that are accepted by the given filter.
         *
         * @param list the surface list for which this will be a subset.
         * @param filter the filter that defines which surfaces in \a list
         * will be included in this subset.
         */
        NSurfaceSubset(const NNormalSurfaceList& list,
            const NSurfaceFilter& filter);
        /**
         * Returns the coordinate system being used by the
         * surfaces stored in this set.
         *
         * \deprecated Users should switch to the identical routine
         * coords() instead.
         *
         * @return the coordinate system used.
         */
        NormalCoords getFlavour() const;
        /**
         * Returns the coordinate system being used by the
         * surfaces stored in this set.
         *
         * @return the coordinate system used.
         */
        NormalCoords coords() const;
        /**
         * Determines if the coordinate system being used
         * allows for almost normal surfaces, that is, allows for
         * octagonal discs.
         *
         * @return \c true if and only if almost normal surfaces are
         * allowed.
         */
        bool allowsAlmostNormal() const;
        /**
         * Determines if the coordinate system being used
         * allows for spun normal surfaces.
         *
         * @return \c true if and only if spun normal surface are
         * supported.
         */
        bool allowsSpun() const;
        /**
         * Determines if the coordinate system being used
         * allows for transversely oriented normal surfaces.
         *
         * @return \c true if and only if transverse orientations are
         * supported.
         */
        bool allowsOriented() const;
        /**
         * Returns whether this set is known to contain only embedded normal
         * surfaces.
         *
         * If it is possible that there are non-embedded surfaces in this
         * set but it is not known whether any are actually present or
         * not, this routine should return \c false.
         *
         * @return \c true if it is known that only embedded normal surfaces
         * exist in this list, or \c false if immersed and/or singular normal
         * surfaces might be present.
         */
        bool isEmbeddedOnly() const;
        /**
         * Returns the triangulation in which these normal surfaces live.
         * 
         * @return the triangulation in which these surfaces live.
         */
        NTriangulation* triangulation() const;
        /**
         * Deprecated routine that returns the triangulation in which these
         * normal surfaces live.
         *
         * \deprecated This routine has been renamed to triangulation().
         * See the triangulation() documentation for further details.
         */
        NTriangulation* getTriangulation() const;

        /**
         * Returns the number of surfaces stored in this list.
         *
         * @return the number of surfaces.
         */
        size_t size() const;
        /**
         * Deprecated routine to return the number of surfaces
         * stored in this list.
         *
         * \deprecated Please use the identical routine size() instead.
         *
         * @return the number of surfaces.
         */
        size_t getNumberOfSurfaces() const;
        /**
         * Returns the surface at the requested index in this set.
         *
         * @param index the index of the requested surface in this set;
         * this must be between 0 and size()-1 inclusive.
         *
         * @return the normal surface at the requested index in this set.
         */
        const NNormalSurface* getSurface(size_t index) const;

        /**
         * Writes the number of surfaces in this set followed by the
         * details of each surface to the given output stream.  Output
         * will be over many lines.
         *
         * \ifacespython Parameter \a out is not present and is assumed
         * to be standard output.
         *
         * @param out the output stream to which to write.
         */
        void writeAllSurfaces(std::ostream& out) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;
};

/*@}*/

// Inline functions for NSurfaceSubset

inline NormalCoords NSurfaceSubset::getFlavour() const {
    return source.coords();
}
inline NormalCoords NSurfaceSubset::coords() const {
    return source.coords();
}
inline bool NSurfaceSubset::allowsAlmostNormal() const {
    return source.allowsAlmostNormal();
}
inline bool NSurfaceSubset::allowsSpun() const {
    return source.allowsSpun();
}
inline bool NSurfaceSubset::allowsOriented() const {
    return source.allowsOriented();
}
inline bool NSurfaceSubset::isEmbeddedOnly() const {
    return source.isEmbeddedOnly();
}
inline NTriangulation* NSurfaceSubset::triangulation() const {
    return source.triangulation();
}
inline NTriangulation* NSurfaceSubset::getTriangulation() const {
    return source.triangulation();
}
inline size_t NSurfaceSubset::size() const {
    return surfaces.size();
}
inline size_t NSurfaceSubset::getNumberOfSurfaces() const {
    return surfaces.size();
}
inline const NNormalSurface* NSurfaceSubset::getSurface(size_t index) const {
    return surfaces[index];
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

