
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

namespace regina {

class AngleStructures;
class NormalHypersurfaces;
class NormalSurfaces;
template <int> class Triangulation;

/**
 * Provides XMLWriter<T> with the class constant requiresTriangulation,
 * for the case where a Regina object of type \a T does not require its
 * underlying triangulation to be written to file before the object
 * itself is written.
 *
 * See the XMLWriter template class for further details.
 */
class XMLWriterRequiresNoTriangulation {
    public:
        /**
         * Indicates whether a Regina object of type \a T requires its
         * underlying triangulation to be written to file before the
         * object itself is written.
         */
        static constexpr bool requiresTriangulation = false;
};

/**
 * Provides XMLWriter<T> with the class constant requiresTriangulation,
 * plus other triangulation-related constants and functions, for the case
 * where a Regina object of type \a T requires its underlying triangulation
 * to be written to file before the object itself is written.
 *
 * See the XMLWriter template class for further details.
 */
template <typename T>
class XMLWriterRequiresTriangulation {
    public:
        /**
         * Indicates whether a Regina object of type \a T requires its
         * underlying triangulation to be written to file before the
         * object itself is written.
         */
        static constexpr bool requiresTriangulation = true;
        /**
         * The dimension of the underlying triangulation for a Regina
         * object of type \a T.
         */
        static constexpr int dimension = std::remove_reference<
            decltype(std::declval<T>().triangulation())>::type::dimension;

    protected:
        std::string triID_;
            /**< The packet ID for the underlying triangulation.  This will
                 be the empty string until wroteTriangulationID() is called. */

    public:
        /**
         * Informs XMLWriter<T> that the underlying triangulation has
         * been written to file, and that its packet ID is \a id.
         *
         * This saves \a id in the field triID_, where XMLWriter<T> can
         * then access it.
         */
        void wroteTriangulationID(std::string id) { triID_ = std::move(id); }
};

/**
 * Used to write one of Regina's objects to XML.  This is typically used
 * by wrapped packets: the XML output code for PacketOf<T> uses XMLWriter<T>
 * to do the "real" work.
 *
 * By calling openPre(), openPost(), writeContent() and close() in turn,
 * this object should output a single XML element (typically with
 * internal character data and/or child elements) that represents a
 * single Regina object of type \a T.
 *
 * Some of Regina's data types (e.g., normal surface/hypersurface lists
 * and angle structure lists) require the underlying triangulation to be
 * written to file beforehand.  The user of this class is responsible for
 * making sure that this happens - specifically, this means:
 *
 * - you should check the class constant XMLWriter<T>::requiresTriangulation
 *   to determine if the triangulation might need to be written;
 *
 * - if so, you should ensure that the triangulation has been written
 *   and then call XMLWriter<T>::wroteTriangulation(id) to communicate
 *   back to this class the corresponding packet ID;
 *
 * - only then can you call openPre() to begin writing this object to file.
 *
 * The generic implementation of PacketOf<T>::writeXMLPacketData()
 * handles all of this correctly.  The constant requiresTriangulation
 * and the function wroteTriangulation() are inherited through
 * the helper class XMLWriterRequiresTriangulation<T>.
 *
 * For types that can be enclosed in a PacketOf<...> wrapper, \a T should be
 * the underlying mathematical type (so, for example, for links the type
 * \a T should be Link, not PacketOf<Link>).
 *
 * Most functions in this template class have no default implementations,
 * and instead require a specialisation for each type \a T that is to be
 * supported.  The only exceptions are the class constructor and openPost().
 */
template <typename T>
class XMLWriter :
        public std::conditional_t<
                std::is_same_v<T, AngleStructures> ||
                std::is_same_v<T, NormalHypersurfaces> ||
                std::is_same_v<T, NormalSurfaces>,
            XMLWriterRequiresTriangulation<T>,
            XMLWriterRequiresNoTriangulation> {
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
         * This constructor has a default implementation, which simply
         * stores the given arguments in the corresponding member variables.
         *
         * \param data the object to be written in XML.
         * \param out the output stream to which the XML will be written.
         * \param format indicates which of Regina's XML file formats to use.
         */
        XMLWriter(const T& data, std::ostream& out, FileFormat format);

        /**
         * Writes the beginning of the opening XML element tag for the object
         * being written.
         *
         * This will output an XML snippet of the form
         * `<foo bar="..." baz="..."`, _without_ the closing angle
         * brackets.  This means that the caller can inject additional
         * attributes into the opening XML element tag simply by writing
         * them to the output stream (with a preceding space) after the
         * call to openPre().
         *
         * Once the caller has injected any additional attributes, it
         * should call openPost() to write the closing angle bracket.
         *
         * This function has no default implementation, and must be
         * specialised for each support type \a T.
         */
        void openPre();

        /**
         * Writes the end of the opening XML element tag for the object
         * being written.
         *
         * In most cases this is simply a closing angle bracket followed
         * by a newline.  See openPre() for further details.
         *
         * This function has a default implementation, which simply
         * writes a closing angle bracket and a newline.  You may still
         * specialise this if needed (e.g., if writing a newline is not
         * appropriate for this particular type \a T).
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
         *
         * This function has no default implementation, and must be
         * specialised for each support type \a T.
         */
        void writeContent();

        /**
         * Writes the closing XML element tag for the object being written.
         *
         * This function has no default implementation, and must be
         * specialised for each support type \a T.
         */
        void close();
};

template <int dim>
class XMLWriter<Triangulation<dim>> {
    public:
        static constexpr bool requiresTriangulation = false;

    private:
        static constexpr bool useSnIndex =
            (Perm<dim + 1>::codeType == PermCodeType::Index);

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

