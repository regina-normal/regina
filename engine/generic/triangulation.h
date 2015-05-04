
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

#include <memory>
#include <string>
#include <vector>
#include "regina-core.h"
#include "output.h"
#include "maths/nperm.h"
#include "utilities/nmarkedvector.h"

namespace regina {

template <int> class Isomorphism;
template <int> class SimplexBase;
template <int> class Simplex;

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
 * \ifacespython This base class is not present, but the "end user" class
 * Triangulation<dim> is.
 *
 * \tparam dim the dimension of the triangulation.  This must be at least 2.
 */
template <int dim>
class REGINA_API TriangulationBase :
        public Output<IsomorphismBase<dim>>,
        public boost::noncopyable {
    static_assert(dim >= 2, "Triangulation requires dimension >= 2.");

    public:
        typedef std::vector<Simplex<dim>*>::const_iterator SimplexIterator;
            /**< Used to iterate through top-dimensional simplices. */

    protected:
        NMarkedVector<Simplex<dim>> simplices_;
            /**< The top-dimensional simplices that form the triangulation. */

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
        /**
         * Destroys this triangulation.
         *
         * The simplices within this triangulation will also be destroyed.
         */
        ~Triangulation();

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
        unsigned long size() const;
        /**
         * Deprecated routine that returns the number of top-dimensional
         * simplices in the triangulation.
         *
         * \deprecated Simply call size() instead.
         *
         * @return the number of top-dimensional simplices.
         */
        unsigned long getNumberOfSimplices() const;
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
        Simplex<dim>* simplex(unsigned long index);
        /**
         * Deprecated routine that returns the top-dimensional simplex at
         * the given index in the triangulation.
         *
         * \deprecated Simply call simplex() instead.
         *
         * See simplex() for further details.
         */
        Simplex<dim>* getSimplex(unsigned long index);
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
        const Simplex<dim>* simplex(unsigned long index) const;
        /**
         * Deprecated routine that returns the top-dimensional simplex at
         * the given index in the triangulation.
         *
         * \deprecated Simply call simplex() instead.
         *
         * See simplex() for further details.
         */
        const Simplex<dim>* getSimplex(unsigned long index) const;
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
         * Simplex<dim>::adjacentTriangle), you should explicitly
         * test for null beforehand.
         *
         * @param simplex specifies which simplex to find in the triangulation.
         * @return the index of the specified simplex; this will be an
         * integer between 0 and size()-1 inclusive.
         */
        long simplexIndex(const Simplex<dim>* simplex) const;
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
        void removeSimplexAt(unsigned long index);
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
        void swapContents(Triangulation& other);
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
         * @param dest the triangulation into which simplices should be moved.
         */
        void moveContentsTo(Triangulation& dest);

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

        /*@}*/
        /**
         * \name Isomorphism Testing
         */
        /*@{*/

// TODO: HERE
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
         * To test for the less strict combinatorial isomorphism (which
         * allows relabelling of the top-dimensional simplices and their
         * vertices), see isIsomorphicTo() instead.
         *
         * This test does \e not examine the textual simplex descriptions,
         * as seen in Simplex<dim>::getDescription(); these may still differ.
         * It also does not test the numbering of vertices, edges and so on,
         * as used by getVertex(), getEdge() and so on;
         * although at the time of writing these will always be
         * numbered the same for identical triangulations, it is
         * conceivable that in future versions of Regina there may
         * be situations in which identical triangulations can acquire
         * different numberings for vertices, edges, etc.
         *
         * @param other the triangulation to compare with this one.
         * @return \c true if and only if the two triangulations are
         * combinatorially identical.
         */
        bool isIdenticalTo(const typename DimTraits<dim>::Triangulation& other)
            const;

