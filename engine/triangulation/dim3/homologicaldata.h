
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

/*! \file triangulation/dim3/homologicaldata.h
 *  \brief Deals with all the details of the cellular homology of a manifold.
 */

#ifndef __REGINA_HOMOLOGICALDATA_H
#ifndef __DOXYGEN
#define __REGINA_HOMOLOGICALDATA_H
#endif

#include "regina-core.h"
#include "core/output.h"
#include "algebra/markedabeliangroup.h"
#include "maths/rational.h"
#include "triangulation/dim3.h"
#include <algorithm>
#include <vector>
#include <cstddef>
#include <optional>

namespace regina {

/**
 * A specialised class that computes a large amount of detailed homological
 * information for a 3-manifold triangulation.
 *
 * This class is both heavyweight and specialised.  If you just wish
 * to compute homology or Euler characteristic, or even map faces of the
 * skeleton to classes in the homology groups, the Triangulation class
 * has simpler and slicker member functions that can do this for you.
 * See for example the Triangulation functions homology(), eulerCharTri(),
 * eulerCharManifold(), and markedHomology().
 *
 * The most important thing that HomologicalData adds is the ability to
 * work with the torsion linking form.  Be aware however that the code that
 * computes the torsion linking form includes some floating-point arithmetic,
 * and this could be subject to round-off error.  Such errors are not expected,
 * since the floating-point code is simply distinguishing different multiples
 * of a known irrational, but nevertheless these results should be considered
 * non-rigorous.
 *
 * Currently, the information computed by HomologicalData includes:
 *
 * - the manifold's homology;
 * - the boundary's homology;
 * - the map from boundary -> manifold;
 * - the dual cellular homology;
 * - the isomorphism on H1 from the dual cellular homology to the regular
 *   cellular homology;
 * - the H1 torsion form;
 * - the Kawauchi-Kojima invariants of torsion linking forms.
 *
 * This class takes a "least effort" approach to all computations. It
 * only computes what is neccessary for your requests.  It also keeps a
 * record of all previous computations you've made. If a computation can
 * be sped up by not recomputing some data, it takes that short-cut.
 *
 * All these algorithms use two transverse CW decompositions of the manifold.
 * They correspond to the (possibly ideal) triangulation native to Regina, 
 * and the dual polyhedral (CW) decomposition which appears in Seifert and
 * Threlfall's textbook.
 *
 * In the following lists we describe the canonical ordering of both the
 * cells and the dual cells of the given triangulation.
 *
 * First we list the cell orderings for the _standard CW decomposition_,
 * which most closely resembles the ideal triangulation.
 *
 * - \b 0-cells: The non-ideal vertices given in the order vertices.begin()
 *               to vertices.end(), followed by the ideal endpoints of the
 *               edges edges.begin() to edges.end() with endpoints
 *               for each edge taken in the order 0,1.
 *
 * - \b 1-cells: edges.begin() to edges.end(), followed by the ideal edges of
 *               faces.begin() to faces.end() in order 0,1,2.
 *
 * - \b 2-cells: faces.begin() to faces.end(), followed by the ideal faces of
 *               tetrahedra.begin() through tetrahedra.end() in order 0,1,2,3.
 *
 * - \b 3-cells: tetrahedra.begin() through tetrahedra.end().
 *
 * Next we list the cell orderings for the _dual CW decomposition_:
 * if the standard CW decomposition came from a morse function \a f, this
 * would be the one for -\a f.
 *
 * - \b 0-cells: tetrahedra.begin() through tetrahedra.end().
 *
 * - \b 1-cells: the non-boundary faces.begin() through faces.end().
 *
 * - \b 2-cells: the non-boundary edges.begin() through edges.end().
 *
 * - \b 3-cells: the non-boundary, non-ideal vertices.begin() through
 *               vertices.end().
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Be aware, however, that
 * it contains an enormous amount of internal data, and even moves will
 * still be relatively expensive - you should try to use just the one
 * HomologicalData object and not copy or move it at all, if possible.
 *
 * This class will eventually be removed in a future release of Regina.
 *
 * \author Ryan Budney
 *
 * \ingroup triangulation
 */
class HomologicalData : public ShortOutput<HomologicalData> {
private:
    /**
     * A fairly primitive class that implements sorted arrays of
     * unsigned integers, with logarithmic-time lookup.  The interface is
     * extremely basic.
     *
     * This class is a placeholder, and is \emph not for long-term use.
     * Eventually it will (probably) be replaced with something richer,
     * slicker and/or more appropriate.
     *
     * \warning A precondition of using this class is that elements are
     * inserted in increasing order only.
     */
    class SortedArray {
        private:
            std::vector<unsigned long> data_;
                /**< The underlying array of integers. */

