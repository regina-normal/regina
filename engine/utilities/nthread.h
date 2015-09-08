
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

/*! \file utilities/nthread.h
 *  \brief Provides very basic thread handling.
 */

#ifndef __NTHREAD_H
#ifndef __DOXYGEN
#define __NTHREAD_H
#endif

#include <pthread.h>
#include <sched.h>
#include "regina-core.h"

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
 * If your code will always be multithreaded then you should use NMutex,
 * which is a typedef for Mutex<true>.  If you need both single-threaded and
 * multithreaded variants of your code, then the Mutex template offers a
 * clean way to support mutexes in a way that avoids tedious if/else blocks
 * and that generates very little overhead for the single-threaded variant.
 *
 * See the documentation for Mutex<true> for further information on how
 * this class is used.
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
 * A mutex is locked by declaring a local variable of type Mutex::MutexLock.
 * See the Mutex::MutexLock class notes for details.
 *
 * Classes can inherit from Mutex to provide mutex protection for
 * their internal data; it is recommended that such inheritance be
 * \c protected and that the member functions alone take full
 * responsibility for locking and unlocking the mutex when appropriate.
 * Alternatively, a standalone Mutex object can be passed about between
 * routines.
 *
 * \ifacespython Not present.
 */
template <>
class REGINA_API Mutex<true> {
    private:
        pthread_mutex_t mutex;
            /**< The C mutex used by internal function calls. */

    public:
        /**
         * Creates a new mutex.
         * The mutex will be created unlocked.
         */
        Mutex();
        /**
         * Destroys this mutex.
         *
         * \pre This mutex is unlocked.
         */
        ~Mutex();

        /**
         * A utility class for locking and unlocking a mutex.
         *
         * A mutex is locked by simply declaring a local variable
         * of type Mutex::MutexLock.  The mutex will be unlocked when
         * this variable goes out of scope.
         */
        class MutexLock {
            private:
                const Mutex* mutex_;
                    /**< The mutex locked by this object. */

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
                 * This is \c const to simplify using mutex locks with
                 * data retrieval routines for subclasses of Mutex.
                 */
                MutexLock(const Mutex* mutex);
                /**
                 * Creates a lock for the given mutex.
                 *
                 * If some other thread has already locked the given
                 * mutex, this thread will be suspended until the mutex
                 * is unlocked by the other thread.
                 * This thread will then lock the mutex itself.
                 *
                 * @param mutex the mutex to be locked by this object.
                 * This is \c const to simplify using mutex locks with
                 * data retrieval routines for subclasses of Mutex.
                 */
                MutexLock(const Mutex& mutex);
                /**
                 * Unlocks the mutex handled by this object.
                 */
                ~MutexLock();
        };

    private:
        /**
         * Locks this mutex.
         * If any thread has already locked this mutex, this thread will
         * be suspended until the mutex is unlocked by the other thread,
         * and will then lock this mutex itself.
         *
         * This routine is \c const to simplify using it in data
         * retrieval routines for subclasses of Mutex.
         *
         * \pre The mutex is not already locked by <i>this</i> thread.
         * Failure to adhere to this precondition will <b>almost certainly
         * deadlock</b> this thread!
         */
        void mutexLock() const;
        /**
         * Unlocks this mutex.
         *
         * This routine is \c const to simplify using it in data
         * retrieval routines for subclasses of Mutex.
         *
         * \pre The mutex is currently locked by this thread.
         */
        void mutexUnlock() const;
};

/**
 * A convenience typedef for Mutex<true>.
 * This is recommended for code that is always multithreaded, and does
 * not require both multithreaded and single-threaded variants.
 */
typedef Mutex<true> NMutex;

/**
 * An empty class that provides the Mutex API but does nothing at all.
 * This is (of course) intended for single-threaded code only, and it
 * generates very little overhead.
 *
 * See Mutex<true> for details of how the API should be used.
 *
 * Multithreaded code should use Mutex<true> instead.
 */
template <>
class REGINA_API Mutex<false> {
    public:
        class MutexLock {
            public:
                /**
                 * A do-nothing constructor.  The argument is ignored.
                 */
                MutexLock(const Mutex*);
                /**
                 * A do-nothing constructor.  The argument is ignored.
                 */
                MutexLock(const Mutex&);
        };
};

typedef pthread_t NThreadID;
    /**< The type used for a thread identifier. */

