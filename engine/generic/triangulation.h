
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

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "regina-core.h"
#include "output.h"
#include "generic/simplex.h"
#include "maths/nperm.h"
#include "utilities/nmarkedvector.h"

namespace regina {

template <int> class Isomorphism;
template <int> class Triangulation;

/**
 * \weakgroup generic
 * @{
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
 * with the output code inherited from NPacket.  Specifically:
 *
 * - for those dimensions where Triangulation<dim> derives from NPacket, the
 *   output routines are inherited from NPacket (which derives from Output).
 *
 * - for other dimensions, Triangulation<dim> derives from Output directly.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * Triangulation<dim> is.
 *
 * \tparam dim the dimension of the triangulation.  This must be at least 2.
 */
template <int dim>
class REGINA_API TriangulationBase : public boost::noncopyable {
    static_assert(dim >= 2, "Triangulation requires dimension >= 2.");

    public:
        typedef typename std::vector<Simplex<dim>*>::const_iterator
                SimplexIterator;
            /**< Used to iterate through top-dimensional simplices. */
        typedef std::vector<Component<dim>*>::const_iterator ComponentIterator;
            /**< Used to iterate through connected components. */

    protected:
        NMarkedVector<Simplex<dim>> simplices_;
            /**< The top-dimensional simplices that form the triangulation. */

    private:
        mutable bool calculatedSkeleton_;
            /**< Has the skeleton been calculated?  This is only done
                 "on demand", when a skeletal property is first queried. */
        mutable NMarkedVector<Dim2Component> components_;
            /**< The connected components that form the triangulation.
                 This list is only filled if/when the skeleton of the
                 triangulation is computed. */
        mutable bool orientable_;
            /**< Is the triangulation orientable?  This property is only set
                 if/when the skeleton of the triangulation is computed. */

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
         * @param copy the triangulation to copy.
         */
        TriangulationBase(const TriangulationBase& copy);
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
         * Deprecated routine that returns the number of top-dimensional
         * simplices in the triangulation.
         *
         * \deprecated Simply call size() instead.
         *
         * @return the number of top-dimensional simplices.
         */
        size_t getNumberOfSimplices() const;
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
         * Deprecated routine that returns all of the top-dimensional
         * simplices in the triangulation.
         *
         * \deprecated Simply call simplices() instead.
         *
         * See simplices() for further details.
         */
        const std::vector<Simplex<dim>*>& getSimplices() const;
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
         * Deprecated routine that returns the top-dimensional simplex at
         * the given index in the triangulation.
         *
         * \deprecated Simply call simplex() instead.
         *
         * See simplex() for further details.
         */
        Simplex<dim>* getSimplex(size_t index);
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
         * Deprecated routine that returns the top-dimensional simplex at
         * the given index in the triangulation.
         *
         * \deprecated Simply call simplex() instead.
         *
         * See simplex() for further details.
         */
        const Simplex<dim>* getSimplex(size_t index) const;
        /**
         * Returns the index of the given top-dimensional simplex in the
         * triangulation.
         *
         * Note that indexing may change when a simplex is added to or
         * removed from the triangulation.
         *
         * \pre The given simplex is contained in this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.  If you are passing the result of some other
         * routine that \e might return null (such as
         * Simplex<dim>::adjacentSimplex), you should explicitly
         * test for null beforehand.
         *
         * @param simplex specifies which simplex to find in the triangulation.
         * @return the index of the specified simplex; this will be an
         * integer between 0 and size()-1 inclusive.
         */
        size_t simplexIndex(const Simplex<dim>* simplex) const;
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
         *
         * Any pointers or references to Simplex<dim> objects will remain valid.
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
         * Deprecated routine that returns the number of connected components
         * in this triangulation.
         *
         * \deprecated Simply call countComponents() instead.
         *
         * See countComponents() for further details.
         */
        size_t getNumberOfComponents() const;

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
         * Deprecated routine that returns all connected components
         * of this triangulation.
         *
         * \deprecated Simply call components() instead.
         *
         * See components() for further details.
         */
        const std::vector<Component<dim>*>& getComponents() const;

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
         * Deprecated routine that returns the requested connected component
         * of this triangulation.
         *
         * \deprecated Simply call component() instead.
         *
         * See component() for further details.
         */
        Component<dim>* getComponent(size_t index) const;

