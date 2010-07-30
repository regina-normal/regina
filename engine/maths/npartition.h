
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

/*! \file maths/npartition.h
 *  \brief Provides utility class for iterating through partitions of subsets
 *   of a set.
 */

#ifndef __NPartition_H
#ifndef __DOXYGEN
#define __NPartition_H
#endif

#include <sstream>

#include "utilities/nbitmask.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 *  Class for dealing with partitions of sets of integers {0,1,...,n-1}. 
 * You can use it to iterate through all subsets of this set, or all
 * subsets of a fixed size. 
 */
class NPartition {
        NBitmask part;
        unsigned long setSize;
        unsigned long subSetSize;
        bool beforeStart, afterEnd; 
public:
        /**
         * Constructor. Ambient set size will be SetSize, subsets of cardinality
         * SubSetSize to be iterated through. Setting SubSetSize to 0 is interpreted
         * as a request for all subsets. 
         * Initial partition to be {0,1,...,SubSetSize-1}
         */
        NPartition(unsigned long SetSize, unsigned long SubSetSize);
        /**
         * Copy constructor. 
         */        
        NPartition(const NPartition& copyMe);
        /**
         * Increment operator
         */
        NPartition operator++();
        /**
         * Has iteration passed the end of the list?
         */
        bool atEnd();
        /**
         * Are we before the start of the list?
         */
        bool atStart();
        /**
         * Request the current partition.
         */
        const NBitmask& partition() const;        
        /**
         * Text output in form of a NBitMask
         */
        std::string textString() const;
};


inline NPartition::NPartition(unsigned long SetSize, unsigned long SubSetSize) : part(SetSize), 
  setSize(SetSize), subSetSize(SubSetSize), beforeStart(false), afterEnd(false)
{  for (unsigned long i=0; i<subSetSize; i++) part.set(i, true); }

inline NPartition::NPartition(const NPartition& copyMe) : part(copyMe.part), setSize(copyMe.setSize), 
 subSetSize(copyMe.subSetSize), beforeStart(copyMe.beforeStart), afterEnd(copyMe.afterEnd) {}

inline NPartition NPartition::operator++()
{
 //   (a) find first one from RHS with a zero to the right. Move it right. 
 //       if not the *last* entry, append the appropriate number of consecutive 1's.
 if (afterEnd) return (*this);
 unsigned long numOnesWithoutZerosToRight=0; // count this from right to left. 
 signed long indx;
 for (indx=setSize-1; (indx>=0) && ((indx==setSize-1)? true : !(part.get(indx) && !part.get(indx+1)) ); indx--) 
  { if ((indx<setSize-1)&&(part.get(indx+1))) { part.set(indx+1, false); }
    if (part.get(indx)) numOnesWithoutZerosToRight++; }
 // can terminate with indx==-1 or at first index of a 1 with a 0 to right
 if (indx != -1)
  {
   part.set(indx, false);  part.set(indx+1,true);
   for (unsigned long i=0; i<numOnesWithoutZerosToRight; i++) part.set(indx+2+i, true); 
  }
 else
  {
   //   (c) if you can't find such a 1 in (b), done with this set size. 
   //   *(d) move up to next set size *if* all set sizes requested...
   if (subSetSize==0) 
    { 
      if (numOnesWithoutZerosToRight == setSize) afterEnd = true;
      else 
       {
        part.reset(); 
        for (unsigned long i=0; i<numOnesWithoutZerosToRight+1; i++) part.set(i, true); 
       }
    }
   else afterEnd=true;      
  }
 return (*this);
}

inline bool NPartition::atEnd()
{ return afterEnd; }

inline bool NPartition::atStart()
{ return beforeStart; }

inline const NBitmask& NPartition::partition() const
{ return part; }        

inline std::string NPartition::textString() const
{
 std::stringstream ss;
 for (unsigned long i=0; i<setSize; i++)
  ss<<( part.get(i) ? "1" : "0" );
 return ss.str();
}


/*@}*/

}// end namespace regina

#endif

