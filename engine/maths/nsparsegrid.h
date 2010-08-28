
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
 * An object that describes an arbitrary k-tuple of elements of a class T.
 *
 * @pre - The class T must have a constructor, assignment operator=, a linear
 *        order <, an inequality != and an output operator <<, and an ability
 *        to cast ints. 
 *
 * \testpart
 *
 */
template <class T>
class NMultiIndex {
 protected:
  std::vector< T > data;
 public: 
 /**
  *  Construct a k-tuple, initialized to be zero.
  */
  NMultiIndex(unsigned long dim); // k == dim of multi-index
 /**
  *  Construct a 2-tuple, initialized to be (i1, i2)
  */
  NMultiIndex(const T& i1, const T& i2); // build a pair i1, i2
 /**
  *  Construct a 3-tuple, initialized to be (i1, i2, i3)
  */
  NMultiIndex(const T& i1, const T& i2, const T& i3); // build a triple i1, i2, i3
 /**
  *  Copy constructor.
  */
  NMultiIndex(const NMultiIndex &cloneMe);
 /**
  *  destructor.
  */
  ~NMultiIndex();
 /**
  *  Return a reference to the i-th element from the k-tuple.
  */
  T & operator[](const unsigned long &index);
 /**
  *  Return a const reference to the i-th element from the k-tuple. 
  */
  const T & entry(const unsigned long &index) const;
 /**
  *  Return the dimension of the k-tuple, ie, k.
  */
  unsigned long dim() const;
 /**
  *  Equality operator.
  */
  bool operator==(const NMultiIndex &q) const;
 /**
  *  Inequality operator.
  */
  bool operator!=(const NMultiIndex &q) const;
 /**
  *  Assignment operator.
  */
  NMultiIndex& operator=(const NMultiIndex &q);
 /**
  *  Lexicographic ordering on k-tuples.
  */
  bool operator<(const NMultiIndex &q) const;
 /**
  *  Output the k-tuple in format: 5,4,21,3,7
  */
  void writeTextShort(std::ostream& out) const;
};

/**
 * Indexing the terms of a multi-variable polynomial. 
 * only differs from NMultiIndex in the way operator<
 * is implemented. Given two k-tuples [a1,...,ak] and
 * [b1,...,bk] the a k-tuple is less than the b k-tuple
 * if and only if either |a1|+...+|ak| < |b1|+...+|bk|
 * or they're equal and [a1,...,ak] is lexicographically
 * less than [b1,...,bk]. See NMultiIndex class documentation
 * for most class details.
 *
 * class T must possess an operator+, operator+=
 */
template <class T>
class NPolynomialIndex : public NMultiIndex<T> {
 public:
 /** 
  * Constructor
  */
  NPolynomialIndex(unsigned long dim); // k == dim of multi-index
 /**
  *  Construct a 2-tuple, initialized to be (i1, i2)
  */
  NPolynomialIndex(const T& i1, const T& i2); // build a pair i1, i2
 /**
  *  Construct a 3-tuple, initialized to be (i1, i2, i3)
  */
  NPolynomialIndex(const T& i1, const T& i2, const T& i3); // build a triple i1, i2, i3
  /**
   *  Copy constructor.
   */
  NPolynomialIndex(const NPolynomialIndex &cloneMe);
  /**
   * Return the negation the indices.
   */
  NPolynomialIndex<T> operator-() const;
  /**
   * Add two multi-indices. Addition of vectors.
   */
  NPolynomialIndex<T> operator+(const NPolynomialIndex &R) const;
  /**
   *  |i1|+|i2|+...+|ik| lexico order.
   */
  bool operator<(const NPolynomialIndex &q) const;
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
	std::map< NMultiIndex< unsigned long >, T* > grid;

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
	const std::map< NMultiIndex< unsigned long >, T* > & getGrid() const;

	/**
	 * sets the entry corresponding to index I to a pointer
         * to a T type equal to val. If already allocated it
         * copies val to the currently allocated T in the grid. 
         */
	void setEntry( const NMultiIndex< unsigned long > & I, const T & val );

