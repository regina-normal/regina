
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file triangulation/detail/triangulation.h
 *  \brief Implementation details for triangulations of arbitrary dimension.
 */

#ifndef __REGINA_TRIANGULATION_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_TRIANGULATION_H_DETAIL
#endif

#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include "regina-core.h"
#include "core/output.h"
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "maths/matrix.h"
#include "packet/packet.h"
#include "triangulation/generic/component.h"
#include "triangulation/generic/boundarycomponent.h"
#include "triangulation/generic/face.h"
#include "triangulation/generic/isomorphism.h"
#include "triangulation/generic/simplex.h"
#include "triangulation/isosigencoding.h"
#include "utilities/exception.h"
#include "utilities/listview.h"
#include "utilities/snapshot.h"
#include "utilities/tightencoding.h"

namespace regina {

template <int dim> class FacetPairing;
template <int dim> class IsoSigClassic;
template <int dim> class XMLTriangulationReader;

/**
 * Contains implementation details and common functionality for Regina's
 * dimension-agnostic classes.
 *
 * For most of Regina's dimension-agnostic classes, such as
 * Triangulation<dim>, Simplex<dim> and Face<dim, subdim>, the bulk of
 * the implementation is hidden away in the namespace regina::detail.
 *
 * Regina's main classes acquire their functionality through inheritance.
 * For example, the end-user class regina::Triangulation<dim> inherits
 * most of its functionality from the implementation class
 * regina::detail::TriangulationBase<dim>.
 *
 * Because of this inheritance, there is typically no need for
 * end users to explicitly refer to the namespace regina::detail.
 *
 * Since regina::detail contains implementation details, its
 * classes are subject to change between releases.  Specifically:
 *
 * - All member functions that are inherited and exposed by the end-user
 *   classes in regina (e.g., Triangulation, Simplex, Face and so on) may be
 *   considered part of Regina's official API, and will be supported from
 *   release to release.
 *
 * - In constrast, any methods that are not exposed by the end-user classes
 *   (including the names and inheritance structure of classes within
 *   regina::detail) might change in subsequent releases without notice.
 */
namespace detail {

/**
 * \defgroup detail Implementation details
 * Implementation details that end users should not need to reference directly.
 */

/**
 * Provides core functionality for <i>dim</i>-dimensional triangulations.
 *
 * Such a triangulation is represented by the class Triangulation<dim>,
 * which uses this as a base class.  End users should not need to refer
 * to TriangulationBase directly.
 *
 * See the Triangulation class notes for further information.
 *
 * Note that this class does not derive from Output.  This is to avoid clashes
 * with the output code inherited from Packet.  Specifically:
 *
 * - for those dimensions where Triangulation<dim> derives from Packet, the
 *   output routines are inherited from Packet (which derives from Output).
 *
 * - for other dimensions, Triangulation<dim> derives from Output directly.
 *
 * \python This base class is not present, but the "end user" class
 * Triangulation<dim> is.
 *
 * \tparam dim the dimension of the triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup detail
 */
template <int dim>
class TriangulationBase :
        public Snapshottable<Triangulation<dim>>,
        public PacketData<Triangulation<dim>>,
        public Output<Triangulation<dim>>,
        public TightEncodable<Triangulation<dim>> {
    static_assert(dim >= 2, "Triangulation requires dimension >= 2.");

    public:
        using typename PacketData<Triangulation<dim>>::ChangeEventSpan;

        static constexpr int dimension = dim;
            /**< A compile-time constant that gives the dimension of the
                 triangulation. */

    protected:
        MarkedVector<Simplex<dim>> simplices_;
            /**< The top-dimensional simplices that form the triangulation. */

    private:
        /**
         * The sequence of all subface dimensions 0,...,(<i>dim</i>-1).
         */
        using subdimensions = std::make_integer_sequence<int, dim>;

        /**
         * A non-existent function used to construct the type of the \a faces_
         * tuple.  Essentially, this lets us pull apart the integer pack
         * \a subdimensions.  The return type is the tuple type that we want.
         */
        template <int... subdim>
        static auto seqToFaces(std::integer_sequence<int, subdim...>) ->
            std::tuple<MarkedVector<Face<dim, subdim>>...>;

        decltype(seqToFaces(subdimensions())) faces_;
            /**< A tuple of vectors holding all faces of this triangulation.
                 Specifically, std::get<k>(faces_)[i] is a pointer to the
                 ith k-face of the triangulation. */

        /**
         * A compile-time constant function that returns the facial dimension
         * corresponding to an element of the \a faces_ tuple.
         *
         * This is to assist code that calls std::apply() on \a faces_,
         * since functions in TriangulationBase have easy access to the
         * tuple type but not the corresponding integer parameter pack
         * of face dimensions.
         *
         * If \a f is an element of \a faces_, possibly with reference
         * qualifiers, then the corresponding face dimension is:
         *
         * \code{.cpp}
         * subdimOf<decltype(f)>()
         * \endcode
         *
         * \tparam TupleElement the type of one of the members of \a faces,
         * or a reference to such a type.
         * \return the face dimension corresponding to \a TupleElement;
         * this will be an integer between 0 and (<i>dim</i>-1) inclusive.
         */
        template <typename TupleElement>
        static constexpr int subdimOf() {
            return std::remove_pointer_t<
                    typename std::remove_reference_t<TupleElement>::value_type
                >::subdimension;
        }

    protected:
        MarkedVector<Component<dim>> components_;
            /**< The connected components that form the triangulation.
                 This list is only filled if/when the skeleton of the
                 triangulation is computed. */

        MarkedVector<BoundaryComponent<dim>> boundaryComponents_;
            /**< The components that form the boundary of the triangulation. */

        std::array<size_t, dim> nBoundaryFaces_;
            /**< The number of boundary faces of each dimension. */

        bool valid_;
            /**< Is this triangulation valid?  See isValid() for details
                 on what this means. */

        uint8_t topologyLock_;
            /**< If non-zero, this will cause
                 Triangulation<dim>::clearAllProperties() to preserve any
                 computed properties that related to the manifold (as
                 opposed to the specific triangulation).  This allows
                 you to avoid recomputing expensive invariants when the
                 underlying manifold is retriangulated.

                 This property should be managed by creating and
                 destroying TopologyLock objects.  The precise value of
                 topologyLock_ indicates the number of TopologyLock
                 objects that currently exist for this triangulation. */

    private:
        bool calculatedSkeleton_;
            /**< Has the skeleton been calculated?  This is only done
                 "on demand", when a skeletal property is first queried. */
        bool orientable_;
            /**< Is the triangulation orientable?  This property is only set
                 if/when the skeleton of the triangulation is computed. */
        mutable std::optional<GroupPresentation> fundGroup_;
            /**< Fundamental group of the triangulation.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<AbelianGroup> H1_;
            /**< First homology group of the triangulation.
                 This is std::nullopt if it has not yet been computed. */

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
        TriangulationBase();
        /**
         * Creates a new copy of the given triangulation.
         *
         * This will also clone any computed properties (such as homology,
         * fundamental group, and so on), as well as the skeleton (vertices,
         * edges, components, etc.).  In particular, the same numbering and
         * labelling will be used for all skeletal objects.
         *
         * If you want a "clean" copy that resets all properties to unknown
         * and leaves the skeleton uncomputed, you can use the two-argument
         * copy constructor instead.
         *
         * \param src the triangulation to copy.
         */
        TriangulationBase(const TriangulationBase<dim>& src);
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties also.
         *
         * If \a cloneProps is \c true, then this constructor will also clone
         * any computed properties (such as homology, fundamental group, and
         * so on), as well as the skeleton (vertices, edges, components, etc.).
         * In particular, the same numbering and labelling will be used for
         * all skeletal objects in both triangulations.
         *
         * If \a cloneProps is \c false, then these properties and skeletal
         * objects will be marked as unknown in the new triangulation, and
         * will be recomputed on demand if/when they are required.  Note
         * in particular that, when the skeleton is recomputed, there is
         * no guarantee that the numbering and labelling for skeletal objects
         * will be the same as in the source triangulation.
         *
         * \param src the triangulation to copy.
         * \param cloneProps \c true if this should also clone any computed
         * properties as well as the skeleton of the given triangulation,
         * or \c false if the new triangulation should have such properties
         * and skeletal data marked as unknown.
         */
        TriangulationBase(const TriangulationBase<dim>& src, bool cloneProps);
        /**
         * Moves the given triangulation into this new triangulation.
         *
         * This is much faster than the copy constructor, but is still linear
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
         * \note This operator is marked \c noexcept, and in particular
         * does not fire any change events.  This is because this triangulation
         * is freshly constructed (and therefore has no listeners yet), and
         * because we assume that \a src is about to be destroyed (an action
         * that _will_ fire a packet destruction event).
         *
         * \param src the triangulation to move.
         */
        TriangulationBase(TriangulationBase&& src) noexcept;
        /**
         * Destroys this triangulation.
         *
         * The simplices within this triangulation will also be destroyed.
         */
        ~TriangulationBase();

        /*@}*/
        /**
         * \name Simplices
         */
        /*@{*/

        /**
         * Returns the number of top-dimensional simplices in the
         * triangulation.
         *
         * \return The number of top-dimensional simplices.
         */
        size_t size() const;
        /**
         * Returns an object that allows iteration through and random access
         * to all top-dimensional simplices in this triangulation.
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
         * for (Simplex<dim>* s : tri.simplices()) { ... }
         * \endcode
         *
         * The object that is returned will remain up-to-date and valid for
         * as long as the triangulation exists: even as simplices are
         * added and/or removed, it will always reflect the simplices
         * that are currently in the triangulation.
         * Nevertheless, it is recommended to treat this object as temporary
         * only, and to call simplices() again each time you need it.
         *
         * \return access to the list of all top-dimensional simplices.
         */
        auto simplices() const;
        /**
         * Returns the top-dimensional simplex at the given index in the
         * triangulation.
         *
         * Note that indexing may change when a simplex is added to or
         * removed from the triangulation.
         *
         * \param index specifies which simplex to return; this
         * value should be between 0 and size()-1 inclusive.
         * \return the <i>index</i>th top-dimensional simplex.
         */
        Simplex<dim>* simplex(size_t index);
        /**
         * Returns the top-dimensional simplex at the given index in the
         * triangulation.
         *
         * Note that indexing may change when a simplex is added to or
         * removed from the triangulation.
         *
         * \param index specifies which simplex to return; this
         * value should be between 0 and size()-1 inclusive.
         * \return the <i>index</i>th top-dimensional simplex.
         */
        const Simplex<dim>* simplex(size_t index) const;
        /**
         * Creates a new top-dimensional simplex and adds it to this
         * triangulation.
         *
         * The new simplex will have an empty description.
         * All (<i>dim</i>+1) facets of the new simplex will be boundary facets.
         *
         * The new simplex will become the last simplex in this
         * triangulation; that is, it will have index size()-1.
         *
         * \return the new simplex.
         */
        Simplex<dim>* newSimplex();
        /**
         * Creates \a k new top-dimensional simplices, adds them to this
         * triangulation, and returns them in a std::array.
         *
         * The main purpose of this routine is to support structured binding;
         * for example:
         *
         * \code{.cpp}
         * auto [r, s, t] = ans.newSimplices<3>();
         * r->join(0, s, {1, 2, 3, 0});
         * ...
         * \endcode
         *
         * All new simplices will have empty descriptions, and all facets
         * of each new simplex will be boundary facets.
         *
         * The new simplices will become the last \a k simplices in this
         * triangulation.  Specifically, if the return value is the array
         * \a ret, then each simplex `ret[i]` will have index
         * `size()-k+i` in the overall triangulation.
         *
         * \python For Python users, the two variants of newSimplices()
         * are essentially merged: the argument \a k is passed as an ordinary
         * runtime argument, and the new top-dimensional simplices will
         * be returned in a Python tuple of size \a k.
         *
         * \tparam k the number of new top-dimensional simplices to add;
         * this must be non-negative.
         *
         * \return an array containing all of the new simplices, in the order
         * in which they were added.
         */
        template <int k>
        std::array<Simplex<dim>*, k> newSimplices();
        /**
         * Creates \a k new top-dimensional simplices and adds them to this
         * triangulation.
         *
         * This is similar to the templated routine `newSimplices<k>()`,
         * but with two key differences:
         *
         * - This routine has the disadvantage that it does not return the new
         *   top-dimensional simplices, which means you cannot use it with
         *   structured binding.
         *
         * - This routine has the advantage that \a k does not need to be known
         *   until runtime, which means this routine is accessible to Python
         *   users.
         *
         * All new simplices will have empty descriptions, and all facets
         * of each new simplex will be boundary facets.
         *
         * The new simplices will become the last \a k simplices in this
         * triangulation.
         *
         * \python For Python users, the two variants of newSimplices()
         * are essentially merged: the argument \a k is passed as an ordinary
         * runtime argument, and the new top-dimensional simplices will
         * be returned in a Python tuple of size \a k.
         *
         * \param k the number of new top-dimensional simplices to add;
         * this must be non-negative.
         */
        void newSimplices(size_t k);
        /**
         * Creates a new top-dimensional simplex with the given
         * description and adds it to this triangulation.
         *
         * All (<i>dim</i>+1) facets of the new simplex will be boundary facets.
         *
         * Descriptions are optional, may have any format, and may be empty.
         * How descriptions are used is entirely up to the user.
         *
         * The new simplex will become the last simplex in this
         * triangulation; that is, it will have index size()-1.
         *
         * \param desc the description to give to the new simplex.
         * \return the new simplex.
         */
        Simplex<dim>* newSimplex(const std::string& desc);
        /**
         * Removes the given top-dimensional simplex from this triangulation.
         *
         * The given simplex will be unglued from any adjacent simplices
         * (if any), and will be destroyed immediately.
         *
         * \pre The given simplex is a top-dimensional simplex in this
         * triangulation.
         *
         * \param simplex the simplex to remove.
         */
        void removeSimplex(Simplex<dim>* simplex);
        /**
         * Removes the top-dimensional simplex at the given index in
         * this triangulation.
         *
         * This is equivalent to calling `removeSimplex(simplex(index))`.
         *
         * The given simplex will be unglued from any adjacent simplices
         * (if any), and will be destroyed immediately.
         *
         * \param index specifies which top-dimensionalsimplex to remove; this
         * must be between 0 and size()-1 inclusive.
         */
        void removeSimplexAt(size_t index);
        /**
         * Removes all simplices from the triangulation.
         * As a result, this triangulation will become empty.
         *
         * All of the simplices that belong to this triangulation will
         * be destroyed immediately.
         */
        void removeAllSimplices();
        /**
         * Moves the contents of this triangulation into the given destination
         * triangulation, without destroying any pre-existing contents.
         *
         * All top-dimensional simplices that currently belong to \a dest
         * will remain there (and will keep the same indices in \a dest).
         * All top-dimensional simplices that belong to this triangulation
         * will be moved into \a dest also (but in general their indices will
         * change).
         *
         * This triangulation will become empty as a result.
         *
         * Any pointers or references to Simplex<dim> objects will remain valid.
         *
         * If your intention is to _replace_ the simplices in \a dest
         * (i.e., you do not need to preserve the original contents),
         * then consider using the move assignment operator instead
         * (which is more streamlined and also moves across any cached
         * properties from the source triangulation).
         *
         * \pre \a dest is not this triangulation.
         *
         * \param dest the triangulation into which simplices should be moved.
         */
        void moveContentsTo(Triangulation<dim>& dest);

        /*@}*/
        /**
         * \name Skeletal Queries
         */
        /*@{*/

        /**
         * Returns the number of connected components in this triangulation.
         *
         * \return the number of connected components.
         */
        size_t countComponents() const;

        /**
         * Returns the number of boundary components in this triangulation.
         *
         * Note that, in Regina's \ref stddim "standard dimensions",
         * each ideal vertex forms its own boundary component, and
         * some invalid vertices do also.  See the BoundaryComponent
         * class notes for full details on what constitutes a boundary
         * component in standard and non-standard dimensions.
         *
         * \return the number of boundary components.
         */
        size_t countBoundaryComponents() const;

        /**
         * Returns the number of <i>subdim</i>-faces in this triangulation.
         *
         * This is the fastest way to count faces if you know \a subdim
         * at compile time.
         *
         * For convenience, this routine explicitly supports the case
         * \a subdim = \a dim.  This is _not_ the case for the routines
         * face() and faces(), which give access to individual faces (the
         * reason relates to the fact that top-dimensional simplices are built
         * manually, whereas lower-dimensional faces are deduced properties).
         *
         * \nopython Instead use the variant `countFaces(subdim)`.
         *
         * \tparam subdim the face dimension; this must be between 0 and
         * \a dim inclusive.
         *
         * \return the number of <i>subdim</i>-faces.
         */
        template <int subdim>
        size_t countFaces() const;

        /**
         * Returns the number of <i>subdim</i>-faces in this triangulation,
         * where the face dimension does not need to be known until runtime.
         *
         * This routine takes linear time in the dimension \a dim.  For C++
         * programmers who know \a subdim at compile time, you are better off
         * using the template function countFaces<subdim>() instead, which
         * is fast constant time.
         *
         * For convenience, this routine explicitly supports the case
         * \a subdim = \a dim.  This is _not_ the case for the routines
         * face() and faces(), which give access to individual faces (the
         * reason relates to the fact that top-dimensional simplices are built
         * manually, whereas lower-dimensional faces are deduced properties).
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative or greater than \a dim).
         *
         * \param subdim the face dimension; this must be between 0 and \a dim
         * inclusive.
         * \return the number of <i>subdim</i>-faces.
         */
        size_t countFaces(int subdim) const;

        /**
         * A dimension-specific alias for countFaces<0>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See countFaces() for further information.
         */
        size_t countVertices() const;

        /**
         * A dimension-specific alias for countFaces<1>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See countFaces() for further information.
         */
        size_t countEdges() const;

        /**
         * A dimension-specific alias for countFaces<2>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See countFaces() for further information.
         */
        size_t countTriangles() const;

        /**
         * A dimension-specific alias for countFaces<3>().
         *
         * This alias is available for dimensions \a dim ≥ 3.
         *
         * See countFaces() for further information.
         */
        size_t countTetrahedra() const;

        /**
         * A dimension-specific alias for countFaces<4>().
         *
         * This alias is available for dimensions \a dim ≥ 4.
         *
         * See countFaces() for further information.
         */
        size_t countPentachora() const;

        /**
         * Returns the f-vector of this triangulation, which counts the
         * number of faces of all dimensions.
         *
         * The vector that is returned will have length <i>dim</i>+1.
         * If this vector is \a f, then \a f[\a k] will be the number of
         * <i>k</i>-faces for each 0 ≤ \a k ≤ \a dim.
         *
         * This routine is significantly more heavyweight than countFaces().
         * Its advantage is that, unlike the templatised countFaces(),
         * it allows you to count faces whose dimensions are not known
         * until runtime.
         *
         * \return the f-vector of this triangulation.
         */
        std::vector<size_t> fVector() const;

        /**
         * Returns an object that allows iteration through and random access
         * to all components of this triangulation.
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
         * for (Component<dim>* c : tri.components()) { ... }
         * \endcode
         *
         * The object that is returned will remain up-to-date and valid for
         * as long as the triangulation exists.  In contrast, however, remember
         * that the individual component objects _within_ this list will be
         * deleted and replaced each time the triangulation changes.
         * Therefore it is best to treat this object as temporary only,
         * and to call components() again each time you need it.
         *
         * \return access to the list of all components.
         */
        auto components() const;

        /**
         * Returns an object that allows iteration through and random access
         * to all boundary components of this triangulation.
         *
         * Note that, in Regina's \ref stddim "standard dimensions",
         * each ideal vertex forms its own boundary component, and
         * some invalid vertices do also.  See the BoundaryComponent
         * class notes for full details on what constitutes a boundary
         * component in standard and non-standard dimensions.
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
         * for (BoundaryComponent<dim>* b : tri.boundaryComponents()) { ... }
         * \endcode
         *
         * The object that is returned will remain up-to-date and valid for
         * as long as the triangulation exists.  In contrast, however, remember
         * that the individual boundary components _within_ this list will be
         * deleted and replaced each time the triangulation changes.
         * Therefore it is best to treat this object as temporary only,
         * and to call boundaryComponents() again each time you need it.
         *
         * \return access to the list of all boundary components.
         */
        auto boundaryComponents() const;

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces of this triangulation, in a way
         * that is optimised for C++ programmers.
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
         * for (Face<dim, subdim>* f : tri.faces<subdim>()) { ... }
         * \endcode
         *
         * The object that is returned will remain up-to-date and valid for
         * as long as the triangulation exists.  In contrast, however,
         * remember that the individual faces _within_ this list will be
         * deleted and replaced each time the triangulation changes.
         * Therefore it is best to treat this object as temporary only,
         * and to call faces() again each time you need it.
         *
         * \nopython Instead use the variant `faces(subdim)`.
         *
         * \tparam subdim the face dimension; this must be between 0 and
         * <i>dim</i>-1 inclusive.
         *
         * \return access to the list of all <i>subdim</i>-faces.
         */
        template <int subdim>
        auto faces() const;

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces of this triangulation, in a way
         * that is optimised for Python programmers.
         *
         * C++ users should not use this routine.  The return type must be
         * fixed at compile time, and so it is a std::variant that can hold
         * any of the lightweight return types from the templated
         * faces<subdim>() function.  This means that the return value will
         * still need compile-time knowledge of \a subdim to extract and
         * use the appropriate face objects.  However, once you know \a subdim
         * at compile time, you are much better off using the (simpler and
         * faster) routine faces<subdim>() instead.
         *
         * For Python users, this routine is much more useful: the return type
         * can be chosen at runtime, and so this routine returns a Python list
         * of Face<dim, subdim> objects (holding all the <i>subdim</i>-faces
         * of the triangulation), which you can use immediately.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative, or greater than or equal to
         * \a dim).
         *
         * \param subdim the face dimension; this must be between 0 and
         * <i>dim</i>-1 inclusive.
         * \return access to the list of all <i>subdim</i>-faces.
         */
        auto faces(int subdim) const;

