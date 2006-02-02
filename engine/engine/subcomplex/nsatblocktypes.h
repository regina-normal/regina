
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file nsatblocktypes.h
 *  \brief Describes several types of saturated blocks within Seifert fibred
 *  space triangulations.
 */

#ifndef __NSATBLOCKTYPES_H
#ifndef __DOXYGEN
#define __NSATBLOCKTYPES_H
#endif

#include "subcomplex/nsatblock.h"

namespace regina {

class NLayeredSolidTorus;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * A degenerate zero-tetrahedron saturated block that corresponds to
 * attaching a Mobius band to a single annulus boundary.
 *
 * This is a degenerate case of the layered solid torus (see the class
 * NSatLST), where instead of joining a solid torus to an annulus
 * boundary we join a Mobius band.  The Mobius band can be thought of as
 * a zero-tetrahedron solid torus with two boundary faces, which in fact
 * are opposite sides of the same face.  By attaching a zero-tetrahedron
 * Mobius band to an annulus boundary, we are effectively joining the
 * two faces of the annulus together.
 *
 * The meridinal disc of this zero-tetrahedron solid torus meets the
 * three edges of the annulus in 1, 1 and 2 places, so it is in fact
 * a degenerate (1,1,2) layered solid torus.  Note that the weight 2 edge
 * is the boundary edge of the Mobius strip.
 */
class NSatMobius : public NSatBlock {
    private:
        int position_;
            /**< Describes how the Mobius band is attached to the
                 boundary annulus.  This can take the value 0, 1 or 2.
                 See the position() documentation for further details. */

    public:
        /**
         * Constructs a clone of the given block structure.
         *
         * @param cloneMe the block structure to clone.
         */
        NSatMobius(const NSatMobius& cloneMe);

        /**
         * Describes how the Mobius band is attached to the
         * boundary annulus.
         *
         * The class notes discuss the weight two edge of the Mobius band
         * (or equivalently the boundary edge of the Mobius band).  The
         * return value of this routine indicates which edge of the
         * boundary annulus this weight two edge is joined to.
         *
         * In the NSatAnnulus class notes, the three edges of the
         * annulus are denoted vertical, horizontal and boundary, and
         * the vertices of each face are given markings 0, 1 and 2.
         *
         * The return value of this routine takes the value 0, 1 or 2 as
         * follows:
         * - 0 means that the weight two edge is joined to the diagonal
         *   edge of the annulus (markings 1 and 2);
         * - 1 means that the weight two edge is joined to the horizontal
         *   edge of the annulus (markings 0 and 2);
         * - 2 means that the weight two edge is joined to the vertical
         *   edge of the annulus (markings 0 and 1).
         *
         * @return the value 0, 1 or 2 as described above.
         */
        int position() const;

        virtual NSatBlock* clone() const;
        virtual void adjustSFS(NSFSpace& sfs, bool reflect) const;
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Determines whether the given annulus is a boundary annulus for
         * a block of this type (Mobius band).  This routine is
         * a specific case of NSatBlock::isBlock(); see that routine for
         * further details.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static NSatMobius* isBlockMobius(const NSatAnnulus& annulus,
            TetList& avoidTets);

    private:
        /**
         * Constructs a partially initialised block.  The boundary
         * annulus will remain uninitialised, and must be initialised
         * before this block can be used.
         *
         * @param position indicates which edge of the boundary annulus
         * meets the weight two edge of the Mobius strip, as described in
         * the position() member function documentation.  This value
         * must be 0, 1 or 2.
         */
        NSatMobius(int position);
};

/**
 * A saturated block that is a layered solid torus.  See the
 * NLayeredSolidTorus class for details.
 *
 * The three boundary edges of the layered solid torus are attached to
 * the vertical, horizontal and diagonal edges of the boundary annulus;
 * see the NSatAnnulus class notes for details on precisely what
 * vertical, horizontal and diagonal mean.
 */
class NSatLST : public NSatBlock {
    private:
        NLayeredSolidTorus* lst_;
            /**< Contains details of the layered solid torus that this
                 block represents. */
        NPerm roles_;
            /**< Describes how the layered solid torus is attached to the
                 boundary annulus.  In particular, edge groups \a roles_[0],
                 \a roles_[1] and \a roles_[2] of the layered solid torus are
                 attached to the vertical, horizontal and diagonal edges of
                 the annulus respectively. */

