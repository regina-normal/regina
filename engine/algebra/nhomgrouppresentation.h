
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file algebra/nhomgrouppresentation.h
 *  \brief Deals with finite presentations of groups.
 */

#ifndef __NHOMGROUPPRESENTATION_H
#ifndef __DOXYGEN
#define __NHOMGROUPPRESENTATION_H
#endif

#include <vector>
#include "regina-core.h"
#include "shareableobject.h"

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
 * \testpart
 *
 * \todo add routine to attempt to verify validity of homomorphism.
 */
class REGINA_API NHomGroupPresentation : public ShareableObject {
    private:
        NGroupPresentation* domain_;
            /**< The domain of the homomorphism. */
        NGroupPresentation* range_;
            /**< The range of the homomorphism. */
        std::vector<NGroupExpression*> map_;
            /**< A map whose ith element is the image in the range
                 of the ith generator from the domain. */
        std::vector<NGroupExpression*> map2_;
            /**< A map whose ith element is the image in the domain
                 of the ith generator from the range. Allocated only
                 if user claims map invertible. */
   

    public:
        /**
         * Creates a new homomorphism from the given data.
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
         * Creates a new homomorphism from the given data.
         * If called using this constructor, one is declaring a map both
         * from the domain to range, and a map from the range to domain. 
         * Usually one would want this second map to be the inverse of
         * the first.  
         *
         * @param domain the domain of the homomorphism.
         * @param range the range of the homomorphism.
         * @param map a vector of length \a g, where \a g is the number
         *  of generators of the domain, and where this homomorphism
         *  sends the <i>i</i>th generator of the domain to the
         *  element <tt>map[i]</tt> of the range.
         * @param map2 a vector of length \a k where \a k is the number
         *  of generators of the range. <tt>map2[i]</tt> is a word in
         *  the generators of the domain.
         *
         * \ifacespython Not present.
         */
        NHomGroupPresentation(const NGroupPresentation &domain,
                const NGroupPresentation &range,
                const std::vector<NGroupExpression> &map,
                const std::vector<NGroupExpression> &map2);


        /**
         * Creates a new identity homomorphism for the given group.
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
        const NGroupPresentation& getDomain() const;
        /**
         * The range of the map.
         *
         * @return a reference to the range.
         */
        const NGroupPresentation& getRange() const;


        /**
         * Evaluate the homomorphism at an element.
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
         * Evaluate the isomorphisms's inverse at an element.  
         *
         * \warning this homomorphism must be known to be an isomorphism for
         *  this procedure to work. 
         *
         * @param arg an element of the range.
         * @return the image of this element in the domain.
         */
        NGroupExpression invEvaluate(const NGroupExpression &arg) const;
        /**
         * Evaluate the isomorphism at a generator of the range.
         *
         * \warning this homomorphism must be known to be an isomorphism for
         *  this procedure to work. 
         *
         * @param i the index of a generator in the domain.
         * @return the image of the <i>i</i>th generator in the range.
         */        
        NGroupExpression invEvaluate(unsigned long i) const;

        /**
         * Simultaneously simplifies:
         *
         * - the presentation of the domain;
         * - the presentation of the range;
         * - the description of the map.
         *
         * Uses the underlying NGroupPresentation::intelligentSimplify.
         * See those routines for details.
         *
         * @return true if the presentations or map have changed.
         */
        bool intelligentSimplify();

        /**
         * Simplifies the domain and range using only nielsen moves, keeping
         * track of the resulting map in the progress.
         *
         * @return true if and only if the presentations have changed. 
         */
        bool intelligentNielsen();

        /**
         *  Simplifies the domain and range using only small cancellation
         * theory. 
         *
         * @return true if and only if the presentations have changed. 
         */
        bool smallCancellation();

        /**
         * Composes the current homomorphism with the input homomorphism.  
         * 
         * @pre the range of the input must equal the domain of this. 
         *
         * @return an auto_ptr<NHomGroupPresentation> to the composition. 
         *  evaluating the return on an element is the same as evaluating
         *  this out the evalution of input. 
         */
        std::auto_ptr<NHomGroupPresentation> composeWith(
            const NHomGroupPresentation& input) const;

        /**
         *  Computes the induced map on the abelianizations.
         *
         * @return the induced map on the abelianizations.
         */
        std::auto_ptr< NHomMarkedAbelianGroup > markedAbelianisation() const;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/*@}*/

inline NHomGroupPresentation::NHomGroupPresentation(
            const NGroupPresentation &domain,
            const NGroupPresentation &range,
            const std::vector<NGroupExpression> &map ) :
        domain_(new NGroupPresentation(domain)), 
        range_(new NGroupPresentation(range)) {
    map_.resize(map.size());
    for (unsigned long i=0; i<map_.size(); i++)
        map_[i] = new NGroupExpression(map[i]);
}

inline NHomGroupPresentation::NHomGroupPresentation(
            const NGroupPresentation &domain,
            const NGroupPresentation &range,
            const std::vector<NGroupExpression> &map,
            const std::vector<NGroupExpression> &map2 ) :
        domain_(new NGroupPresentation(domain)), 
        range_(new NGroupPresentation(range)) {
    map_.resize(map.size());
    map2_.resize(map2.size());
    for (unsigned long i=0; i<map_.size(); i++)
        map_[i] = new NGroupExpression(map[i]);
    for (unsigned long i=0; i<map2_.size(); i++)
        map2_[i] = new NGroupExpression(map2[i]);
}

inline NHomGroupPresentation::NHomGroupPresentation(
        const NHomGroupPresentation& cloneMe) :
        domain_(new NGroupPresentation(*cloneMe.domain_)),
        range_(new NGroupPresentation(*cloneMe.range_)) {
    map_.resize(cloneMe.map_.size());
    map2_.resize(cloneMe.map2_.size());
    for (unsigned long i=0; i<map_.size(); i++)
        map_[i] = new NGroupExpression(*(cloneMe.map_[i]));
    for (unsigned long i=0; i<map2_.size(); i++)
        map2_[i] = new NGroupExpression(*(cloneMe.map2_[i]));
}

inline NHomGroupPresentation::~NHomGroupPresentation() {
    for (unsigned long i=0; i<map_.size(); i++)
        delete map_[i];
    for (unsigned long i=0; i<map2_.size(); i++)
        delete map2_[i];
    delete domain_; delete range_;
}

inline const NGroupPresentation& NHomGroupPresentation::getDomain() const {
    return *domain_;
}

inline const NGroupPresentation& NHomGroupPresentation::getRange() const {
    return *range_;
}

inline NGroupExpression NHomGroupPresentation::evaluate(unsigned long i) 
 const { return (*(map_[i])); }

inline NGroupExpression NHomGroupPresentation::invEvaluate(unsigned long i) 
 const { return (*(map2_[i])); }


} // namespace regina

#endif

