
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include <kapplication.h>
#include <kiconloader.h>
#include <klocale.h>
#include <qlabel.h>
#include <qlayout.h>

PatienceDialog* PatienceDialog::warn(const QString& message,
        QWidget* parent) {
    PatienceDialog* dlg = new PatienceDialog(message, parent);
    dlg->show();

    KApplication::kApplication()->processEvents();

    return dlg;
}

PatienceDialog::PatienceDialog(const QString& message,
        QWidget* parent) :
        KDialog(parent) {
    setCaption(i18n("Please Wait"));
    setModal(0);
    setButtons(None);

    QWidget* page = new QWidget(this);
    setMainWidget(page);
    QHBoxLayout* layout = new QHBoxLayout(page); //, 0, spacingHint());

    QLabel* clock = new QLabel(page);
    clock->setPixmap(UserIcon("patience"));
    clock->setAlignment(Qt::AlignHCenter);
    layout->addWidget(clock);

    QLabel* msg = new QLabel(message, page);
    layout->addWidget(msg);
    layout->setStretchFactor(msg, 1);
}

#include "moc_patiencedialog.cpp"
