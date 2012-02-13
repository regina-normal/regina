
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

#include "file/nglobaldirs.h"
#include "reginasupport.h"
#include <QCoreApplication>

// TODO: Rewrite this and do it properly.

QString ReginaSupport::home_;

const QString& ReginaSupport::home() {
    if (home_.isNull()) {
#ifdef Q_OS_MACX
        home_ = QCoreApplication::applicationDirPath() + "/../Resources";
#else
        home_ = regina::NGlobalDirs::home();
#endif

    }

    return home_;
}

QIcon ReginaSupport::regIcon(const QString& name) {
    QIcon icon(home() + "/icons/regina/" + name + "-22.png");
    icon.addFile(home() + "/icons/regina/" + name + "-16.png");
    icon.addFile(home() + "/icons/regina/" + name + "-32.png");
    icon.addFile(home() + "/icons/regina/" + name + "-8.png");
    return icon;
}

QIcon ReginaSupport::themeIcon(const QString& name) {
    // TODO: Allow theme.
    QIcon icon(home() + "/icons/oxygen/" + name + "-22.png");
    icon.addFile(home() + "/icons/oxygen/" + name + "-16.png");
    icon.addFile(home() + "/icons/oxygen/" + name + "-32.png");
    icon.addFile(home() + "/icons/oxygen/" + name + "-8.png");
    return icon;
}

