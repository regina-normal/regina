
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file triangulation/nhomologicaldata.h
 *  \brief Deals with all the details of the cellular homology of a 3-manifold.
 */

#ifndef __NHOMOLOGICALDATA_H
#ifndef __DOXYGEN
#define __NHOMOLOGICALDATA_H
#endif

#include "algebra/nmarkedabeliangroup.h"
#include "maths/nrational.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nindexedarray.h"
#include "utilities/ptrutils.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace regina {

class NTriangulation;

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Data type that deals with all the detailed homological information in a
 * manifold.  This information includes:
 *
 * - the manifold's homology;
 * - the boundary's homology;
 * - the map from boundary -> manifold;
 * - the dual cellular homology;
 * - the isomorphism on H1 from the dual cellular homology to the regular
 *   cellular homology;
 * - the H1 torsion form;
 * - the Kawauchi-Kojima invariants of torsion linking forms.
 * - the intersection pairing H_2 x H_2 --> H_1 and the corresponding triple product
 *   H_2 x H_2 x H_2 --> Z.
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
 * First we list the cell orderings for the <i>standard CW decomposition</i>,
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
 * Next we list the cell orderings for the <i>dual CW decomposition</i>:
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
 * \testpart
 *
 * \todo \optlong longer term, add cohomology w/coeffs, then once Hom, Ext, Tor, and \otimes
 * 		are added to NMarkedAbelianGroup, add the maps:
 *	UCT  0 --> Ext( H_{i-1}(M;Z); Z_p ) --> H^i(M;Z_p) --> Hom(H_i(M;Z); Z_p) --> 0 
 *      UCT  0 --> H_i(M;Z) \otimes Z_p --> H_i(M;Z_p) --> Tor(H_{i-1}(M;Z), Z_p) --> 0
 *      Would it be worth it?  Dunno.  Via UCT you can get at everything pretty effectively
 *      using only homology with integral coefficients. 
 * \todo \optlong torsion linking form and intersection product as maps out of tensor products.
 *
 * @author Ryan Budney
 */
class NHomologicalData : public ShareableObject {
private:
    /**
     * Stored pointer to a valid triangulation. All routines use this
     * triangulation as reference.
     * This is the triangulation that it is initialized by.
     */
    std::auto_ptr<NTriangulation> tri;

    /**
     * Pointer to homology groups in standard cellular coordinates, in dimensions
     * 0 through 3 respectively. 0 if it has not yet been computed.
     */
    std::auto_ptr<NMarkedAbelianGroup> mHomology0, mHomology1, mHomology2, mHomology3;

    /**
     * Pointer to the boundary homology groups in standard cellular
     * coordinates, dimensions 0 through 2 respectively. 0 if it has not yet been computed.
     */
    std::auto_ptr<NMarkedAbelianGroup> bHomology0, bHomology1, bHomology2;

    /**
     * Pointer to the boundary inclusion on homology, standard
     * cellular coordinates, or 0 if it has not yet been computed.
     */
    std::auto_ptr<NHomMarkedAbelianGroup> bmMap0, bmMap1, bmMap2;

    /**
     * Pointer to the homology groups in dual cellular coordinates, or
     * 0 if it has not yet been computed.
     */
    std::auto_ptr<NMarkedAbelianGroup> dmHomology0, dmHomology1, dmHomology2, dmHomology3;

    /**
     * Pointer to the cellular approx of the identity H1(M) --> H1(M)
     * from dual to standard cellular coordinates, or 0 if it has not yet
     * been computed.
     */
    std::auto_ptr<NHomMarkedAbelianGroup> dmTomMap1;

    /**
     * Pointers to the homology in mixed cellular coordinates. 
     */
    std::auto_ptr<NMarkedAbelianGroup> mH0, mH1, mH2, mH3;

    /** 
     * Pointers to the homomorphisms between dmHomology, mH and dmHomology groups
     * respectively. 
     */
    std::auto_ptr<NHomMarkedAbelianGroup> SMHom0, SMHom1, SMHom2, SMHom3, 
					  DMHom0, DMHom1, DMHom2, DMHom3;

