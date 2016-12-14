
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "maths/primes.h"

namespace regina {

std::vector<Integer> Primes::largePrimes;
std::mutex Primes::largeMutex;

Integer Primes::prime(unsigned long which, bool autoGrow) {
    // Can we grab it straight out of the hard-coded seed list?
    if (which < numPrimeSeeds)
        return primeSeedList[which];

    // From here we need to ensure thread safety.
    std::lock_guard<std::mutex> lock(largeMutex);

    // Do we even have the requested prime stored?
    if (which >= numPrimeSeeds + largePrimes.size()) {
        if (autoGrow)
            growPrimeList(which - numPrimeSeeds - largePrimes.size() + 1);
        else
            return Integer::zero;
    }

    // Got it.
    return largePrimes[which - numPrimeSeeds];
}

void Primes::growPrimeList(unsigned long extras) {
    Integer lastPrime = (largePrimes.empty() ?
        primeSeedList[numPrimeSeeds - 1] :
        largePrimes[largePrimes.size() - 1]);
    Integer newPrime;

    // Since this is all being done through GMP, just bite the bullet
    // and make them all GMP integers (not native integers).
    // This means we can call rawData() with abandon.
    while (extras) {
        mpz_nextprime(newPrime.rawData(), lastPrime.rawData());
        newPrime.tryReduce(); // since rawData() forced it into GMP format
        largePrimes.push_back(newPrime);

        lastPrime = newPrime;
        extras--;
    }
}

std::vector<Integer> Primes::primeDecomp(const Integer& n) {
    std::vector<Integer> retval;

    // Deal with n=0 first.
    if (n == Integer::zero) {
        retval.push_back(Integer::zero);
        return retval;
    }

    Integer temp(n);
    Integer r,q;

    // if the number is negative, put -1 as first factor.
    if (temp < Integer::zero) {
        temp.negate();
        retval.push_back(Integer(-1));
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

    while ( temp != Integer::one ) {
        // now cpi<size(), check to see if temp % prime(cpi) == 0
        q = temp.divisionAlg(prime(cpi), r); // means temp = q*prime(cpi) + r
        if (r == Integer::zero) {
            temp=q;
            retval.push_back(prime(cpi));
            iterSinceDivision=0;
            continue;
        }

        cpi++;
        iterSinceDivision++;

        // after 500 unsuccessful divisions,
        // check to see if it is probably prime.
        if (iterSinceDivision == 500) {
            int res = mpz_probab_prime_p (temp.rawData(), 10);
            // Calling rawData() made temp fat (GMP); try to make it
            // native again if we can (for performance).
            temp.tryReduce();

            if (res) {
                // temp is likely prime.
                // end the search.
                retval.push_back(temp);
                break;
            }
        }
    }

    return retval; // now it's reasonably fast for small numbers.
               // it tends to bog down on numbers with two or more large
               // prime factors.  the GAP algorithm is better, whatever
               // that is... should consider importing it.
}

std::vector<std::pair<Integer, unsigned long> >
        Primes::primePowerDecomp(const Integer& n) {
    std::vector<Integer> list1(primeDecomp(n));
    std::vector< std::pair<Integer, unsigned long> > retlist;

    // go through list1, record number of each prime, put in retlist.
    if (! list1.empty()) {
        Integer cp(list1.front()); // current prime
        unsigned long cc(1); // current count
        std::vector<Integer>::const_iterator it = list1.begin();
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