        public:
            /**
             * Construct an empty array.
             */
            inline SortedArray() = default;

            /**
             * Construct a clone of the given array.
             */
            SortedArray(const SortedArray&) = default;
            /**
             * Move the contents of the given array into this new array.
             */
            SortedArray(SortedArray&&) noexcept = default;

            /**
             * Set this to a clone of the given array.
             */
            SortedArray& operator = (const SortedArray&) = default;
            /**
             * Move the contents of the given array into this array.
             */
            SortedArray& operator = (SortedArray&&) noexcept = default;

            /**
             * Swap the contents of this and the given array.
             */
            void swap(SortedArray& other) noexcept {
                data_.swap(other.data_);
            }

            /**
             * Return the number of elements in this array.
             *
             * \return the number of elements.
             */
            inline size_t size() const {
                return data_.size();
            }
            /**
             * Return the integer at the given index in this array.
             *
             * \param index the requested array index; this must be
             * between 0 and size()-1 inclusive.
             * \return the corresponding element of this array.
             */
            inline unsigned long operator [] (size_t index) const {
                return data_[index];
            }
            /**
             * Finds the index of the given integer in this array.
             *
             * This routine runs in logarithmic time (it uses a
             * binary search).
             *
             * \param value the integer to search for.
             * \return the array index that holds the given integer,
             * or -1 if the given integer is not stored in this array.
             */
            inline ssize_t index(unsigned long value) const {
                auto it = std::lower_bound(data_.begin(), data_.end(), value);
                if (it != data_.end() && *it == value)
                    return (it - data_.begin());
                else
                    return -1;
            }

            /**
             * Pushes the given integer onto the end of this array.
             *
             * \pre The given integer is at least as large as every
             * integer currently stored in the array.
             *
             * \param value the integer to insert into this array.
             */
            inline void push_back(unsigned long value) {
                data_.push_back(value);
            }
    };

    /**
     * The triangulation that this object was initialised with.
     * All routines use this triangulation as reference.
     */
    SnapshotRef<Triangulation<3>> tri_;

    /**
     * The 0-th homology group in standard cellular coordinates,
     * or \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> mHomology0_;
    /**
     * The 1st homology group in standard cellular coordinates,
     * or \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> mHomology1_;
    /**
     * The 2nd homology group in standard cellular coordinates,
     * or \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> mHomology2_;
    /**
     * The 3rd homology group in standard cellular coordinates,
     * or \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> mHomology3_;

    /**
     * The 0-th boundary homology group in standard cellular
     * coordinates, or \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> bHomology0_;
    /**
     * The 1st boundary homology group in standard cellular
     * coordinates, or \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> bHomology1_;
    /**
     * The 2nd boundary homology group in standard cellular
     * coordinates, or \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> bHomology2_;

    /**
     * The boundary inclusion on 0-th homology, standard
     * cellular coordinates, or \nullopt if it has not yet been computed.
     */
    std::optional<HomMarkedAbelianGroup> bmMap0_;
    /**
     * The boundary inclusion on 1st homology, standard
     * cellular coordinates, or \nullopt if it has not yet been computed.
     */
    std::optional<HomMarkedAbelianGroup> bmMap1_;
    /**
     * The boundary inclusion on 2nd homology, standard
     * cellular coordinates, or \nullopt if it has not yet been computed.
     */
    std::optional<HomMarkedAbelianGroup> bmMap2_;

