
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

#include "utilities/nmpi.h"
#include <vector>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * A helper class for finding primes and factorising integers.
 *
 * This class has two functions: (i) to maintain a list of known primes,
 * and (ii) to use this list to factorise integers into prime factors.
 *
 * The primes stored by this class will always be the smallest \a k
 * suspected primes, where \a k may grow dynamically as the program runs.
 * Specifically:
 *
 * - An initial hard-coded list of seed primes is loaded into the class on
 *   startup.  This list contains precisely the smallest 10,000 primes (the
 *   size of this list is subject to change in future versions of Regina).
 *
 * - Whenever a prime beyond the known list is requested (e.g., when a
 *   number greater than the largest stored prime is to be factorised), the
 *   list is extended on the fly.  The extension uses the probabilistic
 *   algorithm shipped with GMP (hence the phrase "suspected primes" above);
 *   regarding this algorithm, the GMP documentation states that "for
 *   practical purposes it's adequate, the chance of a composite passing
 *   will be extremely small."
 *
 * This list is used by the high-level factorisation routines in this
 * class, such as primeDecomp() and primePowerDecomp().  For users only
 * interested in these high-level routines, there is no need to worry
 * about the size of the list; the high-level routines will extend it if
 * necessary.
 *
 * @author Ryan Budney, B.B.
 */
class NPrimes {
    private:
        static const unsigned long numPrimeSeeds;
            /**< The size of the hard-coded list of seed primes. */
        static const unsigned long primeSeedList[];
            /**< The full hard-coded list of seed primes. */

        static std::vector<NLargeInteger> largePrimes;
            /**< Primes (or suspected primes) that have been found thus
                 far, not including the initial seed primes.  This list
                 begins empty, and is expanded as required throughout the
                 life of the program. */

    public:
        /**
         * Returns the number of primes (or suspected primes) currently
         * stored.
         *
         * Primes that are already stored can be accessed instantly;
         * primes larger than those currently stored must be generated
         * on the fly (which takes time).
         *
         * This number may increase as the program runs (according to
         * whether larger primes are requested), but it will never
         * decrease.
         *
         * @return the number of primes or suspected primes currently stored.
         */
        static unsigned long size();

        /**
         * Returns the requested prime (or suspected prime).  More
         * specifically, this routine returns the (\a which + 1)th
         * smallest prime.  Thus prime(0) returns 2, prime(1) returns 3,
         * prime(2) returns 5, and so on.
         *
         * If \a which is smaller than the number of initial seed primes,
         * the result is guaranteed to be the (\a which + 1)th smallest
         * prime (see the NPrimes class notes for the size of the initial
         * seed list).  If \a which is larger, a probabilistic algorithm
         * is used and so there is a possibility that non-primes are
         * included in the list.
         *
         * If \a which < size() then this routine is essentially
         * instantaneous, since the (\a which + 1)th smallest (suspected)
         * prime is already stored.  Otherwise the behaviour depends on
         * the argument \a autoGrow.  If \a autoGrow is \c true (the
         * default) then this routine calculates the requested prime,
         * which might take some time.  If \a autoGrow is \c false then
         * this routine returns zero.
         *
         * @param which indicates which prime is requested.
         * @param autoGrow specifies what to do if the requested
         * prime lies beyond the list currently stored (see above).
         * @return the requested prime (or suspected prime), or zero if
         * \a which was too large and \a autoGrow was \c false.
         */
        static NLargeInteger prime(unsigned long which, bool autoGrow = true);

