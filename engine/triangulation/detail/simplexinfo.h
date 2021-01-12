#ifndef __SIMPLEX_INFO_H
#ifndef __DOXYGEN
#define __SIMPLEX_INFO_H
#endif

#include <vector>
#include <algorithm>
#include"triangulation/dim3.h"
#include"triangulation/dim4.h"
#include"triangulation/example3.h"
#include"triangulation/example4.h"
#include"triangulation/detail/triangulation.h"
#include"triangulation/detail/facenumbering.h"

namespace regina {

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
 */
template <int dim>
class SimplexInfo {
    private:
        int label;
        std::vector<std::vector<std::vector<int>>> vertexAnnotations;
        std::vector<std::vector<int>> simplexAnnotations;

        /**
         * Compares two vectors to see which one is ranked higher. The higher
         * ranked vector is defined as the vector which its lowest index value
         * is greater then the other is less then the vector it is comparing
         * against.
         *
         * @param v1 the first vector compared.
         * @param v2 the second vector compared.
         * @return \c true if and only if v2 is ranked higher then v1 otherwise
         * returns false.
         */
        static bool compArr(const std::vector<int>& v1, const std::vector<int>& v2);

        bool compVertex(int i, int j);
        bool compareSimplex(const SimplexInfo & other);
        void addVertexAnnotation(Simplex<dim>* simplex, int size, std::vector<std::vector<int>>& annotations);
        void addSimplexAnnotation(Simplex<dim>* simplex, int size, std::vector<int>& annotations);     
        void init(Simplex<dim>* simplex, int size);      
    public:
        inline int getLabel() {
            return label;
        }

        //Returns all valid permutations 
        std::vector<int> getAllPerms();
        bool operator ==(const SimplexInfo & other);
        bool operator <(SimplexInfo & other);
        SimplexInfo(Simplex<dim>* simplex, int simplexNum, int size);     
};

} // namespace regina

#endif