    /**
     * The 0-th homology group in dual cellular coordinates, or
     * \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> dmHomology0_;
    /**
     * The 1st homology group in dual cellular coordinates, or
     * \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> dmHomology1_;
    /**
     * The 2nd homology group in dual cellular coordinates, or
     * \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> dmHomology2_;
    /**
     * The 3rd homology group in dual cellular coordinates, or
     * \nullopt if it has not yet been computed.
     */
    std::optional<MarkedAbelianGroup> dmHomology3_;

    /**
     * The cellular approx of the identity H1(M) --> H1(M)
     * from dual to standard cellular coordinates, or \nullopt if it has
     * not yet been computed.
     */
    std::optional<HomMarkedAbelianGroup> dmTomMap1_;

    // below here and the public declaration go the internal bits of
    // data that are not publicly accessible...

    // the chain complexes for the regular cellular homology

    /** true if the indexing of the chain complexes is complete */
    bool ccIndexingComputed_;

    /** number of standard cells in dimension 0, 1, 2, 3. */
    unsigned long numStandardCells[4];
    /** number of dual cells in dimension 0, 1, 2, 3. */
    unsigned long numDualCells[4];
    /** number of (standard) boundary cells in dimension 0, 1, 2. */
    unsigned long numBdryCells[3];

    /** non-ideal vertices */
    SortedArray sNIV;
    /** vertices which are ideal endpoints of edges */
    SortedArray sIEOE;
    /** edges which are ideal end edges of faces */
    SortedArray sIEEOF;
    /** faces which are ideal end faces of tetrahedra */
    SortedArray sIEFOT;
    /** vertices which are not ideal, and nonboundary */
    SortedArray dNINBV;
    /** interior edges ie: non-boundary edges */
    SortedArray dNBE;
    /** non-boundary faces */
    SortedArray dNBF;
    /** boundary, non-ideal vertices */
    SortedArray sBNIV;
    /** boundary non-ideal edges */
    SortedArray sBNIE;
    /** boundary non-ideal faces */
    SortedArray sBNIF;

    /** True if the chain complexes A0,A1,A2,A3,A4, B0,B1,B2,B3,B4,
        Bd0,Bd1,Bd2,Bd3, B0Incl,B1Incl,B2Incl are computed */
    bool chainComplexesComputed;

    /** 0th term in chain complex for cellular homology, using standard
        CW-complex struc, or \nullopt if not yet computed */
    std::optional<MatrixInt> A0_;
    /** 1st term in chain complex for cellular homology, using standard
        CW-complex struc, or \nullopt if not yet computed */
    std::optional<MatrixInt> A1_;
    /** 2nd term in chain complex for cellular homology, using standard
        CW-complex struc, or \nullopt if not yet computed */
    std::optional<MatrixInt> A2_;
    /** 3rd term in chain complex for cellular homology, using standard
        CW-complex struc, or \nullopt if not yet computed */
    std::optional<MatrixInt> A3_;
    /** 4th term in chain complex for cellular homology, using standard
        CW-complex struc, or \nullopt if not yet computed */
    std::optional<MatrixInt> A4_;

    /** 0-th term in chain complex for dual cellular homology, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> B0_; // B0 is #defined in some system headers :/
    /** 1st term in chain complex for dual cellular homology, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> B1_;
    /** 2nd term in chain complex for dual cellular homology, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> B2_;
    /** 3rd term in chain complex for dual cellular homology, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> B3_;
    /** 4th term in chain complex for dual cellular homology, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> B4_;

    /** 0th term in chain complex for boundary cellular homology, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> Bd0_;
    /** 1st term in chain complex for boundary cellular homology, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> Bd1_;
    /** 2nd term in chain complex for boundary cellular homology, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> Bd2_;
    /** 3rd term in chain complex for boundary cellular homology, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> Bd3_;

    /** Chain map from C_0 boundary to C_0 manifold, standard coords, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> B0Incl_;
    /** Chain map from C_1 boundary to C_1 manifold, standard coords, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> B1Incl_;
    /** Chain map from C_2 boundary to C_2 manifold, standard coords, or
        \nullopt if not yet computed */
    std::optional<MatrixInt> B2Incl_;

