
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

/*! \file triangulation/nperm.h
 *  \brief A deprecated header for dealing with permutations of {0,1,2,3}.
 */

#ifndef __NPERM_H
#ifndef __DOXYGEN
#define __NPERM_H
#endif

#warning This header is deprecated; please use maths/nperm4.h instead.

#include "maths/nperm4.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * A legacy typedef provided for backward compatibility only.
 *
 * \deprecated As of Regina 4.6.1, the class NPerm has been renamed as
 * NPerm4 (which has identical functionality).  This NPerm typedef is
 * provided for backward compatibility, and will be removed in some future
 * version of Regina.
 */
typedef NPerm4 NPerm;

/*@}*/

} // namespace regina

#endif

