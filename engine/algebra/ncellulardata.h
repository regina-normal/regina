
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

/*! \file algebra/nCellularData.h
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
#include "utilities/ptrutils.h"

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
 * \todo natural bilinear forms on a manifold, spin structures, test suite stuff: 
 *       LES of pair, natural isos, PD
 * \todo \optlong If cellulardata expands to include things that do not require the standard/dual/mixed
 *       chain complexes, then also shift them off into a chain complex pile that is not precomputed.
 * \todo \optlong Now we have change-of-coefficients maps, so later we should add Bocksteins and the
 *       long exact sequence associated to a change-of-coefficient map.
 * \todo \optlong To minimize memory usage maybe we should consider initializing homs and bilinear forms
 *       using pointers to pre-existing NMarkedAbelianGroups, that way they won't have to be duplicated
 *       and since we already control initialization/destruction via NCellularData, can avoid any pointer
 *       troubles.
 *
 * @author Ryan Budney
 */
class NCellularData : public ShareableObject {
public:

 enum homology_coordinate_system { STD_coord, DUAL_coord, MIX_coord, STD_BDRY_coord, STD_REL_BDRY_coord };
 enum form_type { intersectionForm, torsionlinkingForm, evaluationForm };
 enum variance_type { coVariant, contraVariant }; // homology / cohomology specifier

 struct GroupLocator {
        // if its an unMarkedGroup or MarkedGroup we need to know dimension, variance, coordinates, coefficients
        unsigned long dim;
	variance_type var;
	homology_coordinate_system hcs;	
	unsigned long cof;

	GroupLocator(unsigned long newDim, variance_type newVar, homology_coordinate_system useHcs, 
			unsigned long useCof);
        GroupLocator(const GroupLocator &cloneMe);

	bool operator<(const GroupLocator &rhs) const;
        bool operator==(const GroupLocator &rhs) const;
        bool operator!=(const GroupLocator &rhs) const;
       
        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
 };

 struct HomLocator {
	GroupLocator domain;
	GroupLocator range;

	HomLocator(const GroupLocator &newDomain, const GroupLocator &newRange);
	HomLocator(const HomLocator &cloneMe);

	bool operator<(const HomLocator &rhs) const;
        bool operator==(const HomLocator &rhs) const;
        bool operator!=(const HomLocator &rhs) const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
 };

 struct FormLocator {
	GroupLocator ldomain;
	GroupLocator rdomain;
        form_type ft;

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
     * numDualCells     = number of cells in the dual CW decomposition in dimension: 0, 1, 2, 3, (4). 
     * numMixCells      = number of cells in the mixed CW decomposition in dimensions: 0, 1, 2, 3, (4) 
     * numStandardBdryCells = number of cells in the standard CW decomposition of the boundary 
     *                        in dimensions: 0, 1, 2, (3). 
     * numRelativeCells = number of cells from the standard CW-decomposition rel boundary.
     *
     * numNonIdealBdryCells = numStandardBdryCells - numIdealCells: 0, 1, 2, (3). 
     * numNonIdealCells = number of non-ideal cells in standard CW-decomposition in dimension: 0, 1, 2, 3, (4) 
     * numIdealCells    = number of ideal cells in standard CW-decomposition in dimension: 0, 1, 2, (3) 
     */
   unsigned long numStandardCells[5], numDualCells[5], numMixCells[5], numStandardBdryCells[4], 
                 numNonIdealCells[5], numIdealCells[4], numNonIdealBdryCells[4], 
	         numRelativeCells[5];

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
    unsigned long standardCellCount(unsigned dimension) const;
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
    unsigned long dualCellCount(unsigned dimension) const;
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
    unsigned long boundaryCellCount(unsigned dimension) const;
    /**
     * Returns the number of cells in the mixed cellular decomposition 
     */
    unsigned long mixedCellCount(unsigned dimension) const;
     /**
     * Returns the number of cells relative CW-decomposition of the manifold
     * rel boundary, induced by the standard cellular decomposition. 
     */
    unsigned long relativeCellCount(unsigned dimension) const;
   
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
     * Similarly, check that the homology long exact sequence of the pair (M, \partial M)
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
     *  [4] Strict Poincare Duality maps, these are maps of the form H_i(M;R_1) --> H^{n-i}(M,\partial M;R_2)
     *      or H^i(M;R_1) --> H_{n-i}(M,\partial M;R_2). 
     *      @pre h_desc.domain.var and h_desc.range.var opposite. 
     *      @pre h_desc.domain.dim and h_desc.range.dim add up to the dimension of the manifold. 
     *      @pre h_desc.domain.hcs == DUAL_coord and h_desc.range.hcs == STD_REL_BDRY_coord
     *      @pre h_desc.domain.cof is an integer multiple of h_desc.range.cof, 
     *       both must be Z_2 if the manifold is not orientable.
     */
    const NHomMarkedAbelianGroup* homGroup( const HomLocator &h_desc) const;

    /**
     *  Computes various bilinear forms associated to the homology of the manifold:
     *
     *  1) Homology-Cohomology pairing <.,.>  ie: H_i(M;R) x H^i(M;R) --> R  where R is the coefficients
     *  2) Intersection product               ie: H_i(M;R) x H_j(M;R) --> H_{n-(i+j)}(M;R)
     *  3) Torsion linking form               ie: H_i(M;Z) x H_j(M;Z) --> H_{n-(i+j)-1}(M;Q/Z)
     */
    const NBilinearForm* bilinearForm( const FormLocator &f_desc ) const;

    /**
     * Computes the Poincare polynomial -- this is the polynomial such that the 
     * coefficient of t^i is the rank of the i-th homology group of the manifold. 
     */
    NSVPolynomialRing poincarePolynomial() const;

    //todo: 1) bilinear forms return object for Poincare duality objects.
    //         various steps: 
    //         a) bilinear form class
    //         b) polynomial ring class
    //         c) TLF and intersection pairings. For intersection pairings need to
    //            intelligently determine relative sign of intersections. 
    //      2) fundamental group nonsense
    //      3) full Bockstein sequence

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

inline unsigned long NCellularData::standardCellCount(unsigned dimension) const
{ return numStandardCells[dimension]; }
inline unsigned long NCellularData::dualCellCount(unsigned dimension) const
{ return numDualCells[dimension]; }
inline unsigned long NCellularData::boundaryCellCount(unsigned dimension) const
{ return numStandardBdryCells[dimension]; }
inline unsigned long NCellularData::mixedCellCount(unsigned dimension) const
{ return numMixCells[dimension]; }
inline unsigned long NCellularData::relativeCellCount(unsigned dimension) const
{ return numRelativeCells[dimension]; }

inline long int NCellularData::eulerChar() const
{ return numDualCells[0]-numDualCells[1]+numDualCells[2]-numDualCells[3]+numDualCells[4]; }

// GroupLocator and HomLocator

//inline NCellularData::GroupLocator::~GroupLocator() {}

//inline NCellularData::HomLocator::~HomLocator() {}

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
 ft(FT), ldomain( newLdomain ), rdomain( newRdomain ) {}

inline NCellularData::FormLocator::FormLocator( const FormLocator &cloneMe ) :
 ft(cloneMe.ft), ldomain( cloneMe.ldomain ), rdomain( cloneMe.rdomain ) {}

inline void NCellularData::FormLocator::writeTextShort(std::ostream& out) const
{}

inline void NCellularData::FormLocator::writeTextLong(std::ostream& out) const
{}

} // namespace regina

#endif

