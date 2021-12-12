
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file subcomplex/augtrisolidtorus.h
 *  \brief Deals with augmented triangular solid torus components of a
 *  triangulation.
 */

#ifndef __REGINA_AUGTRISOLIDTORUS_H
#ifndef __DOXYGEN
#define __REGINA_AUGTRISOLIDTORUS_H
#endif

#include <optional>
#include "regina-core.h"
#include "subcomplex/trisolidtorus.h"
#include "subcomplex/layeredsolidtorus.h"
#include "triangulation/forward.h"

namespace regina {

/**
 * Represents an augmented triangular solid torus component of a
 * triangulation.  Such a component is obtained as follows.  Begin with
 * a three-tetrahedron triangular solid torus (as described by
 * TriSolidTorus).  Observe that the three axis edges divide the
 * boundary into three annuli.  Then take one of the following actions.
 *
 *   - To each of these annuli, glue a layered solid torus.
 *     Note that the degenerate (2,1,1) layered solid torus
 *     (i.e., a one-triangle mobius strip) is allowed and corresponds to
 *     simply gluing the two triangles of the annulus together.
 *
 *   - To one of these annuli, glue a layered solid torus as described
 *     above.  Join the other two annuli with a layered chain
 *     in either the manner described by
 *     TriSolidTorus::areAnnuliLinkedMajor() or the manner described by
 *     TriSolidTorus::areAnnuliLinkedAxis().
 *
 * It will be assumed that all layered solid tori other than the
 * degenerate (2,1,1) will have (3,2,1) layered solid tori at their
 * bases.  That is, layered solid tori that begin with the degenerate
 * (2,1,1) and layer over the boundary of the mobius strip are \b not
 * considered in this class.
 *
 * Note that (unless a (1,1,0) layered solid torus is used with the 0
 * edge glued to an axis edge) the resulting space will be a Seifert
 * fibred space over the 2-sphere with at most three exceptional fibres.
 *
 * Of the optional StandardTriangulation routines, manifold() is
 * implemented for most augmented triangular solid tori and
 * homology() is not implemented at all.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other StandardTriangulation
 * subclasses.  Note that the only way to create these objects (aside from
 * copying or moving) is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class AugTriSolidTorus : public StandardTriangulation {
    public:
        static constexpr int CHAIN_NONE = 0;
            /**< Indicates that this augmented triangular solid torus
                 contains no layered chain. */
        static constexpr int CHAIN_MAJOR = 1;
            /**< Indicates that this augmented triangular solid torus
                 contains a layered chain attached as described by
                 TriSolidTorus::areAnnuliLinkedMajor(). */
        static constexpr int CHAIN_AXIS = 2;
            /**< Indicates that this augmented triangular solid torus
                 contains a layered chain attached as described by
                 TriSolidTorus::areAnnuliLinkedAxis(). */

    private:
        TriSolidTorus core_;
            /**< The triangular solid torus at the core of this
                 triangulation. */
        std::optional<LayeredSolidTorus> augTorus_[3];
            /**< The layered solid tori attached to the boundary annuli.
                 If one of the layered solid tori is a degenerate (2,1,1)
                 triangle, the corresponding value will be std::nullopt.
                 Note that <tt>augTorus[i]</tt> will be attached to
                 annulus \c i of the triangular solid torus. */
        Perm<4> edgeGroupRoles_[3];
            /**< Permutation <tt>edgeGroupRoles[i]</tt> describes the role
                 played by each top level edge group of layered solid torus
                 <tt>i</tt>.  For permutation <tt>p</tt>, group <tt>p[0]</tt>
                 is glued to an axis edge, group <tt>p[1]</tt> is glued to a
                 major edge and group <tt>p[2]</tt> is glued to a minor edge. */
        unsigned long chainIndex_;
            /**< The number of tetrahedra in the layered chain if
                 present, or 0 if there is no layered chain. */
        int chainType_;
            /**< The way in which the layered chain is attached, or
                 \a CHAIN_NONE if there is no layered chain. */
        int torusAnnulus_;
            /**< The annulus to which the single layered solid torus is
                 attached (if there is a layered chain), or -1 if there is
                 no layered chain. */

