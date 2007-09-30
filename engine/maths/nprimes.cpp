
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "maths/nprimes.h"

namespace regina {

std::vector<NLargeInteger> NPrimes::largePrimes;

NLargeInteger NPrimes::prime(unsigned long which, bool autoGrow) {
    // Can we grab it straight out of the hard-coded seed list?
    if (which < numPrimeSeeds)
        return primeSeedList[which];

    // Do we even have the requested prime stored?
    if (which >= numPrimeSeeds + largePrimes.size()) {
        if (autoGrow)
            growPrimeList(which - numPrimeSeeds - largePrimes.size() + 1);
        else
            return NLargeInteger::zero;
    }

    // Got it.
    return largePrimes[which - numPrimeSeeds];
}

void NPrimes::growPrimeList(unsigned long extras) {
    NLargeInteger lastPrime = (largePrimes.empty() ?
        primeSeedList[numPrimeSeeds - 1] :
        largePrimes[largePrimes.size() - 1]);
    NLargeInteger newPrime;

    while (extras) {
        mpz_nextprime(newPrime.data, lastPrime.data);
        largePrimes.push_back(newPrime);

        lastPrime = newPrime;
        extras--;
    }
}

std::vector<NLargeInteger> NPrimes::primeDecomp(const NLargeInteger& n) {
    std::vector<NLargeInteger> retval;

    // Deal with n=0 first.
    if (n == NLargeInteger::zero) {
        retval.push_back(NLargeInteger::zero);
        return retval;
    }

    NLargeInteger temp(n);
    NLargeInteger r,q;

    // if the number is negative, put -1 as first factor.
    if (temp < NLargeInteger::zero) {
        temp.negate();
        retval.push_back(NLargeInteger(-1));
    }

    // repeatedly divide the number by the smallest primes until no
    // longer divisible.
    // at present the algorithm is only guaranteed to factorize the integer
    // into its prime factors if none of them are larger than the 500th smallest
    // prime.  it always produces a factorization, but after the 500th it uses
    // a probabilistic test to speed things up. This algorithm is at present
    // ad-hoc since the current usage in Regina rarely demands the
    // factorization of even a 4-digit number.

    unsigned long cpi=0; // current prime index.
    unsigned long iterSinceDivision=0; // keeps track of how many iterations
                                       // since the last successful division

    while ( temp != NLargeInteger::one ) {
        // now cpi<size(), check to see if temp % prime(cpi) == 0
        q = temp.divisionAlg(prime(cpi), r); // means temp = q*prime(cpi) + r
        if (r == NLargeInteger::zero) {
            temp=q;
            retval.push_back(prime(cpi));
            iterSinceDivision=0;
            continue;
        }

        cpi++;
        iterSinceDivision++;
        if (iterSinceDivision == 500) // after 500 unsuccessful divisions,
                                      // check to see if it is probably prime.
            if (mpz_probab_prime_p (temp.data, 10) != 0) {
                // temp is likely prime.
                // end the search.
                retval.push_back(temp);
                break;
            }
    }

    return retval; // now it's reasonably fast for small numbers.
               // it tends to bog down on numbers with two or more large
               // prime factors.  the GAP algorithm is better, whatever
               // that is... should consider importing it.
}

std::vector<std::pair<NLargeInteger, unsigned long> >
        NPrimes::primePowerDecomp(const NLargeInteger& n) {
    std::vector<NLargeInteger> list1(primeDecomp(n));
    std::vector< std::pair<NLargeInteger, unsigned long> > retlist;

    // go through list1, record number of each prime, put in retlist.
    if (! list1.empty()) {
        NLargeInteger cp(list1.front()); // current prime
        unsigned long cc(1); // current count
        std::vector<NLargeInteger>::const_iterator it = list1.begin();
        for (++it; it != list1.end(); ++it) {
            if (*it == cp)
                cc++;
            else {
                // a new prime is coming up.
                retlist.push_back(std::make_pair( cp, cc ) );
                cp = *it;
                cc = 1;
            }
        }
        retlist.push_back(std::make_pair( cp, cc ) );
    }

    return retlist;
}

} // namespace regina

