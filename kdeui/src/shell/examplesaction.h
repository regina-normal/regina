
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file examplesaction.h
 *  \brief Provides an action class that offers access to sample data files.
 */

#ifndef __EXAMPLESACTION_H
#define __EXAMPLESACTION_H

#include <kactionclasses.h>
#include <qstringlist.h>
#include <qvaluevector.h>

/**
 * An action offering a selection of sample data files that can be
 * opened.
 *
 * Much of this class is based upon KRecentFilesAction, as taken from
 * KDE 3.2.3.  KRecentFilesAction was written by Michael Koch and is
 * released under the GNU Library General Public License (v2).
 */
class ExamplesAction : public KSelectAction {
    Q_OBJECT

    private:
        /**
         * GUI components
         */
        KPopupMenu* popup_;

        /**
         * Sample data files
         */
        QValueVector<QString> urls_;
        QStringList descs_;

    public:
        /**
         * Constructor and destructor.  The given slot is to be connected
         * to urlSelected(const KURL&).
         */
        ExamplesAction(const QObject* receiver, const char* slot,
            QObject* parent, const char* name = 0);
        virtual ~ExamplesAction();

        /**
         * Add a sample data file to the list of offerings.
         *
         * The filename should be relative to the Regina examples directory.
         */
        void addURL(const QString& fileName, const QString& description);

        /**
         * KAction overrides.
         */
        virtual int plug(QWidget *widget, int index = -1);

    signals:
        /**
         * Emitted when a sample data file is selected for opening.
         */
        void urlSelected(const KURL& url);

    protected slots:
        /**
         * All activation events lead here.
         */
        void exampleActivated(int index);

        /**
         * Internal slots for keeping the various GUI containers up-to-date
         * and following their individual events.
         */
        void menuAboutToShow();
        void slotClicked();
        virtual void slotActivated();
};

#endif
