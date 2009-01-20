
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

/*! \file ndoubledescriptor.h
 *  \brief Provides a modified double description method for polytope
 *  vertex enumeration.
 */

#ifndef __NDOUBLEDESCRIPTOR_H
#ifndef __DOXYGEN
#define __NDOUBLEDESCRIPTOR_H
#endif

#warning This header is deprecated; please use ndoubledescription.h instead.

#include "enumerate/ndoubledescription.h"

namespace regina {

/**
 * A legacy typedef provided for backward compatibility only.
 *
 * \deprecated As of Regina 4.6, the class NDoubleDescriptor has been
 * renamed as NDoubleDescription.  This renaming is merely for
 * consistency in documentation and nomenclature; the functionality
 * of the new NDoubleDescription class is identical to the old
 * NDoubleDescriptor class as it was in Regina 4.5.1.  This typedef is
 * provided for backward compatibility, and will be removed in some
 * future version of Regina.
 */
typedef NDoubleDescription NDoubleDescriptor;

} // namespace regina

#endif

