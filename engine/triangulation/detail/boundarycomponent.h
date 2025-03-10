
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#ifndef __REGINA_BOUNDARYCOMPONENT_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_BOUNDARYCOMPONENT_H_DETAIL
#endif

/*! \file triangulation/detail/boundarycomponent.h
 *  \brief Implementation details for boundary components of triangulations.
 */

#include <tuple>
#include <vector>
#include "regina-core.h"
#include "core/output.h"
#include "triangulation/detail/strings.h"
#include "triangulation/forward.h"
#include "utilities/listview.h"
#include "utilities/markedvector.h"
#include "utilities/typeutils.h"

namespace regina::detail {

template <int> class TriangulationBase;

/**
 * Helper class that provides core functionality for a boundary component
 * of a <i>dim</i>-dimensional triangulation.
 *
 * Each boundary component is represented by the class BoundaryComponent<dim>,
 * which uses this as a base class.  End users should not need to refer to
 * BoundaryComponentBase directly.
 *
 * See the BoundaryComponent class notes for further information.
 *
 * Neither this class nor the "end user" class BoundaryComponent<dim> support
 * value semantics: they cannot be copied, swapped, or manually constructed.
 * Their memory is managed by the Triangulation class, and their locations
 * in memory define them.  See BoundaryComponent<dim> for further details.
 *
 * \python This base class is not present, but the "end user"
 * class BoundaryComponent<dim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup detail
 */
template <int dim>
class BoundaryComponentBase :
        public ShortOutput<BoundaryComponentBase<dim>>, public MarkedElement {
    public:
        static constexpr int dimension = dim;
            /**< A compile-time constant that gives the dimension of the
                 triangulation that contains this boundary component. */

        /**
         * A compile-time constant indicating whether ideal and/or
         * invalid vertex boundary components are both possible and
         * recognised by this boundary component class.
         */
        static constexpr bool allowVertex = standardDim(dim) && (dim > 2);
        /**
         * A compile-time constant indicating whether this boundary
         * component class supports triangulating boundary components.
         */
        static constexpr bool canBuild = (dim > 2);
        /**
         * A compile-time constant indicating whether this boundary
         * component class stores all lower-dimensional faces (\c true),
         * or only faces of dimension <i>dim</i>-1 (\c false).
         */
        static constexpr bool allFaces = standardDim(dim);

    private:
        /**
         * The sequence of dimensions of all faces that we store.
         */
        using subdimensions = std::conditional_t<allFaces,
            std::make_integer_sequence<int, dim> /* 0,...,(dim-1) */,
            std::integer_sequence<int, dim - 1> /* (dim-1) only */>;

        /**
         * A non-existent function used to construct the type of the \a faces_
         * tuple.  Essentially, this lets us pull apart the integer pack
         * \a subdimensions.  The return type is the tuple type that we want.
         */
        template <int... subdim>
        static auto seqToFaces(std::integer_sequence<int, subdim...>) ->
            std::tuple<std::vector<Face<dim, subdim>*>...>;

        /**
         * A tuple of arrays of faces of this boundary component.
         * If allFaces is true, then std::get<k>(faces_)[i] is a pointer to
         * the ith k-face of the boundary component.
         * If allFaces is false, then the tuple has only one element, and
         * std::get<0>(faces_)[i] is a pointer to the ith (dim-1)-face of the
         * boundary component.
         */
        decltype(seqToFaces(subdimensions())) faces_;

        /**
         * A compile-time constant function that returns the index of
         * the faces_ tuple corresponding to the facial dimension \a subdim.
         * Returns -1 if faces of dimension \a subdim are not stored.
         */
        static constexpr int tupleIndex(int subdim) {
            if constexpr (allFaces) {
                return (subdim >= 0 && subdim < dim ? subdim : -1);
            } else {
                return (subdim == dim - 1 ? 0 : -1);
            }
        }
        /**
         * A compile-time constant giving an index of the faces_ tuple
         * whose corresponding face list is guaranteed to be non-empty.
         *
         * If allFaces is true, we use the vertex list since every
         * boundary component (real, ideal or invalid) has a vertex.
         * If allFaces is false, we use the (dim-1)-face list since only
         * real boundaries are recognised in this case.
         * Either way, the tuple index we need works out to be 0.
         */
        static constexpr int nonEmptyIndex_ = 0;

        /**
         * The number of (dim-2)-faces in the boundary component.  We only
         * store this if allFaces is false (i.e., we are not storing the
         * (dim-2)-faces themselves).
         */
        EnableIf<! allFaces, size_t, 0> nRidges_;

    protected:
        bool orientable_;
            /**< Is this boundary component orientable? */

        EnableIf<canBuild, Triangulation<dim-1>*, nullptr> boundary_;
            /**< A full triangulation of the boundary component.
                 This may be pre-computed when the triangulation skeleton
                 is constructed, or it may be \c null in which case it
                 will be built on demand.
                 For ideal or invalid vertices, this is always \c null since
                 the triangulation is cached by the vertex class instead.
                 We keep this as pointer to avoid issues with cascading
                 template instantiations (this allows us to work with
                 forward declarations of lower-dimensional types instead). */

    public:
        /**
         * Destroys this object.  The cached boundary component triangulation
         * will be destroyed also.
         */
        ~BoundaryComponentBase();

        /**
         * Returns the index of this boundary component in the underlying
         * triangulation.
         *
         * \return the index of this boundary component.
         */
        size_t index() const {
            return markedIndex();
        }

        /**
         * Returns the number of (<i>dim</i>-1)-faces in this boundary
         * component.  These are the top-dimensional faces for a real
         * boundary component.
         *
         * If this is an ideal or invalid vertex boundary component,
         * then this routine will return 0.
         *
         * \return the number of (<i>dim</i>-1)-faces in this boundary
         * component.
         */
        size_t size() const {
            return std::get<tupleIndex(dim-1)>(faces_).size();
        }

        /**
         * Returns the number of (<i>dim</i>-2)-faces in this boundary
         * component.
         *
         * If this is an ideal or invalid vertex boundary component,
         * then this routine will return 0.
         *
         * \return the number of (<i>dim</i>-2)-faces in this boundary
         * component.
         */
        size_t countRidges() const {
            if constexpr (allFaces)
                return std::get<tupleIndex(dim-2)>(faces_).size();
            else
                return nRidges_.value;
        }

        /**
         * Returns the number of <i>subdim</i>-faces in this boundary component.
         *
         * \python Python does not support templates.  Instead,
         * Python users should call this function in the form
         * `countFaces(subdim)`; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * \tparam subdim the dimension of the faces to query.  If \a dim is
         * one of Regina's \ref stddim "standard dimensions", then \a subdim
         * must be between 0 and <i>dim</i>-1 inclusive.  Otherwise, the only
         * allowable values of \a subdim are the facet dimension (<i>dim</i>-1)
         * and the ridge dimension (<i>dim</i>-2).
         *
         * \return the number of <i>subdim</i>-faces.
         */
        template <int subdim>
        size_t countFaces() const {
            if constexpr (allFaces) {
                static_assert(subdim >= 0 && subdim < dim,
                    "BoundaryComponent::countFaces() cannot be used with "
                    "this (dim, subdim) combination.");
                return std::get<tupleIndex(subdim)>(faces_).size();
            } else {
                if constexpr (subdim == dim - 2) {
                    return nRidges_.value;
                } else {
                    static_assert(subdim == dim - 1,
                        "BoundaryComponent::countFaces() cannot be used with "
                        "this (dim, subdim) combination.");
                    return std::get<tupleIndex(subdim)>(faces_).size();
                }
            }
        }

        /**
         * A dimension-specific alias for countFaces<0>().
         *
         * This alias is available only when \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * See countFaces() for further information.
         */
        size_t countVertices() const {
            static_assert(standardDim(dim), "countVertices() is only available "
                "for boundary components in standard dimensions.");
            return countFaces<0>();
        }

        /**
         * A dimension-specific alias for countFaces<1>().
         *
         * This alias is available only when \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * See countFaces() for further information.
         */
        size_t countEdges() const {
            static_assert(standardDim(dim), "countEdges() is only available "
                "for boundary components in standard dimensions.");
            return countFaces<1>();
        }

        /**
         * A dimension-specific alias for countFaces<2>().
         *
         * This alias is available only when \a dim is one of Regina's
         * \ref stddim "standard dimensions" and \a dim ≥ 3.
         *
         * See countFaces() for further information.
         */
        size_t countTriangles() const {
            static_assert(standardDim(dim) && dim >= 3,
                "countTriangles() is only available "
                "for boundary components in standard dimensions dim >= 3.");
            return countFaces<2>();
        }

        /**
         * A dimension-specific alias for countFaces<3>().
         *
         * This alias is only available for dimensions \a dim = 4 and 5.
         *
         * See countFaces() for further information.
         */
        size_t countTetrahedra() const {
            static_assert((standardDim(dim) && dim >= 4) || dim == 5,
                "countTetrahedra() is only available for boundary components "
                "in dimensions dim = 4 or 5.");
            return countFaces<3>();
        }

        /**
         * A dimension-specific alias for countFaces<4>().
         *
         * This alias is only available for dimensions \a dim = 5 and 6.
         *
         * See countFaces() for further information.
         */
        size_t countPentachora() const {
            static_assert(dim == 5 || dim == 6, "countPentachora() is only "
                "available for boundary components in dimensions "
                "dim = 5 or 6.");
            return countFaces<4>();
        }

        /**
         * Returns an object that allows iteration through and random access
         * to all (<i>dim</i>-1)-faces in this boundary component.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * range-based \c for loops.  Note that the elements of the list
         * will be pointers, so your code might look like:
         *
         * \code{.cpp}
         * for (Face<dim, dim-1>* f : bc.facets()) { ... }
         * \endcode
         *
         * The object that is returned will remain valid only for as
         * long as this boundary component object exists.  In particular,
         * the object will become invalid any time that the triangulation
         * changes (since all boundary component objects will be destroyed
         * and others rebuilt in their place).
         * Therefore it is best to treat this object as temporary only,
         * and to call facets() again each time you need it.
         *
         * \return access to the list of all (<i>dim</i>-1)-faces.
         */
        auto facets() const {
            return ListView(std::get<tupleIndex(dim-1)>(faces_));
        }

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces in this boundary component.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * range-based \c for loops.  Note that the elements of the list
         * will be pointers, so your code might look like:
         *
         * \code{.cpp}
         * for (Face<dim, subdim>* f : bc.faces<subdim>()) { ... }
         * \endcode
         *
         * The object that is returned will remain valid only for as
         * long as this boundary component object exists.  In particular,
         * the object will become invalid any time that the triangulation
         * changes (since all boundary component objects will be destroyed
         * and others rebuilt in their place).
         * Therefore it is best to treat this object as temporary only,
         * and to call faces() again each time you need it.
         *
         * \python Python does not support templates.  Instead,
         * Python users should call this function in the form
         * `faces(subdim)`.
         *
         * \tparam subdim the dimension of the faces to query.  If \a dim is
         * one of Regina's \ref stddim "standard dimensions", then \a subdim
         * must be between 0 and <i>dim</i>-1 inclusive.  Otherwise, the only
         * allowable value of \a subdim is the facet dimension (<i>dim</i>-1).
         *
         * \return access to the list of all <i>subdim</i>-faces.
         */
        template <int subdim>
        auto faces() const {
            static_assert(tupleIndex(subdim) >= 0,
                "BoundaryComponent::faces() cannot be used with this "
                "(dim, subdim) combination.");
            return ListView(std::get<tupleIndex(subdim)>(faces_));
        }

        /**
         * A dimension-specific alias for faces<0>().
         *
         * This alias is available only when \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * See faces() for further information.
         */
        auto vertices() const {
            static_assert(standardDim(dim), "vertices() is only available "
                "for boundary components in standard dimensions.");
            return ListView(std::get<tupleIndex(0)>(faces_));
        }

        /**
         * A dimension-specific alias for faces<1>().
         *
         * This alias is available only when \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * See faces() for further information.
         */
        auto edges() const {
            static_assert(standardDim(dim), "edges() is only available "
                "for boundary components in standard dimensions.");
            return ListView(std::get<tupleIndex(1)>(faces_));
        }

        /**
         * A dimension-specific alias for faces<2>().
         *
         * This alias is available only when \a dim is one of Regina's
         * \ref stddim "standard dimensions" and \a dim ≥ 3.
         *
         * See faces() for further information.
         */
        auto triangles() const {
            static_assert(standardDim(dim) && dim >= 3,
                "triangles() is only available "
                "for boundary components in standard dimensions dim >= 3.");
            return ListView(std::get<tupleIndex(2)>(faces_));
        }

        /**
         * A dimension-specific alias for faces<3>().
         *
         * This alias is only available for dimension \a dim = 4.
         *
         * See faces() for further information.
         */
        auto tetrahedra() const {
            static_assert(standardDim(dim) && dim >= 4,
                "tetrahedra() is only available for "
                "boundary components in dimension dim = 4.");
            return ListView(std::get<tupleIndex(3)>(faces_));
        }

        /**
         * A dimension-specific alias for faces<4>().
         *
         * This alias is only available for dimension \a dim = 5.
         *
         * See faces() for further information.
         */
        auto pentachora() const {
            static_assert(dim == 5, "pentachora() is only available for "
                "boundary components in dimension dim = 5.");
            return ListView(std::get<tupleIndex(4)>(faces_));
        }

        /**
         * Returns the requested (<i>dim</i>-1)-face in this boundary component.
         * These are the top-dimensional faces for a real boundary component.
         *
         * Note that the index of a face in the boundary component need
         * not be the index of the same face in the overall triangulation.
         * However, if this is a real boundary component (i.e., it is built
         * from one or more (<i>dim</i>-1)-faces), then the index of each
         * (<i>dim</i>-1)-face in this boundary component will match the
         * index of the corresponding top-dimensional simplex in the
         * (<i>dim</i>-1)-manifold triangulation returned by build().
         *
         * \param index the index of the desired face, ranging from 0 to
         * size()-1 inclusive.
         * \return the requested face.
         */
        Face<dim, dim-1>* facet(size_t index) const {
            return std::get<tupleIndex(dim-1)>(faces_)[index];
        }

        /**
         * Returns the requested <i>subdim</i>-face in this boundary component.
         *
         * Note that the index of a face in the boundary component need
         * not be the index of the same face in the overall triangulation.
         * However, if this is a real boundary component (i.e., it is built
         * from one or more (<i>dim</i>-1)-faces), then the index of each
         * <i>subdim</i>-face in this boundary component will match the
         * index of the corresponding <i>subdim</i>-face in the
         * (<i>dim</i>-1)-manifold triangulation returned by build().
         *
         * \python Python does not support templates.  Instead,
         * Python users should call this function in the form
         * `face(subdim, index)`; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * \tparam subdim the dimension of the faces to query.  If \a dim is
         * one of Regina's \ref stddim "standard dimensions", then \a subdim
         * must be between 0 and <i>dim</i>-1 inclusive.  Otherwise, the only
         * allowable value of \a subdim is the facet dimension (<i>dim</i>-1).
         *
         * \param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * \return the requested face.
         */
        template <int subdim>
        Face<dim, subdim>* face(size_t index) const {
            static_assert(tupleIndex(subdim) >= 0,
                "BoundaryComponent::face() cannot be used with this "
                "(dim, subdim) combination.");
            return std::get<tupleIndex(subdim)>(faces_)[index];
        }

        /**
         * A dimension-specific alias for face<0>().
         *
         * This alias is available only when \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * See face() for further information.
         */
        Face<dim, 0>* vertex(size_t index) const {
            static_assert(standardDim(dim), "vertex() is only available "
                "for boundary components in standard dimensions.");
            return std::get<tupleIndex(0)>(faces_)[index];
        }

        /**
         * A dimension-specific alias for face<1>().
         *
         * This alias is available only when \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * See face() for further information.
         */
        Face<dim, 1>* edge(size_t index) const {
            static_assert(standardDim(dim), "edge() is only available "
                "for boundary components in standard dimensions.");
            return std::get<tupleIndex(1)>(faces_)[index];
        }

        /**
         * A dimension-specific alias for face<2>().
         *
         * This alias is available only when \a dim is one of Regina's
         * \ref stddim "standard dimensions" and \a dim ≥ 3.
         *
         * See face() for further information.
         */
        Face<dim, 2>* triangle(size_t index) const {
            static_assert(standardDim(dim) && dim >= 3,
                "triangle() is only available "
                "for boundary components in standard dimensions dim >= 3.");
            return std::get<tupleIndex(2)>(faces_)[index];
        }

        /**
         * A dimension-specific alias for face<3>().
         *
         * This alias is only available for dimension \a dim = 4.
         *
         * See face() for further information.
         */
        Face<dim, 3>* tetrahedron(size_t index) const {
            static_assert(standardDim(dim) && dim >= 4,
                "tetrahedron() is only available for "
                "boundary components in dimension dim = 4.");
            return std::get<tupleIndex(3)>(faces_)[index];
        }

        /**
         * A dimension-specific alias for face<4>().
         *
         * This alias is only available for dimension \a dim = 5.
         *
         * See face() for further information.
         */
        Face<dim, 4>* pentachoron(size_t index) const {
            static_assert(dim == 5, "pentachoron() is only available for "
                "boundary components in dimension dim = 5.");
            return std::get<tupleIndex(4)>(faces_)[index];
        }

        /**
         * Returns the triangulation to which this boundary component belongs.
         *
         * \return a reference to the triangulation containing this boundary
         * component.
         */
        Triangulation<dim>& triangulation() const {
            // If allFaces is true, then there may be no (dim-1)-simplices,
            // but there is always a vertex.
            // If allFaces is false, then we are guaranteed at least one
            // (dim-1)-simplex.
            // Either way, the array at tuple index 0 should be non-empty/
            return std::get<nonEmptyIndex_>(faces_).front()->triangulation();
        }

        /**
         * Returns the connected component of the triangulation to which this
         * boundary component belongs.
         *
         * \return the component containing this boundary component.
         */
        Component<dim>* component() const {
            // If allFaces is true, then there may be no (dim-1)-simplices,
            // but there is always a vertex.
            // If allFaces is false, then we are guaranteed at least one
            // (dim-1)-simplex.
            // Either way, the array at tuple index 0 should be non-empty/
            return std::get<nonEmptyIndex_>(faces_).front()->component();
        }

        /**
         * Determines if this boundary component is orientable.  If this is
         * an ideal or invalid vertex boundary component, then the
         * orientability of the corresponding vertex link is returned.
         *
         * This routine is fast; in particular, it is pre-computed and
         * does not build a full triangulation of the boundary component.
         *
         * \return \c true if and only if this boundary component is orientable.
         */
        bool isOrientable() const {
            return orientable_;
        }

        /**
         * Returns the Euler characteristic of this boundary component.  If
         * the boundary component consists of a single vertex and nothing else
         * (e.g., it is an ideal vertex), then the Euler characteristic of
         * the vertex link will be returned.
         *
         * This function is, in all "normal" cases, equivalent to
         * triangulating the boundary component via build() and then calling
         * Triangulation<dim-1>::eulerCharTri() on the result.
         *
         * The exception comes from triangulations with "pinched" faces
         * whose links have multiple boundary components (e.g., a vertex
         * whose link is a multiply-punctured sphere, marking a point where
         * different parts of the boundary are "pinched together").
         * If there are such faces, then this routine will return a
         * well-defined but topologically meaningless result.  Essentially,
         * this routine only counts such faces once, even though they "should"
         * be counted multiple times on the boundary since they can be "seen"
         * from distinct sections of the (<i>dim</i>-1)-dimensional boundary.
         * Of course such a triangulation cannot represent a
         * <i>dim</i>-manifold anyway, and so if you do have pinched faces
         * then you almost certainly have bigger problems to deal with.
         *
         * \pre \a dim is one of Regina's \ref stddim "standard dimensions".
         *
         * \warning If this boundary component itself forms an ideal
         * (<i>dim</i>-1)-dimensional triangulation, then again this result
         * is well-defined but topologically meaningless (since it is
         * equivalent to calling eulerCharTri() and not eulerCharManifold()
         * on the triangulated boundary).
         * However, again such boundary components cannot appear in a
         * <i>dim</i>-manifold, and so if you have such boundary components
         * then you almost certainly have bigger problems than this.
         *
         * \return the Euler characteristic of this boundary component.
         */
        long eulerChar() const {
            static_assert(allFaces,
                "BoundaryComponent<dim>::eulerChar() can only be used "
                "when dim is one of Regina's standard dimensions.");
            if constexpr (dim == 2) {
                // There is only one possible answer here.
                return 0;
            } else {
                if (facets().empty()) {
                    // We need the Euler characteristic of the vertex link.
                    Vertex<dim>* vtx = std::get<tupleIndex(0)>(faces_).front();
                    if constexpr (dim == 3) {
                        return vtx->linkEulerChar();
                    } else if constexpr (dim == 4) {
                        // In dimension 4, the link triangulation is
                        // already precomputed.
                        return vtx->buildLink().eulerCharTri();
                    } else {
                        // We should never reach this branch, since
                        // higher-dimensional triangulations do not
                        // understand single-vertex boundary components.
                        return 0;
                    }
                } else {
                    // We have a real boundary component.
                    return std::apply([](auto&&... kFaces) {
                        return (static_cast<long>(kFaces.size()) - ...);
                    }, faces_);
                }
            }
        }

        /**
         * Determines if this boundary component is real.
         * This is the case if and only if it is formed from one or more
         * (dim-1)-faces.
         *
         * See the BoundaryComponent class notes for an overview of real,
         * ideal, and invalid vertex boundary components.
         *
         * \note If \a dim is not one of Regina's
         * \ref stddim "standard dimensions", then real boundary components
         * are the only types of boundary component that Regina will recognise,
         * which means that this routine will always return \c true.
         *
         * \return \c true if and only if this boundary component is real.
         */
        bool isReal() const {
            if constexpr (allFaces)
                return ! std::get<tupleIndex(dim-1)>(faces_).empty();
            else
                return true;
        }

        /**
         * Determines if this boundary component is ideal.
         * This is the case if and only if it consists of a single
         * ideal vertex and no faces of any other dimensions.
         *
         * See the BoundaryComponent class notes for an overview of ideal
         * boundary components, which can only occur in dimensions ≥ 3,
         * and which are only recognised where \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * Note that a boundary component formed from a single _invalid_
         * vertex is _not_ considered to be ideal.  This means that, if a
         * boundary component contains no faces of positive dimension,
         * then one and only one of isIdeal() and isInvalidVertex() will
         * return \c true.
         *
         * \note If \a dim is not one of Regina's
         * \ref stddim "standard dimensions", then real boundary components
         * are the only types of boundary component that Regina will recognise,
         * which means that this routine will always return \c false.
         *
         * \return \c true if and only if this boundary component is ideal.
         */
        bool isIdeal() const {
            if constexpr (allFaces) {
                // Either Vertex::isValid() or Vertex::isIdeal() will do here.
                return (std::get<tupleIndex(dim-1)>(faces_).empty() &&
                    std::get<tupleIndex(0)>(faces_).front()->isValid());
            } else {
                return false;
            }
        }

        /**
         * Determines if this boundary component consists of a single invalid
         * vertex and nothing else.  In particular, such a boundary component
         * must contain no faces of any positive dimension.
         *
         * See the BoundaryComponent class notes for an overview of
         * invalid vertex boundary components, which can only occur in
         * dimensions ≥ 4, and which are only recognised where \a dim is
         * one of Regina's \ref stddim "standard dimensions".
         *
         * An invalid vertex is only placed in its own boundary component if
         * it does not already belong to some larger boundary component
         * (for instance, if its link is an ideal (<i>dim</i>-1)-manifold
         * triangulation).  This means that, for a boundary component
         * consisting of one or more (<i>dim</i>-1)-faces, this routine will
         * return \c false even if the boundary component also includes
         * one or more invalid vertices.
         *
         * Note that, if a boundary component contains no faces of positive
         * dimension, then one and only one of isIdeal() and isInvalidVertex()
         * will return \c true.
         *
         * \note If \a dim is not one of Regina's
         * \ref stddim "standard dimensions", then real boundary components
         * are the only types of boundary component that Regina will recognise,
         * which means that this routine will always return \c false.
         *
         * \return \c true if and only if this boundary component consists of a
         * single invalid vertex and nothing else.
         */
        bool isInvalidVertex() const {
            if constexpr (allFaces) {
                return (std::get<tupleIndex(dim-1)>(faces_).empty() &&
                    ! std::get<tupleIndex(0)>(faces_).front()->isValid());
            } else {
                return false;
            }
        }

        /**
         * Returns the full (<i>dim</i>-1)-dimensional triangulation of this
         * boundary component.  Note that this triangulation is read-only
         * (though of course you can clone it and then operate upon the clone).
         *
         * If this is a real boundary component (i.e., it is built from
         * one or more (<i>dim</i>-1)-faces), then the triangulation of this
         * boundary component is as follows:
         *
         * - Let \a i lie between 0 and size()-1 inclusive.  Then simplex \a i
         *   of the returned (<i>dim</i>-1)-dimensional triangulation is
         *   a copy of `facet(i)` of this boundary component,
         *   and its vertices 0,...,<i>dim</i>-1 are numbered in the
         *   same way.  To relate these (<i>dim</i>-1)-face vertex numbers to
         *   the vertex numbers of top-dimensional simplices in the overall
         *   <i>dim</i>-dimensional triangulation, see
         *   Simplex<dim>::faceMapping<dim-1>().
         *
         * - If this boundary component stores lower-dimensional faces (i.e.,
         *   if the class constant \a allFaces is \c true), then a similar
         *   correspondence holds for these lower-dimensional faces also:
         *   for each \a i, <i>k</i>-face \a i of the returned triangulation is
         *   a copy of `face<k>(i)` of this boundary component,
         *   and its vertices are numbered in the same way.  As an exception,
         *   this correspondence will not hold for dimensions \a k where
         *   there exist _pinched_ <i>k</i>-faces on the boundary (i.e.,
         *   faces where different sections of the boundary are pinched
         *   together, meaning that these faces must be duplicated when the
         *   boundary is triangulated).
         *
         * If this boundary component consists only of a single vertex
         * (i.e., this is an ideal or invalid vertex boundary component),
         * then this routine returns the triangulation of the corresponding
         * vertex link.  See Vertex::buildLink() for details.
         *
         * This routine is fast, since it caches the boundary triangulation.
         * Moreover, it is guaranteed that the full skeleton of this
         * (<i>dim</i>-1)-dimensional triangulation will have been generated
         * already.
         *
         * \pre The dimension \a dim is greater than 2.
         *
         * \return the triangulation of this boundary component.
         */
        const Triangulation<dim-1>& build() const {
            // Make sure we do not try to instantiate Triangulation<1>.
            static_assert(canBuild,
                "BoundaryComponent<dim>::build() can only "
                "be used with dimensions dim > 2.");

            if (boundary_.value)
                return *boundary_.value; // Already cached or pre-computed.
            if constexpr (allowVertex) {
                if (facets().empty()) {
                    // We have an ideal or invalid vertex.
                    return std::get<tupleIndex(0)>(faces_).front()->buildLink();
                }
            }

            return *(const_cast<BoundaryComponentBase<dim>*>(this)->
                boundary_.value = buildRealBoundary());
        }

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const {
            out << "Boundary component " << index();
            if constexpr (allowVertex) {
                out << (isIdeal() ? ", ideal" :
                        isInvalidVertex() ? ", invalid" : ", finite");
            }

            if constexpr (allowVertex) {
                if (isIdeal() || isInvalidVertex()) {
                    // We have no boundary facets.
                    Face<dim, 0>* v = std::get<tupleIndex(0)>(faces_).front();
                    out << " at vertex " << v->index();
                    bool first = true;
                    for (auto& emb : *v) {
                        if (first) {
                            out << ": ";
                            first = false;
                        } else
                            out << ", ";
                        out << emb.simplex()->index()
                            << " (" << emb.vertex() << ')';
                    }
                    return;
                }
            }

            // We have boundary facets.
            bool first = true;
            for (auto s : facets()) {
                if (first) {
                    out << ": ";
                    first = false;
                } else
                    out << ", ";
                out << s->front().simplex()->index() << " ("
                    << s->front().vertices().trunc(dim) << ')';
            }
        }

        // Make this class non-copyable.
        BoundaryComponentBase(const BoundaryComponentBase&) = delete;
        BoundaryComponentBase& operator = (const BoundaryComponentBase&) =
            delete;

    protected:
        /**
         * Default constructor that leaves orientability uninitialised.
         */
        BoundaryComponentBase() = default;

    private:
        /**
         * Pushes the given face onto the end of the list of
         * <i>subdim</i>-faces of this boundary component.
         * This class does not take ownership of the given face.
         *
         * \param face the face to append to the list.
         */
        template <int subdim>
        void push_back(Face<dim, subdim>* face) {
            if constexpr ((! allFaces) && subdim == dim - 2) {
                // We don't store (dim-2)-faces, but we do count them.
                ++nRidges_.value;
            } else {
                static_assert(tupleIndex(subdim) >= 0,
                    "BoundaryComponentBase::push_back() does not support "
                    "this (dim, subdim) combination.");
                std::get<tupleIndex(subdim)>(faces_).push_back(face);
            }
        }

        /**
         * Builds a new triangulation of this boundary component,
         * assuming this is a real boundary component.
         *
         * \pre The dimension \a dim is greater than 2.
         * \pre The number of (dim-1)-faces is strictly positive.
         *
         * \return the newly created boundary triangulation.
         */
        Triangulation<dim-1>* buildRealBoundary() const;

        /**
         * Reorders and relabels all <i>subdim</i>-faces of the given
         * triangulation so that they appear in the same order as the
         * corresponding faces of this boundary component, and so that their
         * vertices are numbered in a corresponding way.
         *
         * If there are any _pinched_ <i>subdim</i>-faces in this
         * boundary component (so they appear multiple times when the
         * boundary is triangulated), then this routine will do nothing.
         *
         * \pre This is a real boundary component.
         * \pre \a tri is a triangulation of this boundary component.
         * \pre For each \a i, the <i>i</i>th top-dimensional simplex of
         * \a tri corresponds to the <i>i</i>th (<i>dim</i>-1)-face of
         * this boundary component, and has its vertices 0,...,(<i>dim</i>-1)
         * labelled in the same way.
         *
         * \tparam subdim the dimension of the faces to reorder and relabel.
         * If this is equal to (<i>dim</i>-1) then this routine will do nothing
         * (since the (<i>dim</i>-1)-faces are already in perfect
         * correspondence).
         *
         * \param tri a triangulation of this boundary component, as
         * described above.
         * \param reference the internal list of all <i>subdim</i>-faces
         * of this boundary component.  While this can of course be deduced,
         * it is passed as a separate argument so that the template parameter
         * \a subdim can be deduced automatically from inside std::apply().
         */
        template <int subdim>
        void reorderAndRelabelFaces(Triangulation<dim - 1>* tri,
                const std::vector<Face<dim, subdim>*>& reference) const;

    friend class Triangulation<dim>;
    friend class TriangulationBase<dim>;
        /**< Allow access to private members. */
};

} // namespace regina::detail

#endif
