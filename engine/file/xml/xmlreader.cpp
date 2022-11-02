
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include <cstdarg>
#include <iostream>
#include <libxml/parserInternals.h>
#include "file/xml/xmlreader.h"

#define ERROR_BUFF_SIZE 1024

namespace regina::xml {

const std::string XMLPropertyDict::empty_;

XMLParser::XMLParser(XMLParserCallback& callback) : _parser_callback(callback) {
    xmlSAXHandler sax_handler = {
        nullptr,            // internalSubset
        nullptr,            // isStandalone
        nullptr,            // hasInternalSubset
        nullptr,            // hasExternalSubset
        nullptr,            // resolveEntity
        _get_entity,        // getEntity
        nullptr,            // entityDecl
        nullptr,            // notationDecl
        nullptr,            // attributeDecl
        nullptr,            // elementDecl
        nullptr,            // unparsedEntityDecl
        nullptr,            // setDocumentLocator
        _start_document,    // startDocument
        _end_document,      // endDocument
        _start_element,     // startElement
        _end_element,       // endElement
        nullptr,            // reference
        _characters,        // characters
        nullptr,            // ignorableWhitespace
        nullptr,            // processingInstruction
        _comment,           // comment
        _warning,           // warning
        _error,             // error
        _fatal_error,       // fatalError
        nullptr,            // getParameterEntity
        nullptr,            // cdataBlock
        nullptr,            // externalSubset
        1                   // initialized
    };

    _context = xmlCreatePushParserCtxt(&sax_handler, this, nullptr, 0, nullptr);

    // Ensure that entities in attributes are replaced.
    _context->replaceEntities = 1;
}

xmlEntityPtr XMLParser::_get_entity(void*, const xmlChar* n) {
    return xmlGetPredefinedEntity(n);
}
void XMLParser::_start_document(void* parser) {
    static_cast<XMLParser*>(parser)->_parser_callback.start_document(
        static_cast<XMLParser*>(parser));
}
void XMLParser::_end_document(void* parser) {
    static_cast<XMLParser*>(parser)->_parser_callback.end_document();
}
void XMLParser::_start_element(void* parser, const xmlChar* n,
        const xmlChar** p) {
    XMLPropertyDict properties;

    if (p) {
        const xmlChar** cur = p;
        const char* name;
        const char* value;
        while (cur && *cur) {
            name = (const char*)(*cur++);
            value = (const char*)(*cur++);
            properties[name] = value;
        }
    }

    static_cast<XMLParser*>(parser)->_parser_callback.start_element(
        (const char*)(n), properties);
}
void XMLParser::_end_element(void* parser, const xmlChar* n) {
    static_cast<XMLParser*>(parser)->_parser_callback.end_element(
        (const char*)(n));
}
void XMLParser::_characters(void* parser, const xmlChar* s, int len) {
    static_cast<XMLParser*>(parser)->_parser_callback.characters(
        std::string((const char*)(s), len));
}
void XMLParser::_comment(void* parser, const xmlChar* s) {
    static_cast<XMLParser*>(parser)->_parser_callback.comment(
        (const char*)(s));
}
void XMLParser::_warning(void* parser, const char* fmt, ...) {
    va_list arg;
    char buff[ERROR_BUFF_SIZE];
    va_start(arg, fmt);
    vsnprintf(buff, ERROR_BUFF_SIZE, fmt, arg);
    va_end(arg);

    static_cast<XMLParser*>(parser)->_parser_callback.warning(buff);
}
void XMLParser::_error(void* parser, const char* fmt, ...) {
    va_list arg;
    char buff[ERROR_BUFF_SIZE];
    va_start(arg, fmt);
    vsnprintf(buff, ERROR_BUFF_SIZE, fmt, arg);
    va_end(arg);

    static_cast<XMLParser*>(parser)->_parser_callback.error(buff);
}
void XMLParser::_fatal_error(void* parser, const char* fmt, ...) {
    va_list arg;
    char buff[ERROR_BUFF_SIZE];
    va_start(arg, fmt);
    vsnprintf(buff, ERROR_BUFF_SIZE, fmt, arg);
    va_end(arg);

    static_cast<XMLParser*>(parser)->_parser_callback.fatal_error(buff);
}

void XMLParser::parse_stream(XMLParserCallback& callback, std::istream& file,
        unsigned chunkSize) {
    XMLParser parser(callback);

    char* buf = new char[chunkSize];
    unsigned chunkRead;
    while (true) {
        // Read in the next chunk.
        for (chunkRead = 0; chunkRead < chunkSize; chunkRead++) {
            int c = file.get();
            if (c == EOF)
                break;
            else
                buf[chunkRead] = static_cast<char>(c);
        }
        if (chunkRead == 0)
            break;
        parser.parse_chunk(std::string(buf, chunkRead));
    }

    parser.finish();
    delete[] buf;
}

} // namespace regina::xml
