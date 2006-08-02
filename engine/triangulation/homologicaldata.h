
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file homologicaldata.h
 *  \brief Deals with all the details of cellular homology of a manifold.
 */

#ifndef __HOMOLOGICALDATA_H
#ifndef __DOXYGEN
#define __HOMOLOGICALDATA_H
#endif

#include <vector>
#include "utilities/nmpi.h"
#include "utilities/nrational.h"
#include "utilities/nindexedarray.h"
#include "shareableobject.h"
#include "maths/nmatrixint.h"
#include "maths/nmatrix.h"
#include "triangulation/marked_abeliangroup.h"
#include "triangulation/ntriangulation.h"
#include <sstream>

namespace regina {

class NFile;
class NMatrixInt; // what are these for?
//class NTriangulation;

/**
 * \addtogroup algebra Algebraic Structures
 * Various algebraic structures.
 * @{
 */

/**
 * Data type that deals with all the detailed homological information in a
 * manifold, including:
 *   a) the manifold's homology
 *   b) the boundary's homology
 *   c) the map from boundary -> manifold
 *   d) the dual cellular homology
 *   e) the isomorphism on H1 from the dual cellular homology to the regular cellular
 *      homology.
 *   f) the H1 torsion form...
 * 
 * @author Ryan Budney 
 * 
 * \testpart
 *
 */
class homologicalData : public ShareableObject{
    protected:
        NTriangulation* tri; // this is the triangulation that it is initialized by.

        MarkedAbelianGroup* mHomology0; // manifolds `regular' cellular homology groups
        bool mHomology0Computed;
        MarkedAbelianGroup* mHomology1; 
        bool mHomology1Computed;
        MarkedAbelianGroup* mHomology2; 
        bool mHomology2Computed;
        MarkedAbelianGroup* mHomology3; 
        bool mHomology3Computed;

        MarkedAbelianGroup* bHomology0; // boundary `regular' cellular homology groups
        bool bHomology0Computed;
        MarkedAbelianGroup* bHomology1;
        bool bHomology1Computed;
        MarkedAbelianGroup* bHomology2;
        bool bHomology2Computed;

        HomMarkedAbelianGroup* bmMap0; // maps from bHomology? to mHomology?
        bool bmMap0Computed;
        HomMarkedAbelianGroup* bmMap1;
        bool bmMap1Computed;
        HomMarkedAbelianGroup* bmMap2;
        bool bmMap2Computed;

        MarkedAbelianGroup* dmHomology0; // manifold's `dual' cellular homology groups
        bool dmHomology0Computed;
        MarkedAbelianGroup* dmHomology1;
        bool dmHomology1Computed;
        MarkedAbelianGroup* dmHomology2;
        bool dmHomology2Computed;
        MarkedAbelianGroup* dmHomology3;
        bool dmHomology3Computed;

        HomMarkedAbelianGroup* dmTomMap1; // isomorphism from dual H1 to regular H1.
        bool dmTomMap1Computed;

        // below here and the public declaration go the internal bits of data that are not
        // publicly accessible...
        
        // the chain complexes for the regular cellular homology

        bool ccIndexingComputed;

        std::vector<unsigned long> numStandardCells; // number of cells of dimension 0, 1, 2, 3.
        std::vector<unsigned long> numDualCells; // dual cells
        std::vector<unsigned long> numBdryCells; // standard boundary cells