        /**
         * A dimension-specific alias for faces<0>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See faces() for further information.
         */
        auto vertices() const;

        /**
         * A dimension-specific alias for faces<1>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See faces() for further information.
         */
        auto edges() const;

        /**
         * A dimension-specific alias for faces<2>(), or an alias for
         * simplices() in dimension \a dim = 2.
         *
         * This alias is available for all dimensions.
         *
         * See faces() for further information.
         */
        auto triangles() const;

        /**
         * A dimension-specific alias for faces<3>(), or an alias for
         * simplices() in dimension \a dim = 3.
         *
         * This alias is available for dimensions \a dim ≥ 3.
         *
         * See faces() for further information.
         */
        auto tetrahedra() const;

        /**
         * A dimension-specific alias for faces<4>(), or an alias for
         * simplices() in dimension \a dim = 4.
         *
         * This alias is available for dimensions \a dim ≥ 4.
         *
         * See faces() for further information.
         */
        auto pentachora() const;

        /**
         * Returns the requested connected component of this triangulation.
         *
         * Note that each time the triangulation changes, all component
         * objects will be deleted and replaced with new ones.
         * Therefore this component object should be considered temporary only.
         *
         * \param index the index of the desired component; this must be
         * between 0 and countComponents()-1 inclusive.
         * \return the requested component.
         */
        Component<dim>* component(size_t index) const;

        /**
         * Returns the requested boundary component of this triangulation.
         *
         * Note that each time the triangulation changes, all
         * boundary components will be deleted and replaced with new
         * ones.  Therefore this object should be considered temporary only.
         *
         * \param index the index of the desired boundary component; this must
         * be between 0 and countBoundaryComponents()-1 inclusive.
         * \return the requested boundary component.
         */
        BoundaryComponent<dim>* boundaryComponent(size_t index) const;

        /**
         * Returns the requested <i>subdim</i>-face of this triangulation,
         * in a way that is optimised for C++ programmers.
         *
         * \nopython Instead use the variant `face(subdim, index)`.
         *
         * \tparam subdim the face dimension; this must be between 0 and
         * <i>dim</i>-1 inclusive.
         *
         * \param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * \return the requested face.
         */
        template <int subdim>
        Face<dim, subdim>* face(size_t index) const;

        /**
         * Returns the requested <i>subdim</i>-face of this triangulation,
         * in a way that is optimised for Python programmers.
         *
         * For C++ users, this routine is not very useful: since precise types
         * must be know at compile time, this routine returns a std::variant
         * \a v that could store a pointer to any class Face<dim, ...>.
         * This means you cannot access the face directly: you will still need
         * some kind of compile-time knowledge of \a subdim before you can
         * extract and use an appropriate Face<dim, subdim> object from \a v.
         * However, once you know \a subdim at compile time, you are better off
         * using the (simpler and faster) routine face<subdim>() instead.
         *
         * For Python users, this routine is much more useful: the return type
         * can be chosen at runtime, and so this routine simply returns a
         * Face<dim, subdim> object of the appropriate face dimension that
         * you can use immediately.
         *
         * The specific return type for C++ programmers will be
         * std::variant<Face<dim, 0>*, ..., Face<dim, dim-1>*>.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative, or greater than or equal to
         * \a dim).
         *
         * \param subdim the face dimension; this must be between 0 and
         * <i>dim</i>-1 inclusive.
         * \param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * \return the requested face.
         */
        auto face(int subdim, size_t index) const;

        /**
         * A dimension-specific alias for face<0>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See face() for further information.
         */
        Face<dim, 0>* vertex(size_t index) const;

        /**
         * A dimension-specific alias for face<1>().
         *
         * This alias is available for all dimensions \a dim.
         *
         * See face() for further information.
         */
        Face<dim, 1>* edge(size_t index) const;

        /**
         * A dimension-specific alias for face<2>(), or an alias for
         * simplex() in dimension \a dim = 2.
         *
         * This alias is available for all dimensions \a dim.
         *
         * See face() for further information.
         */
        Face<dim, 2>* triangle(size_t index);

        /**
         * A dimension-specific alias for face<2>(), or an alias for
         * simplex() in dimension \a dim = 2.
         *
         * This alias is available for all dimensions \a dim.
         * It returns a const triangle pointer in dimension \a dim = 2,
         * and a non-const triangle pointer in all higher dimensions.
         *
         * See face() for further information.
         */
        auto triangle(size_t index) const;

        /**
         * A dimension-specific alias for face<3>(), or an alias for
         * simplex() in dimension \a dim = 3.
         *
         * This alias is available for dimensions \a dim ≥ 3.
         *
         * See face() for further information.
         */
        Face<dim, 3>* tetrahedron(size_t index);

        /**
         * A dimension-specific alias for face<3>(), or an alias for
         * simplex() in dimension \a dim = 3.
         *
         * This alias is available for dimensions \a dim ≥ 3.
         * It returns a const tetrahedron pointer in dimension \a dim = 3,
         * and a non-const tetrahedron pointer in all higher dimensions.
         *
         * See face() for further information.
         */
        auto tetrahedron(size_t index) const;

        /**
         * A dimension-specific alias for face<4>(), or an alias for
         * simplex() in dimension \a dim = 4.
         *
         * This alias is available for dimensions \a dim ≥ 4.
         *
         * See face() for further information.
         */
        Face<dim, 4>* pentachoron(size_t index);

        /**
         * A dimension-specific alias for face<4>(), or an alias for
         * simplex() in dimension \a dim = 4.
         *
         * This alias is available for dimensions \a dim ≥ 4.
         * It returns a const pentachoron pointer in dimension \a dim = 4,
         * and a non-const pentachoron pointer in all higher dimensions.
         *
         * See face() for further information.
         */
        auto pentachoron(size_t index) const;

        /**
         * Translates a face of some other triangulation into the corresponding
         * face of this triangulation, using simplex numbers for the
         * translation.
         *
         * Typically this routine would be used when the given face comes
         * from a triangulation that is combinatorially identical to this,
         * and you wish to obtain the corresponding face of this triangulation.
         *
         * Specifically: if \a other refers to face \a i of top-dimensional
         * simplex number \a k of some other triangulation, then this routine
         * will return face \a i of top-dimensional simplex number \a k of this
         * triangulation.  Note that this routine does _not_ use the face
         * indices within each triangulation (which is outside the user's
         * control), but rather the simplex numbering (which the user has
         * full control over).
         *
         * This routine behaves correctly even if \a other is a null pointer.
         *
         * \pre This triangulation contains at least as many top-dimensional
         * simplices as the triangulation containing \a other (though, as noted
         * above, in typical scenarios both triangulations would actually be
         * combinatorially identical).
         *
         * \tparam subdim the face dimension; this must be between 0 and
         * <i>dim</i>-1 inclusive.
         *
         * \param other the face to translate.
         * \return the corresponding face of this triangulation.
         */
        template <int subdim>
        Face<dim, subdim>* translate(const Face<dim, subdim>* other) const;

        /**
         * Returns the dual graph of this triangulation, expressed as a
         * facet pairing.
         *
         * Calling `tri.pairing()` is equivalent to calling
         * `FacetPairing<dim>(tri)`.
         *
         * \pre This triangulation is not empty.
         *
         * \return the dual graph of this triangulation.
         */
        FacetPairing<dim> pairing() const;

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Determines whether this triangulation is empty.
         * An empty triangulation is one with no simplices at all.
         *
         * \return \c true if and only if this triangulation is empty.
         */
        bool isEmpty() const;

        /**
         * Determines if this triangulation is valid.
         *
         * There are several conditions that might make a
         * <i>dim</i>-dimensional triangulation invalid:
         *
         * -# if some face is identified with itself under a non-identity
         *    permutation (e.g., an edge is identified with itself in
         *    reverse, or a triangle is identified with itself under a
         *    rotation);
         * -# if some <i>subdim</i>-face does not have an appropriate link.
         *    Here the meaning of "appropriate" depends upon the type of face:
         *    - for a face that belongs to some boundary facet(s) of this
         *      triangulation, its link must be a topological ball;
         *    - for a vertex that does not belong to any boundary facets,
         *      its link must be a closed (\a dim - 1)-manifold;
         *    - for a (\a subdim ≥ 1)-face that does not belong to any
         *      boundary facets, its link must be a topological sphere.
         *
         * Condition (1) is tested for all dimensions \a dim.
         * Condition (2) is more difficult, since it relies on undecidable
         * problems.  As a result, (2) is _only_ tested when \a dim is one
         * of Regina's \ref stddim "standard dimensions".
         *
         * If a triangulation is invalid then you can call
         * Face<dim, subdim>::isValid() to discover exactly which face(s)
         * are responsible, and you can call
         * Face<dim, subdim>::hasBadIdentification() and/or
         * Face<dim, subdim>::hasBadLink() to discover exactly which
         * conditions fail.
         *
         * Note that all invalid vertices are considered to be on the
         * boundary; see isBoundary() for details.
         *
         * \return \c true if and only if this triangulation is valid.
         */
        bool isValid() const;

        /**
         * Determines if this triangulation has any boundary facets.
         *
         * This routine returns \c true if and only if the
         * triangulation contains some top-dimension simplex with at
         * least one facet that is not glued to an adjacent simplex.
         *
         * \return \c true if and only if there are boundary facets.
         */
        bool hasBoundaryFacets() const;

        /**
         * Returns the total number of boundary facets in this triangulation.
         *
         * This routine counts facets of top-dimensional simplices that are
         * not glued to some adjacent top-dimensional simplex.
         *
         * This is equivalent to calling countBoundaryFaces<dim-1>().
         *
         * \return the total number of boundary facets.
         */
        size_t countBoundaryFacets() const;

        /**
         * Returns the number of boundary <i>subdim</i>-faces in this
         * triangulation.
         *
         * This is the fastest way to count faces if you know \a subdim
         * at compile time.
         *
         * Specifically, this counts the number of <i>subdim</i>-faces
         * for which isBoundary() returns \c true.  This may lead to some
         * unexpected results in non-standard scenarios; see the documentation
         * for the non-templated countBoundaryFaces(int) for details.
         *
         * \nopython Instead use the variant `countBoundaryFaces(subdim)`.
         *
         * \tparam subdim the face dimension; this must be between 0 and
         * <i>dim</i>-1 inclusive.
         *
         * \return the number of boundary <i>subdim</i>-faces.
         */
        template <int subdim>
        size_t countBoundaryFaces() const;

        /**
         * Returns the number of boundary <i>subdim</i>-faces in this
         * triangulation, where the face dimension does not need to be known
         * until runtime.
         *
         * This routine takes linear time in the dimension \a dim.  For C++
         * programmers who know \a subdim at compile time, you are better off
         * using the template function countBoundaryFaces<subdim>() instead,
         * which is fast constant time.
         *
         * Specifically, this counts the number of <i>subdim</i>-faces
         * for which isBoundary() returns \c true.  This may lead to some
         * unexpected results in non-standard scenarios; for example:
         *
         * - In \ref stddim "non-standard dimensions", ideal vertices are not
         *   recognised and so will not be counted as boundary;
         *
         * - In an invalid triangulation, the number of boundary faces reported
         *   here may be smaller than the number of faces obtained when you
         *   triangulate the boundary using BoundaryComponent::build().
         *   This is because "pinched" faces (where separate parts of the
         *   boundary are identified together) will only be counted once here,
         *   but will "spring apart" into multiple faces when the boundary is
         *   triangulated.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative or greater than <i>dim</i>-1).
         *
         * \param subdim the face dimension; this must be between 0 and
         * <i>dim</i>-1 inclusive.
         * \return the number of boundary <i>subdim</i>-faces.
         */
        size_t countBoundaryFaces(int subdim) const;

        /**
         * Determines if this triangulation is orientable.
         *
         * \return \c true if and only if this triangulation is orientable.
         */
        bool isOrientable() const;

        /**
         * Determines if this triangulation is connected.
         *
         * This routine returns \c false only if there is more than one
         * connected component.  In particular, it returns \c true for
         * the empty triangulation.
         *
         * \return \c true if and only if this triangulation is connected.
         */
        bool isConnected() const;

        /**
         * Determines if this triangulation is oriented; that is, if the
         * vertices of its top-dimensional simplices are labelled in a way
         * that preserves orientation across adjacent facets.
         * Specifically, this routine returns \c true if and only if every
         * gluing permutation has negative sign.
         *
         * Note that _orientable_ triangulations are not always _oriented_
         * by default.  You can call orient() if you need the top-dimensional
         * simplices to be oriented consistently as described above.
         *
         * A non-orientable triangulation can never be oriented.
         *
         * \return \c true if and only if all top-dimensional simplices are
         * oriented consistently.
         *
         * \author Matthias Goerner
         */
        bool isOriented() const;

        /**
         * Returns the Euler characteristic of this triangulation.
         * This will be evaluated strictly as the alternating sum
         * of the number of <i>i</i>-faces (that is,
         * `countVertices() - countEdges() + countTriangles() - ...`).
         *
         * Note that this routine handles ideal triangulations in a
         * non-standard way.  Since it computes the Euler characteristic of
         * the triangulation (and not the underlying manifold), this routine
         * will treat each ideal boundary component as a single vertex, and
         * _not_ as an entire (<i>dim</i>-1)-dimensional boundary component.
         *
         * In Regina's \ref stddim "standard dimensions", for a routine that
         * handles ideal boundary components properly (by treating them as
         * (<i>dim</i>-1)-dimensional boundary components when computing Euler
         * characteristic), you can use the routine eulerCharManifold() instead.
         *
         * \return the Euler characteristic of this triangulation.
         */
        long eulerCharTri() const;

        /*@}*/
        /**
         * \name Algebraic Properties
         */
        /*@{*/

        /**
         * Returns the fundamental group of this triangulation.
         *
         * The fundamental group is computed in the dual 2-skeleton.  This
         * means:
         *
         * - If the triangulation contains any ideal vertices, the fundamental
         *   group will be calculated as if each such vertex had been truncated.
         *
         * - Likewise, if the triangulation contains any invalid faces
         *   of dimension 0,1,...,(<i>dim</i>-3), these will effectively
         *   be truncated also.
         *
         * - In contrast, if the triangulation contains any invalid
         *   (<i>dim</i>-2)-faces (i.e., codimension-2-faces that are
         *   identified with themselves under a non-trivial map), the
         *   fundamental group will be computed _without_ truncating the
         *   centroid of the face.  For instance, if a 3-manifold
         *   triangulation has an edge identified with itself in reverse,
         *   then the fundamental group will be computed without truncating
         *   the resulting projective plane cusp.  This means that, if a
         *   barycentric subdivision is performed on a such a triangulation,
         *   the result of group() might change.
         *
         * Bear in mind that each time the triangulation changes, the
         * fundamental group will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, group() should be called again; this will be instantaneous
         * if the group has already been calculated.
         *
         * Before Regina 7.2, this routine was called fundamentalGroup().
         * It has since been renamed to group() for brevity and for consistency
         * with Link::group().  The more expressive name fundamentalGroup()
         * will be kept, and you are welcome to use that instead if you prefer.
         *
         * \pre This triangulation has at most one component.
         *
         * \warning In dimension 3, if you are calling this from the subclass
         * SnapPeaTriangulation then **any fillings on the cusps will be
         * ignored**.  (This is the same as for every routine implemented by
         * Regina's Triangulation<3> class.)  If you wish to compute the
         * fundamental group with fillings, call
         * SnapPeaTriangulation::fundamentalGroupFilled() instead.
         *
         * \return the fundamental group.
         */
        const GroupPresentation& group() const;
        /**
         * An alias for group(), which returns the fundamental group of this
         * triangulation.
         *
         * See group() for further details, including how ideal vertices and
         * invalid faces are managed.
         *
         * \note In Regina 7.2, the routine fundamentalGroup() was renamed to
         * group() for brevity and for consistency with Link::group().  This
         * more expressive name fundamentalGroup() will be kept as a long-term
         * alias, and you are welcome to continue using it if you prefer.
         *
         * \pre This triangulation has at most one component.
         *
         * \warning In dimension 3, if you are calling this from the subclass
         * SnapPeaTriangulation then **any fillings on the cusps will be
         * ignored**.  (This is the same as for every routine implemented by
         * Regina's Triangulation<3> class.)  If you wish to compute the
         * fundamental group with fillings, call
         * SnapPeaTriangulation::fundamentalGroupFilled() instead.
         *
         * \return the fundamental group.
         */
        const GroupPresentation& fundamentalGroup() const;
        /**
         * Notifies the triangulation that you have simplified the presentation
         * of its fundamental group.  The old group presentation will be
         * replaced by the (hopefully simpler) group that is passed.
         *
         * This routine is useful for situations in which some external
         * body (such as GAP) has simplified the group presentation
         * better than Regina can.
         *
         * Regina does _not_ verify that the new group presentation is
         * equivalent to the old, since this is - well, hard.
         *
         * If the fundamental group has not yet been calculated for this
         * triangulation, then this routine will store the new group as the
         * fundamental group, under the assumption that you have worked out
         * the group through some other clever means without ever having
         * needed to call group() at all.
         *
         * Note that this routine will not fire a packet change event.
         *
         * \param newGroup a new (and hopefully simpler) presentation of
         * the fundamental group of this triangulation.
         */
        void simplifiedFundamentalGroup(GroupPresentation newGroup);

        /**
         * Returns the <i>k</i>th homology group of this triangulation,
         * treating any ideal vertices as though they had been truncated.
         *
         * For C++ programmers who know \a subdim at compile time, you should
         * use this template function `homology<subdim>()`, which is
         * slightly faster than passing \a subdim as an ordinary runtime
         * argument to `homology(subdim)`.
         *
         * See the non-templated homology(int) for full details on exactly what
         * this function computes.
         *
         * \pre Unless you are computing first homology (\a k = 1), this
         * triangulation must be valid, and every face that is not a vertex
         * must have a ball or sphere link.  The link condition already
         * forms part of the validity test if \a dim is one of Regina's
         * \ref stddim "standard dimensions", but in higher dimensions it is
         * the user's own responsibility to ensure this.  See isValid() for
         * details.
         *
         * \exception FailedPrecondition This triangulation is invalid, and
         * the homology dimension \a k is not 1.
         *
         * \nopython Instead use the variant `homology(k)`.
         *
         * \tparam k the dimension of the homology group to return;
         * this must be between 1 and (\a dim - 1) inclusive if \a dim is
         * one of Regina's \ref stddim "standard dimensions", or between
         * 1 and (\a dim - 2) inclusive if not.
         *
         * \return the <i>k</i>th homology group.
         */
        template <int k = 1>
        AbelianGroup homology() const;

        /**
         * Returns the <i>k</i>th homology group of this triangulation,
         * treating any ideal vertices as though they had been truncated,
         * where the parameter \a k does not need to be known until runtime.
         *
         * For C++ programmers who know \a k at compile time, you are better
         * off using the template function homology<k>() instead, which
         * is slightly faster.
         *
         * A problem with computing homology is that, if \a dim is not one of
         * Regina's \ref stddim "standard dimensions", then Regina cannot
         * actually _detect_ ideal vertices (since in general this requires
         * solving undecidable problems).  Currently we resolve this by
         * insisting that, in higher dimensions, the homology dimension \a k
         * is at most (<i>dim</i>-2); the underlying algorithm will then
         * effectively truncate _all_ vertices (since truncating "ordinary"
         * vertices whose links are spheres or balls does not affect the
         * <i>k</i>th homology in such cases).
         *
         * In general, this routine insists on working with a valid
         * triangulation (see isValid() for what this means).
         * However, for historical reasons, if you are computing first
         * homology (\a k = 1) then your triangulation is allowed to be
         * invalid, though the results might or might not be useful to you.
         * The homology will be computed using the dual skeleton: what this
         * means is that any invalid faces of dimension 0,1,...,(<i>dim</i>-3)
         * will be treated as though their centroids had been truncated,
         * but any invalid (<i>dim</i>-2)-faces will be treated _without_
         * such truncation.  A side-effect is that, after performing a
         * barycentric on an invalid triangulation, the group returned by
         * homology<1>() might change.
         *
         * \warning In dimension 3, if you are calling this from the subclass
         * SnapPeaTriangulation then **any fillings on the cusps will be
         * ignored**.  (This is the same as for every routine implemented by
         * Regina's Triangulation<3> class.)  If you wish to compute homology
         * with fillings, call SnapPeaTriangulation::homologyFilled() instead.
         *
         * \pre Unless you are computing first homology (\a k = 1), this
         * triangulation must be valid, and every face that is not a vertex
         * must have a ball or sphere link.  The link condition already
         * forms part of the validity test if \a dim is one of Regina's
         * \ref stddim "standard dimensions", but in higher dimensions it is
         * the user's own responsibility to ensure this.  See isValid() for
         * details.
         *
         * \exception FailedPrecondition This triangulation is invalid, and
         * the homology dimension \a k is not 1.
         *
         * \exception InvalidArgument The homology dimension \a k is outside
         * the supported range.  This range depends upon the triangulation
         * dimension \a dim; for details see the documentation below for the
         * argument \a k.
         *
         * \python Like the C++ template function homology<k>(),
         * you can omit the homology dimension \a k; this will default to 1.
         *
         * \param k the dimension of the homology group to return;
         * this must be between 1 and (\a dim - 1) inclusive if \a dim is
         * one of Regina's \ref stddim "standard dimensions", or between
         * 1 and (\a dim - 2) inclusive if not.
         * \return the <i>k</i>th homology group.
         */
        AbelianGroup homology(int k) const;

