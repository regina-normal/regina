
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

NGroupExpression NHomGroupPresentation::operator[](const NGroupExpression &arg)
 { // evaluate at arg
   NGroupExpression retval(arg); // start with arg, then we use substitute to evaluate.
   // lets increment the generator numbers in retval to be larger than the largest generator used in range, N.
   unsigned long N( range.getNumberOfGenerators() );
   for (unsigned long i=0; i<retval.getNumberOfTerms(); i++)
    {     retval.getTerm(i).generator += N; }
   // then we apply substitute appropriately.
   for (unsigned long i=0; i<map.size(); i++)
    { // map states where domain i-th generator is sent to in range, so we replace gen N+i by map[i]
      retval.substitute( N+i, map[i] );
    }
 
   return retval; 
 }

void NHomGroupPresentation::writeTextShort(std::ostream& out) const
{
 std::cout<<"map[";
 for (unsigned long i=0; i<domain.getNumberOfGenerators(); i++)
  { if (i!=0) out<<", "; out<<"g"<<i<<" --> "; std::cout.flush(); map[i].writeTextShort( out );  }
 out<<"]";
}

void NHomGroupPresentation::writeTextLong(std::ostream& out) const
{
 out<<"Domain "<<domain.stringPresentation()<<" "; 
 writeTextShort(out);
 out<<" "<<range.stringPresentation()<<" Range.";
}

// return true if and only if a modification to either domain, range or the presentation is made.
bool NHomGroupPresentation::intelligentSimplify()
{
bool changed(false); // default

// step 1: simplify presentation of range. 
//         the strategy is to completely mimic NGroupPresentation::intelligentSimplify(), and change the map accordingly

// step 2: simplify presentation of domain. 
//         the strategy is to completely mimic NGroupPresentation::intelligentSimplify(), and change the map accordingly

// step 3: simplify map.
//         try something?? hmm... need to think about this some more.

return changed;
}


} // namespace regina

