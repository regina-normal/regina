
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

#ifndef __BOUNDARYCOMPONENT_H_DETAIL
#ifndef __DOXYGEN
#define __BOUNDARYCOMPONENT_H_DETAIL
#endif

/*! \file triangulation/detail/boundarycomponent.h
 *  \brief Implementation details for boundary components of triangulations.
 */

#include <vector>
#include "regina-core.h"
#include "output.h"
#include "triangulation/alias/face.h"
#include "triangulation/detail/strings.h"
#include "triangulation/forward.h"
#include "utilities/markedvector.h"
#include <boost/noncopyable.hpp>

namespace regina {
namespace detail {

template <int> class TriangulationBase;
template <int, bool> class BoundaryComponentFaceStorage;

/**
 * \weakgroup detail
 * @{
 */

/**
 * Helper class that indicates what data type is used by a boundary
 * component class to store a list of <i>subdim</i>-faces.
 *
 * This is only relevant for boundary components in Regina's
 * \ref stddim "standard dimensions", since boundary components in
 * higher dimensions do not store their lower-dimensional faces.
 */
template <int dim, int subdim>
struct FaceListHolder<BoundaryComponentFaceStorage<dim, true>, subdim> {
    /**
     * The data type used by BoundaryComponent<dim> to store the list of all
     * <i>subdim</i>-faces of the boundary component.
     *
     * The function BoundaryComponent<dim>::faces<subdim>() returns a const
     * reference to this type.
     */
    typedef std::vector<Face<dim, subdim>*> Holder;
};

/**
 * Internal class that stores all <i>subdim</i>-faces in a component or
 * boundary component of a <i>dim</i>-dimensional triangulation.
 *
 * This class is very basic (hence the name "weak").  In particular:
 *
 * - the integer returned by Face::index() has no relation to the index
 *   of the corresponding face in this list;
 *
 * - this list makes no attempt to claim and/or manage ownership of the
 *   faces that it stores.
 *
 * \ifacespython Not present.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the dimension of the faces that this class stores.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class WeakFaceList {
    protected:
        std::vector<Face<dim, subdim>*> faces_;
            /**< The list of faces. */

    private:
        /**
         * An input iterator that runs through the faces of this list in
         * order and converts them to the corresponding faces from some
         * other triangulation \a tri.
         *
         * The iterator relies on an array \a map, where for each face
         * \a f in this list, <tt>map[f->index()]</tt> is the corresponding
         * face of \a tri.  Note that <tt>f->index()</tt> is the index of
         * \a f in its underlying <i>dim</i>-dimensional triangulation,
         * \e not the index of \a f in this list.
         */
        template <int tridim>
        class ReorderIterator {
            private:
                typename std::vector<Face<dim, subdim>*>::const_iterator it_;
                    /**< Points to the current face of this list. */
                Face<tridim, subdim>** map_;
                    /**< The map that converts faces of this list to faces of
                         the other triangulation \a tri, as described in the
                         class notes. */

            public:
                /**
                 * Creates an uninitialised iterator.
                 */
                ReorderIterator() : it_(), map_(0) {
                }

                /**
                 * Creates an iterator that points to the given face of
                 * this list, using the given map to convert faces of
                 * this list to faces of the other triangulation \a tri.
                 * See the iterator class notes for details.
                 */
                ReorderIterator(
                        typename std::vector<Face<dim, subdim>*>::
                            const_iterator it,
                        Face<tridim, subdim>** map) : it_(it), map_(map) {
                }

                /**
                 * Copy constructor.
                 */
                ReorderIterator(const ReorderIterator&) = default;

                /**
                 * Tests whether this and the given iterator point to
                 * the same face.
                 */
                bool operator == (const ReorderIterator& rhs) const {
                    return it_ == rhs.it_;
                }

                /**
                 * Tests whether this and the given iterator point to
                 * different faces.
                 */
                bool operator != (const ReorderIterator& rhs) const {
                    return it_ != rhs.it_;
                }

                /**
                 * Preincrement operator that steps to the next face in this
                 * list.
                 */
                ReorderIterator& operator ++() {
                    ++it_;
                    return *this;
                }

                /**
                 * Postincrement operator that steps to the next face in this
                 * list.
                 */
                ReorderIterator operator ++(int) {
                    ReorderIterator prev(*this);
                    ++it_;
                    return prev;
                }

