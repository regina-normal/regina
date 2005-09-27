
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file nlayeredsurfacebundle.h
 *  \brief Deals with layered surface bundle triangulations.
 */

#ifndef __NLAYEREDSURFACEBUNDLE_H
#ifndef __DOXYGEN
#define __NLAYEREDSURFACEBUNDLE_H
#endif

#include <memory>
#include "subcomplex/nstandardtri.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nmatrix2.h"

namespace regina {

class NTxICore;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * TODO
 */
class NLayeredTorusBundle : public NStandardTriangulation {
    private:
        const NTxICore& core_;
            /**< The core <tt>T x I</tt> triangulation whose boundaries
                 are joined (possibly via a layering of tetrahedra). */
        NIsomorphism* coreIso_;
            /**< Describes how the tetrahedra and vertices of the core
                 <tt>T x I</tt> triangulation returned by NTxICore::core()
                 map to the tetrahedra and vertices of the larger layered
                 torus bundle under consideration. */
        NMatrix2 reln_;
            /**< Describes how the layering of tetrahedra maps the
                 lower boundary curves to the upper boundary curves.
                 See layeringReln() for details. */

    public:
        /**
         * Destroys this layered torus bundle and all of its internal
         * components.
         */
        virtual ~NLayeredTorusBundle();

        /**
         * Returns the <tt>T x I</tt> triangulation at the core of this
         * layered surface bundle.  This is the product <tt>T x I</tt>
         * whose boundaries are joined (possibly via some layering of
         * tetrahedra).
         *
         * Note that the triangulation returned by NTxICore::core()
         * (that is, NLayeredSurfaceBundle::core().core()) may
         * well use different tetrahedron and vertex numbers.  That is,
         * an isomorphic copy of it appears within this layered surface
         * bundle but the individual tetrahedra and vertices may have
         * been permuted.  For a precise mapping from the NTxICore::core()
         * triangulation to this triangulation, see the routine coreIso().
         *
         * @return the core <tt>T x I</tt> triangulation.
         */
        const NTxICore& core() const;

        /**
         * TODO
         */
        const NIsomorphism* coreIso() const;

        /**
         * TODO.
         *
         * Expresses upper alpha/beta in terms of lower.
         */
        const NMatrix2& layeringReln() const;

        /**
         * Determines if the given triangulation is a layered surface bundle.
         *
         * @param tri the triangulation to examine.
         * @return a newly created structure containing details of the
         * layered surface bundle, or \c null if the given triangulation
         * is not a layered surface bundle.
         */
        static NLayeredTorusBundle* isLayeredTorusBundle(NTriangulation* tri);

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new structure based upon the given core <tt>T x I</tt>
         * triangulation.  Aside from this core, the structure will
         * remain uninitialised.
         *
         * Note that only a reference to the core <tt>T x I</tt> is stored.
         * This class does not manage the life span of the core; it is
         * assumed that the core will remain in existence for at least
         * as long as this object does.  (Usually the core is a static or
         * global variable that is not destroyed until the program exits.)
         *
         * @param whichCore a reference to the core <tt>T x I</tt>
         * triangulation upon which this layered surface bundle is based.
         */
        NLayeredTorusBundle(const NTxICore& whichCore);

        /**
         * Contains code common to both writeName() and writeTeXName().
         *
         * @param out the output stream to which to write.
         * @param tex \c true if this routine is called from
         * writeTeXName() or \c false if it is called from writeName().
         * @return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;

        /**
         * Internal to isLayeredTorusBundle().  Determines if the given
         * triangulation is a layered surface bundle with the given core
         * <tt>T x I</tt> triangulation (up to isomorphism).
         *
         * @param tri the triangulation to examine.
         * @param core the core <tt>T x I</tt> to search for.
         * @return a newly created structure containing details of the
         * layered surface bundle, or \c null if the given triangulation is
         * not a layered surface bundle with the given <tt>T x I</tt> core.
         */
        static NLayeredTorusBundle* hunt(NTriangulation* tri,
            const NTxICore& core);
};

/*@}*/

// Inline functions for NLayeredTorusBundle

inline NLayeredTorusBundle::NLayeredTorusBundle(const NTxICore& whichCore) :
        core_(whichCore), coreIso_(0) {
}

inline const NTxICore& NLayeredTorusBundle::core() const {
    return core_;
}

inline const NIsomorphism* NLayeredTorusBundle::coreIso() const {
    return coreIso_;
}

inline const NMatrix2& NLayeredTorusBundle::layeringReln() const {
    return reln_;
}

inline std::ostream& NLayeredTorusBundle::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

inline std::ostream& NLayeredTorusBundle::writeTeXName(std::ostream& out)
        const {
    return writeCommonName(out, true);
}

} // namespace regina

#endif

