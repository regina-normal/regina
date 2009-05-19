
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file progress/nprogress.h
 *  \brief Allows external interfaces to obtain progress reports when
 *  running long calculations.
 */

#ifndef __NPROGRESS_H
#ifndef __DOXYGEN
#define __NPROGRESS_H
#endif

#include <ctime>
#include "shareableobject.h"
#include "utilities/nthread.h"

namespace regina {

/**
 * \addtogroup progress Progress Management
 * Progress reports during long calculations.
 * @{
 */

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
 * If the operation allows it, the reading thread
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
 * NProgress also contains timing support, with measurements in both
 * real time and CPU time.  See the routines getRealTime() and
 * totalCPUTime() for details.
 *
 * Subclasses of NProgress represent the various ways in which progress
 * can be internally stored.  Note that subclass member functions
 * must lock the mutex whenever internal data is being
 * accessed or modified (see NMutex::MutexLock for how this is done).
 * Any public subclass member function that changes the state of
 * progress must set the \a changed flag to \c true, and all public
 * subclass query functions must set the \a changed flag to \c false.
 */
class NProgress : public ShareableObject, protected NMutex {
    protected:
        mutable bool changed;
            /**< Has the state of progress changed since the last query? */

    private:
        bool finished;
            /**< Is the operation whose progress we are reporting
             *   completely finished? */
        bool cancelled;
            /**< Has this operation been cancelled? */
        time_t startReal;
            /**< A real time marker representing when this progress
                 object was constructed. */
        clock_t startCPU;
            /**< A CPU time marker representing when this progress
                 object was constructed. */
        time_t endReal;
            /**< A real time marker representing when this progress
                 object was marked finished. */
        clock_t endCPU;
            /**< A CPU time marker representing when this progress
                 object was marked finished. */

    public:
        /**
         * Performs basic initialisation.
         * Note that the internal mutex is not locked during construction.
         *
         * The internal state-has-changed flag is set to \c true.
         *
         * \ifacespython Not present; NProgress objects should only be
         * created within calculation engine routines whose progress is
         * being watched.
         */
        NProgress();
        /**
         * Destroys this object.
         */
        virtual ~NProgress();

        /**
         * Determines if the state of progress has changed since the
         * last query.  A query is defined to be a call to
         * getDescription(), getPercent() or any of the
         * subclass-specific query routines.
         *
         * This routine allows interfaces to avoid calls to the slower
         * query routines when they can avoid it.
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
         * \ifacespython Not present; this should only be called from
         * within the calculation engine routine whose progress is being
         * watched.
         */
        void setFinished();

        /**
         * Called by an external interface to request that the operation
         * whose progress we are reporting be cancelled.
         * The operation itself should regularly poll
         * isCancelled() to check if an external interface has made this
         * request.
         *
         * Note that if cancellation is not sensible or appropriate, the
         * operation may freely ignore such cancellation requests and need
         * not poll isCancelled() at all.
         *
         * This routine is made const since an external interface should be
         * able to cancel an operation even though it should never
         * modify the state of progress.
         */
        void cancel() const;
        /**
         * Determines whether an external interface has requested that
         * the operation whose progress we are reporting be cancelled.
         *
         * If the operation is polling for cancellation requests and it finds
         * that isCancelled() returns \c true, it should generally exit
         * (cleanly) as soon as possible with only partial or no results.
         * However, if cancellation is not sensible or appropriate, the
         * operation may freely ignore such cancellation requests.
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
        std::string getDescription() const;
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
        /**
         * Returns the real time elapsed since this operation began.
         * This routine may be called both during and after the
         * operation.
         *
         * If the operation has been marked as finished, the total
         * elapsed time from start to finish will be reported.
         * Otherwise the time elasped thus far will be reported.
         *
         * @return the total elapsed real time, measured in seconds.
         *
         * @see totalCPUTime()
         */
        long getRealTime() const;
        /**
         * Returns the total CPU time consumed by the program from the
         * beginning to the end of this operation.  This routine will
         * only return useful results after the operation has finished.
         *
         * If the operation has not yet been marked as finished, this
         * routine will return 0.
         *
         * \warning For CPU time calculations to be correct, the same
         * thread that constructs this progress object must also mark it
         * finished.
         *
         * @return the total CPU time consumed, measured in seconds.
         *
         * @see getRealTime()
         */
        long totalCPUTime() const;

        void writeTextShort(std::ostream& out) const;

    protected:
        /**
         * Returns a string description of the current state of progress.
         *
         * @return the current state of progress.
         */
        virtual std::string internalGetDescription() const = 0;
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
 *
 * \ifacespython Not present; all progress classes communicate with
 * external interfaces through the NProgress interface.
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
        virtual std::string internalGetDescription() const;
        virtual double internalGetPercent() const;
};

/*@}*/

// Inline functions for NProgress

inline NProgress::NProgress() : changed(true), finished(false),
        cancelled(false) {
    startReal = std::time(0);
    startCPU = std::clock();
}
inline NProgress::~NProgress() {
}

inline bool NProgress::hasChanged() const {
    return changed;
}
inline bool NProgress::isFinished() const {
    MutexLock(this);
    return finished;
}
inline void NProgress::setFinished() {
    MutexLock(this);
    endReal = std::time(0);
    endCPU = std::clock();
    finished = true;
}

inline void NProgress::cancel() const {
    const_cast<NProgress*>(this)->cancelled = true;
}
inline bool NProgress::isCancelled() const {
    return cancelled;
}

inline std::string NProgress::getDescription() const {
    changed = false;
    return internalGetDescription();
}
inline bool NProgress::isPercent() const {
    return false;
}
inline double NProgress::getPercent() const {
    changed = false;
    return internalGetPercent();
}

inline void NProgress::writeTextShort(std::ostream& out) const {
    out << "Progress: " << getDescription();
}

inline double NProgress::internalGetPercent() const {
    return 0;
}

inline long NProgress::getRealTime() const {
    MutexLock(this);
    return (finished ? endReal : std::time(0)) - startReal;
}

inline long NProgress::totalCPUTime() const {
    MutexLock(this);
    return (finished ? (endCPU - startCPU) / CLOCKS_PER_SEC : 0);
};

// Inline functions for NProgressFinished

inline NProgressFinished::NProgressFinished() {
    setFinished();
}

inline bool NProgressFinished::isPercent() const {
    return true;
}

inline std::string NProgressFinished::internalGetDescription() const {
    return "Finished.";
}
inline double NProgressFinished::internalGetPercent() const {
    return 100;
}

} // namespace regina

#endif