        /**
         * Determines if this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * Specifically, this routine determines if there is a
         * one-to-one and onto boundary complete combinatorial
         * isomorphism from this triangulation to \a other.  Boundary
         * complete isomorphisms are described in detail in the
         * Isomorphism class notes.
         *
         * In particular, note that this triangulation and \a other must
         * contain the same number of top-dimensional simplices for such an
         * isomorphism to exist.
         *
         * If you need to ensure that top-dimensional simplices are labelled
         * the same in both triangulations, see the stricter test
         * isIdenticalTo() instead.
         *
         * If a boundary complete isomorphism is found, the details of
         * this isomorphism are returned.  The isomorphism is newly
         * constructed, and so to assist with memory management is
         * returned as a std::auto_ptr.  Thus, to test whether an
         * isomorphism exists without having to explicitly deal with the
         * isomorphism itself, you can call
         * <tt>if (isIsomorphicTo(other).get())</tt> and the newly
         * created isomorphism (if it exists) will be automatically
         * destroyed.
         *
         * If more than one such isomorphism exists, only one will be
         * returned.  For a routine that returns all such isomorphisms,
         * see findAllIsomorphisms().
         *
         * \todo \opt Improve the complexity by choosing a simplex
         * mapping from each component and following gluings to
         * determine the others.
         *
         * @param other the triangulation to compare with this one.
         * @return details of the isomorphism if the two triangulations
         * are combinatorially isomorphic, or a null pointer otherwise.
         */
        std::auto_ptr<Isomorphism<dim>> isIsomorphicTo(
            const typename DimTraits<dim>::Triangulation& other) const;

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
         * returned as a std::auto_ptr.  Thus, to test whether an
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
         * @param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * @return details of the isomorphism if such a copy is found,
         * or a null pointer otherwise.
         */
        std::auto_ptr<Isomorphism<dim>> isContainedIn(
            const typename DimTraits<dim>::Triangulation& other) const;

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
         * The isomorphisms that are found will be inserted into the
         * given list.  These isomorphisms will be newly created, and
         * the caller of this routine is responsible for destroying
         * them.  The given list will not be emptied before the new
         * isomorphisms are inserted.
         *
         * \ifacespython Not present.
         *
         * @param other the triangulation to compare with this one.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @return the number of isomorphisms that were found.
         */
        unsigned long findAllIsomorphisms(
            const typename DimTraits<dim>::Triangulation& other,
            std::list<Isomorphism<dim>*>& results) const;

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
         * The isomorphisms that are found will be inserted into the
         * given list.  These isomorphisms will be newly created, and
         * the caller of this routine is responsible for destroying
         * them.  The given list will not be emptied before the new
         * isomorphisms are inserted.
         *
         * \ifacespython Not present.
         *
         * @param other the triangulation in which to search for
         * isomorphic copies of this triangulation.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @return the number of isomorphisms that were found.
         */
        unsigned long findAllSubcomplexesIn(
            const typename DimTraits<dim>::Triangulation& other,
            std::list<Isomorphism<dim>*>& results) const;

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
         * The new triangles will be inserted into this triangulation
         * in the order in which they appear in the given triangulation,
         * and the numbering of their vertices (0-2) will not change.
         * They will be given the same descriptions as appear in the
         * given triangulation.
         *
         * @param source the triangulation whose copy will be inserted.
         */
        void insertTriangulation(const Dim2Triangulation& source);

