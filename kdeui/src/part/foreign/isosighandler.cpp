
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

#include "foreign/isosig.h"
#include "packet/ncontainer.h"
#include "packet/ntext.h"

#include "isosighandler.h"
#include "../packetfilter.h"

#include <klocale.h>
#include <kmessagebox.h>
#include <qfile.h>

const IsoSigHandler IsoSigHandler::instance3(3);
const IsoSigHandler IsoSigHandler::instance4(4);

regina::NPacket* IsoSigHandler::import(const QString& fileName,
        QWidget* parentWidget) const {
    QString explnSuffix = i18n("<p>The file should be a plain text file "
        "containing one %1-manifold triangulation isomorphism signature "
        "per line.</p></qt>").arg(dimension_);

    regina::NPacket* ans = regina::readIsoSigList(
        static_cast<const char*>(QFile::encodeName(fileName)), dimension_);
    if (! ans) {
        KMessageBox::error(parentWidget, i18n("<qt>An error occurred "
            "whilst attempting to read from the file %1.").arg(fileName) +
            explnSuffix);
        return 0;
    }

    regina::NPacket* last = ans->getLastTreeChild();
    if (last == 0) {
        KMessageBox::error(parentWidget, i18n("<qt>The selected file does "
            "not contain any isomorphism signatures.") + explnSuffix);
        return 0;
    } else if (last->getPacketType() == regina::NText::packetType) {
        if (last == ans->getFirstTreeChild()) {
            KMessageBox::error(parentWidget, i18n("<qt>None of the "
                "isomorphism signatures found in the selected file could "
                "be interpreted.") + explnSuffix);
            return 0;
        } else {
            KMessageBox::error(parentWidget, i18n("<qt>One or more of the "
                "isomorphism signatures could not be interpreted.  Details "
                "of the error(s) can be found in the final text packet "
                "beneath the newly imported tree.") +
                explnSuffix);
        }
    }

    // All worked out okay.
    ans->setPacketLabel(i18n("Imported Triangulations").ascii());
    return ans;
}