        NIndexedArray<unsigned long> sNIV;   // non-ideal vertices
        NIndexedArray<unsigned long> sIEOE;  // ideal endpoints of edges
        NIndexedArray<unsigned long> sIEEOF; // ideal end edges of faces
        NIndexedArray<unsigned long> sIEFOT; // ideal end faces of tetrahedra
        NIndexedArray<unsigned long> dNINBV; // nonideal nonboundary vertices
        NIndexedArray<unsigned long> dNBE;   // non-boundary edges
        NIndexedArray<unsigned long> dNBF;   // non-boundary faces
        NIndexedArray<unsigned long> sBNIV;  // boundary non-ideal vertices
        NIndexedArray<unsigned long> sBNIE;  // boundary non-ideal edges
        NIndexedArray<unsigned long> sBNIF;  // boundary non-ideal faces


/**              chain complexes for the corresponding transverse CW
             decompositions of an ideal triangulation.
             These routines are called with B1, B2, B3 
             pre-initialized to the correct dimensions. 
        
             We describe the canonical ordering of both the cells and
             dual cells of tri.

             The standard CW decomposition -- this is the one that most
             closely resembles the ideal triangulation. 

             0-cells: The non-ideal vertices given in the order vertices.begin() to vertices.end()
                        followed by the ideal endpoints of the edges in lexicographical order
                        edges.begin() to edges.end() followed by the endpts 0, 1.
             1-cells: edges.begin() to edges.end() followed by the ideal edges of
                        faces.begin() to faces.end() in order 0,1,2.
             2-cells: faces.begin() to faces.end() followed by ideal faces of
                        tetrahedra.begin() through tetrahedra.end() in order 0,1,2,3
             3-cells: tetrahedra.begin() through tetrahedra.end()

             The Dual CW decomposition -- if the above CW-decomposition came from a morse
                function f, this would be the one for -f. 

             0-cells: tetrahedra.begin() through tetrahedra.end()
             1-cells: the non-boundary faces.begin() through faces.end()
             2-cells: the non-boundary edges.begin() through edges.end()
             3-cells: the non-boundary, non-ideal vertices.begin() through vertices.end()
**/
        bool chainComplexesComputed;

        NMatrixInt* A0; // cellular homology chain complex using regular cells
        NMatrixInt* A1;
        NMatrixInt* A2;
        NMatrixInt* A3;
        NMatrixInt* A4;

        NMatrixInt* B0; // cellular homology chain complex using dual cells
        NMatrixInt* B1;
        NMatrixInt* B2;
        NMatrixInt* B3;
        NMatrixInt* B4;

        NMatrixInt* Bd0; // boundary cellular homology chain complex using regular cells
        NMatrixInt* Bd1;
        NMatrixInt* Bd2;
        NMatrixInt* Bd3;

        NMatrixInt* B0Incl; // chain map from H0 boundary --> H0 manifold
        NMatrixInt* B1Incl; // chain map from H1 boundary --> H1 manifold
        NMatrixInt* B2Incl; // chain map from H2 boundary --> H2 manifold

        NMatrixInt* H1map; // chain map from H1 manifold w/dual cells to H1 manifold w/ regular cells.

        void computeccIndexing(); // computes the indexing of the cells in the chain complexes.
        void computeChainComplexes(); // computes all chain complexes.
        void computeHomology();  // computes all homology groups
        void computeBHomology(); // computes all boundary homology groups
        void computeDHomology(); // computes all dual homology groups
        void computeBIncl(); // computes all HomMarkedAbelianGroup corresp to boundary inclusion

        // torsion form stuff
        bool torsionFormComputed; // this is true if and only if the stuff below is
                                  // computed.

        // the new format will be:
        // h1PrimePowerDecomp a vector<NLargeInteger> that contains the prime power decomposition of
        //        h1 of the 3-manifold
        // pTorsionH1Mat an NMatrixInt pointer containing the corresponding chain complex vectors
        //         corresponding to the above prime power decomposition.
        // linkingFormPD        a vector< NMatrixRing<NRational>* > 
        //         giving the primary decomposition of the torsion linking form matrix.
        std::vector< std::pair< NLargeInteger, std::vector<unsigned long> > > h1PrimePowerDecomp; 
                                   // prime power decomposition of the torsion subgroup of
                                   // H1 of the manifold, in increasing lexicographical order (bi-indexed)
                                   // eg: { (2 2 4), (3 9 9 27), (5 5 25) }
                                   // stored as list { (2, (1, 1, 2)), (3, (1, 2, 2, 3)), (5, (1, 1, 2)) }
        std::vector< NMatrixRing<NRational>* > linkingFormPD;
                                   // the p-primary decomposition of the torsion linking form,
                                   // thus in the above example this would be a vector of 3 matrices,
                                   // 1st a 3x3, 2nd a 4x4 and 3rd a 3x3. corresponding to 2, 3 and 5-torsion
                                   // factors.

        bool torsionLinkingFormIsHyperbolic;
        bool torsionLinkingFormIsSplit;
        bool torsionLinkingFormSatisfiesKKtwoTorCondition;