        /**
         * Returns the <i>k</i>th homology group of this triangulation,
         * without truncating ideal vertices, but with explicit coordinates
         * that track the individual <i>k</i>-faces of this triangulation.
         *
         * For C++ programmers who know \a subdim at compile time, you should
         * use this template function `markedHomology<subdim>()`, which is
         * slightly faster than passing \a subdim as an ordinary runtime
         * argument to `markedHomology(subdim)`.
         *
         * See the non-templated markedHomology(int) for full details on what
         * this function computes, some important caveats to be aware of,
         * and how the group that it returns should be interpreted.
         *
         * \pre This triangulation is valid and non-empty.
         *
         * \exception FailedPrecondition This triangulation is empty or invalid.
         *
         * \nopython Instead use the variant `markedHomology(k)`.
         *
         * \tparam k the dimension of the homology group to compute; this must
         * be between 1 and (<i>dim</i>-1) inclusive.
         *
         * \return the <i>k</i>th homology group of the union of all
         * simplices in this triangulation, as described above.
         */
        template <int k = 1>
        MarkedAbelianGroup markedHomology() const;

        /**
         * Returns the <i>k</i>th homology group of this triangulation,
         * without truncating ideal vertices, but with explicit coordinates
         * that track the individual <i>k</i>-faces of this triangulation,
         * where the parameter \a k does not need to be known until runtime.
         *
         * For C++ programmers who know \a k at compile time, you are better
         * off using the template function markedHomology<k>() instead, which
         * is slightly faster.
         *
         * This is a specialised homology routine; you should only use
         * it if you need to understand how individual <i>k</i>-faces
         * (or chains of <i>k</i>-faces) appear within the homology group.
         *
         * - The major disadvantage of this routine is that it does not
         *   truncate ideal vertices.  Instead it computes the homology
         *   of the union of all top-dimensional simplices, working directly
         *   with the boundary maps between (<i>k</i>+1)-faces, <i>k</i>-faces
         *   and (<i>k</i>-1)-faces of the triangulation.  If your
         *   triangulation is ideal, then this routine will almost certainly
         *   _not_ give the correct homology group for the underlying manifold.
         *   If, however, all of your vertex links are spheres or balls
         *   (i.e., the triangulation is closed or all of its boundary
         *   components are built from unglued (<i>dim</i>-1)-faces),
         *   then the homology of the manifold will be computed correctly.
         *
         * - The major advantage is that, instead of returning a simpler
         *   AbelianGroup, this routine returns a MarkedAbelianGroup.
         *   This allows you to track chains of individual <i>k</i>-faces of
         *   the triangulation as they appear within the homology group.
         *   Specifically, the chain complex cordinates with this
         *   MarkedAbelianGroup represent precisely the <i>k</i>-faces of the
         *   triangulation in the same order as they appear in the list
         *   faces<k>(), using the inherent orientation provided by
         *   Face<dim, k>.
         *
         * \pre This triangulation is valid and non-empty.
         *
         * \exception FailedPrecondition This triangulation is empty or invalid.
         *
         * \exception InvalidArgument The homology dimension \a k is outside
         * the supported range (i.e., less than 1 or greater than or
         * equal to \a dim).
         *
         * \python Like the C++ template function markedHomology<k>(),
         * you can omit the homology dimension \a k; this will default to 1.
         *
         * \param k the dimension of the homology group to compute; this must
         * be between 1 and (<i>dim</i>-1) inclusive.
         * \return the <i>k</i>th homology group of the union of all
         * simplices in this triangulation, as described above.
         */
        MarkedAbelianGroup markedHomology(int k) const;

        /**
         * Returns the boundary map from <i>subdim</i>-faces to
         * (<i>subdim</i>-1)-faces of the triangulation.
         *
         * For C++ programmers who know \a subdim at compile time, you should
         * use this template function `boundaryMap<subdim>()`, which is
         * slightly faster than passing \a subdim as an ordinary runtime
         * argument to `boundaryMap(subdim)`.
         *
         * See the non-templated boundaryMap(int) for full details on
         * what this function computes and how the matrix it returns
         * should be interpreted.
         *
         * \pre This triangulation is valid and non-empty.
         *
         * \nopython Instead use the variant `boundaryMap(subdim)`.
         *
         * \tparam subdim the face dimension; this must be between 1 and
         * \a dim inclusive.
         *
         * \return the boundary map from <i>subdim</i>-faces to
         * (<i>subdim</i>-1)-faces.
         */
        template <int subdim>
        MatrixInt boundaryMap() const;

        /**
         * Returns the boundary map from <i>subdim</i>-faces to
         * (<i>subdim</i>-1)-faces of the triangulation, where the
         * face dimension does not need to be known until runtime.
         *
         * For C++ programmers who know \a subdim at compile time, you are
         * better off using the template function boundaryMap<subdim>()
         * instead, which is slightly faster.
         *
         * This is the boundary map that you would use if you were building
         * the homology groups manually from a chain complex.
         *
         * Unlike homology(), this code does _not_ use the dual skeleton:
         * instead it uses the primal (i.e., ordinary) skeleton.
         *
         * - The main advantage of this is that you can easily match rows and
         *   columns of the returned matrix to faces of this triangulation.
         *
         * - The main disadvantage is that ideal vertices are _not_ treated
         *   as though they were truncated; instead they are just treated
         *   as 0-faces that appear as part of the chain complex.
         *
         * The matrix that is returned should be thought of as acting on
         * column vectors.  Specifically, the <i>c</i>th column of the matrix
         * corresponds to the <i>c</i>th <i>subdim</i>-face of this
         * triangulation, and the <i>r</i>th row corresponds to the <i>r</i>th
         * (<i>subdim</i>-1)-face of this triangulation.
         *
         * For the boundary map, we fix orientations as follows.
         * In simplicial homology, for any \a k, the orientation of a
         * <i>k</i>-simplex is determined by assigning labels 0,...,\a k
         * to its vertices.  For this routine, since every <i>k</i>-face \a f
         * is already a <i>k</i>-simplex, these labels will just be the
         * inherent vertex labels 0,...,\a k of the corresponding Face<k>
         * object.  If you need to convert these labels into vertex numbers of
         * a top-dimensional simplex containing \a f, you can use either
         * Simplex<dim>::faceMapping<k>(), or the equivalent routine
         * FaceEmbedding<k>::vertices().
         *
         * If you wish to convert these boundary maps to homology groups
         * yourself, either the AbelianGroup class (if you do not need
         * to track which face is which) or the MarkedAbelianGroup class
         * (if you do need to track individual faces) can help you do this.
         *
         * Note that, unlike many of the templated face-related routines,
         * this routine explicitly supports the case \a subdim = \a dim.
         *
         * \pre This triangulation is valid and non-empty.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., less than 1 or greater than \a dim).
         *
         * \param subdim the face dimension; this must be between 1 and \a dim
         * inclusive.
         * \return the boundary map from <i>subdim</i>-faces to
         * (<i>subdim</i>-1)-faces.
         */
        MatrixInt boundaryMap(int subdim) const;

        /**
         * Returns the boundary map from dual <i>subdim</i>-faces to
         * dual (<i>subdim</i>-1)-faces of the triangulation.
         *
         * For C++ programmers who know \a subdim at compile time, you should
         * use this template function `dualBoundaryMap<subdim>()`, which is
         * slightly faster than passing \a subdim as an ordinary runtime
         * argument to `dualBoundaryMap(subdim)`.
         *
         * See the non-templated dualBoundaryMap(int) for full details on
         * what this function computes and how the matrix it returns
         * should be interpreted.
         *
         * \pre This triangulation is valid and non-empty.
         *
         * \nopython Instead use the variant `dualBoundaryMap(subdim)`.
         *
         * \tparam subdim the dual face dimension; this must be between
         * 1 and \a dim inclusive if \a dim is one of Regina's standard
         * dimensions, or between 1 and (\a dim - 1) inclusive otherwise.
         *
         * \return the boundary map from dual <i>subdim</i>-faces to
         * dual (<i>subdim</i>-1)-faces.
         */
        template <int subdim>
        MatrixInt dualBoundaryMap() const;

        /**
         * Returns the boundary map from dual <i>subdim</i>-faces to
         * dual (<i>subdim</i>-1)-faces of the triangulation, where the
         * face dimension does not need to be known until runtime.
         *
         * For C++ programmers who know \a subdim at compile time, you are
         * better off using the template function dualBoundaryMap<subdim>()
         * instead, which is slightly faster.
         *
         * This function is analogous to boundaryMap(), but is designed to
         * work with dual faces instead of ordinary (primal) faces.
         * In particular, this is used in the implementation of homology(),
         * which works with the dual skeleton in order to effectively
         * truncate ideal vertices.
         *
         * The matrix that is returned should be thought of as acting on
         * column vectors.  Specifically, the <i>c</i>th column of the matrix
         * corresponds to the <i>c</i>th dual <i>subdim</i>-face of this
         * triangulation, and the <i>r</i>th row corresponds to the <i>r</i>th
         * dual (<i>subdim</i>-1)-face of this triangulation.  Here we index
         * dual faces in the same order as the (primal) faces of the
         * triangulation that they are dual to, except that we omit primal
         * _boundary_ faces (i.e., primal faces for which Face::isBoundary()
         * returns \c true).  Therefore, for triangulations with boundary,
         * the dual face indices and the corresponding primal face indices
         * might not be equal.
         *
         * For this dual boundary map, for positive dual face dimensions \a k,
         * we fix the orientations of the dual <i>k</i>-faces as follows:
         *
         * - In simplicial homology, the orientation of a <i>k</i>-simplex is
         *   determined by assigning labels 0,...,\a k to its vertices.
         *
         * - Consider a dual <i>k</i>-face \a d, and let this be dual to the
         *   primal (<i>dim</i>-<i>k</i>)-face \a f.  In general, \a d will
         *   _not_ be a simplex.  Let \a B denote the barycentre of \a f
         *   (which also appears as the "centre" point of \a d).
         *
         * - Let \a emb be an arbitrary FaceEmbedding<dim-k> for \a f (i.e.,
         *   chosen from `f.embeddings()`), and let \a s be the
         *   corresponding top-dimensional simplex containing \a f (i.e.,
         *   `emb.simplex()`).  For the special case of dual edges
         *   (\a k = 1), this choice matters; here we choose \a emb to be the
         *   first embedding (that is, `f.front()`).  For larger \a k
         *   this choice does not matter; see below for the reasons why.
         *
         * - Now consider how \a d intersects the top-dimensional simplex \a s.
         *   This intersection is a <i>k</i>-polytope with \a B as one of its
         *   vertices.  We can extend this polytope away from \a B, pushing it
         *   all the way through the simplex \a s, until it becomes a
         *   <i>k</i>-simplex \a g whose vertices are \a B along with the
         *   \a k "unused" vertices of \a s that do _not_ appear in \a f.
         *
         * - We can now define the orientation of the dual <i>k</i>-face \a d
         *   to be the orientation of this <i>k</i>-simplex \a g that contains
         *   it.  All that remains now is to orient \a g by choosing a
         *   labelling 0,...,\a k for its vertices.
         *
         * - To orient \a g, we assign the label 0 to \a B, and we
         *   assign the labels 1,...,\a k to the "unused" vertices
         *   `v[dim-k+1]`,...,`v[dim]` of \a s respectively,
         *   where \a v is the permutation `emb.vertices()`.
         *
         * - Finally, we note that for \a k > 1, the orientation for \a d
         *   does not depend on the particular choice of \a s and \a emb: by
         *   the preconditions and the fact that this routine only considers
         *   duals of non-boundary faces, the link of \a f must be a sphere,
         *   and therefore the images of those "other" vertices are fixed in a
         *   way that preserves orientation as you walk around the link.  See
         *   the documentation for Simplex<dim>::faceMapping() for details.
         *
         * - For the special case of dual edges (\a k = 1), the conditions
         *   above can be described more simply: the two endpoints of the dual
         *   edge \a d correspond to the two top-dimensional simplices on
         *   either side of the (<i>dim</i>-1)-face \a f, and we orient \a d
         *   by labelling these endpoints (0, 1) in the order
         *   (`f.back()`, `f.front()`).
         *
         * If you wish to convert these boundary maps to homology groups
         * yourself, either the AbelianGroup class (if you do not need
         * to track which dual face is which) or the MarkedAbelianGroup class
         * (if you do need to track individual dual faces) can help you do this.
         *
         * \pre This triangulation is valid and non-empty.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (as documented for the \a subdim argument below).
         *
         * \param subdim the dual face dimension; this must be between
         * 1 and \a dim inclusive if \a dim is one of Regina's standard
         * dimensions, or between 1 and (\a dim - 1) inclusive otherwise.
         * \return the boundary map from dual <i>subdim</i>-faces to
         * dual (<i>subdim</i>-1)-faces.
         */
        MatrixInt dualBoundaryMap(int subdim) const;

        /**
         * Returns a map from dual chains to primal chains that preserves
         * homology classes.
         *
         * For C++ programmers who know \a subdim at compile time, you should
         * use this template function `dualToPrimal<subdim>()`, which is
         * slightly faster than passing \a subdim as an ordinary runtime
         * argument to `dualToPrimal(subdim)`.
         *
         * See the non-templated dualToPrimal(int) for full details on
         * what this function computes and how the matrix it returns
         * should be interpreted.
         *
         * \pre This trianguation is valid, non-empty, and non-ideal.
         * Note that Regina can only detect ideal triangulations in
         * \ref stddim "standard dimensions"; for higher dimensions it is
         * the user's reponsibility to confirm this some other way.
         *
         * \nopython Instead use the variant `dualToPrimal(subdim)`.
         *
         * \tparam subdim the chain dimension; this must be between
         * 0 and (\a dim - 1) inclusive.
         *
         * \return the map from dual <i>subdim</i>-chains to primal
         * <i>subdim</i>-chains.
         */
        template <int subdim>
        MatrixInt dualToPrimal() const;

        /**
         * Returns a map from dual chains to primal chains that preserves
         * homology classes, where the chain dimension does not need to be
         * known until runtime.
         *
         * For C++ programmers who know \a subdim at compile time, you are
         * better off using the template function dualToPrimal<subdim>()
         * instead, which is slightly faster.
         *
         * The matrix that is returned should be thought of as acting on
         * column vectors.  Specifically, the <i>c</i>th column of the matrix
         * corresponds to the <i>c</i>th dual <i>subdim</i>-face of this
         * triangulation, and the <i>r</i>th row corresponds to the <i>r</i>th
         * primal <i>subdim</i>-face of this triangulation.
         *
         * We index and orient these dual and primal faces in the same manner
         * as dualBoundaryMap() and boundaryMap() respectively.
         * In particular, dual faces are indexed in the same order as the
         * primal (<i>dim</i>-<i>subdim</i>)-faces of the triangulation that
         * they are dual to, except that we omit primal boundary faces.
         * See dualBoundaryMap() and boundaryMap() for further details.
         *
         * The key feature of this map is that, if a column vector \a v
         * represents a cycle \a c in the dual chain complex (i.e., it is a
         * chain with zero boundary), and if this map is represented by the
         * matrix \a M, then the vector `M×v` represents a cycle in the
         * primal chain complex that belongs to the same <i>subdim</i>th
         * homology class as \a c.
         *
         * Regarding implementation: the map is constructed by (i) subdividing
         * each dual face into smaller <i>subdim</i>-simplices whose vertices
         * are barycentres of primal faces of different dimensions, (ii) moving
         * each barycentre to vertex 0 of the corresponding face, and then
         * (iii) discarding any resulting simplices with repeated vertices
         * (which become "flattened" to a dimension less than \a subdim).
         *
         * \pre This trianguation is valid, non-empty, and non-ideal.
         * Note that Regina can only detect ideal triangulations in
         * \ref stddim "standard dimensions"; for higher dimensions it is
         * the user's reponsibility to confirm this some other way.
         *
         * \exception InvalidArgument The chain dimension \a subdim is outside
         * the supported range (as documented for the \a subdim argument below).
         *
         * \param subdim the chain dimension; this must be between
         * 0 and (\a dim - 1) inclusive.
         * \return the map from dual <i>subdim</i>-chains to primal
         * <i>subdim</i>-chains.
         */
        MatrixInt dualToPrimal(int subdim) const;

        /*@}*/
        /**
         * \name Skeletal Transformations
         */
        /*@{*/

        /**
         * Relabels the vertices of top-dimensional simplices in this
         * triangulation so that all simplices are oriented consistently,
         * if possible.
         *
         * This routine works by flipping vertices (\a dim - 1) and \a dim
         * of each top-dimensional simplices that has negative orientation.
         * The result will be a triangulation where the top-dimensional
         * simplices have their vertices labelled in a way that preserves
         * orientation across adjacent facets.
         * In particular, every gluing permutation will have negative sign.
         *
         * If this triangulation includes both orientable and
         * non-orientable components, the orientable components will be
         * oriented as described above and the non-orientable
         * components will be left untouched.
         */
        void orient();

        /**
         * Relabels the vertices of top-dimensional simplices in this
         * triangulation so that all simplices reflect their orientation.
         * In particular, if this triangulation is oriented,
         * then it will be converted into an isomorphic triangulation
         * with the opposite orientation.
         *
         * This routine works by flipping vertices (\a dim - 1) and \a dim
         * of every top-dimensional simplex.
         */
        void reflect();

        /**
         * Checks the eligibility of and/or performs a
         * (\a dim + 1 - \a k)-(\a k + 1) Pachner move about the given
         * <i>k</i>-face.  This involves replacing the (\a dim + 1 - \a k)
         * top-dimensional simplices meeting that <i>k</i>-face with
         * (\a k + 1) new top-dimensional simplices joined along a new
         * internal (\a dim - \a k)-face.
         * This can be done iff (i) the given <i>k</i>-face is valid and
         * non-boundary; (ii) the (\a dim + 1 - \a k) top-dimensional simplices
         * that contain it are distinct; and (iii) these simplices are joined
         * in such a way that the link of the given <i>k</i>-face is the
         * standard triangulation of the (\a dim - 1 - \a k)-sphere as
         * the boundary of a (\a dim - \a k)-simplex.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.  In
         * In the special case \a k = \a dim, the move is always legal
         * and so the \a check argument will simply be ignored.
         *
         * Note that after performing this move, all skeletal objects
         * (facets, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a v)
         * can no longer be used.
         *
         * If this triangulation is currently oriented, then this Pachner move
         * will label the new top-dimensional simplices in a way that
         * preserves the orientation.
         *
         * See the page on \ref pachner for definitions and terminology
         * relating to Pachner moves.  After the move, the new belt face
         * will be formed from vertices 0,1,...,(\a dim - \a k) of
         * `simplices().back()`.
         *
         * \warning For the case \a k = \a dim in Regina's
         * \ref stddim "standard dimensions", the labelling of the belt face
         * has changed as of Regina 5.96 (the first prerelease for Regina 6.0).
         * In versions 5.1 and earlier, the belt face was
         * `simplices().back()->vertex(dim)`, and as of version 5.96
         * it is now `simplices().back()->vertex(0)`.
         *
         * \pre If the move is being performed and no check is being run,
         * it must be known in advance that the move is legal.
         * \pre The given <i>k</i>-face is a <i>k</i>-face of this
         * triangulation.
         *
         * \param f the <i>k</i>-face about which to perform the move.
         * \param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * \param perform \c true if we are to perform the move
         * (defaults to \c true).
         * \return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         *
         * \tparam k the dimension of the given face.  This must be
         * between 0 and (\a dim) inclusive.  You can still perform
         * a Pachner move about a 0-face <i>dim</i>-face, but these moves
         * use specialised implementations (as opposed to this generic
         * template implementation).
         */
        template <int k>
        bool pachner(Face<dim, k>* f, bool check = true, bool perform = true);

        /*@}*/
        /**
         * \name Subdivisions, Extensions and Covers
         */
        /*@{*/

        /**
         * Converts this triangulation into its double cover.
         * Each orientable component will be duplicated, and each
         * non-orientable component will be converted into its
         * orientable double cover.
         */
        void makeDoubleCover();

