
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

/*! \file algebra/nhomgrouppresentation.h
 *  \brief Deals with finite presentations of groups.
 */

#ifndef __NHOMGROUPPRESENTATION_H
#ifndef __DOXYGEN
#define __NHOMGROUPPRESENTATION_H
#endif

#include <vector>
#include "regina-core.h"
#include "output.h"
#include <boost/noncopyable.hpp>

namespace regina {

class NGroupPresentation;

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Represents a homomorphism between groups which are described via finite
 * presentations.
 *
 * Some homomorphisms may be <i>declared isomorphisms</i>.  This means
 * that the user (or some other function in Regina) has proven that this
 * is an isomorphism and has explicitly provided the inverse map.
 * To provide the inverse map, you should call the four-argument constructor
 * NHomGroupPresentation(const NGroupPresentation&, const NGroupPresentation&,
 * const std::vector<NGroupExpression>&, const std::vector<NGroupExpression>&).
 *
 * You can test for a declared isomorphism by calling knowsInverse().  Even
 * if a homomorphism is not a declared isomorphism, it might still be an
 * isomorphism; this just means that no inverse map was explicitly provided.
 *
 * \apinotfinal
 *
 * \todo Add a routine to attempt to verify validity of homomorphism.
 */
class REGINA_API NHomGroupPresentation :
        public Output<NHomGroupPresentation>,
        public boost::noncopyable {
    private:
        NGroupPresentation* domain_;
            /**< The domain of the homomorphism. */
        NGroupPresentation* range_;
            /**< The range of the homomorphism. */
        std::vector<NGroupExpression*> map_;
            /**< A map whose ith element is the image in the range
                 of the ith generator from the domain. */
        std::vector<NGroupExpression*>* inv_;
            /**< Null unless this is a declared isomorphism, in which case
                 this will be a map whose ith element is the image in the
                 domain of the ith generator from the range. */

    public:
        /**
         * Creates a new homomorphism from the given data.
         *
         * \ifacespython Not present.
         *
         * @param domain the domain of the homomorphism.
         * @param range the range of the homomorphism.
         * @param map a vector of length \a g, where \a g is the number
         * of generators of the domain, and where this homomorphism
         * sends the <i>i</i>th generator of the domain to the
         * element <tt>map[i]</tt> of the range.
         *
         * \ifacespython Not present.
         */
        NHomGroupPresentation(const NGroupPresentation &domain,
                const NGroupPresentation &range,
                const std::vector<NGroupExpression> &map);

        /**
         * Creates a declared isomorphism from the given data.
         * Here you must provide both a map from the domain to range,
         * and the inverse map from the range to domain.
         *
         * \pre The argument \a inv is indeed the inverse of \a map.
         *
         * \ifacespython Not present.
         *
         * @param domain the domain of the homomorphism.
         * @param range the range of the homomorphism.
         * @param map a vector of length \a g, where \a g is the number
         * of generators of the domain, and where this homomorphism
         * sends the <i>i</i>th generator of the domain to the
         * element <tt>map[i]</tt> of the range.
         * @param inv a vector of length \a k where \a k is the number
         * of generators of the range, and where the inverse homomorphism
         * sends the <i>i</i>th generator of the range to the
         * element <tt>inv[i]</tt> of the domain.
         */
        NHomGroupPresentation(const NGroupPresentation &domain,
                const NGroupPresentation &range,
                const std::vector<NGroupExpression> &map,
                const std::vector<NGroupExpression> &inv);

        /**
         * Creates a new identity homomorphism for the given group.
         *
         * This will be a declared isomorphism (see the
         * NHomGroupPresentation class notes for details).
         *
         * @param groupForIdentity both the range and domain of the
         * new identity homomorphism.
         */
        NHomGroupPresentation(const NGroupPresentation& groupForIdentity);

        /**
         * Creates a clone of the given group presentation.
         *
         * @param cloneMe the presentation to clone.
         */
        NHomGroupPresentation(const NHomGroupPresentation& cloneMe);

        /**
         * Destroys the group homomorphism.
         */
        ~NHomGroupPresentation();

        /**
         * The domain of the map.
         *
         * @return a reference to the domain.
         */
        const NGroupPresentation& domain() const;
        /**
         * The range of the map.
         *
         * @return a reference to the range.
         */
        const NGroupPresentation& range() const;

        /**
         * Returns whether or not this is a declared isomorphism.
         *
         * A <i>declared isomorphism</i> is a isomorphism for which the
         * user has explicitly provided the inverse map.  See the
         * NHomGroupPresentation class notes for details.
         *
         * @return \c true if and only if this is a declared
         * isomorphism, i.e, the inverse map was explicitly provided.
         */
        bool knowsInverse() const;

        /**
         * Evaluate the homomorphism at an element of the domain.
         *
         * @param arg an element of the domain.
         * @return the image of this element in the range.
         */
        NGroupExpression evaluate(const NGroupExpression &arg) const;

        /**
         * Evaluate the homomorphism at a generator of the domain.
         *
         * @param i the index of a generator in the domain.
         * @return the image of the <i>i</i>th generator in the range.
         */
        NGroupExpression evaluate(unsigned long i) const;

        /**
         * Evaluate the isomorphisms's inverse at an element of the range.
         *
         * \pre This homomorphism is in fact a declared isomorphism.
         * See the NHomGroupPresentation class notes for details
         * on what this means.
         *
         * @param arg an element of the range.
         * @return the image of this element in the domain.
         */
        NGroupExpression invEvaluate(const NGroupExpression &arg) const;
        /**
         * Evaluate the isomorphism at a generator of the range.
         *
         * \pre This homomorphism is in fact a declared isomorphism.
         * See the NHomGroupPresentation class notes for details
         * on what this means.
         *
         * @param i the index of a generator in the range.
         * @return the image of this generator in the domain.
         */
        NGroupExpression invEvaluate(unsigned long i) const;

        /**
         * Simultaneously simplifies:
         *
         * - the presentation of the domain;
         * - the presentation of the range;
         * - the description of the map.
         *
         * Uses the underlying NGroupPresentation::intelligentSimplify().
         * See that routine for details.
         *
         * @return \c true if the presentations or map have changed.
         */
        bool intelligentSimplify();

        /**
         * Simplifies the domain and range using only Nielsen moves, keeping
         * track of the resulting map in the progress.
         *
         * @return \c true if and only if either presentation was changed.
         */
        bool intelligentNielsen();

        /**
         * Simplifies the domain and range using only small cancellation
         * theory.
         *
         * @return \c true if and only if either presentation was changed.
         */
        bool smallCancellation();

        /**
         * Composes this homomorphism with the given input homomorphism.
         *
         * Evaluating the composition on some group element \a x is the
         * same as evaluating <tt>this(input(x))</tt>.
         * In other words, in this composition, \a input is evaluated first
         * and then the output of that is evaluated by this homomorphism.
         *
         * If both of the given homomorphisms are declared isomorphisms,
         * then the return value will be a declared isomoprhism also.
         *
         * \pre the range of \a input must be the same as the domain of this
         * homomorphism.
         *
         * @param input the homomorphism to compose with this.
         * @return the composition of both homomorphisms.
         */
        std::unique_ptr<NHomGroupPresentation> composeWith(
            const NHomGroupPresentation& input) const;

        /**
         * Inverts the homomorphism.
         *
         * This is only possible if the homomorphism is in fact a
         * declared isomorphism (which means that the inverse map is
         * already stored internally).  See the NHomGroupPresentation
         * class notes for further details on declared isomorphisms.
         *
         * If this is not a declared isomorphism then this routine
         * will do nothing and simply return \c false.
         *
         * This operation is (very) fast constant time.
         *
         * @return \c true if and only if the inversion operation was
         * successful (i.e., if this is a declared isomorphism).
         */
        bool invert();

        /**
         * Verifies the map is a valid homomorphism.  Specifically, this
         * routine runs through all the relators in the domain, evaluates
         * the homomorphism on the relators and checks that they simplify
         * to 1 in the range.
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
         * @return \c true if Regina is able to verify that this is a
         * homomorphism, or \c false if the result is inconclusive.
         */
        bool verify() const;

        /**
         * Attempts to verify that a declared isomorphism is, indeed,
         * an isomorphism.
         *
         * This routine works by attempting to verify that
         * <tt>f^-1(f(x))x^-1</tt> simplifes to 1 for all generators \a x
         * in the domain, and likewise for the range.
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
         * See the NHomGroupPresentation class notes for details
         * on what this means.
         *
         * @return \c true if it is verified that this is an
         * isomorphism, or \c false if the result is inconclusive.
         */
        bool verifyIsomorphism() const;

        /**
         * Computes the induced map on the abelianizations of the domain
         * and range.
         *
         * @return the induced map on the abelianizations.
         */
        std::unique_ptr< NHomMarkedAbelianGroup > markedAbelianisation() const;

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

inline NHomGroupPresentation::NHomGroupPresentation(
            const NGroupPresentation &domain,
            const NGroupPresentation &range,
            const std::vector<NGroupExpression> &map ) :
        domain_(new NGroupPresentation(domain)),
        range_(new NGroupPresentation(range)),
        inv_(0) {
    map_.resize(map.size());
    for (unsigned long i=0; i<map_.size(); i++)
        map_[i] = new NGroupExpression(map[i]);
}

inline NHomGroupPresentation::NHomGroupPresentation(
            const NGroupPresentation &domain,
            const NGroupPresentation &range,
            const std::vector<NGroupExpression> &map,
            const std::vector<NGroupExpression> &inv ) :
        domain_(new NGroupPresentation(domain)),
        range_(new NGroupPresentation(range)),
        inv_(new std::vector<NGroupExpression*>) {
    map_.resize(map.size());
    inv_->resize(inv.size());
    for (unsigned long i=0; i<map_.size(); i++)
        map_[i] = new NGroupExpression(map[i]);
    for (unsigned long i=0; i<inv_->size(); i++)
        (*inv_)[i] = new NGroupExpression(inv[i]);
}

inline NHomGroupPresentation::NHomGroupPresentation(
        const NHomGroupPresentation& cloneMe) :
        domain_(new NGroupPresentation(*cloneMe.domain_)),
        range_(new NGroupPresentation(*cloneMe.range_)) {
    map_.resize(cloneMe.map_.size());
    for (unsigned long i=0; i<map_.size(); i++)
        map_[i] = new NGroupExpression(*(cloneMe.map_[i]));
    if (cloneMe.inv_) {
        inv_ = new std::vector<NGroupExpression*>;
        inv_->resize(cloneMe.inv_->size());
        for (unsigned long i=0; i<inv_->size(); i++)
            (*inv_)[i] = new NGroupExpression(*((*cloneMe.inv_)[i]));
    } else
        inv_ = 0;
}

inline NHomGroupPresentation::~NHomGroupPresentation() {
    for (unsigned long i=0; i<map_.size(); i++)
        delete map_[i];
    if (inv_) {
        for (unsigned long i=0; i<inv_->size(); i++)
            delete (*inv_)[i];
        delete inv_;
    }
    delete domain_; delete range_;
}

inline const NGroupPresentation& NHomGroupPresentation::domain() const {
    return *domain_;
}

inline const NGroupPresentation& NHomGroupPresentation::range() const {
    return *range_;
}

inline bool NHomGroupPresentation::knowsInverse() const {
    return inv_;
}

inline NGroupExpression NHomGroupPresentation::evaluate(unsigned long i) const {
    return *(map_[i]);
}

inline NGroupExpression NHomGroupPresentation::invEvaluate(unsigned long i)
        const {
    return *((*inv_)[i]);
}

} // namespace regina

#endif