        /**
         * Inserts into this triangulation a set of triangles and their
         * gluings as described by the given integer arrays.
         *
         * This routine is provided to make it easy to hard-code a
         * medium-sized triangulation in a C++ source file.  All of the
         * pertinent data can be hard-coded into a pair of integer arrays at
         * the beginning of the source file, avoiding an otherwise tedious
         * sequence of many joinTo() calls.
         *
         * An additional \a nTriangles triangles will be inserted into
         * this triangulation.  The relationships between these triangles
         * should be stored in the two arrays as follows.  Note that the
         * new triangles are numbered from 0 to (\a nTriangles - 1), and
         * individual triangle edges are numbered from 0 to 2.
         *
         * The \a adjacencies array describes which triangle edges are
         * joined to which others.  Specifically, <tt>adjacencies[f][e]</tt>
         * should contain the number of the triangle joined to edge \a e
         * of triangle \a f.  If this edge is to be left as a
         * boundary edge, <tt>adjacencies[f][e]</tt> should be -1.
         *
         * The \a gluings array describes the particular gluing permutations
         * used when joining these triangle edges together.  Specifically,
         * <tt>gluings[f][e][0..2]</tt> should describe the permutation
         * used to join edge \a e of triangle \a f to its adjacent
         * triangle.  These three integers should be 0, 1 and 2 in some
         * order, so that <tt>gluings[f][e][i]</tt> contains the image of
         * \a i under this permutation.  If edge \a e of triangle \a f
         * is to be left as a boundary edge, <tt>gluings[f][e][0..2]</tt>
         * may contain anything (and will be duly ignored).
         *
         * It is the responsibility of the caller of this routine to
         * ensure that the given arrays are correct and consistent.
         * No error checking will be performed by this routine.
         *
         * Note that, for an existing triangulation, dumpConstruction()
         * will output a pair of C++ arrays that can be copied into a
         * source file and used to reconstruct the triangulation via
         * this routine.
         *
         * \ifacespython Not present.
         *
         * @param nTriangles the number of additional triangles to insert.
         * @param adjacencies describes which of the new triangle edges
         * are to be identified.  This array must have initial
         * dimension at least \a nTriangles.
         * @param gluings describes the specific gluing permutations by
         * which these new triangle edges should be identified.  This
         * array must also have initial dimension at least \a nTriangles.
         */
        void insertConstruction(unsigned long nTriangles,
            const int adjacencies[][3], const int gluings[][3][3]);

        /*@}*/
        /**
         * \name Exporting Triangulations
         */
        /*@{*/

        /**
         * Constructs the isomorphism signature for this triangulation.
         *
         * An <i>isomorphism signature</i> is a compact text representation of
         * a triangulation.  Unlike dehydrations for 3-manifold triangulations,
         * an isomorphism signature uniquely determines a triangulation up
         * to combinatorial isomorphism (assuming the dimension is known in
         * advance).
         * That is, two triangulations of dimension \a dim are combinatorially
         * isomorphic if and only if their isomorphism signatures are the same.
         *
         * The isomorphism signature is constructed entirely of printable
         * characters, and has length proportional to <tt>n log n</tt>,
         * where \a n is the number of top-dimenisonal simplices.
         *
         * Isomorphism signatures are more general than dehydrations:
         * they can be used with any triangulation (including closed,
         * bounded and/or disconnected triangulations, as well
         * as triangulations with large numbers of triangles).
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
         * (signature, relabelling).
         *
         * \pre If \a relabelling is non-null, then this triangulation
         * must be non-empty and connected.  The facility to return a
         * relabelling for disconnected triangulations may be added to
         * Regina in a later release.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different \a p and \a q.
         *
         * @param relabelling if non-null, this will be modified to point to a
         * new isomorphism describing the relationship between this
         * triangulation and that reconstructed from fromIsoSig(), as
         * described above.
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
         *   describing the triangle gluings in this trianguation;
         * - two additional lines that declare a new Dim2Triangulation and
         *   call insertConstruction() to rebuild this triangulation.
         *
         * The main purpose of this routine is to generate the two integer
         * arrays, which can be tedious and error-prone to code up by hand.
         *
         * Note that the number of lines of code produced grows linearly
         * with the number of triangles.  If this triangulation is very
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
         * The triangulation that is returned will be newly created.
         *
         * Calling isoSig() followed by fromIsoSig() is not guaranteed to
         * produce an identical triangulation to the original, but it
         * \e is guaranteed to produce a combinatorially isomorphic
         * triangulation.
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
         * for different \a p and \a q.
         *
         * @param sig the isomorphism signature of the
         * triangulation to construct.  Note that, unlike dehydration
         * strings for 3-manifold triangulations, case is important for
         * isomorphism signatures.
         * @return a newly allocated triangulation if the reconstruction was
         * successful, or null if the given string was not a valid
         * isomorphism signature.
         */
        static typename DimTraits<dim>::Triangulation* fromIsoSig(
            const std::string& sig);

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
         * return the number of simplices in the first connected component.
         * If you need the total number of simplices in a disconnected
         * triangulation, you will need to reconstruct the full triangulation
         * by calling fromIsoSig() instead.
         *
         * This routine is very fast, since it only examines the first
         * few characters of the isomorphism signature (in which the size
         * of the first component is encoded).  However, it is therefore
         * possible to pass an invalid isomorphism signature and still
         * receive a positive result.  If you need to \e test whether a
         * signature is valid or not, you must call fromIsoSig()
         * instead, which will examine the entire signature in full.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different \a p and \a q.
         *
         * @param sig an isomorphism signature of a \a dim-dimensional
         * triangulation.  Note that, unlike dehydration strings for
         * 3-manifold triangulations, case is important for isomorphism
         * signatures.
         * @return the number of top-dimensional simplices in the first
         * connected component, or 0 if this could not be determined
         * because the given string was not a valid isomorphism signature.
         */
        static size_t isoSigComponentSize(const std::string& sig);

