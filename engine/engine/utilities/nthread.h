
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nthread.h
 *  \brief Provides very basic thread handling.
 */

#ifndef __NTHREAD_H
#ifndef __DOXYGEN
#define __NTHREAD_H
#endif

#include <pthread.h>
#include <sched.h>

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A mutual exclusion device (mutex) used to ensure that different
 * threads do not interfere when working with the same data.
 *
 * A mutex can be either locked or unlocked, and can only be locked by
 * one thread at a time.  If a second thread tries to lock the mutex,
 * it will be suspended until the mutex is unlocked by the original
 * locking thread.
 *
 * Classes can inherit from NMutex to provide mutex protection for
 * their internal data; it is recommended that such inheritance be
 * \c protected and that the member functions alone take full
 * responsibility for locking and unlocking the mutex when appropriate.
 * Alternatively, a standalone NMutex object can be passed about between
 * routines.
 *
 * \ifaces Not present.
 */
class NMutex {
    private:
        pthread_mutex_t mutex;
            /**< The C mutex used by internal function calls. */

    public:
        /**
         * Creates a new mutex.
         * The mutex will be created unlocked.
         */
        NMutex();
        /**
         * Destroys this mutex.
         *
         * \pre This mutex is unlocked.
         */
        ~NMutex();

        /**
         * Locks this mutex.
         * If any thread has already locked this mutex, this thread will
         * be suspended until the mutex is unlocked by the other thread,
         * and will then lock this mutex itself.
         *
         * This routine is \c const to simplify using it in data
         * retrieval routines for subclasses of NMutex.
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
         * retrieval routines for subclasses of NMutex.
         *
         * \pre The mutex is currently locked by this thread.
         */
        void mutexUnlock() const;
};

typedef pthread_t NThreadID;
    /**< The type used for a thread identifier. */

/**
 * Provides very basic thread handling.
 *
 * Each subclass of NThread represents a specific task that new
 * threads should perform.
 *
 * A subclass should override run() so that it performs whatever task is
 * required of each new thread.  Then start() may be called whenever a
 * new thread is required.
 *
 * \ifaces Not present.
 */
class NThread {
    public:
        /**
         * Destroys this thread.
         */
        virtual ~NThread();

        /**
         * Starts a new thread that performs the run() routine.
         * The return value of run() is currently ignored.
         *
         * @param args the arguments to pass to run() when it is started.
         * @param deleteAfterwards \c true if this NThread object should be
         * deleted once run() has finished.
         * @return \c true if and only if the new thread was
         * successfully started.
         */
        bool start(void* args = 0, bool deleteAfterwards = false);
        /**
         * Starts a new thread that performs the given routine.
         * The return value of the given routine is currently ignored.
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
         */
        static void yield();

        /**
         * The routine to run in the new thread
         * when start(void*) is called.
         *
         * @param args the argument passed to start(void*).
         * @return the return value is currently ignored.
         */
        virtual void* run(void* args) = 0;
};

/*@}*/

// Inline functions for NMutex

inline NMutex::NMutex() {
    pthread_mutex_init(&mutex, 0);
}
inline NMutex::~NMutex() {
    pthread_mutex_destroy(&mutex);
}

inline void NMutex::mutexLock() const {
    pthread_mutex_lock(&((NMutex*)this)->mutex);
}
inline void NMutex::mutexUnlock() const {
    pthread_mutex_unlock(&((NMutex*)this)->mutex);
}

// Inline functions for NThread

inline NThread::~NThread() {
}

inline void NThread::yield() {
    sched_yield();
}

} // namespace regina

#endif

