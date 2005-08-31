
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file npluggedibundle.h
 *  \brief Deals with plugged I-bundle triangulations.
 */

#ifndef __NPLUGGEDIBUNDLE_H
#ifndef __DOXYGEN
#define __NPLUGGEDIBUNDLE_H
#endif

#include <memory>
#include "subcomplex/nstandardtri.h"
#include "triangulation/ntriangulation.h"

namespace regina {

class NIsomorphism;
class NLayeredSolidTorus;

/**
 * \weakgroup subcomplex
 * @{
 */

struct NPluggedIBundleCore : public boost::noncopyable {
    static const int T_5_1;
    static const int T_5_2;
    static const int T_5_3;
    static const int T_5_4;
    static const int T_6_1;
    static const int T_6_2;
    static const int T_6_3;
    static const int T_6_4;

    int coreType;
    NTriangulation core;
    unsigned topTet[2][2];
    NPerm topVertices[2][2];
        /**< p[0]-p[1] is vertical, p[0]-p[2] is horizontal. */

    NPluggedIBundleCore(int whichCoreType);
};

/**
 * Represents a plugged triangular solid torus component of a
 * triangulation.  Such a component is obtained as follows.
 *
 * Begin with a three-tetrahedron triangular solid torus (as described by
 * class NTriSolidTorus).  Observe that the three axis edges divide the
 * boundary into three annuli.
 *
 * To each of these annuli a layered chain may be optionally attached.
 * If present, the chain should be attached so its hinge edges are
 * identified with the axis edges of the corresonding annulus and its bottom
 * tetrahedron is layered over either the major edge or minor edge of the
 * corresponding annulus.  The top two faces of the layered chain should
 * remain free.
 *
 * Thus we now have three annuli on the boundary, each represented as a
 * square two of whose (opposite) edges are axis edges of the original
 * triangular solid torus (and possibly also hinge edges of a layered
 * chain).
 *
 * Create a \e plug by gluing two tetrahedra together along a single
 * face.  The six edges that do not run along this common face split the
 * plug boundary into three squares.  These three squares must be glued
 * to the three boundary annuli previously described.  Each axis edge meets
 * two of the annuli; the two corresponding edges of the plug must be
 * non-adjacent (have no common vertex) on the plug.
 * In this way each of the six edges of the plug not running along its
 * interior face corresponds to precisely one of the two instances of
 * precisely one of the three axis edges.
 *
 * If the axis edges are directed so that they all point the
 * same way around the triangular solid torus, these axis edges when
 * drawn on the plug must all point from one common tip of the plug to
 * the other (where the \e tips of the plug are the vertices not meeting the
 * interior face).  The gluings must also be made so that the resulting
 * triangulation component is orientable.
 *
 * Of the optional NStandardTriangulation routines, getManifold() is
 * implemented for most plugged triangular solid tori and
 * getHomologyH1() is not implemented at all.
 */
class NPluggedIBundle : public NStandardTriangulation {
    private:
        NIsomorphism* core;
        int coreType;
        NLayeredSolidTorus* plug[2];
        NPerm edgeGroupRoles[2];
            /**< Permutation <tt>edgeGroupRoles[i]</tt> describes the role
                 played by each top level edge group of layered solid torus
                 <tt>i</tt>.  For permutation <tt>p</tt>, group <tt>p[0]</tt>
                 is glued to a vertical edge, group <tt>p[1]</tt> is glued to a
                 horizontal edge and group <tt>p[2]</tt> is glued to a
                 diagonal edge. */

    public:
        /**
         * Destroys this plugged thin I-bundle; note that the corresponding
         * isomorphism and layered solid tori will also be destroyed.
         */
        virtual ~NPluggedIBundle();
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        NPluggedIBundle* clone() const;

        const NLayeredSolidTorus* getPlug(int which) const;

        static NPluggedIBundle* isPluggedIBundle(NTriangulation* tri);

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new structure with all subcomponent pointers
         * initialised to \c null.
         */
        NPluggedIBundle();

        /**
         * Contains code common to both writeName() and writeTeXName().
         *
         * @param out the output stream to which to write.
         * @param tex \c true if this routine is called from
         * writeTeXName() or \c false if it is called from writeName().
         * @return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;

        static NPluggedIBundle* hunt(NTriangulation* tri,
                const NPluggedIBundleCore& core);
};

/*@}*/

// Inline functions for NPluggedIBundle

inline NPluggedIBundle::NPluggedIBundle() : core(0) {
    plug[0] = plug[1] = 0;
}

inline const NLayeredSolidTorus* NPluggedIBundle::getPlug(int which) const {
    return plug[which];
}

} // namespace regina

#endif