        // these are the Kawauchi-Kojima system of invariants of the torsion linking form.
        std::vector< std::pair< NLargeInteger, 
                     std::vector< unsigned long > > > torRankV;
        std::vector< NLargeInteger > twoTorSigmaV; // 2-torsion invariant
        std::vector< std::pair< NLargeInteger, std::vector< int > > > oddTorLegSymV;
                // the odd p-torsion invariant.

        std::string torsionRankString;
        std::string torsionSigmaString;
        std::string torsionLegendreString;
        std::string embedabilityString;

    public:

        /**
         * Takes as input a triangulation.
         */
        homologicalData(const NTriangulation& input);
        /**
         * Copy constructor.
         */
        homologicalData(const homologicalData& g);
        /**
         * Destructor.
         */
        virtual ~homologicalData();
        /**
         * Needed as a member of SharableObject
         */
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * This routine gives access to the manifold's homology computed
         * with the regular CW-decomposition.
         */
        MarkedAbelianGroup getMH(unsigned q);
           //the manifold's homology groups, computed with the standard CW
           //decomposition. This is typically slower than getDMH? since
           //getDMH? uses the dual cw-decomposition which typically has
           //an order of magnitude less cells.
        /**
         * This routine gives access to the homology of the boundary
         * of the manifold, computed
         * with the regular CW-decomposition.
         */
        MarkedAbelianGroup getBMH(unsigned q);//boundary homology groups

        /**
         * This routine gives access to the homomorphism from the 
         * homology of the boundary to the homology of the manifold.
         */
        HomMarkedAbelianGroup getBMmapH(unsigned q);//map from boundary to the manifold

        /**
         * This routine gives access to the manifold's homology computed
         * with the dual CW-decomposition.
         */
        MarkedAbelianGroup getDMH(unsigned q);//manifold's homology computed with the dual CW-decomposition
        
        /**
         * This routine gives access to the isomorphism from getDMH(1) to
         * getMH(1) given by a cellular approximation to the identity map
         * on the manifold.
         */
        HomMarkedAbelianGroup getH1cellap();//isomorphism from getDMH1 to getMH1 computed via a 
                                        // cellular approximation of the identity map.

        /**
         * A list of the number of cells in the standard CW-decomposition
         * of the manifold.
         */
        std::vector<unsigned long> getNumStandardCells(); // number of cells of dimension 0, 1, 2, 3.
        /**
         * A list of the number of cells in the dual CW-decomposition
         * of the manifold. This is typically much smaller than
         * getNumStandardCells.
         */
        std::vector<unsigned long> getNumDualCells(); // dual cells
        /**
         * A list of the number of cells in the standard CW-decomposition
         * of the boundary of the manifold. This is the subcomplex of
         * the complex used in getNumStandardCells.
         */
        std::vector<unsigned long> getNumBdryCells(); // standard boundary cells
        /**
         * The proper euler characteristic of the manifold, computed from
         * getNumDualCells. 
         */
        long int getEulerChar(); // euler characteristic

        /**
         * temp torsion linking form routine
         */
        void computeTorsionLinkingForm();
        /**
         * Returns the torsion subgroup rank vector. This is one of
         * 3 of the Kawauchi-Kojima complete invariants of the torsion
         * linking form.
         */
        std::vector< std::pair< NLargeInteger, 
                     std::vector< unsigned long > > > getTorsionRankVector() const;
        std::string getTorsionRankVectorString() const;
        /**
         * Returns the 2-torsion sigma vector. This is one of
         * 3 of the Kawauchi-Kojima complete invariants of the torsion
         * linking form.
         */
        std::vector< NLargeInteger > getTorsionSigmaVector() const;
        std::string getTorsionSigmaVectorString() const;

        /**
         * Returns the odd p-torsion Legendre symbol vector. This is one of
         * 3 of the Kawauchi-Kojima complete invariants of the torsion
         * linking form.
         */
        std::vector< std::pair< NLargeInteger, std::vector< int > > > 
           getLegendreSymbolVector() const;
        std::string getTorsionLegendreSymbolVectorString() const;

