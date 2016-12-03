
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

#ifndef __SYNTAX_SYNTAXHIGHLIGHTER_H
#define __SYNTAX_SYNTAXHIGHLIGHTER_H

#include "syntax/definition.h"
#include "syntax/theme.h"
#include <memory>
#include <boost/noncopyable.hpp>

#include <QSyntaxHighlighter>

namespace regina { namespace syntax {
    class ContextSwitch;
    class State;
    class StateData;
} }

/**
 * The SyntaxHighlighter provides an interface to highlight text.
 * This class derives from QSyntaxHighlighter,
 * meaning that it can be used to highlight a QTextDocument or a QML TextEdit.
 * In order to use the SyntaxHighlighter, just call setDefinition() and
 * setTheme(), and the associated documents will automatically be highlighted.
 *
 * This class supports partial re-highlighting during editing.
 */
class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QObject *parent = nullptr);
    explicit SyntaxHighlighter(QTextDocument *document);
    ~SyntaxHighlighter();

    /**
     * Returns the syntax definition used for highlighting.
     *
     * @see setDefinition()
     */
    regina::syntax::Definition definition() const;

    /**
     * Sets the syntax definition used for highlighting.
     *
     * This will trigger re-highlighting if needed.
     */
    void setDefinition(const regina::syntax::Definition &def);

    /**
     * Returns the currently selected theme for highlighting.
     *
     * @note If no Theme was set through setTheme(), the returned Theme will be
     *       invalid, see Theme::isValid().
     */
    regina::syntax::Theme theme() const;

    /**
     * Sets the theme used for highlighting.
     */
    void setTheme(const regina::syntax::Theme &theme);

protected:
    void ensureDefinitionLoaded();
    bool switchContext(regina::syntax::StateData* data,
        const regina::syntax::ContextSwitch &contextSwitch);

    /**
     * Highlight the given line.
     * This will result in any number of applyFormat() calls as a result.
     * @param text A string containing the text of the line to highlight.
     * @param state The highlighting state handle returned by the call
     *        to highlightLine() for the previous line. For the very first line,
     *        just pass a default constructed State().
     * @returns The state of the highlighing engine after processing the
     *        given line. This needs to passed into highlightLine() for the
     *        next line. You can store the state for efficient partial
     *        re-highlighting for example during editing.
     *
     * @see applyFormat()
     */
    regina::syntax::State highlightLine(const QString &text,
        const regina::syntax::State &state);

    /**
     * Applies the given format to the text. The provided @p format
     * must be valid for the interval [@p offset, @p offset + @p length).
     * Both @p offset and @p length count unicode characters (not machine-level
     * characters as might be used for instance in a UTF-8 encoding).
     *
     * @param offset The start column of the interval for which @p format matches
     * @param length The length of the matching text
     * @param format The Format that applies to the range [offset, offset + length)
     *
     * @note Make sure to set a valid Definition, otherwise the parameter
     *       @p format is invalid for the entire line passed to highlightLine()
     *       (cf. Format::isValid()).
     *
     * @see highlightLine()
     */
    void applyFormat(int offset, int length, const regina::syntax::Format &format);

    void highlightBlock(const QString & text) override;

    regina::syntax::Definition m_definition;
    regina::syntax::Theme m_theme;
};

#endif

