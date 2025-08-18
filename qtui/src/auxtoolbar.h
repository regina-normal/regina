
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

/*! \file auxtoolbar.h
 *  \brief Provides a small transparent toolbar for use alongside tables.
 */

#ifndef __AUXTOOLBAR_H
#define __AUXTOOLBAR_H

#include "reginaqt.h"
#include <QApplication>
#include <QBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QToolBar>

/**
 * A small transparent toolbar for use alongside tables.
 *
 * Typically this would be placed beneath a table, and it would contain helper
 * buttons to add and/or remove table items.  It is designed to be pushed
 * right up against the table, without any additional padding or spacing.
 */
class AuxToolBar : public QWidget {
    private:
        QToolBar* bar_;

    public:
        AuxToolBar(QWidget* parent = nullptr);

        void addAction(QAction* action);
        void addLabel(const QString& text);
};

inline AuxToolBar::AuxToolBar(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    // Use something a little smaller than the default margins.
    layout->setContentsMargins(5, 5, 5, 5);

    layout->addStretch(1);

    bar_ = new QToolBar();
    bar_->setFloatable(false);
    bar_->setToolButtonStyle(Qt::ToolButtonIconOnly);
    int size = QApplication::style()->pixelMetric(QStyle::PM_SmallIconSize);
    bar_->setIconSize({ size, size });
    bar_->setStyleSheet("border: none; background-color: transparent");
    layout->addWidget(bar_);

    layout->addStretch(1);
}

inline void AuxToolBar::addAction(QAction* action) {
    bar_->addAction(action);
}

inline void AuxToolBar::addLabel(const QString& text) {
    QLabel* label = new QLabel(text);
    label->setAlignment(Qt::AlignCenter);
    // Add some horizontal space around the label.
    label->setContentsMargins(5, 0, 5, 0);
    bar_->addWidget(label);
}

#endif
