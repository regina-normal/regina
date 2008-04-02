
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "packet/nscript.h"
#include "utilities/stringutils.h"

#include "pythonhandler.h"
#include "../packetfilter.h"

#include <fstream>
#include <klocale.h>
#include <kmessagebox.h>
#include <qtextcodec.h>

namespace {
    const QString scriptMarker("Regina Script:");
    const QString varMarker("Variable ");
    const QString endMetadataMarker("Begin Script");
}

const PythonHandler PythonHandler::instance;

regina::NPacket* PythonHandler::import(const QString& fileName,
        QWidget* parentWidget) const {
    return import(fileName, 0, parentWidget);
}

regina::NPacket* PythonHandler::import(const QString& fileName,
        QTextCodec* encoding, QWidget* parentWidget) const {
    std::ifstream in(fileName.ascii());
    if (! in) {
        KMessageBox::error(parentWidget, i18n(
            "The import file %1 could not be read.").arg(fileName));
        return 0;
    }

    regina::NScript* ans = new regina::NScript();
    ans->setPacketLabel(i18n("Imported Script").ascii());

    // Read in the script.
    bool readingMetadata = true;
    std::string rawLine;
    QString line, metadata;
    int pos;

    getline(in, rawLine);
    while (! (rawLine.empty() && in.eof())) {
        if (encoding)
            line = encoding->toUnicode(rawLine.c_str());
        else
            line = rawLine;

        if (readingMetadata && line.startsWith("###")) {
            // This is a line of metadata.  Perhaps.
            metadata = line.mid(3).stripWhiteSpace();
            if (metadata.isEmpty()) {
                // An empty metadata line.
            } else if (metadata.startsWith(scriptMarker)) {
                // The script label.
                metadata = metadata.mid(scriptMarker.length()).
                    stripWhiteSpace();
                if (! metadata.isEmpty())
                    ans->setPacketLabel(metadata);
            } else if (metadata.startsWith(varMarker)) {
                // A script variable.
                metadata = metadata.mid(varMarker.length()).stripWhiteSpace();
                pos = metadata.find(':');
                if (pos >= 0) {
                    ans->addVariable(
                        metadata.left(pos).stripWhiteSpace(),
                        metadata.mid(pos + 1).stripWhiteSpace());
                } else {
                    // Hmm, it wasn't a script variable after all.
                    readingMetadata = false;
                    ans->addLast(line);
                }
            } else if (metadata == endMetadataMarker) {
                // It's the end of the metadata.
                readingMetadata = false;
            } else {
                // It's not metadata at all.
                readingMetadata = false;
                ans->addLast(line);
            }
        } else {
            // We're out of the metadata.
            readingMetadata = false;
            ans->addLast(line);
        }

        rawLine.clear(); // To deal with files with no final newline.
        getline(in, rawLine);
    }

    return ans;
}

PacketFilter* PythonHandler::canExport() const {
    return new SingleTypeFilter<regina::NScript>();
}

bool PythonHandler::exportData(regina::NPacket* data,
        const QString& fileName, QWidget* parentWidget) const {
    regina::NScript* script = dynamic_cast<regina::NScript*>(data);

    std::ofstream out(fileName.ascii());
    if (! out) {
        KMessageBox::error(parentWidget, i18n(
            "The export file %1 could not be written to.").arg(fileName));
        return false;
    }

    // Write the name of the script.
    out << "### " << scriptMarker << ' '
        << script->getPacketLabel() << std::endl;
    out << "###" << std::endl;

    // Output the value of each variable.
    unsigned long i;
    for (i = 0; i < script->getNumberOfVariables(); i++)
        out << "### " << varMarker << script->getVariableName(i)
            << ": " << script->getVariableValue(i) << std::endl;

    out << "###" << std::endl;
    out << "### " << endMetadataMarker << std::endl;

    for (i = 0; i < script->getNumberOfLines(); i++)
        out << script->getLine(i) << std::endl;

    // All done!
    return true;
}

