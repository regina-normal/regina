
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

#include "maths/nrational.h"
#include "utilities/nindexedarray.h"
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
 * This class mostly takes a "least effort" approach to all computations. It
 * only computes what is neccessary for your requests.  It also keeps a
 * record of all previous computations you've made, we'll call it the 
 * `precomputed pile'. If a computation can be sped up by not recomputing some data, it'll
 * try to take the shortcut.  The only exception to this rule is that all integer coefficient
 * chain complexes for homology in standard, dual and mixed coordinates, and all the natural
 * maps between the various chain complexes are computed on initialization.  By and large these
 * aren't very time-consuming but expecially the barycentric subdivision chain complexes and
 * maps can be fairly large and memory-intensive. 
 *
 * @pre - Assumes input triangulation is valid. 
 *
 * \testpart
 *
 * \todo \optlong Add an option to limit precomputed pile size, then when you reach the limit you 
 *       prune the pile according to how often / recent you use various items, deallocating the oldest
 *       least popular pile items first.
 * \todo \optlong If cellulardata expands to include things that do not require the standard/dual/mixed
 *       chain complexes, then also shift them off into a chain complex pile that is not precomputed.
 *
 * @author Ryan Budney
 */
class NCellularData : public ShareableObject {
public:

 enum homology_coordinate_system { STD_coord, DUAL_coord, MIX_coord };
 enum variance_type { coVariant, contraVariant }; // homology / cohomology specifier

 struct GroupLocator {
        // if its an unMarkedGroup or MarkedGroup we need to know dimension, variance, coordinates, coefficients
        unsigned long dim;
	variance_type var;
	homology_coordinate_system hcs;	
	unsigned long cof;

	GroupLocator(unsigned long newDim, variance_type newVar, homology_coordinate_system useHcs, 
			unsigned long useCof);
	bool operator<(const GroupLocator &rhs) const;
 };

 struct HomLocator {
	GroupLocator domain;
	GroupLocator range;

	HomLocator(const GroupLocator &newDomain, const GroupLocator &newRange);
	bool operator<(const HomLocator &rhs) const;
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

    /** 
     * numStandardCells - number of cells in the standard CW decomposition in dimensions: 0, 1, 2, 3, (4). 
     * numDualCells     - number of cells in the dual CW decomposition in dimension: 0, 1, 2, 3, (4). 
     * numMixCells      - number of cells in the mixed CW decomposition in dimensions: 0, 1, 2, 3, (4) 
     * numStandardBdryCells - number of cells in the standard CW decomposition of the boundary 
     *                        in dimensions: 0, 1, 2, (3). 
     * numNonIdealCells - number of non-ideal cells in standard CW-decomposition in dimension: 0, 1, 2, 3, (4) 
     * numIdealCells    - number of ideal cells in standard CW-decomposition in dimension: 0, 1, 2, (3) 
     */
   unsigned long numStandardCells[5], numDualCells[5], numMixCells[5], numStandardBdryCells[4], 
                 numNonIdealCells[5], numIdealCells[4];

    /** 
     * Chain complex indexing and orientation and boundary-map conventions:
     *
     * nicIx is indexing for the standard cells of the non-ideal variety. This keeps track
     *       of the vertex/edge/face/tet/pent index of the associated cell. 
     *       We orient these cells via their characteristic maps, so boundary maps
     *       signs given by getCodimOneObjectMapping().sign(), all ideal boundary bits given +1
     *       orientation.
     * icIx is Indexing for the standard cells of the ideal variety.  Ie this are the ideal
     *      boundaries of non-ideal cells that *have* ideal boundaries.
     *      icIx[i][j] is stored as (i+2)*[index of (i+1)-simplex containing the j-th
     *      ideal i-cell in its boundary] + [the corresponding index of this in (i+1)-simplex].
     *      We orient these cells as the boundary of this i+1-simplex, so the boundary map is
     *      -1*sign corresponding boundary map of the i+1 simplex.
     * dcIx is Indexing for the dual cells.  dcIx[i] indexes the non-ideal, nonboundary
     *      standard cells of dimension (3/4)-i. We orient these via the getEmbeddings
     *      conventions in Regina.
     * bcIx is Indexing for the boundary cells, standard decomposition, ignoring the ideal ends
     *      of standard cells. 
     * 
     * We systematically use the outer orientation convention to define the boundary maps.
     **/
    std::vector< std::vector<unsigned long> > nicIx, icIx, dcIx, bcIx;

    /** 
     * chain complex for standard cellular homology of the manifold, dual cellular homology, 
     *  mixed cellular homology and standard boundary cellular homology respectively.
     */
    std::vector< NMatrixInt* > sCC, dCC, mCC, bsCC;

    /** 
     * Chain maps: boundary to manifold in standard coords, standard to mixed, 
     *  and dual to mixed. 
     */
    std::vector< NMatrixInt* > bs_sCM, s_mCM, d_mCM;

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
     *
     * Note this only writes pre-computed data.  Thus if you have
     * not yet asked NCellularData to compute anything about this
     * triangulation, writeTextShort may be empty. 
     *
     * @param out the stream to write to.
     */
    virtual void writeTextShort(std::ostream& out) const;

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
    long eulerChar() const;

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
    bool chainComplexesVerified() const;

    /**
     * If all the chain complexes check out, might as well verify a few basic isomorphisms as well. 
     * These are the isomorphisms between homology in standard, dual and mixed coordinates. 
     *
     * This is largely for debugging purposes as in any release this should always produce an answer
     * of true. 
     *
     * @return true provided every natural map between the various homology groups computable that
     *         should be isomorphisms, actually are isomorphisms. 
     */
    bool coordinateIsomorphismsVerified() const;

    /**
     * Computes an NAbelianGroup or retrieves it from the precomputed pile. 
     */
    const NAbelianGroup& unmarkedGroup( const unsigned long dimension, 
		const unsigned long coefficients, const variance_type variance, 
		const homology_coordinate_system coordinates) const;
    /**
     * Computes an NMarkedAbelianGroup or retrieves it from the precomputed pile. 
     */
    const NMarkedAbelianGroup& markedGroup( const unsigned long dimension, 
		const unsigned long coefficients, const variance_type variance, 
		const homology_coordinate_system coordinates) const;
    /**
     * Computes an NHomMarkedAbelianGroup or retrieves it from the precomputed pile. 
     */
    const NHomMarkedAbelianGroup& homGroup( const unsigned long dom_dimension, 
		const unsigned long dom_coefficients, const variance_type dom_variance, 
		const homology_coordinate_system dom_coordinates,  
		const unsigned long ran_dimension, 
		const unsigned long ran_coefficients, const variance_type ran_variance, 
		const homology_coordinate_system ran_coordinates) const;


    //todo: bilinear forms return object for Poincare duality objects.

};

/*@}*/

// Inline functions for NCellularData

// copy constructor
inline NCellularData::NCellularData(const NCellularData& g) : ShareableObject(),
        tri4(clonePtr(g.tri4)), tri3(clonePtr(g.tri3)), 
	nicIx(g.nicIx), icIx(g.icIx), dcIx(g.dcIx), bcIx(g.bcIx), 
	sCC(g.sCC.size()), dCC(g.dCC.size()), mCC(g.mCC.size()), bsCC(g.bsCC.size()), 
	bs_sCM(g.bs_sCM.size()), s_mCM(g.s_mCM.size()), d_mCM(g.d_mCM.size())
{
// copy abelianGroups, markedAbelianGroups, homMarkedAbelianGroups
// todo!

// numStandardCells[5], numDualCells[5], numMixCells[5], numStandardBdryCells[4], 
//               numNonIdealCells[5], numIdealCells[4];
for (unsigned long i=0; i<5; i++) numStandardCells[i] = g.numStandardCells[i];
for (unsigned long i=0; i<5; i++) numDualCells[i] = g.numDualCells[i];
for (unsigned long i=0; i<5; i++) numMixCells[i] = g.numMixCells[i];
for (unsigned long i=0; i<4; i++) numStandardBdryCells[i] = g.numStandardBdryCells[i];
for (unsigned long i=0; i<5; i++) numNonIdealCells[i] = g.numNonIdealCells[i];
for (unsigned long i=0; i<4; i++) numIdealCells[i] = g.numIdealCells[i];

// the chain complexes and maps
for (unsigned long i=0; i<sCC.size(); i++)       sCC[i] = clonePtr(g.sCC[i]);
for (unsigned long i=0; i<dCC.size(); i++)       dCC[i] = clonePtr(g.dCC[i]);
for (unsigned long i=0; i<mCC.size(); i++)       mCC[i] = clonePtr(g.mCC[i]);
for (unsigned long i=0; i<bsCC.size(); i++)     bsCC[i] = clonePtr(g.bsCC[i]);
for (unsigned long i=0; i<bs_sCM.size(); i++) bs_sCM[i] = clonePtr(g.bs_sCM[i]);
for (unsigned long i=0; i<s_mCM.size(); i++)   s_mCM[i] = clonePtr(g.s_mCM[i]);
for (unsigned long i=0; i<d_mCM.size(); i++)   d_mCM[i] = clonePtr(g.d_mCM[i]);
}

// destructor
inline NCellularData::~NCellularData() {
 if (tri3) delete tri3; if (tri4) delete tri4;
 // iterate through abelianGroups, markedAbelianGroups, homMarkedAbelianGroups and deallocate
 // todo...

 // iterate through sCC, dCC, mCC, bsCC and deallocate
 for (unsigned long i=0; i<sCC.size(); i++)   if (sCC[i])  delete sCC[i];
 for (unsigned long i=0; i<dCC.size(); i++)   if (dCC[i])  delete dCC[i];
 for (unsigned long i=0; i<mCC.size(); i++)   if (mCC[i])  delete mCC[i];
 for (unsigned long i=0; i<bsCC.size(); i++)  if (bsCC[i]) delete bsCC[i];
 // iterate through bs_sCM, s_mCM, d_mCM and deallocate
 for (unsigned long i=0; i<bs_sCM.size(); i++) if (bs_sCM[i]) delete bs_sCM[i];
 for (unsigned long i=0; i<s_mCM.size(); i++)  if (s_mCM[i])  delete s_mCM[i];
 for (unsigned long i=0; i<d_mCM.size(); i++)  if (d_mCM[i])  delete d_mCM[i];
}

inline unsigned long NCellularData::standardCellCount(unsigned dimension) const
{ return numStandardCells[dimension]; }
inline unsigned long NCellularData::dualCellCount(unsigned dimension) const
{ return numDualCells[dimension]; }
inline unsigned long NCellularData::boundaryCellCount(unsigned dimension) const
{ return numStandardBdryCells[dimension]; }
inline unsigned long NCellularData::mixedCellCount(unsigned dimension) const
{ return numMixCells[dimension]; }
inline long int NCellularData::eulerChar() const
{ return numDualCells[0]-numDualCells[1]+numDualCells[2]-numDualCells[3]+numDualCells[4]; }

} // namespace regina

#endif

