
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This file is modified from the KDE syntax-highlighting framework,     *
 *  which is copyright (C) 2016 Volker Krause <vkrause@kde.org>           *
 *  and is distributed under the GNU Library General Public License as    *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/**
 * \file syntax/definition.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_DEFINITION_H
#ifndef __DOXYGEN
#define __SYNTAX_DEFINITION_H
#endif

#include "regina-core.h"
#include <memory>
#include <string>
#include <vector>

namespace regina {
namespace syntax {

class Context;
class Format;
class KeywordList;

class DefinitionData;

/**
 * Represents a syntax definition.
 *
 * @section def_intro Introduction to Definitions
 *
 * A Definition is the short term for a syntax highlighting definition. It
 * typically is defined in terms of an XML syntax highlighting file, containing
 * all information about a particular syntax highlighting. This includes the
 * highlighting of keywords, and indentation preferences.
 *
 * @section def_info General Information
 *
 * Each Definition contains a non-translated unique name() and a section().
 * The location of the Definition can be obtained through filePath().
 *
 * @see Repository
 */
class REGINA_API Definition
{
public:
    /**
     * Default constructor, creating an empty (invalid) Definition instance.
     * isValid() for this instance returns @e false.
     *
     * Use the Repository instead to obtain valid instances.
     */
    Definition();

    /**
     * Copy constructor.
     * Both this definition as well as @p other share the Definition data.
     */
    Definition(const Definition &other);

    /**
     * Destructor.
     */
    ~Definition();

    /**
     * Assignment operator.
     * Both this definition as well as @p rhs share the Definition data.
     */
    Definition& operator=(const Definition &rhs);

    /**
     * Checks two definitions for equality.
     */
    bool operator==(const Definition &other) const;

    /**
     * Checks two definitions for inequality.
     */
    bool operator!=(const Definition &other) const;

    /** Checks whether this object refers to a valid syntax definition. */
    bool isValid() const;
    /** Returns the full path to the definition XML file containing
     *  the syntax definition.
     */
    const std::string& filePath() const;

    /** Name of the syntax.
     */
    const std::string& name() const;
    /** The group this syntax definition belongs to.
     */
    const std::string& section() const;
    /** Returns the definition version. */
    int version() const;
    /** Returns @c true if this is an internal definition that should not be
     * displayed to the user.
     */
    bool isHidden() const;
    /** Generalized language style, used for indentation. */
    const std::string& style() const;
    /** Indentation style to be used for this syntax. */
    const std::string& indenter() const;
    /** Name and email of the author of this syntax definition. */
    const std::string& author() const;
    /** License of this syntax definition. */
    const std::string& license() const;

private:
    friend class DefinitionData;
    friend class DefinitionRef;
    explicit Definition(const std::shared_ptr<DefinitionData> &dd);
    std::shared_ptr<DefinitionData> d;
};

} } // namespace regina::syntax

#endif
