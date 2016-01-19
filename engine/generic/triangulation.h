
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file generic/triangulation.h
 *  \brief Deals with triangulations of arbitrary dimension.
 */

#ifndef __TRIANGULATION_H
#ifndef __DOXYGEN
#define __TRIANGULATION_H
#endif

// There are more #includes below - we need to define FaceList
// before including generic/detail/triangulation.h.
#include "utilities/nmarkedvector.h"

namespace regina {

template <int, int> class Face;

/**
 * \addtogroup generic Generic code
 * Template code to work with triangulations of arbitrary dimension.
 * @{
 */

/**
 * Stores the list of all <i>subdim</i>-faces of a <i>dim</i>-dimensional
 * triangulation.
 *
 * This object provides basic container-like behaviour.  It supports begin(),
 * end(), size(), and the random access operator <tt>[]</tt>.
 * In particular, you can iterate through all <i>subdim</i>-faces using C++11
 * range-based \c for loops: <tt>for (auto f : faceList) { ... }</tt>.
 *
 * Strictly speaking, this list holds \e pointers to Face<dim, subdim> objects.
 * So, for example, dereferencing an iterator will return a pointer of type
 * <tt>Face<dim, subdim>*</tt>.  Likewise, the variable \a f in the range-based
 * \c for loop above will be a pointer of type <tt>Face<dim, subdim>*</tt>.
 *
 * \warning Face objects are highly temporary: whenever a triangulation
 * changes, all its face objects will be deleted and new ones will be
 * created in their place.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 * \tparam subdim the dimension of the faces that this class stores.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class FaceList {
    public:
        typedef typename std::vector<Face<dim, subdim>*>::const_iterator
                Iterator;
            /**< An iterator type for iterating through this list of faces.
                 Dereferencing such an iterator will return a pointer of type
                 <tt>Face<dim, subdim>*</tt>. */

    private:
        NMarkedVector<Face<dim, subdim>> faces_;
            /**< All <i>subdim</i>-faces of the underlying triangulation. */

    public:
        /**
         * Explicitly disable the copy constructor.
         */
        FaceList(const FaceList&) = delete;
        /**
         * Explicitly disable the assignment operator.
         */
        FaceList& operator = (const FaceList&) = delete;

        /**
         * Returns the number of <i>subdim</i>-faces in the triangulation.
         *
         * @return the number of <i>subdim</i>-faces.
         */
        size_t size() const;
        /**
         * Returns the requested <i>subdim</i>-face.
         *
         * @param index indicates which face to return; this must be
         * between 0 and size()-1 inclusive.
         * @return the (\a index)th <i>subdim</i>-face.
         */
        Face<dim, subdim>* operator [](size_t index) const;
        /**
         * Returns an iterator pointing to the first <i>subdim</i>-face.
         *
         * @return an iterator at the beginning of this list.
         */
        Iterator begin() const;
        /**
         * Returns an iterator pointing beyond the last <i>subdim</i>-face.
         *
         * @return an iterator beyond the end of this list.
         */
        Iterator end() const;

    protected:
        /**
         * Creates an empty list of <i>subdim</i>-faces.
         */
        FaceList() = default;
        /**
         * Pushes the given face onto the end of this list.
         * This object will take ownership of the given face.
         *
         * @param face the face to push.
         */
        void push_back(Face<dim, subdim>* face);
        /**
         * Destroys all faces in this list, and clears the list itself.
         */
        void destroy();
};

} // namespace regina

#include "generic/detail/triangulation.h"

