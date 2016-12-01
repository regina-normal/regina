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

#include "syntax/context_p.h"
#include "syntax/state.h"
#include "syntax/state_p.h"

#include <cassert>

using namespace KSyntaxHighlighting;

State State::copy() const
{
    State s;
    s.d = std::make_shared<StateData>(*d);
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