	/**
         *  Gives the entry corresponding to index I, the null
         * pointer if it is not allocated.
	 */
        const T* getEntry( const NMultiIndex< unsigned long > & I ) const;

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
        void setEntry( const NMultiIndex< unsigned long > &I, const T &val );

	/**
	 * Increment an entry.  This will allocate the entry if it
         * is not already allocated, and deallocate if after incrementation
         * it becomes zero. 
	 */
	void incEntry( const NMultiIndex< unsigned long > & I, const T & val );
};

/*@}*/

// Inline functions for NMultiIndex

template< class T >
inline NMultiIndex<T>::NMultiIndex(unsigned long dim)
{ data.resize(dim, 0); }

template< class T >
inline NMultiIndex<T>::NMultiIndex(const T& i1, const T& i2)
{ data.resize(2, 0); data[0]=i1; data[1]=i2; }

template< class T >
inline NMultiIndex<T>::NMultiIndex(const T& i1, const T& i2, const T& i3)
{ data.resize(3, 0); data[0]=i1; data[1]=i2; data[2]=i3; }

template< class T >
inline NMultiIndex<T>::NMultiIndex(const NMultiIndex<T> &cloneMe)
{ data = cloneMe.data; }

template< class T >
inline NMultiIndex<T>::~NMultiIndex() {}

template< class T >
inline T& NMultiIndex<T>::operator[](const unsigned long &index)
{ return data[index]; }

template< class T >
inline const T& NMultiIndex<T>::entry(const unsigned long &index) const
{ return data[index]; }

template< class T >
inline unsigned long NMultiIndex<T>::dim() const
{ return data.size(); }

template< class T >
inline bool NMultiIndex<T>::operator==(const NMultiIndex<T> &q) const
{ 
 if (data.size() != q.data.size() ) return false;
 for (unsigned long i=0; i<data.size(); i++) if (data[i] != q.data[i]) return false;
 return true;
}

template< class T >
inline bool NMultiIndex<T>::operator!=(const NMultiIndex<T> &q) const
{ 
 if (data.size() != q.data.size()) return true;
 for (unsigned long i=0; i<data.size(); i++) if (data[i] != q.data[i]) return true;
 return false;
}

template< class T >
inline NMultiIndex<T>& NMultiIndex<T>::operator=(const NMultiIndex<T> &q)
{ 
 data=q.data; 
 return (*this); 
}

template< class T >
inline bool NMultiIndex<T>::operator<(const NMultiIndex<T> &q) const
{ 
 for (unsigned long i=0; i<data.size(); i++) 
  {
   if (data[i]<q.data[i]) return true;
   else if (data[i]>q.data[i]) return false;
  }
 return false;
}

template< class T >
inline void NMultiIndex<T>::writeTextShort(std::ostream& out) const
{
for (unsigned long i=0; i<data.size(); i++)
 {
 if (i!=0) out<<",";
 out<<data[i];
 }
}

// NPolynomialIndex

template< class T >
inline NPolynomialIndex<T>::NPolynomialIndex(unsigned long dim) : NMultiIndex<T>(dim) {}

template< class T >
inline NPolynomialIndex<T>::NPolynomialIndex(const T& i1, const T& i2) : NMultiIndex<T>(i1,i2) {}

template< class T >
inline NPolynomialIndex<T>::NPolynomialIndex(const T& i1, const T& i2, const T& i3) : NMultiIndex<T>(i1,i2.i3) {}

template< class T >
inline NPolynomialIndex<T>::NPolynomialIndex(const NPolynomialIndex<T> &cloneMe) : NMultiIndex<T>(cloneMe) {}

template< class T >
inline NPolynomialIndex<T> NPolynomialIndex<T>::operator-() const
{
 NPolynomialIndex<T> retval(this->data.size());
 for (unsigned long i=0; i<retval.data.size(); i++)
  retval.data[i] = -this->data[i];
 return retval;
}

