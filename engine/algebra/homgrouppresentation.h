
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file algebra/homgrouppresentation.h
 *  \brief Deals with finite presentations of groups.
 */

// The headers grouppresentation.h and homgrouppresentation.h are fragile,
// in that we _must_ ensure that grouppresentation.h is read first.
// That will in turn re-include _this_ header at the correct point in the file.
#include "algebra/grouppresentation.h"

#ifndef __REGINA_HOMGROUPPRESENTATION_H
#ifndef __DOXYGEN
#define __REGINA_HOMGROUPPRESENTATION_H
#endif

namespace regina {

class GroupPresentation;

/**
 * Represents a homomorphism between groups which are described via finite
 * presentations.
 *
 * Some homomorphisms may be _declared isomorphisms_.  This means
 * that the user (or some other function in Regina) has proven that this
 * is an isomorphism and has explicitly provided the inverse map.
 * To provide the inverse map, you should call the four-argument constructor
 * HomGroupPresentation(const GroupPresentation&, const GroupPresentation&,
 * const std::vector<GroupExpression>&, const std::vector<GroupExpression>&).
 *
 * You can test for a declared isomorphism by calling knowsInverse().  Even
 * if a homomorphism is not a declared isomorphism, it might still be an
 * isomorphism; this just means that no inverse map was explicitly provided.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \apinotfinal
 *
 * \todo Add a routine to attempt to verify validity of homomorphism.
 *
 * \ingroup algebra
 */
class HomGroupPresentation : public Output<HomGroupPresentation> {
    private:
        GroupPresentation domain_;
            /**< The domain of the homomorphism. */
        GroupPresentation codomain_;
            /**< The codomain of the homomorphism. */
        std::vector<GroupExpression> map_;
            /**< A map whose ith element is the image in the codomain
                 of the ith generator from the domain. */
        std::optional<std::vector<GroupExpression>> inv_;
            /**< No value unless this is a declared isomorphism, in which case
                 this will be a map whose ith element is the image in the
                 domain of the ith generator from the codomain. */

    public:
        /**
         * Creates a new homomorphism from the given data.
         *
         * \param domain the domain of the homomorphism.
         * \param codomain the codomain of the homomorphism.
         * \param map a vector of length \a g, where \a g is the number
         * of generators of the domain, and where this homomorphism
         * sends the <i>i</i>th generator of the domain to the
         * element `map[i]` of the codomain.
         */
        HomGroupPresentation(GroupPresentation domain,
                GroupPresentation codomain,
                std::vector<GroupExpression> map);

        /**
         * Creates a declared isomorphism from the given data.
         * Here you must provide both a map from the domain to codomain,
         * and the inverse map from the codomain to domain.
         *
         * \pre The argument \a inv is indeed the inverse of \a map.
         *
         * \param domain the domain of the homomorphism.
         * \param codomain the codomain of the homomorphism.
         * \param map a vector of length \a g, where \a g is the number
         * of generators of the domain, and where this homomorphism
         * sends the <i>i</i>th generator of the domain to the
         * element `map[i]` of the codomain.
         * \param inv a vector of length \a k where \a k is the number
         * of generators of the codomain, and where the inverse homomorphism
         * sends the <i>i</i>th generator of the codomain to the
         * element `inv[i]` of the domain.
         */
        HomGroupPresentation(GroupPresentation domain,
                GroupPresentation codomain,
                std::vector<GroupExpression> map,
                std::vector<GroupExpression> inv);

        /**
         * Creates a new identity homomorphism for the given group.
         *
         * This will be a declared isomorphism (see the
         * HomGroupPresentation class notes for details).
         *
         * \param groupForIdentity both the domain and codomain of the
         * new identity homomorphism.
         */
        HomGroupPresentation(const GroupPresentation& groupForIdentity);

        /**
         * Creates a clone of the given homomorphism.
         */
        HomGroupPresentation(const HomGroupPresentation& src) = default;

