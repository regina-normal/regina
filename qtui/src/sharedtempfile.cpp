
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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

#include "sharedtempfile.h"

#include <QDir>
#include <QTemporaryFile>

SharedTempFile::SharedTempFile(const QString& templateFileName,
        QObject* parent) : QObject(parent) {
    tmp_ = new QTemporaryFile(
        QString("%1/%2").arg(QDir::tempPath()).arg(templateFileName));
    if (tmp_->open()) {
        tmp_->close();
        tmp_->setAutoRemove(false);
        localFileName_ = tmp_->fileName();
    } else {
        delete tmp_;
        tmp_ = 0;
        localFileName_ = QString();
    }
}

SharedTempFile::~SharedTempFile() {
    delete tmp_;

    // Silently try to remove the file.
    if (! localFileName_.isNull())
        QFile(localFileName_).remove();
}

void SharedTempFile::share() {
    delete tmp_;
    tmp_ = 0;
}

QUrl SharedTempFile::url() const {
    if (localFileName_.isNull())
        return QUrl();
    else
        return QUrl::fromLocalFile(localFileName_);
}

QString SharedTempFile::localFileName() const {
    return localFileName_;
}

bool SharedTempFile::valid() const {
    return ! localFileName_.isNull();
}