    // below here and the public declaration go the internal bits of
    // data that are not publicly accessible...

    // the chain complexes for the regular cellular homology

    /** true if the indexing of the chain complexes is complete */
    bool ccIndexingComputed;

    /** number of standard cells in dimension 0, 1, 2, 3. */
    unsigned long numStandardCells[4];
    /** number of dual cells in dimension 0, 1, 2, 3. */
    unsigned long numDualCells[4];
    /** number of (standard) boundary cells in dimension 0, 1, 2. */
    unsigned long numBdryCells[3];
    /** number of mixed cells in dimension 0, 1, 2, 3 */
    unsigned long numMixCells[4];
    /** number of ideal boundary cells */
    unsigned long numIdBdryCells[3];

    /** non-ideal vertices */
    NIndexedArray<unsigned long> sNIV;
    /** vertices which are ideal endpoints of edges, stored as 2*edge index + {0,1} endpt data */
    NIndexedArray<unsigned long> sIEOE;
    /** edges which are ideal end edges of faces, stored as 3*face index + {0,1,2} edge data */
    NIndexedArray<unsigned long> sIEEOF;
    /** faces which are ideal end faces of tetrahedra, stored as 4*tet index + {0,1,2,3} face data */
    NIndexedArray<unsigned long> sIEFOT;
    /** vertices which are not ideal, and nonboundary, stored as vertex index */
    NIndexedArray<unsigned long> dNINBV;
    /** interior edges ie: non-boundary edges, stored as edge index */
    NIndexedArray<unsigned long> dNBE;
    /** non-boundary faces, stored as face index */
    NIndexedArray<unsigned long> dNBF;
    /** boundary, non-ideal vertices, stored as vertex index */
    NIndexedArray<unsigned long> sBNIV;
    /** boundary non-ideal edges, stored as edge index */
    NIndexedArray<unsigned long> sBNIE;
    /** boundary non-ideal faces, stored as face index */
    NIndexedArray<unsigned long> sBNIF;

    /** True if the chain complexes A0,A1,A2,A3,A4, B0,B1,B2,B3,B4,
     ** Bd0,Bd1,Bd2,Bd3, B0Incl,B1Incl,B2Incl are computed */
    bool chainComplexesComputed;

    /** 0th term in chain complex for cellular homology, using standard
        CW-complex struc */
    std::auto_ptr<NMatrixInt> A0;
    /** 1st term in chain complex for cellular homology, using standard
        CW-complex struc */
    std::auto_ptr<NMatrixInt> A1;
    /** 2nd term in chain complex for cellular homology, using standard
        CW-complex struc */
    std::auto_ptr<NMatrixInt> A2;
    /** 3rd term in chain complex for cellular homology, using standard
        CW-complex struc */
    std::auto_ptr<NMatrixInt> A3;
    /** 4th term in chain complex for cellular homology, using standard
        CW-complex struc */
    std::auto_ptr<NMatrixInt> A4;

    /** 0-th term in chain complex for dual cellular homology */
    std::auto_ptr<NMatrixInt> B0;
    /** 1st term in chain complex for dual cellular homology */
    std::auto_ptr<NMatrixInt> B1;
    /** 2nd term in chain complex for dual cellular homology */
    std::auto_ptr<NMatrixInt> B2;
    /** 3rd term in chain complex for dual cellular homology */
    std::auto_ptr<NMatrixInt> B3;
    /** 4th term in chain complex for dual cellular homology */
    std::auto_ptr<NMatrixInt> B4;

    /** 0th term in chain complex for boundary cellular homology */
    std::auto_ptr<NMatrixInt> Bd0;
    /** 1st term in chain complex for boundary cellular homology */
    std::auto_ptr<NMatrixInt> Bd1;
    /** 2nd term in chain complex for boundary cellular homology */
    std::auto_ptr<NMatrixInt> Bd2;
    /** 3rd term in chain complex for boundary cellular homology */
    std::auto_ptr<NMatrixInt> Bd3;

