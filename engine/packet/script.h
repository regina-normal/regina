
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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

/*! \file packet/script.h
 *  \brief Contains a packet representing a script.
 */

#ifndef __SCRIPT_H
#ifndef __DOXYGEN
#define __SCRIPT_H
#endif

#include <map>
#include <utility>
#include <vector>
#include "regina-core.h"
#include "packet/packet.h"

namespace regina {

class XMLPacketReader;
class Script;

/**
 * \weakgroup packet
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_SCRIPT> {
    typedef Script Class;
    inline static const char* name() {
        return "Script";
    }
};
#endif

/**
 * A packet representing a Python script that can be run.
 *
 * A script consists of two parts: (i) the \e text, which contains the
 * Python code; and (ii) a set of \e variables, which refer to packets
 * in your packet tree.  When running a script, the variables should be
 * instantiated in the default namespace before the script is run.
 *
 * The values of variables are given by pointers to packets (not packet
 * labels, as in some old versions of Regina).  This affects how variables
 * react to changes in the packets that they point to.  In particular, if a
 * variable \a V points to some packet \a P, then:
 *
 * - if \a P is renamed then \a V will still point to it and the script will
 *   notify listeners that the script has changed;
 * - if \a P is deleted then \a V will take the value \c None, and the script
 *   will likewise notify listeners of the change.
 */
class REGINA_API Script : public Packet, public PacketListener {
    REGINA_PACKET(Script, PACKET_SCRIPT)

    private:
        std::string text_;
            /**< The complete text of this script, including newlines. */
        std::map<std::string, Packet*> variables;
            /**< A map storing the variables with which this script
                 is to be run.  Variable names are mapped to their
                 corresponding values. */

    public:
        /**
         * Initialises to a script with no text and no variables.
         */
        Script();

        /**
         * Returns the complete text of this script.
         *
         * Variables are not considered part of the text; you can get
         * and set them through other member functions (see below).
         *
         * @return the complete text of this script.
         */
        const std::string& text() const;

        /**
         * Replaces the complete text of this script with the given
         * string.
         *
         * Variables are not considered part of the text; you can get
         * and set them through other member functions (see below).
         *
         * @param newText the new text for this script.
         */
        void setText(const std::string& newText);

        /**
         * Adds the given text to the end of this script.
         *
         * @param extraText the text to add.
         */
        void append(const std::string& extraText);

        /**
         * Returns the number of variables associated with this script.
         *
         * @return the number of variables.
         */
        size_t countVariables() const;
        /**
         * Returns the name of the requested variable associated with
         * this script.
         *
         * @param index the index of the requested variable; this must
         * be between 0 and countVariables()-1 inclusive.
         * @return the name of the requested variable.
         */
        const std::string& variableName(size_t index) const;
        /**
         * Returns the index of the variable stored with the given name.
         *
         * @param name the name of the requested variable; note that
         * names are case sensitive.
         * @return the index of the requested variable as an integer
         * between 0 and countVariables()-1 inclusive, or -1 if
         * there is no variable with the given name.
         */
        long variableIndex(const std::string& name) const;
        /**
         * Returns the value of the requested variable associated with
         * this script.  Variables may take the value \c null.
         *
         * @param index the index of the requested variable; this must
         * be between 0 and countVariables()-1 inclusive.
         * @return the value of the requested variable.
         */
        Packet* variableValue(size_t index) const;
        /**
         * Returns the value of the variable stored with the given
         * name.  Variables may take the value \c null.
         *
         * If no variable is stored with the given name, then \c null
         * will likewise be returned.
         *
         * @param name the name of the requested variable; note that
         * names are case sensitive.
         * @return the value of the requested variable.
         */
        Packet* variableValue(const std::string& name) const;