        /*@}*/
        /**
         * \name Output
         */
        /*@{*/

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        /*@}*/

    private:
        /**
         * Internal to isoSig().
         *
         * Constructs a candidate isomorphism signature for a single
         * component of this triangulation.  This candidate signature
         * assumes that the given simplex with the given labelling
         * of its vertices becomes simplex zero with vertices 0..dim
         * under the "canonical isomorphism".
         *
         * @param simp the index of some simplex in this triangulation.
         * @param vertices some ordering of the vertices of the
         * given tetrahedron.
         * @param if this is non-null, it will be filled with the canonical
         * isomorphism; in this case it must already have been constructed
         * for the correct number of simplices.
         * @return the candidate isomorphism signature.
         */
        static std::string isoSigFrom(
            const typename DimTraits<dim>::Triangulation& tri,
            unsigned simp,
            const NPerm<dim+1>& vertices,
            Isomorphism<dim>* relabelling);
};

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
 * \pre \a dim is one of the supported triangulation dimensions.
 * \pre \a subdim is between 0 and \a dim-1 inclusive.
 */
template <int dim, int subdim>
class REGINA_API DegreeLessThan {
    private:
        const typename DimTraits<dim>::Triangulation& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * @param tri the triangulation with which we are working.
         */
        DegreeLessThan(const typename DimTraits<dim>::Triangulation& tri);
        /**
         * Compares the degrees of the \a subdim-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of \a subdim-dimensional faces in
         * the triangulation.
         *
         * @param a the index of the first \a subdim-dimensional face
         * within the triangulation.
         * @param b the index of the second \a subdim-dimensional face
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
 * \pre \a dim is one of the supported triangulation dimensions.
 * \pre \a subdim is between 0 and \a dim-1 inclusive.
 */
template <int dim, int subdim>
class REGINA_API DegreeGreaterThan {
    private:
        const typename DimTraits<dim>::Triangulation& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * @param tri the triangulation with which we are working.
         */
        DegreeGreaterThan(const typename DimTraits<dim>::Triangulation& tri);
        /**
         * Compares the degrees of the \a subdim-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of \a subdim-dimensional faces in
         * the triangulation.
         *
         * @param a the index of the first \a subdim-dimensional face
         * within the triangulation.
         * @param b the index of the second \a subdim-dimensional face
         * within the triangulation.
         * @return \c true if and only if face \a a has greater degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const;
};

/*@}*/

// Inline functions:

template <int dim>
inline bool NGenericTriangulation<dim>::isEmpty() const {
    return (static_cast<const typename DimTraits<dim>::Triangulation*>(this)->
        getNumberOfSimplices() == 0);
}

template <int dim>
inline unsigned long NGenericTriangulation<dim>::size() const {
    return static_cast<const typename DimTraits<dim>::Triangulation*>(this)->
        getNumberOfSimplices();
}

template <int dim, int subdim>
inline DegreeLessThan<dim, subdim>::DegreeLessThan(
        const typename DimTraits<dim>::Triangulation& tri) : tri_(tri) {
}

template <int dim, int subdim>
inline bool DegreeLessThan<dim, subdim>::operator () (
        unsigned a, unsigned b) const {
    return (tri_.template getFace<subdim>(a)->getNumberOfEmbeddings() <
            tri_.template getFace<subdim>(b)->getNumberOfEmbeddings());
}

template <int dim, int subdim>
inline DegreeGreaterThan<dim, subdim>::DegreeGreaterThan(
        const typename DimTraits<dim>::Triangulation& tri) : tri_(tri) {
}

template <int dim, int subdim>
inline bool DegreeGreaterThan<dim, subdim>::operator () (
        unsigned a, unsigned b) const {
    return (tri_.template getFace<subdim>(a)->getNumberOfEmbeddings() >
            tri_.template getFace<subdim>(b)->getNumberOfEmbeddings());
}

} // namespace regina

#endif







/**
 * Stores the triangulation of a 2-manifold along with its
 * various cellular structures and other information.  A 2-manifold
 * triangulation is built from triangular faces.
 *
 * When the triangulation is deleted, the corresponding
 * triangles, the cellular structure and all other properties
 * will be deallocated.
 *
 * Elements of the 1- and 0-skeletons (edges and vertices respectively) are
 * always temporary, as are components and
 * boundary components.  Whenever a change occurs with the triangulation,
 * these objects will all be deleted and a new skeletal structure will be
 * calculated.  The same is true of various other triangulation properties.
 */
    public:

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;

