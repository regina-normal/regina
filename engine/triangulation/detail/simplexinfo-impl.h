
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file triangulation/detail/simplexinfo-impl.h
 *  \brief Implementation for utility class in fast isomorphism signature 
 * computation.
 */

#ifndef __SIMPLEX_INFO_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __SIMPLEX_INFO_IMPL_H_DETAIL
#endif

#include <vector>
#include <algorithm>
#include"triangulation/dim3.h"
#include"triangulation/dim4.h"
#include"triangulation/example3.h"
#include"triangulation/example4.h"
#include"triangulation/detail/triangulation.h"
#include"triangulation/detail/facenumbering.h"
#include "simplexinfo.h"

namespace regina {

static bool SimplexInfo::compArr(const std::vector<int>& v1, const std::vector<int>& v2) {
    /**
     * Iterates through the vectors to determine the higher ranked vector
     */
    for (int i = 0; i < v1.size(); i++) {
        if (v1[i] > v2[i]) {
            return false;
        } else if (v1[i] < v2[i]) {
            return true;
        }
    }     
    /**
     * Returns true if both are equal
     */       
    return true;
}

bool SimplexInfo::compVertex(int i, int j) {
    /**
     * Iterates through dimensions to check for the higher ranked
     * vertex overall
     */    
    for (int k = 0; k < vertexAnnotations.size(); k++) {
        if (vertexAnnotations[k][i] != vertexAnnotations[k][j]) {
            return compArr(vertexAnnotations[k][i], vertexAnnotations[k][j]);
        }
    }
    /**
     * Returns true if both are equal
     */     
    return true;
}

template <int dim, int subdim>
bool SimplexInfo::compareSimplex(const SimplexInfo & other) {
    /**
     * Compares all simplexAnnotations from lowest subdimension
     * up to the (dim + 1) / 2
     */  
    if constexpr (subdim < (dim + 1) / 2) {
        if (simplexAnnotations[subdim] == other.simplexAnnotations[subdim]) {
            return compareSimplex<subdim + 1>(other);
        } else {
            return compArr(simplexAnnotations[subdim], other.simplexAnnotations[subdim]);
        }
    }
    /**
     * Simplices are equal
     */  
    return true; 
} 

template <int dim, int subdim, int numbering = 0, int vertexCount = 0>
void SimplexInfo::addVertexAnnotation(Simplex<dim>* simplex, int size, std::vector<std::vector<int>>& annotations) {
    /**
     * Recusively goes through all faces of a specific subdimension, and adds
     * it as a vertex annotation
     */      
    if constexpr (numbering < FaceNumbering<dim, subdim>::nFaces) {
        if constexpr (vertexCount <= subdim) {
            int vertexNumber = FaceNumbering<dim, subdim>::ordering(numbering)[vertexCount];
            annotations[vertexNumber].emplace_back(simplex->template face<subdim>(numbering)->degree());
            addVertexAnnotation<subdim, numbering + 1, vertexCount + 1>(simplex, size, annotations);
        } else {
            addVertexAnnotation<subdim, numbering + 1>(simplex, size, annotations);
        }
    }
}

template <int dim, int subdim, int numbering = 0>
void SimplexInfo::addSimplexAnnotation(Simplex<dim>* simplex, int size, std::vector<int>& annotations) {
    /**
     * Recusively goes through all faces of a specific subdimension, and adds
     * its simplex annotations. This information is combined with its opposite face
     * of dimension <i>dim - subdim - 1<i> which consist of the <i>dim - subdim<i> 
     * the face doesn't contain.
     */       
    if constexpr (numbering < FaceNumbering<dim, subdim>::nFaces) {
        int first = simplex->template face<subdim>(numbering)->degree();
        int second = simplex->template face<dim - subdim - 1>(numbering)->degree();   
        /**      
         * Note that this annotation ideally is unique for each (first, second)
         * pair. If not, some distinguishing power can be lost but overall
         * the methods are still valid. 
        */
        if (subdim == dim - subdim - 1) { //Unordered Pair
            int lo = std::min(first, second);
            int hi = std::max(first, second);
            annotations[numbering] = lo * size + hi;
        } else { //Ordered Pair
            annotations[numbering] = first * size + second;
        }
        addSimplexAnnotation<subdim, numbering + 1>(simplex, size, annotations);
    }
}   

template <int dim, int subdim = 0>
void SimplexInfo::init(Simplex<dim>* simplex, int size) {
    /**
     * Adds simplex annotations for subdim-faces
     */     
    std::vector<int> subdimSimplexAnnotations(FaceNumbering<dim, subdim>::nFaces);
    addSimplexAnnotation<subdim>(simplex, size, subdimSimplexAnnotations);
    simplexAnnotations.emplace_back(subdimSimplexAnnotations);
    /**
     * Adds vertex annotations for subdim-faces
     */     
    std::vector<std::vector<int>> subdimVertexAnnotations(dim + 1, std::vector<int>());
    addVertexAnnotation<subdim>(simplex, size, subdimVertexAnnotations);
    vertexAnnotations.emplace_back(subdimVertexAnnotations);
    /**
     * Sorts the annotations so the result does not depend on the input simplex's
     * specific permutation in the input.
     */    
    std::sort(simplexAnnotations[subdim].begin(), simplexAnnotations[subdim].end());
    if constexpr (subdim + 1 < (dim + 1) / 2) {
        init<subdim + 1>(simplex, size);
    } 
} 

template <int dim>
std::vector<int> SimplexInfo::getAllPerms() {
    std::vector<int> ans;
    for (int perm = 0; perm < Perm<dim + 1>::nPerms; ++perm) {
        /**
         * Checks that each vertices are ranked in a weakly ascending manner
         */
        for (int i = 1; i < dim + 1; i++) {
            if(!compVertex(Perm<dim + 1>::atIndex(perm)[i - 1], Perm<dim + 1>::atIndex(perm)[i])) {
                break;
            } 
        }
        ans.push_back(perm);
    }
    return ans;
} 

bool SimplexInfo::operator ==(const SimplexInfo & other) {
    for (int i = 0; i < simplexAnnotations.size(); i++) {
        if (simplexAnnotations[i] != other.simplexAnnotations[i]) {
            return false;
        }
    }
    return true;
}

bool SimplexInfo::operator <(SimplexInfo & other) {
    return compareSimplex<0>(other);
}

SimplexInfo::SimplexInfo(Simplex<dim>* simplex, int simplexNum, int size) {
    label = simplexNum;
    init(simplex, size);
}

}// namespace regina

#endif