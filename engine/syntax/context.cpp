
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
#include "syntax/definition_p.h"
#include "syntax/format.h"
#include "syntax/repository.h"
#include "syntax/rule.h"

#include <cassert>
#include <iostream>

#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {
namespace syntax {

Context::Context()
    : m_resolveState(Unknown)
    , m_fallthrough(false)
{
}

Context::~Context()
{
}

Definition Context::definition() const
{
    return m_def.definition();
}

void Context::setDefinition(const DefinitionRef &def)
{
    m_def = def;
}

const std::string& Context::name() const
{
    return m_name;
}

const std::string& Context::attribute() const
{
    return m_attribute;
}

const ContextSwitch& Context::lineEndContext() const
{
    return m_lineEndContext;
}

const ContextSwitch& Context::lineEmptyContext() const
{
    return m_lineEmptyContext;
}

bool Context::fallthrough() const
{
    return m_fallthrough;
}

const ContextSwitch& Context::fallthroughContext() const
{
    return m_fallthroughContext;
}

const std::vector<Rule::Ptr>& Context::rules() const
{
    return m_rules;
}

Format Context::formatByName(const std::string& name) const
{
    auto defData = DefinitionData::get(m_def.definition());
    auto format = defData->formatByName(name);
    if (format.isValid())
        return format;

    // TODO we can avoid multiple lookups in the same definition here, many rules will share definitions
    for (const auto &rule : m_rules) {
        auto defData = DefinitionData::get(rule->definition());
        format = defData->formatByName(name);
        if (format.isValid())
            return format;
    }

    std::cerr << "Unknown format " << name << " in context " << m_name << " of definition " << m_def.definition().name() << std::endl;
    return format;
}

void Context::load(xmlTextReaderPtr reader)
{
    m_name = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
    m_attribute = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"attribute"));
    m_lineEndContext.parse(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"lineEndContext")));
    m_lineEmptyContext.parse(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"lineEmptyContext")));
    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"fallthrough")), m_fallthrough);
    m_fallthroughContext.parse(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"fallthroughContext")));
    if (m_fallthroughContext.isStay())
        m_fallthrough = false;

    if (xmlTextReaderIsEmptyElement(reader))
        return;
    if (xmlTextReaderRead(reader) != 1)
        return;
    while (true) {
        switch (xmlTextReaderNodeType(reader)) {
            case 1 /* start element */:
            {
                auto rule = Rule::create(regina::xml::xmlString(xmlTextReaderName(reader)));
                if (rule) {
                    rule->setDefinition(m_def.definition());
                    if (rule->load(reader))
                        m_rules.push_back(rule);
                } else {
                    // Skip current element.
                    if (xmlTextReaderNext(reader) != 1)
                        return;
                }
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
            }
            case 15 /* end element */:
                return;
            default:
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
        }
    }
}

void Context::resolveContexts()
{
    const auto def = m_def.definition();
    m_lineEndContext.resolve(def);
    m_lineEmptyContext.resolve(def);
    m_fallthroughContext.resolve(def);
    for (const auto &rule : m_rules)
        rule->resolveContext();
}

Context::ResolveState Context::resolveState()
{
    if (m_resolveState == Unknown) {
        for (const auto &rule : m_rules) {
            auto inc = std::dynamic_pointer_cast<IncludeRules>(rule);
            if (inc) {
                m_resolveState = Unresolved;
                return m_resolveState;
            }
        }
        m_resolveState = Resolved;
    }
    return m_resolveState;
}

void Context::resolveIncludes()
{
    if (resolveState() == Resolved)
        return;
    if (resolveState() == Resolving) {
        std::cerr << "Cyclic dependency!" << std::endl;
        return;
    }

    assert(resolveState() == Unresolved);
    m_resolveState = Resolving; // cycle guard

    for (auto it = m_rules.begin(); it != m_rules.end();) {
        auto inc = std::dynamic_pointer_cast<IncludeRules>(*it);
        if (!inc) {
            ++it;
            continue;
        }
        Context* context = nullptr;
        auto myDefData = DefinitionData::get(m_def.definition());
        if (inc->definitionName().empty()) { // local include
            context = myDefData->contextByName(inc->contextName());
        } else {
            auto def = myDefData->repo->definitionForName(inc->definitionName());
            if (!def.isValid()) {
                std::cerr << "Unable to resolve external include rule for definition" << inc->definitionName() << "in" << m_def.definition().name() << std::endl;
                ++it;
                continue;
            }
            auto defData = DefinitionData::get(def);
            defData->load();
            if (inc->contextName().empty())
                context = defData->initialContext();
            else
                context = defData->contextByName(inc->contextName());
        }
        if (!context) {
            std::cerr << "Unable to resolve include rule for definition" << inc->contextName() << "##" << inc->definitionName() << "in" << m_def.definition().name() << std::endl;
            ++it;
            continue;
        }
        context->resolveIncludes();
        if (inc->includeAttribute()) {
            m_attribute = context->attribute();
        }
        it = m_rules.erase(it);
        for (const auto &rule : context->rules()) {
            it = m_rules.insert(it, rule);
            ++it;
        }
    }

    m_resolveState = Resolved;
}

} } // namespace regina::syntax