        /**
         * Returns true iff torsion linking form is hyperbolic.
         */
        bool formIsHyperbolic() const;
        /**
         * Returns true iff torsion linking form is split.
         */
        bool formIsSplit() const;
        /**
         * Returns true iff torsion linking form satisfies the
         * Kawauchi-Kojima 2-torsion condition that on all elements
         * x of order 2^k, 2^{k-1}form(x,x) == 0
         */
        bool formSatKK() const;
        /**
         * Returns a comment on if the manifold might embed in
         * a homology 3-sphere or 4-sphere.
         *
         */
        std::string getEmbeddabilityComment() const;
};

/*@}*/

// Inline functions for homologicalData

// constructor
inline homologicalData::homologicalData(const NTriangulation& input):
        ShareableObject(), 

        mHomology0(0),        mHomology0Computed(false),
        mHomology1(0),         mHomology1Computed(false),
        mHomology2(0),         mHomology2Computed(false),
        mHomology3(0),         mHomology3Computed(false),

        bHomology0(0),        bHomology0Computed(false),
        bHomology1(0),        bHomology1Computed(false),
        bHomology2(0),        bHomology2Computed(false),

        bmMap0(0),        bmMap0Computed(false),
        bmMap1(0),        bmMap1Computed(false),
        bmMap2(0),        bmMap2Computed(false),

        dmHomology0(0),        dmHomology0Computed(false),
        dmHomology1(0),        dmHomology1Computed(false),
        dmHomology2(0),        dmHomology2Computed(false),
        dmHomology3(0),        dmHomology3Computed(false),

