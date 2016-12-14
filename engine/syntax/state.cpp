
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

#include "syntax/context.h"
#include "syntax/state.h"
#include "syntax/state_p.h"

#include <cassert>

namespace regina {
namespace syntax {

State State::copy() const
{
    State s;
    s.d.reset(new StateData(*d));
    return s;
}

StateData::StateData() :
    m_defData(nullptr)
{
}

StateData* StateData::get(State &state)
{
    return state.d.get();
}

bool StateData::isEmpty() const
{
    return m_contextStack.empty();
}

void StateData::clear()
{
    m_contextStack.clear();
}

int StateData::size() const
{
    return m_contextStack.size();
}

void StateData::push(Context *context)
{
    assert(context);
    m_contextStack.push_back(context);
}

void StateData::pop()
{
    m_contextStack.pop_back();
}

Context* StateData::topContext() const
{
    assert(!isEmpty());
    return m_contextStack.back();
}

State::State() :
    d(new StateData)
{
}

State::State(const State &other) :
    d(other.d)
{
}

State::~State()
{
}

State& State::operator=(const State &other)
{
    d = other.d;
    return *this;
}

bool State::operator==(const State &other) const
{
    return d->m_contextStack == other.d->m_contextStack && d->m_defData == other.d->m_defData;
}

bool State::operator!=(const State &other) const
{
    return !(*this == other);
}

} } // namespace regina::syntax