        /**
         * Does a barycentric subdivision of the triangulation.  This is done 
         * in-place, i.e., the triangulation will be modified directly.
         *
         * Each top-dimensional simplex \a s is divided into
         * (\a dim + 1) factorial sub-simplices by placing an extra vertex at
         * the centroid of every face of every dimension.  Each of these
         * sub-simplices \a t is described by a permutation \a p of
         * (0, ..., \a dim).  The vertices of such a sub-simplex \a t are:
         *
         * - vertex \a p[0] of \a s;
         * - the centre of edge (\a p[0], \a p[1]) of \a s;
         * - the centroid of triangle (\a p[0], \a p[1], \a p[2]) of \a s;
         * - ...
         * - the centroid of face (\a p[0], \a p[1], \a p[2], \a p[\a dim])
         *   of \a s, which is the entire simplex \a s itself.
         *
         * The sub-simplices have their vertices numbered in a way that
         * mirrors the original simplex \a s:
         *
         * - vertex \a p[0] of \a s will be labelled \a p[0] in \a t;
         * - the centre of edge (\a p[0], \a p[1]) of \a s will be labelled
         *   \a p[1] in \a t;
         * - the centroid of triangle (\a p[0], \a p[1], \a p[2]) of \a s
         *   will be labelled \a p[2] in \a t;
         * - ...
         * - the centroid of \a s itself will be labelled \a p[\a dim] in \a t.
         *
         * In particular, if this triangulation is currently oriented, then
         * this barycentric subdivision will preserve the orientation.
         *
         * If simplex \a s has index \a i in the original triangulation, then
         * its sub-simplex corresponding to permutation \a p will have index
         * `((dim + 1)! * i + p.orderedSnIndex())` in the resulting
         * triangulation.  In other words: sub-simplices are ordered first
         * according to the original simplex that contains them, and then
         * according to the lexicographical ordering of the corresponding
         * permutations \a p.
         *
         * \pre \a dim is one of Regina's standard dimensions.
         * This precondition is a safety net, since in higher dimensions the
         * triangulation would explode too quickly in size (and for the
         * highest dimensions, possibly beyond the limits of \c size_t).
         *
         * \warning In dimensions 3 and 4, both the labelling and ordering of
         * sub-simplices in the subdivided triangulation has changed as of
         * Regina 5.1.  (Earlier versions of Regina made no guarantee about the
         * labelling and ordering; these guarantees are also new to Regina 5.1).
         *
         * \todo Lock the topological properties of the underlying manifold,
         * to avoid recomputing them after the subdivision.  However, only
         * do this for _valid_ triangulations (since we can have scenarios
         * where invalid triangulations becoming valid and ideal after
         * subdivision, which may change properties such as
         * Triangulation<4>::knownSimpleLinks).
         */
        void subdivide();

        /**
         * Deprecated routine that performs a barycentric subdivision of the
         * triangulation.
         *
         * \deprecated This routine has been renamed to subdivide(), both to
         * shorten the name but also to make it clearer that this triangulation
         * will be modified directly.
         *
         * \pre \a dim is one of Regina's standard dimensions.
         */
        [[deprecated]] void barycentricSubdivision();

        /**
         * Converts each real boundary component into a cusp (i.e., an
         * ideal vertex).  Only boundary components formed from real
         * (<i>dim</i>-1)-faces will be affected; ideal boundary components
         * are already cusps and so will not be changed.
         *
         * One side-effect of this operation is that all spherical
         * boundary components will be filled in with balls.
         *
         * This operation is performed by attaching a new <i>dim</i>-simplex to
         * each boundary (<i>dim</i>-1)-face, and then gluing these new
         * simplices together in a way that mirrors the adjacencies of the
         * underlying boundary facets.  Each boundary component will
         * thereby be pushed up through the new simplices and converted
         * into a cusp formed using vertices of these new simplices.
         *
         * In Regina's \ref stddim "standard dimensions", where triangulations
         * also support an idealToFinite() operation, this routine is a loose
         * converse of that operation.
         *
         * In dimension 2, every boundary component is spherical and so
         * this routine simply fills all the punctures in the underlying
         * surface.  (In dimension 2, triangulations cannot have cusps).
         *
         * \warning If a real boundary component contains vertices whose
         * links are not discs, this operation may have unexpected results.
         *
         * \return \c true if changes were made, or \c false if the
         * original triangulation contained no real boundary components.
         */
        bool finiteToIdeal();

        /*@}*/
        /**
         * \name Decompositions
         */
        /*@{*/

        /**
         * Returns the individual connected components of this triangulation.
         * This triangulation will not be modified.
         *
         * This function is new to Regina 7.0, and it has two important
         * changes of behaviour from the old splitIntoComponents() from
         * Regina 6.0.1 and earlier:
         *
         * - This function does not insert the resulting components into
         *   the packet tree.
         *
         * - This function does not assign labels to the new components.
         *
         * \return a list of individual component triangulations.
         */
        std::vector<Triangulation<dim>> triangulateComponents() const;

        /*@}*/
        /**
         * \name Isomorphism Testing
         */
        /*@{*/

        /**
         * Determines if this triangulation is combinatorially identical
         * to the given triangulation.
         *
         * Here "identical" means that the triangulations have the same
         * number of top-dimensional simplices, with gluings between the same
         * pairs of numbered simplices using the same gluing permutations.
         * In other words, "identical" means that the triangulations
         * are isomorphic via the identity isomorphism.
         *
         * For the less strict notion of _isomorphic_ triangulations,
         * which allows relabelling of the top-dimensional simplices and their
         * vertices, see isIsomorphicTo() instead.
         *
         * This test does _not_ examine the textual simplex descriptions,
         * as seen in Simplex<dim>::description(); these may still differ.
         * It also does not test whether lower-dimensional faces are
         * numbered identically (vertices, edges and so on); this routine
         * is only concerned with top-dimensional simplices.
         *
         * (At the time of writing, two identical triangulations will
         * always number their lower-dimensional faces in the same way.
         * However, it is conceivable that in future versions of Regina there
         * may be situations in which identical triangulations can acquire
         * different numberings for vertices, edges, and so on.)
         *
         * In Regina 6.0.1 and earlier, this comparison was called
         * isIdenticalTo().
         *
         * \param other the triangulation to compare with this.
         * \return \c true if and only if the two triangulations are
         * combinatorially identical.
         */
        bool operator == (const Triangulation<dim>& other) const;

        /**
         * Determines if this triangulation is not combinatorially identical
         * to the given triangulation.
         *
         * Here "identical" means that the triangulations have the same
         * number of top-dimensional simplices, with gluings between the same
         * pairs of numbered simplices using the same gluing permutations.
         * In other words, "identical" means that the triangulations
         * are isomorphic via the identity isomorphism.
         *
         * For the less strict notion of _isomorphic_ triangulations,
         * which allows relabelling of the top-dimensional simplices and their
         * vertices, see isIsomorphicTo() instead.
         *
         * This test does _not_ examine the textual simplex descriptions,
         * as seen in Simplex<dim>::description(); these may still differ.
         * It also does not test whether lower-dimensional faces are
         * numbered identically (vertices, edges and so on); this routine
         * is only concerned with top-dimensional simplices.
         *
         * (At the time of writing, two identical triangulations will
         * always number their lower-dimensional faces in the same way.
         * However, it is conceivable that in future versions of Regina there
         * may be situations in which identical triangulations can acquire
         * different numberings for vertices, edges, and so on.)
         *
         * \param other the triangulation to compare with this.
         * \return \c true if and only if the two triangulations are
         * not combinatorially identical.
         */
        bool operator != (const Triangulation<dim>& other) const;

        /**
         * Determines if this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * Two triangulations are _isomorphic_ if and only it is
         * possible to relabel their top-dimensional simplices and the
         * (<i>dim</i>+1) vertices of each simplex in a way that makes
         * the two triangulations combinatorially identical, as returned
         * by isIdenticalTo().
         *
         * Equivalently, two triangulations are isomorphic if and only if
         * there is a one-to-one and onto boundary complete combinatorial
         * isomorphism from this triangulation to \a other, as described
         * in the Isomorphism class notes.
         *
         * In particular, note that this triangulation and \a other must
         * contain the same number of top-dimensional simplices for such an
         * isomorphism to exist.
         *
         * If the triangulations are isomorphic, then this routine returns
         * one such boundary complete isomorphism (i.e., one such relabelling).
         * Otherwise it returns \nullopt.  Thus, to test whether an isomorphism
         * exists, you can just call `if (isIsomorphicTo(other))`.
         *
         * There may be many such isomorphisms between the two triangulations.
         * If you need to find _all_ such isomorphisms, you may call
         * findAllIsomorphisms() instead.
         *
         * If you need to ensure that top-dimensional simplices are labelled
         * the same in both triangulations (i.e., that the triangulations are
         * related by the _identity_ isomorphism), you should call the
         * stricter test isIdenticalTo() instead.
         *
         * \warning For large dimensions, this routine can become
         * extremely slow: its running time includes a factor of
         * (<i>dim</i>+1)!.
         *
         * \todo \opt Improve the complexity by choosing a simplex
         * mapping from each component and following gluings to
         * determine the others.
         *
         * \param other the triangulation to compare with this one.
         * \return details of the isomorphism if the two triangulations
         * are combinatorially isomorphic, or \nullopt otherwise.
         */
        std::optional<Isomorphism<dim>> isIsomorphicTo(
            const Triangulation<dim>& other) const;

        /**
         * Determines if an isomorphic copy of this triangulation is
         * contained within the given triangulation, possibly as a
         * subcomplex of some larger component (or components).
         *
         * Specifically, this routine determines if there is a boundary
         * incomplete combinatorial isomorphism from this triangulation
         * to \a other.  Boundary incomplete isomorphisms are described
         * in detail in the Isomorphism class notes.
         *
         * In particular, note that facets of top-dimensional simplices that
         * lie on the boundary of this triangulation need not correspond to
         * boundary facets of \a other, and that \a other may contain more
         * top-dimensional simplices than this triangulation.
         *
         * If a boundary incomplete isomorphism is found, the details of
         * this isomorphism are returned.  Thus, to test whether an isomorphism
         * exists, you can just call `if (isContainedIn(other))`.
         *
         * If more than one such isomorphism exists, only one will be
         * returned.  For a routine that returns all such isomorphisms,
         * see findAllSubcomplexesIn().
         *
         * \warning For large dimensions, this routine can become
         * extremely slow: its running time includes a factor of
         * (<i>dim</i>+1)!.
         *
         * \param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * \return details of the isomorphism if such a copy is found,
         * or \nullopt otherwise.
         */
        std::optional<Isomorphism<dim>> isContainedIn(
            const Triangulation<dim>& other) const;

        /**
         * Finds all ways in which this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * This routine behaves identically to isIsomorphicTo(), except that
         * instead of returning just one isomorphism, all such isomorphisms
         * will be found and processed.  See the isIsomorphicTo() notes for
         * details on this.
         *
         * For each isomorphism that is found, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be of type
         *   `(const Isomorphism<dim>&)`; this will be a reference to
         *   the isomorphism that was found.  If \a action wishes to keep the
         *   isomorphism, it should take a deep copy (not a reference), since
         *   the isomorphism may be changed and reused after \a action returns.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return a \c bool.  A return value of \c false
         *   indicates that the search for isomorphisms should continue,
         *   and a return value of \c true indicates that the search
         *   should terminate immediately.
         *
         * - This triangulation _must_ remain constant while the search
         *   runs (i.e., \a action must not modify the triangulation).
         *
         * \warning For large dimensions, this routine can become
         * extremely slow: its running time includes a factor of
         * (<i>dim</i>+1)!.
         *
         * \python There are two versions of this function
         * available in Python.  The first form is
         * `findAllIsomorphisms(other, action)`, which mirrors the C++
         * function: it takes \a action which may be a pure Python function,
         * the return value indicates whether \a action ever terminated the
         * search, but it does _not_ take an additonal argument list (\a args).
         * The second form is `findAllIsomorphisms(other)`, which
         * returns a Python list containing all of the isomorphisms that were
         * found.
         *
         * \param other the triangulation to compare with this one.
         * \param action a function (or other callable object) to call
         * for each isomorphism that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial isomorphism argument.
         * \return \c true if \a action ever terminated the search by returning
         * \c true, or \c false if the search was allowed to run to completion.
         */
        template <typename Action, typename... Args>
        bool findAllIsomorphisms(const Triangulation<dim>& other,
            Action&& action, Args&&... args) const;

        /**
         * Finds all ways in which an isomorphic copy of this triangulation
         * is contained within the given triangulation, possibly as a
         * subcomplex of some larger component (or components).
         *
         * This routine behaves identically to isContainedIn(), except
         * that instead of returning just one isomorphism (which may be
         * boundary incomplete and need not be onto), all such isomorphisms
         * will be found and processed.  See the isContainedIn() notes for
         * details on this.
         *
         * For each isomorphism that is found, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be of type
         *   `(const Isomorphism<dim>&)`; this will be a reference to
         *   the isomorphism that was found.  If \a action wishes to keep the
         *   isomorphism, it should take a deep copy (not a reference), since
         *   the isomorphism may be changed and reused after \a action returns.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return a \c bool.  A return value of \c false
         *   indicates that the search for isomorphisms should continue,
         *   and a return value of \c true indicates that the search
         *   should terminate immediately.
         *
         * - This triangulation _must_ remain constant while the search
         *   runs (i.e., \a action must not modify the triangulation).
         *
         * \warning For large dimensions, this routine can become
         * extremely slow: its running time includes a factor of
         * (<i>dim</i>+1)!.
         *
         * \python There are two versions of this function
         * available in Python.  The first form is
         * `findAllSubcomplexesIn(other, action)`, which mirrors the C++
         * function: it takes \a action which may be a pure Python function,
         * the return value indicates whether \a action ever terminated the
         * search, but it does _not_ take an additonal argument list (\a args).
         * The second form is `findAllSubcomplexesIn(other)`, which
         * returns a Python list containing all of the isomorphisms that were
         * found.
         *
         * \param other the triangulation in which to search for
         * isomorphic copies of this triangulation.
         * \param action a function (or other callable object) to call
         * for each isomorphism that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial isomorphism argument.
         * \return \c true if \a action ever terminated the search by returning
         * \c true, or \c false if the search was allowed to run to completion.
         */
        template <typename Action, typename... Args>
        bool findAllSubcomplexesIn(const Triangulation<dim>& other,
            Action&& action, Args&&... args) const;

        /**
         * Relabel the top-dimensional simplices and their vertices so that
         * this triangulation is in canonical form.  This is essentially
         * the lexicographically smallest labelling when the facet
         * gluings are written out in order.
         *
         * Two triangulations are isomorphic if and only if their canonical
         * forms are identical.
         *
         * The lexicographic ordering assumes that the facet gluings are
         * written in order of simplex index and then facet number.
         * Each gluing is written as the destination simplex index
         * followed by the gluing permutation (which in turn is written
         * as the images of 0,1,...,<i>dim</i> in order).
         *
         * \pre This routine currently works only when the triangulation
         * is connected.  It may be extended to work with disconnected
         * triangulations in later versions of Regina.
         *
         * \return \c true if the triangulation was changed, or \c false
         * if the triangulation was in canonical form to begin with.
         */
        bool makeCanonical();

        /*@}*/
        /**
         * \name Building Triangulations
         */
        /*@{*/

        /**
         * Inserts a copy of the given triangulation into this triangulation.
         *
         * The top-dimensional simplices of \a source will be copied into this
         * triangulation in the same order in which they appear in \a source.
         * That is, if the original size of this triangulation was \a S,
         * then the simplex at index \a i in \a source will be copied into
         * this triangulation as a new simplex at index <i>S</i>+<i>i</i>.
         *
         * The copies will use the same vertex numbering and descriptions
         * as the original simplices from \a source, and any gluings
         * between the simplices of \a source will likewise be copied
         * across as gluings between their copies in this triangulation.
         *
         * This routine behaves correctly when \a source is this triangulation.
         *
         * \param source the triangulation whose copy will be inserted.
         */
        void insertTriangulation(const Triangulation<dim>& source);

        /*@}*/
        /**
         * \name Exporting Triangulations
         */
        /*@{*/

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Constructs the isomorphism signature of the given type for this
         * triangulation.  Support for different _types_ of signature is new
         * to Regina 7.0 (see below for details); all isomorphism signatures
         * created in Regina 6.0.1 or earlier are of the default type
         * IsoSigClassic.
         *
         * An _isomorphism signature_ is a compact representation
         * of a triangulation that uniquely determines the triangulation up to
         * combinatorial isomorphism.  That is, for any fixed signature type
         * \a T, two triangulations of dimension \a dim are combinatorially
         * isomorphic if and only if their isomorphism signatures of
         * type \a T are the same.
         *
         * The length of an isomorphism signature is proportional to
         * `n log n`, where \a n is the number of top-dimenisonal
         * simplices.  The time required to construct it is worst-case
         * `O((dim!) n² log² n)`.  Whilst this is fine for large
         * triangulations, it becomes very slow for large _dimensions_;
         * the main reason for introducing different signature types is that
         * some alternative types can be much faster to compute in practice.
         *
         * Whilst the format of an isomorphism signature bears some
         * similarity to dehydration strings for 3-manifolds, they are more
         * general: isomorphism signatures can be used with any triangulations,
         * including closed, bounded and/or disconnected triangulations,
         * as well as triangulations with many simplices.  Note also that
         * 3-manifold dehydration strings are not unique up to isomorphism
         * (they depend on the particular labelling of tetrahedra).
         *
         * The routine fromIsoSig() can be used to recover a triangulation
         * from an isomorphism signature (only if the default encoding has
         * been used, but it does not matter which signature type was used).
         * The triangulation recovered might not be identical to the original,
         * but it _will_ be combinatorially isomorphic.  If you need the
         * precise relabelling, you can call isoSigDetail() instead.
         *
         * Regina supports several different variants of isomorphism signatures,
         * which are tailored to different computational needs; these are
         * currently determined by the template parameters \a Type and
         * \a Encoding:
         *
         * - The \a Type parameter identifies which signature type is to be
         *   constructed.  Essentially, different signature types use
         *   different rules to determine which labelling of a triangulation
         *   is "canonical".  The default type IsoSigClassic is slow
         *   (it never does better than the worst-case time described above);
         *   its main advantage is that it is consistent with the original
         *   implementation of isomorphism signatures in Regina 4.90.
         *
         * - The \a Encoding parameter controls how Regina encodes a canonical
         *   labelling into a final signature.  The default encoding
         *   IsoSigPrintable returns a std::string consisting entirely of
         *   printable characters in the 7-bit ASCII range.  Importantly, this
         *   default encoding is currently the only encoding from which Regina
         *   can _reconstruct_ a triangulation from its isomorphism signature.
         *
         * You may instead pass your own type and/or encoding parameters as
         * template arguments.  Currently this facility is for internal use
         * only, and the requirements for type and encoding parameters may
         * change in future versions of Regina.  At present:
         *
         * - The \a Type parameter should be a class that is constructible
         *   from a componenent reference, and that offers the member functions
         *   simplex(), perm() and next(); see the implementation of
         *   IsoSigClassic for details.
         *
         * - The \a Encoding parameter should be a class that offers a
         *   \a Signature type alias, and static functions emptySig() and
         *   encode().  See the implementation of IsoSigPrintable for details.
         *
         * For a full and precise description of the classic isomorphism
         * signature format for 3-manifold triangulations, see
         * _Simplification paths in the Pachner graphs of closed orientable
         * 3-manifold triangulations_, Burton, 2011,
         * `arXiv:1110.6080`.  The format for other dimensions is
         * essentially the same, but with minor dimension-specific adjustments.
         *
         * \python Although this is a templated function, all of the
         * variants supplied with Regina are available to Python users.
         * For the default type and encoding, you can simply call isoSig().
         * For other signature types, you should call the function as
         * isoSig_<i>Type</i>, where the suffix \a Type is an abbreviated
         * version of the \a Type template parameter; one such example is
         * \c isoSig_EdgeDegrees (for the case where \a Type is
         * the class IsoSigEdgeDegrees).  Currently Regina only offers one
         * encoding (the default), and so there are no suffixes for encodings.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different dimensions \a p and \a q.
         *
         * \return the isomorphism signature of this triangulation.
         */
        template <class Type = IsoSigClassic<dim>,
            class Encoding = IsoSigPrintable<dim>>
        typename Encoding::Signature isoSig() const;

        /**
         * Constructs the isomorphism signature for this triangulation, along
         * with the relabelling that will occur when the triangulation is
         * reconstructed from it.
         *
         * Essentially, an isomorphism signature is a compact representation
         * of a triangulation that uniquely determines the triangulation up to
         * combinatorial isomorphism.  See isoSig() for much more detail on
         * isomorphism signatures as well as the support for different
         * signature types and encodings.
         *
         * As described in the isoSig() notes, you can call fromIsoSig() to
         * recover a triangulation from an isomorphism signature (assuming
         * the default encoding was used).  Whilst the triangulation that is
         * recovered will be combinatorially isomorphic to the original,
         * it might not be identical.  This routine returns not only the
         * isomorphism signature, but also an isomorphism that describes the
         * precise relationship between this triangulation and the
         * reconstruction from fromIsoSig().
         *
         * Specifically, if this routine returns the pair
         * (\a sig, \a relabelling), this means that the triangulation
         * reconstructed from `fromIsoSig(sig)` will be identical to
         * `relabelling(this)`.
         *
         * \python Although this is a templated function, all of the
         * variants supplied with Regina are available to Python users.  For
         * the default type and encoding, you can simply call isoSigDetail().
         * For other signature types, you should call the function as
         * isoSigDetail_<i>Type</i>, where the suffix \a Type is an abbreviated
         * version of the \a Type template parameter; one such example is
         * \c isoSigDetail_EdgeDegrees (for the case where \a Type is
         * the class IsoSigEdgeDegrees).  Currently Regina only offers one
         * encoding (the default), and so there are no suffixes for encodings.
         *
         * \pre This triangulation must be non-empty and connected.  The
         * facility to return a relabelling for disconnected triangulations
         * may be added to Regina in a later release.
         *
         * \exception FailedPrecondition This triangulation is either
         * empty or disconnected.
         *
         * \return a pair containing (i) the isomorphism signature of this
         * triangulation, and (ii) the isomorphism between this triangulation
         * and the triangulation that would be reconstructed from fromIsoSig().
         */
        template <class Type = IsoSigClassic<dim>,
            class Encoding = IsoSigPrintable<dim>>
        std::pair<typename Encoding::Signature, Isomorphism<dim>> isoSigDetail()
            const;

