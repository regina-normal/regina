
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

/*! \file triangulation/dim2/boundarycomponent2.h
 *  \brief Internal header for boundary components of a 2-manifold
 *  triangulation.
 *
 *  This file is automatically included from triangulation/dim2.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation2.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation2.h.
#include "triangulation/dim2/triangulation2.h"

#ifndef __BOUNDARYCOMPONENT2_H
#ifndef __DOXYGEN
#define __BOUNDARYCOMPONENT2_H
#endif

namespace regina {

template <int> class Component;
template <int> class Triangulation;
template <int, int> class Face;
typedef Component<2> Dim2Component;
typedef Triangulation<2> Dim2Triangulation;
typedef Face<2, 0> Dim2Vertex;
typedef Face<2, 1> Dim2Edge;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a component of the boundary of a 2-manifold triangulation.
 *
 * Boundary components are highly temporary; once a triangulation
 * changes, all its boundary component objects will be deleted and new
 * ones will be created.
 */
class REGINA_API Dim2BoundaryComponent :
        public Output<Dim2BoundaryComponent>,
        public alias::FaceOfTriangulation<Dim2BoundaryComponent, 2>,
        public boost::noncopyable,
        public MarkedElement {
    private:
        std::vector<Dim2Edge*> edges_;
            /**< List of edges in the component. */
        std::vector<Dim2Vertex*> vertices_;
            /**< List of vertices in the component. */

    public:
        /**
         * Returns the index of this boundary component in the underlying
         * triangulation.
         *
         * @return the index of this boundary component.
         */
        size_t index() const;

        /**
         * Returns the number of <i>subdim</i>-faces in this boundary component.
         *
         * \pre The template argument \a subdim is either 0 or 1.
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
         *
         * \pre The template argument \a subdim is either 0 or 1.
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
        Face<2, subdim>* face(size_t index) const;

        /**
         * Returns the component of the triangulation to which this
         * boundary component belongs.
         *
         * @return the component containing this boundary component.
         */
        Dim2Component* component() const;

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
        Dim2BoundaryComponent();

    friend class Triangulation<2>;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for Dim2BoundaryComponent

inline Dim2BoundaryComponent::Dim2BoundaryComponent() {
}

inline size_t Dim2BoundaryComponent::index() const {
    return markedIndex();
}

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline size_t Dim2BoundaryComponent::countFaces<1>() const {
    return edges_.size();
}

template <>
inline size_t Dim2BoundaryComponent::countFaces<0>() const {
    return vertices_.size();
}

template <>
inline Dim2Edge* Dim2BoundaryComponent::face<1>(size_t index) const {
    return edges_[index];
}

template <>
inline Dim2Vertex* Dim2BoundaryComponent::face<0>(size_t index) const {
    return vertices_[index];
}
#endif // ! __DOXYGEN

inline Dim2Component* Dim2BoundaryComponent::component() const {
    return vertices_.front()->component();
}

inline void Dim2BoundaryComponent::writeTextShort(std::ostream& out) const {
    out << "Boundary component";
}

} // namespace regina

#endif

