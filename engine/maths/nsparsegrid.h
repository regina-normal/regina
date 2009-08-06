
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

#include <string>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

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
	 * Dimension of the grid -- this is the dimension of the vectors in grid. 
	 */
	unsigned long gridim;

	/**
	 * Internal storage of the grid. 
	 */
	std::map< std::vector< unsigned long >, T* > grid;

    public:
        /**
	 * Creates a grid with dimensions dim[0]x...xdim[dim.size()-1]
         */
        NSparseGrid(const std::vector< unsigned long > &dim);

        /**
         * Creates a permutation that is a clone of the given
         * bilinear form.
         *
         * @param cloneMe the form to clone.
         */
        NSparseGrid(const NSparseGrid& cloneMe);

        /**
	 * Assignment of bilinear forms.
         */
        NSparseGrid& operator = (const NSparseGrid& cloneMe);

	void setEntry( const std::vector< unsigned long > &I, T &val );

        const T* getEntry( const std::vector< unsigned long > &I );

};

/*@}*/

// Inline functions for NSparseGrid

inline NSparseGrid::NSparseGrid(const NSparseGrid& cloneMe) : code(cloneMe.code) {
}

inline NSparseGrid& NSparseGrid::operator = (const NSparseGrid& cloneMe) {
    code = cloneMe.code;
    return *this;
}


} // namespace regina

#endif