        /**
         * Writes the tight encoding of this triangulation to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Returns C++ code that can be used to reconstruct this triangulation.
         *
         * This code will call Triangulation<dim>::fromGluings(), passing
         * a hard-coded C++ initialiser list.
         *
         * The main purpose of this routine is to generate this hard-coded
         * initialiser list, which can be tedious and error-prone to write
         * by hand.
         *
         * Note that the number of lines of code produced grows linearly
         * with the number of simplices.  If this triangulation is very
         * large, the returned string will be very large as well.
         *
         * \return the C++ code that was generated.
         */
        std::string dumpConstruction() const;

        /*@}*/
        /**
         * \name Importing Triangulations
         */
        /*@{*/

        /**
         * Creates a triangulation from a hard-coded list of gluings.
         *
         * This routine takes a C++ initialiser list, which makes it useful
         * for creating hard-coded examples directly in C++ code without
         * needing to write a tedious sequence of calls to Simplex<dim>::join().
         *
         * Each element of the initialiser list should be a tuple of the form
         * (\a simp, \a facet, \a adj, \a gluing), which indicates that facet
         * \a facet of top-dimensional simplex number \a simp should be glued
         * to top-dimensional simplex number \a adj using the permutation
         * \a gluing.  In other words, such a tuple encodes the same information
         * as calling `simplex(simp).join(facet, simplex(adj), gluing)`
         * upon the triangulation being constructed.
         *
         * Every gluing should be encoded from _one direction only_.
         * This means, for example, that to build a closed 3-manifold
         * triangulation with \a n tetrahedra, you would pass a list of
         * 2<i>n</i> such tuples.  If you attempt to make the same gluing
         * twice (e.g., once from each direction), then this routine will
         * throw an exception.
         *
         * Any facet of a simplex that does not feature in the given list of
         * gluings (either as a source or a destination) will be left as a
         * boundary facet.
         *
         * Note that, as usual, the top-dimensional simplices are numbered
         * 0,...,(<i>size</i>-1), and the facets of each simplex are numbered
         * 0,...,\a dim.
         *
         * As an example, you can construct the figure eight knot complement
         * using the following code:
         *
         * \code
         * Triangulation<3> tri = Triangulation<3>::fromGluings(2, {
         *     { 0, 0, 1, {1,3,0,2} }, { 0, 1, 1, {2,0,3,1} },
         *     { 0, 2, 1, {0,3,2,1} }, { 0, 3, 1, {2,1,0,3} }});
         * \endcode
         *
         * \note If you have an existing triangulation that you would like to
         * hard-code in this way, you can call dumpConstruction() to generate
         * the corresponding C++ source code.
         *
         * \exception InvalidArgument The given list of gluings does not
         * correctly describe a triangulation with \a size top-dimensional
         * simplices.
         *
         * \nopython Instead, use the variant of fromGluings() that takes this
         * same data using a Python list (which need not be constant).
         *
         * \param size the number of top-dimensional simplices in the
         * triangulation to construct.
         * \param gluings describes the gluings between these top-dimensional
         * simplices, as described above.
         * \return the reconstructed triangulation.
         */
        static Triangulation<dim> fromGluings(size_t size,
            std::initializer_list<std::tuple<size_t, int, size_t, Perm<dim+1>>>
            gluings);

        /**
         * Creates a triangulation from a list of gluings.
         *
         * This routine is an analogue to the variant of fromGluings() that
         * takes a C++ initialiser list; however, here the input data may be
         * constructed at runtime (which makes it accessible to Python,
         * amongst other things).
         *
         * The iterator range (\a beginGluings, \a endGluings) should encode
         * the list of gluings for the triangulation.  Each iterator in
         * this range must dereference to a tuple of the form
         * (\a simp, \a facet, \a adj, \a gluing); here \a simp, \a facet
         * and \a adj are all integers, and \a gluing is of type Perm<dim+1>.
         * Each such tuple indicates that facet \a facet of top-dimensional
         * simplex number \a simp should be glued to top-dimensional simplex
         * number \a adj using the permutation \a gluing.  In other words,
         * such a tuple encodes the same information as calling
         * `simplex(simp).join(facet, simplex(adj), gluing)`
         * upon the triangulation being constructed.
         *
         * Every gluing should be encoded from _one direction only_.
         * This means, for example, that to build a closed 3-manifold
         * triangulation with \a n tetrahedra, you would pass a list of
         * 2<i>n</i> such tuples.  If you attempt to make the same gluing
         * twice (e.g., once from each direction), then this routine will
         * throw an exception.
         *
         * Any facet of a simplex that does not feature in the given list of
         * gluings (either as a source or a destination) will be left as a
         * boundary facet.
         *
         * Note that, as usual, the top-dimensional simplices are numbered
         * 0,...,(<i>size</i>-1), and the facets of each simplex are numbered
         * 0,...,\a dim.
         *
         * As an example, Python users can construct the figure eight knot
         * complement as follows:
         *
         * \code{.py}
         * tri = Triangulation3.fromGluings(2, [
         *     ( 0, 0, 1, Perm4(1,3,0,2) ), ( 0, 1, 1, Perm4(2,0,3,1) ),
         *     ( 0, 2, 1, Perm4(0,3,2,1) ), ( 0, 3, 1, Perm4(2,1,0,3) )])
         * \endcode
         *
         * \note The assumption is that the iterators dereference to a
         * std::tuple<size_t, int, size_t, Perm<dim+1>>.  However, this is
         * not strictly necessary - the dereferenced type may be any type that
         * supports std::get (and for which std::get<0..3>() yields suitable
         * integer/permutation types).
         *
         * \exception InvalidArgument The given list of gluings does not
         * correctly describe a triangulation with \a size top-dimensional
         * simplices.
         *
         * \python The gluings should be passed as a single Python
         * list of tuples (not an iterator pair).
         *
         * \param size the number of top-dimensional simplices in the
         * triangulation to construct.
         * \param beginGluings the beginning of the list of gluings, as
         * described above.
         * \param endGluings a past-the-end iterator indicating the end
         * of the list of gluings.
         * \return the reconstructed triangulation.
         */
        template <typename Iterator>
        static Triangulation<dim> fromGluings(size_t size,
            Iterator beginGluings, Iterator endGluings);

        /**
         * Recovers a full triangulation from an isomorphism signature.
         *
         * See isoSig() for more information on isomorphism signatures.
         * It will be assumed that the signature describes a triangulation of
         * dimension \a dim.
         *
         * Currently this routine only supports isomorphism signatures
         * that were created with the default encoding (i.e., there was
         * no \a Encoding template parameter passed to isoSig()).
         *
         * Calling isoSig() followed by fromIsoSig() is not guaranteed to
         * produce an _identical_ triangulation to the original, but it
         * is guaranteed to produce a combinatorially _isomorphic_
         * triangulation.  In other words, fromIsoSig() may reconstruct the
         * triangulation with its simplices and/or vertices relabelled.
         * The optional argument to isoSig() allows you to determine the
         * precise relabelling that will be used, if you need to know it.
         *
         * For a full and precise description of the isomorphism signature
         * format for 3-manifold triangulations, see _Simplification paths in
         * the Pachner graphs of closed orientable 3-manifold triangulations_,
         * Burton, 2011, `arXiv:1110.6080`.  The format for other dimensions is
         * essentially the same, but with minor dimension-specific adjustments.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different dimensions \a p and \a q.
         *
         * \exception InvalidArgument The given string was not a valid
         * <i>dim</i>-dimensional isomorphism signature created using
         * the default encoding.
         *
         * \param sig the isomorphism signature of the triangulation to
         * construct.  Note that isomorphism signatures are case-sensitive
         * (unlike, for example, dehydration strings for 3-manifolds).
         * \return the reconstructed triangulation.
         */
        static Triangulation<dim> fromIsoSig(const std::string& sig);

        /**
         * Alias for fromIsoSig(), to recover a full triangulation from an
         * isomorphism signature.
         *
         * This alias fromSig() is provided to assist with generic code
         * that can work with both triangulations and links.
         *
         * See fromIsoSig() for further details.
         *
         * \exception InvalidArgument The given string was not a valid
         * <i>dim</i>-dimensional isomorphism signature created using
         * the default encoding.
         *
         * \param sig the isomorphism signature of the triangulation to
         * construct.  Note that isomorphism signatures are case-sensitive
         * (unlike, for example, dehydration strings for 3-manifolds).
         * \return the reconstructed triangulation.
         */
        static Triangulation<dim> fromSig(const std::string& sig);

        /**
         * Deduces the number of top-dimensional simplices in a
         * connected triangulation from its isomorphism signature.
         *
         * See isoSig() for more information on isomorphism signatures.
         * It will be assumed that the signature describes a triangulation of
         * dimension \a dim.
         *
         * Currently this routine only supports isomorphism signatures
         * that were created with the default encoding (i.e., there was
         * no \a Encoding template parameter passed to isoSig()).
         *
         * If the signature describes a connected triangulation, this
         * routine will simply return the size of that triangulation
         * (e.g., the number of tetrahedra in the case \a dim = 3).
         * You can also pass an isomorphism signature that describes a
         * disconnected triangulation; however, this routine will only
         * return the number of top-dimensional simplices in the first
         * connected component.  If you need the total size of a
         * disconnected triangulation, you will need to reconstruct the
         * full triangulation by calling fromIsoSig() instead.
         *
         * This routine is very fast, since it only examines the first
         * few characters of the isomorphism signature (in which the size
         * of the first component is encoded).  However, a side-effect
         * of this is that it is possible to pass an _invalid_ isomorphism
         * signature and still receive a positive result.  If you need to
         * test whether a signature is valid or not, you must call fromIsoSig()
         * instead, which will examine the entire signature in full.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different dimensions \a p and \a q.
         *
         * \param sig the isomorphism signature of a <i>dim</i>-dimensional
         * triangulation.  Note that isomorphism signature are case-sensitive
         * (unlike, for example, dehydration strings for 3-manifolds).
         * \return the number of top-dimensional simplices in the first
         * connected component, or 0 if this could not be determined
         * because the given string was not a valid isomorphism signature
         * created using the default encoding.
         */
        static size_t isoSigComponentSize(const std::string& sig);

        /**
         * Reconstructs a triangulation from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input routine _may_ contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of a <i>dim</i>-dimensional triangulation.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for a <i>dim</i>-dimensional triangulation.
         * \return the triangulation represented by the given tight encoding.
         */
        static Triangulation<dim> tightDecode(std::istream& input);

        /*@}*/

    protected:
        /**
         * Sets this to be a (deep) copy of the given triangulation.
         *
         * This will also clone any computed properties (such as homology,
         * fundamental group, and so on), as well as the skeleton (vertices,
         * edges, components, etc.).  In particular, this triangulation
         * will use the same numbering and labelling for all skeletal objects
         * as in the source triangulation.
         *
         * TriangulationBase never calls this operator itself; it is only
         * ever called by the Triangulation<dim> assignment operator.
         *
         * \param src the triangulation to copy.
         * \return a reference to this triangulation.
         */
        TriangulationBase& operator = (const TriangulationBase& src);

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
         * TriangulationBase never calls this operator itself; it is only
         * ever called by the Triangulation<dim> assignment operator.
         *
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \warning This operator does not touch any properties managed
         * by the derived class Triangulation<dim>.  It is assumed that
         * this is being called by the Triangulation<dim> assignment operator,
         * and that this derived class operator will manage its own properties
         * in whatever way it deems best.
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
        TriangulationBase& operator = (TriangulationBase&& src);

        /**
         * Ensures that all "on demand" skeletal objects have been calculated.
         */
        void ensureSkeleton() const;

        /**
         * Determines whether the skeletal objects and properties of this
         * triangulation have been calculated.
         *
         * These are only calculated "on demand", when a skeletal property
         * is first queried.
         *
         * \return \c true if and only if the skeleton has been calculated.
         */
        bool calculatedSkeleton() const;

        /**
         * Calculates all skeletal objects for this triangulation.
         *
         * For this parent class, calculateSkeleton() computes properties such
         * as connected components, orientability, and lower-dimensional faces.
         * Some Triangulation<dim> subclasses may track additional skeletal
         * data, in which case they should reimplement this function.  Their
         * reimplementations _must_ call this parent implementation.
         *
         * You should never call this function directly; instead call
         * ensureSkeleton() instead.
         *
         * For developers: any data members that are computed and stored by
         * calculateSkeleton() would typically also need to be cloned by
         * cloneSkeleton().  Therefore any changes or extensions to
         * calculateSkeleton() would typically need to come with analogous
         * changes or extensions to cloneSkeleton() also.
         *
         * \pre No skeletal objects have been computed, and the
         * corresponding internal lists are all empty.
         *
         * \warning Any call to calculateSkeleton() must first cast down to
         * Triangulation<dim>, to ensure that you are catching the subclass
         * implementation if this exists.  You should never directly call this
         * parent implementation (unless of course you are reimplementing
         * calculateSkeleton() in a Triangulation<dim> subclass).
         */
        void calculateSkeleton();

        /**
         * Builds the skeleton of this triangulation as a clone of the skeleton
         * of the given triangulation.  This clones all skeletal objects (e.g.,
         * faces, components and boundary components) and skeletal properties
         * (e.g., validity and orientability).  In general, this function
         * clones the same properties and data that calculateSkeleton()
         * computes.
         *
         * For this parent class, cloneSkeleton() clones properties and data
         * that are common to all dimensions.  Some Triangulation<dim>
         * subclasses may track additional skeletal properties or data,
         * in which case they should reimplement this function (just as they
         * also reimplement calculateSkeleton()).  Their reimplementations
         * _must_ call this parent implementation.
         *
         * This function is intended only for use by the copy constructor
         * (and related "copy-like" constructors), and the copy assignment
         * operator.  Other code should typically _not_ need to call this
         * function directly.
         *
         * The real point of this routine is to ensure that, when a
         * triangulation is cloned, its skeleton is cloned with exactly
         * the same numbering/labelling of its skeletal objects.  To this end,
         * it is fine to leave some "large" skeletal properties to be computed
         * on demand where this is allowed (e.g., triangulated vertex links
         * or triangulated boundary components, which are allowed to remain
         * uncomputed until required, even when the full skeleton _has_
         * been computed).
         *
         * \pre No skeletal objects have been computed for this triangulation,
         * and the corresponding internal lists are all empty.
         * \pre The skeleton has been fully computed for the given source
         * triangulation.
         * \pre The given source triangulation is combinatorially identical
         * to this triangulation (i.e., both triangulations have the same
         * number of top-dimensional simplices, with gluings between the same
         * pairs of numbered simplices using the same gluing permutations).
         *
         * \warning Any call to cloneSkeleton() must first cast down to
         * Triangulation<dim>, to ensure that you are catching the subclass
         * implementation if this exists.  You should never directly call this
         * parent implementation (unless of course you are reimplementing
         * cloneSkeleton() in a Triangulation<dim> subclass).
         *
         * \param src the triangulation whose skeleton should be cloned.
         */
        void cloneSkeleton(const TriangulationBase& src);

        /**
         * Clears all properties that are managed by this base class.
         * This includes deleting all skeletal objects and emptying the
         * corresponding internal lists, as well as clearing other cached
         * properties and deallocating the corresponding memory where required.
         *
         * Note that TriangulationBase almost never calls this routine itself
         * (the one exception is the copy assignment operator).
         * Typically clearBaseProperties() is only ever called by
         * Triangulation<dim>::clearAllProperties(), which in turn is called by
         * "atomic" routines that change the triangluation (before firing
         * packet change events), as well as the Triangulation<dim> destructor.
         */
        void clearBaseProperties();

        /**
         * Swaps all data that is managed by this base class, including
         * simplices, skeletal data, cached properties and the snapshotting
         * data, with the given triangulation.
         *
         * Note that TriangulationBase never calls this routine itself.
         * Typically swapBaseData() is only ever called by
         * Triangulation<dim>::swap().
         *
         * \param other the triangulation whose data should be
         * swapped with this.
         */
        void swapBaseData(TriangulationBase<dim>& other);

        /**
         * Writes a chunk of XML containing properties of this triangulation.
         * This routine covers those properties that are managed by this base
         * class TriangulationBase and that have already been computed for this
         * triangulation.
         *
         * This routine is typically called from within
         * Triangulation<dim>::writeXMLPacketData().  The XML elements
         * that it writes are child elements of the \c tri element.
         *
         * \param out the output stream to which the XML should be written.
         */
        void writeXMLBaseProperties(std::ostream& out) const;

    private:
        /**
         * Internal to calculateSkeleton().
         *
         * This routine calculates all <i>subdim</i>-faces for the given
         * triangulation.
         *
         * See calculateSkeleton() for further details.
         *
         * This _should_ have been an ordinary member function (not static),
         * but it caused an internal compiler error in gcc8 (see gcc bug #86594,
         * which is fixed in gcc9).  Making the function static is a workaround
         * that we will need to keep until we drop support for gcc8.
         *
         * \tparam subdim the dimension of the faces to compute.
         * This must be between 0 and (\a dim - 1) inclusive.
         */
        template <int subdim>
        static void calculateFaces(TriangulationBase<dim>* tri);

        /**
         * Internal to calculateSkeleton().
         *
         * This routine calculates all real boundary components.
         *
         * See calculateSkeleton() for further details.
         */
        void calculateRealBoundary();

        /**
         * Internal to calculateRealBoundary().
         *
         * This routine identifies and marks all <i>subdim</i>-faces within
         * the given boundary facet.
         *
         * It does not handle ridges or facets, so if \a subdim is greater
         * than <i>dim</i>-3 then this routine does nothing.
         *
         * See calculateRealBoundary() for further details.
         */
        template <int subdim>
        void calculateBoundaryFaces(BoundaryComponent<dim>* bc,
            Face<dim, dim-1>* facet);

        /**
         * Internal to cloneSkeleton().
         *
         * This routine takes a face of the source triangulation, and
         * returns the corresponding face of this triangulation.
         *
         * \pre The <i>subdim</i>-faces of the source triangulation have
         * already been "partially" cloned, in that all corresponding
         * <i>subdim</i>-face objects should have been constructed and
         * inserted into the corresponding face lists for _this_ triangulation
         * in the correct order.  It does not matter if the internal data for
         * these cloned facial objects is not yet completely filled.
         */
        template <int subdim>
        Face<dim, subdim>* clonedFace(const Face<dim, subdim>* src) const;

        /**
         * Internal to cloneSkeleton().
         *
         * This routine clones the list of all <i>k</i>-faces of the source
         * triangulation, for some fixed \a k.  The list is passed as the
         * argument \a srcFaces; the facial dimension \a k will be determined
         * automatically from its type.
         *
         * See cloneSkeleton() for further details.
         *
         * \param srcFaces the list of all <i>k</i>-faces of the source
         * triangulation, as stored in the Triangulation data structure.
         */
        template <typename FaceList>
        void cloneFaces(const FaceList& srcFaces);

        /**
         * Internal to cloneSkeleton().
         *
         * This routine clones the list of all <i>k</i>-faces of some
         * individual boundary component of the source triangulation, for some
         * fixed \a k.  The list is passed as the argument \a srcFaces; the
         * facial dimension \a k will be determined automatically from its type.
         *
         * See cloneSkeleton() for further details.
         *
         * \param bc a boundary component of this triangulation.  Typically
         * this will hold incomplete data, since its internal face lists are
         * still in the process of being filled.
         * \param srcFaces the list of all <i>k</i>-faces of the corresponding
         * boundary component in the source triangulation, as stored in the
         * BoundaryComponent data structure.
         */
        template <typename FaceList>
        void cloneBoundaryFaces(BoundaryComponent<dim>* bc,
                const FaceList& srcFaces);

        /**
         * Internal to isoSig().
         *
         * Constructs a candidate isomorphism signature for a single
         * component of this triangulation.  This candidate signature
         * assumes that the given simplex with the given labelling
         * of its vertices becomes simplex zero with vertices
         * 0,...,\a dim under the "canonical isomorphism".
         *
         * \param simp the index of some top-dimensional simplex in this
         * triangulation.
         * \param vertices some ordering of the vertices of the given simplex.
         * \param relabelling if this is non-null, it will be filled with the
         * canonical isomorphism; in this case it must already have been
         * constructed for the correct number of simplices.
         * \return the candidate isomorphism signature.
         */
        template <class Encoding>
        typename Encoding::Signature isoSigFrom(size_t simp,
            const Perm<dim+1>& vertices, Isomorphism<dim>* relabelling) const;

