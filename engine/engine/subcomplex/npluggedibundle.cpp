
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "subcomplex/npluggedibundle.h"
#include "subcomplex/ntorusplug.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

const int NPluggedIBundleCore::T_3_1 = 301;
const int NPluggedIBundleCore::T_3_2 = 302;
const int NPluggedIBundleCore::T_5_1 = 501;
const int NPluggedIBundleCore::T_6_1 = 601;
const int NPluggedIBundleCore::T_6_2 = 602;
const int NPluggedIBundleCore::T_6_3 = 603;
const int NPluggedIBundleCore::T_6_4 = 604;

namespace {
    const NPluggedIBundleCore core_T_3_1(NPluggedIBundleCore::T_3_1);
    const NPluggedIBundleCore core_T_3_2(NPluggedIBundleCore::T_3_2);
    const NPluggedIBundleCore core_T_5_1(NPluggedIBundleCore::T_5_1);
    const NPluggedIBundleCore core_T_6_1(NPluggedIBundleCore::T_6_1);
    const NPluggedIBundleCore core_T_6_2(NPluggedIBundleCore::T_6_2);
    const NPluggedIBundleCore core_T_6_3(NPluggedIBundleCore::T_6_3);
    const NPluggedIBundleCore core_T_6_4(NPluggedIBundleCore::T_6_4);
}

