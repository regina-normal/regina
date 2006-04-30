
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

#include "foreign/orb.h"
#include "triangulation/ntriangulation.h"

#include "orbhandler.h"
#include "../packetfilter.h"

#include <klocale.h>
#include <kmessagebox.h>

const OrbHandler OrbHandler::instance;

regina::NPacket* OrbHandler::import(const QString& fileName,
        QWidget* parentWidget) const {
    regina::NPacket* ans = regina::readOrb(fileName.ascii());
    if (! ans)
        KMessageBox::error(parentWidget, i18n(
            "The Orb / Casson file %1 could not be imported.  Perhaps the "
            "data is not in Orb format?").arg(fileName));
    return ans;
}