        /**
         * Turns this triangulation into a clone of the given triangulation.
         * The tree structure and label of this triangulation are not touched.
         *
         * @param from the triangulation from which this triangulation
         * will be cloned.
         */
        void cloneFrom(const Dim2Triangulation& from);

    private:
        void deleteTriangles();
            /**< Deallocates all triangles and empties the list. */

        /**
         * Clears any calculated properties and declares them all
         * unknown.  All dynamic memory used for storing known
         * properties is deallocated.
         *
         * In most cases this routine is followed immediately by firing
         * a packet change event.
         */
        virtual void clearAllProperties();

        /**
         * Recalculates vertices, edges, components and
         * boundary components, as well as various other skeletal properties.
         * All appropriate lists are filled.
         *
         * \pre All skeletal lists are empty.
         */
        void calculateSkeleton() const;

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
         * original boundary edges joined to new triangles.
         *
         * See the Dim2Isomorphism class notes for further details
         * regarding boundary complete and boundary incomplete
         * isomorphisms.
         *
         * The isomorphisms found, if any, will be appended to the
         * list \a results.  This list will not be emptied before
         * calculations begin.  All isomorphisms will be newly created,
         * and the caller of this routine is responsible for destroying
         * them.
         *
         * If \a firstOnly is passed as \c true, only the first
         * isomorphism found (if any) will be returned, after which the
         * routine will return immediately.  Otherwise all isomorphisms
         * will be returned.
         *
         * @param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @param completeIsomorphism \c true if isomorphisms must be
         * onto and boundary complete, or \c false if neither of these
         * restrictions should be imposed.
         * @param firstOnly \c true if only one isomorphism should be
         * returned (if any), or \c false if all isomorphisms should be
         * returned.
         * @return the total number of isomorphisms found.
         */
        unsigned long findIsomorphisms(const Dim2Triangulation& other,
                std::list<Dim2Isomorphism*>& results,
                bool completeIsomorphism, bool firstOnly) const;

