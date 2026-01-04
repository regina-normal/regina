
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

/*! \file sighandler.h
 *  \brief Allows interaction with knot and isomorphism signature lists.
 */

#ifndef __SIGHANDLER_H
#define __SIGHANDLER_H

#include "foreign/isosig.h"
#include "packetimporter.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "../packetfilter.h"

#include <QFile>
#include <QTextDocument>

namespace regina {
    class Link;
    template <int dim> requires (supportedDim(dim)) class Triangulation;
}

/**
 * An object responsible for importing data from
 * knot signature or isomorphism signature lists.
 *
 * Rather than creating new objects of this class, the globally
 * available object SigHandler<PacketType>::instance should always be used.
 *
 * \tparam PacketType Indicates which types of signatures to import.
 * This must be either Link (indicating knot signatures), or one of the
 * Triangulation<dim> classes (indicating isomorphism signatures for
 * <i>dim</i>-dimensional triangulations).
 */
template <typename PacketType>
class SigHandler : public PacketImporter {
    using PacketImporter::importData;

    public:
        /**
         * A globally available instance of this class.
         */
        static const SigHandler instance;

    public:
        /**
         * PacketImporter overrides:
         */
        std::shared_ptr<regina::Packet> importData(const QString& fileName,
            ReginaMain* parentWidget) const override;

    private:
        /**
         * Don't allow people to construct their own handlers.
         */
        SigHandler() = default;
};

template <typename PacketType>
const SigHandler<PacketType> SigHandler<PacketType>::instance;

template <typename PacketType>
std::shared_ptr<regina::Packet> SigHandler<PacketType>::importData(
        const QString& fileName, ReginaMain* parentWidget) const {
    QString explnSuffix;
    QString signatures;
    if constexpr (std::is_same_v<PacketType, regina::Link>) {
        explnSuffix = QObject::tr("<p>The file should be a plain text file "
            "containing one knot signature per line.</qt>");
        signatures = QObject::tr("knot signatures");
    } else {
        explnSuffix = QObject::tr("<p>The file should be a plain text file "
            "containing one %1-manifold isomorphism signature per line.  "
            "Isomorphism signatures are described in detail in "
            "<i>Simplification paths in the Pachner graphs "
            "of closed orientable 3-manifold triangulations</i>, "
            "Burton, 2011, <tt>arXiv:1110.6080</tt>.</qt>")
            .arg(PacketType::dimension);
        signatures = QObject::tr("isomorphism signatures");
    }

    std::shared_ptr<regina::Packet> ans = regina::readSigList<PacketType>(
        static_cast<const char*>(QFile::encodeName(fileName)));
    if (! ans) {
        ReginaSupport::sorry(parentWidget,
            QObject::tr("The import failed."),
            QObject::tr("<qt>I could not open the file <tt>%1</tt>.  "
                "Please check that this file is readable.</qt>")
                .arg(fileName.toHtmlEscaped()));
        return nullptr;
    }

    std::shared_ptr<regina::Packet> last = ans->lastChild();
    if (! last) {
        ReginaSupport::sorry(parentWidget,
            QObject::tr("The import failed."),
            QObject::tr("<qt>The selected file does "
            "not contain any %1.").arg(signatures) + explnSuffix);
        return nullptr;
    } else if (last->type() == regina::PacketType::Text) {
        if (last == ans->firstChild()) {
            ReginaSupport::sorry(parentWidget, 
                QObject::tr("The import failed."),
                QObject::tr("<qt>None of the lines in the selected file "
                "could be interpreted as %1.").arg(signatures)
                + explnSuffix);
            return nullptr;
        } else {
            ReginaSupport::warn(parentWidget, 
                QObject::tr("There were problems with the import."),
                QObject::tr("<qt>One or more lines in the selected file "
                "could not be interpreted as %1. "
                "For details, see the text packet "
                "at the end of the imported packet list.").arg(signatures)
                + explnSuffix);
        }
    }

    // All worked out okay.
    if constexpr (std::is_same_v<PacketType, regina::Link>) {
        ans->setLabel(
            QObject::tr("Imported Knots").toUtf8().constData());
    } else {
        ans->setLabel(
            QObject::tr("Imported Triangulations").toUtf8().constData());
    }
    return ans;
}

#endif
