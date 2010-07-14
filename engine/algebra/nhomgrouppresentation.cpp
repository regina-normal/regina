
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

#include <iterator>
#include <sstream>
#include "algebra/ngrouppresentation.h"
#include "algebra/nhomgrouppresentation.h"
#include "file/nfile.h"
#include "maths/numbertheory.h"
#include "utilities/boostutils.h"
#include "utilities/hashmap.h"
#include "utilities/hashutils.h"
#include "utilities/stlutils.h"

namespace regina {

NGroupExpression NHomGroupPresentation::evaluate(const NGroupExpression &arg) const
 { // evaluate at arg
   NGroupExpression retval(arg); // start with arg, then we use substitute to evaluate.
   // lets increment the generator numbers in retval to be larger than the largest generator used in range, N.
   unsigned long N( range.getNumberOfGenerators() );
   for (unsigned long i=0; i<retval.getNumberOfTerms(); i++)
    {     retval.getTerm(i).generator += N; }
   // then we apply substitute appropriately.
   for (unsigned long i=0; i<map.size(); i++)
    { // map states where domain i-th generator is sent to in range, so we replace gen N+i by map[i]
      retval.substitute( N+i, *map[i] );
    }
 
   return retval; 
 }

NHomGroupPresentation NHomGroupPresentation::operator*(const NHomGroupPresentation &arg) const
{
std::vector< NGroupExpression > vals( domain.getNumberOfGenerators() );
for (unsigned long i=0; i<vals.size(); i++)
 {
 NGroupExpression gi; gi.addTermLast( i, 1 );
 vals[i].addTermsLast(evaluate(arg.evaluate(gi)));
 }
return NHomGroupPresentation( domain, arg.range, vals );
}

std::string NHomGroupPresentation::stringOutput() const
{
std::string retval; 
retval.append("Domain "); 
retval.append(domain.stringOutput());
retval.append(" map[");
 for (unsigned long i=0; i<domain.getNumberOfGenerators(); i++)
  { if (i!=0) retval.append(", "); retval.append("g"); 
    std::stringstream num; num<<i; retval.append( num.str() );
    retval.append(" --> "); retval.append(map[i]->stringOutput()); 
  }
 retval.append("] Range ");
 retval.append(range.stringOutput());
return retval;
}


void NHomGroupPresentation::writeTextShort(std::ostream& out) const
{
 std::cout<<"map[";
 for (unsigned long i=0; i<domain.getNumberOfGenerators(); i++)
  { if (i!=0) out<<", "; out<<"g"<<i<<" --> "; std::cout.flush(); map[i]->writeTextShort( out );  }
 out<<"]";
}

void NHomGroupPresentation::writeTextLong(std::ostream& out) const
{
 out<<"Domain "<<domain.stringOutput()<<" "; 
 writeTextShort(out);
 out<<" "<<range.stringOutput()<<" Range.";
}

// return true if and only if a modification to either domain, range or the presentation is made.
bool NHomGroupPresentation::intelligentSimplify()
{
 bool didSomething(false);
 // step 1: simplify presentation of range. 
 //         the strategy is to completely mimic NGroupPresentation::intelligentSimplify(), and change the map accordingly
 regina::NHomGroupPresentation* rangeMap(NULL);
 didSomething = range.intelligentSimplify(rangeMap);
 // step 2: simplify presentation of domain. 
 //         the strategy is to completely mimic NGroupPresentation::intelligentSimplify(), and change the map accordingly
 regina::NHomGroupPresentation* domainMap(NULL);
 didSomething = didSomething || domain.intelligentSimplify(domainMap);

 // step 3: find a hacky inverse to domainMap
 //         by the way domainMap is constructed we know each gi comes from some gk, so look them up. 
 std::vector< unsigned long > invDomainMap( domain.getNumberOfGenerators() );
 for (unsigned long i=0; i<domainMap->getDomain().getNumberOfGenerators(); i++)
  {
   // if map[i] == "gk" then invDomainMap[k] = gi. 
   if (domainMap->evaluate(i).getNumberOfTerms()==1) if (domainMap->evaluate(i).getExponent(0)==1)
     invDomainMap[ domainMap->evaluate(i).getGenerator(0) ] = i;
  } 

 // step 4: compute rangeMap*(*this)*domainMap.inverse()
 //         and replace "map" appropriately. 
 std::vector< NGroupExpression > newMap( domain.getNumberOfGenerators() );
 for (unsigned long i=0; i<newMap.size(); i++)
  newMap[i].addTermsLast( rangeMap->evaluate( *map[ invDomainMap[i] ] ) );

 for (unsigned long i=0; i<map.size(); i++) delete map[i];
 map.resize( newMap.size() );
 for (unsigned long i=0; i<map.size(); i++) map[i] = new NGroupExpression(newMap[i]);

 // step 5: done, clean-up
 delete rangeMap;
 delete domainMap;
 return didSomething;
}


} // namespace regina