NPluggedIBundleCore::NPluggedIBundleCore(int whichCoreType) :
        coreType(whichCoreType) {
    if (coreType == T_3_1) {
        const int adj[3][4] = {
            { 1, 2, 2, -1},
            { 0, 2, 2, -1},
            { 1, 0, 1, 0}
        };

        const int glu[3][4][4] = {
            { { 0, 2, 1, 3 }, { 3, 1, 2, 0 }, { 1, 0, 3, 2 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 2, 0, 3, 1 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 3, 0, 2 }, { 3, 1, 2, 0 }, { 0, 1, 2, 3 }, { 1, 0, 3, 2 } }
        };

        core.insertConstruction(3, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_3_2) {
        const int adj[3][4] = {
            { 2, 2, 1, -1},
            { 2, 2, 0, -1},
            { 1, 0, 1, 0}
        };

        const int glu[3][4][4] = {
            { { 3, 2, 1, 0 }, { 0, 1, 3, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 1, 2, 3 }, { 3, 2, 0, 1 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 2, 3, 1, 0 }, { 3, 2, 1, 0 } }
        };

        core.insertConstruction(3, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_6_1) {
        const int adj[6][4] = {
            { 1, 5, 3, -1},
            { 0, 5, 4, -1},
            { 3, 4, 5, -1},
            { 2, 4, 0, -1},
            { 5, 1, 2, 3},
            { 2, 4, 0, 1}
        };

        const int glu[6][4][4] = {
            { { 0, 2, 1, 3 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 1, 3, 0, 2 }, { 3, 2, 1, 0 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 0, 2, 1, 3 }, { 2, 3, 0, 1 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 0, 2, 3 }, { 3, 2, 1, 0 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } },
            { { 2, 3, 0, 1 }, { 1, 0, 2, 3 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1; topTet[1][0] = 2; topTet[1][1] = 3;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_6_2) {
        const int adj[6][4] = {
            { 1, 5, 3, -1},
            { 0, 5, 4, -1},
            { 3, 4, 5, -1},
            { 2, 4, 0, -1},
            { 5, 1, 2, 3},
            { 2, 4, 1, 0}
        };

        const int glu[6][4][4] = {
            { { 0, 2, 1, 3 }, { 0, 3, 1, 2 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 1, 2, 0, 3 }, { 2, 3, 1, 0 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 0, 2, 1, 3 }, { 2, 3, 0, 1 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 1, 3, 0, 2 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 0, 2, 3 }, { 3, 2, 0, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } },
            { { 2, 3, 0, 1 }, { 1, 0, 2, 3 }, { 2, 0, 1, 3 }, { 0, 2, 3, 1 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1; topTet[1][0] = 2; topTet[1][1] = 3;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_6_3) {
        const int adj[6][4] = {
            { 4, 5, 3, -1},
            { 4, 5, 2, -1},
            { 5, 4, 1, -1},
            { 5, 4, 0, -1},
            { 1, 0, 2, 3},
            { 3, 2, 0, 1}
        };

        const int glu[6][4][4] = {
            { { 1, 3, 2, 0 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 3, 1 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 3, 2, 0 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 3, 1 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 3, 1, 2 }, { 3, 0, 2, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } },
            { { 0, 3, 1, 2 }, { 3, 0, 2, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1; topTet[1][0] = 2; topTet[1][1] = 3;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_6_4) {
        const int adj[6][4] = {
            { 4, 4, 3, -1},
            { 4, 4, 2, -1},
            { 5, 5, 1, -1},
            { 5, 5, 0, -1},
            { 1, 0, 0, 1},
            { 3, 2, 2, 3}
        };

        const int glu[6][4][4] = {
            { { 1, 3, 2, 0 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 3, 1 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 3, 2, 0 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 3, 1 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 3, 1, 2 }, { 3, 0, 2, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } },
            { { 0, 3, 1, 2 }, { 3, 0, 2, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1; topTet[1][0] = 2; topTet[1][1] = 3;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_5_1) {
        const int adj[6][4] = {
            { 3, 4, -1, -1},
            { 3, 4, 5, -1},
            { 3, 4, 5, -1},
            { 2, 1, 5, 0},
            { 2, 1, 5, 0},
            { 4, 3, 2, 1}
        };

        const int glu[6][4][4] = {
            { { 3, 2, 0, 1 }, { 2, 3, 0, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
            { { 1, 0, 2, 3 }, { 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 0, 0, 0, 0 } },
            { { 0, 1, 2, 3 }, { 1, 0, 2, 3 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 1, 2, 3 }, { 1, 0, 2, 3 }, { 3, 2, 1, 0 }, { 2, 3, 1, 0 } },
            { { 1, 0, 2, 3 }, { 0, 1, 2, 3 }, { 3, 2, 0, 1 }, { 2, 3, 0, 1 } },
            { { 2, 3, 1, 0 }, { 3, 2, 1, 0 }, { 0, 1, 2, 3 }, { 0, 1, 3, 2 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 1; topTet[0][1] = 0; topTet[1][0] = 0; topTet[1][1] = 2;
        topVertices[1][0] = NPerm(2, 3);
    }
}

NPluggedIBundle::~NPluggedIBundle() {
    delete core;
    if (plug[0])
        delete plug[0];
    if (plug[1])
        delete plug[1];
}

NPluggedIBundle* NPluggedIBundle::isPluggedIBundle(NTriangulation* tri) {
    // Basic property checks.
    if ((! tri->isClosed()) || tri->isOrientable())
        return 0;
    if (tri->getNumberOfVertices() > 1)
        return 0;
    if (tri->getNumberOfComponents() > 1)
        return 0;
    if (tri->getNumberOfTetrahedra() < 6)
        return 0;

    // We have a 1-vertex 1-component closed non-orientable triangulation
    // with at least six tetrahedra.

    // Hunt for the core thin I-bundle.
    NPluggedIBundle* ans;
    if ((ans = hunt(tri, core_T_3_1)))
        return ans;
    if ((ans = hunt(tri, core_T_3_2)))
        return ans;
    if ((ans = hunt(tri, core_T_5_1)))
        return ans;
    if ((ans = hunt(tri, core_T_6_1)))
        return ans;
    if ((ans = hunt(tri, core_T_6_2)))
        return ans;
    if ((ans = hunt(tri, core_T_6_3)))
        return ans;
    if ((ans = hunt(tri, core_T_6_4)))
        return ans;

    return 0;
}

NPluggedIBundle* NPluggedIBundle::hunt(NTriangulation* tri,
        const NPluggedIBundleCore& core) {
    std::list<NIsomorphism*> isos;
    if (! core.core.findAllSubcomplexesIn(*tri, isos))
        return 0;

    // Run through each isomorphism and look for the corresponding plugs.
    NTorusPlug* plug[2];
    for (std::list<NIsomorphism*>::const_iterator it = isos.begin();
            it != isos.end(); it++) {
        plug[0] = NTorusPlug::isPlugged(
            tri->getTetrahedron((*it)->tetImage(core.topTet[0][0])),
            (*it)->facePerm(core.topTet[0][0]) * core.topVertices[0][0],
            tri->getTetrahedron((*it)->tetImage(core.topTet[0][1])),
            (*it)->facePerm(core.topTet[0][1]) * core.topVertices[0][1]);
        if (! plug[0]) {
            // Delete this isomorphism; we won't need it any more.
            delete *it;
            continue;
        }

        // Do we need a second plug?
        if (core.coreType == NPluggedIBundleCore::T_3_1 ||
                core.coreType == NPluggedIBundleCore::T_3_2) {
            // No second plug.
            plug[1] = 0;
        } else {
            plug[1] = NTorusPlug::isPlugged(
                tri->getTetrahedron((*it)->tetImage(core.topTet[1][0])),
                (*it)->facePerm(core.topTet[1][0]) * core.topVertices[1][0],
                tri->getTetrahedron((*it)->tetImage(core.topTet[1][1])),
                (*it)->facePerm(core.topTet[1][1]) * core.topVertices[1][1]);
            if (! plug[1]) {
                delete plug[0];

                // Delete this isomorphism; we won't need it any more.
                delete *it;
                continue;
            }
        }

        // All good!
        NPluggedIBundle* ans = new NPluggedIBundle();
        ans->core = *it;
        ans->coreType = core.coreType;
        ans->plug[0] = plug[0];
        ans->plug[1] = plug[1];

        // Delete the remaining isomorphisms that we never even
        // looked at.
        for (it++; it != isos.end(); it++)
            delete *it;

        return ans;
    }

    // Nothing found.
    return 0;
}

std::ostream& NPluggedIBundle::writeCommonName(std::ostream& out,
        bool tex) const {
    if (tex) {
        switch(coreType) {
            case NPluggedIBundleCore::T_3_1: out << "$G_{\\tilde{T}_3^1"; break;
            case NPluggedIBundleCore::T_3_2: out << "$G_{\\tilde{T}_3^2"; break;
            case NPluggedIBundleCore::T_6_1: out << "$G_{\\tilde{T}_6^1"; break;
            case NPluggedIBundleCore::T_6_2: out << "$G_{\\tilde{T}_6^2"; break;
            case NPluggedIBundleCore::T_6_3: out << "$G_{\\tilde{T}_6^3"; break;
            case NPluggedIBundleCore::T_6_4: out << "$G_{\\tilde{T}_6^4"; break;
            case NPluggedIBundleCore::T_5_1: out << "$G_{\\tilde{T}_5^1"; break;
        }
    } else {
        switch(coreType) {
            case NPluggedIBundleCore::T_3_1: out << "G(T~3^1"; break;
            case NPluggedIBundleCore::T_3_2: out << "G(T~3^2"; break;
            case NPluggedIBundleCore::T_6_1: out << "G(T~6^1"; break;
            case NPluggedIBundleCore::T_6_2: out << "G(T~6^2"; break;
            case NPluggedIBundleCore::T_6_3: out << "G(T~6^3"; break;
            case NPluggedIBundleCore::T_6_4: out << "G(T~6^4"; break;
            case NPluggedIBundleCore::T_5_1: out << "G(T~5^1"; break;
        }
    }

    // TODO: Normalise parameters.
    // TODO: Also omit parameters where possible.
    // TODO: Also deal gracefully with missing plugs.
    for (int i = 0; i < 2; i++) {
        out << " | ";
        if (plug[i]) {
            if (tex)
                plug[i]->writeTeXName(out);
            else
                plug[i]->writeName(out);
        } else
            out << "---";

        if (coreType == NPluggedIBundleCore::T_3_1 ||
                coreType == NPluggedIBundleCore::T_3_2)
            break;
    }

    return out << (tex ? "}$" : ")");
}

std::ostream& NPluggedIBundle::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

std::ostream& NPluggedIBundle::writeTeXName(std::ostream& out) const {
    return writeCommonName(out, true);
}

void NPluggedIBundle::writeTextLong(std::ostream& out) const {
    out << "Plugged I-bundle: ";
    writeName(out);
}

} // namespace regina
