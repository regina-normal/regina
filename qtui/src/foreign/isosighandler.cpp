
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

#include "foreign/isosig.h"
#include "packet/container.h"
#include "packet/text.h"

#include "isosighandler.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "../packetfilter.h"

#include <QFile>
#include <QTextDocument>

const IsoSigHandler IsoSigHandler::instance2(2);
const IsoSigHandler IsoSigHandler::instance3(3);
const IsoSigHandler IsoSigHandler::instance4(4);

regina::Packet* IsoSigHandler::importData(const QString& fileName,
        ReginaMain* parentWidget) const {
    QString explnSuffix = QObject::tr("<p>The file should be a plain text file "
        "containing one %1-manifold isomorphism signature per line.  "
        "Isomorphism signatures are described in detail in "
        "<i>Simplification paths in the Pachner graphs "
        "of closed orientable 3-manifold triangulations</i>, "
        "Burton, 2011, <tt>arXiv:1110.6080</tt>.</qt>").arg(dimension_);

    regina::Packet* ans = regina::readIsoSigList(
        static_cast<const char*>(QFile::encodeName(fileName)), dimension_);
    if (! ans) {
        ReginaSupport::sorry(parentWidget,
            QObject::tr("The import failed."),
            QObject::tr("<qt>I could not open the file <tt>%1</tt>.  "
                "Please check that this file is readable.</qt>")
                .arg(fileName.toHtmlEscaped()));
        return 0;
    }

    regina::Packet* last = ans->lastChild();
    if (last == 0) {
        ReginaSupport::sorry(parentWidget,
            QObject::tr("The import failed."),
            QObject::tr("<qt>The selected file does "
            "not contain any isomorphism signatures.") + explnSuffix);
        delete ans;
        return 0;
    } else if (last->type() == regina::PACKET_TEXT) {
        if (last == ans->firstChild()) {
            ReginaSupport::sorry(parentWidget, 
                QObject::tr("The import failed."),
                QObject::tr("<qt>None of the lines in the selected file "
                "could be interpreted as isomorphism signatures.")
                + explnSuffix);
            delete ans;
            return 0;
        } else {
            ReginaSupport::warn(parentWidget, 
                QObject::tr("There were problems with the import."),
                QObject::tr("<qt>One or more lines in the selected file "
                "could not be interpreted as isomorphism signatures. "
                "For details, see the text packet "
                "at the end of the imported packet list.")
                + explnSuffix);
        }
    }

    // All worked out okay.
    ans->setLabel(
        QObject::tr("Imported Triangulations").toUtf8().constData());
    return ans;
}

