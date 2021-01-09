
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file utilities/randutils.h
 *  \brief Provides threadsafe random number generation.
 */

#ifndef __RANDUTILS_H
#ifndef __DOXYGEN
#define __RANDUTILS_H
#endif

#include <mutex>
#include <random>
#include <type_traits>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * Offers threadsafe access to Regina's global uniform random bit generator.
 *
 * Regina has at its heart a global uniform random bit generator (URBG) that
 * complies with the C++ UniformRandomBitGenerator concept, and can be used
 * with other data types and algorithms from the standard \c random header.
 *
 * An object of this class offers threadsafe access to this global URBG.
 *
 * This class behaves in many ways like std::lock_guard.  On construction a
 * RandomEngine object will lock an internal mutex that grants exclusive
 * access to the global URBG, and on destruction the RandomEngine object will
 * unlock this mutex.
 *
 * The engine() function grants direct access to the URBG.  To ensure
 * thread safety, you should only use this URBG during the scope of the
 * RandomEngine object (i.e., while the internal mutex is locked).
 * The easiest way to do this is to never store a reference to engine() for
 * later use, but instead to pass engine() directly to other random number
 * generation functions as you use them.
 *
 * The engine is seeded with a default value.  This means that each time
 * you run the same code in a new process (but on the same machine), you
 * should receive the same sequence of random bits.  However, the generation
 * algorithm may be specific to your machine, so running the same code on
 * different machines might well \e not generate the same random bits.
 *
 * If you need to re-seed the random engine with a value that is
 * unpredictable (e.g., using hardware entropy), you can call
 * reseedWithHardware().  If you wish to re-seed the random engine with
 * its default value (to behave as though the process had just started),
 * you can call reseedWithDefault().
 *
 * \ifacespython Python users only have access to the static member
 * functions in this class (which still supports basic random number
 * generation as well as reseeding).
 */
class REGINA_API RandomEngine : std::lock_guard<std::mutex> {
    private:
        static std::default_random_engine engine_;
            /**< Regina's global uniform random bit generator. */
        static std::mutex mutex_;
            /**< The mutex that protects \a engine_. */

    public:
        /**
         * Constructor that locks the internal mutex.
         * The mutex will be unlocked when this object is destroyed.
         *
         * \ifacespython Not present.
         */
        RandomEngine();

        /**
         * Provides access to Regina's global uniform random bit generator
         * (URBG).
         *
         * As described in the class notes, you should pass engine()
         * directly to other random number generation functions as you
         * use them, and not store a reference to it for later use.
         *
         * \ifacespython Not present.
         *
         * @return a reference to the global URBG.
         */
        std::default_random_engine& engine();

        /**
         * A convenience function that returns a random integer modulo
         * \a range, in a thread-safe manner.  The result will be
         * between 0 and (\a range - 1) inclusive, and all such integers
         * should be returned with equal probability.
         *
         * This is expensive because it locks and unlocks the internal mutex.
         * If you need to create many random numbers in quick succession,
         * consider creating a single RandomEngine object and accessing
         * engine() directly using the standard C++ randomness functions.
         *
         * \ifacespython The integer type \a Int will be treated as \c long.
         *
         * \tparam Int a native integer type (e.g., \c int, \c long,
         * \c size_t, <tt>long long</tt>, etc.); this may be either
         * signed or unsigned.
         *
         * @param range the size of the range of possible results;
         * this must be strictly positive.
         * @return a random integer between 0 and (\a range - 1) inclusive.
         */
        template <typename Int>
        static Int rand(Int range);

        /**
         * Reseeds the global uniform random bit generator using
         * hardware entropy.  This should make the sequence of random
         * bits unpredictable (and, in particular, different between
         * runs of your program).
         *
         * Regina does \e not do this by default.
         *
         * This routine is thread-safe, and it locks the internal mutex
         * while it runs.
         */
        static void reseedWithHardware();

        /**
         * Reseeds the global uniform random bit generator using the
         * default seed.  This should make the sequence of random bits
         * completely predictable between runs of the same program on
         * the same machine.  (Note that even when using the same seed,
         * different operating systems, architectures and/or compilers
         * may produce different random bits).
         *
         * Regina seeds the global uniform random bit generator in this
         * way on startup, and so typically you would not need to call
         * this routine yourself.
         *
         * This routine is thread-safe, and it locks the internal mutex
         * while it runs.
         */
        static void reseedWithDefault();
};

/*@}*/

// Inline functions for RandomEngine

inline RandomEngine::RandomEngine() : std::lock_guard<std::mutex>(mutex_) {
}

inline std::default_random_engine& RandomEngine::engine() {
    return engine_;
}

template <typename Int>
Int RandomEngine::rand(Int range) {
    // A slight messiness here is that std::uniform_int_distribution
    // requires the type argument to be one of short, int, long or long long
    // (either signed or unsigned).
    static_assert(std::is_integral<Int>::value,
        "RandomEngine::rand() requires a native integer type");
    static_assert(sizeof(Int) <= sizeof(long long),
        "RandomEngine::rand() requires a type that can fit inside a long long");

    typedef typename std::conditional<std::is_signed<Int>::value,
        typename std::conditional<sizeof(Int) <= sizeof(short), short,
        typename std::conditional<sizeof(Int) <= sizeof(int), int,
        typename std::conditional<sizeof(Int) <= sizeof(long), long,
        long long>::type>::type>::type,
        typename std::conditional<sizeof(Int) <= sizeof(short), unsigned short,
        typename std::conditional<sizeof(Int) <= sizeof(int), unsigned int,
        typename std::conditional<sizeof(Int) <= sizeof(long), unsigned long,
        unsigned long long>::type>::type>::type>::type Arg;

    std::uniform_int_distribution<Arg> d(0, range - 1);
    std::lock_guard<std::mutex> lock(mutex_);
    return d(engine_);
}

} // namespace regina

#endif

