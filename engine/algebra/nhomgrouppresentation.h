
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

#include <algorithm>
#include <list>
#include <memory>
#include <vector>
#include "regina-core.h"
#include "utilities/memutils.h"
#include "algebra/ngrouppresentation.h"
#include "shareableobject.h"

namespace regina {

class NFile;
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
        NGroupPresentation domain_;
            /**< The domain of the homomorphism. */
        NGroupPresentation range_;
            /**< The range of the homomorphism. */
        std::vector<NGroupExpression*> map_;
            /**< A map whose ith element is the image of the ith
                 generator from the domain. */

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
         */
        NHomGroupPresentation(const NGroupPresentation &domain,
                const NGroupPresentation &range,
                const std::vector<NGroupExpression> &map);

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
         * @param the image of the <i>i</i>th generator in the range.
         */
        NGroupExpression evaluate(unsigned long i) const;

        /**
         * Simultaneously simplifies:
         *
         * - the presentation of the domain;
         * - the presentation of the range;
         * - the description of the map.
         *
         * Uses Dehn's algorithm / "small cancellation theory".
         *
         * @return true if the presentations or map have changed.
         */
        bool intelligentSimplify();

        /**
         * Writes the map as a string.
         *
         * @return a string representation of this map.
         */
        std::string toString() const;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/*@}*/

inline NHomGroupPresentation::NHomGroupPresentation(
            const NGroupPresentation &domain,
            const NGroupPresentation &range,
            const std::vector<NGroupExpression> &map ) :
        domain_(domain), range_(range) {
    map_.resize(map.size());
    for (unsigned long i=0; i<map_.size(); i++)
        map_[i] = new NGroupExpression(map[i]);
}

inline NHomGroupPresentation::NHomGroupPresentation(
        const NHomGroupPresentation& cloneMe) :
        domain_(cloneMe.domain_),
        range_(cloneMe.range_) {
    map_.resize(cloneMe.map_.size());
    for (unsigned long i=0; i<map_.size(); i++)
        map_[i] = new NGroupExpression(*(cloneMe.map_[i]));
}

inline NHomGroupPresentation::~NHomGroupPresentation() {
    for (unsigned long i=0; i<map_.size(); i++)
        delete map_[i];
}

inline const NGroupPresentation& NHomGroupPresentation::getDomain() const {
    return domain_;
}

inline const NGroupPresentation& NHomGroupPresentation::getRange() const {
    return range_;
}

inline NGroupExpression NHomGroupPresentation::evaluate(unsigned long i) const {
    return (*(map_[i]));
}

} // namespace regina

#endif

