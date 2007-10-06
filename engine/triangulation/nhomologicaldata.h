
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/*! \file nhomologicaldata.h
 *  \brief Deals with all the details of cellular homology of a manifold.
 */

#ifndef __NHOMOLOGICALDATA_H
#ifndef __DOXYGEN
#define __NHOMOLOGICALDATA_H
#endif

#include "algebra/nmarkedabeliangroup.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nindexedarray.h"
#include "utilities/nrational.h"
#include <vector>

namespace regina {

class NTriangulation;

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Data type that deals with all the detailed homological information in a
 * manifold, including:
 *
 * -# the manifold's homology.
 * -# the boundary's homology.
 * -# the map from boundary -> manifold.
 * -# the dual cellular homology.
 * -# the isomorphism on H1 from the dual cellular homology to the regular
 *    cellular homology.
 * -# the H1 torsion form.
 * -# the Kawauchi-Kojima invariants of torsion linking forms.
 *
 * These algorithms take a "least effort" approach to all computations. It
 * only computes what is neccessary for your requests.  It also keeps a
 * record of all previous computations you've made, so that in case a
 * future computation could be sped-up by not recomputing
 * some data, it takes that short-cut.
 *
 * All these algorithms use two transverse CW decompositions of the manifold.
 * They correspond to the (ideal) triangulation, given a proper boundary, and
 * the CW-dual as in the proof of Poincare Duality.
 *
 * We describe the canonical ordering of both the cells and
 * dual cells of tri.
 *
 * The standard CW decomposition -- this is the one that most
 * closely resembles the ideal triangulation.
 *
 * - 0-cells: The non-ideal vertices given in the order vertices.begin()
 *            to vertices.end() followed by the ideal endpoints of the
 *            edges in lexicographical order edges.begin() to edges.end()
 *            followed by the endpts 0, 1.
 *
 * - 1-cells: edges.begin() to edges.end() followed by the ideal edges of
 *            faces.begin() to faces.end() in order 0,1,2.
 *
 * - 2-cells: faces.begin() to faces.end() followed by ideal faces of
 *            tetrahedra.begin() through tetrahedra.end() in order 0,1,2,3.
 *
 * - 3-cells: tetrahedra.begin() through tetrahedra.end().
 *
 * The Dual CW decomposition -- if the above CW-decomposition came from a
 * morse function f, this would be the one for -f.
 *
 * - 0-cells: tetrahedra.begin() through tetrahedra.end().
 *
 * - 1-cells: the non-boundary faces.begin() through faces.end().
 *
 * - 2-cells: the non-boundary edges.begin() through edges.end().
 *
 * - 3-cells: the non-boundary, non-ideal vertices.begin() through
 *            vertices.end().
 *
 * \testpart
 *
 * @author Ryan Budney
 */
class NHomologicalData : public ShareableObject {
private:
    /**
     * Stored pointer to a valid triangulation. All routines use this
     * triangulation as reference.
     */
    NTriangulation* tri; // this is the triangulation that it is initialized by.

    /**
     * Pointer to the 0-th homology group in standard cellular coordinates,
     * or 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* mHomology0;
    /**
     * Pointer to the 1st homology group in standard cellular coordinates,
     * or 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* mHomology1;
    /**
     * Pointer to the 2nd homology group in standard cellular coordinates,
     * or 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* mHomology2;
    /**
     * Pointer to the 3rd homology group in standard cellular coordinates,
     * or 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* mHomology3;

    /**
     * Pointer to the 0-th boundary homology group in standard cellular
     * coordinates, or 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* bHomology0;
    /**
     * Pointer to the 1st boundary homology group in standard cellular
     * coordinates, or 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* bHomology1;
    /**
     * Pointer to the 2nd boundary homology group in standard cellular
     * coordinates, or 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* bHomology2;

    /**
     * Pointer to the boundary inclusion on 0-th homology, standard
     * cellular coordinates, or 0 if it has not yet been computed.
     */
    NHomMarkedAbelianGroup* bmMap0; // maps from bHomology? to mHomology?
    /**
     * Pointer to the boundary inclusion on 1st homology, standard
     * cellular coordinates, or 0 if it has not yet been computed.
     */
    NHomMarkedAbelianGroup* bmMap1;
    /**
     * Pointer to the boundary inclusion on 2nd homology, standard
     * cellular coordinates, or 0 if it has not yet been computed.
     */
    NHomMarkedAbelianGroup* bmMap2;

