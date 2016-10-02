
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

#include "packet/script.h"
#include "utilities/stringutils.h"

#include "pythonhandler.h"
#include "reginamain.h"
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

regina::Packet* PythonHandler::importData(const QString& fileName,
        ReginaMain* parentWidget) const {
    QFile f(fileName);
    if (! f.open(QIODevice::ReadOnly)) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("The import failed."), 
            QObject::tr("<qt>I could not read from the file <tt>%1</tt>.</qt>").
                arg(fileName.toHtmlEscaped()));
        return 0;
    }
    QTextStream in(&f);

    in.setCodec(ReginaPrefSet::importExportCodec());

    regina::Script* ans = new regina::Script();
    ans->setLabel(QObject::tr("Imported Script").toUtf8().constData());

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
                    ans->setLabel(metadata.toUtf8().constData());
            } else if (metadata.startsWith(varMarker)) {
                // A script variable.
                metadata = metadata.mid(varMarker.length()).trimmed();
                pos = metadata.indexOf(':');
                if (pos >= 0) {
                    ans->addVariable(
                        metadata.left(pos).trimmed().toUtf8().constData(),
                        parentWidget->getPacketTree()->findPacketLabel(
                            metadata.mid(pos + 1).trimmed().
                            toUtf8().constData()));
                } else {
                    // Hmm, it wasn't a script variable after all.
                    readingMetadata = false;
                    ans->append(line.toUtf8().constData());
                    ans->append("\n");
                }
            } else if (metadata == endMetadataMarker) {
                // It's the end of the metadata.
                readingMetadata = false;
            } else {
                // It's not metadata at all.
                readingMetadata = false;
                ans->append(line.toUtf8().constData());
                ans->append("\n");
            }
        } else {
            // We're out of the metadata.
            readingMetadata = false;
            ans->append(line.toUtf8().constData());
            ans->append("\n");
        }

        line = in.readLine();
    }

    return ans;
}

PacketFilter* PythonHandler::canExport() const {
    return new SingleTypeFilter<regina::Script>();
}

bool PythonHandler::exportData(regina::Packet* data, const QString& fileName,
        QWidget* parentWidget) const {
    regina::Script* script = dynamic_cast<regina::Script*>(data);

    QFile f(fileName);
    if (! f.open(QIODevice::WriteOnly)) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("The export failed."), 
            QObject::tr("<qt>I could not write to the file <tt>%1</tt>.</qt>").
                arg(fileName.toHtmlEscaped()));
        return false;
    }
    QTextStream out(&f);

    out.setCodec(ReginaPrefSet::importExportCodec());

    // Write the name of the script.
    out << "### " << scriptMarker << ' ';
    out << QString(script->label().c_str());
    endl(out);
    out << "###";
    endl(out);

    // Output the value of each variable.
    unsigned long i;
    regina::Packet* value;
    for (i = 0; i < script->countVariables(); i++) {
        value = script->variableValue(i);
        out << "### " << varMarker
            << QString(script->variableName(i).c_str())
            << ": " << (value ? QString(value->label().c_str()) : "");
        endl(out);
    }

    out << "###";
    endl(out);
    out << "### " << endMetadataMarker;
    endl(out);
    out << script->text().c_str();

    // All done!
    return true;
}