        /**
         * Changes the name of an existing variable associated with
         * this script.
         *
         * \warning This may change the indices of this and other
         * variables, since (at present) variables are kept stored in
         * sorted order by name.
         *
         * @param index the index of the variable whose name should change;
         * this must be between 0 and countVariables()-1 inclusive.
         * @param name the new name to assign to the variable.
         */
        void setVariableName(size_t index, const std::string& name);
        /**
         * Changes the value of an existing variable associated with
         * this script.
         *
         * @param index the index of the variable whose value should change;
         * this must be between 0 and countVariables()-1 inclusive.
         * @param value the new value to assign to the variable.
         */
        void setVariableValue(size_t index, Packet* value);

        /**
         * Attempts to add a new variable to be associated with this script.
         * If a variable with the given name is already stored, this
         * routine will do nothing.
         *
         * If you need to ensure that a new variable is always added,
         * even if the variable name needs to change, see the routine
         * addVariableName() instead.
         *
         * \warning The index of the new variable might not be
         * countVariables()-1, and this operation may change the
         * indices of other variables also.  This is because (at present)
         * variables are kept stored in sorted order by name.
         *
         * @param name the name of the new variable.
         * @param value the value of the new variable; this is allowed
         * to be \c null.
         * @return \c true if the variable was successfully added, or
         * \c false if a variable with the given name was already stored.
         */
        bool addVariable(const std::string& name, Packet* value);
        /**
         * Adds a new variable to be associated with this script, changing
         * its name if necessary.  If the given variable name does not already
         * exist as a variable name in this script, then it will be used
         * without modification.  Otherwise a new variable name will
         * be constructed by appending additional characters to \a name.
         *
         * \warning The index of the new variable might not be
         * countVariables()-1, and this operation may change the
         * indices of other variables also.  This is because (at present)
         * variables are kept stored in sorted order by name.
         *
         * @param name the string upon which the new variable name will
         * be based.
         * @param value the value of the new variable; this is allowed
         * to be \c null.
         * @return the name of the variable that was added; this might
         * or might not be equal to \a name.
         */
        const std::string& addVariableName(const std::string& name,
            Packet* value);
        /**
         * Removes the variable stored with the given name.
         * If no variable is stored with the given name, this routine
         * will do nothing.
         *
         * \warning This may change the indices of other variables, since
         * (at present) variables are kept stored in sorted order by name.
         *
         * @param name the name of the variable to remove; note that
         * names are case sensitive.
         */
        void removeVariable(const std::string& name);
        /**
         * Removes the variable stored at the given index.
         *
         * \warning This may change the indices of other variables, since
         * (at present) variables are kept stored in sorted order by name.
         *
         * @param index the index of the variable to remove;
         * this must be between 0 and countVariables()-1 inclusive.
         */
        void removeVariable(size_t index);
        /**
         * Removes all variables associated with this script.
         */
        void removeAllVariables();

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);
        virtual bool dependsOnParent() const;

        virtual void packetWasRenamed(Packet* packet);
        virtual void packetToBeDestroyed(Packet* packet);

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NScript has now been renamed to Script.
 */
REGINA_DEPRECATED typedef Script NScript;

/*@}*/

// Inline functions for Script

inline Script::Script() {
}

inline const std::string& Script::text() const {
    return text_;
}
inline void Script::setText(const std::string& newText) {
    if (text_ == newText)
        return; // No change event fired.

    ChangeEventSpan span(this);
    text_ = newText;
}

inline void Script::append(const std::string& extraText) {
    if (extraText.empty())
        return; // No change event fired.

    ChangeEventSpan span(this);
    text_ += extraText;
}

inline size_t Script::countVariables() const {
    return variables.size();
}

inline bool Script::addVariable(const std::string& name, Packet* value) {
    ChangeEventSpan span(this);
    bool ans = variables.insert(std::make_pair(name, value)).second;
    if (value)
        value->listen(this);
    return ans;
}
inline void Script::removeAllVariables() {
    unregisterFromAllPackets();

    ChangeEventSpan span(this);
    variables.clear();
}

inline void Script::writeTextShort(std::ostream& o) const {
    o << "Python script";
}

inline bool Script::dependsOnParent() const {
    return false;
}

} // namespace regina

#endif

