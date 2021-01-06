#ifndef SIMP_INFO_H
#define SIMP_INFO_H

#include <vector>
#include <algorithm>
#include <iostream>

#include"triangulation/dim3.h"
#include"triangulation/dim4.h"
#include"triangulation/example3.h"
#include"triangulation/example4.h"
#include"triangulation/detail/triangulation.h"
#include"triangulation/detail/facenumbering.h"

using namespace regina;

template <int dim>
class SimplexInfo {
    private:
        int label;
        std::vector<std::vector<std::vector<int>>> vertexAnnotations;
        std::vector<std::vector<int>> simplexAnnotations;

        // <= function for sorted vectors
        static bool compArr(const std::vector<int>& v1, const std::vector<int>& v2) {
            if (v1.size() !=  v2.size()) {
                return true;
            }
            for (int i = 0; i < v1.size(); i++) {
                if (v1[i] > v2[i]) {
                    return false;
                } else if (v1[i] < v2[i]) {
                    return true;
                }
            }            
            return true;
        }

        //A <= rank function for a vertex ordering
        bool compVertex(int i, int j) {
            for (int k = 0; k < vertexAnnotations.size(); k++) {
                if (vertexAnnotations[k][i] != vertexAnnotations[k][j]) {
                    return compArr(vertexAnnotations[k][i], vertexAnnotations[k][j]);
                }
            }
            return true;
        }
        
    public:
        int getLabel() {
            return label;
        }

        //Returns all valid permutations 
        std::vector<int> getAllPerms() {
            std::vector<int> ans;
            for (int perm = 0; perm < Perm<dim + 1>::nPerms; ++perm) {
                //Checks that each vertices are ranked in a weakly ascending manner
                for (int i = 1; i < dim + 1; i++) {
                    if(!compVertex(Perm<dim + 1>::atIndex(perm)[i - 1], Perm<dim + 1>::atIndex(perm)[i])) {
                        break;
                    } 
                }
                ans.push_back(perm);
            }
            return ans;
        }

        template <int subdim>
        bool compareSimplex(const SimplexInfo & other) {
            if constexpr (subdim < (dim + 1) / 2) {
                if (simplexAnnotations[subdim] == other.simplexAnnotations[subdim]) {
                    return compareSimplex<subdim + 1>(other);
                } else {
                    return compArr(simplexAnnotations[subdim], other.simplexAnnotations[subdim]);
                }
            }
            //Everything is equal 
            return true; 
        }     

        template <int subdim, int numbering = 0, int vertexCount = 0>
        void addVertexAnnotation(Simplex<dim>* simplex, int size, std::vector<std::vector<int>>& annotations) {
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

        template <int subdim, int numbering = 0>
        void addSimplexAnnotation(Simplex<dim>* simplex, int size, std::vector<int>& annotations) {
            if constexpr (numbering < FaceNumbering<dim, subdim>::nFaces) {
                int first = simplex->template face<subdim>(numbering)->degree();
                int second = simplex->template face<dim - subdim - 1>(numbering)->degree();            
                //Note that this annotation ideally is unique for each (first, second)
                //pair. If not, some distinguishing power can be lost but overall
                //the methods are still valid. 
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

        template <int subdim = 0>
        void init(Simplex<dim>* simplex, int size) {
            //Add annotation for subdim-faces
            std::vector<int> subdimSimplexAnnotations(FaceNumbering<dim, subdim>::nFaces);
            addSimplexAnnotation<subdim>(simplex, size, subdimSimplexAnnotations);
            simplexAnnotations.emplace_back(subdimSimplexAnnotations);
            //Add annotation per vertex
            std::vector<std::vector<int>> subdimVertexAnnotations(dim + 1, std::vector<int>());
            addVertexAnnotation<subdim>(simplex, size, subdimVertexAnnotations);
            vertexAnnotations.emplace_back(subdimVertexAnnotations);
            std::sort(simplexAnnotations[subdim].begin(), simplexAnnotations[subdim].end());
            if constexpr (subdim + 1 < (dim + 1) / 2) {
                init<subdim + 1>(simplex, size);
            } 
        }       

        SimplexInfo(Simplex<dim>* simplex, int simpNum, int size) {
            label = simpNum;
            //Performs initialisation starting from subdim=0 data upwards
            init(simplex, size);
        }

        bool operator <(SimplexInfo & other) {
            return compareSimplex<0>(other);
        }

        bool operator ==(const SimplexInfo & other) {
            for (int i = 0; i < simplexAnnotations.size(); i++) {
                if (simplexAnnotations[i] != other.simplexAnnotations[i]) {
                    return false;
                }
            }
            return true;
        }
};

#endif