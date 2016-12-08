
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file subcomplex/blockedsfs.h
 *  \brief Supports Seifert fibred spaces that are triangulated using
 *  saturated blocks.
 */

#ifndef __BLOCKEDSFS_H
#ifndef __DOXYGEN
#define __BLOCKEDSFS_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri.h"

namespace regina {

class SatBlock;
class SatRegion;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a blocked Seifert fibred space (possibly with boundary).
 * This is a particular type of triangulation of a Seifert fibred space,
 * where this triangulation is formed from a single saturated region.
 * A saturated region is in turn formed from saturated blocks by identifying
 * their boundary annuli in pairs; see the SatRegion class for details.
 *
 * Note that the routines writeName() and writeTeXName() do \e not
 * offer enough information to uniquely identify the triangulation,
 * since this essentially requires a 2-dimensional assembling of
 * saturated blocks.  For full detail, writeTextLong() may be used instead.
 *
 * The optional StandardTriangulation routine manifold() is
 * implemented for this class, but homology() is not.
 */
class REGINA_API BlockedSFS : public StandardTriangulation {
    private:
        SatRegion* region_;
            /**< The single saturated region that describes this entire
                 triangulation. */

    public:
        /**
         * Destroys this structure and its constituent components.
         */
        ~BlockedSFS();

        /**
         * Returns details of the single saturated region that
         * fills this triangulation.
         *
         * @return the single saturated region.
         */
        const SatRegion& region() const;

        /**
         * Determines whether this triangulation is a plugged thin
         * I-bundle or a plugged thick I-bundle.  These structures are
         * described in "Structures of small closed non-orientable
         * 3-manifold triangulations", Benjamin A. Burton,
         * J. Knot Theory Ramifications 16 (2007), 545--574.
         *
         * \ifacespython The argument \a name is not present.  Instead,
         * this routine returns a tuple of size two: the boolean usually
         * returned from this function, and the string usually returned in
         * the argument \a name.
         *
         * @param name used to return the name of the plugged thin/thick
         * I-bundle, if the triangulation is of this form.  If the
         * triangulation is not of this form, this string is not touched.
         * @return \c true if this triangulation is indeed a plugged thin
         * I-bundle or a plugged thick I-bundle.
         */
        bool isPluggedIBundle(std::string& name) const;

        Manifold* manifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        /**
         * Determines if the given triangulation is a blocked Seifert
         * fibred space.
         *
         * @param tri the triangulation to examine.
         * @return a newly created structure containing details of the
         * blocked Seifert fibred space, or \c null if the given
         * triangulation is not a blocked Seifert fibred space.
         */
        static BlockedSFS* isBlockedSFS(Triangulation<3>* tri);

    private:
        /**
         * Constructs a new blocked Seifert fibred space, as described by
         * the given saturated region.  The new object will take ownership
         * of the given region.
         *
         * Note that the new object must describe an existing triangulation.
         *
         * @param region the region describing this entire triangulation.
         */
        BlockedSFS(SatRegion* region);

        /**
         * Attempts to identify the solid torus plugs in a plugged thin
         * I-bundle or a plugged thick I-bundle.  This routine is
         * internal to isPluggedIBundle().
         *
         * It is assumed that the plugged thin/thick I-bundle has been
         * completely identified, with the exception of the two solid
         * torus plugs.  Corresponding parameters describing the
         * core I-bundle must be passed, along with two blocks that
         * should correspond to the two plugs.
         *
         * If the two blocks are indeed solid torus plugs (either layered
         * solid tori or Mobius bands), the full name of the plugged
         * thin/thick I-bundle will be filled in and \c true will be
         * returned.  Note that this name may be normalised or otherwise
         * modified to return a simpler set of parameters for the same
         * triangulation.  If either block is not a solid torus plug then
         * \c false will be returned.
         *
         * @param thin \c true if the overall structure being identified
         * is a plugged thin I-bundle, or \c false if it is a plugged
         * thick I-bundle.
         * @param id identifies the particular thin/thick twisted
         * I-bundle into which the solid tori are plugged.  This must be
         * 1, 2, 3 or 4, to distinguish between the four thin twisted
         * I-bundles or the four thick twisted I-bundles described in
         * the paper "Structures of small closed non-orientable
         * 3-manifold triangulations" (see isPluggedIBundle for details).
         * @param name used to return the full parameterised name of this
         * triangulation.  If the two given blocks are not solid torus
         * plugs, this string is not touched.
         * @param torus0 the block that should provide the solid torus plug
         * corresponding to the first pair of integers in the plugged
         * thin/thick I-bundle parameters.
         * @param horiz0 \c true if the first pair of integers in the
         * plugged thin/thick I-bundle parameters should measure the
         * number of times the meridinal curve cuts the vertical and
         * horizontal edges of the adjacent block (not the block
         * \a torus0, but its neighbour), or \c false if the vertical
         * and diagonal edges should be used instead.
         * @param torus1 the block that should provide the solid torus plug
         * corresponding to the second pair of integers in the plugged
         * thin/thick I-bundle parameters.
         * @param horiz1 \c true if the second pair of integers in the
         * plugged thin/thick I-bundle parameters should measure the
         * number of times the meridinal curve cuts the vertical and
         * horizontal edges of the adjacent block (not the block
         * \a torus1, but its neighbour), or \c false if the vertical
         * and diagonal edges should be used instead.
         * @return \c true if the two given blocks are both solid torus
         * plugs (either layered solid tori or Mobius bands), or \c false
         * otherwise.
         */
        static bool findPluggedTori(bool thin, int id, std::string& name,
            const SatBlock* torus0, bool horiz0,
            const SatBlock* torus1, bool horiz1);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NBlockedSFS has now been renamed to
 * BlockedSFS.
 */
REGINA_DEPRECATED typedef BlockedSFS NBlockedSFS;

/*@}*/

// Inline functions for BlockedSFS

inline BlockedSFS::BlockedSFS(SatRegion* region) : region_(region) {
}

inline const SatRegion& BlockedSFS::region() const {
    return *region_;
}

} // namespace regina

#endif