namespace regina {

/**
 * A <i>dim</i>-dimensional triangulation, built by gluing together
 * <i>dim</i>-dimensional simplices along their (<i>dim</i>-1)-dimensional
 * facets.  Typically (but not necessarily) such triangulations are used
 * to represent <i>dim</i>-manifolds.
 *
 * Such triangulations are not the same as pure simplicial complexes, for two
 * reasons:
 *
 * - The only identifications that the user can explicitly specify are
 *   gluings between <i>dim</i>-dimensional simplices along their
 *   (<i>dim</i>-1)-dimensional facets.  All other identifications between
 *   <i>k</i>-faces (for any \a k) are simply consequences of these
 *   (<i>dim</i>-1)-dimensional gluings.  In contrast, a simplicial complex
 *   allows explicit gluings between faces of any dimension.
 *
 * - There is no requirement for a <i>k</i>-face to have (<i>k</i>+1) distinct
 *   vertices (so, for example, edges may be loops).  Many distinct
 *   <i>k</i>-faces of a top-dimensional simplex may be identified together
 *   as a consequence of the (<i>dim</i>-1)-dimensional gluings, and indeed
 *   we are even allowed to glue together two distinct factets of the same
 *   <i>dim</i>-simplex.  In contrast, a simplicial complex does not allow
 *   any of these situations.
 *
 * Amongst other things, this definition is general enough to capture
 * any reasonable definition of a <i>dim</i>-manifold triangulation.
 * However, there is no requirement that a triangulation must actually
 * represent a manifold (and indeed, testing this condition is undecidable
 * for sufficiently large \a dim).
 *
 * You can construct a triangulation from scratch using routines such as
 * newSimplex() and Simplex<dim>::join().  There are also routines for
 * importing and exporting triangulations in bulk, such as isoSig() and
 * fromIsoSig() (which uses <em>isomorphism signatures</em>), or
 * insertConstruction() and dumpConstruction() (which exports C++ code).
 *
 * In additional to top-dimensional simplices, this class also tracks
 * connected components of the triangulation, as represented by the
 * class Component<dim>.  Such objects are temporary: whenever the
 * triangulation changes, they will be deleted and rebuilt, and any
 * pointers to them will become invalid.  Likewise, if the triangulation
 * is deleted then all component objects will be deleted alongside it.
 *
 * For Regina's \ref stddim "standard dimensions", this template is specialised
 * and offers \e much more functionality.  In order to use these specialised
 * classes, you will need to include the corresponding headers (e.g.,
 * dim2/dim2triangulation.h for \a dim = 2, or triangulation/ntriangulation.h
 * for \a dim = 3).  For convenience, there are typedefs available for
 * these specialised classes (such as Dim2Triangulation and NTriangulation
 * respectively).
 *
 * \ifacespython Python does not support templates.  For standard dimensions
 * this class can be used by appending the dimension as a suffix
 * (e.g., Triangulation2 and Triangulation3 for dimensions 2 and 3).
 * The typedefs mentioned above (e.g., Dim2Triangulation and NTriangulation)
 * are also available.  Higher-dimensional classes are not available
 * in Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class Triangulation :
        public detail::TriangulationBase<dim>,
        public Output<Triangulation<dim>> {
    static_assert(! standardDim(dim),
        "The generic implementation of Triangulation<dim> "
        "should not be used for Regina's standard dimensions.");

    protected:
        using detail::TriangulationBase<dim>::simplices_;

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Default constructor.
         *
         * Creates an empty triangulation.
         */
        Triangulation();
        /**
         * Creates a new copy of the given triangulation.
         *
         * @param copy the triangulation to copy.
         */
        Triangulation(const Triangulation& copy);

        /*@}*/
        /**
         * \name Output
         */
        /*@{*/

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

        /*@}*/

    private:
        /**
         * Clears any calculated properties and declares them all unknown.
         * This must be called by any internal function that changes the
         * triangulation.
         */
        void clearAllProperties();

