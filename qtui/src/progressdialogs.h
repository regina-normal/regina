
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

/*! \file progressdialogs.h
 *  \brief Provides progress dialogs that interact with different types
 *  of calculation engine progress watchers.
 */

#ifndef __PROGRESSDIALOGS_H
#define __PROGRESSDIALOGS_H

#include <QProgressDialog>

namespace regina {
    class ProgressTracker;
    class ProgressTrackerOpen;
};

class QDialogButtonBox;
class QLabel;

/**
 * A dialog that interacts with a calculation engine progress tracker,
 * displays both status messages and percentage progress, and supports
 * cancellation.
 *
 * Upon calling ProgressDialogNumeric::run(), the dialog will be
 * displayed and it will follow the progress of the underlying
 * operation in the calculation engine.  The operation itself should be
 * running in a separate thread.
 */
class ProgressDialogNumeric : public QProgressDialog {
    Q_OBJECT

    private:
        regina::ProgressTracker* tracker_;
            /**< The progress tracker handling the inter-thread
                 communication. */

    public:
        /**
         * Creates a new progress dialog linked to the given
         * calculation engine progress tracker.
         *
         * The progress tracker must not have been started, i.e.,
         * <tt>tracker->isStarted()</tt> must return \c false.
         */
        ProgressDialogNumeric(regina::ProgressTracker* tracker,
                const QString& displayText, QWidget* parent = 0);

        /**
         * Displays the dialog and follows the progress of the
         * underlying operation.
         *
         * This routine will only return once the operation has finished.
         * It returns \c true on successful completion, or \c false
         * if the operation was cancelled.
         */
        bool run();
};

/**
 * A dialog that interacts with a calculation engine progress tracker,
 * displays only status messages (no percentage progress), and does not
 * support cancellation.
 *
 * Upon calling ProgressDialogMessage::run(), the dialog will be
 * displayed and it will follow the progress of the underlying
 * operation in the calculation engine.  The operation itself should be
 * running in a separate thread.
 */
class ProgressDialogMessage : public QDialog {
    Q_OBJECT

    private:
        regina::ProgressTracker* tracker_;
            /**< The progress tracker handling the inter-thread
                 communication. */

        QLabel* msg;
            /**< The current progress message. */

    public:
        /**
         * Creates a new progress dialog linked to the given
         * calculation engine progress tracker.
         *
         * The progress tracker must not have been started, i.e.,
         * <tt>tracker->isStarted()</tt> must return \c false.
         */
        ProgressDialogMessage(regina::ProgressTracker* tracker,
                const QString& displayText, QWidget* parent = 0);

        /**
         * Displays the dialog and follows the progress of the
         * underlying operation.
         *
         * This routine will only return once the operation has finished.
         * It returns \c true on successful completion, or \c false
         * if the operation was cancelled.
         */
        bool run();
};

/**
 * A dialog that interacts with a calculation engine progress tracker,
 * displays an integer number of steps of progress, and supports cancellation.
 *
 * Upon calling ProgressDialogOpen::run(), the dialog will be
 * displayed and it will follow the progress of the underlying
 * operation in the calculation engine.  The operation itself should be
 * running in a separate thread.
 */
class ProgressDialogOpen : public QDialog {
    Q_OBJECT

    private:
        regina::ProgressTrackerOpen* tracker_;
            /**< The progress tracker handling the inter-thread
                 communication. */
        QString detailTemplate_;
            /**< The string displaying the number of steps completed so
                 far.  This string should contain "%1", which will be
                 replaced with the integer number of steps. */

        QLabel* msg;
            /**< The current detailed progress message.  This displays
                 a string based on \a detailTemplate_. */
        QDialogButtonBox* buttons;
            /**< The box containing the cancel button. */

    public:
        /**
         * Creates a new progress dialog linked to the given
         * calculation engine progress tracker.
         *
         * The progress tracker must not have been started, i.e.,
         * <tt>tracker->isStarted()</tt> must return \c false.
         */
        ProgressDialogOpen(regina::ProgressTrackerOpen* tracker,
                const QString& displayText, const QString& detailTemplate,
                QWidget* parent = 0);

        /**
         * Displays the dialog and follows the progress of the
         * underlying operation.
         *
         * This routine will only return once the operation has finished.
         * It returns \c true on successful completion, or \c false
         * if the operation was cancelled.
         */
        bool run();

    public slots:
        /**
         * Called when the user asks to cancel the operation.
         */
        void cancelTask();
};

#endif