    /**
     * Pointer to the 0-th homology group in dual cellular coordinates, or
     * 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* dmHomology0;
    /**
     * Pointer to the 1st homology group in dual cellular coordinates, or
     * 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* dmHomology1;
    /**
     * Pointer to the 2nd homology group in dual cellular coordinates, or
     * 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* dmHomology2;
    /**
     * Pointer to the 3rd homology group in dual cellular coordinates, or
     * 0 if it has not yet been computed.
     */
    NMarkedAbelianGroup* dmHomology3;

    /**
     * Pointer to the cellular approx of the identity H1(M) --> H1(M)
     * from dual to standard cellular coordinates, or 0 if it has not yet
     * been computed.
     */
    NHomMarkedAbelianGroup* dmTomMap1;

    // below here and the public declaration go the internal bits of
    // data that are not publicly accessible...

    // the chain complexes for the regular cellular homology

    /** true if the indexing of the chain complexes is complete */
    bool ccIndexingComputed;

    /** number of standard cells in dimension 0, 1, 2, 3. */
    std::vector<unsigned long> numStandardCells;
    /** number of dual cells in dimension 0, 1, 2, 3. */
    std::vector<unsigned long> numDualCells;
    /** number of (standard) boundary cells in dimension 0, 1, 2. */
    std::vector<unsigned long> numBdryCells;

    /** non-ideal vertices */
    NIndexedArray<unsigned long> sNIV;
    /** vertices which are ideal endpoints of edges */
    NIndexedArray<unsigned long> sIEOE;
    /** edges which are ideal end edges of faces */
    NIndexedArray<unsigned long> sIEEOF;
    /** faces which are ideal end faces of tetrahedra */
    NIndexedArray<unsigned long> sIEFOT;
    /** vertices which are not ideal, and nonboundary */
    NIndexedArray<unsigned long> dNINBV;
    /** interior edges ie: non-boundary edges */
    NIndexedArray<unsigned long> dNBE;
    /** non-boundary faces */
    NIndexedArray<unsigned long> dNBF;
    /** boundary, non-ideal vertices */
    NIndexedArray<unsigned long> sBNIV;
    /** boundary non-ideal edges */
    NIndexedArray<unsigned long> sBNIE;
    /** boundary non-ideal faces */
    NIndexedArray<unsigned long> sBNIF;

    /** True if the chain complexes A0,A1,A2,A3,A4, B0,B1,B2,B3,B4,
     ** Bd0,Bd1,Bd2,Bd3, B0Incl,B1Incl,B2Incl are computed */
    bool chainComplexesComputed;

    /** 0th term in chain complex for cellular homology, using standard
        CW-complex struc */
    NMatrixInt* A0;
    /** 1st term in chain complex for cellular homology, using standard
        CW-complex struc */
    NMatrixInt* A1;
    /** 2nd term in chain complex for cellular homology, using standard
        CW-complex struc */
    NMatrixInt* A2;
    /** 3rd term in chain complex for cellular homology, using standard
        CW-complex struc */
    NMatrixInt* A3;
    /** 4th term in chain complex for cellular homology, using standard
        CW-complex struc */
    NMatrixInt* A4;

    /** 0-th term in chain complex for dual cellular homology */
    NMatrixInt* B0;
    /** 1st term in chain complex for dual cellular homology */
    NMatrixInt* B1;
    /** 2nd term in chain complex for dual cellular homology */
    NMatrixInt* B2;
    /** 3rd term in chain complex for dual cellular homology */
    NMatrixInt* B3;
    /** 4th term in chain complex for dual cellular homology */
    NMatrixInt* B4;

    /** 0th term in chain complex for boundary cellular homology */
    NMatrixInt* Bd0;
    /** 1st term in chain complex for boundary cellular homology */
    NMatrixInt* Bd1;
    /** 2nd term in chain complex for boundary cellular homology */
    NMatrixInt* Bd2;
    /** 3rd term in chain complex for boundary cellular homology */
    NMatrixInt* Bd3;

