
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton and others (see below)            *
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

/*! \file recentfilesaction.h
 *  \brief Provides an action class that offers access to recently-opened files.
 */

#ifndef __RECENTFILESACTION_H
#define __RECENTFILESACTION_H

#include <QMap>
#include <QMenu>

class QUrl;
class QActionGroup;

/**
 * An action offering access to recently-opened files.
 *
 * Much of this code is a stripped-down version of KRecentFilesAction, as
 * taken from KDE 4.4.1.  This KDE code is licensed under the GNU Library
 * General Public License version 2 as published by the Free Software
 * Foundation.  See \a recentfilesaction.cpp for the full KDE license details
 * and copyright holders.
 */
class RecentFilesAction : public QMenu {
    Q_OBJECT

    private:
        /**
         * Data files
         */
        QMap<QAction*, QUrl> urls_;
        QMap<QAction*, QString> shortNames_;

        /**
         * Internal components
         */
        QActionGroup* group_;
        QAction *noEntriesAction_;
        QAction *clearSeparator_;
        QAction *clearAction_;

        /**
         * Configuration
         */
        int maxItems_;

    public:
        /**
         * Constructor and destructor.
         */
        RecentFilesAction(QWidget* parent);
        virtual ~RecentFilesAction();

        /**
         * Add a new data file to the front of the list.
         */
        void addUrl(const QUrl& url);

        /**
         * Get and set the maximum number of files in the list.
         */
        int maxItems() const;
        void setMaxItems(int maxItems);

        /**
         * Action-related overloads from QWidget, and extended versions
         * of these.  They are public because the corresponding QWidget
         * routines are public, but you should not call these directly.
         * Instead use addUrl() and clear().
         */
        virtual void addAction(QAction* action, const QUrl& url,
                const QString& name);
        virtual QAction* removeAction(QAction* action);

    signals:
        /**
         * Emitted when a data file is selected for opening.
         */
        void urlSelected(const QUrl& url);

    public slots:
        /**
         * Clear the list of recent files.
         */
        void clear();

    protected slots:
        /**
         * All activation events lead here.
         */
        void fileActivated(QAction*);
};

#endif
