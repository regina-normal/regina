
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

/*! \file algebra/ncellulardata.h
 *  \brief Deals with cellular homology of 3 and 4-manifolds. 
 */

#ifndef __NCELLULARDATA_H
#ifndef __DOXYGEN
#define __NCELLULARDATA_H
#endif

#include "triangulation/ntriangulation.h"
#include "dim4/dim4triangulation.h"

#include "algebra/nmarkedabeliangroup.h"
#include "algebra/nbilinearform.h"
#include "algebra/nhomgrouppresentation.h"
#include "utilities/ptrutils.h"
#include "utilities/nbooleans.h"
#include "maths/nsparsegrid.h"

#include <algorithm>
#include <memory>
#include <vector>

namespace regina {

class NTriangulation;
class Dim4Triangulation;

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Data type that deals with all the detailed homological information in a
 *  manifold.  This information includes:
 *
 * - the manifold's (and its boundary's) homology and cohomology computed in various coordinate 
 *    systems, with coefficients in an arbitrary cyclic group.
 * - the maps between coordinate systems, Bocksteins, boundary inclusion, etc. 
 * - the bilinear forms coming from Poincare Duality: H_i \otimes H_j --> H_{i+j-n}
 *   and torsion linking \tau H_i \otimes \tau H_{n-i-1} --> Q/Z 
 *
 * This class mostly takes a "least effort" approach to all computations. It only computes 
 * what is neccessary for your requests.  It also keeps a record of all previous computations 
 * you've made, we'll call it the `precomputed pile'. If a computation can be sped up by not 
 * recomputing some data, it'll try to take the shortcut.  At present the only exception to 
 * this rule is that all integer coefficient chain complexes and maps are computed on 
 * initialization, but this is relatively quick.  
 *
 * @pre - Assumes input triangulation is valid. 
 *
 * \testpart
 *
 * \todo 1) natural bilinear forms on a manifold, spin structures.
 *       2) test suite stuff: LES of pair, natural isos, PD, detailed tests for intersection forms.
 *        Move all the test routines out of the NCellularData class and put them in the test suite proper. 
 *        Need some kind of form tests for 4-manifolds.  But need some 4-manifolds that we understand, first.
 *       3) New coordinate systems to implement:
 *        MIX_BDRY_coord, MIX_REL_BDRY_coord, DUAL_BDRY_coord, DUAL_REL_BDRY_coord and all the
 *        various maps.  This is required to get at things like H^i M x H^j M --> H^{i+j} M
 *        cup products.  (current efforts here)
 *        cell counts DONE
 *        chain complex initialization TODO
 *        chain maps TODO
 *        PD / intersection forms TODO
 *       4) Detailed fundamental group presentations and maps bdry -> M, etc.  First we need to implement 
 *          a) max forest in dual 1-skeleton to ideal boundary, dual standard boundary, and extension to dual 1-skeleton
 *             of the triangulation
 *          b) extend to full 1-skeleton as dual ideal, dual std boundary, dual 1-skel of triangulation. 
 *          c) append in all relators. That's a big pile. 
 *          d) push out pi_1 computations for all the boundary components, interior manifold, and inclusion maps.
 *       5) Detailed search for possible memory leaks.
 *       6) Sometimes the torsion linking form takes values in Z_1, sometimes in 0, why is there the difference?
 *       7) add some images into documentation.
 * \todo \optlong If cellulardata expands to include things that do not require the standard/dual/mixed
 *       chain complexes, then also shift them off into a chain complex pile that is not precomputed.
 * \todo \optlong Now we have change-of-coefficients maps, so later we should add Bocksteins and the
 *       long exact sequence associated to a change-of-coefficient map.
 * \todo \optlong To minimize memory usage maybe we should consider initializing homs and bilinear forms
 *       using pointers to pre-existing NMarkedAbelianGroups, that way they won't have to be duplicated
 *       and since we already control initialization/destruction via NCellularData, can avoid any pointer
 *       troubles.
 *
 * Guide to ncellulardata.*.cpp files:
 *
 *       ncellulardata.cpp - contains only the core routines NCellularData::unMarkedGroup, NCellularData::markedGroup, 
 *                           NCellularData::homGroup, NCellularData::poincarePolynomial, NCellularData::bilinearForm.
 *
 *       ncellulardata.init.cc.cpp - contains all the setupIndices() routines, as well as all the chain complex
 *                           initialization routines.
 *
 *       ncellulardata.init.hom.cpp - contains the chain map initialization routines. 
 *
 *       ncellulardata.locators.cpp - contains the FormLocator, GroupLocator and HomLocator classes.
 *
 *       ncellulardata.output.cpp - contains the writeTextShort routines.
 *
 *       ncellulardata.tests.cpp - contains all the canned diagnostic tests. 
 *
 * @author Ryan Budney
 */
class NCellularData : public ShareableObject {
public:

 /**
  * This enum gives names to the 9 standard CW-structures associated to a semi-simplicially (ideall) triangulated
  * manifold.  Ideal triangulations are generally not compact.  These cell decomposition are decompositions corresponding
  * to the compact manifold such that the ideally-triangulated manifold is a dense subspace and such that its cells are
  * the intersection of these cells to that subspace. See NCellularData::unmarkedGroup, NCellularData::markedGroup, 
  * NCellularData::homGroup, NCellularData::bilinearForm, NCellularData::GroupLocator, NCellularData::HomLocator for usage.  
  */
 enum homology_coordinate_system { 
 /**
  * Is the most natural CW-decomposition of a semi-simplicially (ideal) triangulated manifold. The top-dimensional cells are
  * the tetrahedra (of a 3-manifold) or the pentachora (of a 4-manifold). Dual to DUAL_REL_BDRY_coord.
  */
  STD_coord, 
 /**
  * Is the dual polyhedral decomposition to this CW-decomposition. The top-dimensional cells correspond to the interior
  * vertices of the triangulation. Dual to STD_REL_BDRY_coord.
  */
  DUAL_coord, 
 /**
  * Is essentially the CW-decomposition of the barycentric subdivision of the triangulation. For every k-cell in the
  * original triangulation there's k+1 associated k-cells in this triangulation. 
  */
  MIX_coord, 
 /**
  * This is the standard cell decomposition (which is always a triangulation) of the boundary. So this consists of two
  * natural parts -- the part from the standard boundary, and the ideal boundary. Dual to DUAL_BDRY_coord
  */
  STD_BDRY_coord, 
 /**
  * This is the same as STD_coord except the boundary cells are thrown away. Dual to DUAL_coord
  */
  STD_REL_BDRY_coord,              
  /**
  * This is the barycentric subdivision of STD_BDRY_coord. TODO (incomplete)
  */
  MIX_BDRY_coord, 
 /**
  * This is MIX_coord with the boundary cells thrown away. TODO (incomplete)
  */
  MIX_REL_BDRY_coord, 
 /**
  * This is the dual cellular decomposition of the boundary. Dual to STD_BDRY_coord. TODO (incomplete)
  */
  DUAL_BDRY_coord, 
 /**
  * This is the cell decomposition which is dual to STD_coord TODO (incomplete)
  */
  DUAL_REL_BDRY_coord };

 /**
  * Use this to specify if you want homology (coVariant) or cohomology (contraVariant) in a (co)homology
  * computation. See NCellularData::unmarkedGroup, NCellularData::markedGroup, NCellularData::homGroup, 
  * NCellularData::bilinearForm for usage.
  */
 enum variance_type { 
 /**
  * for Homology
  */
  coVariant, 
 /**
  * for coHomology
  */
  contraVariant };

 /**
  * NCellularData has several routines that require GroupLocator objects as arguments: unmarkedGroup, markedGroup, 
  *  homGroup, bilinearForm. See NCellularData::unmarkedGroup, NCellularData::markedGroup, NCellularData::homGroup 
  *  and NCellularData::bilinearForm for usage.
  */
 struct GroupLocator {
        // if its an unMarkedGroup or MarkedGroup we need to know dimension, variance, coordinates, coefficients
        unsigned long dim;
	variance_type var;
	homology_coordinate_system hcs;	
	unsigned long cof;

	/**
	 *  Constructor.
	 */
	GroupLocator(unsigned long newDim, variance_type newVar, homology_coordinate_system useHcs, 
			unsigned long useCof);
        GroupLocator(const GroupLocator &cloneMe);

	bool operator<(const GroupLocator &rhs) const;
        bool operator==(const GroupLocator &rhs) const;
        bool operator!=(const GroupLocator &rhs) const;
       
        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
 };

 /**
  * NCellularData::homGroup requires a HomLocator object as an argument.   
  */
 struct HomLocator {
	GroupLocator domain;
	GroupLocator range;

	/**
	 *  Constructor.
	 */
	HomLocator(const GroupLocator &newDomain, const GroupLocator &newRange);
	HomLocator(const HomLocator &cloneMe);

	bool operator<(const HomLocator &rhs) const;
        bool operator==(const HomLocator &rhs) const;
        bool operator!=(const HomLocator &rhs) const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
 };

 /**
  * Use this enum in the FormLocator constructor to further specify which NBilinearForm
  *  you're interested in.     
  *
  * \todo cupproductForm
  */
 enum form_type { 
 /**
  * intersectionForm is the form which is dual to the cup product, computed by finding homologous 
  * classes in DUAL_coord and STD_REL_BDRY_coord (which are transverse) and intersecting them there.
  */
  intersectionForm, 
 /**
  * torsionlinkingForm is the induced pairing on the torsion classes in shifted degree.
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
  * NCellularData::bilinearForm requires a FormLocator object as an argument.   
  */
 struct FormLocator {
	GroupLocator ldomain;
	GroupLocator rdomain;
        form_type ft;

	/**
	 *  Constructor.
	 */
	FormLocator(form_type FT, const GroupLocator &newLdomain, const GroupLocator &newRdomain);
	FormLocator(const FormLocator &cloneMe);

	bool operator<(const FormLocator &rhs) const;
        bool operator==(const FormLocator &rhs) const;
        bool operator!=(const FormLocator &rhs) const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
 };

private:
    /**
     * Stored pointer to a valid triangulation. All routines use either tri4 or tri3
     * as reference, and only one will be allocated. This is the triangulation NCellularData is
     * initialised by.
     */
    Dim4Triangulation* tri4;
    NTriangulation* tri3;

    // for abelian group computations
    std::map< GroupLocator, NAbelianGroup* > abelianGroups;
    // for marked abelian group computations
    std::map< GroupLocator, NMarkedAbelianGroup* > markedAbelianGroups;
    // for homomorphisms of marked abelian group computations
    std::map< HomLocator, NHomMarkedAbelianGroup* > homMarkedAbelianGroups;
    // for intersection forms
    std::map< FormLocator, NBilinearForm* > bilinearForms;

    /** 
     * numStandardCells = number of cells in the standard CW decomposition in dimensions: 0, 1, 2, 3, (4).
     *                  == numNonIdealCells + numIdealCells 
     * numDualCells     = number of cells in the dual CW decomposition in dimension: 0, 1, 2, 3, (4). 
     * numMixCells      = number of cells in the mixed CW decomposition in dimensions: 0, 1, 2, 3, (4) 
     *                    (see constructor for details)
     * numStandardBdryCells = number of cells in the standard CW decomposition of the boundary 
     *                        in dimensions: 0, 1, 2, (3). 
     *                  == numNonIdealBdryCells + numIdealCells
     * numRelativeCells = number of cells from the standard CW-decomposition rel boundary.
     *
     * numNonIdealBdryCells == numStandardBdryCells - numIdealCells: 0, 1, 2, (3). 
     * numNonIdealCells = number of non-ideal cells in standard CW-decomposition in dimension: 0, 1, 2, 3, (4) 
     * numIdealCells    = number of ideal cells in standard CW-decomposition in dimension: 0, 1, 2, (3) 
     */
   unsigned long numStandardCells[5], numDualCells[5], numMixCells[5], numStandardBdryCells[4], 
                 numNonIdealCells[5], numIdealCells[4], numNonIdealBdryCells[4], 
	         numRelativeCells[5], numDualRelCells[5], numMixRelCells[5], numMixBdryCells[4], 
		 numDualBdryCells[4];

    /** 
     * Chain complex indexing and orientation and boundary-map conventions:
     *
     * nicIx is indexing for the standard cells of the non-ideal variety. This keeps track
     *       of the vertex/edge/face/tet/pent index of the associated cell. 
     *       We orient these cells via their characteristic maps, so boundary maps
     *       signs given by getCodimOneObjectMapping().sign(), all ideal boundary bits given +1
     *       orientation.
     *  icIx is Indexing for the standard cells of the ideal variety.  Ie this are the ideal
     *       boundaries of non-ideal cells that *have* ideal boundaries.
     *       icIx[i][j] is stored as (i+2)*[index of (i+1)-simplex containing the j-th
     *       ideal i-cell in its boundary] + [the corresponding index of this in (i+1)-simplex].
     *       We orient these cells as the boundary of this i+1-simplex, so the boundary map is
     *       -1*sign corresponding boundary map of the i+1 simplex.
     *  dcIx is Indexing for the dual cells.  dcIx[i] indexes the non-ideal, nonboundary
     *       standard cells of dimension (3 or 4)-i. We orient these via the getEmbeddings
     *       conventions in Regina.
     *   rIx relative chain complex for standard homology rel boundary.
     *  bcIx is Indexing for the boundary cells, standard decomposition, ignoring the ideal ends
     *       of standard cells. 
     * 
     * We systematically use the outer orientation convention to define the boundary maps.
     **/
    std::vector< std::vector<unsigned long> > nicIx, icIx, dcIx, bcIx, rIx;

    /** 
     * chain complex for standard cellular homology of the manifold, dual cellular homology, 
     *  mixed cellular homology and standard boundary cellular homology respectively.
     */
    std::vector< NMatrixInt* > sCC, dCC, mCC, bsCC, rCC;

    /** 
     * Chain maps: boundary to manifold in standard coords, standard to mixed, 
     *  dual to mixed, standard to relative, boundary map from relative to boundary.
     */
    std::vector< NMatrixInt* > bs_sCM, s_mCM, d_mCM, s_rCM, rbCM;

public:

    /**
     * Takes as input an Dim4Triangulation -- this class will make its own internal
     * copy of the triangulation so it is okay to deallocate whatever you pass, after
     * the initial call is made. 
     *
     * @param the triangulation to use -- must be valid Dim4Triangulation. 
     */
    NCellularData(const Dim4Triangulation &input);
    /**
     * Takes as input an NTriangulation -- this class will make its own internal
     * copy of the triangulation so it is okay to deallocate whatever you pass, after
     * the initial call is made. 
     *
     * @param pointer to the triangulation to use -- must be a valid NTriangulation. 
     */
    NCellularData(const NTriangulation &input);
    /**
     * Copy constructor.
     *
     * @param h the homological data to clone.
     */
    NCellularData(const NCellularData& cloneMe);
    /**
     * Destructor.
     */
    virtual ~NCellularData();

    /**
     * Short text representation as required by SharableObject.
     * This only prints out precomputed data, so if you haven't
     * done anything with this NCellularData object, this
     * string will be quite short. 
     *
     * @param out the stream to write to.
     */
    virtual void writeTextShort(std::ostream& out) const;

    /**
     * Longer text representation.
     *
     * @param out the stream to write to.
     */
    virtual void writeTextLong(std::ostream& out) const;

    /**
     * @param hcs specifies the cell complex.
     * @param dimension the dimension of the cells in question; this must
     *  be 0, 1, 2, 3 or 4. An out-of-bounds request returns zero.
     * @return the number of cells of the given dimension in the cell complex
     *  specified by hcs.
     */
    unsigned long cellCount(homology_coordinate_system hcs, unsigned dimension) const;
   
    /**
     * The Euler characteristic of the manifold, computed from
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
    long eulerChar() const;

    /**
     * Computes the Poincare polynomial -- this is the polynomial such that the 
     * coefficient of t^i is the rank of the i-th homology group of the manifold. 
     */
    NSVPolynomialRing poincarePolynomial() const;

    /**
     *  If this is a 4-manifold, this routine returns the signature of the H_2 intersection form.
     * If the manifold is not orientable (or not a 4-manifold) this routine returns 0.  
     */
    long int signature() const;

    /**
     * Determine if the torsion linking form is hyperbolic.  Returns true if it is, false if not, 
     * unknown if the request doesn't make sense.
     */
    NTriBool torsionLinkingFormIsHyperbolic() const;

    /**
     * Verifies that the maps used to define the various homology groups for the manifold are
     * actually chain complexes. 
     *
     * This procedure is largely for debugging purposes as in any release this should always produce
     *  true.
     *
     * @return true if all homomorphisms are defined by matrices that really are chain maps of
     *         chain complexes.
     */
    bool chainComplexesVerified() const;

    /**
     * Runs through all the various standard homomorphisms between boundary, standard, dual and mixed
     *  homologies and checks the matrices defining them really are chain maps.
     *
     * @return true if all homomorphisms are defined by matrices that really are chain maps of
     *         chain complexes.
     */
    bool chainMapsVerified() const;

    /**
     * If all the chain complexes check out, might as well verify a few basic isomorphisms as well. 
     * These are the isomorphisms between homology in standard, dual and mixed coordinates. 
     * Optional parameters allow for the check to be done on cohomology, and with coefficients.
     *
     * This is largely for debugging purposes as in any release this should always produce an answer
     * of true. 
     *
     * @return true provided every natural map between the various homology groups computable that
     *         should be isomorphisms, actually are isomorphisms. 
     */
    bool coordinateIsomorphismsVerified(variance_type var=coVariant, unsigned long coef=0) const;

    /**
     * Similarly, check that the homology long exact sequence of the pair (M, bdry M)
     * holds.  At present this isn't fully implemented as it only checks to see that the
     * image of one map is isomorphic to the kernel of the next, and that the composite of
     * one map with the next is zero.  If the image/kernel is infinite this is only a partial
     * check of exactness.  Eventually this will be patched. Optional parameters allow for
     * the corresponding check for cohomology, and with coefficients.
     */
    bool homologyLESVerified(variance_type var=coVariant, unsigned long coef=0) const;

    /**
     * Checks to see if Poincare duality holds.  If the manifold is orientable, it uses integer
     * coefficients, if non-orientable it uses Z_2-coefficients.
     */
    bool poincareDualityVerified() const;

    /**
     * Ensure the intersection forms do what they're supposed to do. 
     */
    bool intersectionFormsVerified() const;

    /**
     * Computes an NAbelianGroup or retrieves it from the precomputed pile. 
     */
    const NAbelianGroup* unmarkedGroup( const GroupLocator &g_desc) const;
    /**
     * Computes an NMarkedAbelianGroup or retrieves it from the precomputed pile. 
     */
    const NMarkedAbelianGroup* markedGroup( const GroupLocator &g_desc) const;
    /**
     *  Computes an NHomMarkedAbelianGroup or retrieves it from the precomputed pile. 
     *
     *  At present there's 4 basic varieties of homomorphisms that Regina knows how to 
     *  compute.  
     *
     *  1) A pure change-of-coefficients map.  These are maps of the form H_i(*,R_1) --> H_i(*,R_2) or
     *     H^i(*,R_2) --> H^i(*,R_1) where R_1 --> R_2 is a quotient map of rings, and
     *     * is either M, (M,\partial M) or (\partial M), ie: any homology_coordinate_system
     *     is valid. todo: add full Bockstein sequence
     *     @pre h_desc.domain.(dim, var, hcs) == h_desc.range.(dim, var, hcs). 
     *     @pre h_desc.domain.cof is an integer multiple of h_desc.range.cof 
     *
     *  2) Maps induced by subdivision of the cell complex maps. 
     *     @pre h_desc.domain.(var, dim) == h_desc.range.(var, dim)
     *     @pre if var==coVariant,    h_desc.domain.hcs == STD_coord or DUAL_coord and
     *                                h_desc.range.hcs == MIX_coord
     *          if var==contraVariant h_desc.domain.hcs == MIX_coord and
     *                                h_desc.range.hcs == STD_coord or DUAL_coord
     *     @pre h_desc.domain.cof is an integer multiple of h_desc.range.cof
     *
     *  3) Homology long exact sequence of the pair (M,\partial M) maps. 
     *     @pre if var==coVariant,    h_desc.domain.hcs == STD_coord, STD_REL_BDRY_coord or STD_BDRY_coord
     *             and respectively:  h_desc.range.hcs == STD_REL_BDRY_coord, STD_BDRY_coord or STD_coord
     *          if var==contraVariant h_desc.domain.hcs == STD_coord, STD_BDRY_coord or STD_REL_BDRY_coord
     *             and respectively:  h_desc.range.hcs == STD_BDRY_coord, STD_REL_BDRY_coord or STD_coord
     *     @pre h_desc.domain.var == h_desc.range.var
     *     @pre h_desc.domain.dim and h_desc.range.dim as in the homology long exact sequence
     *     @pre h_desc.domain.cof is an integer multiple of h_desc.range.cof
     *
     *  4) Strict Poincare Duality maps, these are maps of the form H_i(M;R_1) --> H^{n-i}(M,\partial M;R_2)
     *     or H^i(M;R_1) --> H_{n-i}(M,\partial M;R_2). 
     *     @pre h_desc.domain.var and h_desc.range.var opposite. 
     *     @pre h_desc.domain.dim and h_desc.range.dim add up to the dimension of the manifold. 
     *     @pre h_desc.domain.hcs == DUAL_coord and h_desc.range.hcs == STD_REL_BDRY_coord
     *     @pre h_desc.domain.cof is an integer multiple of h_desc.range.cof, 
     *      both must be Z_2 if the manifold is not orientable.
     *
     *  5) Convienience maps.  These are natural maps users might be interested in that are composites of maps (1)--(4)
     *     and their inverses.  TODO
     */
    const NHomMarkedAbelianGroup* homGroup( const HomLocator &h_desc) const;

    /**
     *  Computes an NBilinearForm or retrieves it from the precomputed pile. 
     *
     *  At present there's plans for 5 bilinear form types that will be requestable.  This procedure
     *  is not fully implemented yet and potentially buggy.  
     *
     *  1) Homology-Cohomology pairing <.,.>  ? ie: H_i(M;R) x H^i(M;R) --> R  where R is the coefficients
     *     
     *
     *  2) Intersection product               ie: (dual)H_i(M;R) x (std rel bdry)H_j(M;R) --> (mix)H_{(i+j)-n}(M;R)
     *                                            (dual)H_i(M;R) x (dual)H_j(M;R) --> (mix)H_{(i+j)-n}(M;R)
     *                                            (std)H_i(M;R) x (std rel bdry)H_j(M;R) --> (mix)H_{(i+j)-n}(M;R)
     *
     *  3) Torsion linking form               ie: tH_i(M;Z) x tH_j(M;Z) --> Q/Z 
     *     (not yet implemented)                  when i+j=n-1, i,j>0. So for 3-manifolds only defined for i,j = 1,1
     *						  and for 4-manifolds i,j=1,2 or 2,1. 
     *      Present implementation has      The range of the form in Q/Z will be taken to be Z_k where k is the largest
     *      ldomain and rdomain given        invariant factor of tH_j(M;Z), so we implement the range as Z_k with
     *      trivial presentations            trivial presentation 0 --> Z --k--> Z ---> Z_k ---> 0
     *
     *  4) cup products                       ie: H^i(M;R) x H^j(M;R) --> H^{i+j}(M;R)
     *     (not yet implemented)              various coordinate systems  
     *     
     */
    const NBilinearForm* bilinearForm( const FormLocator &f_desc ) const;

    /**
     *  Describes the homomorphisms from the fundamental group of the boundary components to the fundamental
     *  group of the manifold's components. This routine uses the dual cellular coordinates, building up
     *  a maximal forest in the dual boundary 1-skeleton, then extending it to a maximal forest in the dual 
     *  1-skeleton. TODO: Not yet implemented.
     *
     * \todo eventually expand to use other coordinate systems, other group homomorphisms.
     */
    const NHomGroupPresentation* homGroupPresentation() const;
     
};


/*@}*/

// Inline functions for NCellularData

// copy constructor
inline NCellularData::NCellularData(const NCellularData& g) : ShareableObject(),
        tri4(clonePtr(g.tri4)), tri3(clonePtr(g.tri3)), 
	nicIx(g.nicIx), icIx(g.icIx), dcIx(g.dcIx), bcIx(g.bcIx), rIx(g.rIx), 
	sCC(g.sCC.size()), dCC(g.dCC.size()), mCC(g.mCC.size()), bsCC(g.bsCC.size()), rCC(g.rCC.size()), 
	bs_sCM(g.bs_sCM.size()), s_mCM(g.s_mCM.size()), d_mCM(g.d_mCM.size()), s_rCM(g.s_rCM.size()), 
	rbCM(g.rbCM.size())
{
// copy abelianGroups, markedAbelianGroups, homMarkedAbelianGroups
std::map< GroupLocator, NAbelianGroup* >::const_iterator abi;
std::map< GroupLocator, NMarkedAbelianGroup* >::const_iterator mabi;
std::map< HomLocator, NHomMarkedAbelianGroup* >::const_iterator hmabi;
std::map< FormLocator, NBilinearForm* >::const_iterator fi;
for (abi = g.abelianGroups.begin(); abi != g.abelianGroups.end(); abi++) abelianGroups.insert( 
 std::pair< GroupLocator, NAbelianGroup* >( abi->first, clonePtr(abi->second) ) );
for (mabi = g.markedAbelianGroups.begin(); mabi != g.markedAbelianGroups.end(); mabi++)	markedAbelianGroups.insert( 
 std::pair< GroupLocator, NMarkedAbelianGroup* >(mabi->first, clonePtr(mabi->second) ) );
for (hmabi = g.homMarkedAbelianGroups.begin(); hmabi != g.homMarkedAbelianGroups.end(); hmabi++) 
 homMarkedAbelianGroups.insert( std::pair< HomLocator, NHomMarkedAbelianGroup* >(hmabi->first, clonePtr(hmabi->second) ) );
for (fi = g.bilinearForms.begin(); fi != g.bilinearForms.end(); fi++) 
 bilinearForms.insert( std::pair< FormLocator, NBilinearForm* >(fi->first, clonePtr(fi->second) ) );

// numStandardCells[5], numDualCells[5], numMixCells[5], numStandardBdryCells[4], 
//               numNonIdealCells[5], numIdealCells[4];
for (unsigned long i=0; i<5; i++) numStandardCells[i] = g.numStandardCells[i];
for (unsigned long i=0; i<5; i++) numDualCells[i] = g.numDualCells[i];
for (unsigned long i=0; i<5; i++) numMixCells[i] = g.numMixCells[i];
for (unsigned long i=0; i<4; i++) numStandardBdryCells[i] = g.numStandardBdryCells[i];
for (unsigned long i=0; i<5; i++) numNonIdealCells[i] = g.numNonIdealCells[i];
for (unsigned long i=0; i<4; i++) numIdealCells[i] = g.numIdealCells[i];
for (unsigned long i=0; i<4; i++) numNonIdealBdryCells[i] = g.numNonIdealBdryCells[i];
for (unsigned long i=0; i<5; i++) numRelativeCells[i] = g.numRelativeCells[i];
for (unsigned long i=0; i<5; i++) numDualRelCells[i] = g.numDualRelCells[i];
for (unsigned long i=0; i<5; i++) numMixRelCells[i] = g.numMixRelCells[i];
for (unsigned long i=0; i<4; i++) numMixBdryCells[i] = g.numMixBdryCells[i];
for (unsigned long i=0; i<4; i++) numDualBdryCells[i] = g.numDualBdryCells[i];

// the chain complexes
for (unsigned long i=0; i<sCC.size(); i++)       sCC[i] = clonePtr(g.sCC[i]);
for (unsigned long i=0; i<dCC.size(); i++)       dCC[i] = clonePtr(g.dCC[i]);
for (unsigned long i=0; i<mCC.size(); i++)       mCC[i] = clonePtr(g.mCC[i]);
for (unsigned long i=0; i<bsCC.size(); i++)     bsCC[i] = clonePtr(g.bsCC[i]);
for (unsigned long i=0; i<rCC.size(); i++)       rCC[i] = clonePtr(g.rCC[i]);
// chain maps
for (unsigned long i=0; i<bs_sCM.size(); i++) bs_sCM[i] = clonePtr(g.bs_sCM[i]);
for (unsigned long i=0; i<s_mCM.size(); i++)   s_mCM[i] = clonePtr(g.s_mCM[i]);
for (unsigned long i=0; i<d_mCM.size(); i++)   d_mCM[i] = clonePtr(g.d_mCM[i]);
for (unsigned long i=0; i<s_rCM.size(); i++)   s_rCM[i] = clonePtr(g.s_rCM[i]);
for (unsigned long i=0; i<rbCM.size(); i++)     rbCM[i] = clonePtr(g.rbCM[i]);
}

// destructor
inline NCellularData::~NCellularData() {
 if (tri4) delete tri4; if (tri3) delete tri3; 
 // iterate through abelianGroups, markedAbelianGroups, homMarkedAbelianGroups and deallocate
 std::map< GroupLocator, NAbelianGroup* >::iterator abi;
 std::map< GroupLocator, NMarkedAbelianGroup* >::iterator mabi;
 std::map< HomLocator, NHomMarkedAbelianGroup* >::iterator hmabi;
 std::map< FormLocator, NBilinearForm* >::iterator fi;
 for (abi = abelianGroups.begin(); abi != abelianGroups.end(); abi++)
	delete abi->second;
 for (mabi = markedAbelianGroups.begin(); mabi != markedAbelianGroups.end(); mabi++)
	delete mabi->second;
 for (hmabi = homMarkedAbelianGroups.begin(); hmabi != homMarkedAbelianGroups.end(); hmabi++)
	delete hmabi->second;
 for (fi = bilinearForms.begin(); fi != bilinearForms.end(); fi++)
	delete fi->second;

 // iterate through sCC, dCC, mCC, bsCC and deallocate
 for (unsigned long i=0; i<sCC.size(); i++)   if (sCC[i])  delete sCC[i];
 for (unsigned long i=0; i<dCC.size(); i++)   if (dCC[i])  delete dCC[i];
 for (unsigned long i=0; i<mCC.size(); i++)   if (mCC[i])  delete mCC[i];
 for (unsigned long i=0; i<bsCC.size(); i++)  if (bsCC[i]) delete bsCC[i];
 for (unsigned long i=0; i<rCC.size(); i++)   if (rCC[i])  delete rCC[i];
 // iterate through bs_sCM, s_mCM, d_mCM and deallocate
 for (unsigned long i=0; i<bs_sCM.size(); i++) if (bs_sCM[i]) delete bs_sCM[i];
 for (unsigned long i=0; i<s_mCM.size(); i++)  if (s_mCM[i])  delete s_mCM[i];
 for (unsigned long i=0; i<d_mCM.size(); i++)  if (d_mCM[i])  delete d_mCM[i];
 for (unsigned long i=0; i<s_rCM.size(); i++)  if (s_rCM[i])  delete s_rCM[i];
 for (unsigned long i=0; i<rbCM.size(); i++)   if (rbCM[i])   delete rbCM[i];
}

inline unsigned long NCellularData::cellCount(homology_coordinate_system hcs, unsigned dimension) const
{
if ( (dimension > 4) && tri4 ) return 0; 
if ( (dimension > 3) && tri3 ) return 0; // out of bounds check
if (hcs == STD_coord) return numStandardCells[dimension]; 
if (hcs == DUAL_coord) return numDualCells[dimension]; 
if (hcs == MIX_coord) return numMixCells[dimension];
if (hcs == MIX_REL_BDRY_coord) return numMixRelCells[dimension]; 
if (hcs == STD_REL_BDRY_coord) return numRelativeCells[dimension]; 
if (hcs == DUAL_REL_BDRY_coord) return numDualRelCells[dimension]; 
if ( (dimension > 3) && tri4 ) return 0;
if ( (dimension > 2) && tri3 ) return 0;
if (hcs == STD_BDRY_coord) return numStandardBdryCells[dimension]; 
if (hcs == MIX_BDRY_coord) return numMixBdryCells[dimension]; 
if (hcs == DUAL_BDRY_coord) return numDualBdryCells[dimension]; 
return 0; // only get here if hcs out of bounds!
}

inline long int NCellularData::eulerChar() const
{ return numDualCells[0]-numDualCells[1]+numDualCells[2]-numDualCells[3]+numDualCells[4]; }

inline long int NCellularData::signature() const
{
 if (tri3) return 0; if (!tri4->isOrientable()) return 0;
 const NBilinearForm* b = bilinearForm( 
       FormLocator( intersectionForm, GroupLocator(2, coVariant, DUAL_coord, 0), GroupLocator(2, coVariant, DUAL_coord, 0) ) );
 return b->signature();
}

// GroupLocator and HomLocator

inline NCellularData::GroupLocator::GroupLocator(unsigned long newDim, variance_type newVar,
	 homology_coordinate_system useHcs, unsigned long useCof) :
 dim(newDim), var(newVar), hcs(useHcs), cof(useCof) {}

inline NCellularData::GroupLocator::GroupLocator(const GroupLocator &cloneMe) : 
 dim(cloneMe.dim), var(cloneMe.var), hcs(cloneMe.hcs), cof(cloneMe.cof) {}

inline void NCellularData::GroupLocator::writeTextShort(std::ostream& out) const
{
if ( (hcs == STD_coord) || (hcs == STD_BDRY_coord) || (hcs == STD_REL_BDRY_coord) ) out<<"(std)"; else
if (hcs == DUAL_coord) out<<"(dual)"; else if (hcs == MIX_coord) out<<"(mix)"; 
out<<"H"<<( var==coVariant ? "_" : "^" )<<dim;
if (hcs == STD_BDRY_coord) out<<"(bM;"; else if (hcs == STD_REL_BDRY_coord) out<<"(M,bM;";
else out<<"(M;";
if (cof == 0) out<<"Z)"; else out<<"Z_"<<cof<<")";
}

inline void NCellularData::GroupLocator::writeTextLong(std::ostream& out) const
{
if ( (hcs == STD_coord) || (hcs == STD_BDRY_coord) || (hcs == STD_REL_BDRY_coord) ) out<<"(std)"; else
if (hcs == DUAL_coord) out<<"(dual)"; else if (hcs == MIX_coord) out<<"(mix)"; 
out<<"H"<<( var==coVariant ? "_" : "^" )<<dim;
if (hcs == STD_BDRY_coord) out<<"(bM;"; else if (hcs == STD_REL_BDRY_coord) out<<"(M,bM;";
else out<<"(M;";
if (cof == 0) out<<"Z)"; else out<<"Z_"<<cof<<")";
}

inline NCellularData::HomLocator::HomLocator(const GroupLocator &newDomain, const GroupLocator &newRange) : 
  domain( newDomain ), range( newRange ) {}

inline NCellularData::HomLocator::HomLocator(const HomLocator &cloneMe) :
  domain( cloneMe.domain ), range( cloneMe.range ) {}

inline void NCellularData::HomLocator::writeTextShort(std::ostream& out) const
{
out<<"map[";
domain.writeTextShort(out);
out<<"-->";
range.writeTextShort(out);
out<<"]";
}

inline void NCellularData::HomLocator::writeTextLong(std::ostream& out) const
{
out<<"map[";
domain.writeTextShort(out);
out<<"-->";
range.writeTextShort(out);
out<<"]";
}

inline NCellularData::FormLocator::FormLocator( form_type FT, const GroupLocator &newLdomain, const GroupLocator &newRdomain) :
 ldomain( newLdomain ), rdomain( newRdomain ), ft(FT)  {}

inline NCellularData::FormLocator::FormLocator( const FormLocator &cloneMe ) :
 ldomain( cloneMe.ldomain ), rdomain( cloneMe.rdomain ), ft(cloneMe.ft)  {}

inline void NCellularData::FormLocator::writeTextShort(std::ostream& out) const
{}

inline void NCellularData::FormLocator::writeTextLong(std::ostream& out) const
{}

} // namespace regina

#endif

