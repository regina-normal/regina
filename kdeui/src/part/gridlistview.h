
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include <klistview.h>

class QListView;

/**
 * Provides support for list views in which individual items are surrounded
 * by grid lines.  To create such a list view, all that is necessary is
 * to fill the list view with items derived from GridListViewItem.
 */
class GridListViewItem : public KListViewItem {
    public:
        /**
         * Constructors.
         */
        GridListViewItem(QListView* parent);
        GridListViewItem(QListViewItem* parent);
        GridListViewItem(QListView* parent, QListViewItem* after);
        GridListViewItem(QListViewItem* parent, QListViewItem* after);
        GridListViewItem(QListView* parent, QString,
            QString = QString::null, QString = QString::null,
            QString = QString::null, QString = QString::null,
            QString = QString::null, QString = QString::null,
            QString = QString::null);
        GridListViewItem(QListViewItem* parent, QString,
            QString = QString::null, QString = QString::null,
            QString = QString::null, QString = QString::null,
            QString = QString::null, QString = QString::null,
            QString = QString::null);
        GridListViewItem(QListView* parent, QListViewItem* after, QString,
            QString = QString::null, QString = QString::null,
            QString = QString::null, QString = QString::null,
            QString = QString::null, QString = QString::null,
            QString = QString::null);
        GridListViewItem(QListViewItem* parent, QListViewItem* after, QString,
            QString = QString::null, QString = QString::null,
            QString = QString::null, QString = QString::null,
            QString = QString::null, QString = QString::null,
            QString = QString::null);

        /**
         * QListViewItem overrides.
         */
        int width(const QFontMetrics& fm, const QListView* lv, int c) const;
        void paintCell(QPainter* p, const QColorGroup& cg, int column,
            int width, int align);
};

inline GridListViewItem::GridListViewItem(QListView* parent) :
        KListViewItem(parent) {
}
inline GridListViewItem::GridListViewItem(QListViewItem* parent) :
        KListViewItem(parent) {
}
inline GridListViewItem::GridListViewItem(QListView* parent,
        QListViewItem* after) : KListViewItem(parent, after) {
}
inline GridListViewItem::GridListViewItem(QListViewItem* parent,
        QListViewItem* after) : KListViewItem(parent, after) {
}
inline GridListViewItem::GridListViewItem(QListView* parent, QString s1,
        QString s2, QString s3, QString s4, QString s5, QString s6,
        QString s7, QString s8) :
        KListViewItem(parent, s1, s2, s3, s4, s5, s6, s7, s8) {
}
inline GridListViewItem::GridListViewItem(QListViewItem* parent, QString s1,
        QString s2, QString s3, QString s4, QString s5, QString s6,
        QString s7, QString s8) :
        KListViewItem(parent, s1, s2, s3, s4, s5, s6, s7, s8) {
}
inline GridListViewItem::GridListViewItem(QListView* parent,
        QListViewItem* after, QString s1, QString s2, QString s3, QString s4,
        QString s5, QString s6, QString s7, QString s8) :
        KListViewItem(parent, after, s1, s2, s3, s4, s5, s6, s7, s8) {
}
inline GridListViewItem::GridListViewItem(QListViewItem* parent,
        QListViewItem* after, QString s1, QString s2, QString s3, QString s4,
        QString s5, QString s6, QString s7, QString s8) :
        KListViewItem(parent, after, s1, s2, s3, s4, s5, s6, s7, s8) {
}

#endif
