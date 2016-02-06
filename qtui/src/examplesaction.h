
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

/*! \file examplesaction.h
 *  \brief Provides an action class that offers access to sample data files.
 */

#ifndef __EXAMPLESACTION_H
#define __EXAMPLESACTION_H

#include <QMap>
#include <QMenu>

class QUrl;
class QActionGroup;

/**
 * An action offering a selection of sample data files that can be
 * opened.
 *
 * Much of this class is based upon KRecentFilesAction, as taken from
 * KDE 3.2.3.  KRecentFilesAction was written by Michael Koch and is
 * released under the GNU Library General Public License (v2).
 */
class ExamplesAction : public QMenu {
    Q_OBJECT

    private:
        /**
         * Sample data files
         */
        QMap<QAction*, QUrl> urls_;

        /**
         * Group of actions in the menu
         */
        QActionGroup* group;

    public:
        /**
         * Constructor and destructor.
         */
        ExamplesAction(QWidget* parent);
        virtual ~ExamplesAction();

        /**
         * Add a sample data file to the list of offerings.
         *
         * The filename should be relative to the Regina examples directory.
         */
        void addUrl(const QString& fileName, const QString& description);

        /**
         * Fill this action with Regina's standard example files.
         */
        void fillStandard();

    signals:
        /**
         * Emitted when a sample data file is selected for opening.
         */
        void urlSelected(const QUrl& url, const QString& description);

    protected slots:
        /**
         * All activation events lead here.
         */
        void exampleActivated(QAction*);
};

#endif