    friend class regina::Simplex<2>;
    friend class regina::SimplexBase<2>;
    friend class regina::NGenericTriangulation<2>;
    friend class regina::NXMLDim2TriangulationReader;
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangle.h"
#include "dim2/dim2edge.h"
#include "dim2/dim2vertex.h"
#include "dim2/dim2component.h"
#include "dim2/dim2boundarycomponent.h"
namespace regina {

// Inline functions for Dim2Triangulation

inline Dim2Triangulation::Dim2Triangulation() :
        NPacket(), calculatedSkeleton_(false) {
}

inline Dim2Triangulation::Dim2Triangulation(const Dim2Triangulation& cloneMe) :
        NPacket(), calculatedSkeleton_(false) {
    cloneFrom(cloneMe);
}

inline Dim2Triangulation::~Dim2Triangulation() {
    clearAllProperties();
    deleteTriangles();
}

inline void Dim2Triangulation::writeTextShort(std::ostream& out) const {
    out << "Triangulation with " << triangles_.size()
        << (triangles_.size() == 1 ? " triangle" : " triangles");
}

inline bool Dim2Triangulation::dependsOnParent() const {
    return false;
}

inline unsigned long Dim2Triangulation::getNumberOfSimplices() const {
    return triangles_.size();
}

inline const std::vector<Simplex<dim>*>& Dim2Triangulation::getSimplices()
        const {
    return (const std::vector<Simplex<dim>*>&)(triangles_);
}

inline Simplex<dim>* Dim2Triangulation::getSimplex(unsigned long index) {
    return triangles_[index];
}

inline const Simplex<dim>* Dim2Triangulation::getSimplex(unsigned long index)
        const {
    return triangles_[index];
}

inline long Dim2Triangulation::simplexIndex(const Simplex<dim>* tri) const {
    return tri->markedIndex();
}

inline Simplex<dim>* Dim2Triangulation::newSimplex() {
    ChangeEventSpan span(this);
    Simplex<dim>* tri = new Simplex<dim>(this);
    triangles_.push_back(tri);
    clearAllProperties();
    return tri;
}

inline Simplex<dim>* Dim2Triangulation::newSimplex(const std::string& desc) {
    ChangeEventSpan span(this);
    Simplex<dim>* tri = new Simplex<dim>(desc, this);
    triangles_.push_back(tri);
    clearAllProperties();
    return tri;
}

inline void Dim2Triangulation::removeSimplex(Simplex<dim>* tri) {
    ChangeEventSpan span(this);

    tri->isolate();
    triangles_.erase(triangles_.begin() + triangleIndex(tri));
    delete tri;

    clearAllProperties();
}

inline void Dim2Triangulation::removeSimplexAt(unsigned long index) {
    ChangeEventSpan span(this);

    Simplex<dim>* ans = triangles_[index];
    ans->isolate();
    triangles_.erase(triangles_.begin() + index);
    delete ans;

    clearAllProperties();
}

inline void Dim2Triangulation::removeAllSimplices() {
    ChangeEventSpan span(this);
    deleteTriangles();
    clearAllProperties();
}

inline bool Dim2Triangulation::hasBoundaryEdges() const {
    return ! isClosed();
}

inline NPacket* Dim2Triangulation::internalClonePacket(NPacket*) const {
    return new Dim2Triangulation(*this);
}

} // namespace regina

#endif


#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "dim2/dim2triangulation.h"
#include "utilities/xmlutils.h"

namespace regina {

Dim2Triangulation::Dim2Triangulation(const std::string& description) :
        calculatedSkeleton_(false) {
    Dim2Triangulation* attempt;

    if ((attempt = fromIsoSig(description))) {
        cloneFrom(*attempt);
        setPacketLabel(description);
    }

    delete attempt;
}

void Dim2Triangulation::swapContents(Dim2Triangulation& other) {
    ChangeEventSpan span1(this);
    ChangeEventSpan span2(&other);

    clearAllProperties();
    other.clearAllProperties();

    triangles_.swap(other.triangles_);

    TriangleIterator it;
    for (it = triangles_.begin(); it != triangles_.end(); ++it)
        (*it)->tri_ = this;
    for (it = other.triangles_.begin(); it != other.triangles_.end(); ++it)
        (*it)->tri_ = &other;
}

void Dim2Triangulation::moveContentsTo(Dim2Triangulation& dest) {
    ChangeEventSpan span1(this);
    ChangeEventSpan span2(&dest);

    clearAllProperties();
    dest.clearAllProperties();

    TriangleIterator it;
    for (it = triangles_.begin(); it != triangles_.end(); ++it) {
        // This is an abuse of NMarkedVector, since for a brief moment
        // each triangle belongs to both vectors triangles_ and dest.triangles_.
        // However, the subsequent clear() operation does not touch the
        // triangle markings (indices), and so we end up with the
        // correct result (i.e., the markings are correct for dest).
        (*it)->tri_ = &dest;
        dest.triangles_.push_back(*it);
    }
    triangles_.clear();
}

void Dim2Triangulation::writeTextLong(std::ostream& out) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();

