
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

#include "patiencedialog.h"

#include <kapplication.h>
#include <kiconloader.h>
#include <klocale.h>
#include <qlabel.h>
#include <qlayout.h>

PatienceDialog* PatienceDialog::warn(const QString& message,
        KInstance* instance, QWidget* parent) {
    PatienceDialog* dlg = new PatienceDialog(message, instance, parent);
    dlg->show();

    KApplication::kApplication()->processEvents();

    return dlg;
}

PatienceDialog::PatienceDialog(const QString& message, KInstance* instance,
        QWidget* parent, const char* name) :
        KDialogBase(Plain, i18n("Please Wait"), 0 /* no buttons */,
        static_cast<ButtonCode>(0), parent, name, false) {
    QFrame* page = plainPage();
    QHBoxLayout* layout = new QHBoxLayout(page, 0, spacingHint());

    QLabel* clock = new QLabel(page);
    clock->setPixmap(UserIcon("patience", instance));
    clock->setAlignment(AlignCenter);
    layout->add(clock);

    QLabel* msg = new QLabel(message, page);
    layout->add(msg);
    layout->setStretchFactor(msg, 1);
}

#include "patiencedialog.moc"