        /**
         * Returns the index of the given connected component in this
         * triangulation.
         *
         * \pre The given component belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param component specifies which component to find in the
         * triangulation.
         * @return the index of the specified component; this will be an
         * integer between 0 and countComponents()-1 inclusive.
         */
        size_t componentIndex(const Component<dim>* component) const;

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
         * as seen in Simplex<dim>::getDescription(); these may still differ.
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
#if 0
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
         * triangulation is <tt>O(n^2 log^2 n)</tt>.
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
#endif
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
#if 0
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
        static Triangulation* fromIsoSig(const std::string& sig);

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
#endif
        /*@}*/

    protected:
        /**
         * Ensures that all "on demand" skeletal objects have been calculated.
         */
        void ensureSkeleton();

        /**
         * Deallocates all skeletal objects that are managed by this
         * triangulation, and empties all corresponding internal lists.
         *
         * The next time a user queries some skeletal property, the
         * skeleton will be recalculated.
         *
         * Triangulation<dim> subclasses should reimplement this if they
         * track additional skeletal data, but they \e must call this
         * parent implementation.
         *
         * Note that TriangulationBase never calls this routine itself.
         * Typically deleteSkeleton() will be called by
         * Triangulation<dim>::clearAllProperties, which in turn is
         * called by the Triangulation<dim> destructor.
         */
        void deleteSkeleton();

