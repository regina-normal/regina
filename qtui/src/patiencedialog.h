
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

/*! \file patiencedialog.h
 *  \brief Provides a dialog that warns the user that they might have a
 *  bit of a wait on their hands.
 */

#ifndef __PATIENCEDIALOG_H
#define __PATIENCEDIALOG_H

#include <QDialog>


/**
 * A non-modal dialog warning the user that they might have to wait a
 * bit for the current operation to finish.
 */
class PatienceDialog : public QDialog {
    Q_OBJECT

    public:
        /**
         * Display a non-modal dialog that presents the given message to
         * the user.  The dialog is intended to be shown while some slow
         * calculation is taking place.  It will have no buttons, and will
         * contain an appropriate "waiting" icon.
         *
         * This routine will take care of updating the display itself, so
         * it may be called immediately before the calculation begins.
         * After the calculation has finished, the dialog (which is
         * returned by this routine) needs to be destroyed.
         */
        static PatienceDialog* warn(const QString& message,
            QWidget* parent = 0);

    protected:
        /**
         * Disable the window-close event.
         */
        virtual void closeEvent(QCloseEvent* e);

    private:
        /**
         * Creates a new non-modal dialog that warns the user with the
         * given message.
         */
        PatienceDialog(const QString& message,
                QWidget* parent);
};

#endif
