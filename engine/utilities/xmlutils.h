
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file utilities/xmlutils.h
 *  \brief Various classes and routines for XML manipulation, some taken
 *  or modified from the libxml++ library.  The libxml2 library is used
 *  to do most of the underlying work.
 *
 *  The libxml++ library is copyright (C) 2000 by Ari Johnson, and is
 *  covered by the GNU Lesser General Public License.
 */

#ifndef __XMLUTILS_H
#ifndef __DOXYGEN
#define __XMLUTILS_H
#endif

#include <map>
#include <string>
#include <sstream>
#include <libxml/parser.h>
#include "regina-core.h"
#include "utilities/boolset.h"

namespace regina {

/**
 * Various classes and routines for XML manipulation, some taken or
 * modified from the libxml++ library.  The libxml2 library is used
 * to do most of the underlying work.
 *
 * See the xmlutils.h file documentation for libxml++ license details.
 */
namespace xml {

/**
 * \weakgroup utilities
 * @{
 */

class XMLParser;

/**
 * Represents a map from property names to property values.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLPropertyDict : private std::map<std::string, std::string> {
    public:
        /**
         * Create a new map.
         */
        XMLPropertyDict();

        /**
         * Return a value for the given key, or the empty string if the
         * key does not exist in the map.
         *
         * @param key the key to look up.
         * @return the value associated with the given key, or the
         * empty string if the key does not exist in the map.
         */
        const std::string& lookup(const std::string& key) const;

        using std::map<std::string, std::string>::const_iterator;
        using std::map<std::string, std::string>::begin;
        using std::map<std::string, std::string>::end;
        using std::map<std::string, std::string>::find;
        using std::map<std::string, std::string>::operator [];

    private:
        static const std::string empty_;
            /**< An empty string that we can pass by reference. */
};

/**
 * Provides the callbacks for an XMLParser.  The various routines in
 * this class will be called when corresponding elements of the XML file
 * being parsed are encountered.
 *
 * The routines in this class do nothing; you will need to create a
 * derived class that overrides some or all of these routines to do the
 * processing that you require.
 *
 * \ifacespython Not present.
 *
 * @author This class was taken and modified from the libxml++ library
 * (<tt>http://lusis.org/~ari/xml++/</tt>).
 */
class REGINA_API XMLParserCallback {
    public:
        /**
         * Default destructor that does nothing.
         */
        virtual ~XMLParserCallback();

        /**
         * Called at the start of the document.
         *
         * @param parser the XML parser that is currently parsing this document.
         */
        virtual void start_document(XMLParser* parser);
        /**
         * Called when the document is finalised.
         */
        virtual void end_document();
        /**
         * Called when an element's opening tag is encountered.
         *
         * @param n the name of the tag.
         * @param p a dictionary of all the properties of the tag.
         */
        virtual void start_element(const std::string& n,
            const regina::xml::XMLPropertyDict& p);
        /**
         * Called when an element's closing tag is encountered.
         * This is called immediately after start_element() if the
         * opening tag is in <tt>\<tag/\></tt> format.
         *
         * @param n the name of the tag.
         */
        virtual void end_element(const std::string& n);
        /**
         * Called when characters are encountered.
         *
         * @param s the characters encountered.
         */
        virtual void characters(const std::string& s);
        /**
         * Called when a comment is encountered.
         *
         * @param s the comment string.
         */
        virtual void comment(const std::string& s);
        /**
         * Called when a parser warning occurs.
         *
         * @param s the warning message.
         */
        virtual void warning(const std::string& s);
        /**
         * Called when a parser error occurs.
         *
         * @param s the error message.
         */
        virtual void error(const std::string& s);
        /**
         * Called when a parser fatal error occurs.
         *
         * @param s the error message.
         */
        virtual void fatal_error(const std::string& s);
};

/**
 * Used to parse an entire XML file.  When particular XML components are
 * encountered, this will be signalled by calling corresponding routines
 * from the XMLParserCallback that is passed to the XMLParser constructor.
 *
 * To parse an entire XML file, simply call static routine parse_stream(),
 * which does not require you to create an XMLParser yourself.
 *
 * If you desire more fine-grained control over the parsing process, you
 * may create an XMLParser yourself and parse the file manually in small
 * pieces.  To do this, routine parse_chunk() should be called
 * repeatedly with consecutive pieces of the XML file.  Once the entire
 * XML file has been sent through parse_chunk(), routine finish() should
 * be called once to signal that processing is complete.
 *
 * \ifacespython Not present.
 *
 * @author This class was taken and modified from the libxml++ library
 * (<tt>http://lusis.org/~ari/xml++/</tt>).
 */
class REGINA_API XMLParser {
    private:
        XMLParserCallback& _parser_callback;
            /**< Provides the callback routines to use with this parser. */
        xmlParserCtxtPtr _context;
            /**< The parser context to use with the underlying XML library. */

    public:
        /**
         * Creates a new XML parser.
         *
         * @param callback the object providing the routines to call
         * when particular XML components are encountered during parsing.
         */
        XMLParser(XMLParserCallback& callback);
        /**
         * Destroys this XML parser.
         */
        virtual ~XMLParser();

        /**
         * Parses the given chunk of XML.
         *
         * @param s the chunk of XML to parse.
         */
        void parse_chunk(const std::string& s);
        /**
         * Signals that there are no more XML chunks to parse.
         */
        void finish();