    out << "Size of the skeleton:\n";
    out << "  Triangles: " << triangles_.size() << '\n';
    out << "  Edges: " << edges_.size() << '\n';
    out << "  Vertices: " << vertices_.size() << '\n';
    out << '\n';

    Simplex<dim>* tri;
    Simplex<dim>* adjTri;
    unsigned triPos;
    int i, j;
    NPerm3 adjPerm;

    out << "Triangle gluing:\n";
    out << "  Triangle  |  glued to:     (01)     (02)     (12)\n";
    out << "  ----------+--------------------------------------\n";
    for (triPos=0; triPos < triangles_.size(); triPos++) {
        tri = triangles_[triPos];
        out << "      " << std::setw(4) << triPos << "  |           ";
        for (i = 2; i >= 0; --i) {
            out << " ";
            adjTri = tri->adjacentTriangle(i);
            if (! adjTri)
                out << "boundary";
            else {
                adjPerm = tri->adjacentGluing(i);
                out << std::setw(3) << triangleIndex(adjTri) << " (";
                for (j = 0; j < 3; ++j) {
                    if (j == i) continue;
                    out << adjPerm[j];
                }
                out << ")";
            }
        }
        out << '\n';
    }
    out << '\n';

    out << "Vertices:\n";
    out << "  Triangle  |  vertex:    0   1   2\n";
    out << "  ----------+----------------------\n";
    for (triPos = 0; triPos < triangles_.size(); ++triPos) {
        tri = triangles_[triPos];
        out << "      " << std::setw(4) << triPos << "  |          ";
        for (i = 0; i < 3; ++i)
            out << ' ' << std::setw(3) <<
                vertexIndex(tri->getVertex(i));
        out << '\n';
    }
    out << '\n';

