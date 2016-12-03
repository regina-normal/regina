
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

/*! \file tri3algebra.h
 *  \brief Provides an algebra viewer for triangulations.
 */

#ifndef __GROUPWIDGET_H
#define __GROUPWIDGET_H

#include <QWidget>

class QLabel;
class QListWidget;

namespace regina {
    class Packet;
    class GroupPresentation;
};

/**
 * A triangulation page for viewing the fundamental group.
 */
class GroupWidget : public QWidget {
    Q_OBJECT

    private:
        const regina::GroupPresentation* group_;
        regina::GroupPresentation* simplified_;

        QWidget* ui_;
        QLabel* fundName_;
        QLabel* fundGens_;
        QLabel* fundRelCount_;
        QListWidget* fundRels_;

    public:
        /**
         * Constructor.
         */
        GroupWidget(bool allowSimplify, bool paddingStretch);
        ~GroupWidget();

        /**
         * Refresh the contents of the widget.
         */
        void refresh(const regina::GroupPresentation* group);

        /**
         * The following routine drops ownership of simplified_ (it is
         * assumed that the caller will claim ownership instead), and
         * sets simplified_ to null.
         */
        regina::GroupPresentation* takeSimplifiedGroup();

    signals:
        /**
         * Indicates that the group presentation has been simplified.
         * Other elements of the UI can use this signal to pass the
         * simplified group presentation back to Regina's calculation engine.
         */
        void simplified();

    public slots:
        /**
         * Group simplification via GAP.
         */
        void simplifyGAP();
        /**
         * Regina's own simplification code.
         */
        void simplifyInternal();
        /**
         * Search for more potentially useful relators.
         */
        void proliferateRelators();

    private:
        /**
         * Returns the full path to the GAP executable, or QString::null
         * if the GAP executable does not appear to be valid.
         */
        QString verifyGAPExec();
};

inline regina::GroupPresentation* GroupWidget::takeSimplifiedGroup() {
    regina::GroupPresentation* ans = simplified_;
    simplified_ = 0;
    return ans;
}

#endif