    public:
        /**
         * Constructs a clone of the given block structure.
         *
         * @param cloneMe the block structure to clone.
         */
        NSatLST(const NSatLST& cloneMe);
        /**
         * Destroys this structure and its internal components.
         */
        ~NSatLST();

        /**
         * Returns details of the layered solid torus that this block
         * represents.
         *
         * @return details of the layered solid torus.
         */
        const NLayeredSolidTorus* lst() const;
        /**
         * Describes how the layered solid torus is attached to the
         * boundary annulus.
         *
         * The NLayeredSolidTorus class notes describe top-level edge
         * groups 0, 1 and 2 for a layered solid torus.  On the other
         * hand, the NSatAnnulus class notes define vertical, horizontal
         * and diagonal edges on the boundary annulus.
         *
         * Suppose that the permutation returned by this routine is \a r.
         * This indicates that:
         * - edge group \a r[0] is attached to the vertical annulus edges;
         * - edge group \a r[1] is attached to the horizontal annulus edges;
         * - edge group \a r[2] is attached to the diagonal annulus edges.
         *
         * The image \a r[3] will always be 3.
         *
         * @return a description of how the layered solid torus is
         * attached to the boundary annulus.
         */
        NPerm roles() const;

        virtual NSatBlock* clone() const;
        virtual void adjustSFS(NSFSpace& sfs, bool reflect) const;
        virtual void transform(const NTriangulation* originalTri,
            const NIsomorphism* iso, NTriangulation* newTri);
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Determines whether the given annulus is a boundary annulus for
         * a block of this type (layered solid torus).  This routine is
         * a specific case of NSatBlock::isBlock(); see that routine for
         * further details.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static NSatLST* isBlockLST(const NSatAnnulus& annulus,
            TetList& avoidTets);

    private:
        /**
         * Constructs a partially initialised block.  The boundary
         * annulus will remain uninitialised, and must be initialised
         * before this block can be used.
         *
         * @param lst details of the layered solid torus.
         * @param roles describes how the layered solid torus is
         * attached to the boundary annulus, as explained in the
         * \a roles_ data member documentation.
         */
        NSatLST(NLayeredSolidTorus* lst, NPerm roles);
};

/**
 * A saturated block that is a three-tetrahedron triangular prism.
 *
 * Such a prism may be of major type or of minor type.  In a \e major
 * type prism, the horizontal edges of the boundary annuli are all
 * major (degree three) edges of the prism.  Likewise, in a \e minor
 * type prism, the horizontal boundary edges are all minor (degree two)
 * edges of the prism.  See the NSatAnnulus class notes for a definition
 * of "horizontal" and the NTriSolidTorus class notes for further
 * details regarding "major" and "minor".
 */
class NSatTriPrism : public NSatBlock {
    protected:
        bool major_;
            /**< Is this prism of major type or of minor type? */

    public:
        /**
         * Constructs a clone of the given block structure.
         *
         * @param cloneMe the block structure to clone.
         */
        NSatTriPrism(const NSatTriPrism& cloneMe);

        /**
         * Is this prism of major type or minor type?  See the class
         * notes for further details.
         *
         * @return \c true if this prism is of major type, or \c false
         * if it is of minor type.
         */
        bool major() const;

        virtual NSatBlock* clone() const;
        virtual void adjustSFS(NSFSpace& sfs, bool reflect) const;
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Determines whether the given annulus is a boundary annulus for
         * a block of this type (triangular prism).  This routine is
         * a specific case of NSatBlock::isBlock(); see that routine for
         * further details.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static NSatTriPrism* isBlockTriPrism(const NSatAnnulus& annulus,
            TetList& avoidTets);

        /**
         * Inserts a new copy of a triangular prism block into the given
         * triangulation, and returns the corresponding block structure.
         *
         * The given triangulation will not be emptied before the new
         * tetrahedra are inserted.
         *
         * @param tri the triangulation into which the new block should
         * be inserted.
         * @param \c true if a block of major type should be inserted,
         * or \c false if a block of minor type should be inserted.
         * @return structural details of the newly inserted block.
         */
        static NSatTriPrism* insertBlock(NTriangulation& tri, bool major);

    protected:
        /**
         * Constructs a partially initialised block.  The boundary
         * annuli will remain uninitialised, and must be initialised
         * before this block can be used.
         *
         * @param major \c true if this block is of major type, or
         * \c false if it is of minor type.
         */
        NSatTriPrism(bool major);

