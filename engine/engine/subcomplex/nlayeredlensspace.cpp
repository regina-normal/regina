
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nedge.h"
    #include "ncomponent.h"
    #include "ntetrahedron.h"
    #include "nlayeredlensspace.h"
    #include "numbertheory.h"
#else
    #include "engine/triangulation/nedge.h"
    #include "engine/triangulation/ncomponent.h"
	#include "engine/triangulation/ntetrahedron.h"
    #include "engine/subcomplex/nlayeredlensspace.h"
    #include "engine/maths/numbertheory.h"
#endif

NLayeredLensSpace* NLayeredLensSpace::isLayeredLensSpace(
		const NComponent* comp) {
	// Basic property check.
	if ((! comp->isClosed()) || (! comp->isOrientable()))
		return 0;
	if (comp->getNumberOfVertices() > 1)
		return 0;

	unsigned long nTet = comp->getNumberOfTetrahedra();
	NLayeredSolidTorus* torus;
	for (unsigned long i = 0; i < nTet; i++) {
		torus = NLayeredSolidTorus::isLayeredSolidTorusBase(
			comp->getTetrahedron(i));
		if (torus) {
			// We have found a layered solid torus; either this makes the
			// layered lens space or nothing makes the layered lens space.
			NTetrahedron* tet = torus->getTopLevel();
			int tf0 = torus->getTopFace(0);
			int tf1 = torus->getTopFace(1);
			if (tet->getAdjacentTetrahedron(tf0) != tet) {
				delete torus;
				return 0;
			}

			/* We already know the component is orientable; no need
			   to check orientation!
			if (perm.sign() == 1) {
				delete torus;
				return 0;
			}*/

			// This is the real thing!
			NLayeredLensSpace* ans = new NLayeredLensSpace();
			ans->torus = torus;

			NPerm perm = tet->getAdjacentTetrahedronGluing(tf0);
			if (perm[tf1] == tf0) {
				// Snapped shut.
				ans->mobiusBoundaryGroup = torus->getTopEdgeGroup(
					5 - edgeNumber[tf0][tf1]);
			} else {
				// Twisted shut.
				ans->mobiusBoundaryGroup = torus->getTopEdgeGroup(
					edgeNumber[perm[tf1]][tf0]);
			}

			// Work out p and q.
			switch (ans->mobiusBoundaryGroup) {
				// For layered solid torus (x < y < z):
				case 0:
					// L( x + 2y, y )
					ans->p =
						torus->getMeridinalCuts(1) + torus->getMeridinalCuts(2);
					ans->q = torus->getMeridinalCuts(1);
					break;
				case 1:
					// L( 2x + y, x )
					ans->p =
						torus->getMeridinalCuts(0) + torus->getMeridinalCuts(2);
					ans->q = torus->getMeridinalCuts(0);
					break;
				case 2:
					// L( y - x, x )
					ans->p =
						torus->getMeridinalCuts(1) - torus->getMeridinalCuts(0);
					if (ans->p == 0)
						ans->q = 1;
					else
						ans->q = torus->getMeridinalCuts(0) % ans->p;
					break;
			}

			// Find the nicest possible value for q.
			// Choices are +/- q, +/- 1/q.
			if (ans->p > 0) {
				if (2 * ans->q > ans->p)
					ans->q = ans->p - ans->q;
				if (ans->q > 0) {
					unsigned long qAlt = modularInverse(ans->p, ans->q);
					if (2 * qAlt > ans->p)
						qAlt = ans->p - qAlt;
					if (qAlt < ans->q)
						ans->q = qAlt;
				}
			}

			return ans;
		}
	}
	return 0;
}
