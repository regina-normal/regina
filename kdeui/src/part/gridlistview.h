
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

/*! \file gridlistview.h
 *  \brief Provides support for list views in which items are surrounded
 *  by grid lines.
 */

#ifndef __GRIDLISTVIEWITEM_H
#define __GRIDLISTVIEWITEM_H

#include <KListWidget>

#include <QtGui/QListWidget>

//class QListWidget;

/**
 * Provides support for list views in which individual items are surrounded
 * by grid lines.  To create such a list view, all that is necessary is
 * to fill the list view with items derived from GridListViewItem.
 */
class GridListViewItem : public QListWidgetItem {
    public:
        /**
         * Constructors.
         */
        GridListViewItem(QListWidget* parent);
//        GridListViewItem(QListWidgetItem* parent);
//        GridListViewItem(QListWidget* parent, QListWidgetItem* after);
//        GridListViewItem(QListWidgetItem* parent, QListWidgetItem* after);
//        GridListViewItem(QListWidget* parent, QString,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null);
//        GridListViewItem(QListWidgetItem* parent, QString,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null);
//        GridListViewItem(QListWidget* parent, QListWidgetItem* after, QString,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null);
//        GridListViewItem(QListWidgetItem* parent, QListWidgetItem* after, QString,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null, QString = QString::null,
//            QString = QString::null);
//
        /**
         * QListWidgetItem overrides.
         */
        int width(const QFontMetrics& fm, const QListWidget* lv, int c) const;
        void paintCell(QPainter* p, const QColorGroup& cg, int column,
            int width, int align);
};

inline GridListViewItem::GridListViewItem(QListWidget* parent) :
        QListWidgetItem( parent, 0) {
}
//inline GridListViewItem::GridListViewItem(QListWidgetItem* parent) :
//        QListWidgetItem( *parent) {
//}
//inline GridListViewItem::GridListViewItem(QListWidget* parent,
//        QListWidgetItem* after) : QListWidgetItem(parent, after) {
//}
//inline GridListViewItem::GridListViewItem(QListWidgetItem* parent,
//        QListWidgetItem* after) : QListWidgetItem(parent, after) {
//}
//inline GridListViewItem::GridListViewItem(QListWidget* parent, QString s1,
//        QString s2, QString s3, QString s4, QString s5, QString s6,
//        QString s7, QString s8) :
//        QListWidgetItem(parent, s1, s2, s3, s4, s5, s6, s7, s8) {
//}
//inline GridListViewItem::GridListViewItem(QListWidgetItem* parent, QString s1,
//        QString s2, QString s3, QString s4, QString s5, QString s6,
//        QString s7, QString s8) :
//        QListWidgetItem(parent, s1, s2, s3, s4, s5, s6, s7, s8) {
//}
//inline GridListViewItem::GridListViewItem(QListWidget* parent,
//        QListWidgetItem* after, QString s1, QString s2, QString s3, QString s4,
//        QString s5, QString s6, QString s7, QString s8) :
//        QListWidgetItem(parent, after, s1, s2, s3, s4, s5, s6, s7, s8) {
//}
//inline GridListViewItem::GridListViewItem(QListWidgetItem* parent,
//        QListWidgetItem* after, QString s1, QString s2, QString s3, QString s4,
//        QString s5, QString s6, QString s7, QString s8) :
//        QListWidgetItem(parent, after, s1, s2, s3, s4, s5, s6, s7, s8) {
//}

#endif
