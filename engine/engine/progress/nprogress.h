
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file nprogress.h
 *  \brief Allows external interfaces to obtain progress reports when
 *  running long calculations.
 */

#ifndef __NPROGRESS_H
#ifndef __DOXYGEN
#define __NPROGRESS_H
#endif

#include "shareableobject.h"
#include "utilities/nthread.h"

/**
 * An object through which external interfaces can obtain progress
 * reports when running long calculations.
 * The running calculation writes to this object to
 * store the current state of progress, and the external interface reads
 * from this object from a different thread.
 *
 * When writing progress information to an NProgress object, the last
 * call should be to setFinished().  This informs all threads
 * that the operation is finished and that the NProgress object can be
 * deleted without the risk that the writing thread will attempt to
 * access it again.
 *
 * If the operation allows it (see isCancellable()), the reading thread
 * may at any time request that the operation be cancelled by calling
 * cancel().  The writing thread should regularly poll isCancelled(),
 * and if it detects a cancellation request should exit cleanly as soon
 * as possible.  Note that the writing thread should still call
 * setFinished() in this situation.
 *
 * NProgress contains multithreading support; a mutex is used to ensure
 * that the reading and writing threads do not interfere.
 * Note that multithreading is in general not supported in the
 * calculation engine, and the second thread must <b>only</b> read the
 * current state of progress and do nothing else.
 *
 * Subclasses of NProgress represent the various ways in which progress
 * can be internally stored.  Note that subclass member functions
 * <b>must</b> lock the mutex whenever internal data is being
 * accessed or modified (see mutexLock() and mutexUnlock())
 * and <b>must</b> call setChanged() if they alter the state of progress.
 *
 * \todo \feature Add timer support; measure the time elapsed between
 * the creation of this NProgress object and the call to setFinished() or
 * cancel(); hopefully measure this both in CPU time and real time.
 */
class NProgress : public ShareableObject, protected NMutex {
    private:
        bool changed;
            /**< Has the state of progress changed since the last query? */
        bool finished;
            /**< Is the operation whose progress we are reporting
             *   completely finished? */
        bool cancellable;
            /**< May this operation be cancelled? */
        bool cancelled;
            /**< Has this operation been cancelled? */
        
    public:
        /**
         * Performs basic initialisation.
         * Note that the internal mutex is not locked during construction.
         *
         * \ifaces Not present.
         *
         * @param newCancellable \c true if and only if this operation
         * allows itself to be cancelled by an external interface.
         * If \c true, the underlying operation should regularly poll
         * isCancelled() and cancel itself if appropriate.
         */
        NProgress(bool newCancellable = false);
        /**
         * Destroys this object.
         */
        virtual ~NProgress();

        /**
         * Determines if the state of progress has changed since the
         * last query.  A query is defined to be a call to
         * getDescription() or getPercent().
         *
         * This routine allows interfaces to avoid calls to the slower
         * routines getDescription() and getPercent() when they can
         * avoid it.
         *
         * If no query has yet been made, this routine will return \c true.
         *
         * @return \c true if and only if the state of progress has
         * changed since the last query.
         */
        bool hasChanged() const;
        /**
         * Is the operation whose progress we are reporting completely
         * finished?
         *
         * Once this routine returns \c true, it will always return \c
         * true; thus there will be no need to call it again.
         *
         * @return \c true if and only if the operation is finished.
         */
        bool isFinished() const;
        /**
         * Signifies that the operation whose progress we are reporting
         * is completely finished.  This <b>must</b> be the final
         * member function call to this NProgress object made by the thread
         * performing the corresponding operation.  It notifies all
         * other threads that the operation is complete and that this
         * NProgress object can be safely deleted.
         *
         * This routine should still be called by the operation thread
         * if it cancels itself in response to a request by an external
         * interface (see cancel()).
         *
         * \ifaces Not present.
         */
        void setFinished();

