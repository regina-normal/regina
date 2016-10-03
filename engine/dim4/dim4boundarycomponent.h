
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

#ifndef __DIM4BOUNDARYCOMPONENT_H
#ifndef __DOXYGEN
#define __DIM4BOUNDARYCOMPONENT_H
#endif

/*! \file dim4/dim4boundarycomponent.h
 *  \brief Deals with components of the boundary of a 4-manifold triangulation.
 */

#include <vector>
#include "regina-core.h"
#include "output.h"
#include "generic/alias/face.h"
#include "utilities/markedvector.h"
#include <boost/noncopyable.hpp>
// NOTE: More #includes follow after the class declarations.

namespace regina {

template <int> class Component;
template <int> class Triangulation;
template <int, int> class Face;
typedef Component<4> Dim4Component;
typedef Triangulation<3> NTriangulation;
typedef Face<4, 3> Dim4Tetrahedron;
typedef Face<4, 2> Dim4Triangle;
typedef Face<4, 1> Dim4Edge;
typedef Face<4, 0> Dim4Vertex;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a component of the boundary of a 4-manifold triangulation.
 *
 * Note that an ideal vertex constitutes a boundary component of its
 * own - it consists of one vertex, no edges, no triangles, and no tetrahedra.
 * Likewise, an invalid vertex is placed in its own boundary
 * component if it does not already belong to some larger boundary
 * component (for instance, if its link is an ideal 3-manifold triangulation).
 * See Dim4Vertex::isBoundary() for details.
 *
 * We can encounter some interesting cases with invalid triangulations.
 * Consider some vertex in a 4-manifold triangulation.  If the link of
 * this vertex is itself a 3-manifold with more than one boundary component
 * (which makes the vertex invalid), then different parts of the 4-manifold
 * boundary are effectively "pinched" together.  If this happens, the
 * different parts of the boundary that are pinched might or might not be
 * listed as part of the same boundary component; if not then the
 * offending vertex will be included in all of these boundary components.
 * Nevertheless, only one of these can be considered the "official" boundary
 * component of the vertex as returned by Dim4Vertex::boundaryComponent().
 * This is all a bit of a mess (and even more so when you consider that
 * the same thing can happen with invalid edges), but then again the
 * entire 4-manifold triangulation is invalid and so you almost certainly
 * have bigger problems to deal with.
 *
 * Boundary components are highly temporary; once a triangulation
 * changes, all its boundary component objects will be deleted and new
 * ones will be created.
 */
class REGINA_API Dim4BoundaryComponent :
        public Output<Dim4BoundaryComponent>,
        public alias::FaceOfTriangulation<Dim4BoundaryComponent, 4>,
        public boost::noncopyable,
        public MarkedElement {
    private:
        std::vector<Dim4Tetrahedron*> tetrahedra_;
            /**< List of tetrahedra in the boundary component. */
        std::vector<Dim4Triangle*> triangles_;
            /**< List of triangles in the boundary component. */
        std::vector<Dim4Edge*> edges_;
            /**< List of edges in the boundary component. */
        std::vector<Dim4Vertex*> vertices_;
            /**< List of vertices in the boundary component. */

        NTriangulation* boundary_;
            /**< A full triangulation of the boundary component formed
                 by joining together individual boundary tetrahedra.
                 If this boundary component does not contain any tetrahedra
                 (such as an ideal boundary component) then this will be
                 the null pointer. */

    public:
        /**
         * Default destructor.
         */
        ~Dim4BoundaryComponent();

        /**
         * Returns the index of this boundary component in the underlying
         * triangulation.
         *
         * @return the index of this boundary component.
         */
        REGINA_INLINE_REQUIRED
        size_t index() const;

        /**
         * Returns the number of <i>subdim</i>-faces in this boundary component.
         *
         * \pre The template argument \a subdim is between 0 and 3 inclusive.
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>countFaces(subdim)</tt>; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * @return the number of <i>subdim</i>-faces.
         */
        template <int subdim>
        size_t countFaces() const;

        /**
         * Returns the requested <i>subdim</i>-face in this boundary component.
         *
         * Note that the index of a face in the boundary component need
         * not be the index of the same face in the overall triangulation.
         * However, if this boundary component is built from one or more
         * tetrahedra (i.e., it is not ideal), then the index of each
         * <i>subdim</i>-face in this boundary component
         * matches the index of the corresponding <i>subdim</i>-face in the
         * 3-manifold triangulation returned by build().
         *
         * \pre The template argument \a subdim is between 0 and 3 inclusive.
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>face(subdim, index)</tt>; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * @param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * @return the requested face.
         */
        template <int subdim>
        Face<4, subdim>* face(size_t index) const;

        /**
         * Returns the component of the triangulation to which this
         * boundary component belongs.
         *
         * @return the component containing this boundary component.
         */
        Dim4Component* component() const;

        /**
         * Returns the full 3-manifold triangulation of this boundary
         * component.  Note that this triangulation is read-only (though
         * of course you can clone it and then operate upon the clone).
         *
         * If this boundary component contains one or more tetrahedra
         * (i.e., it is a regular boundary built from pentachoron facets),
         * then the triangulation of this boundary component is as follows.
         * Let \a i lie between 0 and countTetrahedra()-1 inclusive.
         * Then tetrahedron \a i of the boundary 3-manifold triangulation is
         * a copy of tetrahedron <tt>tetrahedron(i)</tt> of this 4-manifold
         * boundary component, and its vertices 0,1,2,3 are numbered in the
         * same way.  To relate tetrahedron vertex numbers to pentachoron
         * vertex numbers, see Dim4Pentachoron::tetrahedronMapping().
         *
         * If this boundary component consists only of a single vertex
         * (which happens with ideal vertices and also some invalid vertices),
         * then this routine returns the triangulation of the corresponding
         * vertex link.  See Dim4Vertex::link() for details.
         *
         * This routine is fast (it uses a pre-computed triangulation).
         * Moreover, it is guaranteed that the full skeleton of this
         * 3-manifold triangulation will have been generated already.
         *
         * @return the triangulation of this boundary component.
         */
        const NTriangulation* build() const;

        /**
         * Determines if this boundary component is ideal.
         * This is the case if and only if it consists of a single
         * ideal vertex and no tetrahedra.
         *
         * Note that a boundary component formed from a single \e invalid
         * vertex is \e not considered to be ideal.  This means that, if a
         * boundary component contains no tetrahedra at all, then one and
         * only one of isIdeal() and isInvalidVertex() will return \c true.
         *
         * @return \c true if and only if this boundary component is ideal.
         */
        bool isIdeal() const;

        /**
         * Determines if this boundary component consists of a single invalid
         * vertex and nothing else.  In particular, such a boundary component
         * must contain no tetrahedra at all.
         *
         * An invalid vertex is only placed in its own boundary component if
         * it does not already belong to some larger boundary component
         * (for instance, if its link is an ideal 3-manifold triangulation).
         * This means that, for a boundary component consisting of one or
         * more tetrahedra, this routine will return \c false even if the
         * boundary tetrahedra include one or more invalid vertices.
         *
         * Note that, if a boundary component contains no tetrahedra at all,
         * then one and only one of isIdeal() and isInvalidVertex() will return
         * \c true.
         *
         * @return \c true if and only if this boundary component consists of a
         * single invalid vertex and nothing else.
         */
        bool isInvalidVertex() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Default constructor.
         */
        Dim4BoundaryComponent();

        /**
         * Creates a new boundary component consisting only of the given
         * vertex.
         *
         * \pre The given vertex is not part of a boundary tetrahedron,
         * but instead belongs in its own boundary component; an example
         * might be an ideal vertex.
         *
         * @param vertex the vertex to place in the new boundary component.
         */
        Dim4BoundaryComponent(Dim4Vertex* vertex);

    friend class Triangulation<4>;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4vertex.h"
namespace regina {

// Inline functions for Dim4BoundaryComponent

inline Dim4BoundaryComponent::Dim4BoundaryComponent() :
        boundary_(0) {
}

inline Dim4BoundaryComponent::Dim4BoundaryComponent(Dim4Vertex* vertex) :
        boundary_(0) {
    vertices_.push_back(vertex);
}

inline size_t Dim4BoundaryComponent::index() const {
    return markedIndex();
}

#ifndef __DOXYGEN // Doxygen gets confused by the specialisations.

template <>
inline size_t Dim4BoundaryComponent::countFaces<3>() const {
    return tetrahedra_.size();
}

template <>
inline size_t Dim4BoundaryComponent::countFaces<2>() const {
    return triangles_.size();
}

template <>
inline size_t Dim4BoundaryComponent::countFaces<1>() const {
    return edges_.size();
}

template <>
inline size_t Dim4BoundaryComponent::countFaces<0>() const {
    return vertices_.size();
}

template <>
inline Dim4Tetrahedron* Dim4BoundaryComponent::face<3>(size_t index) const {
    return tetrahedra_[index];
}

template <>
inline Dim4Triangle* Dim4BoundaryComponent::face<2>(size_t index) const {
    return triangles_[index];
}

template <>
inline Dim4Edge* Dim4BoundaryComponent::face<1>(size_t index) const {
    return edges_[index];
}

template <>
inline Dim4Vertex* Dim4BoundaryComponent::face<0>(size_t index) const {
    return vertices_[index];
}

#endif // __DOXYGEN

inline Dim4Component* Dim4BoundaryComponent::component() const {
    // There may be no tetrahedra, but there is always a vertex.
    return vertices_.front()->component();
}

inline const NTriangulation* Dim4BoundaryComponent::build() const {
    return (boundary_ ? boundary_ : vertices_.front()->buildLink());
}

inline void Dim4BoundaryComponent::writeTextShort(std::ostream& out) const {
    out << (isIdeal() ? "Ideal " : isInvalidVertex() ? "Invalid " : "Finite ")
        << "boundary component";
}

} // namespace regina

#endif