    /** Chain map from C_0 boundary to C_0 manifold, standard coords */
    std::auto_ptr<NMatrixInt> B0Incl;
    /** Chain map from C_1 boundary to C_1 manifold, standard coords */
    std::auto_ptr<NMatrixInt> B1Incl;
    /** Chain map from C_2 boundary to C_2 manifold, standard coords */
    std::auto_ptr<NMatrixInt> B2Incl;

    /** Isomorphism from C_1 dual to C_1 standard */
    std::auto_ptr<NMatrixInt> H1map;

    /** 0th term in chain complex for mixed cellular homology C0 -> 0 */
    std::auto_ptr<NMatrixInt> M0;
    /** 1st term in chain complex for mixed cellular homology C1 -> C0 */
    std::auto_ptr<NMatrixInt> M1;
    /** 2nd term in chain complex for mixed cellular homology C2 -> C1 */
    std::auto_ptr<NMatrixInt> M2;
    /** 3rd term in chain complex for mixed cellular homology C3 -> C2 */
    std::auto_ptr<NMatrixInt> M3;
    /** 4th term in chain complex for mixed cellular homology 0 -> C3 */
    std::auto_ptr<NMatrixInt> M4;

    /** chain map standard to mixed 0 */
    std::auto_ptr<NMatrixInt> AM0;
    /** chain map AM1 */
    std::auto_ptr<NMatrixInt> AM1;
    /** chain map AM2 */
    std::auto_ptr<NMatrixInt> AM2;
    /** chain map AM3 */
    std::auto_ptr<NMatrixInt> AM3;
    /** chain map dual to mixed 0 */
    std::auto_ptr<NMatrixInt> BM0;
    /** chain map BM1 */
    std::auto_ptr<NMatrixInt> BM1;
    /** chain map BM2 */
    std::auto_ptr<NMatrixInt> BM2;
    /** chain map BM3 */
    std::auto_ptr<NMatrixInt> BM3;

    /** Call this routine to demand the indexing of the chain complexes. */
    void computeccIndexing();
    /** This routine computes all the chain complexes. */
    void computeChainComplexes();
    /** Compute the homology from the point of view of the barycentric CW-decomposition.
	This will also compute all the relevant maps to the simplicial homology of the
	triangulation and the CW-homology of the dual polyhedral decomposition. **/
    void computeBaryCC();

    /** true when the torsionlinking form has been computed. */
    bool torsionFormComputed;
    /**
     * This routine computes the H1 torsion linking form. It is only
     * well-defined for orientable 3-manifolds, so don't bother calling
     * this routine unless you know the manifold is orientable.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     */
    void computeTorsionLinkingForm();
    /**
     * Unlike computeTorsionLinkingForm(), this routine \e can be called
     * for non-orientable manifolds (in which case we look at the
     * orientable double cover).
     *
     * \pre The triangulation is of a connected 3-manifold.
     */
    void computeEmbeddabilityString();

    /** the prime power decomposition of the torsion subgroup of H1
     ** So if the invariant factors were 2,2,4,3,9,9,27,5,5, this would
     ** be the list: (2, (1, 1, 2)), (3, (1, 2, 2, 3)), (5, (1, 1)) */
    std::vector< std::pair< NLargeInteger, std::vector<unsigned long> > >
        h1PrimePowerDecomp;
    /** p-primary decomposition of the torsion linking form as needed to
     ** construct the Kawauchi-Kojima invariants. */
    std::vector< NMatrixRing<NRational>* > linkingFormPD;

    /** True if torsion linking form is `hyperbolic'.   */
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
    std::string embeddabilityString;

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
     * @param h the homological data to clone.
     */
    NHomologicalData(const NHomologicalData& h);
    /**
     * Destructor.
     */
    virtual ~NHomologicalData();
    /**
     * Short text representation as required by SharableObject.
     *
     * Note this only writes pre-computed data.  Thus if you have
     * not yet asked NHomologicalData to compute anything about this
     * triangulation, writeTextShort may be empty. 
     *
     * @param out the stream to write to.
     */
    virtual void writeTextShort(std::ostream& out) const;

