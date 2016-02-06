
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

#include "patiencedialog.h"
#include "reginasupport.h"

#include <QApplication>
#include <QCloseEvent>
#include <QLabel>
#include <QLayout>
#include <QStyle>

PatienceDialog* PatienceDialog::warn(const QString& message,
        QWidget* parent) {
    PatienceDialog* dlg = new PatienceDialog(message, parent);
    dlg->show();
    
    QApplication::instance()->processEvents();

    return dlg;
}

PatienceDialog::PatienceDialog(const QString& message, QWidget* parent) :
        QDialog(parent, Qt::Dialog) {
    setWindowTitle(tr("Please Wait"));
    setModal(0);

    // Disable the window manager buttons (including Close).
    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &
        ~(Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint));

    QHBoxLayout* layout = new QHBoxLayout(this);

    int iconSize = QApplication::style()->pixelMetric(
        QStyle::PM_MessageBoxIconSize);

    QLabel* clock = new QLabel(this);
    clock->setPixmap(ReginaSupport::themeIcon("clock").pixmap(iconSize));
    clock->setAlignment(Qt::AlignHCenter);
    layout->addWidget(clock);

    layout->addSpacing(iconSize / 4 + 2 /* shrug */);

    QLabel* msg = new QLabel(message, this);
    layout->addWidget(msg, 1);
}

void PatienceDialog::closeEvent(QCloseEvent* e) {
    e->ignore();
}

