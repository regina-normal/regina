
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#ifndef __REGINA_BOUNDARYCOMPONENT_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_BOUNDARYCOMPONENT_H_DETAIL
#endif

/*! \file triangulation/boundarycomponent.h
 *  \brief Implementation details for boundary components of triangulations.
 */

#include <ranges>
#include <tuple>
#include <vector>
#include "regina-core.h"
#include "core/output.h"
#include "triangulation/detail/strings.h"
#include "triangulation/forward.h"
#include "utilities/exception.h"
#include "utilities/markedvector.h"
#include "utilities/typeutils.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {
namespace detail {
    template <int dim> requires (supportedDim(dim)) class TriangulationBase;
}

/**
 * A component of the boundary of a <i>dim</i>-manifold triangulation.
 *
 * Regina recognises three types of boundary components:
 *
 * - A _real boundary component_ is formed from unglued facets
 *   of top-dimensional simplices of the triangulation.  For example,
 *   a real boundary component of a 3-manifold triangulation is formed
 *   from boundary triangles in the 2-skeleton.
 *
 * - An _ideal boundary component_ consists of a single ideal vertex;
 *   that is, a vertex whose link is a closed (<i>dim</i>-1)-manifold but not
 *   a sphere.  This is only possible for dimensions \a dim ≥ 3.
 *
 * - An _invalid vertex boundary component_ consists of a single invalid
 *   invalid vertex that does not belong to a real boundary component.  This
 *   means the vertex link must be an ideal (<i>dim</i>-1)-manifold, or an
 *   invalid (<i>dim</i>-1)-manifold with no real boundary.  Invalid vertex
 *   boundary components are only possible for dimensions \a dim ≥ 4.
 *
 * Ideal and invalid vertex boundary components are _only_ recognised
 * when \a dim is one of Regina's \ref stddim "standard dimensions".
 * This is because, in higher dimensions, the relevant conditions rely
 * on undecidable problems.
 *
 * Regina stores different skeletal information for different types of
 * boundary components:
 *
 * - For real boundary components in \ref stddim "standard dimensions",
 *   Regina stores all boundary faces of all dimensions 0,1,...,<i>dim</i>-1.
 *
 * - For real boundary components in higher (non-standard) dimensions,
 *   Regina only stores the boundary (<i>dim</i>-1)-faces (i.e., the unglued
 *   facets of top-dimensional simplices that make up the boundary component).
 *   This is simply to avoid excessive memory usage.
 *
 * - For ideal and invalid vertex boundary components, Regina stores the
 *   relevant vertex.  Such boundary components do not contain any faces
 *   of any dimension greater than zero.
 *
 * We can encounter some interesting cases with invalid triangulations.
 * Consider some face whose link has more than one boundary component
 * (which makes the face invalid).  This means that different parts of the
 * (<i>dim</i>)-manifold boundary are effectively "pinched" together.
 * If this happens, the different parts of the boundary that are pinched
 * might or might not be listed as part of the same boundary component;
 * if not then the offending face will be included in _all_ of these
 * boundary components.  Nevertheless, only one of these can be considered
 * the "official" boundary component of the face as returned by
 * Face::boundaryComponent().  This is all a bit of a mess, but then again the
 * entire triangulation is invalid and so you almost certainly have
 * bigger problems to deal with.
 *
 * Boundary components are highly temporary: whenever a triangulation changes,
 * all of its boundary component objects will be deleted and new ones will be
 * created in their place.
 *
 * Boundary components do not support value semantics: they cannot be copied,
 * swapped, or manually constructed.  Their location in memory defines them,
 * and they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \python Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., BoundaryComponent2 and BoundaryComponent3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \headerfile triangulation/triangulation.h
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
class BoundaryComponent:
        public ShortOutput<BoundaryComponent<dim>>, public MarkedElement {
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

        bool orientable_;
            /**< Is this boundary component orientable? */

        EnableIf<canBuild, typename TriangulationTraits<dim>::Lower*, nullptr>
                boundary_;
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
        ~BoundaryComponent();

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
         * This is the fastest way to count faces if you know \a subdim
         * at compile time.
         *
         * \nopython Instead use the variant `countFaces(subdim)`.
         *
         * \tparam subdim the dimension of the faces to query.  If \a dim is
         * one of Regina's \ref stddim "standard dimensions", then \a subdim
         * must be between 0 and `dim-1` inclusive.  Otherwise, the only
         * allowable values of \a subdim are the facet dimension (`dim-1`)
         * and the ridge dimension (`dim-2`).
         *
         * \return the number of <i>subdim</i>-faces.
         */
        template <int subdim>
        requires (subdim < dim && subdim >= (standardDim(dim) ? 0 : dim - 2))
        size_t countFaces() const {
            if constexpr (allFaces) {
                return std::get<tupleIndex(subdim)>(faces_).size();
            } else {
                if constexpr (subdim == dim - 2)
                    return nRidges_.value;
                else
                    return std::get<tupleIndex(subdim)>(faces_).size();
            }
        }

        /**
         * Returns the number of <i>subdim</i>-faces in this boundary component,
         * where the face dimension does not need to be known until runtime.
         *
         * For C++ programmers who know \a subdim at compile time, you are
         * better off using the template function `countFaces<subdim>()`
         * instead, which is (slightly) faster.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range.
         *
         * \param subdim the dimension of the faces to query.  If \a dim is
         * one of Regina's \ref stddim "standard dimensions", then \a subdim
         * must be between 0 and `dim-1` inclusive.  Otherwise, the only
         * allowable values of \a subdim are the facet dimension (`dim-1`)
         * and the ridge dimension (`dim-2`).
         * \return the number of <i>subdim</i>-faces.
         */
        size_t countFaces(int subdim) const {
            if constexpr (allFaces) {
                if (subdim < 0 || subdim >= dim)
                    throw InvalidArgument(
                        "countFaces(): unsupported face dimension");
                return select_constexpr<0, dim, size_t>(subdim, [this](auto k) {
                    return std::get<tupleIndex(k)>(faces_).size();
                });
            } else {
                switch (subdim) {
                    case dim - 2:
                        return nRidges_.value;
                    case dim - 1:
                        return std::get<tupleIndex(dim - 1)>(faces_).size();
                    default:
                        throw InvalidArgument(
                            "countFaces(): unsupported face dimension");
                }
            }
        }

        /**
         * A dimension-specific alias for countFaces<0>().
         *
         * See countFaces() for further information.
         */
        size_t countVertices() const
                requires (standardDim(dim)) {
            return countFaces<0>();
        }

        /**
         * A dimension-specific alias for countFaces<1>().
         *
         * See countFaces() for further information.
         */
        size_t countEdges() const
                requires (standardDim(dim)) {
            return countFaces<1>();
        }

        /**
         * A dimension-specific alias for countFaces<2>().
         *
         * See countFaces() for further information.
         */
        size_t countTriangles() const
                requires (standardDim(dim) && dim > 2) {
            return countFaces<2>();
        }

        /**
         * A dimension-specific alias for countFaces<3>().
         *
         * See countFaces() for further information.
         */
        size_t countTetrahedra() const
                requires (dim == 4 || dim == 5) {
            return countFaces<3>();
        }

        /**
         * A dimension-specific alias for countFaces<4>().
         *
         * See countFaces() for further information.
         */
        size_t countPentachora() const
                requires (dim == 5 || dim == 6) {
            return countFaces<4>();
        }

        /**
         * Returns an object that allows iteration through and random access
         * to all (<i>dim</i>-1)-faces in this boundary component.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use `auto` (just like this declaration does).
         *
         * The returned object is guaranteed to be a lightweight view type
         * from the `std::ranges` library, which means it supports range-based
         * `for` loops.  Note that the elements of the view will be pointers,
         * so your code might look like:
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
            return std::views::all(std::get<tupleIndex(dim-1)>(faces_));
        }

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces in this boundary component, in a way
         * that is optimised for C++ programmers.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use `auto` (just like this declaration does).
         *
         * The returned object is guaranteed to be a lightweight view type
         * from the `std::ranges` library, which means it supports range-based
         * `for` loops.  Note that the elements of the view will be pointers,
         * so your code might look like:
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
         * \nopython Instead use the variant `faces(subdim)`.
         *
         * \tparam subdim the dimension of the faces to query.  If \a dim is
         * one of Regina's \ref stddim "standard dimensions", then \a subdim
         * must be between 0 and `dim-1` inclusive.  Otherwise, the only
         * allowable value of \a subdim is the facet dimension (`dim-1`).
         *
         * \return access to the list of all <i>subdim</i>-faces.
         */
        template <int subdim>
        requires (subdim == dim - 1 ||
            (standardDim(dim) && subdim >= 0 && subdim < dim))
        auto faces() const {
            return std::views::all(std::get<tupleIndex(subdim)>(faces_));
        }

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces in this boundary component, in a way
         * that is optimised for Python programmers.
         *
         * C++ users should not use this routine.  The return type must be fixed
         * at compile time, and so it is typically a `std::variant` that can
         * hold any of the lightweight view types returned from the templated
         * `faces<subdim>()` function.  This means that the return value will
         * still need compile-time knowledge of \a subdim to extract and
         * use the appropriate face objects.  However, once you know \a subdim
         * at compile time, you are much better off using the (simpler and
         * faster) routine `faces<subdim>()` instead.
         *
         * For Python users, this routine is much more useful: the return type
         * can be chosen at runtime, and so this routine returns a single
         * lightweight view granting access to all of the <i>subdim</i>-faces
         * of the boundary component, which you can use immediately.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range, as described below.
         *
         * \param subdim the dimension of the faces to query.  If \a dim is
         * one of Regina's \ref stddim "standard dimensions", then \a subdim
         * must be between 0 and `dim-1` inclusive.  Otherwise, the only
         * allowable value of \a subdim is the facet dimension (`dim-1`).
         * \return access to the list of all <i>subdim</i>-faces.
         */
        auto faces(int subdim) const {
            if constexpr (allFaces) {
                if (subdim < 0 || subdim >= dim)
                    throw InvalidArgument(
                        "faces(): unsupported face dimension");
                return select_constexpr_as_variant<0, dim>(subdim,
                        [this](auto k) {
                    return std::views::all(std::get<tupleIndex(k)>(faces_));
                });
            } else {
                if (subdim != dim - 1)
                    throw InvalidArgument(
                        "faces(): unsupported face dimension");
                return std::views::all(std::get<tupleIndex(dim-1)>(faces_));
            }
        }

        /**
         * A dimension-specific alias for faces<0>().
         *
         * See faces() for further information.
         */
        auto vertices() const
                requires (standardDim(dim)) {
            return std::views::all(std::get<tupleIndex(0)>(faces_));
        }

        /**
         * A dimension-specific alias for faces<1>().
         *
         * See faces() for further information.
         */
        auto edges() const
                requires (standardDim(dim)) {
            return std::views::all(std::get<tupleIndex(1)>(faces_));
        }

        /**
         * A dimension-specific alias for faces<2>().
         *
         * See faces() for further information.
         */
        auto triangles() const
                requires (standardDim(dim) && dim > 2) {
            return std::views::all(std::get<tupleIndex(2)>(faces_));
        }

        /**
         * A dimension-specific alias for faces<3>().
         *
         * See faces() for further information.
         */
        auto tetrahedra() const
                requires (standardDim(dim) && dim > 3) {
            return std::views::all(std::get<tupleIndex(3)>(faces_));
        }

        /**
         * A dimension-specific alias for faces<4>().
         *
         * See faces() for further information.
         */
        auto pentachora() const
                requires (dim == 5) {
            return std::views::all(std::get<tupleIndex(4)>(faces_));
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
         * Returns the requested <i>subdim</i>-face in this boundary component,
         * in a way that is optimised for C++ programmers.
         *
         * Note that the index of a face in the boundary component need
         * not be the index of the same face in the overall triangulation.
         * However, if this is a real boundary component (i.e., it is built
         * from one or more (<i>dim</i>-1)-faces), then the index of each
         * <i>subdim</i>-face in this boundary component will match the
         * index of the corresponding <i>subdim</i>-face in the
         * (<i>dim</i>-1)-manifold triangulation returned by build().
         *
         * \nopython Instead use the variant `face(subdim, index)`.
         *
         * \tparam subdim the dimension of the faces to query.  If \a dim is
         * one of Regina's \ref stddim "standard dimensions", then \a subdim
         * must be between 0 and `dim-1` inclusive.  Otherwise, the only
         * allowable value of \a subdim is the facet dimension (`dim-1`).
         *
         * \param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * \return the requested face.
         */
        template <int subdim>
        requires (subdim == dim - 1 ||
            (standardDim(dim) && subdim >= 0 && subdim < dim))
        Face<dim, subdim>* face(size_t index) const {
            return std::get<tupleIndex(subdim)>(faces_)[index];
        }

        /**
         * Returns the requested <i>subdim</i>-face in this boundary component,
         * in a way that is optimised for Python programmers.
         *
         * C++ users should not use this routine.  The return type must be
         * fixed at compile time, and so it is typically a `std::variant` that
         * could store a pointer to any class `Face<dim, ...>`.  This means you
         * cannot access the face directly: you will still need some kind of
         * compile-time knowledge of \a subdim before you can extract and use
         * an appropriate `Face<dim, subdim>` object from \a v.  However, once
         * you know \a subdim at compile time, you are better off using the
         * (simpler and faster) routine `face<subdim>()` instead.
         *
         * For Python users, this routine is much more useful: the return type
         * can be chosen at runtime, and so this routine simply returns a
         * `Face<dim, subdim>` object of the appropriate face dimension that
         * you can use immediately.
         *
         * The specific return type for C++ programmers will be
         * `std::variant<Face<dim, 0>*, ..., Face<dim, dim-1>*>` if \a dim is
         * one of Regina's \ref stddim "standard dimensions", or just
         * `Face<dim, dim-1>*` if not.
         *
         * Note that the index of a face in the boundary component need
         * not be the index of the same face in the overall triangulation.
         * However, if this is a real boundary component (i.e., it is built
         * from one or more (<i>dim</i>-1)-faces), then the index of each
         * <i>subdim</i>-face in this boundary component will match the
         * index of the corresponding <i>subdim</i>-face in the
         * (<i>dim</i>-1)-manifold triangulation returned by build().
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range, as described below.
         *
         * \param subdim the dimension of the faces to query.  If \a dim is
         * one of Regina's \ref stddim "standard dimensions", then \a subdim
         * must be between 0 and `dim-1` inclusive.  Otherwise, the only
         * allowable value of \a subdim is the facet dimension (`dim-1`).
         * \param index the index of the desired face, ranging from 0 to
         * `countFaces(subdim)-1` inclusive.
         * \return the requested face.
         */
        auto face(int subdim, size_t index) const {
            if constexpr (allFaces) {
                if (subdim < 0 || subdim >= dim)
                    throw InvalidArgument("face(): unsupported face dimension");
                return select_constexpr_as_variant<0, dim>(subdim,
                        [this, index](auto k) {
                    return std::get<tupleIndex(k)>(faces_)[index];
                });
            } else {
                if (subdim != dim - 1)
                    throw InvalidArgument("face(): unsupported face dimension");
                return std::get<tupleIndex(dim-1)>(faces_)[index];
            }
        }

        /**
         * A dimension-specific alias for face<0>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Vertex* vertex(size_t index) const
                requires (standardDim(dim)) {
            return std::get<tupleIndex(0)>(faces_)[index];
        }

        /**
         * A dimension-specific alias for face<1>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Edge* edge(size_t index) const
                requires (standardDim(dim)) {
            return std::get<tupleIndex(1)>(faces_)[index];
        }

        /**
         * A dimension-specific alias for face<2>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Triangle* triangle(size_t index) const
                requires (standardDim(dim) && dim > 2) {
            return std::get<tupleIndex(2)>(faces_)[index];
        }

        /**
         * A dimension-specific alias for face<3>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Tetrahedron* tetrahedron(size_t index) const
                requires (dim == 4) {
            return std::get<tupleIndex(3)>(faces_)[index];
        }

        /**
         * A dimension-specific alias for face<4>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Pentachoron* pentachoron(size_t index) const
                requires (dim == 5) {
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
        long eulerChar() const requires (standardDim(dim)) {
            static_assert(allFaces); // just to be sure
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
        const TriangulationTraits<dim>::Lower& build() const
                requires (dim > 2) {
            if (boundary_.value)
                return *boundary_.value; // Already cached or pre-computed.
            if constexpr (allowVertex) {
                if (facets().empty()) {
                    // We have an ideal or invalid vertex.
                    return std::get<tupleIndex(0)>(faces_).front()->buildLink();
                }
            }

            return *(const_cast<BoundaryComponent<dim>*>(this)->
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
        BoundaryComponent(const BoundaryComponent&) = delete;
        BoundaryComponent& operator = (const BoundaryComponent&) = delete;

    private:
        /**
         * Default constructor that leaves orientability uninitialised.
         */
        BoundaryComponent() = default;

        /**
         * Pushes the given face onto the end of the list of
         * <i>subdim</i>-faces of this boundary component.
         * This class does not take ownership of the given face.
         *
         * \param face the face to append to the list.
         */
        template <int subdim>
        requires (subdim < dim && subdim >= (standardDim(dim) ? 0 : dim - 2))
        void push_back(Face<dim, subdim>* face) {
            if constexpr ((! allFaces) && subdim == dim - 2) {
                // We don't store (dim-2)-faces, but we do count them.
                ++nRidges_.value;
            } else {
                static_assert(tupleIndex(subdim) >= 0); // just to be sure
                std::get<tupleIndex(subdim)>(faces_).push_back(face);
            }
        }

        /**
         * Builds a new triangulation of this boundary component,
         * assuming this is a real boundary component.
         *
         * \pre The number of (dim-1)-faces is strictly positive.
         *
         * \return the newly created boundary triangulation.
         */
        TriangulationTraits<dim>::Lower* buildRealBoundary() const
            requires (dim > 2);

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
        requires (dim > 2 && subdim >= 0 && subdim < dim)
        void reorderAndRelabelFaces(TriangulationTraits<dim>::Lower* tri,
                const std::vector<Face<dim, subdim>*>& reference) const;

    friend class Triangulation<dim>;
    friend class detail::TriangulationBase<dim>;
        /**< Allow access to private members. */
};

} // namespace regina

#endif
