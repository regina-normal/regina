
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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

#include "reginamanager.h"


ReginaManager::ReginaManager(int argc, char **argv) : 
    QApplication(argc,argv) {
}
   

ReginaMain* ReginaManager::newWindow() {
    ReginaMain *win = new ReginaMain(false, this);
    win->show();
    children.append(win);
    return win;
}

ReginaMain* ReginaManager::newWindow(const QString url) {
    ReginaMain *win = newWindow();
    if ( ! url.isEmpty() )
        win->openUrl(url);
    return win;
}
       
void ReginaManager::onClose(ReginaMain *child) {
    children.removeOne(child);
    // Exit if we have no more to manage
    if (children.size() == 0)
        exit();
}



