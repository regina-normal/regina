
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file sharedtempfile.h
 *  \brief Provides temporary files that can be shared between Regina
 *  and other applications.
 */

#ifndef __SHAREDTEMPFILE_H
#define __SHAREDTEMPFILE_H

// UI includes:
#include <QFile>
#include <QUrl>

class QTemporaryFile;

/**
 * A temporary file that can be shared between Regina and other
 * applications.
 *
 * In most ways this is just like a QTemporaryFile with autoRemove,
 * except that are share() is called, Regina does not keep the file
 * open to the possible exclusion of other applications.
 *
 * The life cycle of a SharedTempFile is as follows:
 *
 * - The constructor creates the temporary file.  At this point Regina
 *   can write to it, but the underlying QTemporaryFile will remain
 *   open internally which means that external applications may not be
 *   able to access the file.
 *
 * - The routine share() is called.  At this point the underlying
 *   QTemporaryFile is destroyed and Regina no longer keeps the file
 *   open internally, which means that external applications can now read it.
 *
 * - The destructor deletes the file.  This ensures that Regina cleans
 *   up after itself; at this point (of course) no application can access
 *   the file.
 *
 * It is safe to destroy a SharedTempFile before share() is called.
 */
class SharedTempFile : public QObject {
    private:
        QTemporaryFile* tmp_;
        QString localFileName_;

    public:
        /**
         * The template filename should contain "XXXXXX", as per the
         * QTemporaryFile documentation.  Unlike QTemporaryFile,
         * however, this should be a filename with no path; the file
         * itself will be created in QDir::tempPath() automatically.
         */
        SharedTempFile(const QString& templateFileName, QObject* parent);
        ~SharedTempFile();

        void share();

        /**
         * If the temporary file was not successfully created,
         * localFileName() return null and url() returns the empty URL.
         * See valid().
         */
        QUrl url() const;
        QString localFileName() const;

        /**
         * Was the temporary file successfully created by the constructor?
         */
        bool valid() const;
};

#endif

