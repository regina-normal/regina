
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/*! \file utilities/mutex.h
 *  \brief Provides mutexes that can be enabled or disabled at compile time.
 */

#ifndef __MUTEX_H
#ifndef __DOXYGEN
#define __MUTEX_H
#endif

#include "regina-core.h"
#include "regina-config.h"
#include <mutex>

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A mutual exclusion device (mutex) used to ensure that different
 * threads do not interfere when working with the same data.
 *
 * The template argument allows multithreading to be enabled or disabled
 * at compile time.  If \a threadingSupport is \c true, then this class
 * behaves correctly as a mutex.  If \a threadingSupport is \c false,
 * then this class offers the same mutex API but does nothing.
 *
 * If your code will always be multithreaded then you should just use the
 * C++ standard threading classes (std::mutex and std::lock_guard).
 * If you need both single-threaded and multithreaded variants of your code,
 * then this Mutex template offers a clean way to support mutexes in a way
 * that avoids tedious if/else blocks and that generates very little overhead
 * for the single-threaded variant.
 *
 * See the documentation for Mutex<true> for further information on how
 * this class should be used.
 *
 * This class is designed to work in conjunction with the standard C++11
 * threading classes (in particular, std::thread).  If you do not have these
 * standard classes available (i.e., if your compiler does not implement
 * enough of the C++11 standard), then you can still use Mutex<false>,
 * but any attempt to use Mutex<true> will cause a compile failure.
 *
 * \tparam threadingSupport \c true if this class should provide full
 * mutex support, or \c false if this class should do nothing.
 */
template <bool threadingSupport>
class REGINA_API Mutex;

/**
 * A mutual exclusion device (mutex) used to ensure that different
 * threads do not interfere when working with the same data.
 *
 * A mutex can be either locked or unlocked, and can only be locked by
 * one thread at a time.  If a second thread tries to lock the mutex,
 * it will be suspended until the mutex is unlocked by the original
 * locking thread.
 *
 * Locks are not recursive: if a thread locks a mutex and then attempts
 * to lock it a second time, this will cause a deadlock.
 *
 * A mutex is locked by declaring a local variable of type Mutex::Lock,
 * and is unlocked when this Mutex::Lock variable goes out of scope.
 *
 * This class uses the default constructor, which is not explicitly
 * declared here.  Mutexes are always created in the unlocked state.
 *
 * \ifacespython Not present.
 */
template <>
class REGINA_API Mutex<true> {
    private:
        std::mutex mutex_;
            /**< The underlying C++ mutex. */

    public:
        /**
         * A utility class for locking and unlocking a mutex.
         *
         * A mutex is locked by simply declaring a local variable
         * of type Mutex::Lock.  The mutex will be unlocked when
         * this variable goes out of scope.
         */
        class Lock {
            private:
                std::lock_guard<std::mutex> lock_;
                    /**< The underlying C++ lock. */

            public:
                /**
                 * Creates a lock for the given mutex.
                 *
                 * If some other thread has already locked the given
                 * mutex, this thread will be suspended until the mutex
                 * is unlocked by the other thread.
                 * This thread will then lock the mutex itself.
                 *
                 * @param mutex the mutex to be locked by this object.
                 */
                Lock(Mutex<true>& mutex);
        };
};

/**
 * An empty class that provides the Mutex API but does nothing at all.
 * This is (of course) intended for single-threaded code only, and it
 * generates very little overhead.  Multithreaded code should use Mutex<true>
 * instead.
 *
 * This Mutex<false> class can be used even if the C++11 standard
 * threading classes are unavailable.
 *
 * This class uses the default constructor, which is not explicitly
 * declared here.
 *
 * See Mutex<true> for details of how the API should be used.
 */
template <>
class REGINA_API Mutex<false> {
    public:
        class Lock {
            public:
                /**
                 * A do-nothing constructor.  The argument is ignored.
                 */
                Lock(Mutex<false>&);
        };
};

/*@}*/

// Inline functions for Mutex::Lock

inline Mutex<true>::Lock::Lock(Mutex<true>& mutex) : lock_(mutex.mutex_) {
}

inline Mutex<false>::Lock::Lock(Mutex<false>&) {
}

} // namespace regina

#endif