        /**
         * Returns the prime factorisation of the given integer as a list
         * of individual primes (or suspected primes).
         *
         * Prime factors are returned in increasing order.  Where
         * a prime power appears in the factorisation, the relevant
         * prime will appear several times in the list.
         *
         * For very large integers, the factorisation becomes
         * probabilistic: (i) this routine examines suspected primes
         * instead of primes (see the class notes), and (ii) if the
         * routine is having trouble finding factors then it will run a
         * probabilistic prime test on whatever portion of \a n still
         * remains (and will assume that portion to be prime if the test
         * passes).
         *
         * The given integer may be negative, in which case -1 will be
         * listed as the first factor (even though -1 is not prime).
         * If 0 is passed then a single factor of 0 will be returned;
         * if 1 is passed then an empty list will be returned.
         * In all cases, the given integer \a n will be the product of
         * all elements of the final list (where an empty product is
         * assumed to be 1).
         *
         * As an example, the prime factors of 54 will be listed as
         * (2, 3, 3, 3), and the prime factors of -90 will be listed as
         * (-1, 2, 3, 3, 5).
         *
         * Note that the internal list of known primes and suspected
         * primes will be expanded as necessary; there is no need for
         * the caller to manage this list manually.
         *
         * \ifacespython In addition to this routine, the routine
         * primeDecompInt() is also available.  The routine
         * primeDecompInt() behaves identically to this routine except
         * that the (i) return values are of ordinary integer type, not
         * NLargeInteger; (ii) the input value \a n must lie within
         * the C++ long integer range (otherwise the behaviour is undefined).
         *
         * @param n the integer to factorise.
         * @return the list of prime factors as described above.
         */
        static std::vector<NLargeInteger> primeDecomp(const NLargeInteger& n);

        /**
         * Returns the prime factorisation of the given integer as a
         * list of prime powers (or suspected prime powers).
         *
         * Factors are returned as (prime, exponent) pairs.  Different
         * pairs describe different primes, and the pairs are sorted
         * in order from smallest prime to largest.  All exponents are
         * strictly positive.
         *
         * For very large integers, the factorisation becomes
         * probabilistic: (i) this routine examines suspected primes
         * instead of primes (see the class notes), and (ii) if the
         * routine is having trouble finding factors then it will run a
         * probabilistic prime test on whatever portion of \a n still
         * remains (and will assume that portion to be prime if the test
         * passes).
         *
         * The given integer may be negative, in which case (-1,1) will
         * be listed as the first prime power (even though -1 is not prime).
         * If 0 is passed then a single pair (0,1) will be returned;
         * if 1 is passed then an empty list will be returned.
         * In all cases, the given integer \a n will be the product of
         * all powers described by the final list (where an empty product
         * is assumed to be 1).
         *
         * As an example, the factorisation of 54 will be reported as
         * [(2,1) (3,3)], and the factorisation of -90 will be reported
         * as [(-1,1) (2,1) (3,2) (5,1)].
         *
         * Note that the internal list of known primes and suspected
         * primes will be expanded as necessary; there is no need for
         * the caller to manage this list manually.
         *
         * The current implementation of this routine merely calls
         * primeDecomp() and rewrites the list of factors by grouping primes.
         *
         * \todo Implement this routine natively to avoid the overhead
         * of the temporary primeDecomp() vector.
         *
         * \ifacespython In addition to this routine, the routine
         * primePowerDecompInt() is also available.  The routine
         * primePowerDecompInt() behaves identically to this routine except
         * that the (i) return values are of ordinary integer type, not
         * NLargeInteger; (ii) the input value \a n must lie within
         * the C++ long integer range (otherwise the behaviour is undefined).
         *
         * @param n the integer to factorise.
         * @return the list of prime power factors as described above.
         */
        static std::vector<std::pair<NLargeInteger, unsigned long> >
            primePowerDecomp(const NLargeInteger& n);

    private:
        /**
         * Private constructor.  No instance of this class is allowed,
         * since everything of interest is static.
         */
        NPrimes();

        /**
         * Adds the given number of primes (or suspected primes) to the
         * list already stored.
         *
         * @param extras the number of additional suspected primes to
         * calculate.
         */
        static void growPrimeList(unsigned long extras = 1);
};

/*@}*/

// Inline functions for NPrimes

inline NPrimes::NPrimes() {
}

inline unsigned long NPrimes::size() {
    return numPrimeSeeds + largePrimes.size();
}

} // namespace regina

#endif