    /** Isomorphism from C_1 dual to C_1 standard, or \nullopt if not yet
        computed */
    std::optional<MatrixInt> H1map_;

    /** Call this routine to demand the indexing of the chain complexes. */
    void computeccIndexing();
    /** This routine computes all the chain complexes. */
    void computeChainComplexes();
    /** Computes all the homology groups of the manifold using standard
        cells. */
    void computeHomology();
    /** Computes all the homology groups of the boundary using its standard
        cells. */
    void computeBHomology();
    /** Computes all the homology groups of the manifold using dual cells. This
        routine is the faster than computeHomology() but it's likely a bit
        slower than Triangulation<3>'s homology routines. */
    void computeDHomology();
    /** The induced map on homology corresponding to inclusion of the
        boundary. */
    void computeBIncl();

    /** true when the torsionlinking form has been computed. */
    bool torsionFormComputed;
    /**
     * This routine computes the H1 torsion linking form. It is only
     * well-defined for orientable 3-manifolds, so don't bother calling
     * this routine unless you know the manifold is orientable.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     */
    void computeTorsionLinkingForm();
    /**
     * Unlike computeTorsionLinkingForm(), this routine _can_ be called
     * for non-orientable manifolds (in which case we look at the
     * orientable double cover).
     *
     * \pre The triangulation is of a connected 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     */
    void computeEmbeddabilityString();

    /** the prime power decomposition of the torsion subgroup of H1
        So if the invariant factors were 2,2,4,3,9,9,27,5,5, this would
        be the list: (2, (1, 1, 2)), (3, (1, 2, 2, 3)), (5, (1, 1)) */
    std::vector< std::pair< Integer, std::vector<unsigned long> > >
        h1PrimePowerDecomp;
    /** p-primary decomposition of the torsion linking form as needed to
        construct the Kawauchi-Kojima invariants. */
    std::vector<Matrix<Rational>> linkingFormPD;

    /** True if torsion linking form is `hyperbolic'.   */
    bool torsionLinkingFormIsHyperbolic;
    /** True if torsion linking form is `split' */
    bool torsionLinkingFormIsSplit;
    /** True if torsion linking form satisfies the Kawauchi-Kojima 2-torsion
        condition */
    bool torsionLinkingFormSatisfiesKKtwoTorCondition;

    /** 1 of 3 Kawauchi-Kojima invariants: this describes the rank of the
        torsion subgroup of H1 */
    std::vector< std::pair< Integer, std::vector< unsigned long > > > torRankV;
    /** 2 of 3 Kawauchi-Kojima invariants: this is the sigma-invariant
        of 2-torsion. */
    std::vector< LargeInteger > twoTorSigmaV;
    /** 3 of 3 Kawauchi-Kojima invariants: this is the Legendre symbol
        invariant of odd torsion. */
    std::vector< std::pair< Integer, std::vector< int > > > oddTorLegSymV;

    /** string representing torRankV */
    std::string torsionRankString;
    /** string representing twoTorSigmaV */
    std::string torsionSigmaString;
    /** string representing oddTorLegSymV */
    std::string torsionLegendreString;
    /** comment on what kind of homology spheres the manifold may or may
        not embed in. */
    std::string embeddabilityString;

public:

