
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

/*! \file maths/sparsegrid.h
 *  \brief An object for storing a sparse n_1xn_2x...xn_k-array of data
 *         from a templated class T.  
 */

#ifndef __SPARSEGRID_H
#ifndef __DOXYGEN
#define __SPARSEGRID_H
#endif

#include <algorithm>
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include "regina-core.h"
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
class MultiIndex {
 protected:
  std::vector< T > data;
 public: 
 /**
  *  Construct a k-tuple, initialized to be zero.
  */
  MultiIndex(unsigned long dim); // k == dim of multi-index
 /**
  *  Construct a 2-tuple, initialized to be (i1, i2)
  */
  MultiIndex(const T& i1, const T& i2); // build a pair i1, i2
 /**
  *  Construct a 3-tuple, initialized to be (i1, i2, i3)
  */
  MultiIndex(const T& i1, const T& i2, const T& i3); // build a triple i1, i2, i3
 /**
  *  Copy constructor.
  */
  MultiIndex(const MultiIndex &cloneMe);
 /**
  *  destructor.
  */
  ~MultiIndex();
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
  bool operator==(const MultiIndex &q) const;
 /**
  *  Inequality operator.
  */
  bool operator!=(const MultiIndex &q) const;
 /**
  *  Assignment operator.
  */
  MultiIndex& operator=(const MultiIndex &q);
 /**
  *  Lexicographic ordering on k-tuples.
  */
  bool operator<(const MultiIndex &q) const;
 /**
  *  Output the k-tuple in format: 5,4,21,3,7
  */
  void writeTextShort(std::ostream& out) const;
};

/**
 * Indexing the terms of a multi-variable polynomial. 
 * only differs from MultiIndex in the way operator<
 * is implemented. Given two k-tuples [a1,...,ak] and
 * [b1,...,bk] the a k-tuple is less than the b k-tuple
 * if and only if either |a1|+...+|ak| < |b1|+...+|bk|
 * or they're equal and [a1,...,ak] is lexicographically
 * less than [b1,...,bk]. See MultiIndex class documentation
 * for most class details.
 *
 * class T must possess an operator+, operator+=
 */
template <class T>
class PolynomialIndex : public MultiIndex<T> {
 public:
 /** 
  * Constructor
  */
  PolynomialIndex(unsigned long dim); // k == dim of multi-index
 /**
  *  Construct a 2-tuple, initialized to be (i1, i2)
  */
  PolynomialIndex(const T& i1, const T& i2); // build a pair i1, i2
 /**
  *  Construct a 3-tuple, initialized to be (i1, i2, i3)
  */
  PolynomialIndex(const T& i1, const T& i2, const T& i3); // build a triple i1, i2, i3
  /**
   *  Copy constructor.
   */
  PolynomialIndex(const PolynomialIndex &cloneMe);
  /**
   * Return the negation the indices.
   */
  PolynomialIndex<T> operator-() const;
  /**
   * Add two multi-indices. Addition of vectors.
   */
  PolynomialIndex<T> operator+(const PolynomialIndex &R) const;
  /**
   *  |i1|+|i2|+...+|ik| lexico order.
   */
  bool operator<(const PolynomialIndex &q) const;
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
class SparseGrid {
    protected:
	/**
	 * Dimension of the grid -- this is the dimension of the multi-index. 
	 */
	unsigned long gridim;

	/**
	 * Internal storage of the grid. 
	 */
	std::map< MultiIndex< unsigned long >, T* > grid;

    public:
        /**
	 * Creates a grid with dimension dim -- ie it is indexed by dim unsigned longs.
         */
        SparseGrid(unsigned long dim);

        /**
         * Creates a permutation that is a clone of the given
         * bilinear form.
         *
         * @param cloneMe the form to clone.
         */
        SparseGrid(const SparseGrid & cloneMe);

	/**
	 * Destructor
	 */
	virtual ~SparseGrid();

   /**
    * Assignment of bilinear forms.
    */
    SparseGrid& operator = (const SparseGrid & cloneMe);

    /**
     * Access to the grid map. 
     */
    const std::map< MultiIndex< unsigned long >, T* > & getGrid() const;

    /**
     * sets the entry corresponding to index I to a pointer
     * to a T type equal to val. If already allocated it
     * copies val to the currently allocated T in the grid. 
     */
    void setEntry( const MultiIndex< unsigned long > & I, const T & val );