    /** Chain map from C_0 boundary to C_0 manifold, standard coords */
    NMatrixInt* B0Incl;
    /** Chain map from C_1 boundary to C_1 manifold, standard coords */
    NMatrixInt* B1Incl;
    /** Chain map from C_2 boundary to C_2 manifold, standard coords */
    NMatrixInt* B2Incl;

    /** Isomorphism from C_1 dual to C_1 standard */
    NMatrixInt* H1map;

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
     ** routine is the faster than computeHomology() but it's likely a bit
     ** slower than NTriangulation's homology routines. */
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
     * \pre The triangulation is of a connected orientable 3-manifold.
     */
    void computeTorsionLinkingForm();

    /** the prime power decomposition of the torsion subgroup of H1
     ** So if the invariant factors were 2,2,4,3,9,9,27,5,5, this would
     ** be the list: (2, (1, 1, 2)), (3, (1, 2, 2, 3)), (5, (1, 1)) */
    std::vector< std::pair< NLargeInteger, std::vector<unsigned long> > >
        h1PrimePowerDecomp;
    /** p-primary decomposition of the torsion linking form as needed to
     ** construct the Kawauchi-Kojima invariants. */
    std::vector< NMatrixRing<NRational>* > linkingFormPD;

    /** True if torsion linking form is `hyperbolic' */
    bool torsionLinkingFormIsHyperbolic;
    /** True if torsion linking form is `split' */
    bool torsionLinkingFormIsSplit;
    /** True if torsion linking form satisfies the Kawauchi-Kojima 2-torsion
     ** condition */
    bool torsionLinkingFormSatisfiesKKtwoTorCondition;

    /** 1 of 3 Kawauchi-Kojima invariants: this describes the rank of the
     ** torsion subgroup of H1 */
    std::vector< std::pair< NLargeInteger,
    std::vector< unsigned long > > > torRankV;
    /** 2 of 3 Kawauchi-Kojima invariants: this is the sigma-invariant
     ** of 2-torsion. */
    std::vector< NLargeInteger > twoTorSigmaV;
    /** 3 of 3 Kawauchi-Kojima invariants: this is the Legendre symbol
     ** invariant of odd torsion. */
    std::vector< std::pair< NLargeInteger, std::vector< int > > > oddTorLegSymV;

    /** string representing torRankV */
    std::string torsionRankString;
    /** string representing twoTorSigmaV */
    std::string torsionSigmaString;
    /** string representing oddTorLegSymV */
    std::string torsionLegendreString;
    /** comment on what kind of homology spheres the manifold may or may
     ** not embed in. */
    std::string embedabilityString;

public:

    /**
     * Takes as input a triangulation.
     *
     * This class takes its own copy of the input triangulation.  This
     * means that the input triangulation can change or even be
     * destroyed, and this homological data will happily continue to work
     * with the original triangulation as it was first passed to the
     * constructor.
     *
     * @param input the triangulation to use.
     */
    NHomologicalData(const NTriangulation& input);
    /**
     * Copy constructor.
     *
     * @param g the homological data to clone.
     */
    NHomologicalData(const NHomologicalData& g);
    /**
     * Destructor.
     */
    virtual ~NHomologicalData();
    /**
     * Needed as a member of SharableObject
     * Note this only writes pre-computed data.  Thus if you have
     * not yet asked NHomologicalData to compute anything about this
     * triangulation, writeTextShort may be empty. 
     * @param out the stream to write to.
     */
    virtual void writeTextShort(std::ostream& out) const;

    /**
     * This routine gives access to the manifold's homology computed
     * with the regular CW-decomposition.
     * @param q the dimension of the homology group, can be 0, 1, 2 or 3.
     */
    NMarkedAbelianGroup getMH(unsigned q);
    //the manifold's homology groups, computed with the standard CW
    //decomposition. This is typically slower than getDMH? since
    //getDMH? uses the dual cw-decomposition which typically has
    //an order of magnitude less cells.
    /**
     * This routine gives access to the homology of the boundary
     * of the manifold, computed
     * with the regular CW-decomposition.
     *
     * @param q the dimension of the homology group: can be 0, 1 or 2.
     * @return the q-th homology group, in standard cellular homology
     * coordinates
     */
    NMarkedAbelianGroup getBMH(unsigned q);//boundary homology groups

