
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

#include "foreign/nsnappea.h"
#include "triangulation/ntriangulation.h"

#include "snappea.h"

#include <klocale.h>
#include <kmessagebox.h>

regina::NPacket* SnapPeaImporter::import(const QString& fileName,
        QWidget* parentWidget) {
    regina::NPacket* ans = regina::readSnapPea(fileName.ascii());
    if (! ans)
        KMessageBox::error(parentWidget, i18n(
            "The SnapPea file %1 could not be imported.  Perhaps the data "
            "is not in SnapPea format?").arg(fileName));
    return ans;
}