        /**
         * Parses an entire XML file.  The given stream will be read
         * from until end-of-file is reached.
         *
         * @param callback the object providing the routines to call
         * when particular XML components are encountered during parsing.
         * @param file the stream from which the raw XML will be read.
         * @param chunkSize the number of characters to read and process
         * at a time (this is the size of each string that will be
         * passed to parse_chunk()).
         */
        static void parse_stream(XMLParserCallback& callback,
            std::istream& file, unsigned chunkSize = 1024);

    private:
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static xmlEntityPtr _get_entity(void *_parser, const xmlChar *n);
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static void _start_document(void *_parser);
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static void _end_document(void *_parser);
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static void _start_element(void *_parser, const xmlChar *n,
            const xmlChar **p);
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static void _end_element(void *_parser, const xmlChar *n);
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static void _characters(void *_parser, const xmlChar *s, int len);
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static void _comment(void *_parser, const xmlChar *s);
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static void _warning(void *_parser, const char *fmt, ...);
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static void _error(void *_parser, const char *fmt, ...);
        /**
         * Link between C callback routine and C++ callback routine.
         */
        static void _fatal_error(void *_parser, const char *fmt, ...);
};

/**
 * Converts the given string from libxml into a C++ string, optionally
 * deallocating the original libxml string.
 *
 * The given libxml string \a str may be \c null, in which case the
 * resulting C++ string will be the empty string.
 *
 * @param str the string to convert.
 * @param free \c true if the original libxml string \a str should be
 * deallocated, or \c false if it should be preserved.
 * @return the given string as a C++ string.
 */
REGINA_API std::string xmlString(xmlChar* str, bool free = true);

/**
 * Returns the given string with special characters converted to XML
 * entities.  For instance, the string <tt>"a \< b"</tt> would be
 * converted to <tt>"a \&lt; b"</tt>.
 *
 * \ifacespython Not present.
 *
 * @param original the string to convert; this string will not be
 * changed.
 * @return the converted string with special characters replaced by
 * XML entities.
 */
REGINA_API std::string xmlEncodeSpecialChars(const std::string& original);

/**
 * Returns the given string encoded so it is suitable for use inside an
 * XML comment.  As well as converting special characters to XML
 * entities, this routine will replace dashes with underscores to avoid
 * double-hyphens (which are illegal in XML comments).
 *
 * \ifacespython Not present.
 *
 * @param comment the string to convert; this string will not be
 * changed.
 * @return the string converted to be usable inside an XML comment.
 */
REGINA_API std::string xmlEncodeComment(const std::string& comment);

/**
 * Returns an XML tag with a single property containing the given value.
 * The tag will be of the form <tt>\<tagName value="..."/\></tt>.
 *
 * The value itself will be written to the tag string using the standard
 * output stream operator \<\<.
 *
 * \pre The property value when written to an output stream does not
 * contain any special characters (such as <tt>\<</tt> or <tt>\&</tt>)
 * that need to be encoded as XML entities.
 *
 * \ifacespython Not present.
 *
 * @param tagName the name of the XML tag to create.
 * @param value the value to assign to the <i>value</i> property of the tag.
 * @return the corresponding XML tag.
 */
template <class T>
inline std::string xmlValueTag(const std::string& tagName, const T& value) {
    std::ostringstream out;
    out << '<' << tagName << " value=\"" << value << "\"/>";
    return out.str();
}

#ifndef __DOXYGEN
    /**
     * Specialisations of xmlValueTag():
     */
    template <>
    inline std::string xmlValueTag<bool>(const std::string& tagName,
            const bool& value) {
        return '<' + tagName + " value=\"" + (value ? 'T' : 'F') + "\"/>";
    }

    template <>
    inline std::string xmlValueTag<BoolSet>(const std::string& tagName,
            const BoolSet& value) {
        return '<' + tagName + " value=\"" +
            (value.hasTrue() ? 'T' : '-') +
            (value.hasFalse() ? 'F' : '-') + "\"/>";
    }
#endif

/*@}*/

// Inline functions for XMLPropertyDict

inline XMLPropertyDict::XMLPropertyDict() {
}

inline const std::string& XMLPropertyDict::lookup(const std::string& key)
        const {
    const_iterator it = find(key);
    return (it == end() ? empty_ : (*it).second);
}

// Inline functions for XMLParserCallback

inline XMLParserCallback::~XMLParserCallback() {
}
inline void XMLParserCallback::start_document(XMLParser*) {
}
inline void XMLParserCallback::end_document() {
}
inline void XMLParserCallback::start_element(const std::string&,
        const regina::xml::XMLPropertyDict&) {
}
inline void XMLParserCallback::end_element(const std::string&) {
}
inline void XMLParserCallback::characters(const std::string&) {
}
inline void XMLParserCallback::comment(const std::string&) {
}
inline void XMLParserCallback::warning(const std::string&) {
}
inline void XMLParserCallback::error(const std::string&) {
}
inline void XMLParserCallback::fatal_error(const std::string&) {
}

// Inline functions for XMLParser

inline XMLParser::~XMLParser() {
    if (_context)
        xmlFreeParserCtxt(_context);
}

inline void XMLParser::parse_chunk(const std::string& s) {
    xmlParseChunk(_context, s.c_str(), static_cast<int>(s.length()), 0);
}

inline void XMLParser::finish() {
    xmlParseChunk(_context, 0, 0, 1);
}

// Inline global functions

inline std::string xmlString(xmlChar* str, bool free) {
    if (! str)
        return std::string();

    std::string ans((const char*)str);
    if (free)
        xmlFree(str);
    return ans;
}

} } // namespace regina::xml

#endif

