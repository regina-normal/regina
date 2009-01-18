
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

#include "foreign/pdf.h"
#include "packet/npdf.h"

#include "pdfhandler.h"
#include "../packetfilter.h"

#include <klocale.h>
#include <kmessagebox.h>
#include <qfile.h>

const PDFHandler PDFHandler::instance;

regina::NPacket* PDFHandler::import(const QString& fileName,
        QWidget* parentWidget) const {
    regina::NPacket* ans = regina::readPDF(
        static_cast<const char*>(QFile::encodeName(fileName)));
    if (! ans)
        KMessageBox::error(parentWidget, i18n(
            "The PDF document %1 could not be read.").arg(fileName));
    ans->setPacketLabel(i18n("PDF document").ascii());
    return ans;
}

PacketFilter* PDFHandler::canExport() const {
    return new SingleTypeFilter<regina::NPDF>();
}

bool PDFHandler::exportData(regina::NPacket* data, const QString& fileName,
        QWidget* parentWidget) const {
    regina::NPDF* pdf = dynamic_cast<regina::NPDF*>(data);
    if (! pdf->data()) {
        KMessageBox::error(parentWidget, i18n(
            "This PDF packet is empty, and so cannot be exported."));
        return false;
    }
    if (! regina::writePDF(
            static_cast<const char*>(QFile::encodeName(fileName)), *pdf)) {
        KMessageBox::error(parentWidget, i18n(
            "The PDF document %1 could not be saved.").arg(fileName));
        return false;
    }
    return true;
}