                /**
                 * Returns the face of the other triangulation \a tri that
                 * corresponds to the current face in this list.  See
                 * the iterator class notes for details.
                 */
                Face<tridim, subdim>* operator * () const {
                    return map_[(*it_)->index()];
                }
        };

    protected:
        /**
         * Reorders and relabels all <i>subdim</i>-faces of the given
         * triangulation so that they appear in the same order as the
         * corresponding faces in this list, and so that their vertices
         * are numbered in a corresponding way.
         *
         * \pre The <i>subdim</i>-faces of the given triangulation \a tri
         * are in one-to-one correspondence with the <i>subdim</i>-faces in
         * this list, though not necessarily in the same order.  Moreover,
         * for each \a i and \a j, this correspondence maps the <i>i</i>th
         * <i>subdim</i>-face of <tt>tri->simplex(j)</tt> to the <i>i</i>th
         * <i>subdim</i>-face of <tt>tridimFaces[j]</tt>.
         *
         * \tparam tridim the dimension of the given triangulation.
         * This must be strictly larger than \a subdim, but it need not
         * be equal to \a dim.
         *
         * @param tri a <i>tridim</i>-dimensional triangulation, as
         * described above.
         * @param tridimFaces a list of <i>tridim</i>-faces that together
         * contain all of the faces in this list, and that are in an
         * \e ordered one-to-one correspondence with the top-dimensional
         * simplices of \a tri as described in the precondition above.
         */
        template <int tridim>
        void reorderAndRelabelFaces(Triangulation<tridim>* tri,
                const std::vector<Face<dim, tridim>*>& tridimFaces) const {
            static_assert(tridim > subdim,
                "reorderAndRelabelFaces() should only be used with "
                "triangulations of dimension tridim > subdim.");

            if (faces_.empty())
                return; // Should never happen.

            // Build a map from
            // subdim-face indices in the d-dim triang -> subdim-faces in tri.
            //
            // This is a partial function: it is only defined for indices
            // of *boundary* subdim-faces in the d-dim triang.
            // We leave the other values of the map uninitialised.
            Face<tridim, subdim>** map = new Face<tridim, subdim>*[
                faces_.front()->triangulation()->template countFaces<subdim>()];

            for (Face<tridim, subdim>* f : tri->template faces<subdim>()) {
                const auto& emb = f->front();
                Face<dim, tridim>* outer = tridimFaces[emb.simplex()->index()];
                map[outer->template face<subdim>(emb.face())->index()] = f;

                // While we have the two corresponding faces in front of us,
                // relabel the vertices of f now.
                //
                // The following two permutations should be equal:
                //
                // - in tridim: emb.simplex()->faceMapping<subdim>(emb.face())
                // - in dim: outer->faceMapping<subdim>(emb.face())
                //
                // The mapping we need to adjust is in tridim.

                Perm<tridim+1> adjust =
                    emb.simplex()->template faceMapping<subdim>(
                        emb.face()).inverse() *
                    Perm<tridim+1>::contract(
                        outer->template faceMapping<subdim>(emb.face()));
                adjust.clear(subdim + 1);
                tri->relabelFace(f, adjust);
            }

            tri->template reorderFaces<subdim>(
                ReorderIterator<tridim>(faces_.begin(), map),
                ReorderIterator<tridim>(faces_.end(), map));

            delete[] map;
        }
};

/**
 * Internal class that helps a component or boundary component store its
 * lists of faces.
 *
 * This class is used with <i>dim</i>-dimensional triangulations.  It provides
 * storage for faces of all dimensions \a subdim and below.
 *
 * \ifacespython Not present.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the maximum dimension of the faces that this class stores.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class WeakFaceListSuite :
        public WeakFaceListSuite<dim, subdim - 1>,
        public WeakFaceList<dim, subdim> {
    protected:
        /**
         * Reorders and relabels all faces of all dimensions 0,...,\a subdim of
         * the given triangulation, so that for each \a k, the <i>k</i>-faces of
         * the given triangulation appear in the same order as the corresponding
         * <i>k</i>-faces in this suite, and have their vertices numbered
         * in a corresponding way.
         *
         * \pre For each dimension \a k = 0,...,\a subdim, the <i>k</i>-faces
         * of the given triangulation \a tri are in one-to-one correspondence
         * with the <i>k</i>-faces in this suite, though not necessarily in the
         * same order.  Moreover, for each \a i and \a j, this correspondence
         * maps the <i>i</i>th <i>k</i>-face of <tt>tri->simplex(j)</tt> to the
         * <i>i</i>th <i>k</i>-face of <tt>tridimFaces[j]</tt>.
         *
         * \tparam tridim the dimension of the given triangulation.
         * This must be strictly larger than \a subdim, but it need not
         * be equal to \a dim.
         *
         * @param tri a <i>tridim</i>-dimensional triangulation, as
         * described above.
         * @param tridimFaces a list of <i>tridim</i>-faces that together
         * contain all of the faces in this suite, and that are in an
         * \e ordered one-to-one correspondence with the top-dimensional
         * simplices of \a tri as described in the precondition above.
         */
        template <int tridim>
        void reorderAndRelabelFaces(Triangulation<tridim>* tri,
                const std::vector<Face<dim, tridim>*>& tridimFaces) const {
            WeakFaceListSuite<dim, subdim - 1>::reorderAndRelabelFaces(
                tri, tridimFaces);
            WeakFaceList<dim, subdim>::reorderAndRelabelFaces(tri, tridimFaces);
        }
};

