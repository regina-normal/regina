
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

/*! \file progressdialogs.h
 *  \brief Provides progress dialogs that interact with different types
 *  of calculation engine progress watchers.
 */

#ifndef __PROGRESSDIALOGS_H
#define __PROGRESSDIALOGS_H

#include <QProgressDialog>

namespace regina {
    class NProgress;
    class NProgressManager;
    class NProgressNumber;
};

class QLabel;

/**
 * A dialog that interacts with the calculation engine progress class
 * regina::NProgressNumber.
 *
 * Upon calling ProgressDialogNumeric::run(), the dialog will be
 * displayed and it will follow the progress of the underlying
 * operation in the calculation engine.  The operation itself should be
 * running in a separate thread.
 */
class ProgressDialogNumeric : public QProgressDialog {
    Q_OBJECT

    private:
        regina::NProgressManager* manager;
            /**< The progress manager handling the inter-thread
                 communication. */
        const regina::NProgressNumber* progress;
            /**< The calculation engine progress watcher. */

    public:
        /**
         * Creates a new progress dialog linked to the given
         * calculation engine progress manager.
         *
         * The progress manager must not have been started, i.e.,
         * <tt>manager->isStarted()</tt> must return \c false.
         * More importantly, it must be guaranteed by the calculation
         * engine that the progress watcher to be assigned by the
         * underlying operation to this manager will be of the class
         * regina::NProgressNumber.
         */
        ProgressDialogNumeric(regina::NProgressManager* useManager,
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
 * A dialog that can interact with any progress class from the
 * calculation engine.
 *
 * Upon calling ProgressDialogMessage::run(), the dialog will be
 * displayed and it will follow the progress of the underlying
 * operation in the calculation engine.  The operation itself should be
 * running in a separate thread.
 *
 * For the time being, this dialog has no cancellation button.
 */
class ProgressDialogMessage : public QDialog {
    Q_OBJECT

    private:
        regina::NProgressManager* manager;
            /**< The progress manager handling the inter-thread
                 communication. */
        const regina::NProgress* progress;
            /**< The calculation engine progress watcher. */

        QLabel* msg;
            /**< The current progress message. */

    public:
        /**
         * Creates a new progress dialog linked to the given
         * calculation engine progress manager.
         *
         * The progress manager must not have been started, i.e.,
         * <tt>manager->isStarted()</tt> must return \c false.
         */
        ProgressDialogMessage(regina::NProgressManager* useManager,
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

#endif
