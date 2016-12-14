
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

#include "syntax/contextswitch.h"
#include "syntax/definition.h"
#include "syntax/definition_p.h"
#include "syntax/repository.h"

#include "utilities/stringutils.h"

#include <iostream>

namespace regina {
namespace syntax {

ContextSwitch::ContextSwitch() :
    m_context(nullptr),
    m_popCount(0)
{
}

ContextSwitch::~ContextSwitch()
{
}

bool ContextSwitch::isStay() const
{
    return m_popCount == 0 && !m_context && m_contextName.empty() && m_defName.empty();
}

int ContextSwitch::popCount() const
{
    return m_popCount;
}

Context* ContextSwitch::context() const
{
    return m_context;
}

void ContextSwitch::parse(const std::string& contextInstr)
{
    if (contextInstr.empty() || contextInstr == "#stay")
        return;

    if (regina::startsWith(contextInstr, "#pop!")) {
        ++m_popCount;
        m_contextName = contextInstr.substr(5);
        return;
    }

    if (regina::startsWith(contextInstr, "#pop")) {
        ++m_popCount;
        parse(contextInstr.substr(4));
        return;
    }

    auto idx = contextInstr.find("##");
    if (idx != std::string::npos) {
        m_contextName = contextInstr.substr(0, idx);
        m_defName = contextInstr.substr(idx + 2);
    } else {
        m_contextName = contextInstr;
    }
}

void ContextSwitch::resolve(const Definition &def)
{
    auto d = def;
    if (!m_defName.empty()) {
        d = DefinitionData::get(def)->repo->definitionForName(m_defName);
        auto data = DefinitionData::get(d);
        data->load();
        if (m_contextName.empty())
            m_context = data->initialContext();
    }

    if (!m_contextName.empty()) {
        m_context = DefinitionData::get(d)->contextByName(m_contextName);
        if (!m_context)
            std::cerr << "cannot find context " << m_contextName << " in " << def.name() << std::endl;
    }
}

} } // namespace regina::syntax
