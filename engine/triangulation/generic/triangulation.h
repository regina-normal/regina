
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file triangulation/generic/triangulation.h
 *  \brief Internal header for working with triangulations of arbitrary
 *  dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#ifndef __REGINA_TRIANGULATION_H
#ifndef __DOXYGEN
#define __REGINA_TRIANGULATION_H
#endif

#include "regina-config.h" // for REGINA_HIGHDIM
#include "packet/packet.h"
#include "triangulation/forward.h"
#include "utilities/markedvector.h"
#include "utilities/xmlutils.h"
#include <type_traits>
#include "triangulation/detail/triangulation.h"

namespace regina {

/**
 * \defgroup generic Generic triangulations
 * Details for implementing triangulations in arbitrary dimensions.
 */

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
 *   we are even allowed to glue together two distinct facets of the same
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
 * exporting and importing triangulations in bulk, such as isoSig() and
 * fromIsoSig() (which use _isomorphism signatures_), or dumpConstruction()
 * and fromGluings() (which use C++ code).
 *
 * In additional to top-dimensional simplices, this class also tracks:
 *
 * - connected components of the triangulation, as represented by the
 *   class Component<dim>;
 * - boundary components of the triangulation, as represented by the
 *   class BoundaryComponent<dim>;
 * - lower-dimensional faces of the triangulation, as represented by the
 *   classes Face<dim, subdim> for \a subdim = 0,...,(<i>dim</i>-1).
 *
 * Such objects are temporary: whenever the triangulation changes, they will
 * be deleted and rebuilt, and any pointers to them will become invalid.
 * Likewise, if the triangulation is deleted then all component objects
 * will be deleted alongside it.
 *
 * Since Regina 7.0, this is no longer a "packet type" that can be
 * inserted directly into the packet tree.  Instead a Triangulation is now a
 * standalone mathematatical object, which makes it slimmer and faster
 * for ad-hoc use.  The consequences of this are:
 *
 * - If you create your own Triangulation, it will not have any of the usual
 *   packet infrastructure.  You cannot add it into the packet tree, and it will
 *   not support a label, tags, child/parent packets, and/or event listeners.
 *
 * - To include a Triangulation in the packet tree, you must create a new
 *   PacketOf<Triangulation>.  This _is_ a packet type, and supports labels,
 *   tags, child/parent packets, and event listeners.  It derives from
 *   Triangulation, and so inherits the full Triangulation interface.
 *
 * - If you are adding new functions to this class that edit the triangulation,
 *   you must still remember to create a ChangeEventSpan.  This will ensure
 *   that, if the triangulation is being managed by a PacketOf<Triangulation>,
 *   then the appropriate packet change events will be fired.  All other events
 *   (aside from packetToBeChanged() and packetWasChanged() are managed
 *   directly by the PacketOf<Triangulation> wrapper class.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * For Regina's \ref stddim "standard dimensions", this template is specialised
 * and offers _much_ more functionality.  In order to use these specialised
 * classes, you will need to include the corresponding headers (e.g.,
 * triangulation/dim2.h for \a dim = 2, or triangulation/dim3.h
 * for \a dim = 3).
 *
 * \python Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., Triangulation2 and Triangulation3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \headerfile triangulation/generic.h
 *
 * \ingroup generic
 */
template <int dim>
class Triangulation : public detail::TriangulationBase<dim> {
    static_assert(! standardDim(dim),
        "The generic implementation of Triangulation<dim> "
        "should not be used for Regina's standard dimensions.");
#ifndef REGINA_HIGHDIM
    static_assert(dim <= 8,
        "This build has been configured without REGINA_HIGHDIM. "
        "The Triangulation class should only be instantiated for "
        "dimensions dim <= 8.");
#endif

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
         * This will clone any computed properties (such as homology,
         * fundamental group, and so on) of the given triangulation also.
         * If you want a "clean" copy that resets all properties to unknown,
         * you can use the two-argument copy constructor instead.
         *
         * \param copy the triangulation to copy.
         */
        Triangulation(const Triangulation& copy);
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties also.
         *
         * \param copy the triangulation to copy.
         * \param cloneProps \c true if this should also clone any computed
         * properties of the given triangulation (such as homology,
         * fundamental group, and so on), or \c false if the new triangulation
         * should have all properties marked as unknown.
         */
        Triangulation(const Triangulation& copy, bool cloneProps);
        /**
         * Moves the given triangulation into this new triangulation.
         *
         * This is much faster than the copy constructor, but is still linear
         * time.  This is because every top-dimensional simplex must be
         * adjusted to point back to this new triangulation instead of \a src.
         *
         * All top-dimensional simplices and skeletal objects (faces,
         * components and boundary components) that belong to \a src will be
         * moved into this triangulation, and so any pointers or references to
         * Simplex<dim>, Face<dim, subdim>, Component<dim> or
         * BoundaryComponent<dim> objects will remain valid.  Likewise, all
         * cached properties will be moved into this triangulation.
         *
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is marked \c noexcept, and in particular
         * does not fire any change events.  This is because this triangulation
         * is freshly constructed (and therefore has no listeners yet), and
         * because we assume that \a src is about to be destroyed (an action
         * that _will_ fire a packet destruction event).
         *
         * \param src the triangulation to move.
         */
        Triangulation(Triangulation&& src) noexcept = default;
        /**
         * Destroys this triangulation.
         *
         * The constituent simplices, the cellular structure and all other
         * properties will also be destroyed.
         */
        ~Triangulation();

        /*@}*/
        /**
         * \name Simplices
         */
        /*@{*/

        /**
         * Sets this to be a (deep) copy of the given triangulation.
         *
         * \return a reference to this triangulation.
         */
        Triangulation& operator = (const Triangulation&) = default;

        /**
         * Moves the contents of the given triangulation into this
         * triangulation.
         *
         * This is much faster than copy assignment, but is still linear
         * time.  This is because every top-dimensional simplex must be
         * adjusted to point back to this triangulation instead of \a src.
         *
         * All top-dimensional simplices and skeletal objects (faces,
         * components and boundary components) that belong to \a src will be
         * moved into this triangulation, and so any pointers or references to
         * Simplex<dim>, Face<dim, subdim>, Component<dim> or
         * BoundaryComponent<dim> objects will remain valid.  Likewise, all
         * cached properties will be moved into this triangulation.
         *
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is _not_ marked \c noexcept, since it fires
         * change events on this triangulation which may in turn call arbitrary
         * code via any registered packet listeners.  It deliberately does
         * _not_ fire change events on \a src, since it assumes that \a src is
         * about to be destroyed (which will fire a destruction event instead).
         *
         * \param src the triangulation to move.
         * \return a reference to this triangulation.
         */
        Triangulation& operator = (Triangulation&& src) = default;

        /**
         * Swaps the contents of this and the given triangulation.
         *
         * All top-dimensional simplices that belong to this triangulation
         * will be moved to \a other, and all top-dimensional simplices
         * that belong to \a other will be moved to this triangulation.
         * Likewise, all skeletal objects (such as lower-dimensional faces,
         * components, and boundary components) and all cached properties
         * will be swapped.
         *
         * In particular, any pointers or references to Simplex<dim> and/or
         * Face<dim, subdim> objects will remain valid.
         *
         * This routine will behave correctly if \a other is in fact
         * this triangulation.
         *
         * \note This swap function is _not_ marked \c noexcept, since it
         * fires change events on both triangulations which may in turn call
         * arbitrary code via any registered packet listeners.
         *
         * \param other the triangulation whose contents should be
         * swapped with this.
         */
        void swap(Triangulation<dim>& other);

        /*@}*/

    private:
        /**
         * Clears any calculated properties, including skeletal data,
         * and declares them all unknown.  This must be called by any
         * internal function that changes the triangulation.
         *
         * In most cases this routine is followed immediately by firing
         * a change event.
         */
        void clearAllProperties();

    friend class detail::SimplexBase<dim>;
    friend class detail::TriangulationBase<dim>;
};

/**
 * Deprecated function object used for sorting faces of triangulations by
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
 * An object of this class behaves like a reference: it is lightweight and can
 * be copy-constructed cheaply, but it does not support assignments or swaps.
 *
 * \deprecated This comparison is a one-liner.  Just use a lambda instead.
 *
 * \pre \a dim is one of Regina's \ref stddim "standard dimensions".
 * \pre \a subdim is between 0 and <i>dim</i>-1 inclusive.
 *
 * \nopython
 *
 * \ingroup generic
 */
template <int dim, int subdim>
class [[deprecated]] DegreeLessThan {
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
         * \param tri the triangulation with which we are working.
         */
        DegreeLessThan(const Triangulation<dim>& tri) : tri_(tri) {}
        /**
         * Creates a new clone of the given function object.
         */
        DegreeLessThan(const DegreeLessThan&) = default;
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
         * \param a the index of the first <i>subdim</i>-dimensional face
         * within the triangulation.
         * \param b the index of the second <i>subdim</i>-dimensional face
         * within the triangulation.
         * \return \c true if and only if face \a a has smaller degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const {
            return (tri_.template face<subdim>(a)->degree() <
                    tri_.template face<subdim>(b)->degree());
        }

        // Make this class non-assignable, since \a tri_ is a reference.
        DegreeLessThan& operator = (const DegreeLessThan&) = delete;
};

/**
 * Deprecated function object used for sorting faces of triangulations by
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
 * An object of this class behaves like a reference: it is lightweight and can
 * be copy-constructed cheaply, but it does not support assignments or swaps.
 *
 * \deprecated This comparison is a one-liner.  Just use a lambda instead.
 *
 * \nopython
 *
 * \pre \a dim is one of Regina's \ref stddim "standard dimensions".
 * \pre \a subdim is between 0 and <i>dim</i>-1 inclusive.
 *
 * \ingroup generic
 */
template <int dim, int subdim>
class [[deprecated]] DegreeGreaterThan {
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
         * \param tri the triangulation with which we are working.
         */
        DegreeGreaterThan(const Triangulation<dim>& tri) : tri_(tri) {}
        /**
         * Creates a new clone of the given function object.
         */
        DegreeGreaterThan(const DegreeGreaterThan&) = default;
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
         * \param a the index of the first <i>subdim</i>-dimensional face
         * within the triangulation.
         * \param b the index of the second <i>subdim</i>-dimensional face
         * within the triangulation.
         * \return \c true if and only if face \a a has greater degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const {
            return (tri_.template face<subdim>(a)->degree() >
                    tri_.template face<subdim>(b)->degree());
        }

        // Make this class non-assignable, since \a tri_ is a reference.
        DegreeGreaterThan& operator = (const DegreeGreaterThan&) = delete;
};

// Inline functions for Triangulation

template <int dim>
inline Triangulation<dim>::Triangulation() : detail::TriangulationBase<dim>() {
}

template <int dim>
inline Triangulation<dim>::Triangulation(const Triangulation& copy) :
        detail::TriangulationBase<dim>(copy) {
    // All properties to clone are held by TriangulationBase.
}

template <int dim>
inline Triangulation<dim>::Triangulation(const Triangulation& copy,
        bool cloneProps) : detail::TriangulationBase<dim>(copy, cloneProps) {
    // All properties to clone are held by TriangulationBase.
}

template <int dim>
inline Triangulation<dim>::~Triangulation() {
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    clearAllProperties();
}

template <int dim>
inline void Triangulation<dim>::clearAllProperties() {
    detail::TriangulationBase<dim>::clearBaseProperties();
}

template <int dim>
void Triangulation<dim>::swap(Triangulation<dim>& other) {
    if (&other == this)
        return;

    typename Triangulation<dim>::ChangeEventSpan span1(*this);
    typename Triangulation<dim>::ChangeEventSpan span2(other);

    // Note: swapBaseData() calls Snapshottable::swap().
    this->swapBaseData(other);
}

} // namespace regina

#endif
