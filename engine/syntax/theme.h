
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
 * \file syntax/theme.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_THEME_H
#ifndef __DOXYGEN
#define __SYNTAX_THEME_H
#endif

#include "regina-core.h"
#include <memory>
#include <string>

namespace regina {
namespace syntax {

class ThemeData;
class RepositoryPrivate;

/**
 * Color theme definition used for highlighting.
 *
 * @section theme_intro Introduction
 *
 * The Theme provides a full color theme for painting the highlighted text.
 * One Theme is defined as a *.theme file on disk. Each
 * Theme has a unique name().
 * Themes shipped by default are typically read-only, see isReadOnly().
 *
 * A Theme defines two sets of colors:
 * - Text colors, including foreground and background colors, colors for
 *   selected text, and properties such as bold and italic. These colors are
 *   used e.g. by the SyntaxHighlighter.
 * - Editor colors, including a background color for the entire editor widget,
 *   the line number color, etc.
 *
 * @section theme_text_colors Text Colors and the Class Format
 *
 * The text colors are used for syntax highlighting.
 * // TODO: elaborate more and explain relation to Format class
 *
 * @section theme_editor_colors Editor Colors
 *
 * If you want to use the SyntaxHighlighting framework to write your own text
 * editor, you also need to paint the background of the editing widget. In
 * addition, the editor may support showing line numbers, a
 * highlight for the current text line, and similar features. All these colors
 * are defined in terms of the "editor colors" and accessible by calling
 * editorColor() with the desired enum EditorColorRole.
 *
 * @section theme_access Accessing a Theme
 *
 * All available Theme%s are accessed through the Repository. These themes are
 * typically valid themes. If you create a Theme on your own, isValid() will
 * return @e false, and all colors provided by this Theme are in fact invalid
 * and therefore unusable.
 *
 * @see Format
 */
class REGINA_API Theme
{
public:
    /**
     * Default styles that can be referenced from syntax definition XML files.
     * Make sure to choose readable colors with good contrast especially in
     * combination with the EditorColorRole%s.
     */
    enum TextStyle {
        //! Default text style for normal text and source code without
        //! special highlighting.
        Normal = 0,
        //! Text style for language keywords.
        Keyword,
        //! Text style for function definitions and function calls.
        Function,
        //! Text style for variables, if applicable. For instance, variables in
        //! PHP typically start with a '$', so all identifiers following the
        //! pattern $foo are highlighted as variable.
        Variable,
        //! Text style for control flow highlighting, such as @e if, @e then,
        //! @e else, @e return, or @e continue.
        ControlFlow,
        //! Text style for operators such as +, -, *, / and :: etc.
        Operator,
        //! Text style for built-in language classes and functions.
        BuiltIn,
        //! Text style for well-known extensions, such as boost.
        Extension,
        //! Text style for preprocessor statements.
        Preprocessor,
        //! Text style for attributes of functions or objects, e.g. \@override
        //! in Java, or __declspec(...) and __attribute__((...)) in C++.
        Attribute,
        //! Text style for single characters such as 'a'.
        Char,
        //! Text style for escaped characters in strings, such as "hello\n".
        SpecialChar,
        //! Text style for strings, for instance "hello world".
        String,
        //! Text style for verbatim strings such as HERE docs.
        VerbatimString,
        //! Text style for special strings such as regular expressions in
        //! ECMAScript or the LaTeX math mode.
        SpecialString,
        //! Text style for includes, imports, modules, or LaTeX packages.
        Import,
        //! Text style for data types such as int, char, float etc.
        DataType,
        //! Text style for decimal values.
        DecVal,
        //! Text style for numbers with base other than 10.
        BaseN,
        //! Text style for floating point numbers.
        Float,
        //! Text style for language constants, e.g. True, False, None in Python
        //! or nullptr in C/C++.
        Constant,
        //! Text style for normal comments.
        Comment,
        //! Text style for comments that reflect API documentation, such as
        //! doxygen /** */ comments.
        Documentation,
        //! Text style for annotations in comments, such as \@param in Doxygen
        //! or JavaDoc.
        Annotation,
        //! Text style that refers to variables in a comment, such as after
        //! \@param \<identifier\> in Doxygen or JavaDoc.
        CommentVar,
        //! Text style for region markers, typically defined by BEGIN/END.
        RegionMarker,
        //! Text style for information, such as the keyword \@note in Doxygen.
        Information,
        //! Text style for warnings, such as the keyword \@warning in Doxygen.
        Warning,
        //! Text style for comment specials such as TODO and WARNING in
        //! comments.
        Alert,
        //! Text style indicating wrong syntax.
        Error,
        //! Text style for attributes that do not match any of the other default
        //! styles.
        Others
    };

