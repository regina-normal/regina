
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

/*! \file generic/detail/boundarycomponent.h
 *  \brief Implementation details for boundary components of triangulations.
 */

#include <vector>
#include "regina-core.h"
#include "output.h"
#include "generic/alias/face.h"
#include "utilities/markedvector.h"
#include <boost/noncopyable.hpp>

namespace regina {

namespace detail {

/**
 * \weakgroup detail
 * @{
 */

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
struct WeakFaceList {
    std::vector<Face<dim, subdim>*> faces_;
        /**< The list of faces. */
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
struct WeakFaceListSuite :
        public WeakFaceListSuite<dim, subdim - 1>,
        public WeakFaceList<dim, subdim> {
};

#ifndef __DOXYGEN

template <int dim>
struct WeakFaceListSuite<dim, 0> : public WeakFaceList<dim, 0> {
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
 * \tparam allFaces \c true if this class should store all faces of all
 * dimensions 0,1,...,<i>dim</i>-1, or \c false if this class should only
 * store faces of dimension <i>dim</i>-1.
 */
template <int dim, bool allFaces>
class BoundaryComponentFaceStorage :
        protected WeakFaceListSuite<dim, dim - 1>,
        public alias::FaceOfTriangulation<
            BoundaryComponentFaceStorage<dim, true>, dim> {
    static_assert(allFaces,
        "The generic BoundaryComponentFaceStorage template should "
        "only be used with allFaces = true.");
    static_assert(standardDim(dim),
        "The BoundaryComponentFaceStorage template should only set "
        "allFaces = true for Regina's standard dimensions.");

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

    protected:
        std::vector<Face<dim, dim-1>*> simplices_;
            /**< List of all (dim-1)-simplices in the boundary component. */

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
            return simplices_.size();
        }

        /**
         * Returns the connected component of the triangulation to which this
         * boundary component belongs.
         *
         * @return the component containing this boundary component.
         */
        Component<dim>* component() const {
            return simplices_.front()->component();
        }

    protected:
        /**
         * Pushes the given face onto the end of the list of
         * (<i>dim</i>-1)-faces of this boundary component.
         * This class does not take ownership of the given face.
         *
         * @param face the face to append to the list.
         */
        void push_back(Face<dim, dim-1>* face) {
            simplices_.push_back(face);
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
 * \tparam allowVertex \c true if ideal and/or invalid vertex boundary
 * components are both possible and recognised in dimension \a dim,
 * or \c false if only real boundary components are supported.
 */
template <int dim, bool allFaces, bool allowVertex>
class BoundaryComponentFaceInterface :
        public BoundaryComponentFaceStorage<dim, allFaces> {
    static_assert(allFaces && allowVertex,
        "The generic BoundaryComponentFaceInterface template should "
        "only be used with allFaces = true and allowVertex = true.");
    // The face storage superclass already tests for standard dimension.
    static_assert(dim > 2,
        "The generic BoundaryComponentFaceInterface template should not "
        "be used for dimension 2.");

    public:
        using BoundaryComponentFaceStorage<dim, allFaces>::size;

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
            return (WeakFaceList<dim, dim-1>::faces_.empty() &&
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
            return (WeakFaceList<dim, dim-1>::faces_.empty() &&
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
                switch (dim) {
                    case 3:
                        out << (size() == 1 ? "Triangle:" : "Triangles:")
                            << std::endl;
                        break;
                    case 4:
                        out << (size() == 1 ? "Tetrahedron:" : "Tetrahedra:")
                            << std::endl;
                        break;
                    // We should never fall through to the default case.
                    // However, best to implement something regardless...
                    default:
                        out << (dim-1)
                            << (size() == 1 ? "-simplex:" : "-simplices:")
                            << std::endl;
                        break;
                }
                for (auto s : WeakFaceList<dim, dim-1>::faces_)
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
        using BoundaryComponentFaceStorage<dim, allFaces>::size;

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

            switch (dim) {
                case 2:
                    out << (size() == 1 ? "Edge:" : "Edges:")
                        << std::endl;
                    break;
                case 5:
                    out << (size() == 1 ? "Pentachoron:" : "Pentachora:")
                        << std::endl;
                    break;
                default:
                    out << (dim-1)
                        << (size() == 1 ? "-simplex:" : "-simplices:")
                        << std::endl;
                    break;
            }
            for (auto s : WeakFaceList<dim, dim-1>::faces_)
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
 * \tparam canBuild \c true if we support triangulating boundary components,
 * or \c false if this is not possible (i.e., the dimension is 2).
 */
template <int dim, bool allFaces, bool allowVertex, bool canBuild>
class BoundaryComponentStorage :
        public BoundaryComponentFaceInterface<dim, allFaces, allowVertex> {
    static_assert(canBuild,
        "The generic BoundaryComponentStoarge template should only be "
        "used with canBuild = true.");
    static_assert(dim > 2,
        "The generic BoundaryComponentStorage template cannot be used "
        "for dimension 2.");

    protected:
        Triangulation<dim-1>* boundary_;
            /**< A full triangulation of the boundary component.
                 If this boundary component is an ideal or invalid vertex,
                 then this will be a null pointer. */

    public:
        /**
         * Destroys this object.  The cached boundary component triangulation
         * will be destroyed also.
         */
        ~BoundaryComponentStorage() {
            delete boundary_;
        }

        /**
         * Returns the full (<i>dim</i>-1)-dimensional triangulation of this
         * boundary component.  Note that this triangulation is read-only
         * (though of course you can clone it and then operate upon the clone).
         *
         * If this is a real boundary component (i.e., it is built from
         * one or more (<i>dim</i>-1)-faces), then the triangulation of this
         * boundary component is as follows.
         * Let \a i lie between 0 and size()-1 inclusive.  Then simplex \a i
         * of the returned (<i>dim</i>-1)-dimensional triangulation is
         * a copy of <tt>face<dim-1>(i)</tt> of this boundary component,
         * and its vertices 0,...,<i>dim</i>-1 are numbered in the
         * same way.  To relate these (<i>dim</i>-1)-face vertex numbers to
         * the vertex numbers of top-dimensional simplices in the overall
         * <i>dim</i>-dimensional triangulation, see
         * Simplex<dim>::faceMapping<dim-1>().
         *
         * If this boundary component consists only of a single vertex
         * (i.e., this is an ideal or invalid vertex boundary component),
         * then this routine returns the triangulation of the corresponding
         * vertex link.  See Vertex::link() for details.
         *
         * This routine is fast (it uses a pre-computed triangulation).
         * Moreover, it is guaranteed that the full skeleton of this
         * (<i>dim</i>-1)-dimensional triangulation will have been generated
         * already.
         *
         * @return the triangulation of this boundary component.
         */
        const Triangulation<dim-1>* build() const {
            return (boundary_ ? boundary_ :
                BoundaryComponentFaceInterface<dim, allFaces, allowVertex>::
                buildVertexLink());
        }

    protected:
        /**
         * Initialises the cached boundary triangulation to \c null.
         */
        BoundaryComponentStorage() : boundary_(0) {
        }
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

    protected:
        /**
         * Default constructor.
         */
        BoundaryComponentBase() = default;

    friend class Triangulation<dim>;
        /**< Allow access to private members. */
};

/*@}*/

} } // namespace regina::detail

#endif