    out << "Edges:\n";
    out << "  Triangle  |  edge:   01  02  12\n";
    out << "  ----------+--------------------\n";
    for (triPos = 0; triPos < triangles_.size(); ++triPos) {
        tri = triangles_[triPos];
        out << "      " << std::setw(4) << triPos << "  |        ";
        for (i = 2; i >= 0; --i)
            out << ' ' << std::setw(3) << edgeIndex(tri->getEdge(i));
        out << '\n';
    }
    out << '\n';
}

void Dim2Triangulation::insertTriangulation(const Dim2Triangulation& X) {
    ChangeEventSpan span(this);

    unsigned long nOrig = getNumberOfTriangles();
    unsigned long nX = X.getNumberOfTriangles();

    unsigned long triPos;
    for (triPos = 0; triPos < nX; ++triPos)
        newTriangle(X.triangles_[triPos]->getDescription());

    // Make the gluings.
    unsigned long adjPos;
    Simplex<dim>* tri;
    Simplex<dim>* adjTri;
    NPerm3 adjPerm;
    int edge;
    for (triPos = 0; triPos < nX; ++triPos) {
        tri = X.triangles_[triPos];
        for (edge = 0; edge < 3; ++edge) {
            adjTri = tri->adjacentTriangle(edge);
            if (adjTri) {
                adjPos = X.triangleIndex(adjTri);
                adjPerm = tri->adjacentGluing(edge);
                if (adjPos > triPos ||
                        (adjPos == triPos && adjPerm[edge] > edge)) {
                    triangles_[nOrig + triPos]->joinTo(edge,
                        triangles_[nOrig + adjPos], adjPerm);
                }
            }
        }
    }
}

void Dim2Triangulation::insertConstruction(unsigned long nTriangles,
        const int adjacencies[][3], const int gluings[][3][3]) {
    if (nTriangles == 0)
        return;

    Simplex<dim>** tri = new Simplex<dim>*[nTriangles];

    unsigned i, j;
    NPerm3 p;

    ChangeEventSpan span(this);

    for (i = 0; i < nTriangles; ++i)
        tri[i] = newTriangle();

    for (i = 0; i < nTriangles; ++i)
        for (j = 0; j < 3; ++j)
            if (adjacencies[i][j] >= 0 &&
                    ! tri[i]->adjacentTriangle(j)) {
                p = NPerm3(gluings[i][j][0], gluings[i][j][1],
                    gluings[i][j][2]);
                tri[i]->joinTo(j, tri[adjacencies[i][j]], p);
            }

    delete[] tri;
}

std::string Dim2Triangulation::dumpConstruction() const {
    std::ostringstream ans;
    ans <<
"/**\n";
    if (! getPacketLabel().empty())
        ans <<
" * 2-manifold triangulation: " << getPacketLabel() << "\n";
    ans <<
" * Code automatically generated by dumpConstruction().\n"
" */\n"
"\n";

    if (triangles_.empty()) {
        ans <<
"/* This triangulation is empty.  No code is being generated. */\n";
        return ans.str();
    }

    ans <<
"/**\n"
" * The following arrays describe the individual gluings of\n"
" * triangle edges.\n"
" */\n"
"\n";

    unsigned long nTriangles = triangles_.size();
    Simplex<dim>* tri;
    NPerm3 perm;
    unsigned long p;
    int e, i;

    ans << "const int adjacencies[" << nTriangles << "][3] = {\n";
    for (p = 0; p < nTriangles; ++p) {
        tri = triangles_[p];

        ans << "    { ";
        for (e = 0; e < 3; ++e) {
            if (tri->adjacentTriangle(e)) {
                ans << triangleIndex(tri->adjacentTriangle(e));
            } else
                ans << "-1";

            if (e < 2)
                ans << ", ";
            else if (p != nTriangles - 1)
                ans << "},\n";
            else
                ans << "}\n";
        }
    }
    ans << "};\n\n";

    ans << "const int gluings[" << nTriangles << "][3][3] = {\n";
    for (p = 0; p < nTriangles; ++p) {
        tri = triangles_[p];

        ans << "    { ";
        for (e = 0; e < 3; ++e) {
            if (tri->adjacentTriangle(e)) {
                perm = tri->adjacentGluing(e);
                ans << "{ ";
                for (i = 0; i < 3; ++i) {
                    ans << perm[i];
                    if (i < 2)
                        ans << ", ";
                    else
                        ans << " }";
                }
            } else
                ans << "{ 0, 0, 0 }";

            if (e < 2)
                ans << ", ";
            else if (p != nTriangles - 1)
                ans << " },\n";
            else
                ans << " }\n";
        }
    }
    ans << "};\n\n";

    ans <<
"/**\n"
" * The following code actually constructs a 2-manifold triangulation\n"
" * based on the information stored in the arrays above.\n"
" */\n"
"\n"
"Dim2Triangulation tri;\n"
"tri.insertConstruction(" << nTriangles << ", adjacencies, gluings);\n"
"\n";

    return ans.str();
}

void Dim2Triangulation::cloneFrom(const Dim2Triangulation& X) {
    ChangeEventSpan span(this);

    removeAllTriangles();

    TriangleIterator it;
    for (it = X.triangles_.begin(); it != X.triangles_.end(); ++it)
        newTriangle((*it)->getDescription());

    // Make the gluings.
    long triPos, adjPos;
    Simplex<dim>* tri;
    Simplex<dim>* adjTri;
    NPerm3 adjPerm;
    int edge;
    triPos = 0;
    for (it = X.triangles_.begin(); it != X.triangles_.end(); ++it) {
        tri = *it;
        for (edge = 0; edge < 3; ++edge) {
            adjTri = tri->adjacentTriangle(edge);
            if (adjTri) {
                adjPos = X.triangleIndex(adjTri);
                adjPerm = tri->adjacentGluing(edge);
                if (adjPos > triPos ||
                        (adjPos == triPos && adjPerm[edge] > edge)) {
                    triangles_[triPos]->joinTo(edge,
                        triangles_[adjPos], adjPerm);
                }
            }
        }
        ++triPos;
    }

    // Properties:
    // None yet for 2-manifold triangulations.
}

void Dim2Triangulation::deleteTriangles() {
    for (TriangleIterator it = triangles_.begin(); it != triangles_.end(); ++it)
        delete *it;
    triangles_.clear();
}

void Dim2Triangulation::clearAllProperties() {
    if (calculatedSkeleton_)
        deleteSkeleton();
}

} // namespace regina
