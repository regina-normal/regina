
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

/*! \file exttabctl.h
 *  \brief Provides an extension to the KDE tabbed control.
 */

#ifndef __EXTTABCTL_H
#define __EXTTABCTL_H

#include <ktabctl.h>

/**
 * An extension to the KDE tabbed control.
 *
 * A public routine setCurrentTab() is provided to allow external
 * entities to change the currently visible tab.  For KTabCtl this
 * functionality is only available through the protected method
 * showTab().
 */
class ExtTabCtl : public KTabCtl {
    Q_OBJECT

    public:
        /**
         * Constructor and destructor.
         */
        ExtTabCtl(QWidget* parent = 0, const char* name = 0);

        /**
         * Change the current tab to the tab at the given index.
         */
        void setCurrentTab(int tabIndex);
};

inline ExtTabCtl::ExtTabCtl(QWidget* parent, const char* name) :
        KTabCtl(parent, name) {
}

inline void ExtTabCtl::setCurrentTab(int tabIndex) {
    showTab(tabIndex);
}

#endif
