
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

/*! \file nprogressmanager.h
 *  \brief Facilitates sharing NProgress objects between an
 *  operation thread and an external interface.
 */

#ifndef __NPROGRESSMANAGER_H
#ifndef __DOXYGEN
#define __NPROGRESSMANAGER_H
#endif

#include "progress/nprogress.h"

namespace regina {

/**
 * \weakgroup progress
 * @{
 */

/**
 * Manages the sharing of an NProgress object between reading and
 * writing threads.
 *
 * The life cycle of an NProgressManager and the corresponding NProgress
 * is as follows.  Note that the <i>reading thread</i> is the interface
 * thread that is querying the state of progress, and the <i>writing
 * thread</i> is the thread in which the operation is actually being
 * performed.
 *
 * - Before the operation begins, an NProgressManager is created and
 *   both the reading and writing threads have access to it.  This can
 *   be achieved for instance by having the reading thread create an
 *   NProgressManager and pass it to the writing thread as a parameter
 *   of the actual operation function call.
 * - The writing thread:
 *   - creates a new NProgress and stores it in the NProgressManager
 *     using setProgress();
 *   - updates the NProgress throughout the operation;
 *   - finally calls NProgress::setFinished() when the operation is
 *     complete;
 *   - <b>does not touch</b> either the NProgress or the NProgressManager
 *     again.
 * - The reading thread:
 *   - repeatedly calls isStarted() and waits until this returns \c true;
 *   - now knows the NProgress has been created and begins querying it
 *     using getProgress() and displaying progress reports;
 *   - whilst querying, repeatedly calls isFinished() to test if the
 *     operation is complete;
 *   - once isFinished() returns \c true, displays the final progress
 *     message to the user and destroys the NProgressManager.
 * - Note that the NProgressManager destructor will automatically destroy the
 *   NProgress.  It is imperative that the writing thread does not touch
 *   either the NProgress or the NProgressManager after calling
 *   NProgress::setFinished(), since once isFinished()
 *   returns \c true the reading
 *   thread might destroy the NProgressManager and thus the NProgress at
 *   any time.  The writing thread cannot destroy these objects because
 *   it has no guarantee that the reading thread is not still reading
 *   progress reports from them.
 */
class NProgressManager : public ShareableObject {
    private:
        NProgress* progress;
            /**< The progress report object that we are managing. */

    public:
        /**
         * Creates a new progress manager with no NProgress to manage.
         */
        NProgressManager();
        /**
         * Destroys this manager as well as the corresponding NProgress.
         *
         * \pre There is an NProgress assigned to this manager; that is,
         * isStarted() returns \c true.
         * \pre The NProgress that we are managing has finished, that is,
         * isFinished() returns \c true.
         */
        ~NProgressManager();

        /**
         * Determines if an NProgress has been assigned to this
         * manager yet.
         *
         * Once this routine returns \c true, it will always return \c
         * true; thus there will be no need to call it again.
         *
         * @return \c true if and only if an NProgress has been assigned
         * to this manager.
         */
        bool isStarted() const;
        /**
         * Determines if the NProgress that we are managing has finished.
         * That is, this routine determines if NProgress::isFinished()
         * returns \c true.
         *
         * Once this routine returns \c true, it will always return \c
         * true; thus there will be no need to call it again.
         *
         * \pre There is an NProgress assigned to this manager; that is,
         * isStarted() returns \c true.
         *
         * @return \c true if and only if the NProgress that we are
         * managing has finished.
         */
        bool isFinished() const;

        /**
         * Returns the NProgress that this manager is managing.
         * If isStarted() returns \c true, you are guaranteed that
         * this routine will not return zero.
         *
         * @return the NProgress that this manager is managing, or 0 if
         * an NProgress has not yet been assigned to this manager.
         */
        const NProgress* getProgress() const;
        /**
         * Assigns the given NProgress to this manager to manage.
         *
         * \pre setProgress() has not already been called.
         *
         * \ifaces Not present.
         *
         * @param newProgress the NProgress that this manager will manage.
         */
        void setProgress(NProgress* newProgress);

        void writeTextShort(std::ostream& out) const;
};

/*@}*/

// Inline functions for NProgressManager

inline NProgressManager::NProgressManager() : progress(0) {
}
inline NProgressManager::~NProgressManager() {
    if (progress)
        delete progress;
}

inline bool NProgressManager::isStarted() const {
    return (progress != 0);
}
inline bool NProgressManager::isFinished() const {
    return progress->isFinished();
}

inline const NProgress* NProgressManager::getProgress() const {
    return progress;
}
inline void NProgressManager::setProgress(NProgress* newProgress) {
    progress = newProgress;
}

inline void NProgressManager::writeTextShort(std::ostream& out) const {
    out << "[Progress Manager]";
}

} // namespace regina

#endif

