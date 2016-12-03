
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

// Regina core includes:
#include "surfaces/surfacefilter.h"

// UI includes:
#include "iconcache.h"
#include "filtercreator.h"
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

FilterCreator::FilterCreator() {
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

FilterCreator::~FilterCreator() {
    delete group;
}

QWidget* FilterCreator::getInterface() {
    return ui;
}

regina::Packet* FilterCreator::createPacket(regina::Packet*,
        QWidget*) {
    regina::Packet* ans;
    if (group->checkedId() == ID_COMB)
        ans = new regina::SurfaceFilterCombination();
    else
        ans = new regina::SurfaceFilterProperties();

    ans->setLabel("Surface filter");
    return ans;
}

