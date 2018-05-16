
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

#include "maths/sparsegrid.h"
#include "maths/integer.h"
#include "maths/matrix.h"

#include "algebra/bilinearform.h"
#include "algebra/cellulardata.h"
#include "algebra/markedabeliangroup.h"

#include "triangulation/example3.h"
#include "triangulation/dim3/triangulation3.h"

#include "testsuite/utilities/testutilities.h"

using regina::Integer;
using regina::Triangulation;
using regina::NExampleTriangulation;
using regina::MarkedAbelianGroup;
using regina::HomMarkedAbelianGroup;
using regina::CellularData;
using regina::MatrixInt;
using regina::BilinearForm;
using regina::SparseGridRing;

class BilinearFormTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(BilinearFormTest);

    CPPUNIT_TEST(constructors_test);
    CPPUNIT_TEST(symmetry_test);
    CPPUNIT_TEST(composition_test);

    CPPUNIT_TEST_SUITE_END();
 
    void copyAndDelete(Triangulation<3>& dest, Triangulation<3>* source) {
            dest.insertTriangulation(*source);
            delete source; }

    private:

    Triangulation<3> weberSeifert;
    Triangulation<3> s1s1s1, comp1;

    std::vector< CellularData* > cdList;

    public:

    void setUp() {
      copyAndDelete(weberSeifert, NExampleTriangulation::weberSeifert());
	  s1s1s1.insertRehydration("gepaadcefeffnkkanax");
	  comp1.insertRehydration("jgofiaaaceedfhiiifkxkfnbtxe");

      cdList.resize(3);
      cdList[0] = new CellularData( weberSeifert ); 
      cdList[1] = new CellularData( s1s1s1 );
      cdList[2] = new CellularData( comp1 );
     } // end setUp

   void tearDown() {
    for (unsigned long i=0; i<cdList.size(); i++) delete cdList[i];  }

   void constructors_test() {
   // let's check that the standard inner product on R^n is an iso 
   // between R^n and its dual for all n == 1,2,3,...,10, say.
   for (unsigned long dim=1; dim<11; dim++)
    {
     MarkedAbelianGroup ldom( dim, Integer::zero );
     MarkedAbelianGroup rdom( dim, Integer::zero );
     MarkedAbelianGroup zed( 1, Integer::zero );
     SparseGridRing< Integer > pairing(3);
     for (unsigned long i=0; i<dim; i++)
      {
       regina::MultiIndex< unsigned long > I(3);
       I[0]=i; I[1]=i; I[2]=0;
       pairing.incEntry( I, Integer::one );
      }
     BilinearForm innP(ldom, rdom, zed, pairing);
     if (!innP.leftAdjoint().isIsomorphism()) 
        CPPUNIT_FAIL("Left-adjoint to standard inner product on R^n"
                     " is not isomorphism.");
     if (!innP.rightAdjoint().isIsomorphism()) 
      CPPUNIT_FAIL("Right-adjoint to standard inner product on R^n is"
                   " not isomorphism.");
     if (!innP.isSymmetric()) CPPUNIT_FAIL("Standard inner product"
            " isn't symmetric.");
     if (innP.zFormSignature() != dim) 
      CPPUNIT_FAIL("Standard inner product doesn't have full signature.");
    }
   }; // end constructorsTest

void symmetry_test() {// TODO
       }

void composition_test() { // TODO
    };

}; // end BilinearFormTest

void addBilinearForm(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(BilinearFormTest::suite());
}