    private:
#if 0
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
        static std::string isoSigFrom(const Triangulation& tri, unsigned simp,
            const NPerm<dim+1>& vertices, Isomorphism<dim>* relabelling);
#endif
};

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
 * For dimensions 2 and 3, this template is specialised and offers
 * \e much more functionality.  In order to use these specialised
 * classes, you will need to include the corresponding headers
 * (dim2/dim2triangulation.h for \a dim = 2, or triangulation/ntriangulation.h
 * for \a dim = 3).  For convenience, there are typedefs available for
 * these specialised classes (Dim2Triangulation and NTriangulation
 * respectively).
 *
 * \ifacespython Python does not support templates.  For \a dim = 2 and 3,
 * this class is available in Python under the names Triangulation2 and
 * Triangulation3 respectively (as well as the typedefs mentioned above).
 * Higher-dimensional classes are not available in Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class REGINA_API Triangulation :
        public TriangulationBase<dim>,
        public Output<Triangulation<dim>> {
    protected:
        using TriangulationBase<dim>::simplices_;

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
         */
        class ChangeEventSpan {
            public:
                /**
                 * Constructor that does nothing.
                 * The argument is ignored.
                 */
                inline ChangeEventSpan(TriangulationBase<dim>*) {
                }
        };

    friend class SimplexBase<dim>;
    friend class TriangulationBase<dim>;
};

// Note that some of our triangulation classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Triangulation<2>;
template <> class Triangulation<3>;

/*@}*/

// Inline functions for TriangulationBase

template <int dim>
inline TriangulationBase<dim>::TriangulationBase() :
        calculatedSkeleton_(false) {
}

template <int dim>
TriangulationBase<dim>::TriangulationBase(const TriangulationBase<dim>& copy) :
        calculatedSkeleton_(false) {
    // We don't fire a change event here since this is a constructor.
    // There should be nobody listening on events yet.
    // Likewise, we don't clearAllProperties() since no properties
    // will have been computed yet.

    SimplexIterator me, you;
    for (you = copy.simplices_.begin(); you != copy.simplices_.end(); ++you)
        simplices_.push_back(new Simplex<dim>((*you)->getDescription(),
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
}

template <int dim>
inline TriangulationBase<dim>::~TriangulationBase() {
    for (auto it = simplices_.begin(); it != simplices_.end(); ++it)
        delete *it;
}

template <int dim>
inline size_t TriangulationBase<dim>::size() const {
    return simplices_.size();
}

template <int dim>
inline size_t TriangulationBase<dim>::getNumberOfSimplices() const {
    return simplices_.size();
}

template <int dim>
inline const std::vector<Simplex<dim>*>& TriangulationBase<dim>::simplices()
        const {
    return (const std::vector<Simplex<dim>*>&)(simplices_);
}

template <int dim>
inline const std::vector<Simplex<dim>*>& TriangulationBase<dim>::getSimplices()
        const {
    return (const std::vector<Simplex<dim>*>&)(simplices_);
}

template <int dim>
inline Simplex<dim>* TriangulationBase<dim>::simplex(size_t index) {
    return simplices_[index];
}

template <int dim>
inline Simplex<dim>* TriangulationBase<dim>::getSimplex(size_t index) {
    return simplices_[index];
}

template <int dim>
inline const Simplex<dim>* TriangulationBase<dim>::simplex(size_t index) const {
    return simplices_[index];
}

template <int dim>
inline const Simplex<dim>* TriangulationBase<dim>::getSimplex(size_t index)
        const {
    return simplices_[index];
}

template <int dim>
inline size_t TriangulationBase<dim>::simplexIndex(const Simplex<dim>* simplex)
        const {
    return simplex->markedIndex();
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
    simplices_.erase(simplices_.begin() + simplex->markedIndex());
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

    for (auto it = simplices_.begin(); it != simplices_.end(); ++it)
        delete *it;
    simplices_.clear();

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::swapContents(Triangulation<dim>& other) {
    typename Triangulation<dim>::ChangeEventSpan span1(
        static_cast<Triangulation<dim>*>(this));
    typename Triangulation<dim>::ChangeEventSpan span2(&other);

    simplices_.swap(other.simplices_);

    SimplexIterator it;
    for (it = simplices_.begin(); it != simplices_.end(); ++it)
        (*it)->tri_ = static_cast<Triangulation<dim>*>(this);
    for (it = other.simplices_.begin(); it != other.simplices_.end(); ++it)
        (*it)->tri_ = &other;

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    static_cast<Triangulation<dim>&>(other).clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::moveContentsTo(Triangulation<dim>& dest) {
    typename Triangulation<dim>::ChangeEventSpan span1(
        static_cast<Triangulation<dim>*>(this));
    typename Triangulation<dim>::ChangeEventSpan span2(&dest);

    SimplexIterator it;
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        // This is an abuse of NMarkedVector, since for a brief moment
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
inline size_t TriangulationBase<dim>::getNumberOfComponents() const {
    ensureSkeleton();
    return components_.size();
}

template <int dim>
inline const std::vector<Component<dim>*>& TriangulationBase<dim>::components()
        const {
    ensureSkeleton();
    return (const std::vector<Component<dim>*>&)(components_);
}

template <int dim>
inline const std::vector<Component<dim>*>&
        TriangulationBase<dim>::getComponents() const {
    ensureSkeleton();
    return (const std::vector<Component<dim>*>&)(components_);
}

template <int dim>
inline Component<dim>* TriangulationBase<dim>::component(size_t index) const {
    ensureSkeleton();
    return components_[index];
}

template <int dim>
inline Component<dim>* TriangulationBase<dim>::getComponent(size_t index)
        const {
    ensureSkeleton();
    return components_[index];
}

template <int dim>
inline size_t TriangulationBase<dim>::componentIndex(
        const Component<dim>* component) const {
    return component->markedIndex();
}

template <int dim>
inline bool TriangulationBase<dim>::isEmpty() const {
    return simplices_.empty();
}

template <int dim>
inline bool TriangulationBase<dim>::hasBoundaryFacets() const {
    for (auto it = simplices_.begin(); it != simplices_.end(); ++it)
        if ((*it)->hasBoundary())
            return true;
    return false;
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
void TriangulationBase<dim>::insertTriangulation(
        const Triangulation<dim>& source) {
    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

    size_t nOrig = getNumberOfSimplices();
    size_t nSource = source.getNumberOfSimplices();

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

    size_t nOrig = getNumberOfSimplices();

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
                s->gluing_[f] = NPerm<dim+1>(gluings[i][f]);
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
#if 0
TODO: output packet labels if we derive from NPacket
    if (! getPacketLabel().empty())
        ans <<
" * " << dim << "-dimensional triangulation: " << getPacketLabel() << "\n";
#endif
    ans <<
" * " << dim << "-dimensional triangulation:\n";
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
    NPerm<dim+1> perm;
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
inline void TriangulationBase<dim>::ensureSkeleton() {
    if (! calculatedSkeleton_)
        calculateSkeleton();
}

template <int dim>
inline void TriangluationBase<dim>::deleteSkeleton() {
    for (auto it = components_.begin(); it != components_.end(); ++it)
        delete *it;

    components_.clear();

    calculatedSkeleton_ = false;
}

// Inline functions for Triangulation

template <int dim>
inline Triangulation<dim>::Triangulation() : TriangulationBase<dim>() {
}

template <int dim>
inline Triangulation<dim>::Triangulation(const Triangulation& copy) :
        TriangulationBase<dim>(copy) {
}

template <int dim>
inline void Triangulation<dim>::clearAllProperties() {
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
                out << std::setw(4) << adj->markedIndex() << " (";
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

} // namespace regina

#endif
