
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file utilities/xmlwriter.h
 *  \brief A general infrastructure for writing packets in XML.
 */

#ifndef __REGINA_XMLWRITER_H
#ifndef __DOXYGEN
#define __REGINA_XMLWRITER_H
#endif

#include "regina-core.h"
#include "file/fileformat.h"
#include "maths/perm.h"
#include "packet/packet.h" // TODO: Remove

namespace regina {

template <int> class Triangulation;

/**
 * Used to write one of Regina's objects to XML.
 *
 * By calling openPre(), openPost(), writeContent() and close() in turn,
 * this object should output a single XML element (typically with
 * internal character data and/or child elements) that represents a
 * single Regina object of type \a T.
 *
 * For types that can be enclosed in a PacketOf<...> wrapper, \a T should be
 * the underlying mathematical type (so, for example, for links the type
 * \a T should be Link, not PacketOf<Link>).
 *
 * Most functions in this template class have no default implementations.
 * TODO: Talk about implementations.
 */
template <typename T>
class XMLWriter {
    private:
        const T& data_;
            /**< The object to be written in XML. */
        std::ostream& out_;
            /**< The output stream to which the XML will be written. */
        FileFormat format_;
            /**< Indicates which of Regina's XML file formats to use. */

    public:
        /**
         * Creates a new writer that will write the given object as an
         * XML fragment.
         *
         * @param data the object to be written in XML.
         * @param out the output stream to which the XML will be written.
         * @param format indicates which of Regina's XML file formats to use.
         */
        XMLWriter(const T& data, std::ostream& out, FileFormat format);

        /**
         * Writes the beginning of the opening XML element tag for the object
         * being written.
         *
         * This will output an XML snippet of the form
         * <tt>\<foo bar="..." baz="..."</tt>, \e without the closing angle
         * brackets.  This means that the caller can inject additional
         * attributes into the opening XML element tag simply by writing
         * them to the output stream (with a preceding space) after the
         * call to openPre().
         *
         * Once the caller has injected any additional attributes, it
         * should call openPost() to write the closing angle bracket.
         */
        void openPre();

        /**
         * Writes the end of the opening XML element tag for the object
         * being written.
         *
         * In most cases this is simply a closing angle bracket followed
         * by a newline.  See openPre() for further details.
         */
        void openPost();

        /**
         * Writes the main content for the object being written.
         *
         * This should include any character data and/or child elements
         * that are specific to the object type.
         *
         * It should not include general packet tree infrastructure,
         * such as packet tags and/or child packets.
         */
        void writeContent();

        /**
         * Writes the closing XML element tag for the object being written.
         */
        void close();
};

template <int dim>
class XMLWriter<Triangulation<dim>> {
    private:
        static constexpr bool useSnIndex =
            (Perm<dim + 1>::codeType == PERM_CODE_INDEX);

        const Triangulation<dim>& data_;
        std::ostream& out_;
        FileFormat format_;

    public:
        XMLWriter(const Triangulation<dim>& data, std::ostream& out,
            FileFormat format);

        void openPre();
        void openPost();
        void writeContent();
        void close();
};

template <typename T>
XMLWriter<T>::XMLWriter(const T& data, std::ostream& out, FileFormat format) :
        data_(data), out_(out), format_(format) {
}

template <typename T>
void XMLWriter<T>::openPost() {
    out_ << ">\n";
}

template <int dim>
XMLWriter<Triangulation<dim>>::XMLWriter(const Triangulation<dim>& data,
        std::ostream& out, FileFormat format) :
        data_(data), out_(out), format_(format) {
}

template <int dim>
void XMLWriter<Triangulation<dim>>::openPost() {
    out_ << ">\n";
}

} // namespace regina

#endif

