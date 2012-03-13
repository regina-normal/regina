
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

#include "foreign/dehydration.h"
#include "packet/ncontainer.h"
#include "packet/ntext.h"

#include "dehydrationhandler.h"
#include "reginasupport.h"
#include "../packetfilter.h"

#include <QFile>
#include <QTextDocument>

const DehydrationHandler DehydrationHandler::instance;

regina::NPacket* DehydrationHandler::importData(const QString& fileName,
        QWidget* parentWidget) const {
    QString explnSuffix = QObject::tr("<p>The file should be a plain text "
        "file containing one dehydration string per line.  "
        "Dehydration strings are described in detail in "
        "<i>A census of cusped hyperbolic 3-manifolds</i>, "
        "Callahan, Hildebrand and Weeks, published in "
        "<i>Mathematics of Computation</i> <b>68</b>, 1999.</qt>");

    regina::NPacket* ans = regina::readDehydrationList(
        static_cast<const char*>(QFile::encodeName(fileName)));
    if (! ans) {
        ReginaSupport::sorry(parentWidget,
            QObject::tr("The import failed."),
            QObject::tr("<qt>I could not open the file <tt>%1</tt>.  "
                "Please check that this file is readable.</qt>")
                .arg(Qt::escape(fileName)));
        return 0;
    }

    regina::NPacket* last = ans->getLastTreeChild();
    if (last == 0) {
        ReginaSupport::sorry(parentWidget,
            QObject::tr("The import failed."),
            QObject::tr("<qt>The selected file does not contain any "
                "dehydration strings.") + explnSuffix);
        delete ans;
        return 0;
    } else if (last->getPacketType() == regina::NText::packetType) {
        if (last == ans->getFirstTreeChild()) {
            ReginaSupport::sorry(parentWidget, 
                QObject::tr("The import failed."),
                QObject::tr("<qt>None of the lines in the selected file "
                "could be interpreted as dehydration strings.")
                + explnSuffix);
            delete ans;
            return 0;
        } else {
            ReginaSupport::warn(parentWidget, 
                QObject::tr("There were problems with the import."),
                QObject::tr("<qt>One or more lines in the selected file "
                "could not be interpreted as dehydration strings. "
                "For details, see the text packet "
                "at the end of the imported packet list.")
                + explnSuffix);
        }
    }

    // All worked out okay.
    ans->setPacketLabel(QObject::tr("Rehydrated Triangulations").
        toAscii().constData());
    return ans;
}