    /**
     * This routine gives access to the homomorphism from the
     * homology of the boundary to the homology of the manifold.
     *
     * @param q the dimension of the map, can be 0, 1 or 2.
     * @return the map from H_q of the boundary to H_q of the manifold,
     * computed in standard coordinates.
     */
    NHomMarkedAbelianGroup getBMmapH(unsigned q);

    /**
     * This routine gives access to the manifold's homology computed
     * with the dual CW-decomposition.
     *
     * @param q the dimension of the homology group, can be 0, 1, 2 or 3.
     * @return H_q of the manifold, computed in the dual CW-decomposition.
     */
    NMarkedAbelianGroup getDMH(unsigned q);

    /**
     * This routine gives access to the isomorphism from getDMH(1) to
     * getMH(1) given by a cellular approximation to the identity map
     * on the manifold.
     *
     * @return The isomorphism from getDMH(1) to getMH(1) computed via
     * a cellular approximation of the identity map from the first 1-skeleton
     * to the 2nd.
     */
    NHomMarkedAbelianGroup getH1cellap();

    /**
     * A list of the number of cells in the standard genuine CW-decomposition
     * of the manifold.
     *
     * In the case that the triangulation is a proper
     * triangulation of a manifold (or delta-complex decomposition) it
     * simply returns the same information as in the NTriangulation
     * vertex, edge, face and tetrahedron lists.
     *
     * In the case that this is an ideal triangulation, this algorithm is
     * returning the details of the corresponding compact manifold with
     * boundary a union of closed surfaces.
     *
     * @return a vector that contains the number of cells in the standard
     * CW-decomposition of the closed manifold.
     */
    std::vector<unsigned long> getNumStandardCells();
    /**
     * A list of the number of cells in the dual CW-decomposition
     * of the manifold. This is typically much smaller than
     * getNumStandardCells().
     *
     * @return A vector that contains the number of cells in the dual
     * CW-decomposition to the triangulation.
     */
    std::vector<unsigned long> getNumDualCells();
    /**
     * A list of the number of cells in the standard CW-decomposition
     * of the boundary of the manifold. This is the subcomplex of
     * the complex used in getNumStandardCells().
     *
     * @return A vector which lists the number of cells in the standard
     * CW-decomposition of the boundary.
     */
    std::vector<unsigned long> getNumBdryCells();
    /**
     * The proper Euler characteristic of the manifold, computed from
     * getNumDualCells. This is the genuine Euler characteristic as defined
     * in any algebraic topology textbooks. It differs from
     * regina::NTriangulation::getEulerCharacteristic(), which handles
     * ideal triangulations in a non-standard way.
     *
     * @return The Euler characteristic of the corresponding compact
     * triangulated 3-manifold ie: ideal vertices are considered surface
     * boundary components.
     */
    long int getEulerChar();

    /**
     * Returns the torsion subgroup rank vector. This is the first of
     * the 3 Kawauchi-Kojima complete invariants of the torsion
     * linking form.  The algorithm assumes the 3-manifold is both
     * orientable and connected.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return A vector that describes the rank of the torsion subgroup of H1,
     * given in prime power form.  Ie it is a vector of pairs (p,x) where
     * p is a prime, and x is its exponent.
     */
    std::vector< std::pair< NLargeInteger,
    std::vector< unsigned long > > > getTorsionRankVector();
    /**
     * Same as getTorsionRankVector() but returns as a human-readable string.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return human-readable prime power factorization of the order of
     * the torsion subgroup of H1.
     */
    std::string getTorsionRankVectorString();
    /**
     * Returns the 2-torsion sigma vector. This is the 2nd of the three
     * Kawauchi-Kojima invariants. Assumes manifold is orientable and
     * connected.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return The Kawauchi-Kojima sigma-vector.
     */
    std::vector< NLargeInteger > getTorsionSigmaVector();
    /**
     * Same as getTorsionSigmaVector() but returns as a human-readable string.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return The Kawauchi-Kojima sigma-vector in human readable form.
     */
    std::string getTorsionSigmaVectorString();

