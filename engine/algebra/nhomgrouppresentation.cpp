
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

#include <iterator>
#include <sstream>
#include "algebra/ngrouppresentation.h"
#include "algebra/nhomgrouppresentation.h"
#include "maths/numbertheory.h"
#include "utilities/boostutils.h"
#include "utilities/stlutils.h"

namespace regina {

NGroupExpression NHomGroupPresentation::evaluate(
                        const NGroupExpression &arg) const
 { // evaluate at arg
   NGroupExpression retval(arg); // Use substitute to build evaluation.
   // increment the generator num in retval to be larger than 
   //  the largest generator used in range, N.
   unsigned long N( range_.getNumberOfGenerators() );
   for (unsigned long i=0; i<retval.getNumberOfTerms(); i++)
    {     retval.getTerm(i).generator += N; }
   // then we apply substitute appropriately.
   for (unsigned long i=0; i<map_.size(); i++)
    { // map_ states where domain i-th generator is sent to in range, 
      //  so we replace gen N+i by map_[i]
      retval.substitute( N+i, *map_[i] );
    }
 
   return retval; 
 }

std::auto_ptr<NHomGroupPresentation> NHomGroupPresentation::operator*(
    const NHomGroupPresentation &arg) const {
    std::vector< NGroupExpression > vals( domain_.getNumberOfGenerators() );
    for (unsigned long i=0; i<vals.size(); i++) {
        NGroupExpression gi; gi.addTermLast( i, 1 );
        vals[i].addTermsLast(evaluate(arg.evaluate(gi)));
    }
    return std::auto_ptr<NHomGroupPresentation>(
        new NHomGroupPresentation( domain_, arg.range_, vals ));
}

std::string NHomGroupPresentation::toString() const
{
std::string retval; 
retval.append("Domain "); 
retval.append(domain_.toString());
retval.append(" map[");
 for (unsigned long i=0; i<domain_.getNumberOfGenerators(); i++)
  { if (i!=0) retval.append(", "); retval.append("g"); 
    std::stringstream num; num<<i; retval.append( num.str() );
    retval.append(" --> "); retval.append(map_[i]->toString()); 
  }
 retval.append("] Range ");
 retval.append(range_.toString());
return retval;
}


void NHomGroupPresentation::writeTextShort(std::ostream& out) const
{
 std::cout<<"map[";
 for (unsigned long i=0; i<domain_.getNumberOfGenerators(); i++)
  { if (i!=0) out<<", "; out<<"g"<<i<<" --> "; 
    std::cout.flush(); map_[i]->writeTextShort( out ); 
  }
  out<<"]";
}

void NHomGroupPresentation::writeTextLong(std::ostream& out) const
{
 out<<"Domain "<<domain_.toString()<<" "; 
 writeTextShort(out);
 out<<" "<<range_.toString()<<" Range.";
}

// return true if and only if a modification to either domain, 
//  range or the presentation is made.
bool NHomGroupPresentation::intelligentSimplify()
{
 bool didSomething(false);

 // step 1: simplify presentation of range. 
 //         the strategy is to completely mimic 
 // NGroupPresentation::intelligentSimplify(), and change the map accordingly
 regina::NHomGroupPresentation* rangeMap(NULL);
 didSomething = range_.intelligentSimplify(rangeMap);

 // step 2: simplify presentation of domain. 
 //         the strategy is to completely mimic 
 // NGroupPresentation::intelligentSimplify(), and change the map accordingly
 regina::NHomGroupPresentation* domainMap(NULL);
 bool temp(domain_.intelligentSimplify(domainMap));
 didSomething = didSomething || temp; 
  // doesn't call intelligentSimplify if didSomething==true?

 // step 3: find a hacky inverse to domainMap
 //         by the way domainMap is constructed we know each gi 
 //         comes from some gk, so look them up. 

 std::vector< unsigned long > invDomainMap( domain_.getNumberOfGenerators() );
 for (unsigned long i=0; i<domainMap->getDomain().getNumberOfGenerators(); i++)
  {
   // if map_[i] == "gk" then invDomainMap[k] = gi. 
   if (domainMap->evaluate(i).getNumberOfTerms()==1) 
    if (domainMap->evaluate(i).getExponent(0)==1)
     invDomainMap[ domainMap->evaluate(i).getGenerator(0) ] = i;
  } 

 // step 4: compute rangeMap*(*this)*domainMap.inverse()
 //         and replace "map" appropriately. 
 std::vector< NGroupExpression > newMap( domain_.getNumberOfGenerators() );
 for (unsigned long i=0; i<newMap.size(); i++)
  newMap[i].addTermsLast( rangeMap->evaluate( *map_[ invDomainMap[i] ] ) );

 for (unsigned long i=0; i<map_.size(); i++) delete map_[i];
 map_.resize( newMap.size() );
 for (unsigned long i=0; i<map_.size(); i++) map_[i] = 
   new NGroupExpression(newMap[i]);

 // step 5: done, clean-up
 delete rangeMap;
 delete domainMap;
 return didSomething;
}


} // namespace regina

