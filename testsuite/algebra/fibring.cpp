
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

#include "algebra/fibring.h"

#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

#include "triangulation/example2.h"
#include "maths/rational.h"

#include "testsuite/algebra/testalgebra.h"

using namespace regina;

class NFibringTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NFibringTest);

    CPPUNIT_TEST(checkPrimitive);
    CPPUNIT_TEST(checkBundle);

    CPPUNIT_TEST_SUITE_END();

    // keeps track of triangulations and associated maps to S1.
    // these are allocated in setUp, and deallocated in tearDown.
    std::map< Triangulation<2>*, NMapToS1* > fib2map;
    std::map< Triangulation<3>*, NMapToS1* > fib3map;
    std::map< Triangulation<4>*, NMapToS1* > fib4map;

    // standard cocycles so that the fibnmap maps are actual fibre bundles.
    std::map< Triangulation<2>*, std::vector<Rational> > cocy2;
    std::map< Triangulation<3>*,    std::vector<Rational> > cocy3;
    std::map< Triangulation<4>*, std::vector<Rational> > cocy4;

    // triangulation names to aid in diagnotics.
    std::map< Triangulation<2>*, std::string > name2;
    std::map< Triangulation<3>*, std::string > name3;
    std::map< Triangulation<4>*, std::string > name4;

    public:
        void setUp() { // initialize triangulations, maps and cocycles
             // annulus
         Triangulation<2>* temp2ptr( Dim2ExampleTriangulation::annulus() );
         fib2map.insert( std::pair< Triangulation<2>*, NMapToS1* >
          ( temp2ptr, new NMapToS1( temp2ptr ) ) );
         std::vector< Rational > tempV(4);
         tempV[0] = Rational(1,10); tempV[1] = Rational(1,1);
         tempV[2] = Rational(9,10); tempV[3] = Rational(-1,1);
         cocy2.insert( std::pair< Triangulation<2>*, std::vector< Rational > >
          ( temp2ptr, tempV ) );
         name2.insert( std::pair< Triangulation<2>*, std::string >
          ( temp2ptr, "Annulus" ) );

             // moebius band
         temp2ptr = Dim2ExampleTriangulation::mobius();
         fib2map.insert( std::pair< Triangulation<2>*, NMapToS1* >
          ( temp2ptr, new NMapToS1( temp2ptr ) ) );
         tempV.resize(2);
         tempV[0] = Rational(1,1); tempV[1] = Rational(2,1);
         cocy2.insert( std::pair< Triangulation<2>*, std::vector< Rational > >
          ( temp2ptr, tempV ) );
         name2.insert( std::pair< Triangulation<2>*, std::string >
          ( temp2ptr, "Moebius band" ) );

             // S1 x D2 - solid torus
         Triangulation<3>* temp3ptr( new Triangulation<3>() );
         Simplex<3>* tet0( temp3ptr->newTetrahedron() );
         tet0->join( 3, tet0, NPerm4(1,3,0,2) ); 
         fib3map.insert( std::pair< Triangulation<3>*, NMapToS1* >
          ( temp3ptr, new NMapToS1( temp3ptr ) ) );
         tempV.resize(3);
         tempV[0] = Rational(1,1); tempV[1] = Rational(2,1);
         tempV[2] = Rational(3,1);
         cocy3.insert( std::pair< Triangulation<3>*, std::vector< Rational > >
          ( temp3ptr, tempV ) );
         name3.insert( std::pair< Triangulation<3>*, std::string >
          ( temp3ptr, "S1 x D2" ) );

            // S1 x S2
         temp3ptr = new Triangulation<3>();
         tet0 = temp3ptr->newTetrahedron();
         Simplex<3>* tet1( temp3ptr->newTetrahedron() );
         tet0->join( 1, tet0, NPerm4(3,0,1,2) ); 
         tet0->join( 2, tet1, NPerm4(2,3,0,1) );
         tet0->join( 3, tet1, NPerm4(2,3,0,1) ); 
         tet1->join( 3, tet1, NPerm4(3,0,1,2) );
         tempV.resize(3);
         tempV[0] = Rational(3,1); tempV[1] = Rational(2,1);
         tempV[2] = Rational(1,1); 
         cocy3.insert( std::pair< Triangulation<3>*, std::vector< Rational > >
          ( temp3ptr, tempV ) );
         name3.insert( std::pair< Triangulation<3>*, std::string >
          ( temp3ptr, "S1 x S2" ) );

            // S1 x~ D3
         Triangulation<4>* temp4ptr( new Triangulation<4>() );
         Simplex<4>* pen0( temp4ptr->newPentachoron() );
         pen0->join( 1, pen0, NPerm5( 4,0,1,2,3 ) ); 
         fib4map.insert( std::pair< Triangulation<4>*, NMapToS1* >
           ( temp4ptr, new NMapToS1( temp4ptr ) ) );
         tempV.resize(4);
         tempV[0] = Rational(4,1); tempV[1] = Rational(3,1);
         tempV[2] = Rational(2,1); tempV[3] = Rational(1,1);
         cocy4.insert( std::pair< Triangulation<4>*, std::vector< Rational > >
          ( temp4ptr, tempV ) );
         name4.insert( std::pair< Triangulation<4>*, std::string >
          ( temp4ptr, "S1 x~ D3" ) );

            // S1 x S3
         temp4ptr = new Triangulation<4>();
         pen0 = temp4ptr->newPentachoron();
         Simplex<4>* pen1( temp4ptr->newPentachoron() );
         pen0->join( 4, pen1, NPerm5( 3,0,1,2,4 ) );  
         pen0->join( 3, pen1, NPerm5( 1,2,4,3,0 ) ); // (04)
         pen0->join( 2, pen1, NPerm5( 0,1,2,4,3 ) );  
         pen0->join( 1, pen1, NPerm5( 0,1,2,4,3 ) ); // (43)
         pen0->join( 0, pen1, NPerm5( 0,1,2,4,3 ) ); 
         tempV.resize(4);
         tempV[0] = Rational(1,1); tempV[1] = Rational(2,1);
         tempV[2] = Rational(3,1); tempV[3] = Rational(-4,1);
         cocy4.insert( std::pair< Triangulation<4>*, std::vector< Rational > >
          ( temp4ptr, tempV ) );
         name4.insert( std::pair< Triangulation<4>*, std::string >
          ( temp4ptr, "S1 x S3" ) );
        }

        void tearDown() { // delete triangulations
         for (std::map< Triangulation<2>*, NMapToS1* >::iterator i=fib2map.begin();    
              i!=fib2map.end(); i++)
          { delete i->first; delete i->second; }

         for (std::map< Triangulation<3>*, NMapToS1* >::iterator i=fib3map.begin();    
              i!=fib3map.end(); i++)
          { delete i->first; delete i->second; }

         for (std::map< Triangulation<4>*, NMapToS1* >::iterator i=fib4map.begin();    
              i!=fib4map.end(); i++)
          { delete i->first; delete i->second; }
        }
    
        void checkPrimitive() { // tests of verifyPrimitiveH1
          std::stringstream dump;
          for (std::map< Triangulation<2>*, NMapToS1* >::iterator i=fib2map.begin();
               i!=fib2map.end(); i++)
           if (!i->second->verifyPrimitiveH1( cocy2[i->first] ) ) {
            dump << "NMapToS1::verifyPrimitiveH1() error 2 on " <<
                    name2[i->first] << ".";
            CPPUNIT_FAIL( dump.str() );
            }

          for (std::map< Triangulation<3>*, NMapToS1* >::iterator i=fib3map.begin();
               i!=fib3map.end(); i++)
           if (!i->second->verifyPrimitiveH1( cocy3[i->first] ) ) {
            dump << "NMapToS1::verifyPrimitiveH1() error 3 on " <<
                    name3[i->first] << ".";
            CPPUNIT_FAIL( dump.str() );
            }

          for (std::map< Triangulation<4>*, NMapToS1* >::iterator i=fib4map.begin();
               i!=fib4map.end(); i++)
           if (!i->second->verifyPrimitiveH1( cocy4[i->first] ) ) {
            dump << "NMapToS1::verifyPrimitiveH1() error 4 on " <<
                     name4[i->first] << ".";
            CPPUNIT_FAIL( dump.str() );
            }
        }

        void checkBundle() { // tests of verifySimpleS1Bundle
          std::stringstream dump;
          for (std::map< Triangulation<2>*, NMapToS1* >::iterator i=fib2map.begin();
               i!=fib2map.end(); i++)
           if (!i->second->verifySimpleS1Bundle( cocy2[i->first] ) ) {
            dump << "NMapToS1::verifySimpleS1Bundle() error 2 on " <<
                    name2[i->first]<<".";
            CPPUNIT_FAIL( dump.str() );
            }

          for (std::map< Triangulation<3>*, NMapToS1* >::iterator i=fib3map.begin();
               i!=fib3map.end(); i++)
           if (!i->second->verifySimpleS1Bundle( cocy3[i->first] ) ) {
            dump << "NMapToS1::verifySimpleS1Bundle() error 3 on " <<
                    name3[i->first]<<".";
            CPPUNIT_FAIL(dump.str());
            }

          for (std::map< Triangulation<4>*, NMapToS1* >::iterator i=fib4map.begin();
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

void addFibring(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NFibringTest::suite());
}