    /**
     * Returns the odd p-torsion Legendre symbol vector. This is the
     * last of the three Kawauchi-Kojima invariants.  Assumes the
     * manifold is orientable and connected.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return The Legendre symbol vector associated to the torsion
     * linking form.
     */
    std::vector< std::pair< NLargeInteger, std::vector< int > > >
        getLegendreSymbolVector();
    /**
     * Same as getLegendreSymbolVector() but returns as a human-readable string.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return the Legendre symbol vector in human-readable form.
     */
    std::string getTorsionLegendreSymbolVectorString();

    /**
     * Returns true iff torsion linking form is `hyperbolic' in
     * the linking-form sense of the word.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return bool value, true iff torsion linking form is hyperbolic.
     */
    bool formIsHyperbolic();
    /**
     * Returns true iff torsion linking form is split.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return bool value, true if the linking form is split.
     */
    bool formIsSplit();
    /**
     * Returns true iff torsion linking form satisfies the
     * Kawauchi-Kojima 2-torsion condition that on all elements
     * x of order 2^k, 2^{k-1}form(x,x) == 0.  This is a
     * neccessary condition for an orientable 3-manifold
     * perhaps with boundary to embed in a homology 4-sphere.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return bool value, true iff form satisfies the 2-torsion
     * condition of Kawauchi-Kojima.
     */
    bool formSatKK();
    /**
     * Returns a comment on if the manifold might embed in
     * a homology 3-sphere or 4-sphere. Basically, it runs
     * through all the Kawauchi-Kojima conditions, plus a
     * few other `elementary' conditions.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return string which gives a one-line description of what
     * is known about where this manifold embeds, based solely
     * on the manifold's homological data.
     */
    std::string getEmbeddabilityComment();
};

/*@}*/

// Inline functions for NHomologicalData

// constructor
inline NHomologicalData::NHomologicalData(const NTriangulation& input):
        ShareableObject(),

        mHomology0(0),
        mHomology1(0),
        mHomology2(0),
        mHomology3(0),

        bHomology0(0),
        bHomology1(0),
        bHomology2(0),

        bmMap0(0),
        bmMap1(0),
        bmMap2(0),

        dmHomology0(0),
        dmHomology1(0),
        dmHomology2(0),
        dmHomology3(0),

        dmTomMap1(0),

        ccIndexingComputed(false),

        numStandardCells(0), numDualCells(0), numBdryCells(0),

        sNIV(0), sIEOE(0), sIEEOF(0), sIEFOT(0),
        dNINBV(0), dNBE(0), dNBF(0),
        sBNIV(0), sBNIE(0), sBNIF(0),

        chainComplexesComputed(false),

        A0(0), A1(0), A2(0), A3(0), A4(0),
        B0(0), B1(0), B2(0), B3(0), B4(0),
        Bd0(0),Bd1(0),Bd2(0),Bd3(0),
        B0Incl(0),B1Incl(0),B2Incl(0),
        H1map(0),

