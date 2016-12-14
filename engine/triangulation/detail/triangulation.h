
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

/*! \file triangulation/detail/triangulation.h
 *  \brief Implementation details for triangulations of arbitrary dimension.
 */

#ifndef __TRIANGULATION_H_DETAIL
#ifndef __DOXYGEN
#define __TRIANGULATION_H_DETAIL
#endif

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "regina-core.h"
#include "output.h"
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "maths/matrix.h"
#include "triangulation/generic/component.h"
#include "triangulation/generic/boundarycomponent.h"
#include "triangulation/generic/face.h"
#include "triangulation/generic/isomorphism.h"
#include "triangulation/generic/simplex.h"
#include "triangulation/alias/face.h"
#include "triangulation/alias/simplex.h"
#include "utilities/property.h"

namespace regina {

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

template <int dim> class XMLTriangulationReaderBase;

/**
 * \addtogroup detail Implementation details
 * Implementation details that end users should not need to reference directly.
 * @{
 */

/**
 * Internal class that helps a triangulation store its lists of faces.
 *
 * This class is used with <i>dim</i>-dimensional triangulations.  It provides
 * storage for all faces of dimension \a subdim and below.  The triangulation
 * class Triangulation<dim> then derives from FaceListSuite<dim, dim-1>.
 */
template <int dim, int subdim>
class FaceListSuite :
        protected FaceListSuite<dim, subdim - 1>,
        protected FaceList<dim, subdim> {
    protected:
        /**
         * Deletes all faces of dimension \a subdim and below.
         * This routine destroys the corresponding Face objects and
         * clears the lists that contain them.
         */
        void deleteFaces();
        /**
         * Swaps all faces of dimension \a subdim and below
         * with those of the given triangulation.
         *
         * @param other the face storage for the triangulation whose
         * faces are to be swapped with this.
         */
        void swapFaces(FaceListSuite<dim, subdim>& other);
        /**
         * Fills the given vector with the first (\a subdim + 1)
         * elements of the f-vector.
         *
         * Specifically, this routine pushes the values
         * \a f[0], ..., \a f[\a subdim] onto the end of the given vector,
         * where \a f[\a k] denotes the number of <i>k</i>-faces that
         * this object stores.
         *
         * @param result the vector in which the results will be placed.
         */
        void fillFVector(std::vector<size_t>& result) const;
        /**
         * Tests whether this and the given triangulation have the same
         * number of <i>k</i>-faces, for each facial dimension
         * \a k &le; \a subdim.
         *
         * @param other the triangulation to compare against this.
         * @return \c true if and only if the face counts considered are
         * identical for both triangluations.
         */
        bool sameFVector(const FaceListSuite<dim, subdim>& other) const;
        /**
         * Tests whether this and the given triangulation have the same
         * <i>k</i>-face degree sequences, for each facial dimension
         * \a k &le; \a subdim.
         *
         * For the purposes of this routine, degree sequences are
         * considered to be unordered.
         *
         * \pre This and the given triangulation are known to have the
         * same number of <i>k</i>-faces as each other, for each facial
         * dimension \a k &le; \a subdim.
         *
         * @param other the triangulation to compare against this.
         * @return \c true if and only if all degree sequences considered
         * are equal.
         */
        bool sameDegrees(const FaceListSuite<dim, subdim>& other) const;
};

#ifndef __DOXYGEN

template <int dim>
class FaceListSuite<dim, 0> : protected FaceList<dim, 0> {
    protected:
        void deleteFaces();
        void swapFaces(FaceListSuite<dim, 0>& other);
        void fillFVector(std::vector<size_t>& result) const;
        bool sameFVector(const FaceListSuite<dim, 0>& other) const;
        bool sameDegrees(const FaceListSuite<dim, 0>& other) const;
};

#endif // __DOXYGEN

/**
 * Helper class that indicates what data type is used by a triangulation
 * class to store a list of <i>subdim</i>-faces.
 */
template <int dim, int subdim>
struct FaceListHolder<detail::TriangulationBase<dim>, subdim> {
    /**
     * The data type used by Triangulation<dim> to store the list of all
     * <i>subdim</i>-faces of the triangulation.
     *
     * The function Triangulation<dim>::faces<subdim>() returns a const
     * reference to this type.
     */
    typedef FaceList<dim, subdim> Holder;
};

/**
 * Internal class used to calculate lower-dimensional faces in a
 * triangulation.
 *
 * Specifically, this class is used to calculate all faces of dimension
 * &le; \a subdim in a <i>dim</i>-dimensional triangulation.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * \tparam subdim the maximum dimension of the faces to compute.
 * \tparam codim the minimum codimension of the faces to compute; this
 * must be equal to \a dim - \a subdim.  It is offered as a separate
 * parameter so that this template class can be independently specialised
 * on both \a subdim and \a codim.
 */
template <int dim, int subdim, int codim>
struct FaceCalculator {
    static_assert(dim == subdim + codim,
        "FaceCalculator template arguments violate subdim + codim = dim.");
    static_assert(codim > 2 && subdim > 0,
        "The generic FaceCalculator template cannot be used for "
        "small face dimension or codimension.");
    /**
     * Calculates all faces of dimension &le; \a subdim in the given
     * triangulation.
     *
     * @param t the triangulation whose faces should be calculated.
     */
    static void calculate(TriangulationBase<dim>& t) {
        t.template calculateSkeletonSubdim<subdim>();
        FaceCalculator<dim, subdim - 1, codim + 1>::calculate(t);
    }
};

#ifndef __DOXYGEN

template <int dim, int subdim>
struct FaceCalculator<dim, subdim, 1> {
    static_assert(dim == subdim + 1,
        "FaceCalculator specialisation violates subdim + codim = dim.");
    static_assert(subdim > 0,
        "FaceCalculator<dim, subdim, 1> specialisation cannot be used "
        "with subdim = 0.");
    static void calculate(TriangulationBase<dim>& t) {
        t.calculateSkeletonCodim1();
        FaceCalculator<dim, subdim - 1, 2>::calculate(t);
    }
};

template <int dim, int subdim>
struct FaceCalculator<dim, subdim, 2> {
    static_assert(dim == subdim + 2,
        "FaceCalculator specialisation violates subdim + codim = dim.");
    static_assert(subdim > 0,
        "FaceCalculator<dim, subdim, 2> specialisation cannot be used "
        "with subdim = 0.");
    static void calculate(TriangulationBase<dim>& t) {
        t.calculateSkeletonCodim2();
        FaceCalculator<dim, subdim - 1, 3>::calculate(t);
    }
};

template <int dim, int codim>
struct FaceCalculator<dim, 0, codim> {
    static_assert(dim == codim,
        "FaceCalculator specialisation violates subdim + codim = dim.");
    static_assert(codim > 2,
        "FaceCalculator<dim, 0, codim> specialisation cannot be used "
        "with codim <= 2.");
    static void calculate(TriangulationBase<dim>& t) {
        t.template calculateSkeletonSubdim<0>();
    }
};

template <int dim>
struct FaceCalculator<dim, 0, 2> {
    static_assert(dim == 2,
        "FaceCalculator specialisation violates subdim + codim = dim.");
    static void calculate(TriangulationBase<dim>& t) {
        t.calculateSkeletonCodim2();
    }
};

#endif // __DOXYGEN

/**
 * Internal class used to identify lower-dimensional faces in a boundary
 * component of a triangulation.
 *
 * Specifically, this class identifies and marks all faces of dimensions
 * 1,...,\a subdim within the given boundary facet of a
 * <i>dim</i>-dimensional triangulation.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * \tparam subdim the maximum dimension of the faces to identify.
 * This must be between -1 and (\a dim - 3) inclusive.  In the cases where
 * \a subdim = 0 or -1, the identify() routine for this class does nothing.
 */
template <int dim, int subdim>
struct BoundaryComponentCalculator {
    static_assert(1 <= subdim && subdim <= dim - 3,
        "The generic BoundaryComponentCalculator template can only be used "
        "for faces of dimension 1 <= subdim <= (dim - 3).");

