
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

#include "qtcompat.h"

#include <qapplication.h>
#include <qglobal.h>
#include <qheader.h>
#include <qlistview.h>

namespace QtCompat {

    /**
     * Taken from Qt 3.2.3 with some modifiations, and licensed under
     * the GPL accordingly.
     */
    void adjustColumn(QListView* view, int col) {
        QHeader* hdr = view->header();
        if (hdr->isStretchEnabled(col))
            return;

        // TODO: Include the width of the column header.
        // Urgl, QHeader::sectionSizeHint() is private.
        // int w = hdr->sectionSizeHint(col, view->fontMetrics()).width();
        int w = 20;
        if (hdr->iconSet(col))
            w += hdr->iconSet(col)->pixmap().width();
        // w = QMAX(w, 20);

        QFontMetrics fm(view->fontMetrics());
        QListViewItemIterator it(view);
        int rootDepth = view->rootIsDecorated() ? view->treeStepSize() : 0;
        while (it.current()) {
            QListViewItem *item = it.current();
            ++it;
            int iw = item->width(fm, view, col);
            if (0 == col)
                iw += view->itemMargin() + rootDepth +
                    item->depth() * view->treeStepSize() - 1;
            w = QMAX(w, iw);
        }
        w = QMAX(w, QApplication::globalStrut().width());

        view->setColumnWidth(col, w);
    }

}