    /**
     * Takes as input a triangulation.
     *
     * This class will maintain a fixed snapshot of the given triangulation as
     * it is at this point in time.  You are free to edit or even destroy
     * the input triangulation later on; if you do then this HomologicalData
     * object will automatically take its own deep copy of the original and
     * continue to use that for its own internal reference.
     *
     * \param input the triangulation to use.
     */
    HomologicalData(const Triangulation<3>& input);
    /**
     * Creates a new copy of the given object.
     *
     * This constructor induces a deep copy of the given object; moreover,
     * this is expensive since HomologicalData objects are _very_ large.
     */
    HomologicalData(const HomologicalData&) = default;
    /**
     * Moves the contents of the given object into this new object.
     *
     * This operation is constant time, but it is still expensive
     * constant time because HomologicalData objects are _very_ large.
     *
     * The object that was passed will no longer be usable.
     */
    HomologicalData(HomologicalData&&) noexcept = default;

    /**
     * Returns the triangulation that this object was initialised with.
     *
     * This will be a snapshot frozen in time of the triangulation
     * that was originally passed to the HomologicalData constructor.
     *
     * This will return a correct result even if the original triangulation
     * has since been modified or destroyed.  However, in order to ensure
     * this behaviour, it is possible that at different points in time
     * this function may return references to different C++ objects.
     *
     * The rules for using the triangulation() reference are:
     *
     * - Do not keep the resulting reference as a long-term reference or
     *   pointer of your own, since in time you may find yourself referring
     *   to the wrong object (see above).  Just call this function again.
     *
     * - You must respect the read-only nature of the result (i.e.,
     *   you must not cast the constness away).  The snapshotting
     *   process detects modifications, and modifying the frozen
     *   snapshot may result in an exception being thrown.
     *
     * \return a reference to the underlying triangulation.
     */
    const Triangulation<3>& triangulation() const;

    /**
     * Sets this to be a copy of the given object.
     *
     * This operator induces a deep copy of the given object; moreover,
     * this is expensive since HomologicalData objects are _very_ large.
     *
     * \return a reference to this normal surface.
     */
    HomologicalData& operator = (const HomologicalData&) = default;
    /**
     * Moves the contents of the given object into this object.
     *
     * This operation is constant time, but it is still expensive
     * constant time because HomologicalData objects are _very_ large.
     *
     * The object that was passed will no longer be usable.
     *
     * \return a reference to this object.
     */
    HomologicalData& operator = (HomologicalData&&) noexcept = default;

    /**
     * Swaps the contents of this and the given object.
     *
     * \warning Although this operation is constant time, the HomologicalData
     * class contains an enormous amount of data spread across many different
     * member variables, and so this should really be considered "expensive
     * constant time".  You should still work to avoid swapping (or moving,
     * and certainly copying) HomologicalData objects where possible.
     *
     * \param other the object whose contents should be swapped with this.
     */
    void swap(HomologicalData& other) noexcept;

    /**
     * Writes a short text representation of this object to the
     * given output stream.
     *
     * Note this only writes pre-computed data.  Thus if you have
     * not yet asked HomologicalData to compute anything about this
     * triangulation, writeTextShort may be empty.
     *
     * \nopython Use str() instead.
     *
     * \param out the output stream to which to write.
     */
    void writeTextShort(std::ostream& out) const;

    /**
     * This routine gives access to the manifold's homology computed
     * with the regular CW-decomposition.
     *
     * This routine is typically slower than dualHomology(), since
     * dualHomology() uses the dual CW-decomposition which typically
     * has an order of magnitude fewer cells.
     *
     * Note that the groups returned by homology() and dualHomology()
     * are isomorphic, though they are generally described by different
     * presentations.
     *
     * \param q the dimension of the homology group: can be 0, 1, 2 or 3.
     * \return the q-th homology group, computed in the standard
     * CW-decomposition.
     */
    const MarkedAbelianGroup& homology(unsigned q);
    /**
     * This routine gives access to the homology of the boundary
     * of the manifold, computed with the regular CW-decomposition.
     *
     * \param q the dimension of the homology group: can be 0, 1 or 2.
     * \return the q-th boundary homology group, in standard cellular
     * homology coordinates
     */
    const MarkedAbelianGroup& bdryHomology(unsigned q);