    private:
        /**
         * Implements a special case of isBlockTriPrism() to search for
         * a block of major type.  See isBlockTriPrism() for further details.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static NSatTriPrism* isBlockTriPrismMajor(const NSatAnnulus& annulus,
            TetList& avoidTets);
};

/**
 * A saturated block that is a six-tetrahedron cube.
 *
 * There are several ways of triangulating a cube with six tetrahedra;
 * the specific method used here is illustrated in the diagram below.
 * Note that none of the four tetrahedra that meet the boundary annuli
 * touch each other, and that each of these four boundary tetrahedra
 * meet both central tetrahedra.  Note also that (unlike other
 * triangulations) this cube cannot be split vertically into two
 * triangular prisms.
 *
 * \image html cube.png
 */
class NSatCube : public NSatBlock {
    public:
        /**
         * Constructs a clone of the given block structure.
         *
         * @param cloneMe the block structure to clone.
         */
        NSatCube(const NSatTriPrism& cloneMe);

        virtual NSatBlock* clone() const;
        virtual void adjustSFS(NSFSpace& sfs, bool reflect) const;
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Determines whether the given annulus is a boundary annulus for
         * a block of this type (cube).  This routine is a specific case
         * of NSatBlock::isBlock(); see that routine for further details.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static NSatCube* isBlockCube(const NSatAnnulus& annulus,
            TetList& avoidTets);

        /**
         * Inserts a new copy of a cube block into the given triangulation,
         * and returns the corresponding block structure.
         *
         * The given triangulation will not be emptied before the new
         * tetrahedra are inserted.
         *
         * @param tri the triangulation into which the new block should
         * be inserted.
         * @return structural details of the newly inserted block.
         */
        static NSatCube* insertBlock(NTriangulation& tri);

    protected:
        /**
         * Constructs an uninitialised block.  The boundary annuli
         * must be initialised before this block can be used.
         */
        NSatCube();
};

/* TODO: New block types.
class NSatReflector : public NSatBlock {
};
class NSatLayering : public NSatBlock {
};
*/

/*@}*/

// Inline functions for NSatMobius

inline NSatMobius::NSatMobius(const NSatMobius& cloneMe) : NSatBlock(cloneMe),
        position_(cloneMe.position_) {
}

inline NSatMobius::NSatMobius(int position) : NSatBlock(1),
        position_(position) {
}

inline int NSatMobius::position() const {
    return position_;
}

inline NSatBlock* NSatMobius::clone() const {
    return new NSatMobius(*this);
}

inline void NSatMobius::writeTextShort(std::ostream& out) const {
    out << "Saturated Mobius band";
}

// Inline functions for NSatLST

inline NSatLST::NSatLST(NLayeredSolidTorus* lst, NPerm roles) : NSatBlock(1),
        lst_(lst), roles_(roles) {
}

inline const NLayeredSolidTorus* NSatLST::lst() const {
    return lst_;
}

inline NPerm NSatLST::roles() const {
    return roles_;
}

inline NSatBlock* NSatLST::clone() const {
    return new NSatLST(*this);
}

inline void NSatLST::writeTextShort(std::ostream& out) const {
    out << "Saturated layered solid torus";
}

// Inline functions for NSatTriPrism

inline NSatTriPrism::NSatTriPrism(const NSatTriPrism& cloneMe) :
        NSatBlock(cloneMe), major_(cloneMe.major_) {
}

inline NSatTriPrism::NSatTriPrism(bool major) : NSatBlock(3), major_(major) {
}

inline bool NSatTriPrism::major() const {
    return major_;
}

inline NSatBlock* NSatTriPrism::clone() const {
    return new NSatTriPrism(*this);
}

inline void NSatTriPrism::writeTextShort(std::ostream& out) const {
    out << "Saturated triangular prism";
}

// Inline functions for NSatCube

inline NSatCube::NSatCube(const NSatCube& cloneMe) : NSatBlock(cloneMe) {
}

inline NSatCube::NSatCube() : NSatBlock(4) {
}

inline NSatBlock* NSatCube::clone() const {
    return new NSatCube(*this);
}

inline void NSatCube::writeTextShort(std::ostream& out) const {
    out << "Saturated cube";
}

} // namespace regina

#endif

