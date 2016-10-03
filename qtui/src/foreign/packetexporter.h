
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

/*! \file packetexporter.h
 *  \brief Provides a basic infrastructure for exporting packets or
 *  packet subtrees to foreign file formats.
 */

#ifndef __PACKETEXPORTER_H
#define __PACKETEXPORTER_H

class PacketFilter;
class QString;
class QTextCodec;
class QWidget;

namespace regina {
    class Packet;
};

/**
 * An object responsible for exporting a packet or packet subtree to a
 * foreign file format.  Different foreign file formats should correspond
 * to different subclasses of PacketExporter.
 */
class PacketExporter {
    public:
        /**
         * Default destructor that does nothing.
         */
        virtual ~PacketExporter();

        /**
         * Returns a newly created packet filter describing which
         * packets can be exported by this subclass of PacketExporter.
         *
         * The caller of this routine is responsible for destroying the
         * new filter.
         */
        virtual PacketFilter* canExport() const = 0;

        /**
         * Export a packet or packet subtree to the given file.
         * If a text encoding is required, the preferred codec
         * ReginaPrefSet::importExportCodec() should be used.
         *
         * This routine should return \c true if and only if the export
         * was successful.  If the export was unsuccessful, an
         * appropriate error should be displayed to the user (with the
         * argument \a parentWidget as the parent widget of the message
         * box).
         */
        virtual bool exportData(regina::Packet* data, const QString& fileName,
            QWidget* parentWidget) const = 0;

        /**
         * Should the GUI inform the user that their preferred codec
         * will be used?
         *
         * The default implementation returns \c false.
         */
        virtual bool useExportEncoding() const;
};

inline PacketExporter::~PacketExporter() {
}

inline bool PacketExporter::useExportEncoding() const {
    return false;
}

#endif