        dmTomMap1(0),        dmTomMap1Computed(false),

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
inline homologicalData::homologicalData(const homologicalData& g) :
        ShareableObject()
{
        // regular cellular homology
        mHomology0Computed = g.mHomology0Computed;
        mHomology1Computed = g.mHomology1Computed;
        mHomology2Computed = g.mHomology2Computed;
        mHomology3Computed = g.mHomology3Computed;

        if (mHomology0Computed) mHomology0 = new MarkedAbelianGroup(*g.mHomology0); else mHomology0=0;
         if (mHomology1Computed) mHomology1 = new MarkedAbelianGroup(*g.mHomology1); else mHomology1=0;
        if (mHomology2Computed) mHomology2 = new MarkedAbelianGroup(*g.mHomology2); else mHomology2=0;
        if (mHomology3Computed) mHomology3 = new MarkedAbelianGroup(*g.mHomology3); else mHomology3=0;

        // regular boundary cellular homology
        bHomology0Computed = g.bHomology0Computed;
        bHomology1Computed = g.bHomology1Computed;
        bHomology2Computed = g.bHomology2Computed;

        if (bHomology0Computed) bHomology0 = new MarkedAbelianGroup(*g.bHomology0); else bHomology0=0;
         if (bHomology1Computed) bHomology1 = new MarkedAbelianGroup(*g.bHomology1); else bHomology1=0;
        if (bHomology2Computed) bHomology2 = new MarkedAbelianGroup(*g.bHomology2); else bHomology2=0;

        // boundary map
        bmMap0Computed = g.bmMap0Computed;
        bmMap1Computed = g.bmMap1Computed;
        bmMap2Computed = g.bmMap2Computed;

        if (bmMap0Computed) bmMap0 = new HomMarkedAbelianGroup(*g.bmMap0); else bmMap0=0;
         if (bmMap1Computed) bmMap1 = new HomMarkedAbelianGroup(*g.bmMap1); else bmMap1=0;
        if (bmMap2Computed) bmMap2 = new HomMarkedAbelianGroup(*g.bmMap2); else bmMap2=0;

        // dual cellular homology
        dmHomology0Computed = g.dmHomology0Computed;
        dmHomology1Computed = g.dmHomology1Computed;
        dmHomology2Computed = g.dmHomology2Computed;
        dmHomology3Computed = g.dmHomology3Computed;

        if (dmHomology0Computed) dmHomology0 = new MarkedAbelianGroup(*g.dmHomology0); else dmHomology0=0;
         if (dmHomology1Computed) dmHomology1 = new MarkedAbelianGroup(*g.dmHomology1); else dmHomology1=0;
        if (dmHomology2Computed) dmHomology2 = new MarkedAbelianGroup(*g.dmHomology2); else dmHomology2=0;
        if (dmHomology3Computed) dmHomology3 = new MarkedAbelianGroup(*g.dmHomology3); else dmHomology3=0;

        // isomorphism between dual H1 and regular H1.
        dmTomMap1Computed = g.dmTomMap1Computed;

        if (dmTomMap1Computed) dmTomMap1 = new HomMarkedAbelianGroup(*g.dmTomMap1); else dmTomMap1=0;

        // the chain complexes...

        ccIndexingComputed = g.ccIndexingComputed;

        if (ccIndexingComputed)
         {
         numStandardCells = g.numStandardCells; // number of cells of dimension 0, 1, 2, 3.
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

        if (chainComplexesComputed)
         {
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
        if (torsionFormComputed)
         {
         h1PrimePowerDecomp = g.h1PrimePowerDecomp;
         linkingFormPD.resize( g.linkingFormPD.size(), 0 );
         for (unsigned long i=0; i<linkingFormPD.size(); i++)
           linkingFormPD[i] = new NMatrixRing<NRational> (*g.linkingFormPD[i]);
         }
}

// destructor
inline homologicalData::~homologicalData() 
{
delete tri;

if (mHomology0Computed) delete mHomology0;
if (mHomology1Computed) delete mHomology1;
if (mHomology2Computed) delete mHomology2;
if (mHomology3Computed) delete mHomology3;

if (bHomology0Computed) delete bHomology0;
if (bHomology1Computed) delete bHomology1;
if (bHomology2Computed) delete bHomology2;

if (bmMap0Computed) delete bmMap0;
if (bmMap1Computed) delete bmMap1;
if (bmMap2Computed) delete bmMap2;

if (dmHomology0Computed) delete dmHomology0;
if (dmHomology1Computed) delete dmHomology1;
if (dmHomology2Computed) delete dmHomology2;
if (dmHomology3Computed) delete dmHomology3;

if (dmTomMap1Computed) delete dmTomMap1;

if (chainComplexesComputed)
        {
        delete A0; delete A1; delete A2; delete A3; delete A4;
        delete B0; delete B1; delete B2; delete B3; delete B4;
        delete Bd0; delete Bd1; delete Bd2; delete Bd3; 
        delete B0Incl; delete B1Incl; delete B2Incl; delete H1map;
        }

if (torsionFormComputed)
 {
 for (unsigned long i=0; i<linkingFormPD.size(); i++)
        delete linkingFormPD[i];
 }
}

inline std::vector<unsigned long> homologicalData::getNumStandardCells() // number of cells of dimension 0, 1, 2, 3.
{
computeccIndexing();
return numStandardCells;
}

inline std::vector<unsigned long> homologicalData::getNumDualCells() // dual cells
{
computeccIndexing();
return numDualCells;
}

inline std::vector<unsigned long> homologicalData::getNumBdryCells() // standard boundary cells
{
computeccIndexing();
return numBdryCells;
}

inline long int homologicalData::getEulerChar() // euler characteristic
{
computeccIndexing();
return numDualCells[0]-numDualCells[1]+numDualCells[2]-numDualCells[3];
}

inline std::vector< std::pair< NLargeInteger, 
       std::vector< unsigned long > > > homologicalData::getTorsionRankVector() const
{ return torRankV; }
inline std::vector< NLargeInteger > homologicalData::getTorsionSigmaVector() const
{ return twoTorSigmaV; }
inline std::vector< std::pair< NLargeInteger, std::vector< int > > > 
           homologicalData::getLegendreSymbolVector() const
{ return oddTorLegSymV; }

inline bool homologicalData::formIsHyperbolic() const { return torsionLinkingFormIsHyperbolic; }
inline bool homologicalData::formIsSplit() const { return torsionLinkingFormIsSplit; }
inline bool homologicalData::formSatKK() const { return torsionLinkingFormSatisfiesKKtwoTorCondition; }


inline std::string homologicalData::getTorsionRankVectorString() const
{return torsionRankString;}

inline std::string homologicalData::getTorsionSigmaVectorString() const
{return torsionSigmaString;}

inline std::string homologicalData::getTorsionLegendreSymbolVectorString() const
{return torsionLegendreString;}

inline std::string homologicalData::getEmbeddabilityComment() const
{return embedabilityString;}



 
} // namespace regina

#endif

