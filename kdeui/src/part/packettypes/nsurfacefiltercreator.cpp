
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

// Regina core includes:
#include "surfaces/sfcombination.h"
#include "surfaces/sfproperties.h"

// UI includes:
#include "nsurfacefiltercreator.h"
#include "../reginapart.h"

#include <kiconloader.h>
#include <klocale.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qradiobutton.h>

namespace {
    const int ID_PROPS = 0;
    const int ID_COMB = 1;
}

NSurfaceFilterCreator::NSurfaceFilterCreator() {
    ui = new QWidget();
    QGridLayout* layout = new QGridLayout(ui, 2, 2, 5);
    layout->setColStretch(1, 1);

    QLabel* pic;

    pic = new QLabel(ui);
    pic->setPixmap(SmallIcon("filter_prop", ReginaPart::factoryInstance()));
    layout->addWidget(pic, 0, 0, Qt::AlignRight);

    pic = new QLabel(ui);
    pic->setPixmap(SmallIcon("filter_comb", ReginaPart::factoryInstance()));
    layout->addWidget(pic, 1, 0, Qt::AlignRight);

    QRadioButton* props = new QRadioButton(
        i18n("Filter by properties"), ui);
    layout->addWidget(props, 0, 1, Qt::AlignLeft);

    QRadioButton* comb = new QRadioButton(
        i18n("Combination (and/or) filter"), ui);
    layout->addWidget(comb, 1, 1, Qt::AlignLeft);

    group = new QButtonGroup();
    group->insert(props, ID_PROPS);
    group->insert(comb, ID_COMB);
    group->setButton(ID_PROPS);
}

NSurfaceFilterCreator::~NSurfaceFilterCreator() {
    delete group;
}

QWidget* NSurfaceFilterCreator::getInterface() {
    return ui;
}

regina::NPacket* NSurfaceFilterCreator::createPacket(regina::NPacket*,
        QWidget*) {
    if (group->selectedId() == ID_COMB)
        return new regina::NSurfaceFilterCombination();
    else
        return new regina::NSurfaceFilterProperties();
}