        /**
         * Determines if an isomorphic copy of this triangulation is
         * contained within the given triangulation.
         *
         * If the argument \a completeIsomorphism is \c true, the
         * isomorphism must be onto and boundary complete.
         * That is, this triangulation must be combinatorially
         * isomorphic to the given triangulation.
         *
         * If the argument \a completeIsomorphism is \c false, the
         * isomorphism may be boundary incomplete and may or may not be
         * onto.  That is, this triangulation must appear as a
         * subcomplex of the given triangulation, possibly with some
         * original boundary facets joined to new top-dimensional simplices.
         *
         * See the Isomorphism class notes for further details
         * regarding boundary complete and boundary incomplete
         * isomorphisms.
         *
         * For each isomorphism that is found, this routine will call
         * \a action, passing a const reference to the isomorphism as well as
         * the given argument list \a args.  See the documentation for
         * findAllIsomorphisms() and findAllSubcomplexesIn() for details on
         * how \a action should behave.
         *
         * \param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * \param complete \c true if isomorphisms must be
         * onto and boundary complete, or \c false if neither of these
         * restrictions should be imposed.
         * \param action a function (or other callable object) to call
         * for each isomorphism that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial isomorphism argument.
         * \return \c true if \a action ever terminated the search by returning
         * \c true, or \c false if the search was allowed to run to completion.
         */
        template <typename Action, typename... Args>
        bool findIsomorphisms(const Triangulation<dim>& other,
                bool complete, Action&& action, Args&&... args) const;

        /**
         * Internal to findIsomorphisms().
         *
         * Examines basic properties of this and the given triangulation to
         * find any immediate evidence that there can be no isomorphic copy
         * of this triangulation within the given triangulation.
         *
         * This routine tests basic properties such as orientability and
         * the sizes of the components, as well as degrees of the faces
         * of each dimension.
         *
         * \pre The skeleton of both this and the given triangulation
         * have been computed.
         * \pre This triangulation is non-empty.
         *
         * \param other the triangulation in which we are searching for an
         * isomorphic copy of this triangulation.
         * \param complete \c true if the isomorphism must be
         * onto and boundary complete, or \c false if neither of these
         * restrictions should be imposed.
         * \return \c true if no immediate obstructions were found, or
         * \c false if evidence was found that such an isomorphism
         * cannot exist.
         */
        bool compatible(const Triangulation<dim>& other, bool complete) const;

        /**
         * Reorders the <i>subdim</i>-faces of this triangulation.
         *
         * The given range of iterators, when dereferenced, should contain
         * exactly the <i>subdim</i>-faces of this triangulation, though
         * possibly in a different order.
         *
         * As a result of calling this routine, the <i>subdim</i>-faces
         * will be reindexed (in particular, Face<dim, subdim>::index()
         * will now return a different value).
         *
         * \pre The skeleton of this triangulation has been computed.
         */
        template <int subdim, typename Iterator>
        void reorderFaces(Iterator begin, Iterator end);

        /**
         * Relabels the vertices of the given face.
         *
         * For each top-dimensional simplex \a s of the triangulation that
         * contains \a f, if the old mapping from vertices of \a f to vertices
         * of \a s (as returned by Simplex<dim>::faceMapping()) is given by the
         * permutation \a p, then the new mapping will become
         * `p * adjust`.
         *
         * \pre For each \a i = <i>subdim</i>+1,...,\a dim, the given
         * permutation maps \a i to itself.
         */
        template <int subdim>
        void relabelFace(Face<dim, subdim>* f, const Perm<dim + 1>& adjust);

        /**
         * Tests whether this and the given triangulation have the same
         * <i>useDim</i>-face degree sequences.
         *
         * For the purposes of this routine, degree sequences are
         * considered to be unordered.
         *
         * \pre This and the given triangulation are known to have the
         * same number of <i>useDim</i>-faces as each other.
         *
         * \param other the triangulation to compare against this.
         * \return \c true if and only if the <i>useDim</i>-face
         * degree sequences are equal.
         */
        template <int useDim>
        bool sameDegreesAt(const TriangulationBase& other) const;
        /**
         * Tests whether this and the given triangulation have the same
         * <i>k</i>-face degree sequences, for each facial dimension \a k
         * contained in the integer pack \a useDim.
         *
         * For the purposes of this routine, degree sequences are
         * considered to be unordered.
         *
         * \pre This and the given triangulation are known to have the
         * same number of <i>k</i>-faces as each other, for each facial
         * dimension \a k contained in the integer pack \a useDim.
         *
         * \param other the triangulation to compare against this.
         * \return \c true if and only if all degree sequences considered
         * are equal.
         */
        template <int... useDim>
        bool sameDegreesAt(const TriangulationBase& other,
            std::integer_sequence<int, useDim...>) const;

    protected:
        /**
         * Creates a temporary lock on the topological properties of
         * the given triangulation.  While this object exists, any
         * computed properties of the underlying _manifold_ will be
         * preserved even when the triangulation changes.  This allows
         * you to avoid recomputing expensive invariants when the
         * underlying manifold is retriangulated.
         *
         * The lock will be created by the class constructor and removed
         * by the class destructor.  That is, the lock will remain in
         * effect until the TopologyLock object goes out of scope (or is
         * otherwise destroyed).
         *
         * Multiple locks are allowed.  If multiple locks are created, then
         * computed properties of the manifold will be preserved as
         * long as any one of these locks still exists.  Multiple locks
         * do not necessarily need to be nested (i.e., the order of
         * destruction does not need to be the reverse order of construction).
         *
         * Regina is currently only able to handle 255 distinct locks on
         * the same triangulation at a time.  This should be enormously more
         * than enough (since external users cannot construct TopologyLock
         * objects, and Regina's own code should not be recursing deeply
         * inside TopologyLock scopes).  However, even if there are somehow
         * more than 255 locks, the worst that will happen is some CPU wastage:
         * some properties may be cleared and need to be recomputed when this
         * was not mathematically necessary.
         *
         * TopologyLock objects are not copyable, movable or swappable.
         * In particular, Regina does not offer any way for a TopologyLock
         * to transfer its destructor's responsibilities (i.e., "unlocking"
         * the topological properties of the triangulation) to another object.
         *
         * \note If you are creating a ChangeEventSpan before retriangulating
         * the manifold and you wish to use a TopologyLock, then you should
         * create the TopologyLock _before_ the ChangeEventSpan (since the
         * ChangeEventSpan calls clearAllProperties() in its destructor,
         * and you need your topology lock to still exist at that point).
         */
        class TopologyLock {
            private:
                TriangulationBase<dim>& tri_;
                    /**< The triangulation whose topological properties
                         are locked. */

            public:
                /**
                 * Creates a new lock on the given triangulation.
                 *
                 * \param tri the triangulation whose topological
                 * properties are to be locked.  This may be \c null
                 * (in which case the lock has no effect).
                 */
                TopologyLock(TriangulationBase<dim>& tri);
                /**
                 * Removes this lock on the associated triangulation.
                 */
                ~TopologyLock();

                // Make this class non-copyable.
                TopologyLock(const TopologyLock&) = delete;
                TopologyLock& operator = (const TopologyLock&) = delete;
        };

    template <int> friend class BoundaryComponentBase;
    friend class regina::XMLTriangulationReader<dim>;
    friend class regina::XMLWriter<Triangulation<dim>>;
};

} // namespace regina::detail -> namespace regina

/**
 * Swaps the contents of the two given triangulations.
 *
 * This global routine simply calls Triangulation<dim>::swap(); it is
 * provided so that Triangulation<dim> meets the C++ Swappable requirements.
 *
 * See Triangulation<dim>::swap() for more details.
 *
 * \note This swap function is _not_ marked \c noexcept, since it
 * fires packet change events which may in turn call arbitrary
 * code via any registered packet listeners.
 *
 * \param lhs the triangulation whose contents should be swapped with \a rhs.
 * \param rhs the triangulation whose contents should be swapped with \a lhs.
 *
 * \ingroup detail
 */
template <int dim>
void swap(Triangulation<dim>& lhs, Triangulation<dim>& rhs) {
    lhs.swap(rhs);
}

