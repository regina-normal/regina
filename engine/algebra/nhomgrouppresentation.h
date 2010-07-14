
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file algebra/nhomgrouppresentation.h
 *  \brief Deals with finite presentations of groups.
 */

#ifndef __NHOMGROUPPRESENTATION_H
#ifndef __DOXYGEN
#define __NHOMGROUPPRESENTATION_H
#endif

#include <algorithm>
#include <list>
#include <vector>
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
 * Represents a homomorphism between groups which are described via finite presentations.
 *
 * \testpart
 *
 * \todo add routine to attempt to verify validity of homomorphism. Will need to add a routine to
 *       NGroupPresentation to attempt to solve word problem.
 */
class NHomGroupPresentation : public ShareableObject {
    private:
	NGroupPresentation domain, range; 
	std::vector< NGroupExpression* > map;

    public:
        /**
         * Creates a new presentation with no generators and no
         * relations. domain is the domain presentation, range the range
         * presentation, map the list that says the i-th generator of domain 
         * is sent to element map[i] of range.
         */
        NHomGroupPresentation(const NGroupPresentation &Domain, const NGroupPresentation &Range, 
			   const std::vector< NGroupExpression > &Map );

        /**
         * Creates a clone of the given group presentation.
         *
         * @param cloneMe the presentation to clone.
         */
        NHomGroupPresentation(const NHomGroupPresentation& cloneMe);

        /**
         * Destroys the group presentation.
         * All relations that are stored will be deallocated.
         */
        ~NHomGroupPresentation();

        /**
         *  The domain of the map.
         */
        const NGroupPresentation& getDomain() const;
        /**
         *  The range of the map.
         */
        const NGroupPresentation& getRange() const;


        /**
	 * Evaluate the homomorphism at an element.
	 */
	NGroupExpression evaluate(const NGroupExpression &arg) const;
        /**
         * Evaluate the homomorphism on the generator gi
         */
	NGroupExpression evaluate(unsigned long i) const;

        /**
	 * Compose homomorphisms. 
	 */
        NHomGroupPresentation operator*(const NHomGroupPresentation &arg) const;

        /**
         * Attempts to simplify simultaneously: 1) the presentation of the domain
         *                                      2) the presentation of the range
         *                                      3) the description of the map
	 * Uses Dehn's algorithm / "small cancellation theory". 
         *  @return true if the presentations or map have changed.
         */
        void dehnAlgorithm();

        /**
         * Writes the map as a string. 
         */
        std::string stringOutput() const;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/*@}*/
inline NHomGroupPresentation::NHomGroupPresentation(const NGroupPresentation &Domain, const NGroupPresentation &Range, 
			   const std::vector< NGroupExpression > &Map ) : ShareableObject(), domain(Domain), 
			   range(Range), map(0) 
{ map.resize(Map.size()); for (unsigned long i=0; i<map.size(); i++) map[i] = new NGroupExpression(Map[i]); }

inline NHomGroupPresentation::NHomGroupPresentation(const NHomGroupPresentation& cloneMe) : ShareableObject(), 
				domain(cloneMe.domain), range(cloneMe.range), map(0) 
{ map.resize(cloneMe.map.size()); for (unsigned long i=0; i<map.size(); i++) map[i] = new NGroupExpression(*(cloneMe.map[i])); }

inline NHomGroupPresentation::~NHomGroupPresentation() 
{ for (unsigned long i=0; i<map.size(); i++) delete map[i]; map.resize(0); }

inline const NGroupPresentation& NHomGroupPresentation::getDomain() const
{ return domain; }

inline const NGroupPresentation& NHomGroupPresentation::getRange() const
{ return range; }

inline NGroupExpression NHomGroupPresentation::evaluate(unsigned long i) const
{ return (*(map[i])); }


} // namespace regina

#endif