    public:
        /**
         * A do-nothing replacement for NPacket::ChangeEventSpan, for
         * use with triangulation classes that do not derive from NPacket.
         *
         * Any function that modifies a packet must, directly or indirectly,
         * create an NPacket::ChangeEventSpan object; the constructor
         * and destructor of this object will in turn fire required events
         * such as NPacketListener::packetToBeChanged() and
         * NPacketListener::packetWasChanged().
         *
         * However, NPacket::ChangeEventSpan can (of course) only work
         * with classes derived from NPacket.  This creates problems for
         * generic functions that modify \e triangulations, since
         * small-dimensional triangulations (such as Triangulation<3>)
         * derive from NPacket, whereas the generic Triangulation<dim> class
         * does not.
         *
         * For this reason, the generic Triangulation<dim> class
         * provides its own ChangeEventSpan type (i.e., this class),
         * which harmlessly does nothing at all.
         *
         * So: Any generic function that modifies a Triangulation<dim>,
         * regardless of the dimension \a dim, should create a
         * Triangulation<dim>::ChangeEventSpan object (instead of an
         * NPacket::ChangeEventSpan object).
         *
         * - If Triangulation<dim> does derive from NPacket, then this will
         *   equate to the inherited type NPacket::ChangeEventSpan,
         *   and the necessary packet events will be fired correctly.
         *
         * - If Triangulation<dim> does not derive from NPacket, then
         *   this will equate to the do-nothing type defined here.
         *   The code will harmlessly compile and do nothing (as intended).
         *
         * See NPacket::ChangeEventSpan for further detail on how
         * ChangeEventSpan objects should be used.
         *
         * \ifacespython Not present.
         */
        class ChangeEventSpan {
            public:
                /**
                 * Constructor that does nothing.
                 * The argument is ignored.
                 */
                inline ChangeEventSpan(detail::TriangulationBase<dim>*) {
                }
        };

    friend class detail::SimplexBase<dim>;
    friend class detail::TriangulationBase<dim>;
};

// Note that some of our triangulation classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Triangulation<2>;
template <> class Triangulation<3>;
template <> class Triangulation<4>;

/**
 * A function object used for sorting faces of triangulations by
 * increasing degree.  This can (for instance) be used with std::sort().
 *
 * The template argument \a dim refers to the dimension of the overall
 * triangluation(s) with which you are working.  The template argument
 * \a subdim refers to the dimension of the faces that you are sorting.
 * So, for instance, to sort edges of a 3-manifold triangulation by
 * increasing edge degree, you would use DegreeLessThan<3, 1>.
 *
 * A single instance of this class works with faces of a single
 * fixed triangulation (which is passed to the class constructor).
 *
 * \pre \a dim is one of Regina's \ref stddim "standard dimensions".
 * \pre \a subdim is between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class DegreeLessThan {
    static_assert(standardDim(dim),
        "DegreeLessThan is only available for Regina's standard dimensions.");

    private:
        const Triangulation<dim>& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * @param tri the triangulation with which we are working.
         */
        DegreeLessThan(const Triangulation<dim>& tri);
        /**
         * Compares the degrees of the <i>subdim</i>-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of <i>subdim</i>-dimensional faces in
         * the triangulation.
         *
         * @param a the index of the first <i>subdim</i>-dimensional face
         * within the triangulation.
         * @param b the index of the second <i>subdim</i>-dimensional face
         * within the triangulation.
         * @return \c true if and only if face \a a has smaller degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const;
};

/**
 * A function object used for sorting faces of triangulations by
 * decreasing degree.  This can (for instance) be used with std::sort().
 *
 * The template argument \a dim refers to the dimension of the overall
 * triangluation(s) with which you are working.  The template argument
 * \a subdim refers to the dimension of the faces that you are sorting.
 * So, for instance, to sort edges of a 3-manifold triangulation by
 * decreasing edge degree, you would use DegreeGreaterThan<3, 1>.
 *
 * A single instance of this class works with faces of a single
 * fixed triangulation (which is passed to the class constructor).
 *
 * \pre \a dim is one of Regina's \ref stddim "standard dimensions".
 * \pre \a subdim is between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class DegreeGreaterThan {
    static_assert(standardDim(dim),
        "DegreeGreaterThan is only available for Regina's standard dimensions.");

    private:
        const Triangulation<dim>& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * @param tri the triangulation with which we are working.
         */
        DegreeGreaterThan(const Triangulation<dim>& tri);
        /**
         * Compares the degrees of the <i>subdim</i>-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of <i>subdim</i>-dimensional faces in
         * the triangulation.
         *
         * @param a the index of the first <i>subdim</i>-dimensional face
         * within the triangulation.
         * @param b the index of the second <i>subdim</i>-dimensional face
         * within the triangulation.
         * @return \c true if and only if face \a a has greater degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const;
};

/*@}*/

// Inline functions for FaceList

