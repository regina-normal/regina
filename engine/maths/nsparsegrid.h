
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

/*! \file maths/nsparsegrid.h
 *  \brief An object for storing a sparse n_1xn_2x...xn_k-array of data
 *         from a templated class T.  
 */

#ifndef __NSPARSEGRID_H
#ifndef __DOXYGEN
#define __NSPARSEGRID_H
#endif

#include <algorithm>
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include "utilities/ptrutils.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * An object for describing elements in a n_1 x n_2 x ... x n_k grid.
 */
class NMultiIndex {
 private:
  std::vector< unsigned long > data;
 public: 
  NMultiIndex(unsigned long dim); // k == dim of multi-index
  NMultiIndex(unsigned long i1, unsigned long i2); // build a pair i1, i2
  NMultiIndex(unsigned long i1, unsigned long i2, unsigned long i3); // build a triple i1, i2, i3
  NMultiIndex(const NMultiIndex &cloneMe);
  ~NMultiIndex();
  unsigned long & operator[](const unsigned long &index);
  const unsigned long & entry(const unsigned long &index) const;
  bool operator==(const NMultiIndex &q) const;
  bool operator!=(const NMultiIndex &q) const;
  NMultiIndex& operator=(const NMultiIndex &q);
  bool operator<(const NMultiIndex &q) const;
  virtual void writeTextShort(std::ostream& out) const;
};

/**
 * An object for storing an arbitrary n_1xn_2x...xn_k array of data from 
 * a templated class T sparsely.  
 *
 * The class T requires the following: 
 *
 *  1) A copy constructor. 
 *  2) An assignment "=" operator. 
 *  3) An output operation std::string T::stringValue()
 *
 * @author Ryan Budney
 */
template <class T>
class NSparseGrid {
    protected:
	/**
	 * Dimension of the grid -- this is the dimension of the multi-index. 
	 */
	unsigned long gridim;

	/**
	 * Internal storage of the grid. 
	 */
	std::map< NMultiIndex, T* > grid;

    public:
        /**
	 * Creates a grid with dimension dim -- ie it is indexed by dim unsigned longs.
         */
        NSparseGrid(unsigned long dim);

        /**
         * Creates a permutation that is a clone of the given
         * bilinear form.
         *
         * @param cloneMe the form to clone.
         */
        NSparseGrid(const NSparseGrid & cloneMe);

	/**
	 * Destructor
	 */
	virtual ~NSparseGrid();

        /**
	 * Assignment of bilinear forms.
         */
        NSparseGrid& operator = (const NSparseGrid & cloneMe);

	/**
	 * Access to the grid map. 
	 */
	const std::map< NMultiIndex, T* > & getGrid() const;

	/**
	 * sets the entry corresponding to index I to a pointer
         * to a T type equal to val. If already allocated it
         * copies val to the currently allocated T in the grid. 
         */
	void setEntry( const NMultiIndex & I, const T & val );

	/**
         *  Gives the entry corresponding to index I, the null
         * pointer if it is not allocated.
	 */
        const T* getEntry( const NMultiIndex & I ) const;

        /**
	 * Lists all elements in the grid.
	 */
        void writeTextShort(std::ostream& out) const;
};

/**
 * An object for storing an arbitrary n_1xn_2x...xn_k array of data from 
 * a templated class T sparsely.  
 *
 * Beyond the requirements of NSparseGrid, the class T is required to be
 *  of "ring type," meaning: 
 *
 *  1) T::zero exists. 
 *  2) T::operator+= exists. 
 *
 * @author Ryan Budney
 */
template <class T>
class NSparseGridRing : public NSparseGrid<T> {
        public:

        NSparseGridRing(unsigned long dim);

        NSparseGridRing(const NSparseGridRing & cloneMe);

        /**
         * Overloaded for a val == T::zero safety check.
         */
        void setEntry( const NMultiIndex &I, const T &val );

	/**
	 * Increment an entry.  This will allocate the entry if it
         * is not already allocated, and deallocate if after incrementation
         * it becomes zero. 
	 */
	void incEntry( const NMultiIndex & I, const T & val );
};

/*@}*/

// Inline functions for NMultiIndex

inline NMultiIndex::NMultiIndex(unsigned long dim)
{ data.resize(dim, 0); }

inline NMultiIndex::NMultiIndex(unsigned long i1, unsigned long i2)
{ data.resize(2, 0); data[0]=i1; data[1]=i2; }

inline NMultiIndex::NMultiIndex(unsigned long i1, unsigned long i2, unsigned long i3)
{ data.resize(3, 0); data[0]=i1; data[1]=i2; data[2]=i3; }

inline NMultiIndex::NMultiIndex(const NMultiIndex &cloneMe)
{ data = cloneMe.data; }

inline NMultiIndex::~NMultiIndex()
{}

inline unsigned long & NMultiIndex::operator[](const unsigned long &index)
{ return data[index]; }