    /**
     * This routine gives access to the manifold's homology computed
     * with the regular CW-decomposition.
     *
     * This routine is typically slower than getDualHomology(), since
     * getDualHomology() uses the dual CW-decomposition which typically
     * has an order of magnitude fewer cells.
     *
     * Note that the groups returned by standardHomology() and getDualHomology()
     * are isomorphic, though they are generally described by different
     * presentations.
     *
     * @param q the dimension of the homology group: can be 0, 1, 2 or 3.
     * @return the q-th homology group, computed in the standard
     * CW-decomposition.
     */
    const NMarkedAbelianGroup& standardHomology(unsigned q);
    /**
     * This routine gives access to the homology of the boundary
     * of the manifold, computed with the regular CW-decomposition.
     *
     * @param q the dimension of the homology group: can be 0, 1 or 2.
     * @return the q-th boundary homology group, in standard cellular
     * homology coordinates
     */
    const NMarkedAbelianGroup& boundaryHomology(unsigned q);

    /**
     * This routine gives access to the homomorphism from the
     * homology of the boundary to the homology of the manifold.
     *
     * @param q the dimension of the map: can be 0, 1 or 2.
     * @return the map from H_q of the boundary to H_q of the manifold,
     * computed in standard coordinates.
     */
    const NHomMarkedAbelianGroup& boundaryHomologyMap(unsigned q);

    /**
     * This routine gives access to the manifold's homology computed
     * with the dual CW-decomposition.
     *
     * This routine is typically faster than standardHomology() since the
     * dual CW-decomposition generally has far fewer cells.
     *
     * Note that the groups returned by standardHomology() and getDualHomology()
     * are isomorphic, though they are generally described by different
     * presentations.
     *
     * @param q the dimension of the homology group: can be 0, 1, 2 or 3.
     * @return the q-th homology group, computed in the dual CW-decomposition.
     */
    const NMarkedAbelianGroup& dualHomology(unsigned q);

    /**
     * This routine gives access to the manifold's homology computed
     * with the mixed dual+regular cellular decomposition. This is
     * an expensive operation and should only be called if you're interested
     * in the triple product H_2 x H_2 --> H_1
     *
     * Note that these groups are isomorphic to the groups given by 
     * standardHomology() and getDualHomology() though they use different, simpler
     * coordinates in their computations
     *
     * @param q the dimension of the homology group: can be 0, 1, 2 or 3.
     * @return the q-th homology group, computed in the dual CW-decomposition.
     */
    const NMarkedAbelianGroup& mixedHomology(unsigned q);

    /**
     * Returns the homomorphism from standard to mixed cellular homology
     */
    const NHomMarkedAbelianGroup& standardToMixedHom(unsigned q);
    /**
     * Returns the homomorphism from dual to mixed cellular homology
     */
    const NHomMarkedAbelianGroup& dualToMixedHom(unsigned q);

    /**
     * Returns the isomorphism from getDualHomology(1) to standardHomology(1)
     * given by a cellular approximation to the identity map on the manifold.
     *
     * @return The isomorphism from getDualHomology(1) to standardHomology(1)
     * computed via a cellular approximation of the identity map from
     * the first 1-skeleton to the second.
     */
    const NHomMarkedAbelianGroup& fastDualToStandardH1();

