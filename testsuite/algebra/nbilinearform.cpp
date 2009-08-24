
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/nsparsegrid.h"
#include "maths/nlargeinteger.h"
#include "algebra/nbilinearform.h";
#include "testsuite/utilities/testutilities.h"

using regina::NLargeInteger;

class NBilinearFormTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NBilinearFormTest);

    CPPUNIT_TEST(constructors_test);
    CPPUNIT_TEST(symmetry_test);
    CPPUNIT_TEST(composition_test);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
  	 }

        void tearDown() {}

	void constructors_test() {
	 // let's check that the standard inner product on R^n is an iso between R^n and its dual for all n == 1,2,3,...,10, say.
         for (unsigned long dim=1; dim<11; dim++)
	  {
	  regina::NMarkedAbelianGroup ldom( dim, regina::NLargeInteger::zero );
	  regina::NMarkedAbelianGroup rdom( dim, regina::NLargeInteger::zero );
	  regina::NMarkedAbelianGroup zed( 1, regina::NLargeInteger::zero );
	  regina::NSparseGrid< regina::NLargeInteger > pairing(3);
          for (unsigned long i=0; i<dim; i++)
           {
            regina::NMultiIndex I(3);
	    I[0]=i; I[1]=i; I[2]=0;
	    pairing.incEntry( I, regina::NLargeInteger::one );
           }
 	  regina::NBilinearForm innP(ldom, rdom, zed, pairing);
	  if (!innP.leftAdjoint().isIsomorphism()) CPPUNIT_FAIL("Left-adjoint to standard inner product on R^n is not isomorphism.");
	  if (!innP.rightAdjoint().isIsomorphism()) CPPUNIT_FAIL("Right-adjoint to standard inner product on R^n is not isomorphism.");
          if (!innP.isSymmetric()) CPPUNIT_FAIL("Standard inner product isn't symmetric.");
          if (innP.signature() != dim) CPPUNIT_FAIL("Standard inner product doesn't have full signature.");
	  }
	};
	void symmetry_test() {
	// todo
        }
	void composition_test() {
	// todo
	};
};

void addNBilinearForm(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NBilinearFormTest::suite());
}

