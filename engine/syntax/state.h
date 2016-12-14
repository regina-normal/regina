
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
 * \file syntax/state.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_STATE_H
#ifndef __DOXYGEN
#define __SYNTAX_STATE_H
#endif

#include "regina-core.h"
#include <memory>

namespace regina {
namespace syntax {

class StateData;

/** Opaque handle to the state of the highlighting engine.
 *  This needs to be fed into AbstractHighlighter for every line of text
 *  and allows concrete highlighter implementations to store state per
 *  line for fast re-highlighting of specific lines (e.g. during editing).
 */
class REGINA_API State
{
public:
    /** Creates an initial state, ie. what should be used for the first line
     *  in a document.
     */
    State();
    State(const State &other);
    ~State();
    /**
     * Performs a shallow copy.
     */
    State& operator=(const State &rhs);
    /**
     * Performs a deep copy.
     */
    State copy() const;

    /** Compares two states for equality.
     *  For two equal states and identical text input, AbstractHighlighter
     *  guarantees to produce equal results. This can be used to only
     *  re-highlight as many lines as necessary during editing.
     */
    bool operator==(const State &other) const;
    /** Compares two states for inequality.
     *  This is the opposite of operator==().
     */
    bool operator!=(const State &other) const;

private:
    friend class StateData;
    std::shared_ptr<StateData> d;
};

} } // namespace regina::syntax

#endif