    /**
     * This routine gives access to the homomorphism from the
     * homology of the boundary to the homology of the manifold.
     *
     * \param q the dimension of the map: can be 0, 1 or 2.
     * \return the map from H_q of the boundary to H_q of the manifold,
     * computed in standard coordinates.
     */
    const HomMarkedAbelianGroup& bdryHomologyMap(unsigned q);

    /**
     * This routine gives access to the manifold's homology computed
     * with the dual CW-decomposition.
     *
     * This routine is typically faster than homology() since the
     * dual CW-decomposition generally has far fewer cells.
     *
     * Note that the groups returned by homology() and dualHomology()
     * are isomorphic, though they are generally described by different
     * presentations.
     *
     * \param q the dimension of the homology group: can be 0, 1, 2 or 3.
     * \return the q-th homology group, computed in the dual CW-decomposition.
     */
    const MarkedAbelianGroup& dualHomology(unsigned q);

    /**
     * Returns the isomorphism from dualHomology(1) to homology(1)
     * given by a cellular approximation to the identity map on the manifold.
     *
     * \return The isomorphism from dualHomology(1) to homology(1)
     * computed via a cellular approximation of the identity map from
     * the first 1-skeleton to the second.
     */
    const HomMarkedAbelianGroup& h1CellAp();

    /**
     * Returns the number of cells of the given dimension
     * in the standard genuine CW-decomposition of the manifold.
     *
     * In the case that the triangulation is a proper
     * triangulation of a manifold (or delta-complex decomposition) it
     * simply returns the same information as in the Triangulation<3>
     * vertex, edge, face and tetrahedron lists.
     *
     * In the case that this is an ideal triangulation, this algorithm
     * returns the details of the corresponding compact manifold with
     * boundary a union of closed surfaces.
     *
     * \param dimension the dimension of the cells in question; this must
     * be 0, 1, 2 or 3.
     * \return the number of cells of the given dimension in the standard
     * CW-decomposition of the closed manifold.
     */
    unsigned long countStandardCells(unsigned dimension);
    /**
     * Returns the number of cells of the given dimension
     * in the dual CW-decomposition of the manifold. This is typically
     * much smaller than countStandardCells().
     *
     * \param dimension the dimension of the cells in question; this must
     * be 0, 1, 2 or 3.
     * \return the number of cells of the given dimension in the dual
     * CW-decomposition to the triangulation.
     */
    unsigned long countDualCells(unsigned dimension);
    /**
     * Returns the number of cells of the given dimension in the
     * standard CW-decomposition of the boundary of the manifold.
     * This is a subcomplex of the complex used in countStandardCells().
     *
     * \param dimension the dimension of the cells in question; this must
     * be 0, 1 or 2.
     * \return the number of cells of the given dimension in the standard
     * CW-decomposition of the boundary.
     */
    unsigned long countBdryCells(unsigned dimension);
    /**
     * The proper Euler characteristic of the manifold, computed from
     * the dual CW-decomposition.
     *
     * This routine calculates the Euler characteristic of the
     * corresponding compact triangulated 3-manifold, with each ideal
     * vertex treated as a surface boundary component.
     *
     * This routine returns the same value as
     * Triangulation<3>::eulerCharManifold(), though it computes it
     * in a different way.
     *
     * On the other hand, this routine differs from
     * Triangulation<3>::eulerCharTri(), which handles ideal triangulations
     * in a non-standard way (treating each ideal vertex as just a single
     * vertex).
     *
     * \return the Euler characteristic of the corresponding compact
     * triangulated 3-manifold.
     */
    long eulerChar();

