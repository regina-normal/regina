
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

#include "packet/nscript.h"
#include "utilities/stringutils.h"

#include "pythonhandler.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../packetfilter.h"

#include <fstream>
#include <QFile>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextStream>

namespace {
    const QString scriptMarker("Regina Script:");
    const QString varMarker("Variable ");
    const QString endMetadataMarker("Begin Script");
}

const PythonHandler PythonHandler::instance;

regina::NPacket* PythonHandler::importData(const QString& fileName,
        QWidget* parentWidget) const {
    QFile f(fileName);
    if (! f.open(QIODevice::ReadOnly)) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("The import failed."), 
            QObject::tr("<qt>I could not read from the file <tt>%1</tt>.</qt>").
                arg(Qt::escape(fileName)));
        return 0;
    }
    QTextStream in(&f);

    in.setCodec(ReginaPrefSet::importExportCodec());

    regina::NScript* ans = new regina::NScript();
    ans->setPacketLabel(QObject::tr("Imported Script").toAscii().constData());

    // Read in the script.
    bool readingMetadata = true;
    QString line, metadata;
    int pos;

    line = in.readLine();
    while (! line.isNull()) {
        if (readingMetadata && line.startsWith("###")) {
            // This is a line of metadata.  Perhaps.
            metadata = line.mid(3).trimmed();
            if (metadata.isEmpty()) {
                // An empty metadata line.
            } else if (metadata.startsWith(scriptMarker)) {
                // The script label.
                metadata = metadata.mid(scriptMarker.length()).
                    trimmed();
                if (! metadata.isEmpty())
                    ans->setPacketLabel(metadata.toAscii().constData());
            } else if (metadata.startsWith(varMarker)) {
                // A script variable.
                metadata = metadata.mid(varMarker.length()).trimmed();
                pos = metadata.indexOf(':');
                if (pos >= 0) {
                    ans->addVariable(
                        metadata.left(pos).trimmed().toAscii().constData(),
                        metadata.mid(pos + 1).trimmed().toAscii().constData());
                } else {
                    // Hmm, it wasn't a script variable after all.
                    readingMetadata = false;
                    ans->addLast(line.toAscii().constData());
                }
            } else if (metadata == endMetadataMarker) {
                // It's the end of the metadata.
                readingMetadata = false;
            } else {
                // It's not metadata at all.
                readingMetadata = false;
                ans->addLast(line.toAscii().constData());
            }
        } else {
            // We're out of the metadata.
            readingMetadata = false;
            ans->addLast(line.toAscii().constData());
        }

        line = in.readLine();
    }

    return ans;
}

PacketFilter* PythonHandler::canExport() const {
    return new SingleTypeFilter<regina::NScript>();
}

bool PythonHandler::exportData(regina::NPacket* data, const QString& fileName,
        QWidget* parentWidget) const {
    regina::NScript* script = dynamic_cast<regina::NScript*>(data);

    QFile f(fileName);
    if (! f.open(QIODevice::WriteOnly)) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("The export failed."), 
            QObject::tr("<qt>I could not write to the file <tt>%1</tt>.</qt>").
                arg(Qt::escape(fileName)));
        return false;
    }
    QTextStream out(&f);

    out.setCodec(ReginaPrefSet::importExportCodec());

    // Write the name of the script.
    out << "### " << scriptMarker << ' ';
    out << QString(script->getPacketLabel().c_str());
    endl(out);
    out << "###";
    endl(out);

    // Output the value of each variable.
    unsigned long i;
    for (i = 0; i < script->getNumberOfVariables(); i++) {
        out << "### " << varMarker << QString(script->getVariableName(i).c_str())
            << ": " << QString(script->getVariableValue(i).c_str());
        endl(out);
    }

    out << "###";
    endl(out);
    out << "### " << endMetadataMarker;
    endl(out);

    for (i = 0; i < script->getNumberOfLines(); i++) {
        out << QString(script->getLine(i).c_str());
        endl(out);
    }

    // All done!
    return true;
}

