
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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
    #include "ncomponent.h"
    #include "ntetrahedron.h"
    #include "naugtrisolidtorus.h"
#else
    #include "engine/triangulation/ncomponent.h"
    #include "engine/triangulation/ntetrahedron.h"
    #include "engine/subcomplex/naugtrisolidtorus.h"
#endif

NAugTriSolidTorus::~NAugTriSolidTorus() {
    if (core)
        delete core;
    for (int i = 0; i < 3; i++)
        if (augTorus[i])
            delete augTorus[i];
}

NAugTriSolidTorus* NAugTriSolidTorus::clone() const {
    NAugTriSolidTorus* ans = new NAugTriSolidTorus();
    ans->core = core->clone();
    for (int i = 0; i < 3; i++) {
        if (augTorus[i])
            ans->augTorus[i] = augTorus[i]->clone();
        ans->edgeGroupRoles[i] = edgeGroupRoles[i];
        ans->alpha[i] = alpha[i];
        ans->beta[i] = beta[i];
    }
    return ans;
}

long NAugTriSolidTorus::getExceptionalFibres() const {
    if (alpha[0] == 0)
        return -1;
    else if (alpha[2] > 1)
        return 3;
    else if (alpha[1] > 1)
        return 2;
    else if (alpha[0] > 1)
        return 1;
    else
        return 0;
}

void NAugTriSolidTorus::writeTextShort(ostream& out) const {
    out << "Augmented triangular solid torus: ";
    switch (getExceptionalFibres()) {
        case -1:
            out << "(undefined)";
            return;
        case 0:
            if (beta[0] == 0) {
                out << "(S2 x S1)";
                return;
            }
            // Otherwise we have a single (1,k) fibre; fall through to
            // the next case.
        case 1:
            out << "(S2, (" << alpha[0] << ',' << beta[0] << "))";
            return;
        case 2:
            out << "(S2, ("
                << alpha[0] << ',' << beta[0] << ") ("
                << alpha[1] << ',' << beta[1] << "))";
            return;
        case 3:
            out << "(S2, ("
                << alpha[0] << ',' << beta[0] << ") ("
                << alpha[1] << ',' << beta[1] << ") ("
                << alpha[2] << ',' << beta[2] << "))";
            return;
        default:
            out << "(internal error; please report this!)";
            return;
    }
}

