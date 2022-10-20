
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

/*! \file utilities/xmlutils.h
 *  \brief Various classes and routines for XML manipulation.  The libxml2
 *  library is used to do most of the underlying work.
 *
 *  The libxml++ library is copyright (C) 2000 by Ari Johnson, and is
 *  covered by the GNU Lesser General Public License.
 */

#ifndef __REGINA_XMLUTILS_H
#ifndef __DOXYGEN
#define __REGINA_XMLUTILS_H
#endif

#include <map>
#include <string>
#include <sstream>
#include "regina-core.h"
#include "utilities/boolset.h"

/**
 * Various classes and routines for XML manipulation, some taken or
 * modified from the libxml++ library.  The libxml2 library is used
 * to do most of the underlying work.
 *
 * See the xmlutils.h file documentation for libxml++ license details.
 */
namespace regina::xml {

/**
 * Returns the given string with special characters converted to XML
 * entities.  For instance, the string <tt>"a \< b"</tt> would be
 * converted to <tt>"a \&lt; b"</tt>.
 *
 * \param original the string to convert; this string will not be
 * changed.
 * \return the converted string with special characters replaced by
 * XML entities.
 *
 * \ingroup utilities
 */
std::string xmlEncodeSpecialChars(const std::string& original);

/**
 * Returns the given string encoded so it is suitable for use inside an
 * XML comment.  As well as converting special characters to XML
 * entities, this routine will replace dashes with underscores to avoid
 * double-hyphens (which are illegal in XML comments).
 *
 * \param comment the string to convert; this string will not be
 * changed.
 * \return the string converted to be usable inside an XML comment.
 *
 * \ingroup utilities
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
 * \nopython
 *
 * \param tagName the name of the XML tag to create.
 * \param value the value to assign to the <i>value</i> property of the tag.
 * \return the corresponding XML tag.
 *
 * \ingroup utilities
 */
template <class T>
inline std::string xmlValueTag(const std::string& tagName, const T& value) {
    std::ostringstream out;
    out << '<' << tagName << " value=\"" << value << "\"/>";
    return out.str();
}

#ifndef __APIDOCS
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
        return '<' + tagName + " value=\"" + value.stringCode() + "\"/>";
    }
#endif

} // namespace regina::xml

#endif