        torsionFormComputed(false),
        h1PrimePowerDecomp(0), linkingFormPD(0) 
{
    tri = new NTriangulation(input);
}



// copy constructor
inline NHomologicalData::NHomologicalData(const NHomologicalData& g) :
ShareableObject() {
    tri = new NTriangulation(*g.tri);

    // regular cellular homology
    if (g.mHomology0) mHomology0 = new NMarkedAbelianGroup(*g.mHomology0);
    else mHomology0=0;
    if (g.mHomology1) mHomology1 = new NMarkedAbelianGroup(*g.mHomology1);
    else mHomology1=0;
    if (g.mHomology2) mHomology2 = new NMarkedAbelianGroup(*g.mHomology2);
    else mHomology2=0;
    if (g.mHomology3) mHomology3 = new NMarkedAbelianGroup(*g.mHomology3);
    else mHomology3=0;

    // regular boundary cellular homology
    if (g.bHomology0) bHomology0 = new NMarkedAbelianGroup(*g.bHomology0);
    else bHomology0=0;
    if (g.bHomology1) bHomology1 = new NMarkedAbelianGroup(*g.bHomology1);
    else bHomology1=0;
    if (g.bHomology2) bHomology2 = new NMarkedAbelianGroup(*g.bHomology2);
    else bHomology2=0;

    // boundary map
    if (g.bmMap0) bmMap0 = new NHomMarkedAbelianGroup(*g.bmMap0);
    else bmMap0=0;
    if (g.bmMap1) bmMap1 = new NHomMarkedAbelianGroup(*g.bmMap1);
    else bmMap1=0;
    if (g.bmMap2) bmMap2 = new NHomMarkedAbelianGroup(*g.bmMap2);
    else bmMap2=0;

    // dual cellular homology
    if (g.dmHomology0) dmHomology0 = new NMarkedAbelianGroup(*g.dmHomology0);
    else dmHomology0=0;
    if (g.dmHomology1) dmHomology1 = new NMarkedAbelianGroup(*g.dmHomology1);
    else dmHomology1=0;
    if (g.dmHomology2) dmHomology2 = new NMarkedAbelianGroup(*g.dmHomology2);
    else dmHomology2=0;
    if (g.dmHomology3) dmHomology3 = new NMarkedAbelianGroup(*g.dmHomology3);
    else dmHomology3=0;

    // isomorphism between dual H1 and regular H1.
    if (g.dmTomMap1) dmTomMap1 = new NHomMarkedAbelianGroup(*g.dmTomMap1);
    else dmTomMap1=0;

    // the chain complexes...
    ccIndexingComputed = g.ccIndexingComputed;

    if (ccIndexingComputed) {
        numStandardCells = g.numStandardCells; // number of cells of
                                               // dimension 0, 1, 2, 3.
        numDualCells = g.numDualCells; // dual cells
        numBdryCells = g.numBdryCells; // standard boundary cells

        sNIV = g.sNIV;   // non-ideal vertices
        sIEOE = g.sIEOE;  // ideal endpoints of edges
        sIEEOF = g.sIEEOF; // ideal end edges of faces
        sIEFOT = g.sIEFOT; // ideal end faces of tetrahedra
        dNINBV = g.dNINBV; // nonideal nonboundary vertices
        dNBE = g.dNBE;   // non-boundary edges
        dNBF = g.dNBF;   // non-boundary faces
        sBNIV = g.sBNIV;  // boundary non-ideal vertices
        sBNIE = g.sBNIE;  // boundary non-ideal edges
        sBNIF = g.sBNIF;  // boundary non-ideal faces
    }

    chainComplexesComputed = g.chainComplexesComputed;

    if (chainComplexesComputed) {
        A0 = new NMatrixInt(*g.A0);
        A1 = new NMatrixInt(*g.A1);
        A2 = new NMatrixInt(*g.A2);
        A3 = new NMatrixInt(*g.A3);
        A4 = new NMatrixInt(*g.A4);
        B0 = new NMatrixInt(*g.B0);
        B1 = new NMatrixInt(*g.B1);
        B2 = new NMatrixInt(*g.B2);
        B3 = new NMatrixInt(*g.B3);
        B4 = new NMatrixInt(*g.B4);
        Bd0 = new NMatrixInt(*g.Bd0);
        Bd1 = new NMatrixInt(*g.Bd1);
        Bd2 = new NMatrixInt(*g.Bd2);
        Bd3 = new NMatrixInt(*g.Bd3);
        B0Incl = new NMatrixInt(*g.B0Incl);
        B1Incl = new NMatrixInt(*g.B1Incl);
        B2Incl = new NMatrixInt(*g.B2Incl);
        H1map = new NMatrixInt(*g.H1map);
    }

    torsionFormComputed = g.torsionFormComputed;
    if (torsionFormComputed) {
        h1PrimePowerDecomp = g.h1PrimePowerDecomp;
        linkingFormPD.resize( g.linkingFormPD.size(), 0 );
        for (unsigned long i=0; i<linkingFormPD.size(); i++)
            linkingFormPD[i] = new NMatrixRing<NRational> (*g.linkingFormPD[i]);
        torsionLinkingFormIsHyperbolic = g.torsionLinkingFormIsHyperbolic;
        torsionLinkingFormIsSplit = g.torsionLinkingFormIsSplit;
        torsionLinkingFormSatisfiesKKtwoTorCondition =
            g.torsionLinkingFormSatisfiesKKtwoTorCondition;
        torRankV = g.torRankV;
        twoTorSigmaV = g.twoTorSigmaV;
        oddTorLegSymV = g.oddTorLegSymV;
        torsionRankString = g.torsionRankString;
        torsionSigmaString = g.torsionSigmaString;
        torsionLegendreString = g.torsionLegendreString;
        embedabilityString = g.embedabilityString;
    }
}

// destructor
inline NHomologicalData::~NHomologicalData() {
    delete tri;

    if (mHomology0) delete mHomology0;
    if (mHomology1) delete mHomology1;
    if (mHomology2) delete mHomology2;
    if (mHomology3) delete mHomology3;

    if (bHomology0) delete bHomology0;
    if (bHomology1) delete bHomology1;
    if (bHomology2) delete bHomology2;

    if (bmMap0) delete bmMap0;
    if (bmMap1) delete bmMap1;
    if (bmMap2) delete bmMap2;

    if (dmHomology0) delete dmHomology0;
    if (dmHomology1) delete dmHomology1;
    if (dmHomology2) delete dmHomology2;
    if (dmHomology3) delete dmHomology3;

    if (dmTomMap1) delete dmTomMap1;

    if (chainComplexesComputed) {
        delete A0;
        delete A1;
        delete A2;
        delete A3;
        delete A4;
        delete B0;
        delete B1;
        delete B2;
        delete B3;
        delete B4;
        delete Bd0;
        delete Bd1;
        delete Bd2;
        delete Bd3;
        delete B0Incl;
        delete B1Incl;
        delete B2Incl;
        delete H1map;
    }

    if (torsionFormComputed) {
        for (unsigned long i=0; i<linkingFormPD.size(); i++)
            delete linkingFormPD[i];
    }
}

inline std::vector<unsigned long> NHomologicalData::getNumStandardCells()
{
    // number of cells of dimension 0, 1, 2, 3.
    computeccIndexing();
    return numStandardCells;
}

inline std::vector<unsigned long> NHomologicalData::getNumDualCells()
{
    // dual cells
    computeccIndexing();
    return numDualCells;
}

inline std::vector<unsigned long> NHomologicalData::getNumBdryCells()
{
    // standard boundary cells
    computeccIndexing();
    return numBdryCells;
}

inline long int NHomologicalData::getEulerChar()
{
    // euler characteristic
    computeccIndexing();
    return numDualCells[0]-numDualCells[1]+numDualCells[2]-numDualCells[3];
}

inline std::vector< std::pair< NLargeInteger,
std::vector< unsigned long > > > NHomologicalData::getTorsionRankVector() 
{
    computeTorsionLinkingForm();
    return torRankV;
}
inline std::vector< NLargeInteger > NHomologicalData::getTorsionSigmaVector()  
{
    computeTorsionLinkingForm();
    return twoTorSigmaV;
}
inline std::vector< std::pair< NLargeInteger, std::vector< int > > >
NHomologicalData::getLegendreSymbolVector() 
{
    computeTorsionLinkingForm();
    return oddTorLegSymV;
}

inline bool NHomologicalData::formIsSplit() 
{
     computeTorsionLinkingForm();
    return torsionLinkingFormIsSplit;
}
inline bool NHomologicalData::formSatKK() 
{
      computeTorsionLinkingForm();
   return torsionLinkingFormSatisfiesKKtwoTorCondition;
}
inline std::string NHomologicalData::getTorsionRankVectorString() 
{
     computeTorsionLinkingForm();
    return torsionRankString;
}

inline std::string NHomologicalData::getTorsionSigmaVectorString()
{
    computeTorsionLinkingForm();
    return torsionSigmaString;
}

inline std::string NHomologicalData::getTorsionLegendreSymbolVectorString() 
{
     computeTorsionLinkingForm();
    return torsionLegendreString;
}

inline std::string NHomologicalData::getEmbeddabilityComment() 
{
     computeTorsionLinkingForm();
    return embedabilityString;
}




} // namespace regina

#endif

