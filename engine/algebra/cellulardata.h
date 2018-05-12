/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file algebra/cellulardata.h
 *  \brief Deals with cellular homology of 3 and 4-manifolds. 
 */

#ifndef __CELLULARDATA_H
#ifndef __DOXYGEN
#define __CELLULARDATA_H
#endif

#include "regina-core.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

#include "algebra/markedabeliangroup.h"
#include "algebra/bilinearform.h"
#include "algebra/homgrouppresentation.h"
#include "algebra/grouppresentation.h"
#include "utilities/ptrutils.h"
#include "utilities/boolset.h"
#include "maths/sparsegrid.h"
#include "maths/perm.h"

#include <algorithm>
#include <memory>
#include <vector>

namespace regina {

// TODO these lines not needed anymore?
//class Triangulation<3>;
//class Triangulation<4>;

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Data type that deals with information in a triangulated 3 or 4-manifold 
 * constructible from the triangulation and dual polyhedral decomposition, 
 * specifically:
 *
 * - the manifold's (and its boundary's) homology and cohomology computed 
 *   in various coordinate systems, with coefficients in an arbitrary cyclic 
 *   group.
 * - the maps between coordinate systems, Bocksteins, boundary inclusion, etc. 
 * - the bilinear forms coming from Poincare Duality: 
 *   H_i \otimes H_j --> H_{i+j-n} and torsion linking
 *   \tau H_i \otimes \tau H_{n-i-1} --> Q/Z 
 * - fundamental groups of the manifold, natural submanifolds and maps 
 *   between them.
 * - homology of covering spaces, Alexander polynomials, etc.
 *
 * This class mostly takes a "least effort" approach to all computations. It 
 * only computes what is neccessary for requests.  It also keeps a record of 
 * all previous computations you've made, we'll call it the `precomputed pile'. 
 * If a computation can be sped up by not recomputing some data, it will try to
 * take the shortcut.  At present the only exception to this rule is that all 
 * integer coefficient chain complexes and maps are computed on 
 * initialization, but this is relatively quick.  
 *
 * @pre - Assumes input triangulation is valid and path-connected.
 *
 * \testpart
 *
 * \todo  1) (a) Homomorphisms to finite groups, (b) covering spaces, 
 *        (c) Pi_2 presentations as a module over Pi_1. Immediately: pi1
 *        presentation simplification code needs smoothing, recognition code 
 *        expanding. For (a) GQuotients( F, G ) from GAP is useful.  To define,
 *        use
 *        f:=FreeGroup(n);;
 *        F:=f / [ f.1^2*f.2^3*f.1^-1, blah, blah ];; relator list
 *        G:=SmallGroup(p,q) or SymmetricGroup(n) or AlternatingGroup(n) or 
 *        DihedralGroup(n)
 *        GQuotients(F,G) lists homomorphisms F-->G up to conjugacy in G. 
 *           1 <= q <= NumberSmallGroup(p) for use in SmallGroup
 *         Structuredescription( groupobject ) eg: for i in AllSmallGroups(8) do 
 *        Display(StructureDescription(i)); od; 
 *         PerfectGroup(number), etc...
 *        Maybe create a static class SimpleListOfFiniteGroups and 
 *        BigListOfFiniteGroups, with procedures hasEpiTo. That kind of thing.  
 *        SimpleList can have simple procedures, 
 *        BigList can have more multi-threaded and sophisticated procedures. 
 *
 * \todo  2) Complete collection of homology natural bilinear forms on a 
 *        manifold, spin structures. Derive chain complexes and maps from 
 *        sparsely stored internal data.  Do not store matrices for these.  
 *        We're still missing several coordinate systems for these (co)homology
 *        computations...
 *
 * \todo  3) test suite stuff: 
 *        Move all the test routines out of the CellularData class and put 
 *        them in the test suite proper. 
 *        With the maximal tree code, add tests that will ensure there's the 
 *        appropriate number of edges of each type.  There's the entire tree, 
 *        then the boundary component trees, edge counts, etc.   Need some pi1 
 *        and group simplification tests.  Compute Alexander ideals of manifolds
 *        where H_1 = Z + Z_n and evaluate the polynomial at 1, etc.  
 *        Currently only DUAL-DUAL TLF, maybe add STD-STD as well. 
 *        Also put in flags in the code to add detailed checks if a compiler 
 *        flag triggered, but we want the detailed checks (like "are these 
 *        maps composable?") only then.
 *
 * \todo  4) Make sure all the maximal trees are copy-constructor safe.  I 
 *        forget if the maximal trees and extra normal data has been made 
 *        copy-safe. 
 *
 * \todo  5) New coordinate systems to implement:
 *        MIX_BDRY_coord, MIX_REL_BDRY_coord, DUAL_BDRY_coord, 
 *        DUAL_REL_BDRY_coord and all the various maps.  This is required to 
 *        get at things like H^i M x H^j M --> H^{i+j} M cup products. 
 *        Not complete: Chain complex initialization. chain maps.  
 *        PD / intersection forms Note, current "mixed" chain complex does not 
 *        subdivide ideal boundary.  Is this an issue?  Also, this is needed 
 *        for (1) and (2).
 *
 * \todo  6) Implement circle packings for 2-manifold triangulations.  
 *        Presumably we'll put this straight into the 2-manifold triangulation 
 *        code.  Circle packings represented as the labelling function 
 *        R : vertices -> (0,\infty].  Code to find the circle packing, code
 *        to display it.  Should summarize the algorithm from Collins-Stephenson
 *        paper here. 
 *
 * \todo  7) Implement a manifold-descriptor class.  The idea is to use the 
 *        tools we have to give as accurate as possible a description of the
 *        manifold.  Rather than Ben's NManifold and NStandardManifold 
 *        routines this will be inaccurate if that's all it can do, and precise 
 *        if it can be precise.  For 3-manifold recognition we'll use the
 *        connect sum decomp from Ben's work, but then we'll split recognition 
 *        into various flavours: using pi1, using triangulation tech, using 
 *        SnapPea, and so on. 
 *
 * \todo  \optlong To minimize memory usage we should consider having homs, 
 *        bilinear forms, etc, not store their initialization data, instead 
 *        trusting it to the CellularData stack.
 *        This is a slow-but-ongoing process....  
 *
 * \todo \optlong We'll also eventually need maximal trees in the standard and 
 *        mixed 1-skeleton, to implement Farber-Levine pairings and Poincare 
 *        duality in covering spaces, in general. 
 *
 * \todo \optlong Make writeTextShort and writeTextLong more pleasant to look 
 *        at.  Currently it's not clear what all the computations mean.  It 
 *        could use a general re-think.  The idea now is they're never really 
 *        used for anything important so they haven't been well thought out 
 *        from the start. 
 *
 * \todo \optlong We should add Bocksteins and the long exact sequence 
 *       associated to a change-of-coefficient map.
 *
 * \todo \optlong Why not just move to monoid presentations, and kill 
 *        generators for a max tree in the presentation?   Would be more 
 *        natural and involve less re-indexing. 
 *
 * \todo \optlong Add orientation cocycle, as canonical element in 
 *       H^1(M;Z_2). We could give this as an explicit cocycle in
 *       H^1(M;Z_2) or perhaps it would be better to describe it as
 *       a homomorphism H_1(M,Z_2) --> Z_2 ? Explicit cocycle seems 
 *       fine. Let's return it in the dual CC coordinates. 
 *
 * Guide to CellularData.*.cpp files:
 *
 *       CellularData.cpp - contains only the core routines to call for 
 *       information stored on internal stacks, or calls appropriate routines to 
 *                       generate such information.
 *                           CellularData::integerChainComplex
 *                           CellularData::integerChainMap
 *                           CellularData::unMarkedGroup 
 *                           CellularData::markedGroup
 *                           CellularData::homGroup 
 *                           CellularData::poincarePolynomial 
 *                           CellularData::bilinearForm
 *                           CellularData::groupPresentation
 *                           CellularData::homGroupPresentation. 
 *                           CellularData::alexanderChainComplex
 *
 *  CellularData.init.indexing.cpp 
 *  - the CellularData::CellularData(Triangulation<3> / Triangulation<4>)
 *    constructor, and the routines that set up the internal indices like icIx
 *
 * CellularData.init.cc.cpp 
 *  - contains the homology chain complex initialization routines.
 *
 * CellularData.init.hom.cpp 
 *  - contains the homology chain map initialization routines. 
 *
 * CellularData.init.pi1.cpp 
 *  - contains initialization routines for the fundamental groups presentations 
 *    and maps between them. This includes code to generate maximal trees in
 *    the 1-skeleta. 
 *
 * CellularData.locators.cpp 
 *  - contains the Locator classes for the internal maps that store the various
 *    computations in CellularData. 
 *
 * CellularData.lookups.cpp  
 *  - contains reverse lookups for the cell indices such as icIx, etc. 
 *
 * CellularData.output.cpp 
 *  - contains the writeTextShort and writeTextLong routines.
 *
 * CellularData.tests.cpp 
 *  - contains all the canned diagnostic tests. 
 *
 * @author Ryan Budney
 */
class REGINA_API CellularData {
public:

 /**
  * This enum gives names to the 9 standard CW-structures associated to a 
  * semi-simplicially (ideal) triangulated manifold.  Ideal triangulations are
  * generally not compact.  These cell decomposition are decompositions 
  * corresponding to the compact manifold such that the ideally-triangulated 
  * manifold is a dense subspace and such that its cells are the intersection 
  * of these cells to that subspace. See CellularData::unmarkedGroup, 
  * CellularData::markedGroup, CellularData::homGroup, 
    CellularData::bilinearForm, 
  * CellularData::GroupLocator, CellularData::HomLocator for usage.  
  *
  * See also the valid_coordinate_systems vector, to iterate through implemented 
  *  coordinate systems.
  */
 enum homology_coordinate_system { 
 first_coord,
 /**
  * Is the most natural CW-decomposition of a semi-simplicially (ideal) 
  * triangulated manifold. The top-dimensional cells are the tetrahedra 
  * (of a 3-manifold) or the pentachora (of a 4-manifold). 
  * Dual to DUAL_REL_BDRY_coord.
  */
  STD_coord = first_coord, // indexed by nicIx then icIx 
 /**
  * Is the dual polyhedral decomposition to this CW-decomposition. 
  * The top-dimensional cells correspond to the interior vertices of the 
  * triangulation. Dual to STD_REL_BDRY_coord.
  */
  DUAL_coord, // indexed by dcIx
 /**
  * Is essentially the CW-decomposition of the barycentric subdivision of the 
  * triangulation. For every k-cell in the original triangulation there's k+1 
  * associated k-cells in this triangulation. 
  */
  MIX_coord, // see docs for indexing
 /**
  * This is the standard cell decomposition (which is always a triangulation)
  * of the boundary. So this consists of two natural parts -- the part from 
  * the standard boundary, and the ideal boundary. Dual to DUAL_BDRY_coord
  */
  STD_BDRY_coord, // indexed by bcIx then icIx
 /**
  * This is the same as STD_coord except the boundary cells are thrown away. 
  * Dual to DUAL_coord
  */
  STD_REL_BDRY_coord,  // indexed by rIx  
 /**
  * This is a placeholder so that the tests know where to stop. 
  */    
  last_implemented_coord, 
 /**
  * This is the barycentric subdivision of STD_BDRY_coord. TODO (incomplete)
  */
  MIX_BDRY_coord, 
 /**
  * This is MIX_coord with the boundary cells thrown away. TODO (incomplete)
  */
  MIX_REL_BDRY_coord, 
 /**
  * This is the dual cellular decomposition of the boundary. Dual to 
  * STD_BDRY_coord. TODO (incomplete)
  */
  DUAL_BDRY_coord, 
 /**
  * This is the cell decomposition which is dual to STD_coord TODO (incomplete)
  */
  DUAL_REL_BDRY_coord };

 /**
  * Use this to specify if you want homology (coVariant) or cohomology 
  * (contraVariant) in a (co)homology computation. 
  * See CellularData::unmarkedGroup, CellularData::markedGroup,
  *  CellularData::homGroup, 
  * CellularData::bilinearForm for usage.
  */
 enum variance_type { 
 /**
  * for Homology.  Using this means you ask for the homology of a given chain 
  * complex in some computation.
  */
  coVariant, 
 /**
  * for coHomology. Using this means you ask for the cohomology of a given 
  * chain complex in some computation, in effect this simply applies the 
  * transpose operator to the matrices from the chain complex had you asked 
  * for coVariant. 
  */
  contraVariant };

 /**
  * The routine CellularData::stringInfo takes as input a StringRequest
  * This is a generic any-string-as-return-value routine.  One of the main
  * requestable items is the Kawauch-Kojima classification of torsion linking
  * forms, which we describe below. 
  *
  * Given a finite abelian group G, a torsion linking form is a symmetric 
  * bilinear function f : G x G --> Q/Z.  Given a prime p, let G_p be the
  * subgroup of elements of order a power of p.  G_p,k will be the subgroup of 
  * elements killed by p^k, i.e. the kernel of the multiplication by p^k map 
  * on G.  f has its primary orthogonal splitting f = \oplus_p f_p, where 
  *  f_p : G_p x G_p --> Q/Z.   
  * G_(p,k) is defined as the quotient G_p,k / (G_p,k-1 + pG_p,k+1).  
  * f_p induces a map
  * f_(p,k) : G_(p,k) x G_(p,k) --> Q/Z defined by 
  * f_(p,k)([x],[y]) = p^(k-1) f_p(x,y). 
  * Fact: G_(p,k) is a vector space over Z_p. 
  *
  * See Theorem 4.1 of Kawauchi and Kojima's paper 
  * "Algebraic classification of linking pairings on $3$-manifolds."  
  * Math. Ann. 253 (1980), no. 1, 29--42.  
  *
  * Note: when computing the torsion linking form, CellularData chooses an 
  * orientation convention for the 3-manifold.  Moreover, the Kawauchi-Kojima 
  * invariants are orientation-sensitive.  We describe below how the invariants
  * change under orientation-reversals. 
  */
 enum StringRequest {
 /**
  * Returns the torsion form rank vector. This is one of the three 
  * Kawauchi-Kojima invariants of a torsion linking form.  It is a notation 
  * for the prime power decomposition of the torsion subgroup of H1.  
  * Z_8 would be 2(0 0 1), Z_8 + Z_8 would be 2(0 0 2).  Z_12 would be
  * 2(0 1) 3(1), etc.  It is a list consisting of p(a b c ... ) elements, 
  * meaning the prime p appears in several ways: there are a summands of
  *  Z_p, b of Z_{p^2}, c of Z_{p^3}, etc. 
  * It can also be seen as the dimension over Z_(p^k) of G_(p,k) described 
  * above.   This is orientation-independent, as it only depends on H1 of 
  * the manifold. 
  */
    TORFORM_powerdecomp,
 /**
  * Returns the 2-torsion sigma vector.  This is an object that takes values 
  * in the set {0,1,2,3,4,5,6,7,inf}, i.e. Z_8 \cup {inf}.  It is only relevant 
  * if H1 has 2-torsion.  Torsion linking forms with only odd torsion are 
  * classified by the power decomposition and the Legendre symbol vector.  
  * This is the part of the classification of torsion linking forms due to 
  * Kawauchi and Kojima. 
  *
  * A characteristic element of f_(2,k) is an element q so that 
  * f_(2,k)(q,x)=f_(2,k)(x,x) for all x in G_(2,k).  It's a fact that this 
  * element exists and it is unique, so we denote it by c_(f,k).   
  * If c_(f,k) is zero, we make the k-th element of the sigma string infinity. 
  * Otherwise, consider the function q_k : G_2 / G_2,k--> Q/Z defined
  * by q_k([x]) = 2^(k-1)f(x,x).  It's a fact that the sum 
  *
  *       GS_k := \Sum_{x in G_2/G_2,k} exp(2\pi iq_k(x))
  *
  * satisfies GS_k = |GS_k| e^{2 \pi i \sigma / 8} for some integer j.  This the 
  * \sigma_k. 
  *
  * Notice that under an orientation reversal, inf is fixed, but GS_k is 
  * modified by conjugation, which on sigma is the involution 
  * 0<->0, 1<->7, 2<->6, 3<->5, 4<->4. 
  */
    TORFORM_sigmastring,
 /**
  * Returns the odd p-torsion Legendre symbol vector. This is the part of the 
  * Kawauchi-Kojima classification that's due to Seifert.  If G_(p,k) is a 
  * trivial group this is defined as zero, if G_(p,k) is non-trivial the idea 
  * is to think of f_(p,k) as a symmetric bilinear form on G_(p,k).  Plugging 
  * in a basis for G_(p,k) you get a square matrix whose entries are
  * elements of Z_p.  We take the Legendre symbol of the determinant of this 
  * matrix.  This is a number -1, 0 or 1.   You get 1 if the determinant is 
  * a quadratic residue mod p (i.e. if its congruent mod p to the square of
  * a non-zero integer), 0 if its congruent to 0 mod p,  and -1 otherwise, 
  * sometimes called "a quadratic non-residue mod p". 
  * We do this for all k so that Z_p^k is a subgroup of G. 
  *
  * Under an orientation reversal, at most the sign can change.  And it does 
  * so by a factor of (-1)^{(p-1)rank(G_(p,k))/2}.  
  */
    TORFORM_legendresymbol,
 /**
  * Hyperbolicity, split, and Kawauchi-Kojima 2-torsion embedding obstruction. 
  * Answer will have form "Hyp Yes, Split No, KKObst Yes"
  */
    TORFORM_tests,
 /**
  * Embeddability information. 
  */
    TORFORM_embinfo
    };

 /**
  * Enum for the CellularData::boolInfo call. 
  */
 enum BoolRequest {
 /**
  *  True if there is a Kawauchi-Kojima 2-torsion obstruction to the manifold 
  * embedding in the 4-sphere.  This obstructs the punctured manifold from 
  * embedding. 
  */
    TORFORM_KKtwoTor,
 /**
  *  True if the torsion linking form is of hyperbolic type -- this means the 
  * manifold is a candidate for embedding in the 4-sphere (or a 
  * homology sphere).  False means the torsion linking form is an obstruction 
  * to embedding.
  */
    TORFORM_hyp, 
 /**
  * The form is split
  */
    TORFORM_split
  };

 /**
  *  CellularData stores chain complexes internally in a stack. The Chain 
  * Complex Locator allows unique identification of a chain complex when 
  * passing requests to CellularData::chainComplex Chain complexes are 
  * stored where dim indicates the dimension of the cells in the domain of
  * the map. 
  */
 struct ChainComplexLocator {
        signed long dim; 
        homology_coordinate_system hcs;	

   /**
    *  Initialization constructor.
    *
    * @param newDim - dimension of the cells.  So if you choose n here, the 
    *   matrix you request with this ChainComplexLocator will describe how 
    *   you n-dimensional cells are incident to (n-1)-dimensional cells. 
    * @param useHcs - which CW complex does this come from?
    */
    ChainComplexLocator(unsigned long newDim, homology_coordinate_system useHcs);
    /**
     *  Copy constructor.
     */
    ChainComplexLocator(const ChainComplexLocator &cloneMe);

    bool operator<(const ChainComplexLocator &rhs) const;
    bool operator==(const ChainComplexLocator &rhs) const;
    bool operator!=(const ChainComplexLocator &rhs) const;
    /**
     * Returns true if chain complex is of a boundary type. 
     */
    bool boundaryType() const; 
       
    void writeTextShort(std::ostream& out) const;
    void writeTextLong(std::ostream& out) const;
 };

 /**
  *  CellularData stores chain maps internally in a stack. The Chain Map
  *  Locator allows unique identification of a chain maps when passing 
  *  requests to CellularData::chainMap
  */
 struct ChainMapLocator { 
        ChainComplexLocator domain, range; 

    /**
     *  Initialization constructor.
     *
     * @param Domain - the ChainComplexLocator describing the domain
     * @param Range  - the ChainComplexLocator describing the range.
     */
  ChainMapLocator(const ChainComplexLocator &Domain, 
                  const ChainComplexLocator &Range);

    /**
     *  Copy constructor.
     *
     * @param cloneMe - object to copy.
     */
    ChainMapLocator(const ChainMapLocator &cloneMe);

    bool operator<(const ChainMapLocator &rhs) const;
    bool operator==(const ChainMapLocator &rhs) const;
    bool operator!=(const ChainMapLocator &rhs) const;
       
    void writeTextShort(std::ostream& out) const;
    void writeTextLong(std::ostream& out) const;
 };

 /**
  * CellularData has several routines that require GroupLocator objects as 
  * arguments: unmarkedGroup, markedGroup, homGroup, bilinearForm. See 
  * CellularData::unmarkedGroup, CellularData::markedGroup, 
  * CellularData::homGroup and CellularData::bilinearForm for usage.
  */
 struct GroupLocator {
        // if its an unMarkedGroup or MarkedGroup we need to know dimension, 
        // variance, coordinates, coefficients
    unsigned long dim;
    variance_type var;
    homology_coordinate_system hcs;	
    unsigned long cof;

    /**
     *  Initialization constructor.
     *
     * @param newDim - dimension of group
     * @param newVar - coVariant or contraVariant? 
     * @param useHcs - which CW complex do you want to use?
     * @param useCof - 0 for integer coefficients, non-zero number
     *   n for Z/nZ coefficients.
     */
    GroupLocator(unsigned long newDim, variance_type newVar, 
            homology_coordinate_system useHcs, 
            unsigned long useCof);
    /**
     *  Copy constructor.
     *
     * @param cloneMe - the object to copy
     */
    GroupLocator(const GroupLocator &cloneMe);

    bool operator<(const GroupLocator &rhs) const;
    bool operator==(const GroupLocator &rhs) const;
    bool operator!=(const GroupLocator &rhs) const;
       
    void writeTextShort(std::ostream& out) const;
    void writeTextLong(std::ostream& out) const;
 };

 /**
  * CellularData::homGroup requires a HomLocator object as an argument.   
  */
 struct HomLocator {
    GroupLocator domain;
    GroupLocator range;

    /**
     *  Initialization constructor.
     *
     * @param newDomain - domain of the group homomorphism
     * @param newRange  - range of the group homomorphism
     */
    HomLocator(const GroupLocator &newDomain, const GroupLocator &newRange);
    /**
     *  Copy constructor.
     *
     * @param cloneMe - object to copy.
     */
    HomLocator(const HomLocator &cloneMe);

    bool operator<(const HomLocator &rhs) const;
    bool operator==(const HomLocator &rhs) const;
    bool operator!=(const HomLocator &rhs) const;

    void writeTextShort(std::ostream& out) const;
    void writeTextLong(std::ostream& out) const;
 };

 /**
  * Use this enum in the FormLocator constructor to further specify 
  * which BilinearForm you're interested in.     
  *
  * \todo cupproductForm
  */
 enum form_type { 
 /**
  * intersectionForm is the form which is dual to the cup product, computed by 
  * finding homologous classes in DUAL_coord and STD_REL_BDRY_coord (which are 
  * transverse) and intersecting them there.
  */
  intersectionForm, 
 /**
  * torsionlinkingForm is the induced pairing on the torsion classes in 
  * shifted degree. At present the domain of this routine is a product of 
  * the trivially-presented torsion subgroups you request, so if you call 
  * lDomain or rDomain you will not get an MarkedAbelianGroup isomorphic to 
  * the group you want but not identical.  Call torsionInclusion() if you want 
  * the map identifying the two. 
  */
  torsionlinkingForm, 
 /**
  * evaluationForm is the homology-cohomology pairing. 
  */
  evaluationForm, 
 /**
  * cupproductForm is the cup product pairing TODO (incomplete)
  */
  cupproductForm };

 /**
  * CellularData::bilinearForm requires a FormLocator object as an argument.   
  */
 struct FormLocator {
    GroupLocator ldomain;
    GroupLocator rdomain;
    form_type ft;

    /**
     *  Initialization constructor.
     * 
     * @param FT - what kind of form are you looking for?
     * @param newLdomain - the left group domain factor
     * @param newRdomain - the right group domain factor
     */
    FormLocator(form_type FT, const GroupLocator &newLdomain, 
                const GroupLocator &newRdomain);
    /**
     *  Copy constructor.
     *
     * @param cloneMe - the object to copy.
     */
    FormLocator(const FormLocator &cloneMe);

    bool operator<(const FormLocator &rhs) const;
    bool operator==(const FormLocator &rhs) const;
    bool operator!=(const FormLocator &rhs) const;

    void writeTextShort(std::ostream& out) const;
    void writeTextLong(std::ostream& out) const;
 };

 /**
  * Enum for specifying characteristic parts of the manifold. 
  */
 enum submanifold_type { 
  /**
   * The entire manifold is specified. 
   */
  whole_manifold, 
  /**
   * The non-ideal part of the boundary is specified.
   */
  standard_boundary, 
  /**
   * The ideal part of the boundary is specified.
   */
  ideal_boundary };

 /**
  * CellularData::groupPresentation requires a groupPresentationLocator as
  * an argument.
  */
 struct GroupPresLocator {
   submanifold_type sub_man; // specify which submanifold
   unsigned long component_index; // which component of the submanifold

   /**
    *  Initialization constructor.
    *
    * @param ST submanfold type 
    * @param CI component index if appropriate
    */
   GroupPresLocator( submanifold_type ST, unsigned long CI=0 );
   /**
    *  Copy constructor.
    *
    * @param cloneMe - the object to copy
    */
   GroupPresLocator( const GroupPresLocator &cloneMe );
   
   bool operator<(const GroupPresLocator &rhs) const;
   bool operator==(const GroupPresLocator &rhs) const;
   bool operator!=(const GroupPresLocator &rhs) const;

   void writeTextShort(std::ostream& out) const;
   void writeTextLong(std::ostream& out) const;
 };

 /**
  * CellularData::homGroupPresentation requires a 
  * homGroupPresentationLocator as an argument.
  */
 struct HomGroupPresLocator {
   submanifold_type inclusion_sub_man; // specify which submanifold
   unsigned long subman_component_index; // which component of the submanifold

   /**
    *  Initialization constructor.
    * 
    * @param ST is identifies the submanifold type for the inclusion, 
    *       standard or ideal boundary
    * @param CI is the index of that component. 
    */
   HomGroupPresLocator( submanifold_type ST, unsigned long CI=0 );
   /**
    *  Copy constructor.
    *
    * @param cloneMe - the object to copy. 
    */
   HomGroupPresLocator( const HomGroupPresLocator &cloneMe );
   
   bool operator<(const HomGroupPresLocator &rhs) const;
   bool operator==(const HomGroupPresLocator &rhs) const;
   bool operator!=(const HomGroupPresLocator &rhs) const;

   void writeTextShort(std::ostream& out) const;
   void writeTextLong(std::ostream& out) const;
 };

 /**
  * For the purpose of various homological computations we need data on how  
  * k-cells are incident to (k-1)-cells.  This struct is used to encode that
  * data at the level of the universal cover of the manifold. 
  */
 struct coverFacetData {
  /**
   * This cell is incident to cell number cellNo, taken from the chain complex
   * indexing. 
   */
  unsigned long cellNo;
  /**
   * sig=+1 or -1 depending on whether or not the boundary orientation matches 
   * or not. 
   */ 
  signed long sig; 
  /**
   * The path from the basepoint of this cell to cell numbered cellNo is a 
   * closed loop, therefore generates an element of pi1 of the fundamental
   * group of the manifold.  This is one such representative. 
   */
  GroupExpression trans;
   /**
    * Constructor.
    */
  coverFacetData( unsigned long cellN, signed long Sig, const GroupExpression& word );
   /**
    * Copy constructor.
    */
  coverFacetData( const coverFacetData& cloneMe );
   /**
    * Assignment operator.
    */
   coverFacetData operator=(const coverFacetData &rhs);
   /**
    * Output operator.
    */
   std::string stringValue() const;
 };

private:
    /**
     * Stored pointer to a valid triangulation. All routines use either tri4 
     * or tri3 as reference, and only one will be allocated. This is the 
     * triangulation CellularData is initialised by.
     */
    Triangulation<4>* tri4;
    Triangulation<3>* tri3;

    // for integer chain complexes
    std::map< ChainComplexLocator, MatrixInt* > integerChainComplexes; 
    // for maps of integer chain complexes
    std::map< ChainMapLocator, MatrixInt* > integerChainMaps;
    // for abelian groups
    std::map< GroupLocator, AbelianGroup* > abelianGroups;
    // for marked abelian groups
    std::map< GroupLocator, MarkedAbelianGroup* > markedAbelianGroups;
    // for homomorphisms of marked abelian group
    std::map< HomLocator, HomMarkedAbelianGroup* > homMarkedAbelianGroups;
    // for bilinear forms
    std::map< FormLocator, BilinearForm* > bilinearForms;
    // for group presentations
    std::map< GroupPresLocator, GroupPresentation* > groupPresentations;
    // for homomorphisms of group presentations
    std::map< HomGroupPresLocator, HomGroupPresentation* > homGroupPresentations;
    // for alexander module chain complexes
    std::map< ChainComplexLocator, MatrixRing< 
        SVPolynomialRing< Integer > >* > alexanderChainComplexes;

    /** 
     * numStandardCells = number of cells in the standard CW decomposition in 
     *                      dimensions: 0, 1, 2, 3, (4).
     *                  == numNonIdealCells + numIdealCells 
     * numDualCells     = number of cells in the dual CW decomposition in 
     *                      dimension: 0, 1, 2, 3, (4). 
     * numMixCells      = number of cells in the mixed CW decomposition in 
     *                      dimensions: 0, 1, 2, 3, (4) 
     *                    (see constructor for details)
     * numStandardBdryCells = number of cells in the standard CW decomposition 
     *      of the boundary in dimensions: 0, 1, 2, (3). 
     *                  == numNonIdealBdryCells + numIdealCells
     * numRelativeCells = number of cells from the standard CW-decomposition 
     *          rel boundary.
     *
     * numNonIdealBdryCells == numStandardBdryCells - 
     *                          numIdealCells: 0, 1, 2, (3). 
     * numNonIdealCells = number of non-ideal cells in standard CW-decomposition
     *               in dimension: 0, 1, 2, 3, (4) 
     * numIdealCells    = number of ideal cells in standard CW-decomposition in
     *               dimension: 0, 1, 2, (3) 
     */
   unsigned long numStandardCells[5], numDualCells[5],    numMixCells[5],          
                 numStandardBdryCells[4],  numNonIdealCells[5],
    numIdealCells[4],   numNonIdealBdryCells[4], numRelativeCells[5], 
    numDualRelCells[5], numMixRelCells[5],       numMixBdryCells[4],
    numDualBdryCells[4];

    /** 
     * Chain complex indexing and orientation and boundary-map conventions:
     *
     * nicIx is indexing for the standard cells of the non-ideal variety. 
     *       This keeps track of the vertex/edge/face/tet/pent index of the 
     *       associated cell.  We orient these cells via their characteristic 
     *       maps, so boundary maps signs given by 
     *       getCodimOneObjectMapping().sign(), all ideal boundary bits given +1
     *       orientation.
     *  icIx is Indexing for the standard cells of the ideal variety.  Ie this 
     *       are the ideal boundaries of non-ideal cells that *have* ideal
     *       boundaries. icIx[i][j] is stored as (i+2)*[index of (i+1)-simplex 
     *       containing the j-th ideal i-cell in its boundary] + [the 
     *       corresponding index of this in (i+1)-simplex]. We orient these 
     *       cells as the boundary of this i+1-simplex, so the boundary map is
     *       -1*sign corresponding boundary map of the i+1 simplex.
     *       TODO: edit the above.  It no longer makes sense to me. 
     *  dcIx is Indexing for the dual cells.  dcIx[i] indexes the non-ideal, 
     *       nonboundary standard cells of dimension (3 or 4)-i. We orient 
     *       these via the getEmbeddings conventions in Regina.
     *  rIx  relative chain complex for standard homology rel boundary. 
     *  bcIx is Indexing for the boundary cells, standard decomposition, 
     *       ignoring the ideal ends of standard cells. 
     * 
     * We systematically use the outwards orientation convention to define 
     *  the boundary maps.
     **/
    std::vector< std::vector<unsigned long> > nicIx, icIx, dcIx, bcIx, rIx;

    // generic type for holding chain complex data 
    // this will be a 2x2 grid 1st coordinate the cell index and 
    //  2nd coordinate the indices of the incident faces.
  public:
    typedef NSparseGrid< coverFacetData > ccMapType; 
    // there is a ccMapType for chain complexes (9 coordinate systems, 
    //  all dimensions)
    typedef std::map< ChainComplexLocator, ccMapType* > ccCollectionType;
  private:
    // the "master" chain complex for the manifold. 
    ccCollectionType genCC;  
    typedef std::map< ChainMapLocator, ccMapType* > cmCollectionType;
    cmCollectionType genCM;

 /** 
  * Chain maps: 
  * 
  *   standard: 
  * sbiCM[i]  :  sbCC[i]   -> sCC[i]  std coords, boundary inclusion             
  * strCM[i]  :  sCC[i]    -> srCC[i] std coords, relative projection       
  * schCM[i]  :  srCC[i+1] -> sbCC[i] std coords, connecting hom 
  *
  *     dual:
  * dbiCM[i]  :  dbCC[i]   -> dCC[i]  dual coords, boundary inclusion     TODO & TESTS
  * dtrCM[i]  :  dCC[i]    -> drCC[i] dual coords, relative projection    TODO & TESTS
  * dchCM[i]  :  drCC[i+1] -> dbCC[i] dual coords, connecting hom         TODO & TESTS
  * 
  *    mixed:
  * mbiCM[i]  :  mbCC[i]   -> mCC[i]  mixed coords, boundary inclusion    TODO & TESTS
  * mtrCM[i]  :  mCC[i]    -> mrCC[i] mixed coords, relative projection   TODO & TESTS
  * mchCM[i]  :  mrCC[i+1] -> mbCC[i] mixed coords, connecting hom        TODO & TESTS
  *
  *   inter-coordinate maps, all dimension-preserving:
  * smCM   :   sCC -> mCC   standard to mixed
  * dmCM   :   dCC -> mCC   dual to mixed
  * smbCM  :  sbCC -> mbCC  standard to mixed,   boundary map  TODO & TESTS
  * dmbCM  :  dbCC -> mbCC  dual to mixed,       boundary map  TODO & TESTS
  * srmCM  :  srCC -> mrCC  standard to mixed,   relative map  TODO & TESTS      
  * drmCM  :  drCC -> mrCC  dual to mixed,       relative map  TODO & TESTS 
  */
  std::vector< MatrixInt* > sbiCM, strCM, schCM,   dbiCM, dtrCM, dchCM,   
    mbiCM, mtrCM, mchCM,  smCM, dmCM, smbCM, dmbCM, srmCM, drmCM;

   /**
    * Data that stores the maximal tree in the dual 1-skeleton.  
    * n=dimension of top-dimensional simplices.
    *
    *  maxTreeStd    stores the edges dual to co-dimension 1 simplices, 
    *                indexed by nicIx[n-1]
    *  maxTreeStB    boundary edges dual to co-dimension 2 boundary faces, 
    *                indexed by bcIx[n-2]
    *  maxTreeIdB    boundary edges dual to co-dimension 2 ideal boundary faces,
    *                indexed by icIx[n-2]
    *  maxTreeSttIdB edges connecting top-dimensional simplices barycentres 
    *                to boundary cd1 barycentres, indexed by icIx[n-1]
    */
   std::set< unsigned long > maxTreeStd, maxTreeStB, maxTreeIdB, maxTreeSttIdB; 
    // 1-cells in maximal tree

   struct dim4BoundaryFaceInclusion
    { Face<4,3> *firsttet, *secondtet; // tet in ambient 4-manifold
      unsigned long firstfacnum, secondfacnum; }; // triangle index in each tet

   struct dim4BoundaryEdgeInclusion
    { std::vector< Face<4,3>* > tet; // tet in ambient 4-manifold
      std::vector< unsigned long > edgenum; // edge number in the tet.
      std::vector< Perm<4> > edginc; }; // edge inclusion in tet.

   struct dim4BoundaryVertexInclusion
    { std::vector< Face<4,3>* > tet; // tet in ambient pentachoron.
      std::vector< unsigned long > vrtnum; // vertex number in the tet
      std::vector< Perm<4> > vrtinc; }; // inclusion of the vertex in the tet

   struct dim3BoundaryEdgeInclusion
    { Face<3,2> *firstfac, *secondfac;
      unsigned long firstedgnum, secondedgnum; 
      //Perm<3> firstperm, secondperm; // TODO check if needed? possibly not.
    };

   struct dim3BoundaryVertexInclusion
    { std::vector< Face<3,2>* > face;
      std::vector< unsigned long > vrtnum; 
      std::vector< Perm<3> > vrtinc; };

    /**
     * Normal orientations for cells Regina does not naturally give normal 
     * orientations to. We will have to build this by hand.  For a 
     * dim4Triangulation we need to build the boundary Triangulation<3> class
     * Ben suggests iterating through the dim4Boundary components, appending 
     * them into an Triangulation<3> using dim4BoundaryComponent::getTriangulation
     * and Triangulation<3>::insertTriangulation.  Building up the Triangulation<3>
     *  -> Triangulation<4> indices using dim4BoundaryComponent::getEdge 
     * getFace, etc, maps.  Then we can build the classes below.  For 3-manifold
     * and boundary 2-manifold triangulations we'll likely have to build these
     *  by hands.  The edges shouldn't be bad, the vertices might take work.
     *
     * normalsDim4BdryFaces is a vector that assigns to the i-th boundary face 
     * [tri4->getFace(bcIx[2][i])] the two boundary tetrahedra that contain 
     * it and the face number of the face in the tetrahedron. 
     *
     * normalsDim4BdryEdges is a vector that assigns to the i-th boundary 
     * edge [tri4->getFace(bcIx[1][i])] the circle of tetrahedra incident to 
     * that edge, with edginc[2] and edginc[3] forming the 
     *  normal orientation in agreement with the indexing of tet. 
     *
     * normalsDim4BdryVertices is a vector that assigns to the i-th boundary 
     * vertex  [tri4->getVertex(bcIx[0][i])] the sphere of tetrahedra incident
     * to that vertex, with vrtinc[1], vrtinc[2], vrtinc[3] forming a normal
     * orientation.
     *
     * normalsDim3BdryEdges is a vector that assigns to the i-th boundary face 
     * [tri3->getEdge(bcIx[1][i])] the two boundary faces that contain it and 
     * the edge number of the edge in the Face<3,2>.  firstperm and secondperm
     * describe where the edge vertices 0, 1 get sent to in the face, with 2 
     * going to the edge number in the face. 
     *
     * normalsDim3BdryVertices is a vector that assigns to the i-th boundary 
     * vertex [tri3->getVertex(bcIx[0][i])] the circle of faces incident to 
     * that vertex, with vrtinc[1] and vrtinc[2] forming the normal orientation 
     * in agreement with the indexing of face.  
     */
   std::vector< dim4BoundaryFaceInclusion >   normalsDim4BdryFaces;
   std::vector< dim4BoundaryEdgeInclusion >   normalsDim4BdryEdges;  
   std::vector< dim4BoundaryVertexInclusion > normalsDim4BdryVertices;
   std::vector< dim3BoundaryEdgeInclusion >   normalsDim3BdryEdges;
   std::vector< dim3BoundaryVertexInclusion > normalsDim3BdryVertices; 

   /**
    * Keeps track of numbers of standard and ideal boundary components. 
    * Eventually used to initialize length of stdBdryPi1Gen and idBdryPi1Gen 
    * lengths. These are initialized in buildMaximalTree()
    */
   unsigned long numStdBdryComps, numIdealBdryComps;

    /**
     * Given a co-dimension 1 object in the standard boundary of the 
     * triangulation, you'd sometimes like to know which boundary component 
     * it belongs to. This stores those indices in a vector. 
     * Input is the bcIx[n-2] index. Initialized in buildMaximalTree()
     */
   std::vector< unsigned long > stdBdryCompIndexCD1;
    /**
     * Given a co-dimension 1 object in the standard boundary of the 
     * triangulation, you'd sometimes like to know which boundary component 
     * it belongs to. This stores those indices in a vector. 
     *  Input is the icIx[n-2] index. Initialized in buildMaximalTree()
     */
   std::vector< unsigned long > idBdryCompIndexCD1; 
   /**
    * Given the i-th std boundary component, the j-th generator of pi1 of this
    * component is represented by bcIx[n-2][ stdBdryPi1Gen[i][j] ] Initialized 
    * in buildMaximalTree()
    */
   std::vector< std::vector< unsigned long > > stdBdryPi1Gen;
   /**
    * Given ideal boundary component i, the j-th generator of pi1 of this
    * component is represented by icIx[n-2][ stdBdryPi1Gen[i][j] ] Initialized
    * in buildMaximalTree()
    */
   std::vector< std::vector< unsigned long > > idBdryPi1Gen;

    /**
     * Routine returns true if and only if tet is represents an edge in the 
     * maximal tree for the dual 1-skeleton of the triangulation. Any 
     * tetrahedron from the triangulation can potentially represent an edge. 
     * Corresponds to maxTreeStd, maxTreeStB, maxTreeIdB, and MaxTreeSttIdB 
     * respectively.
     */
    bool inMaximalTree(const Face<4,3>* tet) const;
    bool inMaximalTree(const Face<4,2>* fac) const;
    bool inMaximalTree(const Face<4,3>* tet, unsigned long num) const;
    bool inMaximalTree(const Simplex<4>* pen, unsigned long num) const;
    /**
     * Same routines for 3-dimensional triangulations.
     */
    bool inMaximalTree(const Face<3,2>* fac) const;
    bool inMaximalTree(const Face<3,1>* edg) const;
    bool inMaximalTree(const Face<3,2>* fac, unsigned long num) const;
    bool inMaximalTree(const Simplex<3>* tet, unsigned long num) const;
    
 public:
   /**
    * During initialization many reverse-lookups are needed.  We proved 
    * them in one place. see cellulardata.lookups.cpp for implementations.
    */ 
   unsigned long nicIxLookup(const Face<3,0>* vrt) const;
   unsigned long nicIxLookup(const Face<3,1>* edg) const;
   unsigned long nicIxLookup(const Face<3,2>* fac) const;
   unsigned long nicIxLookup(const Simplex<3>* tet) const;
   unsigned long nicIxLookup(const Face<4,0>* vrt) const;
   unsigned long nicIxLookup(const Face<4,1>* edg) const;
   unsigned long nicIxLookup(const Face<4,2>* fac) const;
   unsigned long nicIxLookup(const Face<4,3>* tet) const;
   unsigned long nicIxLookup(const Simplex<4>* pen) const;

   unsigned long icIxLookup(const Face<3,1>* edg, unsigned long i) const;
   unsigned long icIxLookup(const Face<3,2>* fac, unsigned long i) const;
   unsigned long icIxLookup(const Simplex<3>* tet, unsigned long i) const;
   unsigned long icIxLookup(const Face<4,1>* edg, unsigned long i) const;
   unsigned long icIxLookup(const Face<4,2>* fac, unsigned long i) const;
   unsigned long icIxLookup(const Face<4,3>* tet, unsigned long i) const;
   unsigned long icIxLookup(const Simplex<4>* edg, unsigned long i) const;

   unsigned long dcIxLookup(const Simplex<3>* tet) const; 
   unsigned long dcIxLookup(const Face<3,2>* fac) const; 
   unsigned long dcIxLookup(const Face<3,1>* edg) const; 
   unsigned long dcIxLookup(const Face<3,0>* vrt) const; 
   unsigned long dcIxLookup(const Simplex<4>* pen) const; 
   unsigned long dcIxLookup(const Face<4,3>* tet) const; 
   unsigned long dcIxLookup(const Face<4,2>* fac) const; 
   unsigned long dcIxLookup(const Face<4,1>* edg) const; 
   unsigned long dcIxLookup(const Face<4,0>* vrt) const; 

   unsigned long bcIxLookup(const Face<3,0>* vrt) const; 
   unsigned long bcIxLookup(const Face<3,1>* edg) const; 
   unsigned long bcIxLookup(const Face<3,2>* fac) const; 
   unsigned long bcIxLookup(const Face<4,0>* vrt) const;
   unsigned long bcIxLookup(const Face<4,1>* edg) const;
   unsigned long bcIxLookup(const Face<4,2>* fac) const;
   unsigned long bcIxLookup(const Face<4,3>* tet) const;

   unsigned long rIxLookup(const Face<3,0>* vrt) const;
   unsigned long rIxLookup(const Face<3,1>* edg) const;
   unsigned long rIxLookup(const Face<3,2>* fac) const;
   unsigned long rIxLookup(const Simplex<3>* tet) const;
   unsigned long rIxLookup(const Face<4,0>* vrt) const;
   unsigned long rIxLookup(const Face<4,1>* edg) const;
   unsigned long rIxLookup(const Face<4,2>* fac) const;
   unsigned long rIxLookup(const Face<4,3>* tet) const;
   unsigned long rIxLookup(const Simplex<4>* pen) const;

   unsigned long pi1Lookup(const Face<4,3>* tet) const;
   unsigned long pi1Lookup(const Face<4,2>* fac) const;
   unsigned long pi1Lookup(const Face<4,3>* tet, unsigned long num) const;
   unsigned long pi1Lookup(const Simplex<4>* pen, unsigned long num) const;
   unsigned long pi1Lookup(const Face<3,2>* fac) const;
   unsigned long pi1Lookup(const Face<3,1>* edg) const;
   unsigned long pi1Lookup(const Face<3,2>* fac, unsigned long num) const;
   unsigned long pi1Lookup(const Simplex<3>* tet, unsigned long num) const;

   /**
    * Returns the Triangulation<3>/Triangulation<4> index of the cell with 
    * CellularData STD_coord index idx.  
    */
   unsigned long nicIndex( const unsigned long dim, 
                           const unsigned long idx ) const; // nicIx
   /**
    * Returns the Triangulation<3>/Triangulation<4> index of the cell with 
    * CellularData STD_coord index idx.  The first entry is the index of
    * the simplex, and the 2nd is the index of the facet in that simplex
    * corresponding to the ideal cell.  So icIndex( 0, 2 ) is represents
    * the 2nd ideal 0-cell.  The first coordinate is the edge index, and 
    * the second coordinate is either 0 or 1, indicating which end of the
    * edge. 
    */
   std::pair< unsigned long, unsigned long > icIndex( const unsigned long dim, 
                                      const unsigned long idx ) const; // icIx
   /**
    * Returns the Triangulation<3>/Triangulation<4> index of the cell with 
    * CellularData DUAL_coord index idx.
    */
   unsigned long dcIndex( const unsigned long dim, 
                          const unsigned long idx ) const; // dcIx
   /* ??
    */
   unsigned long bcIndex( const unsigned long dim, 
                          const unsigned long idx ) const; // bcIx;
   /* ??
    */
   unsigned long rIndex( const unsigned long dim, 
                         const unsigned long idx ) const; // rIx;

   unsigned long stdCellCount( const unsigned long dim ) const;
   unsigned long idCellCount( const unsigned long dim ) const;

 private:
   /**
    *  Internal routines to set up chain complexes. 
    */
   void fillStandardHomologyCC();   void fillDualHomologyCC();   
   void fillMixedHomologyCC();      void fillBoundaryHomologyCC(); 
   // void fillBoundaryDualHomologyCC()  // void fillBoundaryMixedHomologyCC() // TODO
   void fillRelativeHomologyCC(); 
   // void fillRelativeDualHomologyCC()  // void fillRelativeMixedHomologyCC() // TODO

   /**
    * Internal routines to set up chain maps. 
    */
   void fillStandardToMixedHomCM(); // void fillDualToMixedHomCM();  // TODO

   /**
    * internal routine to build the maximal tree in the dual 1-skeleton,
    * suitable for computing pi1 information about the manifold. Called after
    * buildExtraNormalData() and before buildFundGrpPres()
    */
   void buildMaximalTree();

   /**
    *  Routine constructs tables normalsDim4BdryFaces normalsDim4BdryEdges 
    *  normalsDim3BdryEdges normalsDim3BdryVertices for homology and fundamental 
    *  group computations.  Called before buildMaximalTree()
    */
   void buildExtraNormalData();

   /**
    * Internal routine to construct the fundamental group presentation for the 
    * ambient manifold.
    */
   void buildFundGrpPres() const;

public:

    /**
     * Takes as input an Triangulation<4> -- this class will make its own 
     * internal copy of the triangulation so it is okay to deallocate whatever
     *  you pass, after the initial call is made. 
     *
     * @param the triangulation to use -- must be valid Triangulation<4>. 
     */
    CellularData(const Triangulation<4> &input);
    /**
     * Takes as input an Triangulation<3> -- this class will make its own internal
     * copy of the triangulation so it is okay to deallocate whatever you pass, 
     * after the initial call is made. 
     *
     * @param pointer to the triangulation to use -- must be a valid 
     * Triangulation<3>. 
     */
    CellularData(const Triangulation<3> &input);
    /**
     * Copy constructor.
     *
     * @param h the homological data to clone.
     */
    CellularData(const CellularData& cloneMe);
    /**
     * Destructor.
     */
    ~CellularData();

    /**
     * Short text representation as required by SharableObject.
     * This only prints out precomputed data, so if you haven't
     * done anything with this CellularData object, this
     * string will be quite short. 
     *
     * @param out the stream to write to.
     */
    void writeTextShort(std::ostream& out) const;

    /**
     * Longer text representation.
     *
     * @param out the stream to write to.
     */
    void writeTextLong(std::ostream& out) const;

    /**
     * @param coord_system specifies the cell complex and dimension. 
     * @return the number of cells of the given dimension in the cell complex
     *  specified by coord_system.
     */
    unsigned long cellCount( const ChainComplexLocator &coord_system) const;

    /**
     *  The number of path-components of a given submanifold type. 
     */
    unsigned long components( submanifold_type ctype ) const; 
   
    /**
     * The Euler characteristic of the manifold, computed from
     * the dual CW-decomposition.
     *
     * This routine calculates the Euler characteristic of the
     * corresponding compact triangulated 3-manifold, with each ideal
     * vertex treated as a surface boundary component.
     *
     * This routine returns the same value as
     * Triangulation<3>::getEulerCharManifold(), though it computes it
     * in a different way.
     *
     * On the other hand, this routine differs from
     * Triangulation<3>::getEulerCharTri(), which handles ideal triangulations
     * in a non-standard way (treating each ideal vertex as just a single
     * vertex).
     *
     * @return the Euler characteristic of the corresponding compact
     * triangulated 3-manifold.
     */
    long eulerChar() const;

    /**
     * Computes the Poincare polynomial -- this is the polynomial such that the 
     * coefficient of t^i is the rank of the i-th homology group of the 
     * manifold. 
     */
    SVPolynomialRing< Integer > poincarePolynomial() const;

    /**
     * If this is a 4-manifold, this routine returns the signature of the H_2 
     * intersection form. If the manifold is not orientable (or not a 
     * 4-manifold) this routine returns 0.  
     */
    long int signature() const;

    /**
     * This returns the Hurewicz map.  It is the natural homomorphism from 
     * the fundamental group to the 1st homology group.  We will store the
     * Hurewicz map as a matrix whose (j,i)-th entry is where the i-th
     * generator of the fundamental group in DUAL_coord is sent to in the
     * DUAL_coord version of H_1 with integer coefficients. 
     *
     * \apinotfinal moreover, this routine is not fully implemented yet. 
     */
    MatrixInt hurewicz_map_H1() const;

    /**
     *  This returns the i-th Stiefel-Whitney class of the manifold.  It 
     * will be returned as an element of the i-th cochain complex with Z_2
     * coefficients, in DUAL_coord. At present only i==1 is implemented. 
     *
     * \apinotfinal We'll repackage this routine before CellularData is
     *  released.  I should also implement i==2 to give tests for the
     *  spin structures paper. 
     */
    std::vector< bool > stiefel_whitney( unsigned long i ) const;

    /**
     * This is a rephrasing of the 1st Stiefel-Whitney class but in terms
     * of the fundamental group.  The value of this vector on k is the
     * evaluation of the Stiefel-Whitney class on the k-th generator of 
     * the fundamental group, in DUAL_coord. 
     *
     * \apinotfinal We'll repackage this routine before CellularData is
     *  released.
     */
    std::vector< bool > PI1_stiefel_whitney1() const;

    /**
     * Determine if the torsion linking form is hyperbolic.  Returns true if 
     * it is, false if not, or if the request does not make sense.
     */
    bool torsionLinkingFormIsHyperbolic() const;

    /**
     * Verifies that the maps used to define the various homology groups for 
     * the manifold are actually chain complexes. 
     *
     * This procedure is largely for debugging purposes as in any release this
     * should always produce true.
     *
     * @return true if all homomorphisms are defined by matrices that really 
     * are chain maps of chain complexes.
     */
    bool chainComplexesVerified() const;

    /**
     * Runs through all the various standard homomorphisms between boundary, 
     * standard, dual and mixed homologies and checks the matrices defining 
     * them really are chain maps.
     *
     * @return true if all homomorphisms are defined by matrices that really 
     *  are chain maps of chain complexes.
     */
    bool chainMapsVerified() const;

    /**
     * If all the chain complexes check out, might as well verify a few basic 
     * isomorphisms as well. These are the isomorphisms between homology in 
     * standard, dual and mixed coordinates. Optional parameters allow for 
     * the check to be done on cohomology, and with coefficients.
     *
     * This is largely for debugging purposes as in any release this should 
     * always produce an answer of true. 
     *
     * @return true provided every natural map between the various homology 
     * groups computable that should be isomorphisms, actually are isomorphisms. 
     */
    bool coordinateIsomorphismsVerified(variance_type var=coVariant, 
        unsigned long coef=0) const;

    /**
     * Similarly, check that the homology long exact sequence of the pair 
     * (M, bdry M) holds.  At present this isn't fully implemented as it only 
     * checks to see that the image of one map is isomorphic to the kernel of 
     * the next, and that the composite of one map with the next is zero.  If 
     * the image/kernel is infinite this is only a partial check of exactness. 
     * Eventually this will be patched. Optional parameters allow for
     * the corresponding check for cohomology, and with coefficients.
     */
    bool homologyLESVerified(variance_type var=coVariant, 
        unsigned long coef=0) const;

    /**
     * Checks to see if Poincare duality holds.  If the manifold is orientable,
     * it uses integer coefficients, if non-orientable it uses Z_2-coefficients.
     */
    bool poincareDualityVerified() const;

    /**
     * Ensure the intersection forms do what they're supposed to do. 
     */
    bool intersectionFormsVerified() const;

    /**
     * Computes a chain complex or retrieves it from the precomputed pile.
     */
    const MatrixInt* integerChainComplex( 
            const ChainComplexLocator &c_desc ) const;

    /**
     * Computes a map of chain complexes or retrieves it from the precomputed
     * pile.
     */
    const MatrixInt* integerChainMap( const ChainMapLocator &m_desc ) const;

    /**
     * Computes an AbelianGroup or retrieves it from the precomputed pile. 
     */
    const AbelianGroup* unmarkedGroup( const GroupLocator &g_desc) const;
    /**
     * Computes an MarkedAbelianGroup or retrieves it from the precomputed 
     * pile. 
     */
    const MarkedAbelianGroup* markedGroup( const GroupLocator &g_desc) const;
    /**
     * Computes an HomMarkedAbelianGroup or retrieves it from the 
     * precomputed pile. 
     *
     * At present there's 4 basic varieties of homomorphisms that Regina knows
     * how to compute.  
     *
     *  1) A pure change-of-coefficients map.  These are maps of the form 
     *     H_i(*,R_1) --> H_i(*,R_2) or
     *     H^i(*,R_2) --> H^i(*,R_1) where R_1 --> R_2 is a quotient map of 
     *     rings, and * is either M, (M,\partial M) or (\partial M), ie: 
     *     any homology_coordinate_system
     *     is valid. todo: add full Bockstein sequence
     *     @pre h_desc.domain.(dim, var, hcs) == h_desc.range.(dim, var, hcs). 
     *     @pre h_desc.domain.cof is an integer multiple of h_desc.range.cof 
     *
     *  2) Maps induced by subdivision of the cell complex maps. 
     *     @pre h_desc.domain.(var, dim) == h_desc.range.(var, dim)
     *     @pre if var==coVariant, h_desc.domain.hcs == STD_coord or DUAL_coord
     *                             h_desc.range.hcs == MIX_coord
     *          if var==contraVariant h_desc.domain.hcs == MIX_coord and
     *                              h_desc.range.hcs == STD_coord or DUAL_coord
     *     @pre h_desc.domain.cof is an integer multiple of h_desc.range.cof
     *
     *  3) Homology long exact sequence of the pair (M,\partial M) maps. 
     *     @pre if var==coVariant,    h_desc.domain.hcs == STD_coord, 
     *                                STD_REL_BDRY_coord or STD_BDRY_coord
     *             and respectively:  h_desc.range.hcs == STD_REL_BDRY_coord,
     *                                           STD_BDRY_coord or STD_coord
     *          if var==contraVariant h_desc.domain.hcs == STD_coord, 
     *                                  STD_BDRY_coord or STD_REL_BDRY_coord
     *             and respectively:  h_desc.range.hcs == STD_BDRY_coord, 
     *                                      STD_REL_BDRY_coord or STD_coord
     *     @pre h_desc.domain.var == h_desc.range.var
     *     @pre h_desc.domain.dim and h_desc.range.dim as in the homology long 
     *                                          exact sequence
     *     @pre h_desc.domain.cof is an integer multiple of h_desc.range.cof
     *
     *  4) Strict Poincare Duality maps, these are maps of the form 
     *          H_i(M;R_1) --> H^{n-i}(M,\partial M;R_2)
     *       or H^i(M;R_1) --> H_{n-i}(M,\partial M;R_2). 
     *     @pre h_desc.domain.var and h_desc.range.var opposite. 
     *     @pre h_desc.domain.dim and h_desc.range.dim add up to the dimension
     *                   of the manifold. 
     *     @pre h_desc.domain.hcs == DUAL_coord and 
     *          h_desc.range.hcs == STD_REL_BDRY_coord
     *     @pre h_desc.domain.cof is an integer multiple of h_desc.range.cof, 
     *      both must be Z_2 if the manifold is not orientable.
     *
     *  5) Convienience maps.  These are natural maps users might be interested
     *     in that are composites of maps (1)--(4) and their inverses.  TODO
     */
    const HomMarkedAbelianGroup* homGroup( const HomLocator &h_desc) const;

    /**
     *  Computes an BilinearForm or retrieves it from the precomputed pile. 
     *
     *  At present there's plans for 5 bilinear form types that will be 
     *  requestable.  This procedure is not fully implemented yet and 
     *  potentially buggy.  
     *
     *  1) Homology-Cohomology pairing <.,.>  ? ie:
     *     H_i(M;R) x H^i(M;R) --> R  where R is the coefficients
     *     
     *
     *  2) Intersection product  
     *     (dual)H_i(M;R) x (std rel bdry)H_j(M;R) --> (mix)H_{(i+j)-n}(M;R)
     *     (dual)H_i(M;R) x (dual)H_j(M;R) --> (mix)H_{(i+j)-n}(M;R)
     *     (std)H_i(M;R) x (std rel bdry)H_j(M;R) --> (mix)H_{(i+j)-n}(M;R)
     *
     *  3) Torsion linking form    ie: tH_i(M;Z) x tH_j(M;Z) --> Q/Z 
     *     (not yet implemented)   when i+j=n-1, i,j>0. 
     *      So for 3-manifolds only defined for i,j = 1,1
     *	      and for 4-manifolds i,j=1,2 or 2,1. 
     *      Present implementation has The range of the form in Q/Z will be 
     *   taken to be  Z_k where k is the largest
     *      ldomain and rdomain given   invariant factor of tH_j(M;Z), 
     *      the range as Z_k with trivial presentations            
     *   trivial presentation 0 --> Z --k--> Z ---> Z_k ---> 0
     *
     *  4) cup products           ie: H^i(M;R) x H^j(M;R) --> H^{i+j}(M;R)
     *     (not yet implemented)           various coordinate systems  TODO
     *     
     */
    const BilinearForm* bilinearForm( const FormLocator &f_desc ) const;

    /**
     *  Describes presentations of various groups associated to the manifold.  
     *  There is the fundamental group of the entire manifold, and fundamental
     *  groups of the various boundary components.
     *
     *  The fundamental groups are computed by first finding a maximal tree in 
     *  the dual 1-skeleton to the manifold.  Unlike Triangulation<3> and 
     *  Triangulation<4>'s maximal forest routines, this routine produces a 
     *  maximal tree in the dual 1-skeleton that restricts to maximal trees in 
     *  the boundary and ideal boundaries of the manifold. So it is quite a bit 
     *  larger in general than the Triangulation<3> and Triangulation<4> routine
     *  outputs. But this allows for computations of maps between groups.
     *  The generators of pi1 are the dual 1-cells not in the maximal tree. 
     *  They are indexed in this order:
     *
     *  First the dual 1-cells that cross co-dimension 1 simplices from the 
     *  triangulation.  Second the dual 1-cells in the standard boundary that 
     *  cross co-dimension 2 simplices from the boundary triangulation. 
     *  Third, the dual 1-cells in the ideal boundary that cross co-dimension 
     *  2 ideal simplices from the ideal boundary triangulation. 
     *  Fourth, the dual 1-cells connecting top-dimensional simplex barycentres
     *  to ideal boundary simplex barycentres.
     */
    const GroupPresentation* groupPresentation( 
          const GroupPresLocator &g_desc ) const;

    /**
     *  Describes the homomorphisms from the fundamental group of the boundary
     *  components to the fundamental group of the manifold's components. This 
     *  routine uses the dual cellular coordinates, constructing a maximal 
     *  forest in the dual 1-skeleton which restricts to maximal forests in 
     *  the boundary and ideal boundary components. 
     */
    const HomGroupPresentation* homGroupPresentation( 
            const HomGroupPresLocator &h_desc ) const;

    /**
     * Chain complex for the Alexander module of the manifold.  Returns NULL 
     * if this does not make sense -- i.e. if the 1st Betti number of the
     * manifold is not 1. Presently only the C_2 -> C_1 -> C_0 part of the 
     * complex is defined, in dual coordinates, for 4-manifolds.
     */
    const MatrixRing< SVPolynomialRing< Integer > >* 
          alexanderChainComplex( const ChainComplexLocator &a_desc ) const;

    /**
     *  Computes the presentation matrix for the 1-dimensional Alexander module. 
     *  Returns null if rank H1 != 1. 
     */
    std::unique_ptr< MatrixRing< SVPolynomialRing< Integer > > > 
            alexanderPresentationMatrix() const;

    /**
     *  Computes the H1 Alexander ideal. Tries to reduce it as much as possible.
     */
    std::unique_ptr< std::list< SVPolynomialRing< Integer > > > 
        alexanderIdeal() const;

    /**
     *  Generic routine for all string-response queries with enum input. 
     */
    std::string stringInfo( const StringRequest &s_desc ) const;

    /**
     *  Generic routine for all bool-response queries with enum input.
     */
    bool boolInfo( const BoolRequest &b_desc) const;

    /**
     *  Returns the group of simplicial automorphisms of the triangulation. 
     * TODO: not yet implemented. Return value needs fixing, for one..
     */
    void simplicialAutomorphismGroup() const;

    /**
     *  Provides a text string that identifies the manifold uniquely 
     *  (if possible) or provides as much detail as CellularData can 
     *  ascertain, if not.  
     *
     *  NOTE: at present very little has been implemented. This is on the 
     *    current TODO list. 
     */
    std::string textDescriptor();
};


/*@}*/

// Inline functions for CellularData

// copy constructor
inline CellularData::CellularData(const CellularData& g) : 
        tri4(clonePtr(g.tri4)), tri3(clonePtr(g.tri3)), 
 // chain complex indexing
    nicIx(g.nicIx), icIx(g.icIx), dcIx(g.dcIx), bcIx(g.bcIx), rIx(g.rIx), 
 // chain maps 
    sbiCM(g.sbiCM.size()), strCM(g.strCM.size()), schCM(g.schCM.size()), 
        dbiCM(g.dbiCM.size()), dtrCM(g.dtrCM.size()), dchCM(g.dchCM.size()), 
        mbiCM(g.mbiCM.size()), mtrCM(g.mtrCM.size()), mchCM(g.mchCM.size()), 
        smCM(g.smCM.size()),   dmCM(g.dmCM.size()),   smbCM(g.smbCM.size()), 
        dmbCM(g.dmbCM.size()), srmCM(g.srmCM.size()), drmCM(g.drmCM.size())
{
// copy all the pre-computed data. 
// the "master" chain complex collection for the manifold. 
ccCollectionType::const_iterator ccmi;
for (ccmi = g.genCC.begin(); ccmi != g.genCC.end(); ccmi++)
 genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
    (ccmi->first, clonePtr(ccmi->second) ) );
// the "master" chain map collection for the manifold. 
cmCollectionType::const_iterator cmi;
for (cmi = g.genCM.begin(); cmi != g.genCM.end(); cmi++)
 genCM.insert( std::pair< ChainMapLocator, ccMapType* >
    (cmi->first, clonePtr(cmi->second) ) );
 // integer chain complexes
std::map< ChainComplexLocator, MatrixInt* >::const_iterator ci;
for (ci = g.integerChainComplexes.begin(); ci != g.integerChainComplexes.end(); 
     ci++) integerChainComplexes.insert( 
 std::pair< ChainComplexLocator, MatrixInt* >
    ( ci->first, clonePtr(ci->second) ) );
 // integer chain maps
std::map< ChainMapLocator, MatrixInt* >::const_iterator mi;
for (mi = g.integerChainMaps.begin(); mi != g.integerChainMaps.end(); 
     mi++) integerChainMaps.insert( 
 std::pair< ChainMapLocator, MatrixInt* >( mi->first, clonePtr(mi->second) ) );
 // abelianGroups
std::map< GroupLocator, AbelianGroup* >::const_iterator abi;
for (abi = g.abelianGroups.begin(); abi != g.abelianGroups.end(); abi++) 
 abelianGroups.insert( std::pair< GroupLocator, AbelianGroup* >
    ( abi->first, clonePtr(abi->second) ) );
 // markedAbelianGroups
std::map< GroupLocator, MarkedAbelianGroup* >::const_iterator mabi;
for (mabi = g.markedAbelianGroups.begin(); mabi != g.markedAbelianGroups.end(); 
     mabi++)    markedAbelianGroups.insert(  
     std::pair< GroupLocator, MarkedAbelianGroup* >
     (mabi->first, clonePtr(mabi->second) ) );
 // homMarkedAbelianGroups
std::map< HomLocator, HomMarkedAbelianGroup* >::const_iterator hmabi;
for (hmabi = g.homMarkedAbelianGroups.begin(); 
     hmabi != g.homMarkedAbelianGroups.end(); hmabi++) 
 homMarkedAbelianGroups.insert( std::pair< HomLocator, 
 HomMarkedAbelianGroup* >(hmabi->first, clonePtr(hmabi->second) ) );
 // bilinearForms
std::map< FormLocator, BilinearForm* >::const_iterator fi;
for (fi = g.bilinearForms.begin(); fi != g.bilinearForms.end(); fi++) 
 bilinearForms.insert( std::pair< FormLocator, BilinearForm* >
   (fi->first, clonePtr(fi->second) ) );
 // groupPresentations
std::map< GroupPresLocator, GroupPresentation* >::const_iterator pi;
for (pi = g.groupPresentations.begin(); pi != g.groupPresentations.end(); pi++) 
 groupPresentations.insert( std::pair< GroupPresLocator, GroupPresentation* >
 (pi->first, clonePtr(pi->second) ) );
 // homGroupPresentations
std::map< HomGroupPresLocator, HomGroupPresentation* >::const_iterator hpi;
for (hpi = g.homGroupPresentations.begin(); 
     hpi != g.homGroupPresentations.end(); hpi++) 
 homGroupPresentations.insert( std::pair< HomGroupPresLocator, 
  HomGroupPresentation* >(hpi->first, clonePtr(hpi->second) ) );
 // alexanderChainComplexes
std::map< ChainComplexLocator, MatrixRing< 
          SVPolynomialRing< Integer > >* >::const_iterator amci;
for (amci = g.alexanderChainComplexes.begin(); 
     amci != g.alexanderChainComplexes.end(); amci++)
 alexanderChainComplexes.insert( std::pair< ChainComplexLocator, 
    MatrixRing< SVPolynomialRing< Integer > >* >
  (amci->first, clonePtr(amci->second) ) );

// numStandardCells[5], numDualCells[5], numMixCells[5],numStandardBdryCells[4], 
//               numNonIdealCells[5], numIdealCells[4];
for (unsigned long i=0; i<5; i++) numStandardCells[i] = g.numStandardCells[i];
for (unsigned long i=0; i<5; i++) numDualCells[i] = g.numDualCells[i];
for (unsigned long i=0; i<5; i++) numMixCells[i] = g.numMixCells[i];
for (unsigned long i=0; i<4; i++) 
     numStandardBdryCells[i] = g.numStandardBdryCells[i];
for (unsigned long i=0; i<5; i++) numNonIdealCells[i] = g.numNonIdealCells[i];
for (unsigned long i=0; i<4; i++) numIdealCells[i] = g.numIdealCells[i];
for (unsigned long i=0; i<4; i++) 
    numNonIdealBdryCells[i] = g.numNonIdealBdryCells[i];
for (unsigned long i=0; i<5; i++) numRelativeCells[i] = g.numRelativeCells[i];
for (unsigned long i=0; i<5; i++) numDualRelCells[i] = g.numDualRelCells[i];
for (unsigned long i=0; i<5; i++) numMixRelCells[i] = g.numMixRelCells[i];
for (unsigned long i=0; i<4; i++) numMixBdryCells[i] = g.numMixBdryCells[i];
for (unsigned long i=0; i<4; i++) numDualBdryCells[i] = g.numDualBdryCells[i];

// chain maps
for (unsigned long i=0; i<sbiCM.size(); i++)   sbiCM[i] =  clonePtr(g.sbiCM[i]);
for (unsigned long i=0; i<strCM.size(); i++)   strCM[i] =  clonePtr(g.strCM[i]);
for (unsigned long i=0; i<schCM.size(); i++)   schCM[i] =  clonePtr(g.schCM[i]);
for (unsigned long i=0; i<dbiCM.size(); i++)   dbiCM[i] =  clonePtr(g.dbiCM[i]);
for (unsigned long i=0; i<dtrCM.size(); i++)   dtrCM[i] =  clonePtr(g.dtrCM[i]);
for (unsigned long i=0; i<dchCM.size(); i++)   dchCM[i] =  clonePtr(g.dchCM[i]);
for (unsigned long i=0; i<mbiCM.size(); i++)   mbiCM[i] =  clonePtr(g.mbiCM[i]);
for (unsigned long i=0; i<mtrCM.size(); i++)   mtrCM[i] =  clonePtr(g.mtrCM[i]);
for (unsigned long i=0; i<mchCM.size(); i++)   mchCM[i] =  clonePtr(g.mchCM[i]);
for (unsigned long i=0; i<smCM.size(); i++)    smCM[i]  =  clonePtr(g.smCM[i]);
for (unsigned long i=0; i<dmCM.size(); i++)    dmCM[i]  =  clonePtr(g.dmCM[i]);
for (unsigned long i=0; i<smbCM.size(); i++)   smbCM[i] =  clonePtr(g.smbCM[i]);
for (unsigned long i=0; i<dmbCM.size(); i++)   dmbCM[i] =  clonePtr(g.dmbCM[i]);
for (unsigned long i=0; i<srmCM.size(); i++)   srmCM[i] =  clonePtr(g.srmCM[i]);
for (unsigned long i=0; i<drmCM.size(); i++)   drmCM[i] =  clonePtr(g.drmCM[i]);

// maximal tree, extraNormalData
buildExtraNormalData(); buildMaximalTree();
}


// destructor
inline CellularData::~CellularData() {
 if (tri4) delete tri4; if (tri3) delete tri3; 
 // iterate through all the stored data and delete

 // master chain complex
 ccCollectionType::const_iterator ccmi;
 for (ccmi = genCC.begin(); ccmi != genCC.end(); ccmi++)
  delete ccmi->second;
 // master chain map collection
 cmCollectionType::const_iterator cmi;
 for (cmi = genCM.begin(); cmi != genCM.end(); cmi++)
  delete cmi->second;
 // integer chain complexes.
 std::map< ChainComplexLocator, MatrixInt* >::const_iterator ci;
 for (ci = integerChainComplexes.begin(); 
      ci != integerChainComplexes.end(); ci++) 
  delete ci->second;
 // integer chain maps
 std::map< ChainMapLocator, MatrixInt* >::const_iterator mi;
 for (mi = integerChainMaps.begin(); mi != integerChainMaps.end(); mi++) 
  delete mi->second;
 // abelian groups
 std::map< GroupLocator, AbelianGroup* >::iterator abi;
 for (abi = abelianGroups.begin(); abi != abelianGroups.end(); abi++)
    delete abi->second;
 // marked abelian groups
 std::map< GroupLocator, MarkedAbelianGroup* >::iterator mabi;
 for (mabi = markedAbelianGroups.begin(); 
      mabi != markedAbelianGroups.end(); mabi++)
    delete mabi->second;
 // hom marked abelian groups
 std::map< HomLocator, HomMarkedAbelianGroup* >::iterator hmabi;
 for (hmabi = homMarkedAbelianGroups.begin(); 
      hmabi != homMarkedAbelianGroups.end(); hmabi++)
    delete hmabi->second; 
 // bilinear forms
 std::map< FormLocator, BilinearForm* >::iterator fi;
 for (fi = bilinearForms.begin(); fi != bilinearForms.end(); fi++)
    delete fi->second;
 // group presentations
 std::map< GroupPresLocator, GroupPresentation* >::iterator gi;
 for (gi = groupPresentations.begin(); gi != groupPresentations.end(); gi++) 
    delete gi->second;
 // homomorphisms of group presentations
 std::map< HomGroupPresLocator, HomGroupPresentation* >::iterator hi;
 for (hi = homGroupPresentations.begin(); 
      hi != homGroupPresentations.end(); hi++)
    delete hi->second; 
 // alexanderChainComplexes
 std::map< ChainComplexLocator, MatrixRing< 
           SVPolynomialRing< Integer > >* >::iterator amci;
 for (amci = alexanderChainComplexes.begin(); 
      amci != alexanderChainComplexes.end(); amci++)
  delete amci->second;

 // iterate through sbiCM, strCM, schCM,  dbiCM, dtrCM, dchCM,  
 //                 mbiCM, mtrCM, mchCM,  smCM, dmCM, smbCM, 
 //                 dmbCM, srmCM, drmCM and deallocate
 for (unsigned long i=0; i<sbiCM.size(); i++) if (sbiCM[i]) delete sbiCM[i];
 for (unsigned long i=0; i<strCM.size(); i++) if (strCM[i]) delete strCM[i];
 for (unsigned long i=0; i<schCM.size(); i++) if (schCM[i]) delete schCM[i];
 for (unsigned long i=0; i<dbiCM.size(); i++) if (dbiCM[i]) delete dbiCM[i];
 for (unsigned long i=0; i<dtrCM.size(); i++) if (dtrCM[i]) delete dtrCM[i];
 for (unsigned long i=0; i<dchCM.size(); i++) if (dchCM[i]) delete dchCM[i];
 for (unsigned long i=0; i<mbiCM.size(); i++) if (mbiCM[i]) delete mbiCM[i];
 for (unsigned long i=0; i<mtrCM.size(); i++) if (mtrCM[i]) delete mtrCM[i];
 for (unsigned long i=0; i<mchCM.size(); i++) if (mchCM[i]) delete mchCM[i];
 for (unsigned long i=0; i<smCM.size(); i++)  if (smCM[i])  delete smCM[i];
 for (unsigned long i=0; i<dmCM.size(); i++)  if (dmCM[i])  delete dmCM[i];
 for (unsigned long i=0; i<smbCM.size(); i++) if (smbCM[i]) delete smbCM[i];
 for (unsigned long i=0; i<dmbCM.size(); i++) if (dmbCM[i]) delete dmbCM[i];
 for (unsigned long i=0; i<srmCM.size(); i++) if (srmCM[i]) delete srmCM[i];
 for (unsigned long i=0; i<drmCM.size(); i++) if (drmCM[i]) delete drmCM[i];
}


} // namespace regina

#endif

