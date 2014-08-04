
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>

#include "algebra/nfibring.h"

#include "dim2/dim2triangulation.h"
#include "dim2/dim2exampletriangulation.h"
#include "triangulation/ntriangulation.h"
#include "dim4/dim4triangulation.h"

#include "maths/nrational.h"
#include "testsuite/algebra/testalgebra.h"

using regina::NMapToS1;
using regina::Dim2Triangulation;
using regina::Dim2ExampleTriangulation;
using regina::NTriangulation;
using regina::NTetrahedron;
using regina::NPerm4;
using regina::NPerm5;
using regina::Dim4Triangulation;
using regina::Dim4Pentachoron;
using regina::NRational;

class NFibringTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NFibringTest);

    CPPUNIT_TEST(checkPrimitive);
    CPPUNIT_TEST(checkBundle);

    CPPUNIT_TEST_SUITE_END();

    // keeps track of triangulations and associated maps to S1.
    // these are allocated in setUp, and deallocated in tearDown.
    std::map< Dim2Triangulation*, NMapToS1* > fib2map;
    std::map< NTriangulation*, NMapToS1* > fib3map;
    std::map< Dim4Triangulation*, NMapToS1* > fib4map;

    // standard cocycles so that the fibnmap maps are actual fibre bundles.
    std::map< Dim2Triangulation*, std::vector<NRational> > cocy2;
    std::map< NTriangulation*,    std::vector<NRational> > cocy3;
    std::map< Dim4Triangulation*, std::vector<NRational> > cocy4;

    // triangulation names to aid in diagnotics.
    std::map< Dim2Triangulation*, std::string > name2;
    std::map< NTriangulation*, std::string > name3;
    std::map< Dim4Triangulation*, std::string > name4;

    public:
        void setUp() { // initialize triangulations, maps and cocycles
             // annulus
         Dim2Triangulation* temp2ptr( Dim2ExampleTriangulation::annulus() );
         fib2map.insert( std::pair< Dim2Triangulation*, NMapToS1* >
          ( temp2ptr, new NMapToS1( temp2ptr ) ) );
         std::vector< NRational > tempV(4);
         tempV[0] = NRational(1,10); tempV[1] = NRational(1,1);
         tempV[2] = NRational(9,10); tempV[3] = NRational(-1,1);
         cocy2.insert( std::pair< Dim2Triangulation*, std::vector< NRational > >
          ( temp2ptr, tempV ) );
         name2.insert( std::pair< Dim2Triangulation*, std::string >
          ( temp2ptr, "Annulus" ) );

             // moebius band
         temp2ptr = Dim2ExampleTriangulation::mobius();
         fib2map.insert( std::pair< Dim2Triangulation*, NMapToS1* >
          ( temp2ptr, new NMapToS1( temp2ptr ) ) );
         tempV.resize(2);
         tempV[0] = NRational(1,1); tempV[1] = NRational(2,1);
         cocy2.insert( std::pair< Dim2Triangulation*, std::vector< NRational > >
          ( temp2ptr, tempV ) );
         name2.insert( std::pair< Dim2Triangulation*, std::string >
          ( temp2ptr, "Moebius band" ) );

             // S1 x D2 - solid torus
         NTriangulation* temp3ptr( new NTriangulation() );
         NTetrahedron* tet0( temp3ptr->newTetrahedron() );
         tet0->joinTo( 3, tet0, NPerm4(1,3,0,2) ); 
         fib3map.insert( std::pair< NTriangulation*, NMapToS1* >
          ( temp3ptr, new NMapToS1( temp3ptr ) ) );
         tempV.resize(3);
         tempV[0] = NRational(1,1); tempV[1] = NRational(2,1);
         tempV[2] = NRational(3,1);
         cocy3.insert( std::pair< NTriangulation*, std::vector< NRational > >
          ( temp3ptr, tempV ) );
         name3.insert( std::pair< NTriangulation*, std::string >
          ( temp3ptr, "S1 x D2" ) );

            // S1 x S2
         temp3ptr = new NTriangulation();
         tet0 = temp3ptr->newTetrahedron();
         NTetrahedron* tet1( temp3ptr->newTetrahedron() );
         tet0->joinTo( 1, tet0, NPerm4(3,0,1,2) ); 
         tet0->joinTo( 2, tet1, NPerm4(2,3,0,1) );
         tet0->joinTo( 3, tet1, NPerm4(2,3,0,1) ); 
         tet1->joinTo( 3, tet1, NPerm4(3,0,1,2) );
         tempV.resize(3);
         tempV[0] = NRational(3,1); tempV[1] = NRational(2,1);
         tempV[2] = NRational(1,1); 
         cocy3.insert( std::pair< NTriangulation*, std::vector< NRational > >
          ( temp3ptr, tempV ) );
         name3.insert( std::pair< NTriangulation*, std::string >
          ( temp3ptr, "S1 x S2" ) );

            // S1 x~ D3
         Dim4Triangulation* temp4ptr( new Dim4Triangulation() );
         Dim4Pentachoron* pen0( temp4ptr->newPentachoron() );
         pen0->joinTo( 1, pen0, NPerm5( 4,0,1,2,3 ) ); 
         fib4map.insert( std::pair< Dim4Triangulation*, NMapToS1* >
           ( temp4ptr, new NMapToS1( temp4ptr ) ) );
         tempV.resize(4);
         tempV[0] = NRational(4,1); tempV[1] = NRational(3,1);
         tempV[2] = NRational(2,1); tempV[3] = NRational(1,1);
         cocy4.insert( std::pair< Dim4Triangulation*, std::vector< NRational > >
          ( temp4ptr, tempV ) );
         name4.insert( std::pair< Dim4Triangulation*, std::string >
          ( temp4ptr, "S1 x~ D3" ) );

            // S1 x S3
         temp4ptr = new Dim4Triangulation();
         pen0 = temp4ptr->newPentachoron();
         Dim4Pentachoron* pen1( temp4ptr->newPentachoron() );
         pen0->joinTo( 4, pen1, NPerm5( 3,0,1,2,4 ) );  
         pen0->joinTo( 3, pen1, NPerm5( 1,2,4,3,0 ) ); // (04)
         pen0->joinTo( 2, pen1, NPerm5( 0,1,2,4,3 ) );  
         pen0->joinTo( 1, pen1, NPerm5( 0,1,2,4,3 ) ); // (43)
         pen0->joinTo( 0, pen1, NPerm5( 0,1,2,4,3 ) ); 
         tempV.resize(4);
         tempV[0] = NRational(1,1); tempV[1] = NRational(2,1);
         tempV[2] = NRational(3,1); tempV[3] = NRational(-4,1);
         cocy4.insert( std::pair< Dim4Triangulation*, std::vector< NRational > >
          ( temp4ptr, tempV ) );
         name4.insert( std::pair< Dim4Triangulation*, std::string >
          ( temp4ptr, "S1 x S3" ) );
        }

        void tearDown() { // delete triangulations
         for (std::map< Dim2Triangulation*, NMapToS1* >::iterator i=fib2map.begin();    
              i!=fib2map.end(); i++)
          { delete i->first; delete i->second; }

         for (std::map< NTriangulation*, NMapToS1* >::iterator i=fib3map.begin();    
              i!=fib3map.end(); i++)
          { delete i->first; delete i->second; }

         for (std::map< Dim4Triangulation*, NMapToS1* >::iterator i=fib4map.begin();    
              i!=fib4map.end(); i++)
          { delete i->first; delete i->second; }
        }
    
        void checkPrimitive() { // tests of verifyPrimitiveH1
          std::stringstream dump;
          for (std::map< Dim2Triangulation*, NMapToS1* >::iterator i=fib2map.begin();
               i!=fib2map.end(); i++)
           if (!i->second->verifyPrimitiveH1( cocy2[i->first] ) ) {
            dump << "NMapToS1::verifyPrimitiveH1() error 2 on " <<
                    name2[i->first] << ".";
            CPPUNIT_FAIL( dump.str() );
            }

          for (std::map< NTriangulation*, NMapToS1* >::iterator i=fib3map.begin();
               i!=fib3map.end(); i++)
           if (!i->second->verifyPrimitiveH1( cocy3[i->first] ) ) {
            dump << "NMapToS1::verifyPrimitiveH1() error 3 on " <<
                    name3[i->first] << ".";
            CPPUNIT_FAIL( dump.str() );
            }

          for (std::map< Dim4Triangulation*, NMapToS1* >::iterator i=fib4map.begin();
               i!=fib4map.end(); i++)
           if (!i->second->verifyPrimitiveH1( cocy4[i->first] ) ) {
            dump << "NMapToS1::verifyPrimitiveH1() error 4 on " <<
                     name4[i->first] << ".";
            CPPUNIT_FAIL( dump.str() );
            }
        }

        void checkBundle() { // tests of verifySimpleS1Bundle
          std::stringstream dump;
          for (std::map< Dim2Triangulation*, NMapToS1* >::iterator i=fib2map.begin();
               i!=fib2map.end(); i++)
           if (!i->second->verifySimpleS1Bundle( cocy2[i->first] ) ) {
            dump << "NMapToS1::verifySimpleS1Bundle() error 2 on " <<
                    name2[i->first]<<".";
            CPPUNIT_FAIL( dump.str() );
            }

          for (std::map< NTriangulation*, NMapToS1* >::iterator i=fib3map.begin();
               i!=fib3map.end(); i++)
           if (!i->second->verifySimpleS1Bundle( cocy3[i->first] ) ) {
            dump << "NMapToS1::verifySimpleS1Bundle() error 3 on " <<
                    name3[i->first]<<".";
            CPPUNIT_FAIL(dump.str());
            }

          for (std::map< Dim4Triangulation*, NMapToS1* >::iterator i=fib4map.begin();
               i!=fib4map.end(); i++)
           if (!i->second->verifySimpleS1Bundle( cocy4[i->first] ) ) {
            dump << "NMapToS1::verifySimpleS1Bundle() error 4 on " <<
                    name4[i->first] <<".";
            CPPUNIT_FAIL(dump.str());
            }
        }

        // TODO triangulateFibre tests.
        // 
        // TODO eventually some more sophisticated tests. Various tests where
        //  the cocycles are not bundles, nor primitive.  Also strength tests
        //  for the algorithms that search for bundle structures.
};

void addNFibring(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NFibringTest::suite());
}

