
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file census/gluingpermsearcher.h
 *  \brief Supports searching through all possible sets of gluing permutations
 *  for a given facet pairing.
 */

#ifndef __REGINA_GLUINGPERMSEARCHER_H
#ifndef __DOXYGEN
#define __REGINA_GLUINGPERMSEARCHER_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * A utility class used to build a census of triangulations, by searching
 * through all possible sets of gluing permutations to match a given
 * facet pairing.
 *
 * Currently this class is only implemented in Regina's
 * \ref stddim "standard dimensions".  In higher dimensions, the class
 * GluingPermSearcher<dim> remains undefined, as a placeholder for
 * if/when Regina implements higher-dimensional census code.
 *
 * Gluing permutation searchers are designed to manage the construction of
 * a large census of triangulations, and so they cannot be copied, moved or
 * swapped.  They can be constructed manually if you need fine-grained control,
 * but often this is not necessary either; instead you can run a "typical"
 * census using the static routine GluingPermSearcher<dim>::findAllPerms().
 *
 * For further information, see the documentation for the specialisations
 * GluingPermSearcher<2>, GluingPermSearcher<3> and GluingPermSearcher<4>.
 *
 * \python Python does not support templates.  Instead this class can
 * be used by appending the dimension as a suffix (e.g., GluingPermSearcher2
 * and GluingPermSearcher3 for dimensions 2 and 3).
 *
 * \ingroup census
 */
template <int dim>
class GluingPermSearcher;

} // namespace regina

#endif