inline const unsigned long & NMultiIndex::entry(const unsigned long &index) const
{ return data[index]; }

inline bool NMultiIndex::operator==(const NMultiIndex &q) const
{ 
if (data.size() != q.data.size() ) return false;
for (unsigned long i=0; i<data.size(); i++) if (data[i] != q.data[i]) return false;
return true;
}

inline bool NMultiIndex::operator!=(const NMultiIndex &q) const
{ 
if (data.size() != q.data.size()) return true;
for (unsigned long i=0; i<data.size(); i++) if (data[i] != q.data[i]) return true;
return false;
}

inline NMultiIndex& NMultiIndex::operator=(const NMultiIndex &q)
{ 
data=q.data; 
return (*this); 
}

inline bool NMultiIndex::operator<(const NMultiIndex &q) const
{ 
 for (unsigned long i=0; i<data.size(); i++) 
  {
   if (data[i]<q.data[i]) return true;
   else if (data[i]>q.data[i]) return false;
  }
 return false;
}

inline void NMultiIndex::writeTextShort(std::ostream& out) const
{
for (unsigned long i=0; i<data.size(); i++)
 {
 if (i!=0) out<<", ";
 out<<data[i];
 }
}

// Inline functions for NSparseGrid

template <class T>
inline NSparseGrid<T>::NSparseGrid(unsigned long dim)
{ gridim = dim; }

template <class T>
inline NSparseGrid<T>::NSparseGrid(const NSparseGrid & cloneMe)
{
 typename std::map< NMultiIndex, T* >::const_iterator i;
 for (i = cloneMe.grid.begin(); i != cloneMe.grid.end(); i++)
  grid.insert( std::pair< NMultiIndex, T* >( i->first, clonePtr(i->second) ) );
 gridim = cloneMe.gridim;
}

template <class T>
inline NSparseGrid<T>::~NSparseGrid()
{
 typename std::map< NMultiIndex, T* >::iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
  delete(i->second);
 grid.clear();
}

template <class T>
inline NSparseGrid<T>& NSparseGrid<T>::operator = (const NSparseGrid& cloneMe)
{
 // delete old grid
 typename std::map< NMultiIndex, T* >::iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
   delete(i->second); 
 grid.clear();  
 // copy cloneMe
 for (i = cloneMe.grid.begin(); i != cloneMe.grid.end(); i++)
  grid.insert( std::pair< NMultiIndex, T* >( i->first, clonePtr(i->second) ) );
 gridim = cloneMe.gridim;
 return (*this);
}

template <class T>
const std::map< NMultiIndex, T* > & NSparseGrid<T>::getGrid() const
{ return grid; }

template <class T>
inline void NSparseGrid<T>::setEntry( const NMultiIndex &I, const T &val )
{
 // determine if multi-index I is in grid, if so replace by val
 typename std::map< NMultiIndex, T* >::iterator p;
 p = grid.find( I );
 if ( p != grid.end() ) {(*(p->second)) = val;}
 // if not, insert
 else { grid[I] = new T(val); }
}

template <class T>
inline const T* NSparseGrid<T>::getEntry( const NMultiIndex &I ) const
{
 typename std::map< NMultiIndex, T* >::const_iterator p;
 p = grid.find( I );
 if ( p != grid.end() ) return (p->second);
 else return NULL;
}

template <class T>
inline void NSparseGrid<T>::writeTextShort(std::ostream& out) const
{
 typename std::map< NMultiIndex, T* >::const_iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
  {
   if (i!=grid.begin()) out<<", ";
   out<<"[";
   out<<"(";   i->first.writeTextShort(out); out<<"), ";
   out<<(i->second)->stringValue();
   out<<"]";
  }
}

// NSparseGridRing
template <class T> 
inline void NSparseGridRing<T>::incEntry( const NMultiIndex & I, const T & val )
{
 if (val == 0) return;
 typename std::map< NMultiIndex, T* >::iterator p;
 p = this->grid.find( I );
 if ( p != this->grid.end() ) 
  { (*(p->second)) += val;
    if ( (*(p->second)) == T::zero) { delete p->second; this->grid.erase(p); }
  }
 else this->grid[I] = new T(val);
}

template <class T>
inline NSparseGridRing<T>::NSparseGridRing(unsigned long dim) : NSparseGrid<T>(dim)
{}

template <class T>
inline NSparseGridRing<T>::NSparseGridRing(const NSparseGridRing & cloneMe) : 
 NSparseGrid<T>(cloneMe)
{}

template <class T>
inline void NSparseGridRing<T>::setEntry( const NMultiIndex &I, const T &val )
{
 // determine if multi-index I is in grid, if so replace by val
 if (val == T::zero) return;
 typename std::map< NMultiIndex, T* >::iterator p;
 p = this->grid.find( I );
 if ( p != this->grid.end() ) {(*(p->second)) = val;}
 // if not, insert
 else { this->grid[I] = new T(val); }
}



} // namespace regina

#endif