namespace detail {

// Inline functions for TriangulationBase

template <int dim>
inline TriangulationBase<dim>::TriangulationBase() :
        topologyLock_(0), calculatedSkeleton_(false) {
}

template <int dim>
inline TriangulationBase<dim>::TriangulationBase(
        const TriangulationBase<dim>& src) : TriangulationBase(src, true) {
}

template <int dim>
TriangulationBase<dim>::TriangulationBase(const TriangulationBase<dim>& src,
        bool cloneProps) :
        Snapshottable<Triangulation<dim>>(src),
        topologyLock_(0),
        calculatedSkeleton_(false) {
    // We don't fire a change event here since this is a constructor.
    // There should be nobody listening on events yet.
    // Likewise, we don't clearAllProperties() since no properties
    // will have been computed yet.

    simplices_.reserve(src.simplices_.size());

    for (auto s : src.simplices_)
        simplices_.push_back(new Simplex<dim>(s->description(),
            static_cast<Triangulation<dim>*>(this)));

    // Copy the internal simplex data, including gluings.
    int f;
    auto me = simplices_.begin();
    auto you = src.simplices_.begin();
    for ( ; me != simplices_.end(); ++me, ++you) {
        for (f = 0; f <= dim; ++f) {
            if ((*you)->adj_[f]) {
                (*me)->adj_[f] = simplices_[(*you)->adj_[f]->index()];
                (*me)->gluing_[f] = (*you)->gluing_[f];
            } else
                (*me)->adj_[f] = nullptr;
        }
    }

    // Clone the skeleton:
    if (src.calculatedSkeleton_)
        static_cast<Triangulation<dim>*>(this)->cloneSkeleton(
            static_cast<const Triangulation<dim>&>(src));

    // Clone properties:
    if (cloneProps) {
        fundGroup_ = src.fundGroup_;
        H1_ = src.H1_;
    }
}

template <int dim>
TriangulationBase<dim>::TriangulationBase(TriangulationBase<dim>&& src)
        noexcept :
        Snapshottable<Triangulation<dim>>(std::move(src)),
        nBoundaryFaces_(std::move(src.nBoundaryFaces_)),
        valid_(src.valid_),
        topologyLock_(0), // locks cannot move between objects
        calculatedSkeleton_(src.calculatedSkeleton_),
        orientable_(src.orientable_),
        fundGroup_(std::move(src.fundGroup_)),
        H1_(std::move(src.H1_)) {
    // For our simplices and skeletal components, we use swaps instead
    // of moves because we need to ensure that src's lists finish empty.
    // Otherwise src will try to destroy the objects that they contain.
    simplices_.swap(src.simplices_);
    faces_.swap(src.faces_);
    components_.swap(src.components_);
    boundaryComponents_.swap(src.boundaryComponents_);

    // Simplices store their triangulations, and so this information needs
    // to be updated.  Alas this means that moves are not constant time.
    for (Simplex<dim>* s : simplices_)
        s->tri_ = static_cast<Triangulation<dim>*>(this);
}

template <int dim>
TriangulationBase<dim>& TriangulationBase<dim>::operator =
        (const TriangulationBase<dim>& src) {
    if (std::addressof(src) == this)
        return *this;

    Snapshottable<Triangulation<dim>>::operator =(src);

    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    for (auto s : simplices_)
        delete s;
    simplices_.clear();

    // We do not call Triangulation<dim>::clearAllProperties(), since we
    // assume the derived class Triangulation<dim> will manage properties
    // as it sees best.
    // Instead we just clear the properties that we manage (which includes
    // destroying all skeletal objects and setting calculatedSkeleton_ to
    // false).
    clearBaseProperties();

    simplices_.reserve(src.simplices_.size());

    for (auto s : src.simplices_)
        simplices_.push_back(new Simplex<dim>(s->description(),
            static_cast<Triangulation<dim>*>(this)));

    // Copy the internal simplex data, including gluings.
    int f;
    auto me = simplices_.begin();
    auto you = src.simplices_.begin();
    for ( ; me != simplices_.end(); ++me, ++you) {
        for (f = 0; f <= dim; ++f) {
            if ((*you)->adj_[f]) {
                (*me)->adj_[f] = simplices_[(*you)->adj_[f]->index()];
                (*me)->gluing_[f] = (*you)->gluing_[f];
            } else
                (*me)->adj_[f] = nullptr;
        }
    }

    // Do not touch topologyLock_, since other objects are managing this.

    // Clone the skeleton:
    if (src.calculatedSkeleton_)
        static_cast<Triangulation<dim>*>(this)->cloneSkeleton(
            static_cast<const Triangulation<dim>&>(src));

    // Clone properties:
    fundGroup_ = src.fundGroup_;
    H1_ = src.H1_;

    return *this;
}

template <int dim>
TriangulationBase<dim>& TriangulationBase<dim>::operator =
        (TriangulationBase<dim>&& src) {
    Snapshottable<Triangulation<dim>>::operator =(std::move(src));

    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    // We have already moved out of src, but this was in fact correct use
    // of the snapshotting machinery.
    // NOLINTNEXTLINE(bugprone-use-after-move)
    simplices_.swap(src.simplices_);
    faces_.swap(src.faces_);
    components_.swap(src.components_);
    boundaryComponents_.swap(src.boundaryComponents_);

    // Simplices store their triangulations, and so this information needs
    // to be updated.  Alas this means that moves are not constant time.
    for (Simplex<dim>* s : simplices_)
        s->tri_ = static_cast<Triangulation<dim>*>(this);

    // Do not touch topologyLock_, since other objects are managing this.

    nBoundaryFaces_.swap(src.nBoundaryFaces_);
    valid_ = src.valid_;
    calculatedSkeleton_ = src.calculatedSkeleton_;
    orientable_ = src.orientable_;

    fundGroup_ = std::move(src.fundGroup_);
    H1_ = std::move(src.H1_);

    // Let src dispose of the original simplices and skeletal objects in
    // its own destructor.
    return *this;
}

template <int dim>
inline TriangulationBase<dim>::~TriangulationBase() {
    for (auto s : simplices_)
        delete s;
}

template <int dim>
inline size_t TriangulationBase<dim>::size() const {
    return simplices_.size();
}

template <int dim>
inline auto TriangulationBase<dim>::simplices() const {
    return ListView(simplices_);
}

template <int dim>
inline Simplex<dim>* TriangulationBase<dim>::simplex(size_t index) {
    return simplices_[index];
}

template <int dim>
inline const Simplex<dim>* TriangulationBase<dim>::simplex(size_t index) const {
    return simplices_[index];
}

template <int dim>
Simplex<dim>* TriangulationBase<dim>::newSimplex() {
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    auto* s = new Simplex<dim>(static_cast<Triangulation<dim>*>(this));
    simplices_.push_back(s);
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    return s;
}

template <int dim>
template <int k>
std::array<Simplex<dim>*, k> TriangulationBase<dim>::newSimplices() {
    static_assert(k >= 0,
        "The template argument k to newSimplices() must be non-negative.");

    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    std::array<Simplex<dim>*, k> ans;
    for (int i = 0; i < k; ++i)
        simplices_.push_back(ans[i] = new Simplex<dim>(
            static_cast<Triangulation<dim>*>(this)));

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    return ans;
}

template <int dim>
void TriangulationBase<dim>::newSimplices(size_t k) {
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    for (size_t i = 0; i < k; ++i)
        simplices_.push_back(new Simplex<dim>(
            static_cast<Triangulation<dim>*>(this)));

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
Simplex<dim>* TriangulationBase<dim>::newSimplex(const std::string& desc) {
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    auto* s = new Simplex<dim>(desc, static_cast<Triangulation<dim>*>(this));
    simplices_.push_back(s);
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    return s;
}

template <int dim>
inline void TriangulationBase<dim>::removeSimplex(Simplex<dim>* simplex) {
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    simplex->isolate();
    simplices_.erase(simplices_.begin() + simplex->index());
    delete simplex;

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
inline void TriangulationBase<dim>::removeSimplexAt(size_t index) {
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    Simplex<dim>* simplex = simplices_[index];
    simplex->isolate();
    simplices_.erase(simplices_.begin() + index);
    delete simplex;

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
inline void TriangulationBase<dim>::removeAllSimplices() {
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    for (auto s : simplices_)
        delete s;
    simplices_.clear();

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::moveContentsTo(Triangulation<dim>& dest) {
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    dest.Snapshottable<Triangulation<dim>>::takeSnapshot();

    ChangeEventSpan span1(static_cast<Triangulation<dim>&>(*this));
    ChangeEventSpan span2(dest);

    for (auto* s : simplices_) {
        // This is an abuse of MarkedVector, since for a brief moment
        // each triangle belongs to both vectors simplices_ and dest.simplices_.
        // However, the subsequent clear() operation does not touch the
        // markings (indices), and so we end up with the correct result
        // (i.e., the markings are correct for dest).
        s->tri_ = &dest;
        dest.simplices_.push_back(s);
    }
    simplices_.clear();

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    static_cast<Triangulation<dim>&>(dest).clearAllProperties();
}

template <int dim>
inline size_t TriangulationBase<dim>::countComponents() const {
    ensureSkeleton();
    return components_.size();
}

template <int dim>
inline size_t TriangulationBase<dim>::countBoundaryComponents() const {
    ensureSkeleton();
    return boundaryComponents_.size();
}

template <int dim>
template <int subdim>
inline size_t TriangulationBase<dim>::countFaces() const {
    if constexpr (subdim == dim)
        return size();
    else {
        ensureSkeleton();
        return std::get<subdim>(faces_).size();
    }
}

template <int dim>
inline size_t TriangulationBase<dim>::countFaces(int subdim) const {
    if (subdim == dim)
        return size();
    if (subdim < 0 || subdim > dim)
        throw InvalidArgument("countFaces(): unsupported face dimension");

    return select_constexpr<0, dim, size_t>(subdim, [this](auto k) {
        return countFaces<k>();
    });
}

template <int dim>
inline size_t TriangulationBase<dim>::countVertices() const {
    ensureSkeleton();
    return std::get<0>(faces_).size();
}

template <int dim>
inline size_t TriangulationBase<dim>::countEdges() const {
    ensureSkeleton();
    return std::get<1>(faces_).size();
}

template <int dim>
inline size_t TriangulationBase<dim>::countTriangles() const {
    if constexpr (dim == 2) {
        return simplices_.size();
    } else {
        ensureSkeleton();
        return std::get<2>(faces_).size();
    }
}

template <int dim>
inline size_t TriangulationBase<dim>::countTetrahedra() const {
    static_assert(dim >= 3, "countTetrahedra() is only available "
        "for triangulations of dimension >= 3.");
    if constexpr (dim == 3) {
        return simplices_.size();
    } else {
        ensureSkeleton();
        return std::get<3>(faces_).size();
    }
}

template <int dim>
inline size_t TriangulationBase<dim>::countPentachora() const {
    static_assert(dim >= 4, "countPentachora() is only available "
        "for triangulations of dimension >= 4.");
    if constexpr (dim == 4) {
        return simplices_.size();
    } else {
        ensureSkeleton();
        return std::get<4>(faces_).size();
    }
}

template <int dim>
inline std::vector<size_t> TriangulationBase<dim>::fVector() const {
    ensureSkeleton();
    return std::apply([this](auto&&... kFaces) {
        return std::vector<size_t>{ kFaces.size()..., size() };
    }, faces_);
}

template <int dim>
inline auto TriangulationBase<dim>::components() const {
    ensureSkeleton();
    return ListView(components_);
}

template <int dim>
inline auto TriangulationBase<dim>::boundaryComponents() const {
    ensureSkeleton();
    return ListView(boundaryComponents_);
}

template <int dim>
template <int subdim>
inline auto TriangulationBase<dim>::faces() const {
    ensureSkeleton();
    return ListView(std::get<subdim>(faces_));
}

template <int dim>
inline auto TriangulationBase<dim>::faces(int subdim) const {
    if (subdim < 0 || subdim >= dim)
        throw InvalidArgument("faces(): unsupported face dimension");

    return select_constexpr_as_variant<0, dim>(subdim, [this](auto k) {
        return faces<k>();
    });
}

template <int dim>
inline auto TriangulationBase<dim>::vertices() const {
    ensureSkeleton();
    return ListView(std::get<0>(faces_));
}

template <int dim>
inline auto TriangulationBase<dim>::edges() const {
    ensureSkeleton();
    return ListView(std::get<1>(faces_));
}

template <int dim>
inline auto TriangulationBase<dim>::triangles() const {
    if constexpr (dim == 2) {
        return ListView(simplices_);
    } else {
        ensureSkeleton();
        return ListView(std::get<2>(faces_));
    }
}

template <int dim>
inline auto TriangulationBase<dim>::tetrahedra() const {
    static_assert(dim >= 3, "tetrahedra() is only available "
        "for triangulations of dimension >= 3.");
    if constexpr (dim == 3) {
        return ListView(simplices_);
    } else {
        ensureSkeleton();
        return ListView(std::get<3>(faces_));
    }
}

template <int dim>
inline auto TriangulationBase<dim>::pentachora() const {
    static_assert(dim >= 4, "pentachora() is only available "
        "for triangulations of dimension >= 4.");
    if constexpr (dim == 4) {
        return ListView(simplices_);
    } else {
        ensureSkeleton();
        return ListView(std::get<4>(faces_));
    }
}

template <int dim>
template <int subdim, typename Iterator>
inline void TriangulationBase<dim>::reorderFaces(Iterator begin, Iterator end) {
    std::get<subdim>(faces_).refill(begin, end);
}

template <int dim>
template <int subdim>
inline void TriangulationBase<dim>::relabelFace(Face<dim, subdim>* f,
        const Perm<dim + 1>& adjust) {
    for (const auto& emb : *f)
        std::get<subdim>(emb.simplex()->mappings_)[emb.face()] =
            emb.vertices() * adjust;
}

template <int dim>
inline Component<dim>* TriangulationBase<dim>::component(size_t index) const {
    ensureSkeleton();
    return components_[index];
}

template <int dim>
inline BoundaryComponent<dim>* TriangulationBase<dim>::boundaryComponent(
        size_t index) const {
    ensureSkeleton();
    return boundaryComponents_[index];
}

template <int dim>
template <int subdim>
inline Face<dim, subdim>* TriangulationBase<dim>::face(size_t index) const {
    ensureSkeleton();
    return std::get<subdim>(faces_)[index];
}

template <int dim>
inline auto TriangulationBase<dim>::face(int subdim, size_t index) const {
    if (subdim < 0 || subdim >= dim)
        throw InvalidArgument("face(): unsupported face dimension");

    return select_constexpr_as_variant<0, dim>(subdim, [this, index](auto k) {
        return face<k>(index);
    });
}

template <int dim>
inline Face<dim, 0>* TriangulationBase<dim>::vertex(size_t index) const {
    ensureSkeleton();
    return std::get<0>(faces_)[index];
}

template <int dim>
inline Face<dim, 1>* TriangulationBase<dim>::edge(size_t index) const {
    ensureSkeleton();
    return std::get<1>(faces_)[index];
}

template <int dim>
inline Face<dim, 2>* TriangulationBase<dim>::triangle(size_t index) {
    if constexpr (dim == 2) {
        return simplices_[index];
    } else {
        ensureSkeleton();
        return std::get<2>(faces_)[index];
    }
}

template <int dim>
inline auto TriangulationBase<dim>::triangle(size_t index) const {
    if constexpr (dim == 2) {
        return simplices_[index];
    } else {
        ensureSkeleton();
        return std::get<2>(faces_)[index];
    }
}

template <int dim>
inline Face<dim, 3>* TriangulationBase<dim>::tetrahedron(size_t index) {
    static_assert(dim >= 3, "tetrahedron() is only available "
        "for triangulations of dimension >= 3.");
    if constexpr (dim == 3) {
        return simplices_[index];
    } else {
        ensureSkeleton();
        return std::get<3>(faces_)[index];
    }
}

template <int dim>
inline auto TriangulationBase<dim>::tetrahedron(size_t index) const {
    static_assert(dim >= 3, "tetrahedron() is only available "
        "for triangulations of dimension >= 3.");
    if constexpr (dim == 3) {
        return simplices_[index];
    } else {
        ensureSkeleton();
        return std::get<3>(faces_)[index];
    }
}

template <int dim>
inline Face<dim, 4>* TriangulationBase<dim>::pentachoron(size_t index) {
    static_assert(dim >= 4, "pentachoron() is only available "
        "for triangulations of dimension >= 4.");
    if constexpr (dim == 4) {
        return simplices_[index];
    } else {
        ensureSkeleton();
        return std::get<4>(faces_)[index];
    }
}

template <int dim>
inline auto TriangulationBase<dim>::pentachoron(size_t index) const {
    static_assert(dim >= 4, "pentachoron() is only available "
        "for triangulations of dimension >= 4.");
    if constexpr (dim == 4) {
        return simplices_[index];
    } else {
        ensureSkeleton();
        return std::get<4>(faces_)[index];
    }
}

template <int dim>
template <int subdim>
inline Face<dim, subdim>* TriangulationBase<dim>::translate(
        const Face<dim, subdim>* other) const {
    static_assert(0 <= subdim && subdim < dim, "translate() requires a "
        "facial dimension between 0 and dim-1 inclusive.");

    if (other) {
        const auto& emb = other->front();
        return simplices_[emb.simplex()->index()]->template face<subdim>(
            emb.face());
    } else
        return nullptr;
}

template <int dim>
inline FacetPairing<dim> TriangulationBase<dim>::pairing() const {
    return FacetPairing<dim>(static_cast<const Triangulation<dim>&>(*this));
}

template <int dim>
inline bool TriangulationBase<dim>::isEmpty() const {
    return simplices_.empty();
}

template <int dim>
inline bool TriangulationBase<dim>::isValid() const {
    if constexpr (dim == 2) {
        // There is nothing that can go wrong in dimension 2.
        return true;
    } else {
        ensureSkeleton();
        return valid_;
    }
}

template <int dim>
inline bool TriangulationBase<dim>::hasBoundaryFacets() const {
    ensureSkeleton();
    return nBoundaryFaces_[dim - 1] > 0;
}

template <int dim>
inline size_t TriangulationBase<dim>::countBoundaryFacets() const {
    ensureSkeleton();
    return nBoundaryFaces_[dim - 1];
}

template <int dim>
template <int subdim>
inline size_t TriangulationBase<dim>::countBoundaryFaces() const {
    static_assert(subdim >= 0 && subdim < dim,
        "countBoundaryFaces() requires 0 <= subdim < dim.");
    ensureSkeleton();
    return nBoundaryFaces_[subdim];
}

template <int dim>
inline size_t TriangulationBase<dim>::countBoundaryFaces(int subdim) const {
    if (subdim < 0 || subdim >= dim)
        throw InvalidArgument(
            "countBoundaryFaces(): unsupported face dimension");

    return select_constexpr<0, dim, size_t>(subdim, [this](auto k) {
        return countBoundaryFaces<k>();
    });
}

template <int dim>
inline bool TriangulationBase<dim>::isOrientable() const {
    ensureSkeleton();
    return orientable_;
}

template <int dim>
inline bool TriangulationBase<dim>::isConnected() const {
    ensureSkeleton();
    return (components_.size() <= 1);
}

template <int dim>
bool TriangulationBase<dim>::operator == (const Triangulation<dim>& other)
        const {
    if (simplices_.size() != other.simplices_.size())
        return false;

    int f;
    auto me = simplices_.begin();
    auto you = other.simplices_.begin();
    for ( ; me != simplices_.end(); ++me, ++you) {
        for (f = 0; f <= dim; ++f) {
            if ((*you)->adj_[f]) {
                if ((*me)->adj_[f] != simplices_[(*you)->adj_[f]->index()])
                    return false;
                if ((*me)->gluing_[f] != (*you)->gluing_[f])
                    return false;
            } else {
                if ((*me)->adj_[f])
                    return false;
            }
        }
    }

    return true;
}

template <int dim>
inline bool TriangulationBase<dim>::operator != (
        const Triangulation<dim>& other) const {
    return ! ((*this) == other);
}

template <int dim>
inline std::optional<Isomorphism<dim>> TriangulationBase<dim>::isIsomorphicTo(
        const Triangulation<dim>& other) const {
    std::optional<Isomorphism<dim>> ans;
    findIsomorphisms(other, true, [&ans](const Isomorphism<dim>& iso) {
        ans = iso;
        return true; // stop searching
    });
    return ans;
}

template <int dim>
inline std::optional<Isomorphism<dim>> TriangulationBase<dim>::isContainedIn(
        const Triangulation<dim>& other) const {
    std::optional<Isomorphism<dim>> ans;
    findIsomorphisms(other, false, [&ans](const Isomorphism<dim>& iso) {
        ans = iso;
        return true; // stop searching
    });
    return ans;
}

template <int dim>
template <typename Action, typename... Args>
inline bool TriangulationBase<dim>::findAllIsomorphisms(
        const Triangulation<dim>& other, Action&& action, Args&&... args)
        const {
    return findIsomorphisms(other, true, std::forward<Action>(action),
        std::forward<Args>(args)...);
}

template <int dim>
template <typename Action, typename... Args>
inline bool TriangulationBase<dim>::findAllSubcomplexesIn(
        const Triangulation<dim>& other, Action&& action, Args&&... args)
        const {
    return findIsomorphisms(other, false, std::forward<Action>(action),
        std::forward<Args>(args)...);
}

template <int dim>
void TriangulationBase<dim>::insertTriangulation(
        const Triangulation<dim>& source) {
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    size_t nOrig = size();
    size_t nSource = source.size();

    // To ensure that things work even if source is this triangulation:
    // - we only make nSource iterations through each loop;
    // - we avoid using iterators over source, which could be invalidated.

    size_t i;
    for (i = 0; i < nSource; ++i)
        simplices_.push_back(new Simplex<dim>(
            source.simplices_[i]->description_,
            static_cast<Triangulation<dim>*>(this)));

    Simplex<dim> *me, *you;
    int f;
    for (i = 0; i < nSource; ++i) {
        me = simplices_[nOrig + i];
        you = source.simplices_[i];
        for (f = 0; f <= dim; ++f) {
            if (you->adj_[f]) {
                me->adj_[f] = simplices_[nOrig + you->adj_[f]->index()];
                me->gluing_[f] = you->gluing_[f];
            } else
                me->adj_[f] = nullptr;
        }
    }

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::tightEncode(std::ostream& out) const {
    regina::detail::tightEncodeIndex(out, size());

    // For dimension 2, we *could* squeeze two permutations into a single
    // character.  However, this requires care: if we write the two
    // permutations at the point where we see the first then this requires
    // read-forward in the encoding routine, and if we write them at the point
    // where we see the second then this makes decoding difficult (since we
    // don't know in advance which simplex facets were skipped over in the
    // encoding).  Since dimension 2 is not where our real space problems
    // occur, we leave the encoding as is for now.

    // Write each gluing from one side only, in the forward direction.
    for (auto* s : simplices_)
        for (int i = 0; i <= dim; ++i)
            if (auto* adj = s->adjacentSimplex(i)) {
                // If we have already seen this gluing from the other side,
                // skip over it completely.
                if (adj->index() > s->index() ||
                        (adj->index() == s->index() &&
                        s->adjacentFacet(i) >= i)) {
                    regina::detail::tightEncodeIndex(out, adj->index());
                    // TODO: For dimension 2, we can combine pairs of
                    // gluings in a single character.
                    s->adjacentGluing(i).tightEncode(out);
                }
            } else
                regina::detail::tightEncodeNoIndex(out);
}

template <int dim>
Triangulation<dim> TriangulationBase<dim>::tightDecode(std::istream& input) {
    auto size = regina::detail::tightDecodeIndex<size_t>(input);

    Triangulation<dim> ans;
    // Note: new simplices are initialised with all adj_[i] null.
    for (size_t i = 0; i < size; ++i)
        ans.simplices_.push_back(new Simplex<dim>(&ans));

    for (auto* s : ans.simplices_)
        for (int i = 0; i <= dim; ++i) {
            if (s->adjacentSimplex(i))
                continue;

            auto adjIdx = regina::detail::tightDecodeIndex<ssize_t>(input);
            if (adjIdx >= 0) {
                // This is a non-boundary facet.
                if (adjIdx >= static_cast<ssize_t>(size))
                    throw InvalidInput("The tight encoding contains "
                        "invalid gluings");

                Perm<dim+1> gluing = Perm<dim+1>::tightDecode(input);
                if (adjIdx < static_cast<ssize_t>(s->index()) ||
                        (adjIdx == static_cast<ssize_t>(s->index()) &&
                        gluing[i] <= i))
                    throw InvalidInput("The tight encoding contains "
                        "unexpected gluings");

                Simplex<dim>* adj = ans.simplices_[adjIdx];
                if (adj->adjacentSimplex(gluing[i])) {
                    // Some other gluing has already claimed this facet.
                    throw InvalidInput("The tight encoding contains "
                        "inconsistent gluings");
                }

                s->join(i, adj, gluing);
            }
        }

    return ans;
}

template <int dim>
template <typename Iterator>
Triangulation<dim> TriangulationBase<dim>::fromGluings(size_t size,
        Iterator beginGluings, Iterator endGluings) {
    Triangulation<dim> ans;

    // Note: new simplices are initialised with all adj_[i] null.
    for (size_t i = 0; i < size; ++i)
        ans.simplices_.push_back(new Simplex<dim>(&ans));

    for (auto it = beginGluings; it != endGluings; ++it) {
        if (std::get<0>(*it) >= size)
            throw InvalidArgument(
                "fromGluings(): source simplex out of range");
        if (std::get<2>(*it) >= size)
            throw InvalidArgument(
                "fromGluings(): adjacent simplex out of range");
        if (std::get<1>(*it) < 0 || std::get<1>(*it) > dim)
            throw InvalidArgument(
                "fromGluings(): facet number out of range");

        Simplex<dim>* s = ans.simplices_[std::get<0>(*it)];
        Simplex<dim>* adj = ans.simplices_[std::get<2>(*it)];
        int facet = std::get<1>(*it);

        if (s->adj_[facet])
            throw InvalidArgument(
                "fromGluings(): source facet already glued to something");
        if (adj->adj_[std::get<3>(*it)[facet]])
            throw InvalidArgument(
                "fromGluings(): destination facet already glued to something");
        if (s == adj && std::get<3>(*it)[facet] == facet)
            throw InvalidArgument(
                "fromGluings(): a facet cannot be glued to itself");

        s->adj_[facet] = adj;
        s->gluing_[facet] = std::get<3>(*it);

        adj->adj_[std::get<3>(*it)[facet]] = s;
        adj->gluing_[std::get<3>(*it)[facet]] = std::get<3>(*it).inverse();
    }

    return ans;
}

template <int dim>
inline Triangulation<dim> TriangulationBase<dim>::fromGluings(size_t size,
        std::initializer_list<std::tuple<size_t, int, size_t, Perm<dim+1>>>
        gluings) {
    return fromGluings(size, gluings.begin(), gluings.end());
}

template <int dim>
std::string TriangulationBase<dim>::dumpConstruction() const {
    std::ostringstream ans;

    ans << "Triangulation<" << dim << "> tri = Triangulation<" << dim
        << ">::fromGluings(" << size() << ", {\n";

    size_t wrote = 0;
    for (size_t i = 0; i < size(); ++i) {
        Simplex<dim>* s = simplices_[i];
        for (int j = 0; j <= dim; ++j) {
            Simplex<dim>* adj = s->adjacentSimplex(j);
            if (adj) {
                Perm<dim + 1> g = s->adjacentGluing(j);
                if (adj->index() > i || (adj->index() == i && g[j] > j)) {
                    if (wrote == 0)
                        ans << "    ";
                    else if (wrote % 2 == 0)
                        ans << ",\n    ";
                    else
                        ans << ", ";

                    ans << "{ " << i << ", " << j << ", " << adj->index()
                        << ", {";
                    for (int k = 0; k <= dim; ++k) {
                        if (k > 0)
                            ans << ',';
                        ans << g[k];
                    }
                    ans << "} }";

                    ++wrote;
                }
            }
        }
    }
    ans << "});\n";
    return ans.str();
}

template <int dim>
void TriangulationBase<dim>::writeTextShort(std::ostream& out) const {
    if (isEmpty()) {
        out << "Empty " << dim << "-D triangulation";
        return;
    }

    if (! isValid())
        out << "Invalid ";
    else if constexpr (dim == 2) {
        if (hasBoundaryFacets())
            out << "Bounded ";
        else
            out << "Closed ";
    } else if constexpr (standardDim(dim)) {
        if (static_cast<const Triangulation<dim>*>(this)->isClosed())
            out << "Closed ";
        else if (static_cast<const Triangulation<dim>*>(this)->isIdeal()) {
            if (hasBoundaryFacets())
                out << "Ideal/bounded ";
            else
                out << "Ideal ";
        } else
            out << "Bounded ";
    } else {
        if (hasBoundaryFacets())
            out << "Bounded ";
        else
            out << "Possibly closed ";
    }

    if (isOrientable())
        out << "orientable ";
    else
        out << "non-orientable ";

    out << dim << "-D triangulation, f = (";
    for (auto f : fVector())
        out << ' ' << f;
    out << " )";
}

template <int dim>
void TriangulationBase<dim>::writeTextLong(std::ostream& out) const {
    ensureSkeleton();

    if constexpr (dim > 4) {
        writeTextShort(out);
        out << "\n\n";
    } else {
        out << "Size of the skeleton:\n";
        if constexpr (dim >= 4)
            out << "  Pentachora: " << countFaces<4>() << '\n';
        if constexpr (dim >= 3)
            out << "  Tetrahedra: " << countFaces<3>() << '\n';
        out << "  Triangles: " << countFaces<2>() << '\n';
        out << "  Edges: " << countFaces<1>() << '\n';
        out << "  Vertices: " << countFaces<0>() << '\n';
        out << '\n';
    }

    Simplex<dim>* simp;
    Simplex<dim>* adj;
    size_t pos;
    int j;
    Perm<dim+1> gluing;

    if constexpr (dim == 2)
        out << "Triangle gluing:\n  Triangle  |  glued to:";
    else if constexpr (dim == 3)
        out << "Tetrahedron gluing:\n  Tet  |  glued to:";
    else if constexpr (dim == 4)
        out << "Pentachoron gluing:\n  Pent  |  glued to:";
    else
        out << "  Simplex  |  glued to:";
    for (int i = dim; i >= 0; --i) {
        if constexpr (dim == 3)
            out << "      (";
        else
            out << "     (";
        for (j = 0; j <= dim; ++j)
            if (j != i)
                out << regina::digit(j);
        out << ')';
    }
    out << '\n';
    if constexpr (dim == 2)
        out << "  ----------+-----------";
    else if constexpr (dim == 3)
        out << "  -----+-----------";
    else if constexpr (dim == 4)
        out << "  ------+-----------";
    else
        out << "  ---------+-----------";
    for (int i = dim; i >= 0; --i)
        for (j = 0; j < (dim == 3 ? 11 : 7 + dim); ++j)
            out << '-';
    out << '\n';
    for (pos=0; pos < simplices_.size(); pos++) {
        simp = simplices_[pos];
        if constexpr (dim == 2)
            out << "      ";
        else if constexpr (dim == 3)
            out << ' ';
        else if constexpr (dim == 4)
            out << "  ";
        else
            out << "     ";
        out << std::setw(4) << pos << "  |           ";
        for (int i = dim; i >= 0; --i) {
            adj = simp->adjacentSimplex(i);
            if (! adj) {
                for (j = 0; j < (dim == 3 ? 3 : dim - 1); ++j)
                    out << ' ';
                out << "boundary";
            } else {
                gluing = simp->adjacentGluing(i);
                out << std::setw(dim == 3 ? 5 : 4) << adj->index() << " (";
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

    // For Regina's standard dimensions, write skeletal details also.

    if constexpr (dim <= 4) {
        out << "Vertices:\n";
        if constexpr (dim == 2)
            out << "  Triangle  |  vertex: ";
        else if constexpr (dim == 3)
            out << "  Tet  |  vertex: ";
        else if constexpr (dim == 4)
            out << "  Pent  |  vertex: ";
        for (int i = 0; i <= dim; ++i)
            out << "   " << i;
        out << '\n';
        if constexpr (dim == 2)
            out << "  ----------+----------";
        else if constexpr (dim == 3)
            out << "  -----+----------";
        else if constexpr (dim == 4)
            out << "  ------+----------";
        for (int i = 0; i <= dim; ++i)
            out << "----";
        out << '\n';
        for (size_t triPos = 0; triPos < simplices_.size(); ++triPos) {
            const Simplex<dim>* tri = simplices_[triPos];
            if constexpr (dim == 2)
                out << "      " << std::setw(4) << triPos << "  |          ";
            else if constexpr (dim == 3)
                out << " " << std::setw(4) << triPos << "  |          ";
            else if constexpr (dim == 4)
                out << "  " << std::setw(4) << triPos << "  |          ";
            for (int i = 0; i <= dim; ++i)
                out << ' ' << std::setw(3) << tri->vertex(i)->index();
            out << '\n';
        }
        out << '\n';

        out << "Edges:\n";
        if constexpr (dim == 2)
            out << "  Triangle  |  edge: ";
        else if constexpr (dim == 3)
            out << "  Tet  |  edge: ";
        else if constexpr (dim == 4)
            out << "  Pent  |  edge: ";
        for (int i1 = 0; i1 < dim; ++i1)
            for (int i2 = i1 + 1; i2 <= dim; ++i2)
                out << "  " << i1 << i2;
        out << '\n';
        if constexpr (dim == 2)
            out << "  ----------+--------";
        else if constexpr (dim == 3)
            out << "  -----+--------";
        else if constexpr (dim == 4)
            out << "  ------+--------";
        for (int i = 0; i < ((dim * (dim + 1)) / 2); ++i)
            out << "----";
        out << '\n';
        for (size_t triPos = 0; triPos < simplices_.size(); ++triPos) {
            const Simplex<dim>* tri = simplices_[triPos];
            if constexpr (dim == 2)
                out << "      " << std::setw(4) << triPos << "  |        ";
            else if constexpr (dim == 3)
                out << " " << std::setw(4) << triPos << "  |        ";
            else if constexpr (dim == 4)
                out << "  " << std::setw(4) << triPos << "  |        ";
            // Forward lexicographic numbering kicks in at dimension 3.
            if constexpr (dim == 2) {
                for (int i = 2; i >= 0; --i)
                    out << ' ' << std::setw(3) << tri->edge(i)->index();
            } else {
                for (int i = 0; i < ((dim * (dim + 1)) / 2); ++i)
                    out << ' ' << std::setw(3) << tri->edge(i)->index();
            }
            out << '\n';
        }
        out << '\n';

        if constexpr (dim >= 3) {
            out << "Triangles:\n";
            if constexpr (dim == 3)
                out << "  Tet  |  face: ";
            else
                out << "  Pent  |  triangle: ";
            for (int i1 = 0; i1 < dim - 1; ++i1)
                for (int i2 = i1 + 1; i2 < dim; ++i2)
                    for (int i3 = i2 + 1; i3 <= dim; ++i3)
                        out << ' ' << i1 << i2 << i3;
            out << '\n';
            if constexpr (dim == 3)
                out << "  -----+--------";
            else
                out << "  ------+------------";
            for (int i = 0; i < (dim == 3 ? 4 : 10); ++i)
                out << "----";
            out << '\n';
            for (size_t tetPos = 0; tetPos < simplices_.size(); ++tetPos) {
                const Simplex<dim>* tet = simplices_[tetPos];
                if constexpr (dim == 3)
                    out << "  " << std::setw(3) << tetPos << "  |        ";
                else
                    out << "  " << std::setw(4) << tetPos << "  |            ";
                // Forward lexicographic numbering kicks in at dimension 5.
                // Here we are only working with dimensions 3 and 4.
                for (int face = (dim == 3 ? 3 : 9); face >= 0; face--)
                    out << ' ' << std::setw(3) << tet->triangle(face)->index();
                out << '\n';
            }
            out << '\n';
        }

        if constexpr (dim == 4) {
            // Here the dimension is fixed, and so we can just hard-code
            // everything for dimension 4 specifically.
            out << "Tetrahedra:\n";
            out << "  Pent  |  facet:  0123 0124 0134 0234 1234\n";
            out << "  ------+----------------------------------\n";
            for (size_t pentPos = 0; pentPos < simplices_.size(); ++pentPos) {
                const Simplex<dim>* pent = simplices_[pentPos];
                out << "  " << std::setw(4) << pentPos << "  |         ";
                for (int i = 4; i >= 0; --i)
                    out << ' ' << std::setw(4) << pent->tetrahedron(i)->index();
                out << '\n';
            }
            out << '\n';
        }
    }
}

template <int dim>
inline void TriangulationBase<dim>::ensureSkeleton() const {
    if (! calculatedSkeleton_)
        const_cast<Triangulation<dim>*>(
            static_cast<const Triangulation<dim>*>(this))->calculateSkeleton();
}

template <int dim>
inline bool TriangulationBase<dim>::calculatedSkeleton() const {
    return calculatedSkeleton_;
}

template <int dim>
bool TriangulationBase<dim>::isOriented() const {
    // Calling isOrientable() will force a skeletal calculation if this
    // has not been done already.
    if (! isOrientable())
        return false;

    for (auto s : simplices_)
        if (s->orientation() != 1)
            return false;

    return true;
}

template <int dim>
inline long TriangulationBase<dim>::eulerCharTri() const {
    ensureSkeleton();
    return std::apply([this](auto&&... kFaces) {
        return (static_cast<long>(kFaces.size()) - ... - size());
    }, faces_);
}

template <int dim>
void TriangulationBase<dim>::orient() {
    ensureSkeleton();

    TopologyLock lock(*this);
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    int f;
    for (auto s : simplices_)
        if (s->orientation_ == -1 && s->component_->isOrientable()) {
            // Flip vertices (dim - 1) and dim of s.
            std::swap(s->adj_[dim - 1], s->adj_[dim]);
            std::swap(s->gluing_[dim - 1], s->gluing_[dim]);

            for (f = 0; f <= dim; ++f)
                if (s->adj_[f]) {
                    if (s->adj_[f]->orientation_ == -1) {
                        // The adjacent simplex is also being flipped.
                        // Fix the gluing from this side now, and fix it from
                        // the other side when we process the other simplex.
                        s->gluing_[f] = Perm<dim + 1>(dim - 1, dim) *
                            s->gluing_[f] * Perm<dim + 1>(dim - 1, dim);
                    } else {
                        // The adjacent simplex will be left intact.
                        // Fix the gluing from both sides now.
                        s->gluing_[f] = s->gluing_[f] *
                            Perm<dim + 1>(dim - 1, dim);
                        s->adj_[f]->gluing_[s->gluing_[f][f]] =
                            s->gluing_[f].inverse();
                    }
                }
        }

    // Don't forget to call clearAllProperties(), since we are manipulating
    // the gluing-related data members of Simplex<dim> directly.
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::reflect() {
    ensureSkeleton();

    TopologyLock lock(*this);
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    int f;
    for (auto s : simplices_) {
        // Flip vertices (dim - 1) and dim of s.
        std::swap(s->adj_[dim - 1], s->adj_[dim]);
        std::swap(s->gluing_[dim - 1], s->gluing_[dim]);

        for (f = 0; f <= dim; ++f)
            if (s->adj_[f]) {
                // Fix the gluing from this side now, and fix it from
                // the other side when we process the other simplex.
                s->gluing_[f] = Perm<dim + 1>(dim - 1, dim) *
                    s->gluing_[f] * Perm<dim + 1>(dim - 1, dim);
            }
    }

    // Don't forget to call clearAllProperties(), since we are manipulating
    // the gluing-related data members of Simplex<dim> directly.
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::makeDoubleCover() {
    size_t sheetSize = simplices_.size();
    if (sheetSize == 0)
        return;

    // Ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    // Create a second sheet of simplices.
    auto* upper = new Simplex<dim>*[sheetSize];
    size_t i;
    for (i = 0; i < sheetSize; i++)
        upper[i] = newSimplex(simplices_[i]->description());

    // Reset each simplex orientation.
    auto sit = simplices_.begin();
    for (i = 0; i < sheetSize; i++) {
        (*sit++)->orientation_ = 0;
        upper[i]->orientation_ = 0;
    }

    // Run through the upper sheet and recreate the gluings as we
    // propagate simplex orientations through components.
    //
    // We use a breadth-first search to propagate orientations.
    // The underlying queue is implemented using a plain C array - since each
    // simplex is processed only once, an array of size sheetSize is enough.
    //
    // We will ignore the requirement that the lowest-index simplex in each
    // component must have orientation +1: this is because our new orientations
    // are temporary only.  (The calls to newSimplex() above will force a full
    // recomputation of the skeleton when next required.)
    auto* queue = new size_t[sheetSize];
    size_t queueStart = 0, queueEnd = 0;

    int facet;
    size_t upperSimp;
    Simplex<dim>* lowerSimp;
    size_t upperAdj;
    Simplex<dim>* lowerAdj;
    int lowerAdjOrientation;
    Perm<dim + 1> gluing;
    for (i = 0; i < sheetSize; i++)
        if (upper[i]->orientation_ == 0) {
            // We've found a new component.
            // Completely recreate the gluings for this component.
            upper[i]->orientation_ = 1;
            simplices_[i]->orientation_ = -1;
            queue[queueEnd++] = i;

            while (queueStart < queueEnd) {
                upperSimp = queue[queueStart++];
                lowerSimp = simplices_[upperSimp];

                for (facet = 0; facet <= dim; ++facet) {
                    lowerAdj = lowerSimp->adjacentSimplex(facet);

                    // See if this simplex is glued to something in the
                    // lower sheet.
                    if (! lowerAdj)
                        continue;

                    // Make sure we haven't already fixed this gluing in
                    // the upper sheet.
                    if (upper[upperSimp]->adjacentSimplex(facet))
                        continue;

                    // Determine the expected orientation of the
                    // adjacent simplex in the lower sheet.
                    gluing = lowerSimp->adjacentGluing(facet);
                    lowerAdjOrientation = (gluing.sign() == 1 ?
                        -lowerSimp->orientation_ : lowerSimp->orientation_);

                    upperAdj = lowerAdj->index();
                    if (lowerAdj->orientation_ == 0) {
                        // We haven't seen the adjacent simplex yet.
                        lowerAdj->orientation_ = lowerAdjOrientation;
                        upper[upperAdj]->orientation_ = -lowerAdjOrientation;
                        upper[upperSimp]->join(facet, upper[upperAdj], gluing);
                        queue[queueEnd++] = upperAdj;
                    } else if (lowerAdj->orientation_ == lowerAdjOrientation) {
                        // The adjacent simplex already has the
                        // correct orientation.
                        upper[upperSimp]->join(facet, upper[upperAdj], gluing);
                    } else {
                        // The adjacent simplex already has the
                        // incorrect orientation.  Make a cross between
                        // the two sheets.
                        lowerSimp->unjoin(facet);
                        lowerSimp->join(facet, upper[upperAdj], gluing);
                        upper[upperSimp]->join(facet, lowerAdj, gluing);
                    }
                }
            }
        }

    // Tidy up.
    delete[] upper;
    delete[] queue;
}

template <int dim>
void TriangulationBase<dim>::subdivide() {
    size_t nOld = simplices_.size();
    if (nOld == 0)
        return;

    Triangulation<dim> staging;
    // Ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span(staging);

    static_assert(standardDim(dim),
        "subdivide() may only be used in standard dimensions.");

    auto* newSimp = new Simplex<dim>*[nOld * Perm<dim+1>::nPerms];

    // A top-dimensional simplex in the subdivision is uniquely defined
    // by a permutation p on (dim+1) elements.
    //
    // As described in the documentation for subdivide(),
    // this is the simplex that:
    // - meets the boundary in the facet opposite vertex p[dim];
    // - meets that facet in the (dim-2)-face opposite vertex p[dim-1];
    // - meets that (dim-2)-face in the (dim-3)-face opposite vertex p[dim-2];
    // - ...
    // - meets that edge in the vertex opposite vertex p[1];
    // - directly touches vertex p[0].

    size_t simp;
    for (simp = 0; simp < Perm<dim+1>::nPerms * nOld; ++simp)
        newSimp[simp] = staging.newSimplex();

    // Do all of the internal gluings
    typename Perm<dim+1>::Index permIdx, adjIdx;
    Perm<dim+1> perm, glue;
    int i;
    for (simp=0; simp < nOld; ++simp)
        for (permIdx = 0; permIdx < Perm<dim+1>::nPerms; ++permIdx) {
            perm = Perm<dim+1>::orderedSn[permIdx];

            // Internal gluings within the old simplex:
            for (i = 0; i < dim; ++i) {
                adjIdx = (perm * Perm<dim+1>(i, i+1)).orderedSnIndex();
                if (permIdx < adjIdx)
                    newSimp[Perm<dim+1>::nPerms * simp + permIdx]->join(perm[i],
                        newSimp[Perm<dim+1>::nPerms * simp + adjIdx],
                        Perm<dim+1>(perm[i], perm[i+1]));
            }

            // Adjacent gluings to the adjacent simplex:
            Simplex<dim>* oldSimp = simplex(simp);
            if (! oldSimp->adjacentSimplex(perm[dim]))
                continue; // This hits a boundary facet.
            if (newSimp[Perm<dim+1>::nPerms * simp + permIdx]->adjacentSimplex(
                    perm[dim]))
                continue; // We've already done this gluing from the other side.

            glue = oldSimp->adjacentGluing(perm[dim]);
            newSimp[Perm<dim+1>::nPerms * simp + permIdx]->join(perm[dim],
                newSimp[Perm<dim+1>::nPerms * oldSimp->adjacentSimplex(
                    perm[dim])->index() + (glue * perm).orderedSnIndex()],
                glue);
        }

    // Delete the existing simplices and put in the new ones.
    // TODO: If the skeleton has been calculated and we know the
    // triangulation to be valid, then preserve vertex link properties.
    static_cast<Triangulation<dim>*>(this)->swap(staging);
    delete[] newSimp;
}

template <int dim>
inline void TriangulationBase<dim>::barycentricSubdivision() {
    subdivide();
}

template <int dim>
bool TriangulationBase<dim>::finiteToIdeal() {
    if (! hasBoundaryFacets())
        return false;

    // Make a list of all boundary facets, indexed by (dim-1)-face number,
    // and create the corresponding new simplices.
    // We put these new simplices in a new "staging" triangulation for
    // the time being, since we will still need to iterate through
    // (dim-2)-faces of the original triangulation.

    size_t nFaces = countFaces<dim - 1>();

    auto* bdry = new Simplex<dim>*[nFaces];
    auto* bdryPerm = new Perm<dim + 1>[nFaces];
    auto* cone = new Simplex<dim>*[nFaces];

    Triangulation<dim> staging;
    // Ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span1(staging);

    for (Face<dim, dim - 1>* f : faces<dim - 1>()) {
        if (f->degree() > 1) {
            // Not a boundary facet.
            bdry[f->index()] = cone[f->index()] = nullptr;
            continue;
        }

        bdry[f->index()] = f->front().simplex();
        bdryPerm[f->index()] = f->front().vertices();
        cone[f->index()] = staging.newSimplex();
    }

    // Glue the new simplices to each other.
    Face<dim, dim - 1> *facet1, *facet2;
    Perm<dim + 1> f1Perm, f2Perm;
    for (auto ridge : faces<dim - 2>()) {
        // Is this (dim-2)-face on a real boundary component?
        // Look for the boundary facets at either end.
        const FaceEmbedding<dim, dim - 2>& e1 = ridge->front();
        facet1 = e1.simplex()->template face<dim - 1>(e1.vertices()[dim]);
        if (facet1->degree() > 1)
            continue;

        // Yes!  We're on a real boundary component.
        const FaceEmbedding<dim, dim - 2>& e2 = ridge->back();
        facet2 = e2.simplex()->template face<dim - 1>(e2.vertices()[dim - 1]);

        f1Perm = bdryPerm[facet1->index()].inverse() * e1.vertices();
        f2Perm = bdryPerm[facet2->index()].inverse() * e2.vertices() *
            Perm<dim + 1>(dim - 1, dim);

        cone[facet1->index()]->join(f1Perm[dim - 1],
            cone[facet2->index()], f2Perm * f1Perm.inverse());
    }

    // Now join the new simplices to the boundary facets of the original
    // triangulation.
    // Again, ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span2(static_cast<Triangulation<dim>&>(*this));

    staging.moveContentsTo(static_cast<Triangulation<dim>&>(*this));

    for (size_t i = 0; i < nFaces; ++i)
        if (cone[i])
            cone[i]->join(dim, bdry[i], bdryPerm[i]);

    // Clean up and return.
    delete[] cone;
    delete[] bdryPerm;
    delete[] bdry;
    return true;
}

template <int dim>
std::vector<Triangulation<dim>>
        TriangulationBase<dim>::triangulateComponents() const {
    // Knock off the empty triangulation first.
    if (simplices_.empty())
        return { };

    // Create the new component triangulations.
    // Note that the following line forces a skeletal recalculation.
    size_t nComp = countComponents();

    // Initialise the component triangulations.
    std::vector<Triangulation<dim>> ans(nComp);

    // Clone the simplices, sorting them into the new components.
    auto* newSimp = new Simplex<dim>*[size()];
    Simplex<dim> *simp, *adj;
    size_t simpPos, adjPos;
    Perm<dim + 1> adjPerm;
    int facet;

    for (simpPos = 0; simpPos < size(); ++simpPos)
        newSimp[simpPos] = ans[simplices_[simpPos]->component()->index()].
            newSimplex(simplices_[simpPos]->description());

    // Clone the simplex gluings also.
    for (simpPos = 0; simpPos < size(); ++simpPos) {
        simp = simplices_[simpPos];
        for (facet = 0; facet <= dim; ++facet) {
            adj = simp->adjacentSimplex(facet);
            if (adj) {
                adjPos = adj->index();
                adjPerm = simp->adjacentGluing(facet);
                if (adjPos > simpPos ||
                        (adjPos == simpPos && adjPerm[facet] > facet))
                    newSimp[simpPos]->join(facet, newSimp[adjPos], adjPerm);
            }
        }
    }

    delete[] newSimp;
    return ans;
}

template <int dim>
inline const GroupPresentation& TriangulationBase<dim>::fundamentalGroup()
        const {
    return group();
}

template <int dim>
inline void TriangulationBase<dim>::simplifiedFundamentalGroup(
        GroupPresentation newGroup) {
    fundGroup_ = std::move(newGroup);
}

template <int dim>
inline AbelianGroup TriangulationBase<dim>::homology(int k) const {
    // upperBound is one more than the largest allowed k.
    constexpr int upperBound = (standardDim(dim) ? dim : (dim - 1));
    if (k < 1 || k >= upperBound)
        throw InvalidArgument("homology(): unsupported homology dimension");

    return select_constexpr<1, upperBound, AbelianGroup>(k, [this](auto k) {
        return homology<k>();
    });
}

template <int dim>
template <int k>
inline MarkedAbelianGroup TriangulationBase<dim>::markedHomology() const {
    static_assert(1 <= k && k < dim);

    if (isEmpty())
        throw FailedPrecondition("markedHomology(): triangulation is empty");
    if (! isValid())
        throw FailedPrecondition("markedHomology(): triangulation is invalid");

    return MarkedAbelianGroup(boundaryMap<k>(), boundaryMap<k + 1>());
}

template <int dim>
inline MarkedAbelianGroup TriangulationBase<dim>::markedHomology(int k) const {
    if (k < 1 || k >= dim)
        throw InvalidArgument(
            "markedHomology(): unsupported homology dimension");

    return select_constexpr<1, dim, MarkedAbelianGroup>(k, [this](auto k) {
        return markedHomology<k>();
    });
}

template <int dim>
inline MatrixInt TriangulationBase<dim>::boundaryMap(int subdim) const {
    if (subdim < 1 || subdim > dim)
        throw InvalidArgument("boundaryMap(): unsupported face dimension");

    return select_constexpr<1, dim + 1, MatrixInt>(subdim, [this](auto k) {
        return boundaryMap<k>();
    });
}

template <int dim>
inline MatrixInt TriangulationBase<dim>::dualBoundaryMap(int subdim) const {
    constexpr int maxSubdim = (standardDim(dim) ? dim : dim - 1);
    if (subdim < 1 || subdim > maxSubdim)
        throw InvalidArgument(
            "dualBoundaryMap(): unsupported dual face dimension");

    return select_constexpr<1, maxSubdim+1, MatrixInt>(subdim, [this](auto k) {
        return dualBoundaryMap<k>();
    });
}

template <int dim>
inline MatrixInt TriangulationBase<dim>::dualToPrimal(int subdim) const {
    if (subdim < 0 || subdim >= dim)
        throw InvalidArgument(
            "dualToPrimal(): unsupported face dimension");

    return select_constexpr<0, dim, MatrixInt>(subdim, [this](auto k) {
        return dualToPrimal<k>();
    });
}

template <int dim>
void TriangulationBase<dim>::writeXMLBaseProperties(std::ostream& out) const {
    if (fundGroup_.has_value()) {
        out << "  <fundgroup>\n";
        fundGroup_->writeXMLData(out);
        out << "  </fundgroup>\n";
    }
    if (H1_.has_value()) {
        out << "  <H1>";
        H1_->writeXMLData(out);
        out << "</H1>\n";
    }
}

template <int dim>
template <int subdim>
inline Face<dim, subdim>* TriangulationBase<dim>::clonedFace(
        const Face<dim, subdim>* src) const {
    // This is a tiny function; it exists mainly to help in scenarios where
    // the integer subdim is awkward to obtain (since it will be deduced
    // here automatically from the type of the argument \a src).
    return std::get<subdim>(faces_)[src->index()];
}

template <int dim>
template <typename FaceList>
inline void TriangulationBase<dim>::cloneBoundaryFaces(
        BoundaryComponent<dim>* bc, const FaceList& srcFaces) {
    // This is a tiny function; it exists so it can be used within a
    // C++17 fold expression.
    for (auto f : srcFaces)
        bc->push_back(clonedFace(f));
}

template <int dim>
inline Triangulation<dim> TriangulationBase<dim>::fromSig(
        const std::string& sig) {
    return TriangulationBase<dim>::fromIsoSig(sig);
}

template <int dim>
template <int useDim>
bool TriangulationBase<dim>::sameDegreesAt(const TriangulationBase<dim>& other)
        const {
    // We may assume that # faces is the same for both triangulations.
    size_t n = std::get<useDim>(faces_).size();

    auto* deg1 = new size_t[n];
    auto* deg2 = new size_t[n];

    size_t* p;
    p = deg1;
    for (auto f : std::get<useDim>(faces_))
        *p++ = f->degree();
    p = deg2;
    for (auto f : std::get<useDim>(other.faces_))
        *p++ = f->degree();

    std::sort(deg1, deg1 + n);
    std::sort(deg2, deg2 + n);

    bool ans = std::equal(deg1, deg1 + n, deg2);

    delete[] deg1;
    delete[] deg2;

    return ans;
}

template <int dim>
template <int... useDim>
inline bool TriangulationBase<dim>::sameDegreesAt(
        const TriangulationBase& other,
        std::integer_sequence<int, useDim...>) const {
    return (sameDegreesAt<useDim>(other) && ...);
}

// Inline functions for TriangulationBase::TopologyLock

template <int dim>
inline TriangulationBase<dim>::TopologyLock::TopologyLock(
        TriangulationBase<dim>& tri) : tri_(tri) {
    ++tri_.topologyLock_;
}

template <int dim>
inline TriangulationBase<dim>::TopologyLock::~TopologyLock() {
    --tri_.topologyLock_;
}

} } // namespace regina::detail

#include "triangulation/detail/canonical-impl.h"

#endif
