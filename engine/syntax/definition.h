/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KSYNTAXHIGHLIGHTING_DEFINITION_H
#define KSYNTAXHIGHLIGHTING_DEFINITION_H

#include <memory>
#include <string>
#include <vector>

namespace KSyntaxHighlighting {

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
class Definition
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

}

#endif
