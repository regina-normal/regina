
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
 * \file syntax/repository.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_REPOSITORY_H
#ifndef __DOXYGEN
#define __SYNTAX_REPOSITORY_H
#endif

#include "regina-core.h"
#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>

namespace regina {

/**
 * @namespace syntax
 *
 * Syntax highlighting engine for Kate syntax definitions.
 * In order to access the syntax highlighting Definition files, use the
 * class Repository.
 *
 * @see Repository
 */
namespace syntax {

class Definition;
class RepositoryPrivate;
class Theme;

/**
 * @brief Syntax highlighting repository.
 *
 * @section repo_intro Introduction
 *
 * The Repository gives access to all syntax Definitions available on the
 * system, typically described in *.xml files. The Definition files are read
 * from the resource that is compiled into the executable, and from the file
 * system. If a Definition exists in the resource and on the file system,
 * then the one from the file system is chosen.
 *
 * @section repo_access Definitions and Themes
 *
 * Typically, only one instance of the Repository is needed. This single
 * instance can be thought of as a singleton you keep alive throughout the
 * lifetime of your application. Then, call definitionForName() with the
 * given language name (e.g. "Python" or "Java").
 *
 * In addition to Definitions, the Repository also provides a list of Themes.
 * A Theme is defined by a set of default text style colors as well as editor
 * colors. These colors together provide all required colros for drawing all
 * primitives of a text editor. All available Theme%s can be queried through
 * themes(), and a Theme with a specific name is obtained through theme().
 * Additionally, defaultTheme() provides a way to obtain a default theme for
 * either a light or a black color theme.
 *
 * @see Definition, Theme, AbstractHighlighter
 */
class REGINA_API Repository : public boost::noncopyable
{
public:
    /**
     * Create a new syntax definition repository.
     * This will read the meta data information of all available syntax
     * definition, which is a moderately expensive operation, it's therefore
     * recommended to keep a single instance of Repository around as long
     * as you need highlighting in your application.
     */
    Repository();
    ~Repository();

    /**
     * Returns the Definition named @p defName.
     *
     * If no Definition is found, Definition::isValid() of the returned instance
     * returns false.
     *
     * @note This uses case sensitive, untranslated names. For instance,
     *       the javascript.xml definition file sets its name to @e JavaScript.
     *       Therefore, only the string "JavaScript" will return a valid
     *       Definition file.
     */
    Definition definitionForName(const std::string& defName) const;

    /**
     * Returns all available color themes.
     * The returned list should never be empty.
     */
    const std::vector<Theme>& themes() const;

    /**
     * Returns the theme called @p themeName.
     * If the requested theme cannot be found, the retunred Theme is invalid,
     * see Theme::isValid().
     */
    Theme theme(const std::string& themeName) const;

    /**
     * Built-in default theme types.
     * @see defaultTheme()
     */
    enum DefaultTheme {
        //! Theme with a light background color.
        LightTheme,
        //! Theme with a dark background color.
        DarkTheme
    };

    /**
     * Returns a default theme instance of the given type.
     * The returned Theme is guaranteed to be a valid theme.
     */
    Theme defaultTheme(DefaultTheme t = LightTheme);

    /**
     * Reloads the repository.
     * This is a moderately expensive operations and should thus only be
     * triggered when the installed syntax definition files changed.
     */
    void reload();

private:
    friend class RepositoryPrivate;
    std::unique_ptr<RepositoryPrivate> d;
};

} } // namespace regina::syntax

#endif