#ifndef __DOXYGEN

template <int dim>
class WeakFaceListSuite<dim, 0> : public WeakFaceList<dim, 0> {
    protected:
        template <int tridim>
        void reorderAndRelabelFaces(Triangulation<tridim>* tri,
                const std::vector<Face<dim, tridim>*>& tridimFaces) const {
            WeakFaceList<dim, 0>::reorderAndRelabelFaces(tri, tridimFaces);
        }
};

#endif // __DOXYGEN

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
 * \tparam allFaces_ \c true if this class should store all faces of all
 * dimensions 0,1,...,<i>dim</i>-1, or \c false if this class should only
 * store faces of dimension <i>dim</i>-1.
 */
template <int dim, bool allFaces_>
class BoundaryComponentFaceStorage :
        protected WeakFaceListSuite<dim, dim - 1>,
        public alias::FacesOfTriangulation<
            BoundaryComponentFaceStorage<dim, true>, dim>,
        public alias::FaceOfTriangulation<
            BoundaryComponentFaceStorage<dim, true>, dim> {
    static_assert(allFaces_,
        "The generic BoundaryComponentFaceStorage template should "
        "only be used with allFaces = true.");
    static_assert(standardDim(dim),
        "The BoundaryComponentFaceStorage template should only set "
        "allFaces = true for Regina's standard dimensions.");

    public:
        /**
         * A compile-time constant indicating whether this boundary
         * component class stores all lower-dimensional faces (\c true),
         * or only faces of dimension <i>dim</i>-1 (\c false).
         *
         * This is a compile-time constant only, with no linkage - any attempt
         * to create a reference or pointer to it will give a linker error.
         *
         * \ifacespython Not present.
         */
        static constexpr bool allFaces = true;

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
            return WeakFaceList<dim, dim-1>::faces_.size();
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
            return WeakFaceList<dim, dim-2>::faces_.size();
        }

        /**
         * Returns the number of <i>subdim</i>-faces in this boundary component.
         *
         * This routine is only available where \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>countFaces(subdim)</tt>; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * \tparam subdim the dimension of the faces to query.  This must
         * be between 0 and <i>dim</i>-1 inclusive.
         *
         * @return the number of <i>subdim</i>-faces.
         */
        template <int subdim>
        size_t countFaces() const {
            return WeakFaceList<dim, subdim>::faces_.size();
        }

        /**
         * Returns all (<i>dim</i>-1)-faces in this boundary component.
         *
         * The reference that is returned will remain valid only for as
         * long as this boundary component object exists.  In particular,
         * the reference will become invalid any time that the triangulation
         * changes (since all boundary component objects will be destroyed
         * and others rebuilt in their place).
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all (<i>dim</i>-1)-faces.
         */
        const std::vector<Face<dim, dim-1>*>& facets() const {
            return WeakFaceList<dim, dim-1>::faces_;
        }

        /**
         * Returns all <i>subdim</i>-faces in this boundary component.
         *
         * The reference that is returned will remain valid only for as
         * long as this boundary component object exists.  In particular,
         * the reference will become invalid any time that the triangulation
         * changes (since all boundary component objects will be destroyed
         * and others rebuilt in their place).
         *
         * \ifacespython Python users should call this function in the
         * form <tt>faces(subdim)</tt>.  It will then return a Python list
         * containing all the <i>subdim</i>-faces of the boundary component.
         * Be warned that, unlike in C++, this Python list will be a
         * snapshot of the faces when this function is called, and will
         * \e not be kept up-to-date as the triangulation changes.
         *
         * \tparam subdim the dimension of the faces to query.  This must
         * be between 0 and <i>dim</i>-1 inclusive.
         *
         * @return access to the list of all <i>subdim</i>-faces.
         */
        template <int subdim>
        const std::vector<Face<dim, subdim>*>& faces() const {
            return WeakFaceList<dim, subdim>::faces_;
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
            return WeakFaceList<dim, dim-1>::faces_[index];
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
         * This routine is only available where \a dim is one of Regina's
         * \ref stddim "standard dimensions".
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>face(subdim, index)</tt>; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * \tparam subdim the dimension of the face to query.  This must
         * be between 0 and <i>dim</i>-1 inclusive.
         *
         * @param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * @return the requested face.
         */
        template <int subdim>
        Face<dim, subdim>* face(size_t index) const {
            return WeakFaceList<dim, subdim>::faces_[index];
        }

        /**
         * Returns the triangulation to which this boundary component belongs.
         *
         * @return the triangulation containing this boundary component.
         */
        Triangulation<dim>* triangulation() const {
            // There may be no (dim-1)-simplices, but there is always a vertex.
            return WeakFaceList<dim, 0>::faces_.front()->triangulation();
        }

        /**
         * Returns the connected component of the triangulation to which this
         * boundary component belongs.
         *
         * @return the component containing this boundary component.
         */
        Component<dim>* component() const {
            // There may be no (dim-1)-simplices, but there is always a vertex.
            return WeakFaceList<dim, 0>::faces_.front()->component();
        }

    protected:
        /**
         * Pushes the given face onto the end of the list of
         * <i>subdim</i>-faces of this boundary component.
         * This class does not take ownership of the given face.
         *
         * \tparam subdim the dimension of the face to append.  This must
         * be between 0 and <i>dim</i>-1 inclusive.
         *
         * @param face the face to append to the list.
         */
        template <int subdim>
        void push_back(Face<dim, subdim>* face) {
            WeakFaceList<dim, subdim>::faces_.push_back(face);
        }

        /**
         * Reorders all lower-dimensional faces of the given triangulation
         * so that they appear in the same order as the corresponding
         * faces of this boundary component, and relabels these faces so
         * that their vertices are numbered in a corresponding way.
         * This affects all faces of dimensions 0,...,(<i>dim</i>-2).
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
            WeakFaceListSuite<dim, dim - 2>::reorderAndRelabelFaces(tri,
                WeakFaceList<dim, dim - 1>::faces_);
        }
};

/**
 * Helper class for storing the necessary faces of a boundary component of a
 * <i>dim</i>-dimensional triangulation.  See the general
 * BoundaryComponentFaceStorage template notes for further details.
 *
 * This specialisation is used for dimensions in which only
 * (<i>dim</i>-1)-dimensional faces are stored.  It therefore removes the
 * member functions for accessing lower-dimensional faces.
 */
template <int dim>
class BoundaryComponentFaceStorage<dim, false> {
    static_assert(! standardDim(dim),
        "The BoundaryComponentFaceStorage template should not set "
        "allFaces = false for Regina's standard dimensions.");

    public:
        /**
         * A compile-time constant indicating whether this boundary
         * component class stores all lower-dimensional faces (\c true),
         * or only faces of dimension <i>dim</i>-1 (\c false).
         *
         * This is a compile-time constant only, with no linkage - any attempt
         * to create a reference or pointer to it will give a linker error.
         *
         * \ifacespython Not present.
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
         * Returns all (<i>dim</i>-1)-faces in this boundary component.
         *
         * The reference that is returned will remain valid only for as
         * long as this boundary component object exists.  In particular,
         * the reference will become invalid any time that the triangulation
         * changes (since all boundary component objects will be destroyed
         * and others rebuilt in their place).
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all (<i>dim</i>-1)-faces.
         */
        const std::vector<Face<dim, dim-1>*>& facets() const {
            return facets_;
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

    protected:
        /**
         * Default constructor that initialises the number of ridges to zero.
         */
        BoundaryComponentFaceStorage() : nRidges_(0) {
        }

        /**
         * Pushes the given face onto the end of the list of
         * (<i>dim</i>-1)-faces of this boundary component.
         * This class does not take ownership of the given face.
         *
         * @param face the face to append to the list.
         */
        void push_back(Face<dim, dim-1>* face) {
            facets_.push_back(face);
        }

        /**
         * Increments the number of (<i>dim</i>-2)-faces in this
         * boundary component.  Since this boundary component class does
         * not store any lower-dimensional faces, this routine does not
         * store the given face.
         */
        void push_back(Face<dim, dim-2>*) {
            ++nRidges_;
        }

        /**
         * Does nothing, since this boundary component does not store
         * lower-dimensional faces.
         *
         * \tparam subdim the dimension of the given face.  This must
         * be between 0 and <i>dim</i>-3 inclusive.
         */
        template <int subdim>
        void push_back(Face<dim, subdim>*) {
            static_assert(subdim <= dim - 3,
                "The no-op push_back() should only be called for "
                "faces of dimension <= dim - 3.");
        }

        /**
         * Reorders all lower-dimensional faces of the given triangulation
         * so that they appear in the same order as the corresponding
         * faces of this boundary component, and relabels these faces so
         * that their vertices are numbered in a corresponding way.
         * This affects all faces of dimensions 0,...,(<i>dim</i>-2).
         *
         * In this specialised class template, this function does nothing
         * because faces of dimension 0,...,(<i>dim</i>-2) are not stored.
         */
        void reorderAndRelabelFaces(Triangulation<dim-1>*) const {
        }
};

/**
 * Helper class for querying the faces of a boundary component of a
 * <i>dim</i>-dimensional triangulation.  Every class BoundaryComponent<dim>
 * inherits from this template.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * BoundaryComponent<dim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam allFaces \c true if this class should store all faces of all
 * dimensions 0,1,...,<i>dim</i>-1, or \c false if this class should only
 * store faces of dimension <i>dim</i>-1.
 * \tparam allowVertex_ \c true if ideal and/or invalid vertex boundary
 * components are both possible and recognised in dimension \a dim,
 * or \c false if only real boundary components are supported.
 */
template <int dim, bool allFaces, bool allowVertex_>
class BoundaryComponentFaceInterface :
        public BoundaryComponentFaceStorage<dim, allFaces> {
    static_assert(allFaces && allowVertex_,
        "The generic BoundaryComponentFaceInterface template should "
        "only be used with allFaces = true and allowVertex = true.");
    // The face storage superclass already tests for standard dimension.
    static_assert(dim > 2,
        "The generic BoundaryComponentFaceInterface template should not "
        "be used for dimension 2.");

    public:
        /**
         * A compile-time constant indicating whether ideal and/or
         * invalid vertex boundary components are both possible and
         * recognised by this boundary component class.
         *
         * This is a compile-time constant only, with no linkage - any attempt
         * to create a reference or pointer to it will give a linker error.
         *
         * \ifacespython Not present.
         */
        static constexpr bool allowVertex = true;

    public:
        using BoundaryComponentFaceStorage<dim, allFaces>::size;
        using BoundaryComponentFaceStorage<dim, allFaces>::facets;

        /**
         * Determines if this boundary component is real.
         * This is the case if and only if it is formed from one or more
         * (dim-1)-faces.
         *
         * See the BoundaryComponent class notes for an overview of real,
         * ideal, and invalid vertex boundary components.
         *
         * This routine is only available where \a dim is at least 3
         * and is one of Regina's \ref stddim "standard dimensions".
         * (In other dimensions, real boundary components are the only
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
         * This routine is only available where \a dim is at least 3
         * and is one of Regina's \ref stddim "standard dimensions".
         *
         * @return \c true if and only if this boundary component is ideal.
         */
        bool isIdeal() const {
            // Either of Vertex::isValid() or Vertex::isIdeal() will do here.
            return (facets().empty() &&
                WeakFaceList<dim, 0>::faces_.front()->isValid());
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
         * This routine is only available where \a dim is at least 3
         * and is one of Regina's \ref stddim "standard dimensions".
         *
         * @return \c true if and only if this boundary component consists of a
         * single invalid vertex and nothing else.
         */
        bool isInvalidVertex() const {
            return (facets().empty() &&
                ! WeakFaceList<dim, 0>::faces_.front()->isValid());
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
            out << (isIdeal() ? "Ideal " : isInvalidVertex() ? "Invalid " :
                "Finite ") << "boundary component";
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

            if (isIdeal() || isInvalidVertex()) {
                Face<dim, 0>* v = WeakFaceList<dim, 0>::faces_.front();
                out << "Vertex: " << v->index() << std::endl;
                out << "Appears as:" << std::endl;
                for (auto& emb : *v)
                    out << "  " << emb.simplex()->index()
                        << " (" << emb.vertex() << ')' << std::endl;
            } else {
                out << (size() == 1 ? Strings<dim-1>::Face :
                    Strings<dim-1>::Faces) << ':' << std::endl;
                for (auto s : facets())
                    out << "  " << s->front().simplex()->index() << " ("
                        << s->front().vertices().trunc(dim) << ')' << std::endl;
            }
        }

    protected:
        /**
         * Triangulates the vertex link for an ideal or invalid vertex
         * boundary component.
         *
         * @return the triangulated vertex link.
         */
        const Triangulation<dim-1>* buildVertexLink() const {
            return WeakFaceList<dim, 0>::faces_.front()->buildLink();
        }
};

/**
 * Helper class for querying the faces of a boundary component of a
 * <i>dim</i>-dimensional triangulation.  See the general
 * BoundaryComponentFaceInterface template notes for further details.
 *
 * This specialisation is used for dimensions in which only real boundary
 * components are supported - that is, ideal and/or invalid vertex
 * boundary components are either not recognised or not possible.
 * It therefore removes the member functions that query ideal and/or
 * invalid vertices.
 */
template <int dim, bool allFaces>
class BoundaryComponentFaceInterface<dim, allFaces, false> :
        public BoundaryComponentFaceStorage<dim, allFaces> {
    static_assert(dim == 2 || ! standardDim(dim),
        "The BoundaryComponentFaceInterface template should not set "
        "allowVertex = false for standard dimensions greater than 2.");

    public:
        /**
         * A compile-time constant indicating whether ideal and/or
         * invalid vertex boundary components are both possible and
         * recognised by this boundary component class.
         *
         * This is a compile-time constant only, with no linkage - any attempt
         * to create a reference or pointer to it will give a linker error.
         *
         * \ifacespython Not present.
         */
        static constexpr bool allowVertex = false;

    public:
        using BoundaryComponentFaceStorage<dim, allFaces>::size;
        using BoundaryComponentFaceStorage<dim, allFaces>::facets;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const {
            out << "Boundary component";
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

            out << (size() == 1 ? Strings<dim-1>::Face :
                Strings<dim-1>::Faces) << ':' << std::endl;
            for (auto s : facets())
                out << "  " << s->front().simplex()->index() << " ("
                    << s->front().vertices().trunc(dim) << ')' << std::endl;
        }

    protected:
        /**
         * Always returns \c null.
         *
         * In general, this routine triangulates the vertex link for an
         * ideal or invalid vertex boundary component.  However, this
         * specialisation is used for cases where such boundary components
         * are either not recognised or not possible, and so this routine
         * returns \c null always.
         *
         * @return \c null.
         */
        constexpr const Triangulation<dim-1>* buildVertexLink() const {
            // Make sure we do not try to instantiate Triangulation<1>.
            static_assert(dim > 2, "Routine buildVertexLink() should "
                "not be called for dimension 2.");

            return 0;
        }
};

/**
 * Helper class that manages all data storage for a boundary component of a
 * <i>dim</i>-dimensional triangulation.  Every class BoundaryComponent<dim>
 * inherits from this template.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * BoundaryComponent<dim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam allFaces \c true if this class should store all faces of all
 * dimensions 0,1,...,<i>dim</i>-1, or \c false if this class should only
 * store faces of dimension <i>dim</i>-1.
 * \tparam allowVertex \c true if ideal and/or invalid vertex boundary
 * components are both possible and recognised in dimension \a dim,
 * or \c false if only real boundary components are supported.
 * \tparam canBuild_ \c true if we support triangulating boundary components,
 * or \c false if this is not possible (i.e., the dimension is 2).
 */
template <int dim, bool allFaces, bool allowVertex, bool canBuild_>
class BoundaryComponentStorage :
        public BoundaryComponentFaceInterface<dim, allFaces, allowVertex> {
    static_assert(canBuild_,
        "The generic BoundaryComponentStoarge template should only be "
        "used with canBuild = true.");
    static_assert(dim > 2,
        "The generic BoundaryComponentStorage template cannot be used "
        "for dimension 2.");

    public:
        /**
         * A compile-time constant indicating whether this boundary
         * component class supports triangulating boundary components.
         *
         * This is a compile-time constant only, with no linkage - any attempt
         * to create a reference or pointer to it will give a linker error.
         *
         * \ifacespython Not present.
         */
        static constexpr bool canBuild = true;

    protected:
        Triangulation<dim-1>* boundary_;
            /**< A full triangulation of the boundary component.
                 This may be pre-computed when the triangulation skeleton
                 is constructed, or it may be \c null in which case it
                 will be built on demand.
                 For ideal or invalid vertices, this is always \c null since
                 the triangulation is cached by the vertex class instead.*/

    public:
        using BoundaryComponentFaceStorage<dim, allFaces>::facets;

        /**
         * Destroys this object.  The cached boundary component triangulation
         * will be destroyed also.
         */
        ~BoundaryComponentStorage();

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
         *   if the template argument \a allFaces is \c true), then a similar
         *   correspondence holds for these lower-dimensional faces also:
         *   for each \a i, <i>k</i>-face \a i of the returned triangulation is
         *   a copy of <tt>face<k>(i)</tt> of this boundary component,
         *   and its vertices are numbered in the same way.
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
         * @return the triangulation of this boundary component.
         */
        const Triangulation<dim-1>* build() const {
            if (boundary_)
                return boundary_; // Already cached or pre-computed.
            if (facets().empty())
                return buildVertexLink(); // Ideal or invalid vertex.

            return (
                const_cast<BoundaryComponentStorage<dim, allFaces,
                    allowVertex, canBuild_>*>(this)->
                boundary_ = buildRealBoundary());
        }

    protected:
        using BoundaryComponentFaceInterface<dim, allFaces, allowVertex>::
            buildVertexLink;

        /**
         * Initialises the cached boundary triangulation to \c null.
         */
        BoundaryComponentStorage() : boundary_(0) {
        }

    private:
        /**
         * Builds a new triangulation of this boundary component,
         * assuming this is a real boundary component.
         *
         * \pre The number of (dim-1)-faces is strictly positive.
         *
         * @return the newly created boundary triangulation.
         */
        Triangulation<dim-1>* buildRealBoundary() const;
};

/**
 * Helper class that manages all data storage for a boundary component of a
 * <i>dim</i>-dimensional triangulation.  See the general
 * BoundaryComponentStorage template notes for further details.
 *
 * This specialisation is used for dimensions in which you cannot triangulate
 * boundary components (i.e., dimension 2).  It therefore removes the
 * member functions that work with boundary component triangulations.
 */
template <int dim, bool allFaces, bool allowVertex>
class BoundaryComponentStorage<dim, allFaces, allowVertex, false> :
        public BoundaryComponentFaceInterface<dim, allFaces, allowVertex> {
    static_assert(dim == 2,
        "The BoundaryComponentStorage template should not set "
        "canBuild = false for dimensions greater than 2.");

    public:
        /**
         * A compile-time constant indicating whether this boundary
         * component class supports triangulating boundary components.
         *
         * This is a compile-time constant only, with no linkage - any attempt
         * to create a reference or pointer to it will give a linker error.
         *
         * \ifacespython Not present.
         */
        static constexpr bool canBuild = false;
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
        public BoundaryComponentStorage<dim,
            standardDim(dim) /* allFaces */,
            (standardDim(dim) && dim > 2) /* allowVertex */,
            (dim > 2) /* canBuild */>,
        public boost::noncopyable,
        public MarkedElement {
    protected:
        bool orientable_;
            /**< Is this boundary component orientable? */

    public:
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

    protected:
        /**
         * Default constructor that leaves orientability uninitialised.
         */
        BoundaryComponentBase() = default;

    friend class Triangulation<dim>;
        /**< Allow access to private members. */
};

/*@}*/

} } // namespace regina::detail

#endif
