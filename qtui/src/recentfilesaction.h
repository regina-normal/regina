
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton and others (see below)            *
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

/*! \file recentfilesaction.h
 *  \brief Provides an action class that offers access to recently-opened files.
 */

#ifndef __RECENTFILESACTION_H
#define __RECENTFILESACTION_H

#include <QList>
#include <QMenu>

class QUrl;

/**
 * An action offering access to recently-opened files.
 *
 * This code was originally derived from KRecentFilesAction, as
 * taken from KDE 4.4.1.  This KDE code is licensed under the GNU Library
 * General Public License version 2 as published by the Free Software
 * Foundation.  See \a recentfilesaction.cpp for the full KDE license details
 * and copyright holders.
 */
class RecentFilesAction : public QMenu {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        QList<QAction*> urlActions_;
        QAction *noEntriesAction_;
        QAction *clearSeparator_;
        QAction *clearAction_;

    public:
        /**
         * Constructor and destructor.
         */
        RecentFilesAction(QWidget* parent);
        virtual ~RecentFilesAction();

    signals:
        /**
         * Emitted when a data file is selected for opening.
         */
        void urlSelected(const QUrl& url);

    private slots:
        /**
         * All activation events lead here.
         */
        void fileActivated();

        /**
         * Slots that update the GUI items as the list of recent files changes.
         */
        void addUrl(const QUrl& url);
        void promoteUrl(const QUrl& url);
        void removeUrlLast();
        void clearUrls();
        void fillUrls();
};

#endif
