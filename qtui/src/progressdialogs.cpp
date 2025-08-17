
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

// Regina core includes:
#include "progress/progresstracker.h"

// UI includes:
#include "progressdialogs.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>

// Indicates how we convert floating-point percentages to integer counts.
// Integer progress will range from 0 to 100*SLICES_PER_PERCENT.
#define SLICES_PER_PERCENT 10

ProgressDialogNumeric::ProgressDialogNumeric(
        regina::ProgressTracker* tracker,
        const QString& displayText, QWidget* parent) :
        QProgressDialog(parent),
        /* Don't use Qt::Popup because the layout breaks under fink. */
        tracker_(tracker), metrics_(font()) {
    setLabelText(displayText);
    setWindowTitle(tr("Working"));
    setMinimumDuration(500);
    setWindowModality(Qt::WindowModal);
}

bool ProgressDialogNumeric::run() {
    show();
    QCoreApplication::instance()->processEvents();

    setMinimum(0);
    setMaximum(SLICES_PER_PERCENT * 100);
    bool stillRunning = true;
    while (! tracker_->isFinished()) {
        if (stillRunning) {
            if (wasCanceled()) {
                stillRunning = false;
                tracker_->cancel();
                // The dialog should be hidden now, so there is no point
                // updating descriptions / percentages from here on.
            } else {
                if (tracker_->percentChanged())
                    setValue(tracker_->percent() * SLICES_PER_PERCENT);
                if (tracker_->descriptionChanged()) {
                    QString text = tracker_->description().c_str();

#if QT_VERSION >= QT_VERSION_CHECK(5,11,0)
                    int w = metrics_.horizontalAdvance(text) + 100;
#else
                    int w = metrics_.width(text) + 100;
#endif
                    if (width() < w)
                        resize(w, height());

                    setLabelText(text);
                }
            }
            QCoreApplication::instance()->processEvents();
        }
        QThread::usleep(250);
    }

    return (! tracker_->isCancelled());
}

ProgressDialogMessage::ProgressDialogMessage(
        regina::ProgressTrackerBase* tracker,
        const QString& header, QWidget* parent) :
        QDialog(parent), tracker_(tracker) {
    setWindowTitle(tr("Working"));
    setWindowModality(Qt::WindowModal);

    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(QString("<b>%1</b>").arg(header));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    auto* separator = new QFrame();
    separator->setFrameStyle(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    stage = new QLabel();
    stage->setText(tracker_->description().c_str());
    stage->setAlignment(Qt::AlignLeft);
    stage->setTextFormat(Qt::PlainText);
    layout->addWidget(stage);

    layout->addStretch(1);
}

bool ProgressDialogMessage::run() {
    show();
    QCoreApplication::instance()->processEvents();

    while (! tracker_->isFinished()) {
        if (tracker_->descriptionChanged()) {
            stage->setText(tracker_->description().c_str());
        }
        QCoreApplication::instance()->processEvents();
        QThread::usleep(250);
    }

    return (! tracker_->isCancelled()); // Always true, for now.
}

ProgressDialogOpen::ProgressDialogOpen(regina::ProgressTrackerOpen* tracker,
        const QString& header, QString detailTemplate, QWidget* parent) :
        QDialog(parent), tracker_(tracker),
        detailTemplate_(std::move(detailTemplate)) {
    setWindowTitle(tr("Working"));
    setWindowModality(Qt::WindowModal);

    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(QString("<b>%1</b>").arg(header));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    auto* separator = new QFrame();
    separator->setFrameStyle(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    msg = new QLabel(tr("Starting"));
    msg->setAlignment(Qt::AlignLeft);
    msg->setTextFormat(Qt::PlainText);
    layout->addWidget(msg);

    layout->addStretch(1);

    buttons = new QDialogButtonBox(QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(rejected()), this, SLOT(cancelTask()));
    layout->addWidget(buttons);
}

bool ProgressDialogOpen::run() {
    show();
    QCoreApplication::instance()->processEvents();

    while (! tracker_->isFinished()) {
        if (tracker_->stepsChanged()) {
            msg->setText(detailTemplate_.arg(tracker_->steps()));
        }
        QCoreApplication::instance()->processEvents();
        QThread::msleep(100); // Less frequent updates; just 10 per second.
    }

    bool success = ! tracker_->isCancelled();
    buttons->button(QDialogButtonBox::Cancel)->setEnabled(false);
    tracker_ = nullptr;
    return success;
}

void ProgressDialogOpen::cancelTask() {
    // If run() is currently running, then this function will be called
    // from QCoreApplication::processEvents() within the main run() loop.
    // In particular, it will be called from the same thread, and so we
    // do not need to worry about race conditions with tracker_.
    //
    // If run() has finished, then tracker_ will be null.

    buttons->button(QDialogButtonBox::Cancel)->setEnabled(false);
    tracker_->cancel();
}

ProgressDialogObjective::ProgressDialogObjective(
        regina::ProgressTrackerObjective* tracker, const QString& header,
        QString detailTemplate, QWidget* parent) :
        QDialog(parent), tracker_(tracker),
        detailTemplate_(std::move(detailTemplate)) {
    setWindowTitle(tr("Working"));
    setWindowModality(Qt::WindowModal);

    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(QString("<b>%1</b>").arg(header));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    auto* separator = new QFrame();
    separator->setFrameStyle(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    stage = new QLabel();
    stage->setText(tracker_->description().c_str());
    stage->setAlignment(Qt::AlignLeft);
    stage->setTextFormat(Qt::PlainText);
    layout->addWidget(stage);

    msg = new QLabel();
    msg->setText(detailTemplate_.arg(tracker_->objective()));
    msg->setAlignment(Qt::AlignLeft);
    msg->setTextFormat(Qt::PlainText);
    layout->addWidget(msg);

    layout->addStretch(1);

    buttons = new QDialogButtonBox(QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(rejected()), this, SLOT(cancelTask()));
    layout->addWidget(buttons);
}

bool ProgressDialogObjective::run() {
    show();
    QCoreApplication::instance()->processEvents();

    while (! tracker_->isFinished()) {
        if (tracker_->descriptionChanged()) {
            stage->setText(tracker_->description().c_str());
        }
        if (tracker_->objectiveChanged()) {
            msg->setText(detailTemplate_.arg(tracker_->objective()));
        }
        QCoreApplication::instance()->processEvents();
        QThread::msleep(100); // Less frequent updates; just 10 per second.
    }

    bool success = ! tracker_->isCancelled();
    buttons->button(QDialogButtonBox::Cancel)->setEnabled(false);
    tracker_ = nullptr;
    return success;
}

void ProgressDialogObjective::cancelTask() {
    // If run() is currently running, then this function will be called
    // from QCoreApplication::processEvents() within the main run() loop.
    // In particular, it will be called from the same thread, and so we
    // do not need to worry about race conditions with tracker_.
    //
    // If run() has finished, then tracker_ will be null.

    buttons->button(QDialogButtonBox::Cancel)->setEnabled(false);
    tracker_->cancel();
}

