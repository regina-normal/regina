
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

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * An object for describing elements in a n_1xn_2x...xn_k grid
 */
class NMultiIndex {
 private:
  std::vector< unsigned long > data;
 public: 
  NMultiIndex(const unsigned long &dim);
  NMultiIndex(const NMultiIndex &cloneMe);
  ~NMultiIndex();
  unsigned long & operator[](const unsigned long &index);
  bool operator==(const NMultiIndex &q) const;
  bool operator!=(const NMultiIndex &q) const;
  NMultiIndex& operator=(const NMultiIndex &q);
  bool operator<(const NMultiIndex &q) const;
};

/**
 * An object for storing an arbitrary n_1xn_2x...xn_k array of data from 
 * a templated class T sparsely. 
 *
 * @author Ryan Budney
 */
template <class T>
class NSparseGrid {
    private:
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
        NSparseGrid(unsigned long & dim);

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
	~NSparseGrid();

        /**
	 * Assignment of bilinear forms.
         */
        NSparseGrid& operator = (const NSparseGrid & cloneMe);

	void setEntry( const NMultiIndex & I, T & val );

        const T* getEntry( const NMultiIndex & I ) const;

};

/*@}*/

// Inline functions for NMultiIndex

inline NMultiIndex::NMultiIndex(const unsigned long &dim)
{ data.resize(dim); }

inline NMultiIndex::NMultiIndex(const NMultiIndex &cloneMe)
{ data = cloneMe.data; }

inline NMultiIndex::~NMultiIndex()
{}

inline unsigned long & NMultiIndex::operator[](const unsigned long &index)
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

// Inline functions for NSparseGrid

template <class T>
inline NSparseGrid<T>::NSparseGrid(unsigned long & dim)
{ gridim = dim; }

template <class T>
inline NSparseGrid<T>::NSparseGrid(const NSparseGrid & cloneMe)
{
 std::map< NMultiIndex, T* >::const_iterator i;
 for (i = cloneMe.grid.begin(); i != cloneMe.grid.end(); i++)
  grid.insert( std::pair< NMultiIndex, T* >( i->first, clonePtr(i->second) ) );
 gridim = cloneMe.gridim;
}

template <class T>
inline NSparseGrid<T>::~NSparseGrid()
{
 std::map< NMultiIndex, T* >::iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
  delete(i->second);
}

template <class T>
inline NSparseGrid<T>& NSparseGrid<T>::operator = (const NSparseGrid& cloneMe)
{
 // delete old grid
 std::map< NMultiIndex, T* >::iterator i;
 for (i = grid.begin(); i != grid.end(); i++)
   delete(i->second); 
 grid.clear();  
 // copy cloneMe
 std::map< NMultiIndex, T* >::iterator i;
 for (i = cloneMe.grid.begin(); i != cloneMe.grid.end(); i++)
  grid.insert( std::pair< NMultiIndex, T* >( i->first, clonePtr(i->second) ) );
 gridim = cloneMe.gridim;
 return (*this);
}

template <class T>
inline void NSparseGrid::setEntry( const NMultiIndex &I, T &val )
{
// determine if multi-index I is in grid, if so replace by val
 std::map< NMultiIndex, T* >::iterator p;
 p = grid.find( I );
 if ( p != grid.end() ) *(p->second) = val;
// if not, insert
 else grid[I] = new T(val);
}

template <class T>
inline const T* NSparseGrid::getEntry( const NMultiIndex &I ) const
{
 std::map< NMultiIndex, T* >::const_iterator p;
 p = grid.find( I );
 if ( p != grid.end() ) return (p->second);
 else return NULL;
}

} // namespace regina

#endif

