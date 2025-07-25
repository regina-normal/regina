
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "packet/attachment.h"

#include "attachmenthandler.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "../packetfilter.h"

#include <QFile>
#include <QTextDocument>

const AttachmentHandler AttachmentHandler::instance;

std::shared_ptr<regina::Packet> AttachmentHandler::importData(
        const QString& fileName, ReginaMain* parentWidget) const {
    std::shared_ptr<regina::Attachment> ans =
        std::make_shared<regina::Attachment>(
        static_cast<const char*>(QFile::encodeName(fileName)));
    if (ans->isNull()) {
        ReginaSupport::sorry(parentWidget,
            QObject::tr("The import failed."),
            QObject::tr("<qt>Please check that the file <tt>%1</tt> "
            "is readable and non-empty.</qt>").arg(fileName.toHtmlEscaped()));
        return nullptr;
    } else
        ans->setLabel(ans->filename());
    return ans;
}