    /**
     * Identifies and marks all faces of dimension &le; \a subdim within the
     * given boundary facet of the given <i>dim</i>-dimensional triangulation.
     *
     * This routine pushes all such <i>subdim</i>-faces onto the relevant list
     * for the given boundary component, and also marks the boundary component
     * within these <i>subdim</i>-faces themselves.
     *
     * @param t the underlying triangulation.
     * @param bc the boundary component of \a t currently under construction.
     * @param facet a boundary facet that belongs to \a bc.
     */
    static void identify(TriangulationBase<dim>& t,
            BoundaryComponent<dim>* bc, Face<dim, dim-1>* facet) {
        t.template calculateBoundaryFaces<subdim>(bc, facet);
        BoundaryComponentCalculator<dim, subdim - 1>::identify(t, bc, facet);
    }
};

#ifndef __DOXYGEN

template <int dim>
struct BoundaryComponentCalculator<dim, 0> {
    static void identify(TriangulationBase<dim>&, void*, void*) {
    }
};

template <int dim>
struct BoundaryComponentCalculator<dim, -1> {
    static void identify(TriangulationBase<dim>&, void*, void*) {
    }
};

#endif // __DOXYGEN

/**
 * Internal class used to calculate the Euler characteristic of a
 * triangulation.
 *
 * Specifically, this class calculates the alternating sum of the number
 * of faces of dimensions \a subdim, ..., \a dim within a
 * <i>dim</i>-dimensional triangulation.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * \tparam subdim the minimum dimension of the faces to consider.
 */
template <int dim, int subdim>
struct EulerCalculator {
    /**
     * Computes the alternating sum of the number of faces of \a tri of
     * dimensions \a subdim, ..., \a dim.  Specifically, this computes
     * <tt>tri.countFaces<subdim>() - tri.countFaces<subdim+1>() + ...
     * +/- tri.countFaces<dim>()</tt>.
     *
     * @param tri the triangulations whose face counts are to be computed.
     * @return the resulting "partial" Euler characteristic.
     */
    static long compute(const TriangulationBase<dim>& tri) {
        // Remember to cast away the unsignedness of size_t, since this
        // is an alternating sum.
        return static_cast<long>(tri.template countFaces<subdim>()) -
            EulerCalculator<dim, subdim + 1>::compute(tri);
    }
};

#ifndef __DOXYGEN

template <int dim>
struct EulerCalculator<dim, dim> {
    static long compute(const TriangulationBase<dim>& tri) {
        return tri.size();
    }
};

#endif // __DOXYGEN

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
 * \ifacespython This base class is not present, but the "end user" class
 * Triangulation<dim> is.
 *
 * \tparam dim the dimension of the triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
class TriangulationBase :
        protected FaceListSuite<dim, dim - 1>,
        public alias::Simplices<TriangulationBase<dim>, dim>,
        public alias::SimplexAt<TriangulationBase<dim>, dim, true>,
        public alias::FaceOfTriangulation<TriangulationBase<dim>, dim>,
        public alias::FacesOfTriangulation<TriangulationBase<dim>, dim>,
        public boost::noncopyable {
    static_assert(dim >= 2, "Triangulation requires dimension >= 2.");

    public:
        static constexpr int dimension = dim;
            /**< A compile-time constant that gives the dimension of the
                 triangulation. */

        typedef typename std::vector<Simplex<dim>*>::const_iterator
                SimplexIterator;
            /**< Used to iterate through top-dimensional simplices. */
        typedef typename std::vector<Component<dim>*>::const_iterator
                ComponentIterator;
            /**< Used to iterate through connected components. */
        typedef typename std::vector<BoundaryComponent<dim>*>::const_iterator
                BoundaryComponentIterator;
            /**< Used to iterate through boundary components. */

    protected:
        MarkedVector<Simplex<dim>> simplices_;
            /**< The top-dimensional simplices that form the triangulation. */
        MarkedVector<BoundaryComponent<dim>> boundaryComponents_;
            /**< The components that form the boundary of the triangulation. */
        bool valid_;
            /**< Is this triangulation valid?  See isValid() for details
                 on what this means. */

    private:
        bool calculatedSkeleton_;
            /**< Has the skeleton been calculated?  This is only done
                 "on demand", when a skeletal property is first queried. */
        MarkedVector<Component<dim>> components_;
            /**< The connected components that form the triangulation.
                 This list is only filled if/when the skeleton of the
                 triangulation is computed. */
        bool orientable_;
            /**< Is the triangulation orientable?  This property is only set
                 if/when the skeleton of the triangulation is computed. */
        mutable Property<GroupPresentation, StoreManagedPtr> fundGroup_;
            /**< Fundamental group of the triangulation. */
        mutable Property<AbelianGroup, StoreManagedPtr> H1_;
            /**< First homology group of the triangulation. */

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
         * This will clone any computed properties (such as homology,
         * fundamental group, and so on) of the given triangulation also.
         * If you want a "clean" copy that resets all properties to unknown,
         * you can use the two-argument copy constructor instead.
         *
         * @param copy the triangulation to copy.
         */
        TriangulationBase(const TriangulationBase<dim>& copy);
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties also.
         *
         * @param copy the triangulation to copy.
         * @param cloneProps \c true if this should also clone any computed
         * properties of the given triangulation (such as homology,
         * fundamental group, and so on), or \c false if the new triangulation
         * should have all properties marked as unknown.
         */
        TriangulationBase(const TriangulationBase<dim>& copy, bool cloneProps);
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
         * @return The number of top-dimensional simplices.
         */
        size_t size() const;
        /**
         * Returns all top-dimensional simplices in the triangulation.
         *
         * The reference that is returned will remain valid for as long as
         * the triangulation exists: even as simplices are added and/or
         * removed, it will always reflect the simplices that are currently
         * in the triangulation.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all top-dimensional simplices.
         */
        const std::vector<Simplex<dim>*>& simplices() const;
        /**
         * Returns the top-dimensional simplex at the given index in the
         * triangulation.
         *
         * Note that indexing may change when a simplex is added to or
         * removed from the triangulation.
         *
         * @param index specifies which simplex to return; this
         * value should be between 0 and size()-1 inclusive.
         * @return the <i>index</i>th top-dimensional simplex.
         */
        Simplex<dim>* simplex(size_t index);
        /**
         * Returns the top-dimensional simplex at the given index in the
         * triangulation.
         *
         * Note that indexing may change when a simplex is added to or
         * removed from the triangulation.
         *
         * @param index specifies which simplex to return; this
         * value should be between 0 and size()-1 inclusive.
         * @return the <i>index</i>th top-dimensional simplex.
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
         * @return the new simplex.
         */
        Simplex<dim>* newSimplex();
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
         * @param desc the description to give to the new simplex.
         * @return the new simplex.
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
         * @param simplex the simplex to remove.
         */
        void removeSimplex(Simplex<dim>* simplex);
        /**
         * Removes the top-dimensional simplex at the given index in
         * this triangulation.
         *
         * This is equivalent to calling <tt>removeSimplex(simplex(index))</tt>.
         *
         * The given simplex will be unglued from any adjacent simplices
         * (if any), and will be destroyed immediately.
         *
         * @param index specifies which top-dimensionalsimplex to remove; this
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
         * Swaps the contents of this and the given triangulation.
         *
         * All top-dimensional simplices that belong to this triangulation
         * will be moved to \a other, and all top-dimensional simplices
         * that belong to \a other will be moved to this triangulation.
         * Likewise, all skeletal objects (such as lower-dimensional faces,
         * components, and boundary components) and all cached properties
         * (such as homology and fundamental group) will be swapped.
         *
         * In particular, any pointers or references to Simplex<dim> and/or
         * Face<dim, subdim> objects will remain valid.
         *
         * This routine will behave correctly if \a other is in fact
         * this triangulation.
         *
         * @param other the triangulation whose contents should be
         * swapped with this.
         */
        void swapContents(Triangulation<dim>& other);
        /**
         * Moves the contents of this triangulation into the given
         * destination triangulation, without destroying any pre-existing
         * contents.
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
         * \pre \a dest is not this triangulation.
         *
         * @param dest the triangulation into which simplices should be moved.
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
         * @return the number of connected components.
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
         * @return the number of boundary components.
         */
        size_t countBoundaryComponents() const;

        /**
         * Returns the number of <i>subdim</i>-faces in this triangulation.
         *
         * \pre The template argument \a subdim is between 0 and <i>dim</i>-1
         * inclusive.
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
         * Returns the f-vector of this triangulation, which counts the
         * number of faces of all dimensions.
         *
         * The vector that is returned will have length <i>dim</i>+1.
         * If this vector is \a f, then \a f[\a k] will be the number of
         * <i>k</i>-faces for each 0 &le; \a k &le; \a dim.
         *
         * This routine is significantly more heavyweight than countFaces().
         * Its advantage is that, unlike the templatised countFaces(),
         * it allows you to count faces whose dimensions are not known
         * until runtime.
         *
         * @return the f-vector of this triangulation.
         */
        std::vector<size_t> fVector() const;

        /**
         * Returns all connected components of this triangulation.
         *
         * Note that each time the triangulation changes, all component
         * objects will be deleted and replaced with new ones.
         * Therefore these component objects should be considered temporary
         * only.
         *
         * In contrast, this reference to the \e list of all components
         * will remain valid and up-to-date for as long as the triangulation
         * exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all components.
         */
        const std::vector<Component<dim>*>& components() const;

        /**
         * Returns all boundary components of this triangulation.
         *
         * Note that, in Regina's \ref stddim "standard dimensions",
         * each ideal vertex forms its own boundary component, and
         * some invalid vertices do also.  See the BoundaryComponent
         * class notes for full details on what constitutes a boundary
         * component in standard and non-standard dimensions.
         *
         * Bear in mind that each time the triangulation changes, all
         * boundary component objects will be deleted and replaced with new
         * ones.  Therefore these boundary component objects should be
         * considered temporary only.
         *
         * In contrast, this reference to the \e list of BoundaryComponent
         * objects will remain valid and up-to-date for as long as the
         * triangulation exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all boundary components.
         */
        const std::vector<BoundaryComponent<dim>*>& boundaryComponents() const;

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, all
         * face objects will be deleted and replaced with new ones.
         * Therefore these face objects should be considered temporary only.
         *
         * In contrast, this reference to the FaceList object itself will
         * remain valid and up-to-date for as long as the triangulation exists.
         *
         * \ifacespython Python users should call this function in the
         * form <tt>faces(subdim)</tt>.  It will then return a Python list
         * containing all the <i>subdim</i>-faces of the triangulation.
         * Be warned that, unlike in C++, this Python list will be a
         * snapshot of the faces when this function is called, and will
         * \e not be kept up-to-date as the triangulation changes.
         *
         * @return access to the list of all <i>subdim</i>-faces.
         */
        template <int subdim>
        const FaceList<dim, subdim>& faces() const;

        /**
         * Returns the requested connected component of this triangulation.
         *
         * Note that each time the triangulation changes, all component
         * objects will be deleted and replaced with new ones.
         * Therefore this component object should be considered temporary only.
         *
         * @param index the index of the desired component; this must be
         * between 0 and countComponents()-1 inclusive.
         * @return the requested component.
         */
        Component<dim>* component(size_t index) const;

        /**
         * Returns the requested boundary component of this triangulation.
         *
         * Note that each time the triangulation changes, all
         * boundary components will be deleted and replaced with new
         * ones.  Therefore this object should be considered temporary only.
         *
         * @param index the index of the desired boundary component; this must
         * be between 0 and countBoundaryComponents()-1 inclusive.
         * @return the requested boundary component.
         */
        BoundaryComponent<dim>* boundaryComponent(size_t index) const;

        /**
         * Returns the requested <i>subdim</i>-face of this triangulation.
         *
         * \pre The template argument \a subdim is between 0 and <i>dim</i>-1
         * inclusive.
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
        Face<dim, subdim>* face(size_t index) const;

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Determines whether this triangulation is empty.
         * An empty triangulation is one with no simplices at all.
         *
         * @return \c true if and only if this triangulation is empty.
         */
        bool isEmpty() const;

        /**
         * Determines if this triangulation is valid.
         *
         * There are several conditions that might make a
         * <i>dim</i>-dimensional triangulation invalid:
         *
         * 1. if some face is identified with itself under a non-identity
         *    permutation (e.g., an edge is identified with itself in
         *    reverse, or a triangle is identified with itself under a
         *    rotation);
         * 2. if some <i>subdim</i>-face does not have an appropriate link.
         *    Here the meaning of "appropriate" depends upon the type of face:
         *    - for a face that belongs to some boundary facet(s) of this
         *      triangulation, its link must be a topological ball;
         *    - for a vertex that does not belong to any boundary facets,
         *      its link must be a closed (\a dim - 1)-manifold;
         *    - for a (\a subdim &ge; 1)-face that does not belong to any
         *      boundary facets, its link must be a topological sphere.
         *
         * Condition (1) is tested for all dimensions \a dim.
         * Condition (2) is more difficult, since it relies on undecidable
         * problems.  As a result, (2) is \e only tested when \a dim is one
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
         * @return \c true if and only if this triangulation is valid.
         */
        bool isValid() const;

        /**
         * Determines if this triangulation has any boundary facets.
         *
         * This routine returns \c true if and only if the
         * triangulation contains some top-dimension simplex with at
         * least one facet that is not glued to an adjacent simplex.
         *
         * @return \c true if and only if there are boundary facets.
         */
        bool hasBoundaryFacets() const;

        /**
         * Returns the total number of boundary facets in this triangulation.
         *
         * This routine counts facets of top-dimensional simplices that are
         * not glued to some adjacent top-dimensional simplex.
         *
         * @return the total number of boundary facets.
         */
        size_t countBoundaryFacets() const;

        /**
         * Determines if this triangulation is orientable.
         *
         * @return \c true if and only if this triangulation is orientable.
         */
        bool isOrientable() const;

        /**
         * Determines if this triangulation is connected.
         *
         * @return \c true if and only if this triangulation is connected.
         */
        bool isConnected() const;

        /**
         * Determines if this triangulation is oriented; that is, if the
         * vertices of its top-dimensional simplices are labelled in a way
         * that preserves orientation across adjacent facets.
         * Specifically, this routine returns \c true if and only if every
         * gluing permutation has negative sign.
         *
         * Note that \e orientable triangulations are not always \e oriented
         * by default.  You can call orient() if you need the top-dimensional
         * simplices to be oriented consistently as described above.
         *
         * A non-orientable triangulation can never be oriented.
         *
         * @return \c true if and only if all top-dimensional simplices are
         * oriented consistently.
         *
         * @author Matthias Goerner
         */
        bool isOriented() const;

        /**
         * Returns the Euler characteristic of this triangulation.
         * This will be evaluated strictly as the alternating sum
         * of the number of <i>i</i>-faces (that is,
         * <tt>countVertices() - countEdges() + countTriangles() - ...</tt>).
         *
         * Note that this routine handles ideal triangulations in a
         * non-standard way.  Since it computes the Euler characteristic of
         * the triangulation (and not the underlying manifold), this routine
         * will treat each ideal boundary component as a single vertex, and
         * \e not as an entire (<i>dim</i>-1)-dimensional boundary component.
         *
         * In Regina's \ref stddim "standard dimensions", for a routine that
         * handles ideal boundary components properly (by treating them as
         * (<i>dim</i>-1)-dimensional boundary components when computing Euler
         * characteristic), you can use the routine eulerCharManifold() instead.
         *
         * @return the Euler characteristic of this triangulation.
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
         *   fundamental group will be computed \e without truncating the
         *   centroid of the face.  For instance, if a 3-manifold
         *   triangulation has an edge identified with itself in reverse,
         *   then the fundamental group will be computed without truncating
         *   the resulting projective plane cusp.  This means that, if a
         *   barycentric subdivision is performed on a such a
         *   triangulation, the result of fundamentalGroup() might change.
         *
         * Bear in mind that each time the triangulation changes, the
         * fundamental group will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, fundamentalGroup() should be called again; this will
         * be instantaneous if the group has already been calculated.
         *
         * \pre This triangulation has at most one component.
         *
         * \warning In dimension 3, if you are calling this from the subclass
         * SnapPeaTriangulation then <b>any fillings on the cusps will be
         * ignored</b>.  (This is the same as for every routine implemented by
         * Regina's Triangulation<3> class.)  If you wish to compute the
         * fundamental group with fillings, call
         * SnapPeaTriangulation::fundamentalGroupFilled() instead.
         *
         * @return the fundamental group.
         */
        const GroupPresentation& fundamentalGroup() const;
        /**
         * Notifies the triangulation that you have simplified the
         * presentation of its fundamental group.  The old group
         * presentation will be destroyed, and this triangulation will take
         * ownership of the new (hopefully simpler) group that is passed.
         *
         * This routine is useful for situations in which some external
         * body (such as GAP) has simplified the group presentation
         * better than Regina can.
         *
         * Regina does \e not verify that the new group presentation is
         * equivalent to the old, since this is - well, hard.
         *
         * If the fundamental group has not yet been calculated for this
         * triangulation, this routine will nevertheless take ownership
         * of the new group, under the assumption that you have worked
         * out the group through some other clever means without ever
         * having needed to call fundamentalGroup() at all.
         *
         * Note that this routine will not fire a packet change event.
         *
         * @param newGroup a new (and hopefully simpler) presentation of
         * the fundamental group of this triangulation.
         */
        void simplifiedFundamentalGroup(GroupPresentation* newGroup);

        /**
         * Returns the first homology group for this triangulation.
         *
         * The homology is computed in the dual 2-skeleton.  This means:
         *
         * - If the triangulation contains any ideal vertices, the homology
         *   will be calculated as if each such vertex had been truncated.
         *
         * - Likewise, if the triangulation contains any invalid faces
         *   of dimension 0,1,...,(<i>dim</i>-3), these will effectively
         *   be truncated also.
         *
         * - In contrast, if the triangulation contains any invalid
         *   (<i>dim</i>-2)-faces (i.e., codimension-2-faces that are
         *   identified with themselves under a non-trivial map), the
         *   homology will be computed \e without truncating the
         *   centroid of the face.  For instance, if a 3-manifold
         *   triangulation has an edge identified with itself in reverse,
         *   then the homology will be computed without truncating the
         *   resulting projective plane cusp.  This means that, if a
         *   barycentric subdivision is performed on a such a
         *   triangulation, the result of homology() might change.
         *
         * This routine can also be accessed via the alias homologyH1()
         * (a name that is more specific, but a little longer to type).
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homology() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \warning In dimension 3, if you are calling this from the subclass
         * SnapPeaTriangulation then <b>any fillings on the cusps will be
         * ignored</b>.  (This is the same as for every routine implemented by
         * Regina's Triangulation<3> class.)  If you wish to compute homology
         * with fillings, call SnapPeaTriangulation::homologyFilled() instead.
         *
         * @return the first homology group.
         */
        const AbelianGroup& homology() const;

        /**
         * Returns the first homology group for this triangulation.
         *
         * This is identical to calling homology().  See the homology()
         * documentation for further details.
         *
         * @return the first homology group.
         */
        const AbelianGroup& homologyH1() const;

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
         * If simplex \a s has index \a i in the original triangulation, then
         * its sub-simplex corresponding to permutation \a p will have index
         * <tt>((dim + 1)! * i + p.index())</tt> in the resulting triangulation.
         * In other words: sub-simplices are ordered first according to the
         * original simplex that contains them, and then according to the
         * lexicographical ordering of the corresponding permutations \a p.
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
         */
        void barycentricSubdivision();

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
         * @return \c true if changes were made, or \c false if the
         * original triangulation contained no real boundary components.
         */
        bool finiteToIdeal();

        /*@}*/
        /**
         * \name Decompositions
         */
        /*@{*/

        /**
         * Splits a disconnected triangulation into many smaller triangulations,
         * one for each component.  The new component triangulations will be
         * inserted as children of the given parent packet.  The original
         * triangulation (i.e., this triangulation) will be left unchanged.
         *
         * If the given parent packet is 0, the new component triangulations
         * will be inserted as children of this triangulation.
         *
         * By default, this routine will assign sensible packet labels to each
         * of the new component triangulations.  If these component
         * triangulations are only temporary objects used as part of some
         * larger algorithm, then labels are unnecessary - in this case
         * you can pass \a setLabels as \c false to avoid the (small) overhead
         * that these packet labels incur.
         *
         * @param componentParent the packet beneath which the new
         * component triangulations will be inserted, or 0 if they
         * should be inserted directly beneath this triangulation.
         * @param setLabels \c true if the new component triangulations
         * should be assigned sensible packet labels, or \c false if
         * they should be left without labels at all.
         * @return the number of new component triangulations constructed.
         */
        size_t splitIntoComponents(Packet* componentParent = 0,
            bool setLabels = true);

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
         * For the less strict notion of \e isomorphic triangulations,
         * which allows relabelling of the top-dimensional simplices and their
         * vertices, see isIsomorphicTo() instead.
         *
         * This test does \e not examine the textual simplex descriptions,
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
         * @param other the triangulation to compare with this one.
         * @return \c true if and only if the two triangulations are
         * combinatorially identical.
         */
        bool isIdenticalTo(const Triangulation<dim>& other) const;

        /**
         * Determines if this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * Two triangulations are \e isomorphic if and only it is
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
         * The isomorphism will be newly constructed, and to assist with
         * memory management, it will be returned as a std::unique_ptr.
         * Thus, to test whether an isomorphism exists without having to
         * explicitly manage with the isomorphism itself, you can just call
         * <tt>if (isIsomorphicTo(other).get())</tt>, in which case the newly
         * created isomorphism (if it exists) will be automatically
         * destroyed.
         *
         * There may be many such isomorphisms between the two triangulations.
         * If you need to find \e all such isomorphisms, you may call
         * findAllIsomorphisms() instead.
         *
         * If you need to ensure that top-dimensional simplices are labelled
         * the same in both triangulations (i.e., that the triangulations are
         * related by the \e identity isomorphism), you should call the
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
         * @param other the triangulation to compare with this one.
         * @return details of the isomorphism if the two triangulations
         * are combinatorially isomorphic, or a null pointer otherwise.
         */
        std::unique_ptr<Isomorphism<dim>> isIsomorphicTo(
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
         * this isomorphism are returned.  The isomorphism is newly
         * constructed, and so to assist with memory management is
         * returned as a std::unique_ptr.  Thus, to test whether an
         * isomorphism exists without having to explicitly deal with the
         * isomorphism itself, you can call
         * <tt>if (isContainedIn(other).get())</tt> and the newly
         * created isomorphism (if it exists) will be automatically
         * destroyed.
         *
         * If more than one such isomorphism exists, only one will be
         * returned.  For a routine that returns all such isomorphisms,
         * see findAllSubcomplexesIn().
         *
         * \warning For large dimensions, this routine can become
         * extremely slow: its running time includes a factor of
         * (<i>dim</i>+1)!.
         *
         * @param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * @return details of the isomorphism if such a copy is found,
         * or a null pointer otherwise.
         */
        std::unique_ptr<Isomorphism<dim>> isContainedIn(
            const Triangulation<dim>& other) const;

        /**
         * Finds all ways in which this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * This routine behaves identically to isIsomorphicTo(), except that
         * instead of returning just one isomorphism, all such isomorphisms
         * are returned.
         *
         * See the isIsomorphicTo() notes for additional information.
         *
         * The isomorphisms that are found will be written to the given
         * output iterator.  This iterator must accept objects of type
         * Isomorphism<dim>*.  As an example, \a output might be a
         * back_insert_iterator for a std::vector<Isomorphism<dim>*>.
         *
         * The isomorphisms that are written to the given output iterator
         * will be newly created, and the caller of this routine is
         * responsible for destroying them.
         *
         * \ifacespython The \a output argument is not present.
         * Instead, this routine returns a python list containing all of
         * the isomorphisms that were found.
         *
         * \warning For large dimensions, this routine can become
         * extremely slow: its running time includes a factor of
         * (<i>dim</i>+1)!.
         *
         * @param other the triangulation to compare with this one.
         * @param output the output iterator to which the isomorphisms
         * will be written.
         * @return the number of isomorphisms that were found.
         */
        template <typename OutputIterator>
        size_t findAllIsomorphisms(const Triangulation<dim>& other,
            OutputIterator output) const;

        /**
         * Finds all ways in which an isomorphic copy of this triangulation
         * is contained within the given triangulation, possibly as a
         * subcomplex of some larger component (or components).
         *
         * This routine behaves identically to isContainedIn(), except
         * that instead of returning just one isomorphism (which may be
         * boundary incomplete and need not be onto), all such isomorphisms
         * are returned.
         *
         * See the isContainedIn() notes for additional information.
         *
         * The isomorphisms that are found will be written to the given
         * output iterator.  This iterator must accept objects of type
         * Isomorphism<dim>*.  As an example, \a output might be a
         * back_insert_iterator for a std::vector<Isomorphism<dim>*>.
         *
         * The isomorphisms that are written to the given output iterator
         * will be newly created, and the caller of this routine is
         * responsible for destroying them.
         *
         * \warning For large dimensions, this routine can become
         * extremely slow: its running time includes a factor of
         * (<i>dim</i>+1)!.
         *
         * \ifacespython Not present.
         *
         * @param other the triangulation in which to search for
         * isomorphic copies of this triangulation.
         * @param output the output iterator to which the isomorphisms
         * will be written.
         * @return the number of isomorphisms that were found.
         */
        template <typename OutputIterator>
        size_t findAllSubcomplexesIn(const Triangulation<dim>& other,
            OutputIterator output) const;

        /*@}*/
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
         * @return \c true if the triangulation was changed, or \c false
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
         * @param source the triangulation whose copy will be inserted.
         */
        void insertTriangulation(const Triangulation<dim>& source);

        /**
         * Inserts a given triangulation into this triangulation, where
         * the given triangulation is described by a pair of integer arrays.
         *
         * The main purpose of this routine is to allow users to hard-code
         * triangulations into C++ source files.  In particular, all of the
         * simplex gluings can be hard-coded into a pair of integer arrays
         * at the beginning of the source file, avoiding an otherwise tedious
         * sequence of many calls to Simplex<dim>::join().  If you have
         * a particular triangulation that you would like to hard-code
         * in this way, you can call dumpConstruction() to generate the
         * corresponding integer arrays as C++ source code.
         *
         * This routine will insert an additional \a nSimplices top-dimensional
         * simplices into this triangulation.  We number these simplices
         * 0,1,...,<i>nSimplices</i>-1.  The gluings between these
         * new simplices should be stored in the two arrays as follows.
         *
         * The \a adjacencies array describes which simplices are joined to
         * which others.  Specifically, <tt>adjacencies[s][f]</tt> indicates
         * which of the new simplices is joined to facet \a f of simplex \a s.
         * This should be between 0 and <i>nSimplices</i>-1 inclusive, or -1
         * if facet \a f of simplex \a s is to be left as a boundary facet.
         *
         * The \a gluings array describes the particular gluing permutations
         * used to join these simplices together.  Specifically,
         * <tt>gluings[s][f][0..\a dim]</tt> should describe the permutation
         * used to join facet \a f of simplex \a s to its adjacent simplex.
         * These <i>dim</i>+1 integers should be 0,1,...,\a dim in some
         * order, so that <tt>gluings[s][f][i]</tt> contains the image of \a i
         * under this permutation.  If facet \a f of simplex \a s is to be
         * left as a boundary facet, then <tt>gluings[s][f][0..\a dim]</tt>
         * may contain anything (and will be duly ignored).
         *
         * If this triangulation is empty before this routine is called, then
         * the new simplices will be given indices 0,1,...,<i>nSimplices</i>-1
         * according to the numbering described above.  Otherwise they will be
         * inserted after any pre-existing simplices, and so they will be given
         * larger indices instead.  In the latter case, the \a adjacencies
         * array should still refer to the new simplices as
         * 0,1,...,<i>nSimplices</i>-1, and this routine will handle any
         * renumbering automatically at runtime.
         *
         * It is the responsibility of the caller of this routine to
         * ensure that the given arrays are correct and consistent.
         * No error checking will be performed by this routine.
         *
         * \ifacespython Not present.
         *
         * @param nSimplices the number of additional simplices to insert.
         * @param adjacencies describes which simplices are adjace to
         * which others, as described above.  This array must have initial
         * dimension at least \a nSimplices.
         * @param gluings describes the specific gluing permutations, as
         * described above.  This array must also have initial dimension
         * at least \a nSimplices.
         */
        void insertConstruction(
            size_t nSimplices,
            const int adjacencies[][dim+1],
            const int gluings[][dim+1][dim+1]);

        /*@}*/
        /**
         * \name Exporting Triangulations
         */
        /*@{*/

        /**
         * Constructs the isomorphism signature for this triangulation.
         *
         * An <i>isomorphism signature</i> is a compact text representation of
         * a triangulation that uniquely determines the triangulation up to
         * combinatorial isomorphism.  That is, two triangulations of
         * dimension \a dim are combinatorially isomorphic if and only if
         * their isomorphism signatures are the same.
         *
         * The isomorphism signature is constructed entirely of printable
         * characters, and has length proportional to <tt>n log n</tt>,
         * where \a n is the number of top-dimenisonal simplices.
         *
         * Whilst the format of an isomorphism signature bears some
         * similarity to dehydration strings for 3-manifolds, they are more
         * general: isomorphism signatures can be used with any triangulations,
         * including closed, bounded and/or disconnected triangulations,
         * as well as triangulations with many simplices.  Note also that
         * 3-manifold dehydration strings are not unique up to isomorphism
         * (they depend on the particular labelling of tetrahedra).
         *
         * The time required to construct the isomorphism signature of a
         * triangulation is <tt>O((dim!) n^2 log^2 n)</tt>.  Whilst this
         * is fine for large triangulation, it will be extremly slow for
         * large \e dimensions.
         *
         * The routine fromIsoSig() can be used to recover a
         * triangulation from an isomorphism signature.  The triangulation
         * recovered might not be identical to the original, but it will be
         * combinatorially isomorphic.
         *
         * If \a relabelling is non-null (i.e., it points to some
         * Isomorphism pointer \a p), then it will be modified to point
         * to a new isomorphism that describes the precise relationship
         * between this triangulation and the reconstruction from fromIsoSig().
         * Specifically, the triangulation that is reconstructed from
         * fromIsoSig() will be combinatorially identical to
         * <tt>relabelling.apply(this)</tt>.
         *
         * For a full and precise description of the isomorphism signature
         * format for 3-manifold triangulations, see <i>Simplification paths
         * in the Pachner graphs of closed orientable 3-manifold
         * triangulations</i>, Burton, 2011, <tt>arXiv:1110.6080</tt>.
         * The format for other dimensions is essentially the same, but with
         * minor dimension-specific adjustments.
         *
         * \ifacespython The isomorphism argument is not present.
         * Instead there are two routines: fromIsoSig(), which returns a
         * string only, and fromIsoSigDetail(), which returns a pair
         * (\a signature, \a relabelling).
         *
         * \pre If \a relabelling is non-null, then this triangulation
         * must be non-empty and connected.  The facility to return a
         * relabelling for disconnected triangulations may be added to
         * Regina in a later release.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different dimensions \a p and \a q.
         *
         * @param relabelling if this is non-null, it will be modified to
         * point to a new isomorphism that describes the relationship between
         * this triangulation and the triangulation that will be reconstructed
         * from fromIsoSig(), as described above.
         * @return the isomorphism signature of this triangulation.
         */
        std::string isoSig(Isomorphism<dim>** relabelling = 0) const;

        /**
         * Returns C++ code that can be used with insertConstruction()
         * to reconstruct this triangulation.
         *
         * The code produced will consist of the following:
         *
         * - the declaration and initialisation of two integer arrays,
         *   describing the gluings between simplices of this trianguation;
         * - two additional lines that declare a new Triangulation<dim> and
         *   call insertConstruction() to rebuild this triangulation.
         *
         * The main purpose of this routine is to generate the two integer
         * arrays, which can be tedious and error-prone to code up by hand.
         *
         * Note that the number of lines of code produced grows linearly
         * with the number of simplices.  If this triangulation is very
         * large, the returned string will be very large as well.
         *
         * @return the C++ code that was generated.
         */
        std::string dumpConstruction() const;

        /*@}*/
        /**
         * \name Importing Triangulations
         */
        /*@{*/

        /**
         * Recovers a full triangulation from an isomorphism signature.
         *
         * See isoSig() for more information on isomorphism signatures.
         * It will be assumed that the signature describes a triangulation of
         * dimension \a dim.
         *
         * The triangulation that is returned will be newly created, and
         * it is the responsibility of the caller of this routine to
         * destroy it.
         *
         * Calling isoSig() followed by fromIsoSig() is not guaranteed to
         * produce an \e identical triangulation to the original, but it
         * is guaranteed to produce a combinatorially \e isomorphic
         * triangulation.  In other words, fromIsoSig() may reconstruct the
         * triangulation with its simplices and/or vertices relabelled.
         * The optional argument to isoSig() allows you to determine the
         * precise relabelling that will be used, if you need to know it.
         *
         * For a full and precise description of the isomorphism signature
         * format for 3-manifold triangulations, see <i>Simplification paths
         * in the Pachner graphs of closed orientable 3-manifold
         * triangulations</i>, Burton, 2011, <tt>arXiv:1110.6080</tt>.
         * The format for other dimensions is essentially the same, but with
         * minor dimension-specific adjustments.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different dimensions \a p and \a q.
         *
         * @param sig the isomorphism signature of the triangulation to
         * construct.  Note that isomorphism signature are case-sensitive
         * (unlike, for example, dehydration strings for 3-manifolds).
         * @return a newly allocated triangulation if the reconstruction was
         * successful, or \c null if the given string was not a valid
         * <i>dim</i>-dimensional isomorphism signature.
         */
        static Triangulation<dim>* fromIsoSig(const std::string& sig);

        /**
         * Deduces the number of top-dimensional simplices in a
         * connected triangulation from its isomorphism signature.
         *
         * See isoSig() for more information on isomorphism signatures.
         * It will be assumed that the signature describes a triangulation of
         * dimension \a dim.
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
         * of this is that it is possible to pass an \e invalid isomorphism
         * signature and still receive a positive result.  If you need to
         * test whether a signature is valid or not, you must call fromIsoSig()
         * instead, which will examine the entire signature in full.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different dimensions \a p and \a q.
         *
         * @param sig the isomorphism signature of a <i>dim</i>-dimensional
         * triangulation.  Note that isomorphism signature are case-sensitive
         * (unlike, for example, dehydration strings for 3-manifolds).
         * @return the number of top-dimensional simplices in the first
         * connected component, or 0 if this could not be determined
         * because the given string was not a valid isomorphism signature.
         */
        static size_t isoSigComponentSize(const std::string& sig);

        /*@}*/

    protected:
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
         * @return \c true if and only if the skeleton has been calculated.
         */
        bool calculatedSkeleton() const;

        /**
         * Calculates all skeletal objects for this triangulation.
         *
         * For this parent class, calculateSkeleton() computes properties such
         * as connected components, orientability, and lower-dimensional faces.
         * Some Triangulation<dim> subclasses may track additional skeletal
         * data, in which case they should reimplement this function.  Their
         * reimplementations \e must call this parent implementation.
         *
         * You should never call this function directly; instead call
         * ensureSkeleton() instead.
         *
         * \pre No skeletal objects have been computed, and the
         * corresponding internal lists are all empty.
         *
         * \warning Any call to calculateSkeleton() must first cast down to
         * Triangulation<dim>.  You should never directly call this
         * parent implementation (unless of course you are reimplementing
         * calculateSkeleton() in a Triangulation<dim> subclass).
         */
        void calculateSkeleton();

        /**
         * Clears all properties that are managed by this base class.
         * This includes deleting all skeletal objects and emptying the
         * corresponding internal lists, as well as clearing other cached
         * properties and deallocating the corresponding memory where required.
         *
         * Note that TriangulationBase never calls this routine itself.
         * Typically clearBaseProperties() is only ever called by
         * Triangulation<dim>::clearAllProperties(), which in turn is called by
         * "atomic" routines that change the triangluation (before firing
         * packet change events), as well as the Triangulation<dim> destructor.
         */
        void clearBaseProperties();

        /**
         * Swaps all properties that are managed by this base class,
         * including skeletal data, with the given triangulation.
         *
         * Note that TriangulationBase never calls this routine itself.
         * Typically swapBaseProperties() is only ever called by
         * Triangulation<dim>::swapAllProperties(), which in turn is
         * called by swapContents().
         *
         * @param other the triangulation whose properties should be
         * swapped with this.
         */
        void swapBaseProperties(TriangulationBase<dim>& other);

        /**
         * Writes a chunk of XML containing properties of this triangulation.
         * This routine covers those properties that are managed by this base
         * class TriangulationBase and that have already been computed for this
         * triangulation.
         *
         * This routine is typically called from within
         * Triangulation<dim>::writeXMLPacketData().  The XML elements
         * that it writes are child elements of the \c packet element.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLBaseProperties(std::ostream& out) const;

    private:
        /**
         * Internal to calculateSkeleton().
         *
         * This routine calculates all codimension-1-faces.
         *
         * See calculateSkeleton() for further details.
         */
        void calculateSkeletonCodim1();

        /**
         * Internal to calculateSkeleton().
         *
         * This routine calculates all codimension-2-faces.
         *
         * See calculateSkeleton() for further details.
         */
        void calculateSkeletonCodim2();

        /**
         * Internal to calculateSkeleton().
         *
         * This routine calculates all <i>subdim</i>-faces.
         *
         * See calculateSkeleton() for further details.
         *
         * \tparam subdim the dimension of the faces to compute.
         * This must be between 0 and (\a dim - 3) inclusive.
         */
        template <int subdim>
        void calculateSkeletonSubdim();

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
         * See calculateRealBoundary() for further details.
         */
        template <int subdim>
        void calculateBoundaryFaces(BoundaryComponent<dim>* bc,
            Face<dim, dim-1>* facet);

        /**
         * Internal to isoSig().
         *
         * Constructs a candidate isomorphism signature for a single
         * component of this triangulation.  This candidate signature
         * assumes that the given simplex with the given labelling
         * of its vertices becomes simplex zero with vertices
         * 0,...,\a dim under the "canonical isomorphism".
         *
         * @param tri the triangulation under consideration.
         * @param simp the index of some top-dimensional simplex in this
         * triangulation.
         * @param vertices some ordering of the vertices of the given simplex.
         * @param relabelling if this is non-null, it will be filled with the
         * canonical isomorphism; in this case it must already have been
         * constructed for the correct number of simplices.
         * @return the candidate isomorphism signature.
         */
        std::string isoSigFrom(size_t simp, const Perm<dim+1>& vertices,
            Isomorphism<dim>* relabelling) const;

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
         * The isomorphisms that are found will be written to the given
         * output iterator.  This iterator must accept objects of type
         * Isomorphism<dim>*.  As an example, \a output might be a
         * back_insert_iterator for a std::vector<Isomorphism<dim>*>.
         *
         * The isomorphisms that are written to the given output iterator
         * will be newly created, and the caller of this routine is
         * responsible for destroying them.
         *
         * If \a firstOnly is passed as \c true, only the first
         * isomorphism found (if any) will be returned, after which the
         * routine will return immediately.  Otherwise all isomorphisms
         * will be returned.
         *
         * @param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * @param output the output iterator to which the isomorphisms
         * will be written.
         * @param completeIsomorphism \c true if isomorphisms must be
         * onto and boundary complete, or \c false if neither of these
         * restrictions should be imposed.
         * @param firstOnly \c true if only one isomorphism should be
         * returned (if any), or \c false if all isomorphisms should be
         * returned.
         * @return the total number of isomorphisms found.
         */
        template <typename OutputIterator>
        size_t findIsomorphisms(const Triangulation<dim>& other,
                OutputIterator output, bool complete, bool firstOnly) const;

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
         * @param other the triangulation in which we are searching for an
         * isomorphic copy of this triangulation.
         * @param completeIsomorphism \c true if the isomorphism must be
         * onto and boundary complete, or \c false if neither of these
         * restrictions should be imposed.
         * @return \c true if no immediate obstructions were found, or
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
         * <tt>p * adjust</tt>.
         *
         * \pre For each \a i = <i>subdim</i>+1,...,\a dim, the given
         * permutation maps \a i to itself.
         */
        template <int subdim>
        void relabelFace(Face<dim, subdim>* f, const Perm<dim + 1>& adjust);

    template <int, int, int> friend struct FaceCalculator;
    template <int, int> friend struct BoundaryComponentCalculator;
    template <int, int> friend class WeakFaceList;
    friend class regina::detail::XMLTriangulationReaderBase<dim>;
};

/*@}*/

// Inline functions for FaceListSuite

template <int dim, int subdim>
inline void FaceListSuite<dim, subdim>::deleteFaces() {
    FaceList<dim, subdim>::destroy();
    FaceListSuite<dim, subdim - 1>::deleteFaces();
}

template <int dim, int subdim>
inline void FaceListSuite<dim, subdim>::swapFaces(
        FaceListSuite<dim, subdim>& other) {
    FaceList<dim, subdim>::swap(other);
    FaceListSuite<dim, subdim - 1>::swapFaces(other);
}

template <int dim>
inline void FaceListSuite<dim, 0>::deleteFaces() {
    FaceList<dim, 0>::destroy();
}

template <int dim>
inline void FaceListSuite<dim, 0>::swapFaces(FaceListSuite<dim, 0>& other) {
    FaceList<dim, 0>::swap(other);
}

template <int dim, int subdim>
inline void FaceListSuite<dim, subdim>::fillFVector(
        std::vector<size_t>& result) const {
    FaceListSuite<dim, subdim - 1>::fillFVector(result);
    result.push_back(FaceList<dim, subdim>::size());
}

template <int dim>
inline void FaceListSuite<dim, 0>::fillFVector(
        std::vector<size_t>& result) const {
    result.push_back(FaceList<dim, 0>::size());
}

template <int dim, int subdim>
inline bool FaceListSuite<dim, subdim>::sameFVector(
        const FaceListSuite<dim, subdim>& other) const {
    return FaceListSuite<dim, subdim - 1>::sameFVector(other) &&
        (FaceList<dim, subdim>::size() == other.FaceList<dim, subdim>::size());
}

template <int dim>
inline bool FaceListSuite<dim, 0>::sameFVector(
        const FaceListSuite<dim, 0>& other) const {
    return (FaceList<dim, 0>::size() == other.FaceList<dim, 0>::size());
}

template <int dim, int subdim>
inline bool FaceListSuite<dim, subdim>::sameDegrees(
        const FaceListSuite<dim, subdim>& other) const {
    return FaceListSuite<dim, subdim - 1>::sameDegrees(other) &&
        FaceList<dim, subdim>::sameDegrees(other);
}

template <int dim>
inline bool FaceListSuite<dim, 0>::sameDegrees(
        const FaceListSuite<dim, 0>& other) const {
    return FaceList<dim, 0>::sameDegrees(other);
}

// Inline functions for TriangulationBase

template <int dim>
inline TriangulationBase<dim>::TriangulationBase() :
        calculatedSkeleton_(false) {
}

template <int dim>
inline TriangulationBase<dim>::TriangulationBase(
        const TriangulationBase<dim>& copy) : TriangulationBase(copy, true) {
}

template <int dim>
TriangulationBase<dim>::TriangulationBase(const TriangulationBase<dim>& copy,
        bool cloneProps) :
        calculatedSkeleton_(false) {
    // We don't fire a change event here since this is a constructor.
    // There should be nobody listening on events yet.
    // Likewise, we don't clearAllProperties() since no properties
    // will have been computed yet.

    SimplexIterator me, you;
    for (you = copy.simplices_.begin(); you != copy.simplices_.end(); ++you)
        simplices_.push_back(new Simplex<dim>((*you)->description(),
            static_cast<Triangulation<dim>*>(this)));

    // Copy the internal simplex data, including gluings.
    int f;
    for (me = simplices_.begin(), you = copy.simplices_.begin();
            me != simplices_.end(); ++me, ++you) {
        for (f = 0; f <= dim; ++f) {
            if ((*you)->adj_[f]) {
                (*me)->adj_[f] = simplices_[(*you)->adj_[f]->index()];
                (*me)->gluing_[f] = (*you)->gluing_[f];
            } else
                (*me)->adj_[f] = 0;
        }
    }

    // Clone properties:
    if (cloneProps) {
        if (copy.fundGroup_.known())
            fundGroup_ = new GroupPresentation(*(copy.fundGroup_.value()));
        if (copy.H1_.known())
            H1_ = new AbelianGroup(*(copy.H1_.value()));
    }
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
inline const std::vector<Simplex<dim>*>& TriangulationBase<dim>::simplices()
        const {
    return (const std::vector<Simplex<dim>*>&)(simplices_);
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
    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));
    Simplex<dim>* s = new Simplex<dim>(static_cast<Triangulation<dim>*>(this));
    simplices_.push_back(s);
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    return s;
}

template <int dim>
Simplex<dim>* TriangulationBase<dim>::newSimplex(const std::string& desc) {
    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));
    Simplex<dim>* s = new Simplex<dim>(desc,
        static_cast<Triangulation<dim>*>(this));
    simplices_.push_back(s);
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    return s;
}