    /**
     * Returns the torsion form rank vector. This is the first of
     * the three Kawauchi-Kojima complete invariants of the torsion
     * linking form.
     *
     * This vector describes the rank of the torsion subgroup of H1,
     * given in prime power form.  It is a vector of pairs (\a p, \a x),
     * where \a p is a prime and \a x is its exponent.
     *
     * For details, see "Algebraic classification of linking pairings on
     * 3-manifolds", Akio Kawauchi and Sadayoshi Kojima,
     * Math. Ann. 253 (1980), 29--42.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return the torsion form rank vector.
     */
    const std::vector< std::pair< Integer,
        std::vector< unsigned long > > >& torsionRankVector();
    /**
     * Same as torsionRankVector() but returns as a human-readable string.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return human-readable prime power factorization of the order of
     * the torsion subgroup of H1.
     */
    const std::string& torsionRankVectorString();
    /**
     * Returns the 2-torsion sigma vector. This is the second of the three
     * Kawauchi-Kojima invariants. It is orientation-sensitive.
     *
     * For details, see "Algebraic classification of linking pairings on
     * 3-manifolds", Akio Kawauchi and Sadayoshi Kojima,
     * Math. Ann. 253 (1980), 29--42. 
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return the Kawauchi-Kojima sigma-vector.
     */
    const std::vector<LargeInteger>& torsionSigmaVector();
    /**
     * Same as torsionSigmaVector() but returns as a human-readable string.
     * This is an orientation-sensitive invariant.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return the Kawauchi-Kojima sigma-vector in human readable form.
     */
    const std::string& torsionSigmaVectorString();

    /**
     * Returns the odd p-torsion Legendre symbol vector. This is the
     * last of the three Kawauchi-Kojima invariants.
     *
     * For details, see "Algebraic classification of linking pairings on
     * 3-manifolds", Akio Kawauchi and Sadayoshi Kojima,
     * Math. Ann. 253 (1980), 29--42.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return the Legendre symbol vector associated to the torsion
     * linking form.
     */
    const std::vector< std::pair< Integer, std::vector< int > > >&
        torsionLegendreSymbolVector();
    /**
     * Same as torsionLegendreSymbolVector() but returns as a
     * human-readable string.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return the Legendre symbol vector in human-readable form.
     */
    const std::string& torsionLegendreSymbolVectorString();

    /**
     * Returns true iff torsion linking form is `hyperbolic' in
     * the linking-form sense of the word.
     *
     * To be a little more precise, Poincare-duality in a
     * compact orientable boundaryless manifold
     * gives an isomorphism between the torsion subgroup of H_1(M) 
     * denoted tH_1(M) and Hom(tH_1(M),Q/Z), where Q is the rationals and Z the 
     * integers.  The associated bilinear form (with values in Q/Z) is said
     * to be `hyperbolic' if tH_1(M) splits as a direct sum A+B such
     * that Poincare duality sends A to Hom(B,Q/Z) and B to Hom(A,Q/Z).
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return \c true iff the torsion linking form is hyperbolic.
     */
    bool formIsHyperbolic();
    /**
     * Returns true iff the torsion linking form is split.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return \c true iff the linking form is split.
     */
    bool formIsSplit();
    /**
     * Returns true iff the torsion linking form satisfies the
     * Kawauchi-Kojima 2-torsion condition.  This condition
     * states that on all elements \a x of order 2^k,
     * 2^{k-1}form(x,x) = 0.
     *
     * This is a neccessary condition for an orientable 3-manifold
     * perhaps with boundary to embed in a homology 4-sphere.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return \c true iff the form satisfies the 2-torsion
     * condition of Kawauchi-Kojima.
     */
    bool formSatKK();
    /**
     * Returns a comment on whether the manifold might embed in
     * a homology 3-sphere or 4-sphere. Basically, this routine runs
     * through all the Kawauchi-Kojima conditions, plus a
     * few other `elementary' conditions.
     *
     * Each comment will be formatted as one or more English sentences
     * (i.e., with capitalisation and punctuation).  The comments
     * themselves are subject to change between releases of Regina,
     * since later releases may have more detailed tests at their disposal.
     *
     * This routine is available for both orientable and non-orientable
     * triangulations.  In the non-orientable case it may return
     * additional information regarding the orientable double cover.
     *
     * \pre The triangulation is of a connected 3-manifold.
     *
     * \warning The code that computes the torsion linking form includes
     * some floating-point arithmetic that could be subject to round-off error.
     * Such errors are not expected, since this code is simply distinguishing
     * different multiples of a known irrational; nevertheless, these
     * results should at present be considered non-rigorous.
     *
     * \exception UnsolvedCase The torsion linking form could not be computed.
     * This should be rare: the only way it can occur is during an internal
     * rational-to-double conversion if the rational is out of range.
     *
     * \return a string giving a one-line description of what
     * is known about where this manifold embeds, based solely
     * on the manifold's homological data.
     */
    const std::string& embeddabilityComment();
};

/**
 * Swaps the contents of the two given HomologicalData objects.
 *
 * This global routine simply calls HomologicalData::swap(); it is provided
 * so that HomologicalData meets the C++ Swappable requirements.
 *
 * \warning Although this operation is constant time, the HomologicalData
 * class contains an enormous amount of data spread across many different
 * member variables, and so this should really be considered "expensive
 * constant time".  You should still work to avoid swapping (or moving,
 * and certainly copying) HomologicalData objects where possible.
 *
 * \param a the first object whose contents should be swapped.
 * \param b the second object whose contents should be swapped.
 *
 * \ingroup triangulation
 */
inline void swap(HomologicalData& a, HomologicalData& b) noexcept;

// Inline functions for HomologicalData

// constructor
inline HomologicalData::HomologicalData(const Triangulation<3>& input):

