
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

#ifndef __NPRIMES_H
#ifndef __DOXYGEN
#define __NPRIMES_H
#endif

/*! \file nprimes.h
 *  \brief Support for finding primes and factorising integers.
 */

#include "maths/nmpi.h"
#include <vector>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

class NPrimes {
    public:
        static NPrimes list;

    private:
        static const unsigned long numPrimeSeeds;
        static const unsigned long primeSeedList[];

        std::vector<NLargeInteger> largePrimes;

    public:
        unsigned long size() const;

        NLargeInteger prime(unsigned long which, bool autoGrow = true);
        NLargeInteger operator [] (unsigned long which);

        /**
         * Returns the prime factorisation of the given integer as a list
         * of individual primes.
         *
         * Prime factors are returned in increasing order.  Where
         * a prime power appears in the factorisation, the relevant
         * prime will appear several times in the list.
         *
         * The given integer may be negative, in which case -1 will be
         * listed as the first factor (even though -1 is not prime).
         * If 0 is passed then a single factor of 0 will be returned;
         * if 1 is passed then an empty list will be returned.
         *
         * For example, the prime factors of 54 will be listed as
         * (2, 3, 3, 3), and the prime factors of -90 will be listed as
         * (-1, 2, 3, 3, 5).
         *
         * @author Ryan Budney
         */
        std::vector<NLargeInteger> primeFactors(const NLargeInteger& n) const;

        /**
         * Returns the prime factorisation of the given integer as a
         * list of prime powers.
         *
         * Factors are returned as (prime, exponent) pairs.  Different
         * pairs describe different primes, and the pairs are sorted
         * in order from smallest prime to largest.  All exponents are
         * strictly positive.
         *
         * The given integer may be negative, in which case (-1,1) will
         * be listed as the first prime power (even though -1 is not prime).
         * If 0 is passed then a single pair (0,1) will be returned;
         * if 1 is passed then an empty list will be returned.
         *
         * For example, the factorisation of 54 will be reported as
         * [(2,1) (3,3)], and the factorisation of -90 will be reported
         * as [(-1,1) (2,1) (3,2) (5,1)].
         *
         * Note that this routine calls primeFactors().
         *
         * @author Ryan Budney
         */
        std::vector<std::pair<NLargeInteger, unsigned long> >
            primePowerDecomp(const NLargeInteger& n) const;

    private:
        /**
         * Private constructor.  Only a single instance of this class
         * is allowed (namely the static instance NPrimes::list).
         */
        NPrimes();

        /**
         * Increases the list of known primes by the given amount.
         *
         * @author Ryan Budney
         */
        void growPrimeList(long extras = 1);
};

/*@}*/

// Inline functions for NPrimes

inline NPrimes::NPrimes() {
}

inline unsigned long NPrimes::size() const {
    return numPrimeSeeds + largePrimes.size();
}

inline NLargeInteger NPrimes::prime(unsigned long which, bool autoGrow) {
    if (which < numPrimeSeeds)
        return primeSeedList[which];

    if (numPrimeSeeds + largePrimes.size() <= which)
        growPrimeList(which - numPrimeSeeds - largePrimes.size() + 1);

    return largePrimes[which - numPrimeSeeds];
}

inline NLargeInteger NPrimes::operator [] (unsigned long which) {
    return prime(which, true);
}

} // namespace regina

#endif

