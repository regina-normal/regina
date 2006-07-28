
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

#include <cstdlib>
#include "utilities/nmpi.h"

namespace regina {

const NLargeInteger NLargeInteger::zero;
const NLargeInteger NLargeInteger::one(1);
const NLargeInteger NLargeInteger::infinity(true, true);
std::vector<NLargeInteger> NLargeInteger::primeList;

std::string NLargeInteger::stringValue(int base) const {
    if (infinite)
        return "inf";
    else {
        char* str = mpz_get_str(0, base, data);
        std::string ans(str);
        free(str);
        return ans;
    }
}

NLargeInteger NLargeInteger::gcdWithCoeffs(const NLargeInteger& other,
        NLargeInteger& u, NLargeInteger& v) const {
    NLargeInteger ans;

    // Check for zero coefficients.
    if ((*this) == 0) {
        u = 0L;
        if (other == 0) {
            v = 0L;
            // ans is already zero.
            return ans;
        }
        v = 1;
        ans = other;
        if (ans < 0) {
            v.negate();
            ans.negate();
        }
        return ans;
    }
    if (other == 0) {
        v = 0L;
        u = 1;
        ans = *this;
        if (ans < 0) {
            u.negate();
            ans.negate();
        }
        return ans;
    }
    
    // Neither argument is zero.
    // Run the gcd algorithm.
    mpz_gcdext(ans.data, u.data, v.data, data, other.data);

    // Ensure the gcd is positive.
    if (ans < 0) {
        ans.negate();
        u.negate();
        v.negate();
    }

    // Get u and v in the correct range.
    NLargeInteger addToU(other);
    NLargeInteger addToV(*this);
    addToU.divByExact(ans);
    addToV.divByExact(ans);
    if (addToV < 0)
        addToV.negate();
    else
        addToU.negate();

    // We can add (addToU, addToV) to u and v.
    // We also know that addToV is positive.

    // Add enough copies to make v*sign(other) just non-positive.
    NLargeInteger copies(v);
    if (other > 0) {
        // v must be just non-positive.
        if (v > 0) {
            copies -= 1;
            copies /= addToV;
            copies.negate();
            copies -= 1;
        } else {
            copies /= addToV;
            copies.negate();
        }
    }
    else {
        // v must be just non-negative.
        if (v < 0) {
            copies += 1;
            copies /= addToV;
            copies.negate();
            copies += 1;

        } else {
            copies /= addToV;
            copies.negate();
        }
    }
    addToU *= copies;
    addToV *= copies;
    u += addToU;
    v += addToV;
    return ans;
}

std::ostream& operator << (std::ostream& out, const NLargeInteger& large) {
    if (large.infinite)
        out << "inf";
    else {
        char* str = mpz_get_str(0, 10, large.data);
        out << str;
        free(str);
    }
    return out;
}

NLargeInteger NLargeInteger::euclideanAlg(const NLargeInteger& d, 
		NLargeInteger& r) const
{
	NLargeInteger q;
	if (infinite) 
		{
		q = infinite;
		r = zero;
		} else
	if (d.infinite)
		{
		q = zero;
		r = (*this);
		} else
	{
	mpz_fdiv_qr( q.data, r.data, data, d.data ); 
	} // q r n d is the format.
return q;
}

void NLargeInteger::setInitialPrimes()
{
if (primeList.size() == 0)
 {
 primeList.reserve(numPrimeSeeds);
 for (long i=0; i<numPrimeSeeds; i++)
   primeList.push_back(NLargeInteger(primeSeedList[i]));
 }
}

std::vector<NLargeInteger> NLargeInteger::getPrimeList()
{
return primeList;
}

void NLargeInteger::growPrimeList()
{ // uses: void mpz_nextprime (mpz_t rop, mpz_t op)
  // rop input will be primeList last element
  if (primeList.size()==0) setInitialPrimes();
   else
   {
   NLargeInteger nextP;
   mpz_nextprime( nextP.data,  primeList[primeList.size()-1].data );
   primeList.push_back(nextP);
   }
}

std::vector<NLargeInteger> NLargeInteger::primeFactors()
{
std::vector<NLargeInteger> retval;
NLargeInteger temp=(*this);
NLargeInteger r,q;

if (primeList.size()==0) setInitialPrimes();

// if the number is negative, put -1 as first prime factor.
if (temp < zero) { temp=temp.abs(); retval.push_back(NLargeInteger(-1)); }

// repeatedly divide the number by the smallest primes until no
// longer divisible. 
// at present the algorithm is only guaranteed to factorize the integer
// into its prime factors if none of them are larger than the 500th largest
// prime.  it always produces a factorization, but after the 500th it uses
// a probabilistic test to speed things up. This algorithm is at present ad-hoc
// since the current usage in Regina rarely demands the factorization of even
// a 4-digit number. 

unsigned long cpi=0; // current prime index.
unsigned long iterSinceDivision=0; // keeps track of how many iterations since the 
	// last successful division

if (temp > zero) while ( temp != one )
	{
	// test to see if cpi < primeList.size(), if not, lengthen list, return to loop.
	if (cpi >= primeList.size()) { growPrimeList(); continue; }
	// now cpi<primeList.size(), check to see if temp % primeList[cpi] == 0
	q = temp.euclideanAlg(primeList[cpi],r); // means temp = q*primeList[cpi] + r
	if (r == zero) { temp=q; retval.push_back(primeList[cpi]); iterSinceDivision=0; } else 
		{ cpi++; iterSinceDivision++; }
	if (iterSinceDivision == 500) // after 500 unsuccessful divisions, check to see if it
				      // is probably prime.
	   if (mpz_probab_prime_p (temp.data, 10) != 0)
	    { // temp is likely prime.	
	      // end the search.
	     retval.push_back(temp);
	     temp=one;
	    }
	}
return retval; // now it's reasonably fast for small numbers.
	       // it tends to bog down on numbers with two or more large prime factors.
	       // the GAP algorithm is better, whatever that is... should consider
	       // importing it. 
}

std::vector< std::pair<NLargeInteger, unsigned long> > NLargeInteger::primePowerDecomp()
{
std::vector<NLargeInteger> list1(primeFactors());
std::vector< std::pair<NLargeInteger, unsigned long> > retlist;
// go through list1, record number of each prime, put in retlist.
if (list1.size()>0)
 {
 NLargeInteger cp(list1[0]); // current prime
 unsigned long cc(1); // current count
 for (unsigned long i=1; i<list1.size(); i++)
	{
	if (list1[i]==cp) cc++; else
	 { // a new prime is coming up.
	  retlist.push_back(std::make_pair( cp, cc ) );
	  cp=list1[i];
	  cc=1;
	 }
	}
  retlist.push_back(std::make_pair( cp, cc ) );
 }
else
 {
 retlist.push_back(std::make_pair( *this, 1 ) );
 }

return retlist;
}



} // namespace regina

