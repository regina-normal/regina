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

#ifndef __SYNTAX_CONTEXT_H
#ifndef __DOXYGEN
#define __SYNTAX_CONTEXT_H
#endif

#include "syntax/rule.h"
#include "syntax/contextswitch.h"
#include "syntax/definition.h"
#include "syntax/definitionref.h"

#include <boost/noncopyable.hpp>
#include <libxml/xmlreader.h>

namespace regina {
namespace syntax {

class Context : public boost::noncopyable
{
public:
    Context();
    ~Context();

    Definition definition() const;
    void setDefinition(const DefinitionRef &def);

    const std::string& name() const;
    const std::string& attribute() const;
    const ContextSwitch& lineEndContext() const;
    const ContextSwitch& lineEmptyContext() const;

    bool fallthrough() const;
    const ContextSwitch& fallthroughContext() const;

    const std::vector<Rule::Ptr>& rules() const;

    /** Attempts to find the format named @p name in the
     *  enclosing definition, or the source definition of any
     *  included rule.
     */
    Format formatByName(const std::string& name) const;

    void load(xmlTextReaderPtr reader);
    void resolveContexts();
    void resolveIncludes();

private:
    enum ResolveState {
        Unknown,
        Unresolved,
        Resolving,
        Resolved
    };
    ResolveState resolveState();

    DefinitionRef m_def;
    std::string m_name;
    std::string m_attribute;
    ContextSwitch m_lineEndContext;
    ContextSwitch m_lineEmptyContext;
    ContextSwitch m_fallthroughContext;

    std::vector<Rule::Ptr> m_rules;

    ResolveState m_resolveState;
    bool m_fallthrough;
};

} } // namespace regina::syntax

#endif
