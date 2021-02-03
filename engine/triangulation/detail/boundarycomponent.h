
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

#ifndef __BOUNDARYCOMPONENT_H_DETAIL
#ifndef __DOXYGEN
#define __BOUNDARYCOMPONENT_H_DETAIL
#endif

/*! \file triangulation/detail/boundarycomponent.h
 *  \brief Implementation details for boundary components of triangulations.
 */

#include <tuple>
#include <vector>
#include "regina-core.h"
#include "core/output.h"
#include "triangulation/alias/face.h"
#include "triangulation/detail/strings.h"
#include "triangulation/forward.h"
#include "utilities/listview.h"
#include "utilities/markedvector.h"
#include "utilities/typeutils.h"

namespace regina::detail {

template <int> class TriangulationBase;

/**
 * \weakgroup detail
 * @{
 */

/**
 * Helper class for storing the necessary faces of a boundary component of a
 * <i>dim</i>-dimensional triangulation.  Every class BoundaryComponent<dim>
 * inherits from this template.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * BoundaryComponent<dim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the dimensions of all faces that should be stored;
 * for Regina's standard dimensions this must be exactly the sequence
 * 0, 1, ..., <i>dim</i>-1, and for higher dimensions it must contain
 * <i>dim</i>-1 and nothing else.
 */
template <int dim, int... subdim>
class BoundaryComponentFaceStorage :
        public alias::FacesOfTriangulation<
            BoundaryComponentFaceStorage<dim, subdim...>, dim>,
        public alias::FaceOfTriangulation<
            BoundaryComponentFaceStorage<dim, subdim...>, dim> {
    static_assert(standardDim(dim),
        "The BoundaryComponentFaceStorage template should only use "
        "a full set of face dimensions for Regina's standard dimensions.");
    static_assert(sizeof...(subdim) == dim &&
        (subdim + ...) == dim * (dim - 1) / 2,
        "The many-argument BoundaryComponentFaceStorage template has been "
        "given an unexpected set of face dimensions.");

    public:
        /**
         * A compile-time constant indicating whether this boundary
         * component class stores all lower-dimensional faces (\c true),
         * or only faces of dimension <i>dim</i>-1 (\c false).
         */
        static constexpr bool allFaces = true;

    protected:
        std::tuple<std::vector<Face<dim, subdim>*>...> faces_;
            /**< Each element of this tuple stores all faces of a
                 particular dimension \a subdim. */

    private:
        template <int useDim> class ReorderIterator;

    public:
        /**
         * Returns the number of (<i>dim</i>-1)-faces in this boundary
         * component.  These are the top-dimensional faces for a real
         * boundary component.
         *
         * If this is an ideal or invalid vertex boundary component,
         * then this routine will return 0.
         *
         * @return the number of (<i>dim</i>-1)-faces in this boundary
         * component.
         */
        size_t size() const {
            return std::get<dim - 1>(faces_).size();
        }

        /**
         * Returns the number of (<i>dim</i>-2)-faces in this boundary
         * component.
         *
         * If this is an ideal or invalid vertex boundary component,
         * then this routine will return 0.
         *
         * @return the number of (<i>dim</i>-2)-faces in this boundary
         * component.
         */
        size_t countRidges() const {
            return std::get<dim - 2>(faces_).size();
        }

        /**
         * Returns the number of <i>useDim</i>-faces in this boundary component.
         *
         * This routine is only available where \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>countFaces(useDim)</tt>; that is, the template parameter
         * \a useDim becomes the first argument of the function.
         *
         * \tparam useDim the dimension of the faces to query.  This must
         * be between 0 and <i>dim</i>-1 inclusive.
         *
         * @return the number of <i>useDim</i>-faces.
         */
        template <int useDim>
        size_t countFaces() const {
            return std::get<useDim>(faces_).size();
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
         * C++11 range-based \c for loops.  Note that the elements of the list
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
         * \ifacespython This routine returns a Python list.
         *
         * @return access to the list of all (<i>dim</i>-1)-faces.
         */
        auto facets() const {
            return ListView(std::get<dim - 1>(faces_));
        }

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>useDim</i>-faces in this boundary component.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * C++11 range-based \c for loops.  Note that the elements of the list
         * will be pointers, so your code might look like:
         *
         * \code{.cpp}
         * for (Face<dim, useDim>* f : bc.faces<useDim>()) { ... }
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
         * This routine is only available where \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * \ifacespython Python users should call this function in the
         * form <tt>faces(useDim)</tt>.  It will then return a Python list
         * containing all the <i>useDim</i>-faces of the boundary component.
         * Be warned that, unlike in C++, this Python list will be a
         * snapshot of the faces when this function is called, and will
         * \e not be kept up-to-date as the triangulation changes.
         *
         * \tparam useDim the dimension of the faces to query.  This must
         * be between 0 and <i>dim</i>-1 inclusive.
         *
         * @return access to the list of all <i>useDim</i>-faces.
         */
        template <int useDim>
        auto faces() const {
            return ListView(std::get<useDim>(faces_));
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
         * @param index the index of the desired face, ranging from 0 to
         * size()-1 inclusive.
         * @return the requested face.
         */
        Face<dim, dim-1>* facet(size_t index) const {
            return std::get<dim - 1>(faces_)[index];
        }

        /**
         * Returns the requested <i>useDim</i>-face in this boundary component.
         *
         * Note that the index of a face in the boundary component need
         * not be the index of the same face in the overall triangulation.
         * However, if this is a real boundary component (i.e., it is built
         * from one or more (<i>dim</i>-1)-faces), then the index of each
         * <i>useDim</i>-face in this boundary component will match the
         * index of the corresponding <i>useDim</i>-face in the
         * (<i>dim</i>-1)-manifold triangulation returned by build().
         *
         * This routine is only available where \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>face(useDim, index)</tt>; that is, the template parameter
         * \a useDim becomes the first argument of the function.
         *
         * \tparam useDim the dimension of the face to query.  This must
         * be between 0 and <i>dim</i>-1 inclusive.
         *
         * @param index the index of the desired face, ranging from 0 to
         * countFaces<useDim>()-1 inclusive.
         * @return the requested face.
         */
        template <int useDim>
        Face<dim, useDim>* face(size_t index) const {
            return std::get<useDim>(faces_)[index];
        }

        /**
         * Returns the triangulation to which this boundary component belongs.
         *
         * @return the triangulation containing this boundary component.
         */
        Triangulation<dim>* triangulation() const {
            // There may be no (dim-1)-simplices, but there is always a vertex.
            return std::get<0>(faces_).front()->triangulation();
        }

        /**
         * Returns the connected component of the triangulation to which this
         * boundary component belongs.
         *
         * @return the component containing this boundary component.
         */
        Component<dim>* component() const {
            // There may be no (dim-1)-simplices, but there is always a vertex.
            return std::get<0>(faces_).front()->component();
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
         * This routine is only available where \a dim is one of Regina's
         * \ref stddim "standard dimensions".
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
         * @return the Euler characteristic of this boundary component.
         */
        long eulerChar() const {
            if constexpr (dim == 2) {
                // There is only one possible answer here.
                return 0;
            } else {
                if (facets().empty()) {
                    // We need the Euler characteristic of the vertex link.
                    Vertex<dim>* vertex = std::get<0>(faces_).front();
                    if constexpr (dim == 3) {
                        return vertex->linkEulerChar();
                    } else if constexpr (dim == 4) {
                        // In dimension 4, the link triangulation is
                        // already precomputed.
                        return vertex->buildLink()->eulerCharTri();
                    } else {
                        // We should never reach this branch, since
                        // higher-dimensional triangulations do not
                        // understand single-vertex boundary components.
                        return 0;
                    }
                } else {
                    // We have a real boundary component.
                    return (static_cast<long>(std::get<subdim>(faces_).size()) -
                        ...);
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
         * This routine is only available where \a dim is
         * one of Regina's \ref stddim "standard dimensions".
         * (In higher dimensions, real boundary components are the only
         * types of boundary component that Regina will recognise.)
         *
         * @return \c true if and only if this boundary component is real.
         */
        bool isReal() const {
            return ! facets().empty();
        }

        /**
         * Determines if this boundary component is ideal.
         * This is the case if and only if it consists of a single
         * ideal vertex and no faces of any other dimensions.
         *
         * See the BoundaryComponent class notes for an overview of ideal
         * boundary components, which can only occur in dimensions &ge; 3,
         * and which are only recognised where \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * Note that a boundary component formed from a single \e invalid
         * vertex is \e not considered to be ideal.  This means that, if a
         * boundary component contains no faces of positive dimension,
         * then one and only one of isIdeal() and isInvalidVertex() will
         * return \c true.
         *
         * This routine is only available where \a dim is
         * one of Regina's \ref stddim "standard dimensions".
         * (In higher dimensions, Regina does not recognise ideal
         * boundary components at all.)
         *
         * @return \c true if and only if this boundary component is ideal.
         */
        bool isIdeal() const {
            // Either of Vertex::isValid() or Vertex::isIdeal() will do here.
            return (facets().empty() &&
                std::get<0>(faces_).front()->isValid());
        }

        /**
         * Determines if this boundary component consists of a single invalid
         * vertex and nothing else.  In particular, such a boundary component
         * must contain no faces of any positive dimension.
         *
         * See the BoundaryComponent class notes for an overview of
         * invalid vertex boundary components, which can only occur in
         * dimensions &ge; 4, and which are only recognised where \a dim is
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
         * This routine is only available where \a dim is
         * one of Regina's \ref stddim "standard dimensions".
         * (In higher dimensions, Regina does not recognise invalid vertex
         * boundary components at all.)
         *
         * @return \c true if and only if this boundary component consists of a
         * single invalid vertex and nothing else.
         */
        bool isInvalidVertex() const {
            return (facets().empty() &&
                ! std::get<0>(faces_).front()->isValid());
        }

        // Make this class non-copyable.
        BoundaryComponentFaceStorage(const BoundaryComponentFaceStorage&) =
            delete;
        BoundaryComponentFaceStorage& operator = (
            const BoundaryComponentFaceStorage&) = delete;

    protected:
        /**
         * Default constructor that leaves all face lists empty.
         */
        BoundaryComponentFaceStorage() = default;

        /**
         * Triangulates the vertex link for an ideal or invalid vertex
         * boundary component.
         *
         * @return the triangulated vertex link.
         */
        const Triangulation<dim-1>* buildVertexLink() const {
            // Make sure we do not try to instantiate Triangulation<1>.
            static_assert(dim > 2,
                "BoundaryComponent<dim>::buildVertexLink() can only "
                "be used with dimensions dim > 2.");

            return std::get<0>(faces_).front()->buildLink();
        }

        /**
         * Pushes the given face onto the end of the list of
         * <i>useDim</i>-faces of this boundary component.
         * This class does not take ownership of the given face.
         *
         * \tparam useDim the dimension of the face to append.  This must
         * be between 0 and <i>dim</i>-1 inclusive.
         *
         * @param face the face to append to the list.
         */
        template <int useDim>
        void push_back(Face<dim, useDim>* face) {
            std::get<useDim>(faces_).push_back(face);
        }

        /**
         * Reorders and relabels all <i>useDim</i>-faces of the given
         * triangulation so that they appear in the same order as the
         * corresponding faces of this boundary component, and so that their
         * vertices are numbered in a corresponding way.
         *
         * If there are any \e pinched <i>useDim</i>-faces in this
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
         * \tparam useDim the dimension of the faces to reorder and relabel.
         * This must be between 0 and (<i>dim</i>-1) inclusive, though if it
         * is equal to (<i>dim</i>-1) then it will do nothing (since the
         * (<i>dim</i>-1)-faces are already in perfect correspondence).
         *
         * @param tri a triangulation of this boundary component, as
         * described above.
         */
        template <int useDim>
        void reorderAndRelabelFacesAt(Triangulation<dim - 1>* tri) const {
            static_assert(useDim >= 0 && useDim < dim,
                "An invalid face dimension was given to "
                "BoundaryComponentFaceStorage::reorderAndRelabelFacesAt().");

            if constexpr (useDim == dim - 1) {
                // The (dim-1) faces are already in perfect correspondence.
                return;
            } else {
                if (std::get<useDim>(faces_).empty())
                    return; // Should never happen.

                // Check for pinched faces: if these are present then
                // the situation is hopeless, since such faces are
                // effectively duplicated when we triangulate the boundary,
                // and so the numbers of faces do not match.
                if (countFaces<useDim>() != tri->template countFaces<useDim>())
                    return;

                // Build a map from (useDim-face indices in the d-dim triang
                // that owns this boundary component) to (useDim-faces in tri).
                //
                // This is a partial function: it is only defined for indices
                // of *boundary* useDim-faces in this d-dim triang.
                // We leave the other values of the map uninitialised.
                Face<dim - 1, useDim>** map = new Face<dim - 1, useDim>*[
                    std::get<useDim>(faces_).front()->triangulation()->
                    template countFaces<useDim>()];

                for (Face<dim - 1, useDim>* f : tri->template faces<useDim>()) {
                    const auto& emb = f->front();
                    Face<dim, dim - 1>* outer =
                        std::get<dim-1>(faces_)[emb.simplex()->index()];
                    map[outer->template face<useDim>(emb.face())->index()] = f;

                    // While we have the two corresponding faces in front of us,
                    // relabel the vertices of f now.
                    //
                    // The following two permutations should be made equal:
                    //
                    // - emb.simplex()->faceMapping<useDim>(emb.face())
                    // - outer->faceMapping<useDim>(emb.face())
                    //
                    Perm<dim> adjust =
                        emb.simplex()->template faceMapping<useDim>(
                            emb.face()).inverse() *
                        Perm<dim>::contract(
                            outer->template faceMapping<useDim>(emb.face()));
                    adjust.clear(useDim + 1);
                    tri->relabelFace(f, adjust);
                }

                tri->template reorderFaces<useDim>(
                    ReorderIterator<useDim>(
                        std::get<useDim>(faces_).begin(), map),
                    ReorderIterator<useDim>(
                        std::get<useDim>(faces_).end(), map));

                delete[] map;
            }
        }

        /**
         * Reorders and relabels all lower-dimensional faces of the given
         * triangulation so that they appear in the same order as the
         * corresponding faces of this boundary component, and so that their
         * vertices are numbered in a corresponding way.
         * This affects all faces of dimensions 0,...,(<i>dim</i>-2).
         *
         * If there are any \e pinched <i>k</i>-faces in this
         * boundary component (so they appear multiple times when the
         * boundary is triangulated), then this routine will not reorder
         * and relabel the <i>k</i>-faces (but it will still reorder and
         * relabel faces of other dimensions).
         *
         * \pre This is a real boundary component.
         * \pre \a tri is a triangulation of this boundary component.
         * \pre For each \a i, the <i>i</i>th top-dimensional simplex of
         * \a tri corresponds to the <i>i</i>th (<i>dim</i>-1)-face of
         * this boundary component, and has its vertices 0,...,(<i>dim</i>-1)
         * labelled in the same way.
         *
         * @param tri a triangulation of this boundary component, as
         * described above.
         */
        void reorderAndRelabelFaces(Triangulation<dim-1>* tri) const {
            (reorderAndRelabelFacesAt<subdim>(tri), ...);
        }

    private:
        /**
         * An input iterator that runs through the <i>useDim</i>-faces of
         * this boundary component in order and (when dereferenced) converts
         * them to the corresponding faces from some other triangulation \a tri.
         *
         * The iterator relies on an array \a map, where for each face \a f
         * of this boundary component, <tt>map[f->index()]</tt> is the
         * corresponding face of \a tri.  Note that <tt>f->index()</tt> is the
         * index of \a f in the underlying <i>dim</i>-dimensional triangulation,
         * \e not the index of \a f in this boundary component's facet list.
         */
        template <int useDim>
        class ReorderIterator {
            private:
                typedef typename std::vector<Face<dim, useDim>*>::const_iterator
                    InternalIterator;
                InternalIterator it_;
                    /**< The current face of this boundary component. */
                Face<dim - 1, useDim>** map_;

            public:
                ReorderIterator() : it_(), map_(0) {
                }
                ReorderIterator(InternalIterator it,
                        Face<dim - 1, useDim>** map) : it_(it), map_(map) {
                }

                ReorderIterator(const ReorderIterator&) = default;
                ReorderIterator& operator = (const ReorderIterator&) = default;

                bool operator == (const ReorderIterator& rhs) const {
                    return it_ == rhs.it_;
                }
                bool operator != (const ReorderIterator& rhs) const {
                    return it_ != rhs.it_;
                }

                ReorderIterator& operator ++() {
                    ++it_;
                    return *this;
                }
                ReorderIterator operator ++(int) {
                    ReorderIterator prev(*this);
                    ++it_;
                    return prev;
                }

                Face<dim - 1, useDim>* operator * () const {
                    return map_[(*it_)->index()];
                }
        };
};

/**
 * Helper class for storing the necessary faces of a boundary component of a
 * <i>dim</i>-dimensional triangulation.  See the general
 * BoundaryComponentFaceStorage template notes for further details.
 *
 * This specialisation is used for dimensions in which only
 * (<i>dim</i>-1)-dimensional faces are stored.  It therefore removes the
 * member functions for accessing lower-dimensional faces.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * \tparam facetDim the boundary facet dimension <i>dim</i>-1.
 * Ideally this would be specified directly as <tt>dim-1</tt> in the partial
 * template specialisation, and this \e should be legal according to CWG1315;
 * however, it fails to build under some versions of gcc (e.g., 10.2.0).
 */
template <int dim, int facetDim>
class BoundaryComponentFaceStorage<dim, facetDim> {
    static_assert(! standardDim(dim),
        "The BoundaryComponentFaceStorage template should not set "
        "face dimensions to only (dim-1) for Regina's standard dimensions.");
    static_assert(facetDim == dim - 1,
        "The two-argument BoundaryComponentFaceStorage template has been "
        "given an unexpected boundary facet dimension.");

    public:
        /**
         * A compile-time constant indicating whether this boundary
         * component class stores all lower-dimensional faces (\c true),
         * or only faces of dimension <i>dim</i>-1 (\c false).
         */
        static constexpr bool allFaces = false;

    protected:
        std::vector<Face<dim, dim-1>*> facets_;
            /**< List of all (dim-1)-simplices in the boundary component. */
        size_t nRidges_;
            /**< The number of (dim-2)-faces in the boundary component. */

    public:
        /**
         * Returns the number of (<i>dim</i>-1)-faces in this boundary
         * component.  These are the top-dimensional faces for a real
         * boundary component.
         *
         * @return the number of (<i>dim</i>-1)-faces in this boundary
         * component.
         */
        size_t size() const {
            return facets_.size();
        }

        /**
         * Returns the number of (<i>dim</i>-2)-faces in this boundary
         * component.
         *
         * If this is an ideal or invalid vertex boundary component,
         * then this routine will return 0.
         *
         * @return the number of (<i>dim</i>-2)-faces in this boundary
         * component.
         */
        size_t countRidges() const {
            return nRidges_;
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
         * C++11 range-based \c for loops.  Note that the elements of the list
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
         * \ifacespython This routine returns a Python list.
         *
         * @return access to the list of all (<i>dim</i>-1)-faces.
         */
        auto facets() const {
            return ListView(facets_);
        }

        /**
         * Returns the requested (<i>dim</i>-1)-face in this boundary component.
         * These are the top-dimensional faces for a real boundary component.
         *
         * Note that the index of a face in the boundary component need not be
         * the index of the same face in the overall triangulation.  However,
         * the index of each (<i>dim</i>-1)-face in this boundary component
         * will match the index of the corresponding top-dimensional simplex
         * in the (<i>dim</i>-1)-manifold triangulation returned by build().
         *
         * @param index the index of the desired face, ranging from 0 to
         * size()-1 inclusive.
         * @return the requested face.
         */
        Face<dim, dim-1>* facet(size_t index) const {
            return facets_[index];
        }

        /**
         * Returns the triangulation to which this boundary component belongs.
         *
         * @return the triangulation containing this boundary component.
         */
        Triangulation<dim>* triangulation() const {
            return facets_.front()->triangulation();
        }

        /**
         * Returns the connected component of the triangulation to which this
         * boundary component belongs.
         *
         * @return the component containing this boundary component.
         */
        Component<dim>* component() const {
            return facets_.front()->component();
        }

        // Make this class non-copyable.
        BoundaryComponentFaceStorage(const BoundaryComponentFaceStorage&) =
            delete;
        BoundaryComponentFaceStorage& operator = (
            const BoundaryComponentFaceStorage&) = delete;

    protected:
        /**
         * Default constructor that initialises the number of ridges to zero.
         */
        BoundaryComponentFaceStorage() : nRidges_(0) {
        }

        /**
         * Pushes the given facet (that is, (<i>dim</i>-1)-face) onto the end
         * of the list of facets of this boundary component.
         * This class does not take ownership of the given facet.
         *
         * @param facet the facet to append to the list.
         */
        void push_back(Face<dim, dim-1>* facet) {
            facets_.push_back(facet);
        }

        /**
         * Increments the number of ridges (that is, (<i>dim</i>-2)-faces) in
         * this boundary component.  Since this boundary component class does
         * not store any lower-dimensional faces, this routine does not
         * store the given face.
         */
        void push_back(Face<dim, dim-2>*) {
            ++nRidges_;
        }
};

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
 * \ifacespython This base class is not present, but the "end user"
 * class BoundaryComponent<dim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
class BoundaryComponentBase :
        public Output<BoundaryComponentBase<dim>>,
#ifdef __DOXYGEN
        // Doxygen doesn't understand std::conditional_t or ExpandSequence.
        // The syntax here is not valid C++ but for doxygen it's just fine.
        public BoundaryComponentFaceStorage<dim, 0, 1, ..., dim - 1>,
#else
        public std::conditional_t<standardDim(dim),
            ExpandSequence<BoundaryComponentFaceStorage, dim> /* all faces */,
            BoundaryComponentFaceStorage<dim, dim - 1> /* only facets */>,
#endif
        public MarkedElement {
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

    protected:
        bool orientable_;
            /**< Is this boundary component orientable? */

        EnableIf<canBuild, Triangulation<dim-1>*, nullptr> boundary_;
            /**< A full triangulation of the boundary component.
                 This may be pre-computed when the triangulation skeleton
                 is constructed, or it may be \c null in which case it
                 will be built on demand.
                 For ideal or invalid vertices, this is always \c null since
                 the triangulation is cached by the vertex class instead.*/

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
         * @return the index of this boundary component.
         */
        size_t index() const {
            return markedIndex();
        }

        /**
         * Determines if this boundary component is orientable.  If this is
         * an ideal or invalid vertex boundary component, then the
         * orientability of the corresponding vertex link is returned.
         *
         * This routine is fast; in particular, it is pre-computed and
         * does not build a full triangulation of the boundary component.
         *
         * @return \c true if and only if this boundary component is
         * orientable.
         */
        bool isOrientable() const {
            return orientable_;
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
         *   a copy of <tt>facet(i)</tt> of this boundary component,
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
         *   a copy of <tt>face<k>(i)</tt> of this boundary component,
         *   and its vertices are numbered in the same way.  As an exception,
         *   this correspondence will not hold for dimensions \a k where
         *   there exist \e pinched <i>k</i>-faces on the boundary (i.e.,
         *   faces where different sections of the boundary are pinched
         *   together, meaning that these faces must be duplicated when the
         *   boundary is triangulated).
         *
         * If this boundary component consists only of a single vertex
         * (i.e., this is an ideal or invalid vertex boundary component),
         * then this routine returns the triangulation of the corresponding
         * vertex link.  See Vertex::link() for details.
         *
         * This routine is fast, since it caches the boundary triangulation.
         * Moreover, it is guaranteed that the full skeleton of this
         * (<i>dim</i>-1)-dimensional triangulation will have been generated
         * already.
         *
         * \pre The dimension \a dim is greater than 2.
         *
         * @return the triangulation of this boundary component.
         */
        const Triangulation<dim-1>* build() const {
            // Make sure we do not try to instantiate Triangulation<1>.
            static_assert(canBuild,
                "BoundaryComponent<dim>::build() can only "
                "be used with dimensions dim > 2.");

            if (boundary_.value)
                return boundary_.value; // Already cached or pre-computed.
            if constexpr (allowVertex)
                if (this->facets().empty())
                    return this->buildVertexLink(); // Ideal or invalid vertex.

            return const_cast<BoundaryComponentBase<dim>*>(this)->
                boundary_.value = buildRealBoundary();
        }

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const {
            if constexpr (allowVertex) {
                out << (this->isIdeal() ? "Ideal " :
                        this->isInvalidVertex() ? "Invalid " :
                        "Finite ") << "boundary component";
            } else {
                out << "Boundary component";
            }
        }

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const {
            writeTextShort(out);
            out << std::endl;

            if constexpr (allowVertex) {
                if (this->isIdeal() || this->isInvalidVertex()) {
                    // We have no boundary facets.
                    Face<dim, 0>* v = std::get<0>(this->faces_).front();
                    out << "Vertex: " << v->index() << std::endl;
                    out << "Appears as:" << std::endl;
                    for (auto& emb : *v)
                        out << "  " << emb.simplex()->index()
                            << " (" << emb.vertex() << ')' << std::endl;
                    return;
                }
            }

            // We have boundary facets.
            out << (this->size() == 1 ? Strings<dim-1>::Face :
                Strings<dim-1>::Faces) << ':' << std::endl;
            for (auto s : this->facets())
                out << "  " << s->front().simplex()->index() << " ("
                    << s->front().vertices().trunc(dim) << ')' << std::endl;
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
         * Builds a new triangulation of this boundary component,
         * assuming this is a real boundary component.
         *
         * \pre The dimension \a dim is greater than 2.
         * \pre The number of (dim-1)-faces is strictly positive.
         *
         * @return the newly created boundary triangulation.
         */
        Triangulation<dim-1>* buildRealBoundary() const;

    friend class Triangulation<dim>;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina::detail

#endif