    /**
     * Editor color roles, used to paint line numbers, editor background etc.
     * The colors typically should have good contrast with the colors used
     * in the TextStyle%s.
     */
    enum EditorColorRole {
        //! Background color for the editing area.
        BackgroundColor = 0,
        //! Background color for selected text.
        TextSelection,
        //! Background color for the line of the current text cursor.
        CurrentLine,
        //! Background color for matching text while searching.
        SearchHighlight,
        //! Background color for replaced text for a search & replace action.
        ReplaceHighlight,
        //! Background color for matching bracket pairs (including quotes)
        BracketMatching,
        //! Foreground color for visualizing tabs and trailing spaces.
        TabMarker,
        //! Color used to underline spell check errors.
        SpellChecking,
        //! Color used to draw vertical indentation levels, typically a line.
        IndentationLine,
        //! Background color for the icon border.
        IconBorder,
        //! Foreground color for drawing the line numbers. This should have a
        //! good contrast with the IconBorder background color.
        LineNumbers,
        //! Foreground color for drawing the current line number. This should
        //! have a good contrast with the IconBorder background color.
        CurrentLineNumber,
        //! Color used in the icon border to indicate dynamically wrapped lines.
        //! This color should have a good contrast with the IconBorder
        //! background color.
        WordWrapMarker,
        //! Color used to draw a vertical line for marking changed lines.
        ModifiedLines,
        //! Color used to draw a vertical line for marking saved lines.
        SavedLines,
        //! Line color used to draw separator lines, e.g. at column 80 in the
        //! text editor area.
        Separator,
        //! Background color for bookmarks.
        MarkBookmark,
        //! Background color for active breakpoints.
        MarkBreakpointActive,
        //! Background color for a reached breakpoint.
        MarkBreakpointReached,
        //! Background color for inactive (disabled) breakpoints.
        MarkBreakpointDisabled,
        //! Background color for marking the current execution position.
        MarkExecution,
        //! Background color for general warning marks.
        MarkWarning,
        //! Background color for general error marks.
        MarkError,
        //! Background color for text templates (snippets).
        TemplateBackground,
        //! Background color for all editable placeholders in text templates.
        TemplatePlaceholder,
        //! Background color for the currently active placeholder in text
        //! templates.
        TemplateFocusedPlaceholder,
        //! Background color for read-only placeholders in text templates.
        TemplateReadOnlyPlaceholder
    };

    /**
     * Default constructor, creating an invalid Theme, see isValid().
     */
    Theme();

    /**
     * Copy constructor, sharing the Theme data with @p copy.
     */
    Theme(const Theme &copy);

    /**
     * Destructor.
     */
    ~Theme();

    /**
     * Assignment operator, sharing the Theme data with @p other.
     */
    Theme &operator=(const Theme &other);

    /**
     * Returns @c true if this is a valid Theme.
     * If the theme is invalid, none of the returned colors are well-defined.
     */
    bool isValid() const;

    /**
     * Returns the unique name of this Theme.
     */
    const std::string& name() const;

    /**
     * Returns @c true if this Theme is read-only.
     * Typically, themes that are shipped by default are read-only.
     */
    bool isReadOnly() const;

    /**
     * Returns the full path and file name to this Theme.
     *
     * If the theme is invalid (isValid()), an empty string is returned.
     */
    const std::string& filePath() const;

    /**
     * Returns the text color to be used for @p style.
     * The color will be returned as an integer of the form #AARRGGBB.
     * @c 0 is returned for styles that do not specify a text color,
     * use the default text color in that case.
     */
    unsigned textColor(TextStyle style) const;

    /**
     * Returns the selected text color to be used for @p style.
     * The color will be returned as an integer of the form #AARRGGBB.
     * @c 0 is returned for styles that do not specify a selected text color,
     * use the default textColor() in that case.
     */
    unsigned selectedTextColor(TextStyle style) const;

    /**
     * Returns the background color to be used for @p style.
     * The color will be returned as an integer of the form #AARRGGBB.
     * @c 0 is returned for styles that do not specify a background color,
     * use the default background color in that case.
     */
    unsigned backgroundColor(TextStyle style) const;

    /**
     * Returns the background color to be used for selected text for @p style.
     * The color will be returned as an integer of the form #AARRGGBB.
     * @c 0 is returned for styles that do not specify a background color,
     * use the default backgroundColor() in that case.
     */
    unsigned selectedBackgroundColor(TextStyle style) const;

    /**
     * Returns whether the given style should be shown in bold.
     */
    bool isBold(TextStyle style) const;

    /**
     * Returns whether the given style should be shown in italic.
     */
    bool isItalic(TextStyle style) const;

    /**
     * Returns whether the given style should be shown underlined.
     */
    bool isUnderline(TextStyle style) const;

    /**
     * Returns whether the given style should be shown struck through.
     */
    bool isStrikeThrough(TextStyle style) const;

    /**
     * Sorts themes alphabetically by name.
     */
    bool operator < (const Theme& rhs) const;

public:
    /**
     * Returns the editor color for the requested @p role.
     * The color will be returned as an integer of the form #AARRGGBB.
     */
    unsigned editorColor(EditorColorRole role) const;

private:
    /**
     * Constructor taking a shared ThemeData instance.
     */
    explicit Theme(ThemeData* data);
    friend class RepositoryPrivate;
    friend class ThemeData;

private:
    /**
     * Shared data holder.
     */
    std::shared_ptr<ThemeData> m_data;
};

inline bool Theme::operator < (const Theme& rhs) const {
    return name() < rhs.name();
}

} } // namespace regina::syntax

#endif