        /**
         * Determines whether the operation whose progress we are reporting
         * allows itself to be cancelled by an external interface.
         *
         * @return \c true if and only if cancellation is allowed.
         */
        bool isCancellable() const;
        /**
         * Called by an external interface to request that the operation
         * whose progress we are reporting be cancelled.
         * The operation itself should regularly poll
         * isCancelled() to check if an external interface has made this
         * request.
         *
         * If isCancellable() is \c false, the operation may freely
         * ignore such cancellation requests and need not poll
         * isCancelled() at all.
         *
         * \pre isCancellable() returns \c true.
         */
        void cancel();
        /**
         * Determines whether an external interface has requested that
         * the operation whose progress we are reporting be cancelled.
         *
         * If isCancellable() is \c false, the operation may freely
         * ignore such cancellation requests.  If isCancellable() is
         * \c true and the operation calls this function which
         * returns \c true, the operation should exit cleanly as soon as
         * possible with only partial or no results.
         *
         * Note that even if the underlying operation cancels itself, it
         * should still call setFinished().
         *
         * @return \c true if and only if an external interface has
         * requested that the operation be cancelled.
         */
        bool isCancelled() const;

        /**
         * Returns a string description of the current state of
         * progress.  Note that subclasses must override
         * internalGetDescription(), not this routine.
         *
         * @return the current state of progress.
         */
        NString getDescription() const;
        /**
         * Determines if the state of progress can be expressed as a
         * percentage.
         *
         * The default implementation returns \c false.
         *
         * @return \c true if and only if progress can be expressed as a
         * percentage.
         */
        virtual bool isPercent() const;
        /**
         * Returns the current state of progress as a percentage.
         * Note that subclasses must override internalGetPercent(),
         * not this routine.
         *
         * \pre Progress can be expressed as a percentage (see isPercent()).
         *
         * @return the current state of progress as a percentage.
         */
        double getPercent() const;

        void writeTextShort(ostream&) const;

    protected:
        /**
         * Sets the flag that the current state of progress has changed.
         * This routine <b>must</b> be called by any subclass member
         * function that changes the state of progress.
         *
         * The changed flag is initially set to \c true, so this routine
         * need not be called from any subclass constructors.  This
         * routine does not lock the mutex.
         */
        void setChanged();

        /**
         * Returns a string description of the current state of
         * progress.
         *
         * @return the current state of progress.
         */
        virtual NString internalGetDescription() const = 0;
        /**
         * Returns the current state of progress as a percentage.
         *
         * The default implementation returns 0.
         *
         * \pre Progress can be expressed as a percentage (see isPercent()).
         *
         * @return the current state of progress as a percentage.
         */
        virtual double internalGetPercent() const;
};

/**
 * A progress report that immediately claims it is finished.
 * There is no need to call setFinished(); this will be done
 * automatically by the constructor.
 */
class NProgressFinished : public NProgress {
    public:
        /**
         * Creates a new finished progress report.
         * This constructor will automatically call setFinished().
         */
        NProgressFinished();

        virtual bool isPercent() const;

    protected:
        virtual NString internalGetDescription() const;
        virtual double internalGetPercent() const;
};

// Inline functions for NProgress

inline NProgress::NProgress(bool newCancellable) : changed(true),
        finished(false), cancellable(newCancellable), cancelled(false) {
}
inline NProgress::~NProgress() {
}

inline bool NProgress::hasChanged() const {
    return changed;
}
inline bool NProgress::isFinished() const {
    mutexLock();
    bool ans = finished;
    mutexUnlock();
    return ans;
}
inline void NProgress::setFinished() {
    mutexLock();
    finished = true;
    mutexUnlock();
}

inline bool NProgress::isCancellable() const {
    return cancellable;
}
inline void NProgress::cancel() {
    cancelled = true;
}
inline bool NProgress::isCancelled() const {
    return cancelled;
}

inline NString NProgress::getDescription() const {
    ((NProgress*)this)->changed = false;
    return internalGetDescription();
}
inline bool NProgress::isPercent() const {
    return false;
}
inline double NProgress::getPercent() const {
    ((NProgress*)this)->changed = false;
    return internalGetPercent();
}

inline void NProgress::writeTextShort(ostream& out) const {
    out << "Progress: " << getDescription();
}

inline void NProgress::setChanged() {
    changed = true;
}

inline double NProgress::internalGetPercent() const {
    return 0;
}

// Inline functions for NProgressFinished

inline NProgressFinished::NProgressFinished() {
    setFinished();
}

inline bool NProgressFinished::isPercent() const {
    return true;
}

inline NString NProgressFinished::internalGetDescription() const {
    return "Finished.";
}
inline double NProgressFinished::internalGetPercent() const {
    return 100;
}

#endif