template <int dim>
inline void TriangulationBase<dim>::removeSimplex(Simplex<dim>* simplex) {
    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

    simplex->isolate();
    simplices_.erase(simplices_.begin() + simplex->index());
    delete simplex;

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
inline void TriangulationBase<dim>::removeSimplexAt(size_t index) {
    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

    Simplex<dim>* simplex = simplices_[index];
    simplex->isolate();
    simplices_.erase(simplices_.begin() + index);
    delete simplex;

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
inline void TriangulationBase<dim>::removeAllSimplices() {
    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

    for (auto s : simplices_)
        delete s;
    simplices_.clear();

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::swapContents(Triangulation<dim>& other) {
    if (&other == this)
        return;

    typename Triangulation<dim>::ChangeEventSpan span1(
        static_cast<Triangulation<dim>*>(this));
    typename Triangulation<dim>::ChangeEventSpan span2(&other);

    simplices_.swap(other.simplices_);

    for (auto s : simplices_)
        s->tri_ = static_cast<Triangulation<dim>*>(this);
    for (auto s : other.simplices_)
        s->tri_ = &other;

    static_cast<Triangulation<dim>*>(this)->swapAllProperties(other);
}

template <int dim>
void TriangulationBase<dim>::moveContentsTo(Triangulation<dim>& dest) {
    typename Triangulation<dim>::ChangeEventSpan span1(
        static_cast<Triangulation<dim>*>(this));
    typename Triangulation<dim>::ChangeEventSpan span2(&dest);

    SimplexIterator it;
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        // This is an abuse of MarkedVector, since for a brief moment
        // each triangle belongs to both vectors simplices_ and dest.simplices_.
        // However, the subsequent clear() operation does not touch the
        // markings (indices), and so we end up with the correct result
        // (i.e., the markings are correct for dest).
        (*it)->tri_ = &dest;
        dest.simplices_.push_back(*it);
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
    ensureSkeleton();
    return FaceList<dim, subdim>::size();
}

template <int dim>
inline std::vector<size_t> TriangulationBase<dim>::fVector() const {
    ensureSkeleton();

    std::vector<size_t> ans;
    FaceListSuite<dim, dim - 1>::fillFVector(ans);
    ans.push_back(size());
    return ans;
}

template <int dim>
inline const std::vector<Component<dim>*>& TriangulationBase<dim>::components()
        const {
    ensureSkeleton();
    return (const std::vector<Component<dim>*>&)(components_);
}

template <int dim>
inline const std::vector<BoundaryComponent<dim>*>& TriangulationBase<dim>::
        boundaryComponents() const {
    ensureSkeleton();
    return (const std::vector<BoundaryComponent<dim>*>&)(boundaryComponents_);
}

template <int dim>
template <int subdim>
inline const FaceList<dim, subdim>& TriangulationBase<dim>::faces() const {
    ensureSkeleton();
    return *this;
}

template <int dim>
template <int subdim, typename Iterator>
inline void TriangulationBase<dim>::reorderFaces(Iterator begin, Iterator end) {
    FaceList<dim, subdim>::template reorderFaces<Iterator>(begin, end);
}

template <int dim>
template <int subdim>
inline void TriangulationBase<dim>::relabelFace(Face<dim, subdim>* f,
        const Perm<dim + 1>& adjust) {
    for (const auto& emb : *f)
        emb.simplex()->SimplexFaces<dim, subdim>::mapping_[emb.face()] =
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
    return FaceList<dim, subdim>::operator [](index);
}

template <int dim>
inline bool TriangulationBase<dim>::isEmpty() const {
    return simplices_.empty();
}

template <int dim>
inline bool TriangulationBase<dim>::isValid() const {
    ensureSkeleton();
    return valid_;
}

template <int dim>
inline bool TriangulationBase<dim>::hasBoundaryFacets() const {
    ensureSkeleton();
    return (2 * countFaces<dim - 1>() > (dim + 1) * simplices_.size());
}

template <int dim>
inline size_t TriangulationBase<dim>::countBoundaryFacets() const {
    ensureSkeleton();
    return 2 * countFaces<dim - 1>() - (dim + 1) * simplices_.size();
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
bool TriangulationBase<dim>::isIdenticalTo(const Triangulation<dim>& other)
        const {
    if (simplices_.size() != other.simplices_.size())
        return false;

    SimplexIterator me, you;
    int f;
    for (me = simplices_.begin(), you = other.simplices_.begin();
            me != simplices_.end(); ++me, ++you) {
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
inline std::unique_ptr<Isomorphism<dim>> TriangulationBase<dim>::isIsomorphicTo(
        const Triangulation<dim>& other) const {
    Isomorphism<dim>* results[1];
    if (findIsomorphisms(other, results, true, true))
        return std::unique_ptr<Isomorphism<dim>>(results[0]);
    else
        return nullptr;
}

template <int dim>
inline std::unique_ptr<Isomorphism<dim>> TriangulationBase<dim>::isContainedIn(
        const Triangulation<dim>& other) const {
    Isomorphism<dim>* results[1];
    if (findIsomorphisms(other, results, false, true))
        return std::unique_ptr<Isomorphism<dim>>(results[0]);
    else
        return nullptr;
}

template <int dim>
template <typename OutputIterator>
inline size_t TriangulationBase<dim>::findAllIsomorphisms(
        const Triangulation<dim>& other, OutputIterator output) const {
    return findIsomorphisms(other, output, true, false);
}

template <int dim>
template <typename OutputIterator>
inline size_t TriangulationBase<dim>::findAllSubcomplexesIn(
        const Triangulation<dim>& other, OutputIterator output) const {
    return findIsomorphisms(other, output, false, false);
}

template <int dim>
void TriangulationBase<dim>::insertTriangulation(
        const Triangulation<dim>& source) {
    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

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
                me->adj_[f] = 0;
        }
    }

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::insertConstruction(size_t nSimplices,
        const int adjacencies[][dim+1], const int gluings[][dim+1][dim+1]) {
    if (nSimplices == 0)
        return;

    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

    size_t nOrig = size();

    // Each time we loop through simplices we must only make nSimplices
    // iterations.  This ensures that the routine behaves correctly even
    // if source is this triangulation.
    size_t i;
    for (i = 0; i < nSimplices; ++i)
        simplices_.push_back(new Simplex<dim>(
            static_cast<Triangulation<dim>*>(this)));

    Simplex<dim>* s;
    int f;
    for (i = 0; i < nSimplices; ++i) {
        s = simplices_[nOrig + i];
        for (f = 0; f <= dim; ++f) {
            if (adjacencies[i][f] >= 0) {
                s->adj_[f] = simplices_[nOrig + adjacencies[i][f]];
                s->gluing_[f] = Perm<dim+1>(gluings[i][f]);
            } else
                s->adj_[f] = 0;
        }
    }

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
std::string TriangulationBase<dim>::dumpConstruction() const {
    std::ostringstream ans;
    ans <<
"/**\n";
    if (! static_cast<const Triangulation<dim>*>(this)->label().empty()) {
        ans <<
" * " << dim << "-dimensional triangulation: "
      << static_cast<const Triangulation<dim>*>(this)->label() << "\n";
    } else {
        ans <<
" * " << dim << "-dimensional triangulation:\n";
    }
    ans <<
" * Code automatically generated by dumpConstruction().\n"
" */\n"
"\n";

    if (simplices_.empty()) {
        ans <<
"/* This triangulation is empty.  No code is being generated. */\n";
        return ans.str();
    }

    ans <<
"/**\n"
" * The following arrays describe the gluings between simplices.\n"
" */\n"
"\n";

    size_t nSimplices = simplices_.size();
    Simplex<dim>* s;
    Perm<dim+1> perm;
    size_t p;
    int f, i;

    ans << "const int adjacencies[" << nSimplices << "][" << (dim+1)
        << "] = {\n";
    for (p = 0; p < nSimplices; ++p) {
        s = simplices_[p];

        ans << "    { ";
        for (f = 0; f <= dim; ++f) {
            if (s->adjacentSimplex(f)) {
                ans << s->adjacentSimplex(f)->index();
            } else
                ans << "-1";

            if (f < dim)
                ans << ", ";
            else if (p != nSimplices - 1)
                ans << "},\n";
            else
                ans << "}\n";
        }
    }
    ans << "};\n\n";

    ans << "const int gluings[" << nSimplices << "][" << (dim+1)
        << "][" << (dim+1) << "] = {\n";
    for (p = 0; p < nSimplices; ++p) {
        s = simplices_[p];

        ans << "    { ";
        for (f = 0; f <= dim; ++f) {
            if (s->adjacentSimplex(f)) {
                perm = s->adjacentGluing(f);
                ans << "{ ";
                for (i = 0; i <= dim; ++i) {
                    ans << perm[i];
                    if (i < dim)
                        ans << ", ";
                    else
                        ans << " }";
                }
            } else {
                ans << "{ ";
                for (i = 0; i < dim; ++i)
                    ans << "0, ";
                ans << "0 }";
            }

            if (f < dim)
                ans << ", ";
            else if (p != nSimplices - 1)
                ans << " },\n";
            else
                ans << " }\n";
        }
    }
    ans << "};\n\n";

    ans <<
"/**\n"
" * The following code constructs a " << dim << "-dimensional triangulation\n"
" * based on the information stored in the arrays above.\n"
" */\n"
"\n"
"Triangulation<" << dim << "> tri;\n"
"tri.insertConstruction(" << nSimplices << ", adjacencies, gluings);\n"
"\n";

    return ans.str();
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
    return EulerCalculator<dim, 0>::compute(*this);
}

template <int dim>
void TriangulationBase<dim>::orient() {
    ensureSkeleton();

    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

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
void TriangulationBase<dim>::makeDoubleCover() {
    size_t sheetSize = simplices_.size();
    if (sheetSize == 0)
        return;

    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

    // Create a second sheet of simplices.
    Simplex<dim>** upper = new Simplex<dim>*[sheetSize];
    size_t i;
    for (i = 0; i < sheetSize; i++)
        upper[i] = newSimplex(simplices_[i]->description());

    // Reset each simplex orientation.
    SimplexIterator sit = simplices_.begin();
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
    size_t* queue = new size_t[sheetSize];
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
void TriangulationBase<dim>::barycentricSubdivision() {
    // IMPORTANT: If the labelling of new simplices ever changes, then the
    // 3-dimensional drillEdge() code must be rewritten as well (since it
    // relies on the specific labelling scheme that we use here).

    size_t nOld = simplices_.size();
    if (nOld == 0)
        return;

    Triangulation<dim> staging;
    typename Triangulation<dim>::ChangeEventSpan span(&staging);

    static_assert(standardDim(dim),
        "barycentricSubdivision() may only be used in standard dimensions.");

    Simplex<dim>** newSimp = new Simplex<dim>*[nOld * Perm<dim+1>::nPerms];
    Simplex<dim>* oldSimp;

    // A top-dimensional simplex in the subdivision is uniquely defined
    // by a permutation p on (dim+1) elements.
    //
    // As described in the documentation for barycentricSubdivision(),
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
    int permIdx;
    Perm<dim+1> perm, glue;
    int i;
    for (simp=0; simp < nOld; ++simp)
        for (permIdx = 0; permIdx < Perm<dim+1>::nPerms; ++permIdx) {
            perm = Perm<dim+1>::atIndex(permIdx);

            // Internal gluings within the old simplex:
            for (i = 0; i < dim; ++i)
                newSimp[Perm<dim+1>::nPerms * simp + permIdx]->join(perm[i],
                    newSimp[Perm<dim+1>::nPerms * simp +
                        (perm * Perm<dim+1>(i, i+1)).index()],
                    Perm<dim+1>(perm[i], perm[i+1]));

            // Adjacent gluings to the adjacent simplex:
            oldSimp = simplex(simp);
            if (! oldSimp->adjacentSimplex(perm[dim]))
                continue; // This hits a boundary facet.
            if (newSimp[Perm<dim+1>::nPerms * simp + permIdx]->adjacentSimplex(
                    perm[dim]))
                continue; // We've already done this gluing from the other side.

            glue = oldSimp->adjacentGluing(perm[dim]);
            newSimp[Perm<dim+1>::nPerms * simp + permIdx]->join(perm[dim],
                newSimp[Perm<dim+1>::nPerms * oldSimp->adjacentSimplex(
                    perm[dim])->index() + (glue * perm).index()],
                glue);
        }

    // Delete the existing simplices and put in the new ones.
    swapContents(staging);
    delete[] newSimp;
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

    Simplex<dim>** bdry = new Simplex<dim>*[nFaces];
    Perm<dim + 1>* bdryPerm = new Perm<dim + 1>[nFaces];
    Simplex<dim>** cone = new Simplex<dim>*[nFaces];

    Triangulation<dim> staging;
    typename Triangulation<dim>::ChangeEventSpan span1(&staging);

    for (Face<dim, dim - 1>* f : faces<dim - 1>()) {
        if (f->degree() > 1) {
            // Not a boundary facet.
            bdry[f->index()] = cone[f->index()] = 0;
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
    typename Triangulation<dim>::ChangeEventSpan span2(
        static_cast<Triangulation<dim>*>(this));

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
size_t TriangulationBase<dim>::splitIntoComponents(Packet* componentParent,
        bool setLabels) {
    // Knock off the empty triangulation first.
    if (simplices_.empty())
        return 0;

    if (! componentParent)
        componentParent = static_cast<Triangulation<dim>*>(this);

    // Create the new component triangulations.
    // Note that the following line forces a skeletal recalculation.
    size_t nComp = countComponents();

    // Initialise the component triangulations.
    Triangulation<dim>** newTris = new Triangulation<dim>*[nComp];
    size_t whichComp;
    for (whichComp = 0; whichComp < nComp; ++whichComp)
        newTris[whichComp] = new Triangulation<dim>();

    // Clone the simplices, sorting them into the new components.
    Simplex<dim>** newSimp = new Simplex<dim>*[size()];
    Simplex<dim> *simp, *adj;
    size_t simpPos, adjPos;
    Perm<dim + 1> adjPerm;
    int facet;

    for (simpPos = 0; simpPos < size(); ++simpPos)
        newSimp[simpPos] = newTris[simplices_[simpPos]->component()->index()]->
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

    // Insert the component triangulations into the packet tree and clean up.
    for (whichComp = 0; whichComp < nComp; ++whichComp) {
        componentParent->insertChildLast(newTris[whichComp]);

        if (setLabels) {
            std::ostringstream label;
            label << "Component #" << (whichComp + 1);
            newTris[whichComp]->setLabel(
                static_cast<Triangulation<dim>*>(this)->
                adornedLabel(label.str()));
        }
    }

    delete[] newSimp;
    delete[] newTris;

    return whichComp;
}

template <int dim>
inline void TriangulationBase<dim>::simplifiedFundamentalGroup(
        GroupPresentation* newGroup) {
    fundGroup_ = newGroup;
}

template <int dim>
inline const AbelianGroup& TriangulationBase<dim>::homologyH1() const {
    return homology();
}

template <int dim>
const AbelianGroup& TriangulationBase<dim>::homology() const {
    if (H1_.known())
        return *H1_.value();

    if (isEmpty())
        return *(H1_ = new AbelianGroup());

    // Calculate a maximal forest in the dual 1-skeleton.
    ensureSkeleton();

    // Build a presentation matrix.
    // Each non-boundary not-in-forest (dim-1)-face is a generator.
    // Each non-boundary (dim-2)-face is a relation.
    long nBdryRidges = 0;
    for (auto bc : boundaryComponents())
        nBdryRidges += bc->countRidges();

    // Cast away all unsignedness in case we run into problems subtracting.
    long nGens = static_cast<long>(countFaces<dim-1>())
        - static_cast<long>(countBoundaryFacets())
        + static_cast<long>(countComponents())
        - static_cast<long>(size());
    long nRels = static_cast<long>(countFaces<dim-2>()) - nBdryRidges;

    MatrixInt pres(nRels, nGens);

    // Find out which (dim-1)-face corresponds to which generator.
    long* genIndex = new long[countFaces<dim-1>()];
    long i = 0;
    for (Face<dim, dim-1>* f : faces<dim-1>())
        if (! (f->isBoundary() || f->inMaximalForest()))
            genIndex[f->index()] = i++;

    // Run through each (dim-2)-face and put the relations into the matrix.
    Simplex<dim>* simp;
    int facet;
    Face<dim, dim-1>* gen;
    i = 0;
    for (Face<dim, dim-2>* f : faces<dim-2>()) {
        if (! f->isBoundary()) {
            // Put in the relation corresponding to this (dim-2)-face.
            for (auto& emb : *f) {
                simp = emb.simplex();
                facet = emb.vertices()[dim-1];
                gen = simp->template face<dim-1>(facet);
                if (! gen->inMaximalForest()) {
                    // We define the "direction" for this dual edge to point
                    // from embedding gen->front() to embedding gen->back().
                    //
                    // Test whether we are traversing this dual edge forwards
                    // or backwards as we walk around the (dim-2)-face f.
                    if ((gen->front().simplex() == simp) &&
                            (gen->front().face() == facet))
                        pres.entry(i, genIndex[gen->index()]) += 1;
                    else
                        pres.entry(i, genIndex[gen->index()]) -= 1;
                }
            }
            ++i;
        }
    }

    delete[] genIndex;

    // Build the group from the presentation matrix and tidy up.
    AbelianGroup* ans = new AbelianGroup();
    ans->addGroup(pres);
    return *(H1_ = ans);
}

template <int dim>
const GroupPresentation& TriangulationBase<dim>::fundamentalGroup() const {
    if (fundGroup_.known())
        return *fundGroup_.value();

    GroupPresentation* ans = new GroupPresentation();

    if (isEmpty())
        return *(fundGroup_ = ans);

    // Calculate a maximal forest in the dual 1-skeleton.
    ensureSkeleton();

    // Each non-boundary not-in-forest (dim-1)-face is a generator.
    // Each non-boundary (dim-2)-face is a relation.

    // Cast away all unsignedness in case we run into problems subtracting.
    long nGens = static_cast<long>(countFaces<dim-1>())
        - static_cast<long>(countBoundaryFacets())
        + static_cast<long>(countComponents())
        - static_cast<long>(size());

    // Insert the generators.
    ans->addGenerator(nGens);

    // Find out which (dim-1)-face corresponds to which generator.
    long* genIndex = new long[countFaces<dim-1>()];
    long i = 0;
    for (Face<dim, dim-1>* f : faces<dim-1>())
        if (! (f->isBoundary() || f->inMaximalForest()))
            genIndex[f->index()] = i++;

    // Run through each (dim-2)-face and insert the corresponding relations.
    Simplex<dim>* simp;
    int facet;
    Face<dim, dim-1>* gen;
    GroupExpression* rel;
    for (Face<dim, dim-2>* f : faces<dim-2>()) {
        if (! f->isBoundary()) {
            // Put in the relation corresponding to this triangle.
            rel = new GroupExpression();
            for (auto& emb : *f) {
                simp = emb.simplex();
                facet = emb.vertices()[dim-1];
                gen = simp->template face<dim-1>(facet);
                if (! gen->inMaximalForest()) {
                    // We define the "direction" for this dual edge to point
                    // from embedding gen->front() to embedding gen->back().
                    //
                    // Test whether we are traversing this dual edge forwards or
                    // backwards as we walk around the (dim-2)-face f.
                    if ((gen->front().simplex() == simp) &&
                            (gen->front().face() == facet))
                        rel->addTermLast(genIndex[gen->index()], 1);
                    else
                        rel->addTermLast(genIndex[gen->index()], -1);
                }
            }
            ans->addRelation(rel);
        }
    }

    // Tidy up.
    delete[] genIndex;
    ans->intelligentSimplify();

    return *(fundGroup_ = ans);
}

template <int dim>
void TriangulationBase<dim>::writeXMLBaseProperties(std::ostream& out) const {
    if (fundGroup_.known()) {
        out << "  <fundgroup>\n";
        fundGroup_.value()->writeXMLData(out);
        out << "  </fundgroup>\n";
    }
    if (H1_.known()) {
        out << "  <H1>";
        H1_.value()->writeXMLData(out);
        out << "</H1>\n";
    }
}

} } // namespace regina::detail

#include "triangulation/detail/canonical-impl.h"

#endif
