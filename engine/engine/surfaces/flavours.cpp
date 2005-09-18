
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include "surfaces/flavourregistry.h"

namespace regina {

#define __FLAVOUR_REGISTRY_BODY

// Bring in routines from the flavour registry.

#define REGISTER_FLAVOUR(id_name, class, n, almost_normal, t) \
    NVector<NLargeInteger>* class::clone() const { \
        return new class(*this); \
    } \
    bool class::allowsAlmostNormal() const { \
        return almost_normal; \
    }

#include "surfaces/flavourregistry.h"

// Tidy up.
#undef REGISTER_FLAVOUR
#undef __FLAVOUR_REGISTRY_BODY

} // namespace regina

