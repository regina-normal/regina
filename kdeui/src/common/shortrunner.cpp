
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

#include "shortrunner.h"

QString ShortRunner::run(bool mergeStderr) {
    // Prepare to receive standard output.
    connect(&proc, SIGNAL(receivedStdout(KProcess*, char*, int)),
        this, SLOT(collectOutput(KProcess*, char*, int)));

    // Start the child process running.
    KProcess::Communication comm = (mergeStderr ?
        KProcess::Communication(KProcess::Stdout | KProcess::MergedStderr) :
        KProcess::Stdout);
    if (! proc.start(KProcess::DontCare, comm))
        return QString::null;

    // Wait for it to finish, within a reasonable time limit.
    if (proc.wait(timeout)) {
        // All good.
        QMutexLocker lock(&outputMutex);
        return output;
    } else {
        // Timed out.
        if (! proc.kill())
            proc.kill(SIGKILL);
        reachedTimeout = true;
        return QString::null;
    }
}

void ShortRunner::collectOutput(KProcess*, char* buffer, int buflen) {
    QMutexLocker lock(&outputMutex);
    output += QString::fromLatin1(buffer, buflen);
}

#include "shortrunner.moc"
