
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

/*! \file progress/nprogresstracker.h
 *  \brief Facilitates progress tracking and cancellation for long operations.
 */

#ifndef __NPROGRESSTRACKER_H
#ifndef __DOXYGEN
#define __NPROGRESSTRACKER_H
#endif

#include "regina-core.h"
#include <mutex>
#include <string>

namespace regina {

/**
 * \weakgroup progress
 * @{
 */

/**
 * Manages progress tracking and cancellation polling for long operations.
 * A typical progress tracker is simultaneously used by a \e writing thread,
 * which is performing the long calculations, and a \e reading thread,
 * which displays progress updates to the user and/or takes cancellation
 * requests from the user.
 *
 * Progress works through a series of \e stages.  Each stage has a text
 * description, as well as a percentage progress that rises from 0 to 100
 * as the stage progresses.  Each stage also has a fractional weight
 * (between 0 and 1 inclusive), and the percentage progress of the entire
 * calculation is taken to be the weighted sum of the progress of the
 * individual stages.  The weights of all stages should sum to 1.
 *
 * The life cycle of an NProgressTracker is as follows.
 *
 * - The reading thread creates an NProgressTracker, and passes it to
 *   the writing thread when the calculation begins.
 *
 * - The writing thread:
 *   - creates the first stage by calling newStage();
 *   - as the stage progresses, repeatedly updates the percentage progress of
 *     this stage by calling setPercent() and polls for cancellation by calling
 *     isCancelled();
 *   - moves through any additional stages in a similar fashion, by
 *     calling newStage() and then repeatedly calling setPercent() and
 *     isCancelled();
 *   - calls setFinished() once all processing is complete (regardless
 *     of whether the operation finished naturally or was cancelled by
 *     the user);
 *   - never touches the progress tracker again.
 *
 * - The reading thread:
 *   - passes the progress tracker to the writing thread;
 *   - repeatedly polls the state of the tracker by calling
 *     percentChanged(), descriptionChanged() and/or isFinished();
 *   - if percentChanged() returns \c true, collects the total
 *     percentage progress by calling percent() and displays this to the user;
 *   - if descriptionChanged() returns \c true, collects the description of
 *     the current stage by calling description() and displays this to the user;
 *   - if the user ever chooses to cancel the operation, calls cancel()
 *     but continues polling the state of the tracker as above;
 *   - once isFinished() returns \c true, displays any final information
 *     to the user and destroys the NProgressTracker.
 *
 * It is imperative that the writing thread does not access the tracker
 * after calling setFinished(), and it is imperative that the reading
 * thread does not destroy the tracker until after isFinished() returns
 * \c true.  In particular, even if the reading thread has called cancel(),
 * it must still wait upon isFinished() before destroying the tracker.
 * Until isFinished() returns \c true, there is no guarantee that the
 * writing thread has detected and honoured the cancellation request.
 */
class REGINA_API NProgressTracker {
    private:
        double percent_;
            /**< The percentage progress through the current stage.
                 Note that this is typically not the same as the percentage
                 progress through the entire operation. */
        std::string desc_;
            /**< The human-readable description of the current stage. */
        bool percentChanged_;
            /**< Has the percentage progress changed since the last call
                 to percentChanged()? */
        bool descChanged_;
            /**< Has the description changed since the last call
                 to descriptionChanged()? */
        bool cancelled_;
            /**< Has the reading thread requested that the operation be
                 cancelled? */
        bool finished_;
            /**< Has the writing thread declared that it has finished
                 all processing? */

        double prevPercent_;
            /**< The total percentage progress of all previous stages,
                 measured as a percentage of the entire operation. */
        double currWeight_;
            /**< The fractional weight assigned to the current stage;
                 this must be between 0 and 1 inclusive. */

        std::mutex lock_;
            /**< A mutex to stop the reading and writing threads from
                 interfering with each other. */

    public:
        /**
         * Creates a new progress tracker.
         * This sets a sensible state description (which declares that
         * the operation is initialising), and marks the current
         * progress as zero percent complete.
         *
         * This is typically called by the reading thread.
         */
        NProgressTracker();

        /**
         * Queries whether the writing thread has finished all
         * processing.  This will eventually return \c true regardless
         * of whether the processing finished naturally or was cancelled
         * by the reading thread.
         *
         * This is typically called by the reading thread.
         *
         * @return \c true if and only if the writing thread has
         * finished all processing.
         */
        bool isFinished();
        /**
         * Queries whether the percentage progress has changed since the
         * last call to percentChanged().  If this is the first time
         * percentChanged() is called, the result will be \c true.
         *
         * This is typically called by the reading thread.
         *
         * @return \c true if and only if the percentage progress has changed.
         */
        bool percentChanged();
        /**
         * Queries whether the stage description has changed since the
         * last call to descriptionChanged().  If this is the first time
         * descriptionChanged() is called, the result will be \c true.
         *
         * This is typically called by the reading thread.
         *
         * @return \c true if and only if the stage description has changed.
         */
        bool descriptionChanged();
        /**
         * Returns the percentage progress through the entire operation.
         * This combines the progress through the current stage with all
         * previous stages, taking into account the relative weights that the
         * writing thread has passed to newStage().
         *
         * This is typically called by the reading thread.
         *
         * @return \c the current percentage progress.
         */
        double percent();
        /**
         * Returns the human-readable description of the current stage.
         *
         * This is typically called by the reading thread.
         *
         * @return \c the current stage description.
         */
        std::string description();
        /**
         * Indicates to the writing thread that the user wishes to
         * cancel the operation.  The writing thread might not detect
         * and/or respond to this request immediately (or indeed ever),
         * and so the reading thread should continue to wait until
         * isFinished() returns \c true before it cleans up and destroys
         * this progress tracker.
         *
         * This is typically called by the reading thread.
         */
        void cancel();

