
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file enumerate/ndoubledescriptor.h
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
 * \weakgroup enumerate
 * @{
 */

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
REGINA_DEPRECATED typedef NDoubleDescription NDoubleDescriptor;

/*@}*/

} // namespace regina

#endif