        // We clone the properties of input, since the embeddability
        // string wants to know whether input is the 3-sphere.
        tri_(input),

        ccIndexingComputed_(false),
        chainComplexesComputed(false),

        torsionFormComputed(false)
{
    std::fill(numStandardCells, numStandardCells + 4, 0);
    std::fill(numDualCells, numDualCells + 4, 0);
    std::fill(numBdryCells, numBdryCells + 3, 0);
}

inline const Triangulation<3>& HomologicalData::triangulation() const {
    return *tri_;
}

inline unsigned long HomologicalData::countStandardCells(unsigned dimension)
{
    // number of cells of dimension 0, 1, 2, 3.
    computeccIndexing();
    return numStandardCells[dimension];
}

inline unsigned long HomologicalData::countDualCells(unsigned dimension)
{
    // dual cells
    computeccIndexing();
    return numDualCells[dimension];
}

inline unsigned long HomologicalData::countBdryCells(unsigned dimension)
{
    // standard boundary cells
    computeccIndexing();
    return numBdryCells[dimension];
}

inline long int HomologicalData::eulerChar()
{
    // euler characteristic
    computeccIndexing();
    return numDualCells[0]-numDualCells[1]+numDualCells[2]-numDualCells[3];
}

inline const std::vector< std::pair< Integer,
std::vector< unsigned long > > >& HomologicalData::torsionRankVector()
{
    computeTorsionLinkingForm();
    return torRankV;
}
inline const std::vector<LargeInteger>& HomologicalData::torsionSigmaVector()
{
    computeTorsionLinkingForm();
    return twoTorSigmaV;
}
inline const std::vector< std::pair< Integer, std::vector< int > > >&
HomologicalData::torsionLegendreSymbolVector()
{
    computeTorsionLinkingForm();
    return oddTorLegSymV;
}

inline bool HomologicalData::formIsSplit()
{
    computeTorsionLinkingForm();
    return torsionLinkingFormIsSplit;
}
inline bool HomologicalData::formSatKK()
{
    computeTorsionLinkingForm();
    return torsionLinkingFormSatisfiesKKtwoTorCondition;
}
inline const std::string& HomologicalData::torsionRankVectorString()
{
    computeTorsionLinkingForm();
    return torsionRankString;
}

inline const std::string& HomologicalData::torsionSigmaVectorString()
{
    computeTorsionLinkingForm();
    return torsionSigmaString;
}

inline const std::string&
HomologicalData::torsionLegendreSymbolVectorString()
{
    computeTorsionLinkingForm();
    return torsionLegendreString;
}

inline const std::string& HomologicalData::embeddabilityComment()
{
    computeEmbeddabilityString();
    return embeddabilityString;
}

inline void swap(HomologicalData& a, HomologicalData& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