    public:
        /**
         * Creates a new copy of this structure.
         */
        AugTriSolidTorus(const AugTriSolidTorus&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * @return a reference to this structure.
         */
        AugTriSolidTorus& operator = (const AugTriSolidTorus&) = default;

        /**
         * Deprecated routine that returns a new copy of this structure.
         *
         * \deprecated Just use the copy constructor instead.
         *
         * @return a newly created clone.
         */
        [[deprecated]] AugTriSolidTorus* clone() const;

        /**
         * Swaps the contents of this and the given structure.
         *
         * @param other the structure whose contents should be swapped
         * with this.
         */
        void swap(AugTriSolidTorus& other) noexcept;

        /**
         * Returns the triangular solid torus at the core of this
         * triangulation.
         *
         * @return the core triangular solid torus.
         */
        const TriSolidTorus& core() const;
        /**
         * Returns the layered solid torus attached to the requested
         * annulus on the boundary of the core triangular solid torus.
         * If the layered solid torus is a degenerate (2,1,1) mobius
         * band (i.e., the two triangles of the corresponding annulus have
         * simply been glued together), then no value will be returned.
         *
         * @param annulus specifies which annulus to examine; this must
         * be 0, 1 or 2.
         * @return the corresponding layered solid torus.
         */
        const std::optional<LayeredSolidTorus>& augTorus(int annulus) const;

        /**
         * Returns a permutation describing the role played by each top
         * level edge group of the layered solid torus glued to the
         * requested annulus of the core triangular solid torus.  See
         * LayeredSolidTorus::topEdge() for details regarding edge groups.
         *
         * If the permutation returned is <tt>p</tt>, edge group <tt>p[0]</tt>
         * will be glued to an axis edge, group <tt>p[1]</tt> will be
         * glued to a major edge and group <tt>p[2]</tt> will be glued
         * to a minor edge.  <tt>p[3]</tt> will always be 3.
         *
         * Even if the corresponding layered solid torus is a degenerate
         * (2,1,1) mobius band (i.e., augTorus() returns \c null),
         * the concept of edge groups is still
         * meaningful and this routine will return correct results.
         *
         * @param annulus specifies which annulus to examine; this must
         * be 0, 1 or 2.  It is the layered solid torus glued to this
         * annulus whose edge groups will be described.
         * @return a permutation describing the roles of the
         * corresponding top level edge groups.
         */
        Perm<4> edgeGroupRoles(int annulus) const;

        /**
         * Returns the number of tetrahedra in the layered chain linking
         * two of the boundary annuli of the core triangular solid torus.
         * Note that this count does not include any of the tetrahedra
         * actually belonging to the triangular solid torus.
         *
         * @return the number of tetrahedra in the layered chain, or 0
         * if there is no layered chain linking two boundary annuli.
         */
        unsigned long chainLength() const;

        /**
         * Returns the way in which a layered chain links
         * two of the boundary annuli of the core triangular solid torus.
         * This will be one of the chain type constants defined in this
         * class.
         *
         * @return the type of layered chain, or \a CHAIN_NONE
         * if there is no layered chain linking two boundary annuli.
         */
        int chainType() const;

        /**
         * Returns the single boundary annulus of the core triangular
         * solid torus to which a layered solid torus is attached.  This
         * routine is only meaningful if the other two annuli are linked
         * by a layered chain.
         *
         * The integer returned will be 0, 1 or 2; see the
         * TriSolidTorus class notes for how the boundary annuli are
         * numbered.
         *
         * @return the single annulus to which the layered solid torus
         * is attached, or -1 if there is no layered chain (and thus all
         * three annuli have layered solid tori attached).
         */
        int torusAnnulus() const;

        /**
         * Determines whether the core triangular solid torus has two of
         * its boundary annuli linked by a layered chain as described in
         * the general class notes.
         *
         * @return \c true if and only if the layered chain described in
         * the class notes is present.
         */
        bool hasLayeredChain() const;

        /**
         * Determines whether this and the given structure represent
         * the same type of augmented triangular solid torus.
         *
         * Specifically, two structures will compare as equal if and only if:
         *
         * - in both structures, the layered solid tori attached to the
         *   same numbered annuli have the same three integer parameters,
         *   and have their top level edge groups attached to the annuli in
         *   the same manner;
         *
         * - either both structures do not include a layered chain, or else
         *   both structures do include a layered chain of the same type and
         *   length, attached to the same numbered annulus.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * this test does not account for the many symmetries in an
         * augmented triangular solid torus).
         *
         * @param other the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * represent the same type of augmented triangular solid torus.
         */
        bool operator == (const AugTriSolidTorus& other) const;

        /**
         * Determines whether this and the given structure represent
         * different types of augmented triangular solid torus.
         *
         * Specifically, two structures will compare as equal if and only if:
         *
         * - in both structures, the layered solid tori attached to the
         *   same numbered annuli have the same three integer parameters,
         *   and have their top level edge groups attached to the annuli in
         *   the same manner;
         *
         * - either both structures do not include a layered chain, or else
         *   both structures do include a layered chain of the same type and
         *   length, attached to the same numbered annulus.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * this test does not account for the many symmetries in an
         * augmented triangular solid torus).
         *
         * @param other the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * represent different types of augmented triangular solid torus.
         */
        bool operator != (const AugTriSolidTorus& other) const;

        /**
         * Determines if the given triangulation component is an
         * augmented triangular solid torus.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * @param comp the triangulation component to examine.
         * @return a structure containing details of the augmented triangular
         * solid torus, or \c null if the given component is not an augmented
         * triangular solid torus.
         */
        static std::unique_ptr<AugTriSolidTorus> recognise(
            const Component<3>* comp);
        /**
         * A deprecated alias to recognise if a component forms an
         * augmented triangular solid torus.
         *
         * \deprecated This function has been renamed to recognise().
         * See recognise() for details on the parameters and return value.
         */
        [[deprecated]] static std::unique_ptr<AugTriSolidTorus>
            isAugTriSolidTorus(const Component<3>* comp);

        std::unique_ptr<Manifold> manifold() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure with the given core.
         * The chain type will be initialised to CHAIN_NONE, and
         * all other data members will be left uninitialised.
         */
        AugTriSolidTorus(const TriSolidTorus& core);

        /**
         * Contains code common to both writeName() and writeTeXName().
         *
         * @param out the output stream to which to write.
         * @param tex \c true if this routine is called from
         * writeTeXName() or \c false if it is called from writeName().
         * @return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls AugTriSolidTorus::swap(); it is provided
 * so that AugTriSolidTorus meets the C++ Swappable requirements.
 *
 * @param a the first structure whose contents should be swapped.
 * @param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(AugTriSolidTorus& a, AugTriSolidTorus& b) noexcept;

// Inline functions for AugTriSolidTorus

inline AugTriSolidTorus::AugTriSolidTorus(const TriSolidTorus& core) :
        core_(core), chainType_(CHAIN_NONE) {
}

inline AugTriSolidTorus* AugTriSolidTorus::clone() const {
    return new AugTriSolidTorus(*this);
}

inline void AugTriSolidTorus::swap(AugTriSolidTorus& other) noexcept {
    core_.swap(other.core_);
    augTorus_[0].swap(other.augTorus_[0]);
    augTorus_[1].swap(other.augTorus_[1]);
    augTorus_[2].swap(other.augTorus_[2]);
    std::swap_ranges(edgeGroupRoles_, edgeGroupRoles_ + 3,
        other.edgeGroupRoles_);
    std::swap(chainIndex_, other.chainIndex_);
    std::swap(chainType_, other.chainType_);
    std::swap(torusAnnulus_, other.torusAnnulus_);
}

inline const TriSolidTorus& AugTriSolidTorus::core() const {
    return core_;
}
inline const std::optional<LayeredSolidTorus>& AugTriSolidTorus::augTorus(
        int annulus) const {
    return augTorus_[annulus];
}
inline Perm<4> AugTriSolidTorus::edgeGroupRoles(int annulus) const {
    return edgeGroupRoles_[annulus];
}
inline unsigned long AugTriSolidTorus::chainLength() const {
    return chainIndex_;
}
inline int AugTriSolidTorus::chainType() const {
    return chainType_;
}
inline int AugTriSolidTorus::torusAnnulus() const {
    return torusAnnulus_;
}
inline bool AugTriSolidTorus::hasLayeredChain() const {
    return (chainIndex_ != 0);
}

inline bool AugTriSolidTorus::operator == (
        const AugTriSolidTorus& other) const {
    if (chainIndex_ != other.chainIndex_)
        return false;

    if (chainIndex_) {
        return
            chainType_ == other.chainType_ &&
            torusAnnulus_ == other.torusAnnulus_ &&
            edgeGroupRoles_[torusAnnulus_] ==
                other.edgeGroupRoles_[torusAnnulus_] &&
            augTorus_[torusAnnulus_] == other.augTorus_[torusAnnulus_];
    } else {
        return
            std::equal(edgeGroupRoles_, edgeGroupRoles_ + 3,
                other.edgeGroupRoles_) &&
            std::equal(augTorus_, augTorus_ + 3, other.augTorus_);
    }
}

inline bool AugTriSolidTorus::operator != (
        const AugTriSolidTorus& other) const {
    return ! ((*this) == other);
}

inline void swap(AugTriSolidTorus& a, AugTriSolidTorus& b) noexcept {
    a.swap(b);
}

inline std::unique_ptr<AugTriSolidTorus> AugTriSolidTorus::isAugTriSolidTorus(
        const Component<3>* comp) {
    return recognise(comp);
}

} // namespace regina

#endif