template< class T >
inline NPolynomialIndex<T> NPolynomialIndex<T>::operator+(const NPolynomialIndex &R) const
{
 NPolynomialIndex<T> retval(this->data.size());
 for (unsigned long i=0; i<retval.data.size(); i++)
  retval.data[i] = this->data[i] + R.data[i];
 return retval;
}

template< class T >
inline bool NPolynomialIndex<T>::operator<(const NPolynomialIndex<T> &q) const
{ 
 T sum1(0); T sum2(0); 
 for (unsigned long i=0; i<this->data.size(); i++)
  { sum1 += abs(this->data[i]); sum2 += abs(q.data[i]); }
 if (sum1 < sum2) return true; if (sum2 > sum1) return false; 
 for (unsigned long i=0; i<this->data.size(); i++) 
  {
   if (this->data[i]<q.data[i]) return true;
   else if (this->data[i]>q.data[i]) return false;
  }
 return false;
}

// Inline functions for NSparseGrid

template <class T>
inline NSparseGrid<T>::NSparseGrid(unsigned long dim)
{ gridim = dim; }

template <class T>
inline NSparseGrid<T>::NSparseGrid(const NSparseGrid & cloneMe)
{
 typename std::map< NMultiIndex< unsigned long >, T* >::const_iterator i;
 for (i = cloneMe.grid.begin(); i != cloneMe.grid.end(); i++)
  grid.insert( std::pair< NMultiIndex< unsigned long >, T* >( i->first, clonePtr(i->second) ) );
 gridim = cloneMe.gridim;
}

template <class T>
inline NSparseGrid<T>::~NSparseGrid()
{
 typename std::map< NMultiIndex< unsigned long >, T* >::iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
  delete(i->second);
 grid.clear();
}

template <class T>
inline NSparseGrid<T>& NSparseGrid<T>::operator = (const NSparseGrid& cloneMe)
{
 // delete old grid
 typename std::map< NMultiIndex< unsigned long >, T* >::iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
   delete(i->second); 
 grid.clear();  
 // copy cloneMe
 for (i = cloneMe.grid.begin(); i != cloneMe.grid.end(); i++)
  grid.insert( std::pair< NMultiIndex< unsigned long >, T* >( i->first, clonePtr(i->second) ) );
 gridim = cloneMe.gridim;
 return (*this);
}

template <class T>
const std::map< NMultiIndex< unsigned long >, T* > & NSparseGrid<T>::getGrid() const
{ return grid; }

template <class T>
inline void NSparseGrid<T>::setEntry( const NMultiIndex< unsigned long > &I, const T &val )
{
 // determine if multi-index I is in grid, if so replace by val
 typename std::map< NMultiIndex< unsigned long >, T* >::iterator p;
 p = grid.find( I );
 if ( p != grid.end() ) {(*(p->second)) = val;}
 // if not, insert
 else { grid[I] = new T(val); }
}

template <class T>
inline const T* NSparseGrid<T>::getEntry( const NMultiIndex< unsigned long > &I ) const
{
 typename std::map< NMultiIndex< unsigned long >, T* >::const_iterator p;
 p = grid.find( I );
 if ( p != grid.end() ) return (p->second);
 else return NULL;
}

template <class T>
inline void NSparseGrid<T>::writeTextShort(std::ostream& out) const
{
 typename std::map< NMultiIndex< unsigned long >, T* >::const_iterator i;
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
inline void NSparseGridRing<T>::incEntry( const NMultiIndex< unsigned long > & I, const T & val )
{
 if (val == 0) return;
 typename std::map< NMultiIndex< unsigned long >, T* >::iterator p;
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
inline void NSparseGridRing<T>::setEntry( const NMultiIndex< unsigned long > &I, const T &val )
{
 // determine if multi-index I is in grid, if so replace by val
 if (val == T::zero) return;
 typename std::map< NMultiIndex< unsigned long >, T* >::iterator p;
 p = this->grid.find( I );
 if ( p != this->grid.end() ) {(*(p->second)) = val;}
 // if not, insert
 else { this->grid[I] = new T(val); }
}



} // namespace regina

#endif

