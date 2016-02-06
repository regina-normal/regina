
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

/*! \file manifold/notation.h
 *  \brief Explains notation used for describing various types of 3-manifold.
 */

/*! \page sfsnotation Notation for Seifert fibred spaces
 *
 *  Consider a Seifert fibred space over some base orbifold, containing
 *  one or more exceptional fibres and optionally one or more boundary
 *  components.  Here we describe the notation used for the exceptional
 *  fibres, as well as their relationship to curves on the boundary
 *  components.
 *
 *  Consider the illustration below.  Here the base orbifold is
 *  an annulus, with boundary components at the top and bottom of the
 *  diagram.  The two circles in the middle correspond to two
 *  exceptional fibres.
 *
 *  \image html sfsnotation.png
 *
 *  The boundary curves \a o1 and \a o2, as well as the curves \a e1 and
 *  \a e2 bounding the exceptional fibres, can be given consistent
 *  orientations as illustrated by the arrows.  The fibres can also be
 *  consistently oriented; let them be parallel copies of some curve \a f.
 *
 *  Suppose that we describe the space as having exceptional fibres
 *  with parameters (\a p1, \a q1) and (\a p2, \a q2).  This corresponds
 *  to removing all fibres inside curves \a e1 and \a e2, and filling
 *  the resulting boundaries with solid tori whose meridinal curves are
 *  (\a p1 * \a e1 + \a q1 * \a f) and (\a p2 * \a e2 + \a q2 * \a f).
 *
 *  An obstruction constant of \a b is treated as an additional
 *  exceptional fibre with parameters (1, \a b) according to the
 *  description above.
 *
 *  Where necessary, we use the oriented curves \a o1 and \a o2 as
 *  curves on the boundaries <i>"representing the base orbifold"</i>,
 *  and parallel copies of \a f as curves on the boundaries
 *  <i>"representing the fibres"</i>.  This becomes particularly
 *  important when joining different boundary components together.
 *
 *  It is worth noting the following observation.  Suppose we have a
 *  Seifert fibred space with boundary, and let one of its
 *  boundary components have oriented curves (\a f, \a o) representing the
 *  fibres and base orbifold respectively.  Then exactly the same space can
 *  be written with an additional (1,1) fibre (i.e., the obstruction
 *  constant \a b is incremented by one), with the effect that the
 *  curves on that boundary representing the fibres and base orbifold
 *  become (\a f, \a o + \a f) instead.
 */

