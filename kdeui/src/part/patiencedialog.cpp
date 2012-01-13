
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

#include "patiencedialog.h"

#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QPixmap>

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

    QWidget* page = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(page);

    QLabel* clock = new QLabel(page);
    clock->setPixmap(QPixmap("patience"));
    clock->setAlignment(Qt::AlignHCenter);
    layout->addWidget(clock);

    QLabel* msg = new QLabel(message, page);
    layout->addWidget(msg, 1);
}

