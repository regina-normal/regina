
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/*! \file generic/facenumbering.h
 *  \brief Describes the way in which <i>subdim</i>-faces are numbered
 *  within a <i>dim</i>-dimensional simplex.
 */

#ifndef __FACENUMBERING_H
#ifndef __DOXYGEN
#define __FACENUMBERING_H
#endif

#include "generic/detail/facenumbering.h"

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * Specifies how <i>subdim</i>-faces are numbered within a
 * <i>dim</i>-dimensional simplex.
 *
 * Every class Face<dim, subdim> inherits from this class, which means
 * you can access these routines as Face<dim, subdim>::ordering(),
 * Face<dim, subdim>::faceNumber(), and so on.
 *
 * An advantage of referring to FaceNumbering<dim, subdim> directly (as
 * opposed to Face<dim, subdim>) is that its header is lightweight: it does not
 * pull in the large and complex headers required by Face<dim, subdim>.
 *
 * This class is specialised (and optimised) in Regina's
 * \ref stddim "standard dimensions".
 *
 * \ifacespython This class is not available in Python.  However, all of
 * its routines can be accessed through Face<dim, subdim> (which in Python
 * becomes Face<i>dim</i>_<i>subdim</i>, or one of the typedefs in
 * \ref stddim "standard dimensions" such as Dim2Edge, NVertex and so on).
 *
 * \tparam dim the dimension of the simplex whose faces we are numbering.
 * This must be at least 2.
 * \tparam subdim the dimension of the faces that we are numbering.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 *
 * TODO: Finish docs.
 */
template <int dim, int subdim>
class FaceNumbering : public detail::FaceNumberingImpl<
        dim, subdim, ((dim + 1) >= 2 * (subdim + 1))> {
};

} // namespace regina

#endif