/**
 * Provides very basic thread handling.
 *
 * Throughout this documentation, "thread object" refers to an instance
 * of an NThread subclass, and otherwise "thread" has its usual meaning
 * as a thread executing code in the underlying operating system.
 *
 * Each subclass of NThread represents a specific task that new
 * threads can perform.  Each subclass should override run() to perform
 * its corresponding task.  The user may then create a thread object
 * and call start() upon it, which will spawn a new thread that executes run().
 *
 * A thread object is called \e running if its corresponding thread is
 * running.  You must not call start() on a thread object that is already
 * running.  Instead, if you wish to run multiple instances of the same task
 * in parallel, you must start these from different thread objects.
 *
 * \warning At the time of writing (admittedly long ago now), Qt has only
 * limited support for multithreading.  When working with an existing packet
 * tree in a new thread, the \e only modification that you may make is to
 * insert new packets.
 * Modifications of any other type (such as changing, renaming, deleting
 * or reordering existing packets) could lead to a crash within Qt or Xlib
 * when running the GUI.
 * Of course, a new thread may create, modify and delete its own temporary
 * packet trees as it chooses (and it may in fact insert them into a
 * pre-existing packet tree once all modifications are completed).
 *
 * \ifacespython Not present.
 */
class REGINA_API NThread {
    private:
        NThreadID id_;
            /**< The system ID of the running thread, or undefined if this
                 thread is not running. */
    public:
        /**
         * Destroys this thread.
         */
        virtual ~NThread();

        /**
         * Starts a new thread and performs run() within this new thread.
         * The return value of run() is ignored.
         *
         * \pre This thread object is not already running.
         *
         * \warning If you pass \a deleteAfterwards as \c true, you must
         * not attempt to access this thread object again (since it
         * could be deleted at any time).  In particular, you will not
         * be able to call join() to wait for the new thread to terminate.
         *
         * @param args the arguments to pass to run() when it is started.
         * @param deleteAfterwards \c true if this NThread object should be
         * deleted once run() has finished.
         * @return \c true if and only if the new thread was
         * successfully started.
         */
        bool start(void* args = 0, bool deleteAfterwards = false);

        /**
         * Waits for a previously-started thread to terminate.
         *
         * Once this function returns, it is guaranteed that the thread
         * is no longer running.
         *
         * \pre This thread was started by calling the non-static
         * start(), and with \a deleteAfterwards set to \c false (the default).
         *
         * \pre You have not already called join() on the same thread.
         * However, you \e may repeatedly call start() and then join() on
         * the same thread \e object (i.e., the same instance of your
         * NThread subclass), as long as each call to start() occurs
         * after the previous thread has finished execution.
         *
         * \pre You are not calling join() from within the thread that
         * you are waiting to terminate.  That is, you are not calling
         * join() from within run().
         */
        void join();

        /**
         * Starts a new thread that performs the given routine.
         * The return value of the given routine is currently ignored.
         *
         * \deprecated This variant of start() is deprecated.  You
         * should instead create a new thread object and call the
         * non-static start() on that objet.
         *
         * @param routine the routine to run in the new thread.
         * @param args the arguments to pass to \a routine when it is
         * started.
         * @param id a location in which the ID of the new thread will
         * be placed, or 0 if the new thread ID is not required.  If
         * non-zero, this parameter must point to an already extisting
         * NThreadID that may contain any value.
         * @return \c true if and only if the new thread was
         * successfully started.
         */
        static bool start(void* (*routine)(void*), void* args,
            NThreadID* id);

        /**
         * Causes the currently running thread to voluntarily relinquish
         * the processor.  Another thread of equal or higher priority
         * will be given a turn instead.
         *
         * \deprecated Use of this routine within Regina is not advised,
         * and this routine will be removed in some future release.
         */
        static void yield();

        /**
         * The routine to run in the new thread
         * when start(void*, bool) is called.
         *
         * @param args the argument passed to start(void*, bool).
         * @return the return value will be ignored.
         */
        virtual void* run(void* args) = 0;
};

/*@}*/

// Inline functions for Mutex

inline Mutex<true>::Mutex() {
    pthread_mutex_init(&mutex, 0);
}
inline Mutex<true>::~Mutex() {
    pthread_mutex_destroy(&mutex);
}

inline void Mutex<true>::mutexLock() const {
    pthread_mutex_lock(& const_cast<Mutex<true>*>(this)->mutex);
}
inline void Mutex<true>::mutexUnlock() const {
    pthread_mutex_unlock(& const_cast<Mutex<true>*>(this)->mutex);
}

// Inline functions for Mutex::MutexLock

inline Mutex<true>::MutexLock::MutexLock(const Mutex* mutex) : mutex_(mutex) {
    mutex_->mutexLock();
}

inline Mutex<true>::MutexLock::MutexLock(const Mutex& mutex) : mutex_(&mutex) {
    mutex_->mutexLock();
}

inline Mutex<true>::MutexLock::~MutexLock() {
    mutex_->mutexUnlock();
}

inline Mutex<false>::MutexLock::MutexLock(const Mutex*) {
}

inline Mutex<false>::MutexLock::MutexLock(const Mutex&) {
}

// Inline functions for NThread

inline NThread::~NThread() {
}

inline void NThread::yield() {
    sched_yield();
}

inline void NThread::join() {
    pthread_join(id_, 0);
}

} // namespace regina

#endif