    /**
     * Returns the number of cells of the given dimension
     * in the standard genuine CW-decomposition of the manifold.
     *
     * In the case that the triangulation is a proper
     * triangulation of a manifold (or delta-complex decomposition) it
     * simply returns the same information as in the NTriangulation
     * vertex, edge, face and tetrahedron lists.
     *
     * In the case that this is an ideal triangulation, this algorithm
     * returns the details of the corresponding compact manifold with
     * boundary a union of closed surfaces.
     *
     * @param dimension the dimension of the cells in question; this must
     * be 0, 1, 2 or 3.
     * @return the number of cells of the given dimension in the standard
     * CW-decomposition of the closed manifold.
     */
    unsigned long standardCellCount(unsigned dimension);
    /**
     * Returns the number of cells of the given dimension
     * in the dual CW-decomposition of the manifold. This is typically
     * much smaller than getNumStandardCells().
     *
     * @param dimension the dimension of the cells in question; this must
     * be 0, 1, 2 or 3.
     * @return the number of cells of the given dimension in the dual
     * CW-decomposition to the triangulation.
     */
    unsigned long dualCellCount(unsigned dimension);
    /**
     * Returns the number of cells of the given dimension in the
     * standard CW-decomposition of the boundary of the manifold.
     * This is a subcomplex of the complex used in getNumStandardCells().
     *
     * @param dimension the dimension of the cells in question; this must
     * be 0, 1 or 2.
     * @return the number of cells of the given dimension in the standard
     * CW-decomposition of the boundary.
     */
    unsigned long boundaryCellCount(unsigned dimension);

    /**
     * Returns the number of cells in the mixed cellular decomposition 
     */
    unsigned long mixedCellCount(unsigned dimension);

    /**
     * The proper Euler characteristic of the manifold, computed from
     * the dual CW-decomposition.
     *
     * This routine calculates the Euler characteristic of the
     * corresponding compact triangulated 3-manifold, with each ideal
     * vertex treated as a surface boundary component.
     *
     * This routine returns the same value as
     * NTriangulation::getEulerCharManifold(), though it computes it
     * in a different way.
     *
     * On the other hand, this routine differs from
     * NTriangulation::getEulerCharTri(), which handles ideal triangulations
     * in a non-standard way (treating each ideal vertex as just a single
     * vertex).
     *
     * @return the Euler characteristic of the corresponding compact
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
     * \ifacespython Not available, though the string routine
     * getTorsionRankVectorString() can still be used.
     *
     * @return the torsion form rank vector.
     */
    const std::vector< std::pair< NLargeInteger,
        std::vector< unsigned long > > >& torsionRankVector();
    /**
     * Same as getTorsionRankVector() but returns as a human-readable string.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return human-readable prime power factorization of the order of
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
     * \ifacespython Not available, though the string routine
     * getTorsionSigmaVectorString() can still be used.
     *
     * @return the Kawauchi-Kojima sigma-vector.
     */
    const std::vector<NLargeInteger>& torsionSigmaVector();
    /**
     * Same as getTorsionSigmaVector() but returns as a human-readable string.
     * This is an orientation-sensitive invariant.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return the Kawauchi-Kojima sigma-vector in human readable form.
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
     * \ifacespython Not available, though the string routine
     * getTorsionLegendreSymbolVectorString() can still be used.
     *
     * @return the Legendre symbol vector associated to the torsion
     * linking form.
     */
    const std::vector< std::pair< NLargeInteger, std::vector< int > > >&
        torsionLegendreSymbolVector();
    /**
     * Same as getTorsionLegendreSymbolVector() but returns as a
     * human-readable string.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return the Legendre symbol vector in human-readable form.
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
     * @return \c true iff the torsion linking form is hyperbolic.
     */
    bool formIsHyperbolic();
    /**
     * Returns true iff the torsion linking form is split.
     *
     * \pre The triangulation is of a connected orientable 3-manifold.
     *
     * @return \c true iff the linking form is split.
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
     * @return \c true iff the form satisfies the 2-torsion
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
     * @return a string giving a one-line description of what
     * is known about where this manifold embeds, based solely
     * on the manifold's homological data.
     */
    const std::string& embeddabilityComment();

    /**
     * Computes the image H_2(M;R) x H_2(M;R) --> H_1(M;R)
     * as an abelian group. R represents any cyclic group. 
     * Where the map is the H_2-intersection form -- Poincare Dual to the cup product 
     * pairing H^1(M,\partial M;R) \otimes H^1(M, \partial M;R) --> H^2(M, \partial M;R).
     *
     * Warning: the current implementation can be painfully slow as the linear algebra gets a little
     *  out of control.
     *
     * \pre The manifold needs to be oriented unless p==2, in which case non-orientable
     *      manifolds are fine.
     */
    NMarkedAbelianGroup imgH2form(unsigned long p=0);

