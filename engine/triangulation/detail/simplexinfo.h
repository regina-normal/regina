
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

/*! \file triangulation/detail/simplexinfo.h
 *  \brief Header file for utility class in fast isomorphism signature 
 * computation.
 */

#ifndef __SIMPLEX_INFO_H
#ifndef __DOXYGEN
#define __SIMPLEX_INFO_H
#endif

#include <vector>
#include <algorithm>

namespace regina {
namespace detail {
#ifndef __DOXYGEN
/**
 * Internal class that stores combinatorial information of a single simplex
 * in a triangulation and utilises it for comparisons.
 *
 * This class is used with isomorphism signature computations and is used
 * to extract information from a single simplex in a triangulation. For a 
 * <i>dim</i>-dimensional, this class stores annotated information per
 * vertex about the corresponding degrees of faces incident to that vertex
 * for every subdimension. It also stores information about subdimensional
 * face degrees for a simplex to distinguish itself from other simplices
 * in a triangulation. This information is stored in the same way regardless
 * of the orientation of the simplex.
 * 
 * \tparam dim the dimension of the underlying triangulation.
 */
template <int dim>
class SimplexInfo {
    private:
        int label;
        std::vector<std::vector<std::vector<int>>> vertexAnnotations;
        std::vector<std::vector<int>> simplexAnnotations;

        /**
         * Compares two vertices to check which one is ranked higher. The higher
         * ranked vertex is the vertex with higher ranked vector in vertexAnnotations
         * with lower subdimensional faces. 
         *
         * @param i the label of the first vertex compared.
         * @param j the label of the second vertex compared.
         * @return \c true if and only if vertex j is ranked higher then vertex i otherwise
         * returns false.
         */
        bool compVertex(int i, int j);

        /**
         * Calling addVertexAnnotation with <i>subdim<i> adds vertex annotations for all vertices
         * for the dimension <i>subdim<i>. Vertex annotations of a specific subdimension for
         * a specific vertex are the degrees of all faces containing that vertex of that 
         * dimension in sorted format.
         * 
         * \tparam subdim the dimension of the faces to utilise in comparison
         * 
         * @param simplex The simplex used for annotations
         * @param size The size of the original triangulation
         * @param annotations The annotations for that specific subdimension
         */    
        template <int subdim>
        void addVertexAnnotation(Simplex<dim>* simplex, int size, std::vector<std::vector<int>>& annotations);

        /**
         * Calling addSimplexAnnotation with <i>subdim<i> adds simplex annotations for the 
         * dimension <i>subdim<i>. Simplex annotations of a specific subdimension are
         * a vector of the degrees of all faces of that dimension.
         * 
         * \tparam subdim the dimension of the faces to utilise in comparison
         * \tparam numbering the face dimesnion value to add
         * 
         * @param simplex The simplex used for annotations
         * @param size The size of the original triangulation
         * @param annotations The annotations for that specific subdimension
         */         
        template <int subdim>
        void addSimplexAnnotation(Simplex<dim>* simplex, int size, std::vector<int>& annotations);   

        /**
         * A helper function for the constructor of the class <b>SimplexInfo<b>. 
         * Adds vertex and simplex annotations for all possible subdimensions for
         * the given simplex and stores for usage in comparisons.
         *
         * \tparam subdim the dimension of the faces to utilise in comparison
         * 
         * @param simplex The simplex used for annotations
         * @param size The size of the original triangulation
         */          
        template <int subdim = 0>
        void init(std::true_type, Simplex<dim>* simplex, int size);      

        template <int subdim>
        void init(std::false_type, Simplex<dim>* simplex, int size) {}   
    public:
        /**
         * Gets the original label of a simplex in a triangulation before
         * ranking it.
         *
         * @return The original label of the simplex in a triangulation
         */    
        inline int getLabel() {
            return label;
        }

        void debug_print() const {
            for(int subdim = 0; subdim < simplexAnnotations.size(); subdim++) {
                std::cout << subdim << std::endl;
                for (int i = 0; i < simplexAnnotations[subdim].size(); i++) {
                    std::cout << simplexAnnotations[subdim][i] << " ";
                }
                std::cout << std::endl;
            }            
        }

        /**
         * Gets all permutations of vertices for a given simplex where each
         * vertex label in the comparison is ranked equal or higher then
         * the previous vertex label in the permutation. This function can be utilised
         * to reduce the total number of permutations necessary to process.
         *
         * @return A vector of all such permutations
         */
        std::vector<int> getAllPerms();

        /**
         * Compares two simplices and checks if they are equal.
         *
         * @return \c true if and only if all combinatorial data of both
         * simplices are the same. Otherwise returns false.
         */
        bool operator ==(const SimplexInfo<dim> & other) const;

        /**
         * Compares two simplices to check which one is ranked higher. The higher
         * ranked simplex is simplex with the higher ranked simplexAnnotations in 
         * the lower subdimensional faces.
         * 
         * @param other the simplex compared against
         * @return \c true if and only if the current simplex is ranked higher than
         * the other simplex
         */      
        bool operator <(const SimplexInfo<dim> & other) const;

        /**
         * Preprocesses combinatorial data for usage in further operations.
         * Constructor for the class.
         * 
         * Adds vertex and simplex annotations for all subdimensions of
         * the given simplex and stores for usage in comparisons.
         *
         * @param simplex The simplex used for annotations
         * @param size The size of the original triangulation
         * @param simplexNum The original label of the simplex in a triangulation
         */
        SimplexInfo(Simplex<dim>* simplex, int simplexNum, int size);     
};
#endif
} } // namespace regina::detail

#endif