        /**
         * Moves the contents of the given homomorphism into this new
         * homomorphism.  This is a fast (constant time) operation.
         *
         * The homomorphism that was passed will no longer be usable.
         */
        HomGroupPresentation(HomGroupPresentation&& src) noexcept = default;

        /**
         * Sets this to be a clone of the given homomorphism.
         *
         * \return a reference to this homomorphism.
         */
        HomGroupPresentation& operator = (const HomGroupPresentation&) =
            default;

        /**
         * Moves the contents of the given homomorphism to this homomorphism.
         * This is a fast (constant time) operation.
         *
         * The homomorphism that was passed will no longer be usable.
         *
         * \return a reference to this homomorphism.
         */
        HomGroupPresentation& operator = (HomGroupPresentation&&) noexcept =
            default;

        /**
         * Swaps the contents of this and the given homomorphism.
         *
         * \param other the homomorphism whose contents should be swapped with
         * this.
         */
        void swap(HomGroupPresentation& other) noexcept;

        /**
         * Determines whether this and the given homomorphism have
         * identical presentations.
         *
         * This routine does _not_ test whether the two homomorphisms
         * are equal in the sense that each element of the domain maps
         * to the same group element of the codomain - in general this
         * is an undecidable problem.
         *
         * Instead, this routine tests whether the two homomorphisms map the
         * <i>i</i>th generator of the domain to precisely the same _word_
         * in the codomain, for each \a i.
         *
         * This routine will not test whether the domains and codomains
         * have identical presentations, or whether either homomorphism
         * is declared to be an isomorphism.  However, if the two domains
         * have different numbers of generators then the two homomorphisms
         * will compare as not equal (since it will be impossible to compare
         * the words that the corresponding generators map to).
         *
         * \param other the homomorphism to compare with this.
         * \return \c true if and only if this and the given homomorphisms
         * have identical presentations.
         */
        bool operator == (const HomGroupPresentation& other) const;

        /**
         * The domain of the map.
         *
         * \return a reference to the domain.
         */
        const GroupPresentation& domain() const;
        /**
         * The codomain of the map.
         *
         * \return a reference to the codomain.
         */
        const GroupPresentation& codomain() const;

        /**
         * Returns whether or not this is a declared isomorphism.
         *
         * A _declared isomorphism_ is a isomorphism for which the
         * user has explicitly provided the inverse map.  See the
         * HomGroupPresentation class notes for details.
         *
         * \return \c true if and only if this is a declared
         * isomorphism, i.e, the inverse map was explicitly provided.
         */
        bool knowsInverse() const;

        /**
         * Evaluate the homomorphism at an element of the domain.
         *
         * \param arg an element of the domain.
         * \return the image of this element in the codomain.
         */
        GroupExpression evaluate(GroupExpression arg) const;

        /**
         * Evaluate the homomorphism at a generator of the domain.
         *
         * \param i the index of a generator in the domain.
         * \return the image of the <i>i</i>th generator in the codomain.
         */
        GroupExpression evaluate(unsigned long i) const;

        /**
         * Evaluate the isomorphisms's inverse at an element of the codomain.
         *
         * \pre This homomorphism is in fact a declared isomorphism.
         * See the HomGroupPresentation class notes for details
         * on what this means.
         *
         * \param arg an element of the codomain.
         * \return the image of this element in the domain.
         */
        GroupExpression invEvaluate(GroupExpression arg) const;
        /**
         * Evaluate the isomorphism at a generator of the codomain.
         *
         * \pre This homomorphism is in fact a declared isomorphism.
         * See the HomGroupPresentation class notes for details
         * on what this means.
         *
         * \param i the index of a generator in the codomain.
         * \return the image of this generator in the domain.
         */
        GroupExpression invEvaluate(unsigned long i) const;

        /**
         * Simultaneously simplifies the presentation of the domain, the
         * presentation of the codomain, and the description of the map.
         *
         * Uses the underlying GroupPresentation::simplify().
         * See that routine for details.
         *
         * \return \c true if and only if either presentation and/or the
         * map was changed.
         */
        bool simplify();

