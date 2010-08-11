
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
#include "algebra/nmvpolynomialring.h"
#include "maths/nsparsegrid.h"
#include "maths/nlargeinteger.h"
#include "testsuite/utilities/testutilities.h"

using regina::NLargeInteger;
using regina::NMVPolynomialRing;
using regina::NMultiIndex;

class NMVPolynomialRingTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NMVPolynomialRingTest);

    CPPUNIT_TEST(additive_struc_test);
    CPPUNIT_TEST(multiplicative_struc_test);
    CPPUNIT_TEST(ring_struc_test);
    //CPPUNIT_TEST(degree_and_width_test);
    //CPPUNIT_TEST(bogus_terms);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {}
        void tearDown() {}

	void additive_struc_test() {
	  for (unsigned long k=0; k<20; k++)
	   {
	   NMVPolynomialRing< NLargeInteger > p1, p2, p3;
	   for (unsigned long i=0; i<10; i++)
	    {
                NMultiIndex< signed long > I(2); 
                 I[0] = NLargeInteger::randomBinary(4).longValue() - 8;
                 I[1] = NLargeInteger::randomBinary(4).longValue() - 8;
		p1 += NMVPolynomialRing< NLargeInteger >( NLargeInteger::randomBinary(7)-64, I );
                 I[0] = NLargeInteger::randomBinary(4).longValue() - 8;
                 I[1] = NLargeInteger::randomBinary(4).longValue() - 8;
		p2 += NMVPolynomialRing< NLargeInteger >( NLargeInteger::randomBinary(4)-8, I );
                 I[0] = NLargeInteger::randomBinary(4).longValue() - 8;
                 I[1] = NLargeInteger::randomBinary(4).longValue() - 8;
		p3 += NMVPolynomialRing< NLargeInteger >( NLargeInteger::randomBinary(3)-4, I );
	    }
	   if ( (p1+p2)+p3 != p1+(p2+p3) ) CPPUNIT_FAIL("Addition not associative.");
	   if ( p1 + NMVPolynomialRing< NLargeInteger >::zero != p1 ) CPPUNIT_FAIL("Zero does not exist (1).");
	   if ( NMVPolynomialRing< NLargeInteger >::zero + p1 != p1 ) CPPUNIT_FAIL("Zero does not exist (2).");
	   if ( (p1 - p1) != NMVPolynomialRing< NLargeInteger >::zero ) CPPUNIT_FAIL("No additive inverse.");
	   if ( p1 + p2 != p2 + p1 ) CPPUNIT_FAIL("Addition not commutative.");
	   }
	 // + is assoc, id, inverses
	}
	void multiplicative_struc_test() {
	  for (unsigned long k=0; k<20; k++)
	   {
	   NMVPolynomialRing< NLargeInteger > p1, p2, p3;
	   for (unsigned long i=0; i<10; i++)
	    {
                NMultiIndex< signed long > I(2); 
                 I[0] = NLargeInteger::randomBinary(4).longValue() - 8;
                 I[1] = NLargeInteger::randomBinary(4).longValue() - 8;
		p1 += NMVPolynomialRing< NLargeInteger >( NLargeInteger::randomBinary(7)-64, I );
                 I[0] = NLargeInteger::randomBinary(4).longValue() - 8;
                 I[1] = NLargeInteger::randomBinary(4).longValue() - 8;
		p2 += NMVPolynomialRing< NLargeInteger >( NLargeInteger::randomBinary(4)-8, I );
                 I[0] = NLargeInteger::randomBinary(4).longValue() - 8;
                 I[1] = NLargeInteger::randomBinary(4).longValue() - 8;
		p3 += NMVPolynomialRing< NLargeInteger >( NLargeInteger::randomBinary(3)-4, I );
	    }
	   if ( (p1*p2)*p3 != p1*(p2*p3) ) CPPUNIT_FAIL("Multiplication not associative.");
           NMVPolynomialRing< NLargeInteger > mvOne ( NLargeInteger::one, NMultiIndex< signed long >(0,0) ); 
	   if ( p1 * mvOne != p1 ) CPPUNIT_FAIL("One does not exist (1).");
	   if ( mvOne * p1 != p1 ) CPPUNIT_FAIL("One does not exist (2).");
	   if ( p1 * p2 != p2 * p1 ) CPPUNIT_FAIL("Multiplication not commutative.");
	   }
	 // * is assoc, id
	}
	void ring_struc_test() {
	  for (unsigned long k=0; k<20; k++)
	   {
	   NMVPolynomialRing< NLargeInteger > p1, p2, p3;
	   for (unsigned long i=0; i<10; i++)
	    {
                NMultiIndex< signed long > I(2); 
                 I[0] = NLargeInteger::randomBinary(4).longValue() - 8;
                 I[1] = NLargeInteger::randomBinary(4).longValue() - 8;
		p1 += NMVPolynomialRing< NLargeInteger >( NLargeInteger::randomBinary(7)-64, I );
                 I[0] = NLargeInteger::randomBinary(4).longValue() - 8;
                 I[1] = NLargeInteger::randomBinary(4).longValue() - 8;
		p2 += NMVPolynomialRing< NLargeInteger >( NLargeInteger::randomBinary(4)-8, I );
                 I[0] = NLargeInteger::randomBinary(4).longValue() - 8;
                 I[1] = NLargeInteger::randomBinary(4).longValue() - 8;
		p3 += NMVPolynomialRing< NLargeInteger >( NLargeInteger::randomBinary(3)-4, I );
	    }
	   if ( (p1+p2)*p3 != (p1*p3) + (p2*p3) ) CPPUNIT_FAIL("Multiplication and addition do not distribute (1).");
	   if ( p1*(p2+p3) != (p1*p2) + (p1*p3) ) CPPUNIT_FAIL("Multiplication and addition do not distribute (2).");
	   }
	 // distrib
	}
};

void addNMVPolynomialRing(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NMVPolynomialRingTest::suite());
}