void NAugTriSolidTorus::findExceptionalFibres() {
    // We will always assume the existance of an extra (1, k) fibre.
    long k = 1;

    int i;
    for (i = 0; i < 3; i++) {
        if (edgeGroupRoles[i][2] == 2) {
            if (augTorus[i]) {
                alpha[i] = augTorus[i]->getMeridinalCuts(edgeGroupRoles[i][0]);
                beta[i] = augTorus[i]->getMeridinalCuts(edgeGroupRoles[i][1]);
            } else {
                alpha[i] = 1;
                beta[i] = 1;
            }
        } else {
            if (augTorus[i]) {
                alpha[i] = augTorus[i]->getMeridinalCuts(edgeGroupRoles[i][0]);
                beta[i] = -augTorus[i]->getMeridinalCuts(edgeGroupRoles[i][1]);
            } else {
                alpha[i] = (edgeGroupRoles[i][0] == 2 ? 2 : 1);
                beta[i] = -(edgeGroupRoles[i][1] == 2 ? 2 : 1);
            }
        }
        // Merge (1,b) fibres in with the (1,k) immediately.
        if (alpha[i] == 1) {
            k += beta[i];
            beta[i] = 0;
        }
    }

    // We now have a correct set of parameters in rather non-standard form.
    int nZeroBetas = 0;
    for (i = 0; i < 3; i++) {
        if (alpha[i] == 0) {
            // Currently beta[i] is already +/-1, and as far as the
            // layered solid torus goes the two are indistinguishable.
            // Thus we'll go ahead and make it 1.
            beta[i] = 1;
        } else if (beta[i] >= alpha[i]) {
            k += (beta[i] / alpha[i]);
            beta[i] = beta[i] % alpha[i];
        } else if (beta[i] < 0) {
            k += (beta[i] / alpha[i]);
            beta[i] = beta[i] % alpha[i];
            if (beta[i] < 0) {
                k--;
                beta[i] += alpha[i];
            }
        }

        if (beta[i] == 0)
            nZeroBetas++;
    }

    // Now all beta parameters are between 0 and alpha - 1.
    if (k < (-k - 3 + nZeroBetas)) {
        k = -k;
        for (i = 0; i < 3; i++)
            if (alpha[i] == 0)
                beta[i] = 1;
            else {
                beta[i] = -beta[i];
                if (beta[i] < 0) {
                    k--;
                    beta[i] += alpha[i];
                }
            }
    }

    // All that's left now is to sort and then merge (1,k) with the final
    // exceptional fibre.
    // Run the following for loop with (i,j) = (0,1), (0,2), (1,2).
    int j;
    long tmp;
    for (i = 0; i < 2; i++)
        for (j = i+1; j < 3; j++)
            if (alpha[i] == 1 || (alpha[i] > alpha[j] && alpha[j] != 1) ||
                    (alpha[i] == alpha[j] && beta[i] > beta[j])) {
                tmp = alpha[i]; alpha[i] = alpha[j]; alpha[j] = tmp;
                tmp = beta[i]; beta[i] = beta[j]; beta[j] = tmp;
            }

    if (alpha[2] != 1)
        beta[2] += (alpha[2] * k);
    else if (alpha[1] != 1)
        beta[1] += (alpha[1] * k);
    else
        beta[0] += (alpha[0] * k);
}

NAugTriSolidTorus* NAugTriSolidTorus::isAugTriSolidTorus(
        const NComponent* comp) {
    // Basic property checks.
    if ((! comp->isClosed()) || (! comp->isOrientable()))
        return 0;
    if (comp->getNumberOfVertices() > 1)
        return 0;

    // We have a 1-vertex closed orientable triangulation.

    unsigned long nTet = comp->getNumberOfTetrahedra();
    if (nTet < 3)
        return 0;

    // Handle the 3-tetrahedron case separately.
    if (nTet == 3) {
        NTetrahedron* base = comp->getTetrahedron(0);
        NTriSolidTorus* core;
        NPerm annulusMap[3];
        // Check every possible choice of vertex roles in tetrahedron 0.
        // Note that we do twice the checks we need to since (a,b,c,d)
        // gives an equivalent core to (b,a,d,c).
        int i, j;
        for (i = 0; i < 24; i++) {
            core = NTriSolidTorus::isTriSolidTorus(base, allPermsS4[i]);
            if (core) {
                // Check that the annuli are being glued to themselves.
                // Since the component is orientable, that's all we need
                // to know.
                for (j = 0; j < 3; j++) {
                    if (! core->isAnnulusSelfIdentified(j, annulusMap + j)) {
                        delete core;
                        core = 0;
                        break;
                    }
                }

                if (core) {
                    // We got one!
                    NAugTriSolidTorus* ans = new NAugTriSolidTorus();
                    ans->core = core;

                    // Work out how the mobius strip is glued onto each
                    // annulus.
                    for (j = 0; j < 3; j++) {
                        switch (annulusMap[j][0]) {
                            case 0:
                                ans->edgeGroupRoles[j] = NPerm(2, 0, 1, 3);
                                break;
                            case 1:
                                ans->edgeGroupRoles[j] = NPerm(0, 1, 2, 3);
                                break;
                            case 3:
                                ans->edgeGroupRoles[j] = NPerm(1, 2, 0, 3);
                                break;
                        }
                    }

                    ans->findExceptionalFibres();
                    return ans;
                }
            }
        }
    }

    // Ignoring the >3 tetrahedron case for now.
    return 0;

    /*
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

            // This is the real thing!
            NAugTriSolidTorus* ans = new NAugTriSolidTorus();
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
    */
}
