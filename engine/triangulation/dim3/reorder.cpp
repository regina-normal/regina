
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

#include "triangulation/dim3.h"

#include <iostream>
#include <cstdlib>

namespace regina {

namespace { // Begin anonymous namespace

void reorder_fatal_error(const char* msg) {
    std::cerr << "ERROR: " << msg << std::endl;
    std::exit(1);
}

// Given is a tetrahedron with an ordering inducing edge orientations.
// edge_orientations_on_tet[i] == -1 means that we intend to flip the
// edge orientation of the i-th edge of the tetrahedron.
// perm_from_edges returns the permutation that needs to be applied to the
// tetrahedron to achieve this.

Perm<4> perm_from_edges(const int edge_orientations_on_tet[6]) {
    // p[i] = number of edge_orientations pointing to vertex i
    int p[4] = {0, 0, 0, 0};
    for(int i = 0; i < 6; i++)
        if(edge_orientations_on_tet[i] == +1)
            p[Edge<3>::edgeVertex[i][1]]++;
        else
            p[Edge<3>::edgeVertex[i][0]]++;

    // Consistency check
    for(int i = 0; i < 4; i++) {
        if(p[i] > 3 || p[i] < 0)
            reorder_fatal_error("bad permutation in reorder.cpp");
        for(int j = i+1; j < 4; j++)
            if(p[i] == p[j])
                reorder_fatal_error("bad permutation in reorder.cpp");
    }

    return Perm<4>(p[0],p[1],p[2],p[3]);
}

// edge_orientations[i] is the edge orientation of the i-th edge in
//                                     the triangulation
// edge_orientations_tet[i] is the edge orientation of the i-th edge of
//                                     the tetrahedron
// edge_orientations_tet[i] == +1 means that the edge is oriented pointing
//                                     from the lower to the higher vertex

// writes the result into edge_orientations_tet

void edge_orientations_on_tet(const Triangulation<3> &trig,
                              const std::vector<int> &edge_orientations,
                              const Tetrahedron<3> *tet,
                              int edge_orientations_tet[6]) {
    for(int i = 0; i < 6; i++)
    {
        // to get the edge orientation on a tetrahedron's edge
        // look it up in edge_orientations

        int orientation = edge_orientations[tet->edge(i)->index()];

        // a tetrahedron's edge might be identified with the edge in the
        // triangulation in a way that the edge orientation is not consistent

        Perm<4> perm = tet->edgeMapping(i);
        if(perm[0] > perm[1])
            orientation = - orientation;
        edge_orientations_tet[i] = orientation;
    }
}

// edge_orientations_tet are as above
// v0, v1, v2 are the vertices of the face
// It is assumed that v0 < v1 < v2

inline bool check_consistency_on_face(const int edge_orientations_tet[6],
                                      int v0, int v1, int v2) {

    // There are only two ways to get a cyclic orientation of edges on a face

    if(   (edge_orientations_tet[Edge<3>::edgeNumber[v0][v1]] == +1)
       && (edge_orientations_tet[Edge<3>::edgeNumber[v1][v2]] == +1)
       && (edge_orientations_tet[Edge<3>::edgeNumber[v0][v2]] == -1))
        return false;

    if(   (edge_orientations_tet[Edge<3>::edgeNumber[v0][v1]] == -1)
       && (edge_orientations_tet[Edge<3>::edgeNumber[v1][v2]] == -1)
       && (edge_orientations_tet[Edge<3>::edgeNumber[v0][v2]] == +1))
        return false;

    return true;
}

// edge_orientations[i] == 0 means that the edge_orientation has not been
// assigned yet, so ignore it for testing

// checks that the edge_orientations give an ordering of the triangulation

bool check_consistency_on_tet(const Triangulation<3> &trig,
                              const std::vector<int> &edge_orientations,
                              const Tetrahedron<3> *tet,
                              bool force_oriented)
{
    int edge_orientations_tet[6];

    // compute how the assignment of orientations to edges of the triangulation
    // look on the tetrahedron
    edge_orientations_on_tet(trig,edge_orientations,tet,edge_orientations_tet);

    // check that edge orientations are acyclic on each face of tet

    if(!check_consistency_on_face(edge_orientations_tet,1,2,3))
        return false;
    if(!check_consistency_on_face(edge_orientations_tet,0,2,3))
        return false;
    if(!check_consistency_on_face(edge_orientations_tet,0,1,3))
        return false;
    if(!check_consistency_on_face(edge_orientations_tet,0,1,2))
        return false;

    // if we do not need to check for consistent orientation, we are done:

    if(!force_oriented)
        return true;

    // orientation can't be determined until all edge_orientations are assigned

    for(int i = 0; i < 6; i++)
        if(edge_orientations_tet[i] == 0)
            return true;

    // check for valid orientation

    Perm<4> p = perm_from_edges(edge_orientations_tet);
    if(p.sign() * tet->orientation() == -1)
        return false;

    return true;
}

// checks that the edge orientations give a valid ordering of the triangulation

bool check_consistency_around_edge(const Triangulation<3> &trig,
                                   const std::vector<int> &edge_orientations,
                                   int edge_index,
                                   bool force_oriented)
{
    // iterate through all tetrahedra around an edge

    for(auto& emb : *trig.edge(edge_index))
        if(!check_consistency_on_tet(trig, edge_orientations,
                                     emb.tetrahedron(), force_oriented))
            return false;

    return true;
}

// construct isomorphism from edge orientations

Isomorphism<3>* iso_from_edges(const Triangulation<3> &trig,
                             const std::vector<int> & edge_orientations,
                             bool force_oriented) {

    Isomorphism<3>* iso = new Isomorphism<3>(trig.size());

    // iterate through all tetrahedra

    for(unsigned i = 0; i < trig.size(); i++) {

        // consistency check

        if(!check_consistency_on_tet(trig,
                                     edge_orientations,
                                     trig.tetrahedron(i),
                                     force_oriented))
            reorder_fatal_error("Inconsistent edge orientations in reorder.cpp");

        int edge_orientations_tet[6];

        // compute how the edge orientations look on the tetrahedron

        edge_orientations_on_tet(trig,
                                 edge_orientations,
                                 trig.tetrahedron(i),
                                 edge_orientations_tet);

        // derive the permutation
        iso->facePerm(i) = perm_from_edges(edge_orientations_tet);
        iso->tetImage(i) = i;
    }
    return iso;
}

// Find edge orientations (through back tracking) such that they induce a valid
// ordering on each tetrahedron (and if force_oriented also a consistent
// orientation on each tetrahedron).
// If the function succeeds the isomorphism turning the triangulation into an
// ordered triangulation is returned.

Isomorphism<3>* ordering_iso(const Triangulation<3> &trig, bool force_oriented)
{
    std::vector<int> edge_orientations(trig.countEdges(),0);

    int i = 0;

    while(true) {
        if(i < 0)
            return NULL;

        if(i >= static_cast<int>(trig.countEdges()))
            return iso_from_edges(trig, edge_orientations, force_oriented);

        if(edge_orientations[i] == 0) {
            edge_orientations[i] = +1;
            if(check_consistency_around_edge(trig,edge_orientations,
                                             i,force_oriented))
                ++i;
        }
        else
            if(edge_orientations[i] == +1) {
                edge_orientations[i] = -1;
                if(check_consistency_around_edge(trig,edge_orientations,
                                                 i,force_oriented))
                    ++i;
            }
            else {
                edge_orientations[i] = 0;
                --i;
            }
    }
    return NULL;
}

} // End anonymous namespace

bool Triangulation<3>::isOrdered() const {
    TetrahedronIterator it;

    for(it = simplices_.begin(); it != simplices_.end(); ++it)
        for(int face = 0; face < 4; face++)

            if((*it)->adj_[face]) {
                Perm<4> perm = (*it) -> gluing_[face];

                // check that the permutation is order preserving on the face
                int last = -1;
                for(int k = 0; k < 4; ++k)
                    if( k != face ) {
                        if(perm[k] < last)
                            return false;
                        last = perm[k];
                    }
            }
    return true;
}

bool Triangulation<3>::order(bool force_oriented) {
    ensureSkeleton();

    if(force_oriented && !isOrientable())
        return false;

    // find the isomorphism to order (and orient) the triangulation

    Isomorphism<3>* iso = ordering_iso(*this, force_oriented);
    if(!iso) return false;

    // apply the isomorphism

    iso -> applyInPlace(this);
    delete iso;

    // consistency check

    if(! isOrdered())
        reorder_fatal_error("Triangulation<3>::order returned unordered triangulation in reorder.cpp");
    if(force_oriented && ! isOriented())
        reorder_fatal_error("Triangulation<3>::order returned unoriented triangulation in reorder.cpp");

    return true;
}

}
