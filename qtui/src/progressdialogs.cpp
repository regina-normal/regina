
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

// Regina core includes:
#include "progress/nprogressmanager.h"
#include "progress/nprogresstypes.h"

// UI includes:
#include "progressdialogs.h"

#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QThread>
#include <QVBoxLayout>

namespace {
    /**
     * A subclass of QThread that gives us public access to sleep
     * routines.
     */
    class WaitingThread : public QThread {
        public:
            static void tinySleep() {
                QThread::usleep(250);
            }
    };
}

ProgressDialogNumeric::ProgressDialogNumeric(
        regina::NProgressManager* useManager,
        const QString& displayText, QWidget* parent) :
        QProgressDialog(parent),
        /* Don't use Qt::Popup because the layout breaks under fink. */
        manager(useManager), progress(0) {
    setLabelText(displayText);
    setWindowTitle(tr("Working"));
    setMinimumDuration(500);
    setWindowModality(Qt::WindowModal);
}

bool ProgressDialogNumeric::run() {
    show();
    QCoreApplication::instance()->processEvents();

    while (! manager->isStarted())
        WaitingThread::tinySleep();

    progress = dynamic_cast<const regina::NProgressNumber*>(
        manager->getProgress());
    regina::NProgressStateNumeric state;
    setMinimum(0); // Start at 0
    while (! progress->isFinished()) {
        if (wasCanceled())
            progress->cancel();
        if (progress->hasChanged()) {
            state = progress->getNumericState();
            if (state.outOf > 0) {
                setMaximum(state.outOf);
                setValue(state.completed);
            } else {
                setMaximum(0);
                setValue(0);
            }
        }
        QCoreApplication::instance()->processEvents();
        WaitingThread::tinySleep();
    }

    return (! progress->isCancelled());
}

ProgressDialogMessage::ProgressDialogMessage(
        regina::NProgressManager* useManager,
        const QString& displayText, QWidget* parent) :
        QDialog(parent),
        manager(useManager), progress(0) {
    setWindowTitle(tr("Working"));
    setWindowModality(Qt::WindowModal);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(QString("<qt><b>%1</b></qt>").arg(displayText));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QFrame* separator = new QFrame();
    separator->setFrameStyle(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    msg = new QLabel(tr("Status: Starting"));
    msg->setAlignment(Qt::AlignLeft);
    msg->setTextFormat(Qt::PlainText);
    layout->addWidget(msg);

    layout->addStretch(1);
}

bool ProgressDialogMessage::run() {
    show();
    QCoreApplication::instance()->processEvents();

    while (! manager->isStarted())
        WaitingThread::tinySleep();

    progress = manager->getProgress();
    msg->setText(tr("Status: %1").arg(progress->getDescription().c_str()));
    while (! progress->isFinished()) {
        if (progress->hasChanged()) {
            msg->setText(tr("Status: %1").arg(
                progress->getDescription().c_str()));
        }
        QCoreApplication::instance()->processEvents();
        WaitingThread::tinySleep();
    }

    return (! progress->isCancelled()); // Always true, for now.
}