        /**
         * Deprecated alias for simplify(), which simultaneously simplifies
         * the presentation of the domain, the presentation of the codomain,
         * and the description of the map.
         *
         * \deprecated This routine has been renamed to simplify().
         * See simplify() for further details.
         *
         * \return \c true if and only if either presentation and/or the
         * map was changed.
         */
        [[deprecated]] bool intelligentSimplify();

        /**
         * Simplifies the domain and codomain using only Nielsen moves, keeping
         * track of the resulting map in the progress.
         *
         * \return \c true if and only if either presentation and/or the
         * map was changed.
         */
        bool nielsen();

        /**
         * Deprecated alias for nielsen(), which simplifies the domain and
         * codomain using only Nielsen moves, keeping track of the resulting
         * map in the progress.
         *
         * \deprecated This routine has been renamed to nielsen().
         * See nielsen() for further details.
         *
         * \return \c true if and only if either presentation and/or the
         * map was changed.
         */
        [[deprecated]] bool intelligentNielsen();

        /**
         * Simplifies the domain and codomain using only small cancellation
         * theory.
         *
         * \return \c true if and only if either presentation and/or the
         * map was changed.
         */
        bool smallCancellation();

        /**
         * Composes this homomorphism with the given homomorphism.
         *
         * Evaluating the composition on some group element \a x is the
         * same as evaluating `this(rhs(x))`.
         * In other words, in this composition, \a rhs is evaluated first
         * and then the output of that is evaluated by this homomorphism.
         *
         * If both of the given homomorphisms are declared isomorphisms,
         * then the return value will be a declared isomoprhism also.
         *
         * \pre the codomain of \a rhs must be the same as the domain of this
         * homomorphism.
         *
         * \param rhs the homomorphism to compose this with.
         * \return the composition of both homomorphisms.
         */
        HomGroupPresentation operator * (const HomGroupPresentation& rhs) const;

        /**
         * Composes this homomorphism with the given homomorphism.
         *
         * Evaluating the composition on some group element \a x is the
         * same as evaluating `this(rhs(x))`.
         * In other words, in this composition, \a rhs is evaluated first
         * and then the output of that is evaluated by this homomorphism.
         *
         * If both of the given homomorphisms are declared isomorphisms,
         * then the return value will be a declared isomoprhism also.
         *
         * \pre the codomain of \a rhs must be the same as the domain of this
         * homomorphism.
         *
         * \param rhs the homomorphism to compose this with.
         * \return the composition of both homomorphisms.
         */
        HomGroupPresentation operator * (HomGroupPresentation&& rhs) const;

        /**
         * Inverts the homomorphism.
         *
         * This is only possible if the homomorphism is in fact a
         * declared isomorphism (which means that the inverse map is
         * already stored internally).  See the HomGroupPresentation
         * class notes for further details on declared isomorphisms.
         *
         * If this is not a declared isomorphism then this routine
         * will do nothing and simply return \c false.
         *
         * This operation is (very) fast constant time.
         *
         * \return \c true if and only if the inversion operation was
         * successful (i.e., if this is a declared isomorphism).
         */
        bool invert();

        /**
         * Verifies the map is a valid homomorphism.  Specifically, this
         * routine runs through all the relators in the domain, evaluates
         * the homomorphism on the relators and checks that they simplify
         * to 1 in the codomain.
         *
         * This routine does not guarantee a conclusive result (since
         * the word problem is, in general, undecidable).  If this
         * routine returns \c true then this proves that the
         * homomorphism is indeed valid.  If this routine returns \c false,
         * then the result is inconclusive (i.e., it might still be
         * valid but Regina was not able to prove this).
         *
         * This routine is intended for sanity checking only: any homomorphism
         * that you construct in Regina should always be valid in this sense.
         *
         * \return \c true if Regina is able to verify that this is a
         * homomorphism, or \c false if the result is inconclusive.
         */
        bool verify() const;