template <int dim, int subdim>
inline size_t FaceList<dim, subdim>::size() const {
    return faces_.size();
}

template <int dim, int subdim>
inline Face<dim, subdim>* FaceList<dim, subdim>::operator [](size_t index)
        const {
    return faces_[index];
}

template <int dim, int subdim>
inline typename FaceList<dim, subdim>::Iterator FaceList<dim, subdim>::begin()
        const {
    return faces_.begin();
}

template <int dim, int subdim>
inline typename FaceList<dim, subdim>::Iterator FaceList<dim, subdim>::end()
        const {
    return faces_.end();
}

template <int dim, int subdim>
inline void FaceList<dim, subdim>::push_back(Face<dim, subdim>* face) {
    faces_.push_back(face);
}

template <int dim, int subdim>
inline void FaceList<dim, subdim>::destroy() {
    for (auto f : faces_)
        delete f;
    faces_.clear();
}

// Inline functions for Triangulation

template <int dim>
inline Triangulation<dim>::Triangulation() : detail::TriangulationBase<dim>() {
}

template <int dim>
inline Triangulation<dim>::Triangulation(const Triangulation& copy) :
        detail::TriangulationBase<dim>(copy) {
}

template <int dim>
inline void Triangulation<dim>::clearAllProperties() {
    // We have no override for deleteSkeleton().
    // Just call the base class implementation.
    if (detail::TriangulationBase<dim>::calculatedSkeleton())
        detail::TriangulationBase<dim>::deleteSkeleton();
}

template <int dim>
inline void Triangulation<dim>::writeTextShort(std::ostream& out) const {
    if (simplices_.size() == 0)
        out << "Empty " << dim << "-dimensional triangulation";
    else
        out << "Triangulation with " << simplices_.size() << ' ' << dim << '-'
            << (simplices_.size() == 1 ? "simplex" : "simplices");
}

template <int dim>
void Triangulation<dim>::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << "\n\n";

    Simplex<dim>* simp;
    Simplex<dim>* adj;
    size_t pos;
    int i, j;
    NPerm<dim+1> gluing;

    out << "  Simplex  |  glued to:";
    for (i = dim; i >= 0; --i) {
        out << "     (";
        for (j = 0; j <= dim; ++j)
            if (j != i)
                out << regina::digit(j);
        out << ')';
    }
    out << '\n';
    out << "  ---------+-----------";
    for (i = dim; i >= 0; --i)
        for (j = 0; j < 7 + dim; ++j)
            out << '-';
    out << '\n';
    for (pos=0; pos < simplices_.size(); pos++) {
        simp = simplices_[pos];
        out << "     " << std::setw(4) << pos << "  |           ";
        for (i = dim; i >= 0; --i) {
            adj = simp->adjacentSimplex(i);
            if (! adj) {
                for (j = 0; j < dim - 1; ++j)
                    out << ' ';
                out << "boundary";
            } else {
                gluing = simp->adjacentGluing(i);
                out << std::setw(4) << adj->index() << " (";
                for (j = 0; j <= dim; ++j) {
                    if (j != i)
                        out << regina::digit(gluing[j]);
                }
                out << ")";
            }
        }
        out << '\n';
    }
    out << '\n';
}

// Inline functions for DegreeLessThan / DegreeGreaterThan

template <int dim, int subdim>
inline DegreeLessThan<dim, subdim>::DegreeLessThan(
        const Triangulation<dim>& tri) : tri_(tri) {
}

template <int dim, int subdim>
inline bool DegreeLessThan<dim, subdim>::operator () (
        unsigned a, unsigned b) const {
    return (tri_.template face<subdim>(a)->degree() <
            tri_.template face<subdim>(b)->degree());
}

template <int dim, int subdim>
inline DegreeGreaterThan<dim, subdim>::DegreeGreaterThan(
        const Triangulation<dim>& tri) : tri_(tri) {
}

template <int dim, int subdim>
inline bool DegreeGreaterThan<dim, subdim>::operator () (
        unsigned a, unsigned b) const {
    return (tri_.template face<subdim>(a)->degree() >
            tri_.template face<subdim>(b)->degree());
}

} // namespace regina

#endif
