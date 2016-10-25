
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

#include "triangulation/dim3.h"

#include "reginaprefset.h"
#include "reginasupport.h"
#include "sourcehandler.h"
#include "../packetfilter.h"

#include <fstream>
#include <QFile>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextStream>

const SourceHandler SourceHandler::instance;

PacketFilter* SourceHandler::canExport() const {
    return new SubclassFilter<regina::Triangulation<3>>();
}

bool SourceHandler::exportData(regina::Packet* data, const QString& fileName,
        QWidget* parentWidget) const {
    regina::Triangulation<3>* tri = dynamic_cast<regina::Triangulation<3>*>(data);

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

    out << tri->dumpConstruction().c_str();
    return true;
}

