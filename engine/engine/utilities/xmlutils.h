
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file xmlutils.h
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

#include <string>
#include <sstream>
#include <libxml/parser.h>
#include "utilities/nbooleans.h"
#include "utilities/hashmap.h"
#include "utilities/hashutils.h"

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

/**
 * Represents a hashed map from property names to property values.
 *
 * \ifaces Not present.
 */
class XMLPropertyDict : public stdhash::hash_map<std::string,
        std::string, HashString> {
    public:
        /**
         * Create a new hashed map.
         */
        XMLPropertyDict();

        /**
         * Return a value for the given key, even if the key does not
         * exist in the hashed map.
         *
         * @param key the key to look up.
         * @param defaultVal the value to return if the key does not exist.
         * @return the value associated with the given key, or parameter
         * \a default if the key does not exist in the hashed map.
         */
        const std::string& lookup(const std::string& key,
            const std::string& defaultVal = std::string()) const;
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
 * \ifaces Not present.
 *
 * @author This class was taken and modified from the libxml++ library
 * (<tt>http://lusis.org/~ari/xml++/</tt>).
 */
class XMLParserCallback {
    public:
        /**
         * Called at the start of the document.
         */
        virtual void start_document();
        /**
         * Called when the document is finalised.
         */
        virtual void end_document();
        /**
         * Called when an element's opening tag is encountered.
         *
         * @param n the name of the tag.
         * @param p a hashed dictionary of all the properties of the tag.
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
 * \ifaces Not present.
 *
 * @author This class was taken and modified from the libxml++ library
 * (<tt>http://lusis.org/~ari/xml++/</tt>).
 */
class XMLParser {
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
 * Returns the given string with special characters converted to XML
 * entities.  For instance, the string <tt>"a \< b"</tt> would be
 * converted to <tt>"a \&lt; b"</tt>.
 *
 * \ifaces Not present.
 *
 * @param original the string to convert; this string will not be
 * changed.
 * @return the converted string with special characters replaced by
 * XML entities.
 */
std::string xmlEncodeSpecialChars(const std::string& original);

/**
 * Returns the given string encoded so it is suitable for use inside an
 * XML comment.  As well as converting special characters to XML
 * entities, this routine will replace dashes with underscores to avoid
 * double-hyphens (which are illegal in XML comments).
 *
 * \ifaces Not present.
 *
 * @param comment the string to convert; this string will not be
 * changed.
 * @return the string converted to be usable inside an XML comment.
 */
std::string xmlEncodeComment(const std::string& comment);

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
    inline std::string xmlValueTag<NBoolSet>(const std::string& tagName,
            const NBoolSet& value) {
        return '<' + tagName + " value=\"" +
            (value.hasTrue() ? 'T' : '-') +
            (value.hasFalse() ? 'F' : '-') + "\"/>";
    }
#endif

/*@}*/

// Inline functions for XMLPropertyDict

inline XMLPropertyDict::XMLPropertyDict() {
}

inline const std::string& XMLPropertyDict::lookup(const std::string& key,
        const std::string& defaultVal) const {
    const_iterator it = find(key);
    return (it == end() ? defaultVal : (*it).second);
}

// Inline functions for XMLParserCallback

inline void XMLParserCallback::start_document() {
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
    xmlParseChunk(_context, s.c_str(), s.length(), 0);
}

inline void XMLParser::finish() {
    xmlParseChunk(_context, 0, 0, 1);
}

} } // namespace regina::xml

#endif

