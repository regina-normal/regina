
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

namespace {
    const std::string scriptMarker("Regina Script:");
    const std::string varMarker("Variable ");
    const std::string endMetadataMarker("Begin Script");
}

const PythonHandler PythonHandler::instance;

regina::NPacket* PythonHandler::import(const QString& fileName,
        QWidget* parentWidget) const {
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
    std::string line, metadata;
    std::string::size_type pos;

    getline(in, line);
    while (! (line.empty() && in.eof())) {
        if (readingMetadata && regina::startsWith(line, "###")) {
            // This is a line of metadata.  Perhaps.
            metadata = regina::stripWhitespace(line.substr(3));
            if (metadata.empty()) {
                // An empty metadata line.
            } else if (regina::startsWith(metadata, scriptMarker)) {
                // The script label.
                metadata = regina::stripWhitespace(metadata.substr(
                    scriptMarker.length()));
                if (! metadata.empty())
                    ans->setPacketLabel(metadata);
            } else if (regina::startsWith(metadata, varMarker)) {
                // A script variable.
                metadata = regina::stripWhitespace(metadata.substr(
                    varMarker.length()));
                pos = metadata.find(':');
                if (pos < metadata.length()) {
                    ans->addVariable(
                        regina::stripWhitespace(metadata.substr(0, pos)),
                        regina::stripWhitespace(metadata.substr(pos + 1)));
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

        line.clear(); // To deal with files with no final newline.
        getline(in, line);
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