    /**
     *  Gives the entry corresponding to index I, the null
     * pointer if it is not allocated.
     */
    const T* getEntry( const MultiIndex< unsigned long > & I ) const;

    /**
     * Lists all elements in the grid.
     */
    void writeTextShort(std::ostream& out) const;
};

/**
 * An object for storing an arbitrary n_1xn_2x...xn_k array of data from 
 * a templated class T sparsely.  
 *
 * Beyond the requirements of SparseGrid, the class T is required to be
 *  of "ring type," meaning: 
 *
 *  1) T::zero exists. 
 *  2) T::operator+= exists. 
 *
 * @author Ryan Budney
 */
template <class T>
class SparseGridRing : public SparseGrid<T> {
   public:

   SparseGridRing(unsigned long dim);

   SparseGridRing(const SparseGridRing & cloneMe);

   /**
    * Overloaded for a val == T::zero safety check.
    */
   void setEntry( const MultiIndex< unsigned long > &I, const T &val );

   /**
    * Increment an entry.  This will allocate the entry if it
    * is not already allocated, and deallocate if after incrementation
    * it becomes zero. 
    */
   void incEntry( const MultiIndex< unsigned long > & I, const T & val );
};

/*@}*/

// Inline functions for MultiIndex

template< class T >
inline MultiIndex<T>::MultiIndex(unsigned long dim)
{ data.resize(dim, 0); }

template< class T >
inline MultiIndex<T>::MultiIndex(const T& i1, const T& i2)
{ data.resize(2, 0); data[0]=i1; data[1]=i2; }

template< class T >
inline MultiIndex<T>::MultiIndex(const T& i1, const T& i2, const T& i3)
{ data.resize(3, 0); data[0]=i1; data[1]=i2; data[2]=i3; }

template< class T >
inline MultiIndex<T>::MultiIndex(const MultiIndex<T> &cloneMe)
{ data = cloneMe.data; }

template< class T >
inline MultiIndex<T>::~MultiIndex() {}

template< class T >
inline T& MultiIndex<T>::operator[](const unsigned long &index)
{ return data[index]; }

template< class T >
inline const T& MultiIndex<T>::entry(const unsigned long &index) const
{ return data[index]; }

template< class T >
inline unsigned long MultiIndex<T>::dim() const
{ return data.size(); }

template< class T >
inline bool MultiIndex<T>::operator==(const MultiIndex<T> &q) const
{ 
 if (data.size() != q.data.size() ) return false;
 for (unsigned long i=0; i<data.size(); i++) if (data[i] != q.data[i]) return false;
 return true;
}

template< class T >
inline bool MultiIndex<T>::operator!=(const MultiIndex<T> &q) const
{ 
 if (data.size() != q.data.size()) return true;
 for (unsigned long i=0; i<data.size(); i++) if (data[i] != q.data[i]) return true;
 return false;
}

template< class T >
inline MultiIndex<T>& MultiIndex<T>::operator=(const MultiIndex<T> &q)
{ 
 data=q.data; 
 return (*this); 
}

template< class T >
inline bool MultiIndex<T>::operator<(const MultiIndex<T> &q) const
{ 
 for (unsigned long i=0; i<data.size(); i++) 
  {
   if (data[i]<q.data[i]) return true;
   else if (data[i]>q.data[i]) return false;
  }
 return false;
}

template< class T >
inline void MultiIndex<T>::writeTextShort(std::ostream& out) const
{
for (unsigned long i=0; i<data.size(); i++)
 {
 if (i!=0) out<<",";
 out<<data[i];
 }
}

// PolynomialIndex

template< class T >
inline PolynomialIndex<T>::PolynomialIndex(unsigned long dim) : MultiIndex<T>(dim) {}

template< class T >
inline PolynomialIndex<T>::PolynomialIndex(const T& i1, const T& i2) : MultiIndex<T>(i1,i2) {}

template< class T >
inline PolynomialIndex<T>::PolynomialIndex(const T& i1, const T& i2, const T& i3) : MultiIndex<T>(i1,i2.i3) {}

template< class T >
inline PolynomialIndex<T>::PolynomialIndex(const PolynomialIndex<T> &cloneMe) : MultiIndex<T>(cloneMe) {}

template< class T >
inline PolynomialIndex<T> PolynomialIndex<T>::operator-() const
{
 PolynomialIndex<T> retval(this->data.size());
 for (unsigned long i=0; i<retval.data.size(); i++)
  retval.data[i] = -this->data[i];
 return retval;
}

template< class T >
inline PolynomialIndex<T> PolynomialIndex<T>::operator+(const PolynomialIndex &R) const
{
 PolynomialIndex<T> retval(this->data.size());
 for (unsigned long i=0; i<retval.data.size(); i++)
  retval.data[i] = this->data[i] + R.data[i];
 return retval;
}

template< class T >
inline bool PolynomialIndex<T>::operator<(const PolynomialIndex<T> &q) const
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

// Inline functions for SparseGrid

template <class T>
inline SparseGrid<T>::SparseGrid(unsigned long dim)
{ gridim = dim; }

template <class T>
inline SparseGrid<T>::SparseGrid(const SparseGrid & cloneMe)
{
 typename std::map< MultiIndex< unsigned long >, T* >::const_iterator i;
 for (i = cloneMe.grid.begin(); i != cloneMe.grid.end(); i++)
  grid.insert( std::pair< MultiIndex< unsigned long >, T* >( i->first, clonePtr(i->second) ) );
 gridim = cloneMe.gridim;
}

template <class T>
inline SparseGrid<T>::~SparseGrid()
{
 typename std::map< MultiIndex< unsigned long >, T* >::iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
  delete(i->second);
 grid.clear();
}

template <class T>
inline SparseGrid<T>& SparseGrid<T>::operator = (const SparseGrid& cloneMe)
{
 // delete old grid
 typename std::map< MultiIndex< unsigned long >, T* >::iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
   delete(i->second); 
 grid.clear();  
 // copy cloneMe
 for (i = cloneMe.grid.begin(); i != cloneMe.grid.end(); i++)
  grid.insert( std::pair< MultiIndex< unsigned long >, T* >( i->first, clonePtr(i->second) ) );
 gridim = cloneMe.gridim;
 return (*this);
}

template <class T>
const std::map< MultiIndex< unsigned long >, T* > & SparseGrid<T>::getGrid() const
{ return grid; }

template <class T>
inline void SparseGrid<T>::setEntry( const MultiIndex< unsigned long > &I, const T &val )
{
 // determine if multi-index I is in grid, if so replace by val
 typename std::map< MultiIndex< unsigned long >, T* >::iterator p;
 p = grid.find( I );
 if ( p != grid.end() ) {(*(p->second)) = val;}
 // if not, insert
 else { grid[I] = new T(val); }
}

template <class T>
inline const T* SparseGrid<T>::getEntry( const MultiIndex< unsigned long > &I ) const
{
 typename std::map< MultiIndex< unsigned long >, T* >::const_iterator p;
 p = grid.find( I );
 if ( p != grid.end() ) return (p->second);
 else return NULL;
}

template <class T>
inline void SparseGrid<T>::writeTextShort(std::ostream& out) const
{
 typename std::map< MultiIndex< unsigned long >, T* >::const_iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
  {
   if (i!=grid.begin()) out<<", ";
   out<<"[";
   out<<"(";   i->first.writeTextShort(out); out<<"), ";
   out<<(i->second)->stringValue();
   out<<"]";
  }
}

// SparseGridRing
template <class T> 
inline void SparseGridRing<T>::incEntry( const MultiIndex< unsigned long > & I, const T & val )
{
 if (val == 0) return;
 typename std::map< MultiIndex< unsigned long >, T* >::iterator p;
 p = this->grid.find( I );
 if ( p != this->grid.end() ) 
  { (*(p->second)) += val;
    if ( (*(p->second)) == T::zero) { delete p->second; this->grid.erase(p); }
  }
 else this->grid[I] = new T(val);
}

template <class T>
inline SparseGridRing<T>::SparseGridRing(unsigned long dim) : SparseGrid<T>(dim)
{}

template <class T>
inline SparseGridRing<T>::SparseGridRing(const SparseGridRing & cloneMe) : 
 SparseGrid<T>(cloneMe)
{}

template <class T>
inline void SparseGridRing<T>::setEntry( const MultiIndex< unsigned long > &I, const T &val )
{
 // determine if multi-index I is in grid, if so replace by val
 if (val == T::zero) return;
 typename std::map< MultiIndex< unsigned long >, T* >::iterator p;
 p = this->grid.find( I );
 if ( p != this->grid.end() ) {(*(p->second)) = val;}
 // if not, insert
 else { this->grid[I] = new T(val); }
}



} // namespace regina

#endif