    /**
     * Runs through all the various standard homomorphisms between boundary, standard, dual and mixed
     *  homologies and checks the matrices defining them really are chain maps.
     *
     * This procedure is largely for debugging purposes as in any release this should always produce
     *  true.
     *
     * @return true if all homomorphisms are defined by matrices that really are chain maps of
     *         chain complexes.
     */
    bool verifyChainComplexes();

    /**
     * If all the chain complexes check out, might as well verify a few basic isomorphisms as well. 
     * There's the isomorphisms between all the homology groups in the 3 coordinate systems, and also
     * there's the cellular homotopy between H1 in standard and dual coordiantes. 
     *
     * This is mainly for debugging purposes as in any release this should always produce an answer
     * of true. 
     *
     * @return true provided every natural map between the various homology groups computable that
     *         should be isomorphisms, actually are isomorphisms. 
     */
    bool verifyCoordinateIsomorphisms(NLargeInteger p = NLargeInteger::zero);

};

/*@}*/

// Inline functions for NHomologicalData

// constructor
inline NHomologicalData::NHomologicalData(const NTriangulation& input):
        ShareableObject(),

        tri(new NTriangulation(input)),

        ccIndexingComputed(false),

        sNIV(0), sIEOE(0), sIEEOF(0), sIEFOT(0),
        dNINBV(0), dNBE(0), dNBF(0),
        sBNIV(0), sBNIE(0), sBNIF(0),

        chainComplexesComputed(false),

        torsionFormComputed(false),
        h1PrimePowerDecomp(0), linkingFormPD(0) 
{
    std::fill(numStandardCells, numStandardCells + 4, 0);
    std::fill(numDualCells, numDualCells + 4, 0);
    std::fill(numBdryCells, numBdryCells + 3, 0);
    std::fill(numIdBdryCells, numIdBdryCells + 3, 0);
    std::fill(numMixCells, numMixCells+4, 0);
}



// copy constructor
inline NHomologicalData::NHomologicalData(const NHomologicalData& g) :
        ShareableObject(),

        tri(clonePtr(g.tri)),

        mHomology0(clonePtr(g.mHomology0)), mHomology1(clonePtr(g.mHomology1)),
        mHomology2(clonePtr(g.mHomology2)), mHomology3(clonePtr(g.mHomology3)),

        bHomology0(clonePtr(g.bHomology0)), bHomology1(clonePtr(g.bHomology1)),
        bHomology2(clonePtr(g.bHomology2)),

        bmMap0(clonePtr(g.bmMap0)), bmMap1(clonePtr(g.bmMap1)), bmMap2(clonePtr(g.bmMap2)),

        dmHomology0(clonePtr(g.dmHomology0)),      dmHomology1(clonePtr(g.dmHomology1)),
        dmHomology2(clonePtr(g.dmHomology2)),      dmHomology3(clonePtr(g.dmHomology3)),

        dmTomMap1(clonePtr(g.dmTomMap1)),

	mH0(clonePtr(g.mH0)), mH1(clonePtr(g.mH1)), mH2(clonePtr(g.mH2)), mH3(clonePtr(g.mH3)),

	SMHom0(clonePtr(g.SMHom0)), SMHom1(clonePtr(g.SMHom1)), 
	SMHom2(clonePtr(g.SMHom2)), SMHom3(clonePtr(g.SMHom3)), 
	DMHom0(clonePtr(g.DMHom0)), DMHom1(clonePtr(g.DMHom1)), 
	DMHom2(clonePtr(g.DMHom2)), DMHom3(clonePtr(g.DMHom3)), 

        ccIndexingComputed(g.ccIndexingComputed),

        chainComplexesComputed(g.chainComplexesComputed),
        A0(clonePtr(g.A0)), A1(clonePtr(g.A1)), A2(clonePtr(g.A2)),
        A3(clonePtr(g.A3)), A4(clonePtr(g.A4)), B0(clonePtr(g.B0)), 
        B1(clonePtr(g.B1)), B2(clonePtr(g.B2)), B3(clonePtr(g.B3)), B4(clonePtr(g.B4)),
        Bd0(clonePtr(g.Bd0)), Bd1(clonePtr(g.Bd1)), Bd2(clonePtr(g.Bd2)), Bd3(clonePtr(g.Bd3)),
        B0Incl(clonePtr(g.B0Incl)), B1Incl(clonePtr(g.B1Incl)), B2Incl(clonePtr(g.B2Incl)),
        H1map(clonePtr(g.H1map)), M0(clonePtr(g.M0)), M1(clonePtr(g.M1)), M2(clonePtr(g.M2)),
        M3(clonePtr(g.M3)), M4(clonePtr(g.M4)),	AM0(clonePtr(g.AM0)), AM1(clonePtr(g.AM1)), 
	AM2(clonePtr(g.AM2)), AM3(clonePtr(g.AM3)), BM0(clonePtr(g.BM0)), BM1(clonePtr(g.BM1)), 
	BM2(clonePtr(g.BM2)), BM3(clonePtr(g.BM3)), 

        torsionFormComputed(g.torsionFormComputed),
        embeddabilityString(g.embeddabilityString)
{
    // More complex initialisation:
    if (ccIndexingComputed) {
        // Numbers of cells, dual cells and standard boundary cells
        std::copy(g.numStandardCells, g.numStandardCells + 4, numStandardCells);
        std::copy(g.numDualCells, g.numDualCells + 4, numDualCells);
        std::copy(g.numBdryCells, g.numBdryCells + 3, numBdryCells);
        std::copy(g.numIdBdryCells, g.numIdBdryCells + 3, numIdBdryCells);
        std::copy(g.numMixCells, g.numMixCells+4, numMixCells);

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
    }
}

