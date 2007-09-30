
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

/*! \file shortrunner.h
 *  \brief Provides a means for running a process and collecting output
 *  with a timeout for safety.
 */

#ifndef __SHORTRUNNER_H
#define __SHORTRUNNER_H

#include <kprocess.h>
#include <qmutex.h>

/**
 * A one-shot class for running a process and collecting output.
 * A timeout must be past to guard against a child process that does not
 * terminate.
 *
 * An object of this class can only be used once.
 */
class ShortRunner : public QObject {
    Q_OBJECT

    private:
        KProcess proc;
        int timeout;
        QString output;
        QMutex outputMutex;
        bool reachedTimeout;

    public:
        ShortRunner(int timeoutSeconds = 2);

        /**
         * Argument passing routines akin to those in KProcess.
         */
        ShortRunner& operator << (const char* arg);
        ShortRunner& operator << (const QString& arg);

        /**
         * Runs the process and collects standard output (optionally
         * with standard error also).
         *
         * Returns QString::null if the process could not be started,
         * or if it times out (call timedOut() to distinguish between
         * these two cases).  Otherwise returns the output produced (and
         * an empty string if no output was produced).
         */
        QString run(bool mergeStderr = false);

        /**
         * Did the process reach the timeout before termination?
         * This routine is used to distinguish between the never-started
         * and never-finished cases, both of which return QString::null
         * from run().
         */
        bool timedOut() const;

    private slots:
        /**
         * Process notifications.
         */
        void collectOutput(KProcess*, char* buffer, int buflen);
};

inline ShortRunner::ShortRunner(int timeoutSeconds) :
        timeout(timeoutSeconds), output(""), reachedTimeout(false) {
}

inline ShortRunner& ShortRunner::operator << (const char* arg) {
    proc << arg;
    return *this;
}

inline ShortRunner& ShortRunner::operator << (const QString& arg) {
    proc << arg;
    return *this;
}

inline bool ShortRunner::timedOut() const {
    return reachedTimeout;
}

#endif