        /**
         * Used by the writing thread to indicate that it has moved on
         * to a new stage of processing.  The percentage progress through
         * the current stage will automatically be set to 100.
         *
         * This is typically called by the writing thread.
         *
         * @param desc a human-readable description of the new stage.
         * Typically this begins with a capital and does not include a
         * final period (full stop).
         * @param weight the relative weight of this stage as a fraction
         * of the entire operation.  This weight must be between 0 and 1
         * inclusive, and the weights of \e all stages must sum to 1
         * in total.
         */
        void newStage(const char* desc, double weight = 1);
        /**
         * Queries whether the reading thread has made a request for the
         * writing thread to cancel the operation; in other words, whether
         * cancel() has been called.
         *
         * This is typically called by the writing thread.
         *
         * @return \c true if and only if a cancellation request has
         * been made.
         */
        bool isCancelled();
        /**
         * Used by the writing thread to indicate the level of progress
         * through the current stage.
         *
         * Unlike percent(), which measures progress in the context of
         * the entire operation, this routine takes a percentage that
         * is strictly relative to the current stage (i.e., the stage most
         * recently declared through a call to newStage()).  When the
         * stage begins, setPercent() would typically be given a figure
         * close to 0; when the stage is finished, setPercent() would
         * typically be given a figure close to 100.
         *
         * There is no actual need to call setPercent(0) at the
         * beginning of the stage or setPercent(100) at the end of the
         * stage, since other routines (such as the constructor,
         * newStage() and setFinished()) will take care of this for you.
         *
         * This is typically called by the writing thread.
         *
         * @param percent the percentage progress through this stage, as
         * a number between 0 and 100 inclusive.
         * @return \c true if there has been no cancellation request, or
         * \c false if cancel() has been called (typically by the reading
         * thread).
         */
        bool setPercent(double percent);
        /**
         * Used by the writing thread to indicate that it has finished
         * all processing.  The percentage progress through both the
         * current stage and the entire operation will automatically
         * be set to 100, and the stage description will be updated to
         * indicate that the operation is finished.
         *
         * This is typically called by the writing thread.
         */
        void setFinished();
};

/*@}*/

// Inline functions for NProgressTracker

inline NProgressTracker::NProgressTracker() :
        percent_(0), percentChanged_(true), descChanged_(true),
        cancelled_(false), finished_(false),
        prevPercent_(0), currWeight_(0) {
    desc_ = "Initialising";
}

inline bool NProgressTracker::isFinished() {
    std::lock_guard<std::mutex> lock(lock_);
    return finished_;
}

inline bool NProgressTracker::percentChanged() {
    std::lock_guard<std::mutex> lock(lock_);
    if (percentChanged_) {
        const_cast<NProgressTracker*>(this)->percentChanged_ = false;
        return true;
    } else
        return false;
}

inline bool NProgressTracker::descriptionChanged() {
    std::lock_guard<std::mutex> lock(lock_);
    if (descChanged_) {
        const_cast<NProgressTracker*>(this)->descChanged_ = false;
        return true;
    } else
        return false;
}

inline double NProgressTracker::percent() {
    std::lock_guard<std::mutex> lock(lock_);
    return prevPercent_ + currWeight_ * percent_;
}

inline std::string NProgressTracker::description() {
    std::lock_guard<std::mutex> lock(lock_);
    return desc_;
}

inline void NProgressTracker::cancel() {
    std::lock_guard<std::mutex> lock(lock_);
    cancelled_ = true;
}

inline void NProgressTracker::newStage(const char* desc, double weight) {
    std::lock_guard<std::mutex> lock(lock_);
    desc_ = desc;
    percent_ = 0;
    prevPercent_ += 100 * currWeight_;
    currWeight_ = weight;
    percentChanged_ = descChanged_ = true;
}

inline bool NProgressTracker::isCancelled() {
    std::lock_guard<std::mutex> lock(lock_);
    return cancelled_;
}

inline bool NProgressTracker::setPercent(double percent) {
    std::lock_guard<std::mutex> lock(lock_);
    percent_ = percent;
    percentChanged_ = true;
    return ! cancelled_;
}

inline void NProgressTracker::setFinished() {
    std::lock_guard<std::mutex> lock(lock_);
    prevPercent_ = 100;
    currWeight_ = 0;
    percent_ = 0;
    desc_ = "Finished";
    finished_ = true;
    percentChanged_ = descChanged_ = true;
}

} // namespace regina

#endif

