
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
#include "surfaces/sfcombination.h"
#include "surfaces/sfproperties.h"

// UI includes:
#include "iconcache.h"
#include "nsurfacefiltercreator.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QApplication>
#include <QButtonGroup>
#include <QLabel>
#include <QLayout>
#include <QRadioButton>
#include <QStyle>
#include <QWhatsThis>

namespace {
    const int ID_PROPS = 0;
    const int ID_COMB = 1;
}

NSurfaceFilterCreator::NSurfaceFilterCreator() {
    ui = new QWidget();
    QGridLayout* layout = new QGridLayout(ui);//, 2, 2, 5);
    layout->setColumnStretch(1, 1);

    int iconSize = QApplication::style()->pixelMetric(
        QStyle::PM_ButtonIconSize);

    QLabel* pic;
    QString msg;

    pic = new QLabel(ui);
    pic->setPixmap(IconCache::icon(IconCache::filter_prop).pixmap(
        iconSize, iconSize));
    layout->addWidget(pic, 0, 0, Qt::AlignRight);

    QRadioButton* props = new QRadioButton(
        QObject::tr("Filter by properties"), ui);
    layout->addWidget(props, 0, 1, Qt::AlignLeft);

    msg = QObject::tr("Create a filter that examines properties of normal surfaces, "
        "such as orientability, boundary and Euler characteristic.");
    pic->setWhatsThis(msg);
    props->setWhatsThis(msg);

    pic = new QLabel(ui);
    pic->setPixmap(IconCache::icon(IconCache::filter_comb).pixmap(
        iconSize, iconSize));
    layout->addWidget(pic, 1, 0, Qt::AlignRight);

    QRadioButton* comb = new QRadioButton(
        QObject::tr("Combination (AND/OR) filter"), ui);
    layout->addWidget(comb, 1, 1, Qt::AlignLeft);

    msg = QObject::tr("Create a filter that combines other filters using boolean "
        "AND or OR.");
    pic->setWhatsThis(msg);
    comb->setWhatsThis(msg);

    group = new QButtonGroup();
    group->addButton(props, ID_PROPS);
    group->addButton(comb, ID_COMB);
    group->button(ID_PROPS)->setChecked(true);
}

NSurfaceFilterCreator::~NSurfaceFilterCreator() {
    delete group;
}

QWidget* NSurfaceFilterCreator::getInterface() {
    return ui;
}

regina::NPacket* NSurfaceFilterCreator::createPacket(regina::NPacket*,
        QWidget*) {
    if (group->checkedId() == ID_COMB)
        return new regina::NSurfaceFilterCombination();
    else
        return new regina::NSurfaceFilterProperties();
}

