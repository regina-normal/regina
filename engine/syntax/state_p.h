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

#ifndef KSYNTAXHIGHLIGHTING_STATE_P_H
#define KSYNTAXHIGHLIGHTING_STATE_P_H

#include <deque>

namespace regina {
namespace syntax {

class Context;
class DefinitionData;

class StateData
{
public:
    StateData();
    static StateData* get(State &state);

    bool isEmpty() const;
    void clear();
    int size() const;
    void push(Context *context);
    void pop();
    Context* topContext() const;

    DefinitionData *m_defData;
private:
    friend class State;
    std::deque<Context*> m_contextStack;
};

} } // namespace regina::syntax

#endif