        /**
         * Attempts to verify that a declared isomorphism is, indeed,
         * an isomorphism.
         *
         * This routine works by attempting to verify that
         * `f^-1(f(x))x^-1` simplifes to 1 for all generators \a x
         * in the domain, and likewise for the codomain.
         *
         * This routine does not guarantee a conclusive result.  If this
         * routine returns \c true then this proves that this is indeed
         * an isomorphism.  If this routine returns \c false then the result
         * is inconclusive (i.e., it might still be an isomorphism
         * but Regina was not able to prove this).
         *
         * You probably only want to run this on good presentations for small
         * cancellation theory - an automorphism of a poorly-presented group
         * likely will not be noticed.
         *
         * This routine is intended for sanity checking only: any homomorphism
         * that you construct as a declared isomorphism should always be
         * an isomorphism.
         *
         * \pre This homomorphism is in fact a declared isomorphism.
         * See the HomGroupPresentation class notes for details
         * on what this means.
         *
         * \return \c true if it is verified that this is an
         * isomorphism, or \c false if the result is inconclusive.
         */
        bool verifyIsomorphism() const;

        /**
         * Computes the induced map on the abelianizations of the domain
         * and codomain.
         *
         * \return the induced map on the abelianizations.
         */
        HomMarkedAbelianGroup markedAbelianisation() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;
};

/**
 * Swaps the contents of the two given homomorphisms.
 *
 * This global routine simply calls HomGroupPresentation::swap(); it is provided
 * so that HomGroupPresentation meets the C++ Swappable requirements.
 *
 * \param lhs the homomorphism whose contents should be swapped with \a rhs.
 * \param rhs the homomorphism whose contents should be swapped with \a lhs.
 *
 * \ingroup algebra
 */
void swap(HomGroupPresentation& lhs, HomGroupPresentation& rhs) noexcept;

inline HomGroupPresentation::HomGroupPresentation(
            GroupPresentation domain,
            GroupPresentation codomain,
            std::vector<GroupExpression> map ) :
        domain_(std::move(domain)),
        codomain_(std::move(codomain)),
        map_(std::move(map)) {
}

inline HomGroupPresentation::HomGroupPresentation(
            GroupPresentation domain,
            GroupPresentation codomain,
            std::vector<GroupExpression> map,
            std::vector<GroupExpression> inv ) :
        domain_(std::move(domain)),
        codomain_(std::move(codomain)),
        map_(std::move(map)),
        inv_(std::move(inv)) {
}

inline void HomGroupPresentation::swap(HomGroupPresentation& other) noexcept {
    domain_.swap(other.domain_);
    codomain_.swap(other.codomain_);
    map_.swap(other.map_);
    inv_.swap(other.inv_);
}

inline bool HomGroupPresentation::operator == (
        const HomGroupPresentation& other) const {
    return map_ == other.map_;
}

inline const GroupPresentation& HomGroupPresentation::domain() const {
    return domain_;
}

inline const GroupPresentation& HomGroupPresentation::codomain() const {
    return codomain_;
}

inline bool HomGroupPresentation::knowsInverse() const {
    return inv_.has_value();
}

inline GroupExpression HomGroupPresentation::evaluate(unsigned long i) const {
    return map_[i];
}

inline GroupExpression HomGroupPresentation::evaluate(GroupExpression arg)
        const {
    arg.substitute(map_);
    return arg;
}

inline GroupExpression HomGroupPresentation::invEvaluate(GroupExpression arg)
        const {
    arg.substitute(*inv_);
    return arg;
}

inline GroupExpression HomGroupPresentation::invEvaluate(unsigned long i)
        const {
    return (*inv_)[i];
}

inline bool HomGroupPresentation::intelligentSimplify() {
    return simplify();
}

inline bool HomGroupPresentation::intelligentNielsen() {
    return nielsen();
}

inline void swap(HomGroupPresentation& a, HomGroupPresentation& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

