
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

#include "syntax/abstracthighlighter.h"

#include <QSyntaxHighlighter>

namespace regina {
namespace syntax {

class SyntaxHighlighterPrivate;

/** A QSyntaxHighlighter implementation for use with QTextDocument.
 *  This supports partial re-highlighting during editing.
 */
class SyntaxHighlighter : public QSyntaxHighlighter, public AbstractHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QObject *parent = nullptr);
    explicit SyntaxHighlighter(QTextDocument *document);
    ~SyntaxHighlighter();

    void setDefinition(const Definition &def) override;

protected:
    void highlightBlock(const QString & text) override;
    void applyFormat(int offset, int length, const Format &format) override;
};

} } // namespace regina::syntax

#endif