// destructor
inline NHomologicalData::~NHomologicalData() {
    if (torsionFormComputed) {
        for (unsigned long i=0; i<linkingFormPD.size(); i++)
            delete linkingFormPD[i];
    }
}

inline unsigned long NHomologicalData::standardCellCount(unsigned dimension)
{
    // number of cells of dimension 0, 1, 2, 3.
    computeccIndexing();
    return numStandardCells[dimension];
}

inline unsigned long NHomologicalData::dualCellCount(unsigned dimension)
{
    // dual cells
    computeccIndexing();
    return numDualCells[dimension];
}

inline unsigned long NHomologicalData::boundaryCellCount(unsigned dimension)
{
    // standard boundary cells
    computeccIndexing();
    return numBdryCells[dimension];
}

inline unsigned long NHomologicalData::mixedCellCount(unsigned dimension)
{
     computeBaryCC();
     return numMixCells[dimension];
}

inline long int NHomologicalData::eulerChar()
{
    // euler characteristic
    computeccIndexing();
    return numDualCells[0]-numDualCells[1]+numDualCells[2]-numDualCells[3];
}

inline const std::vector< std::pair< NLargeInteger,
std::vector< unsigned long > > >& NHomologicalData::torsionRankVector()
{
    computeTorsionLinkingForm();
    return torRankV;
}
inline const std::vector<NLargeInteger>&
NHomologicalData::torsionSigmaVector()
{
    computeTorsionLinkingForm();
    return twoTorSigmaV;
}
inline const std::vector< std::pair< NLargeInteger, std::vector< int > > >&
NHomologicalData::torsionLegendreSymbolVector()
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
inline const std::string& NHomologicalData::torsionRankVectorString()
{
    computeTorsionLinkingForm();
    return torsionRankString;
}

inline const std::string& NHomologicalData::torsionSigmaVectorString()
{
    computeTorsionLinkingForm();
    return torsionSigmaString;
}

inline const std::string&
NHomologicalData::torsionLegendreSymbolVectorString()
{
    computeTorsionLinkingForm();
    return torsionLegendreString;
}

inline const std::string& NHomologicalData::embeddabilityComment()
{
    computeEmbeddabilityString();
    return embeddabilityString;
}

} // namespace regina

#endif

