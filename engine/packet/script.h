
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#ifndef __REGINA_SCRIPT_H
#ifndef __DOXYGEN
#define __REGINA_SCRIPT_H
#endif

#include <map>
#include <utility>
#include <vector>
#include "regina-core.h"
#include "packet/packet.h"

namespace regina {

class Script;

/**
 * A packet representing a Python script that can be run.
 *
 * A script consists of two parts: (i) the _text_, which contains the
 * Python code; and (ii) a set of _variables_, which refer to packets
 * in your packet tree.  When running a script, the variables should be
 * instantiated in the default namespace before the script is run.
 *
 * The way variables are stored has changed as of Regina 7.0:
 *
 * - the value of each variable is now stored as a std::weak_ptr<Packet>;
 * - scripts do not listen to packet events on their variables.
 *
 * This means that:
 *
 * - If a packet is stored as the value of some script variable, this
 *   does not prevent the packet from being destroyed (since the script
 *   only holds weak pointers).
 *
 * - If such a packet changes its packet label or is destroyed, the script
 *   will not notify its _own_ listeners of the change.  If a user interface
 *   needs to know about this change (e.g., because it shows packet labels
 *   of script variables in a visual table), it will need to set up packet
 *   listeners for the individual variables - it is no longer enough just to
 *   listen on the script itself.  Note that packet labels are completely
 *   independent of the variable names stored in the script.
 *
 * - If such a packet is destroyed, the script will behave correctly: any
 *   subsequent request to retrieve the value of the script variable will
 *   return a null pointer (not an invalid pointer to a non-existent packet).
 *
 * For historical reference, the previous behaviour was:
 *
 * - In Regina 6.0.1 and earlier (before Regina switched to widespread use of
 *   std::shared_ptr for packets), scripts would notify their own listeners
 *   when a variable packet was destroyed or changed its packet label.
 *   This was implemented by having each script listen for packet events on
 *   its own variables.
 *
 * - In Regina 4.94 and earlier (before the file format supported
 *   cross-referencing between packets), script variables were stored purely
 *   by their packet labels (not pointers to the packets themselves).
 *
 * Like all packet types, this class does not support C++ move semantics
 * since this would interfere with the structure of the packet tree.
 * It does support copy construction, copy assignment and swaps; however,
 * these operations only copy/swap the mathematical content, not the packet
 * infrastructure (e.g., they do not touch packet labels, or the packet
 * tree, or event listeners).
 *
 * \ingroup packet
 */
class Script : public Packet {
    REGINA_PACKET(PACKET_SCRIPT, "Script")

    private:
        std::string text_;
            /**< The complete text of this script, including newlines. */
        std::map<std::string, std::weak_ptr<Packet>> variables_;
            /**< A map storing the variables with which this script
                 is to be run.  Variable names are mapped to their
                 corresponding values. */

    public:
        /**
         * Initialises to a script with no text and no variables.
         */
        Script() = default;

        /**
         * Creates a new copy of the given script packet.
         *
         * Like all packet types, this only copies the script content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * it will not clone the given packet's children, and it will not
         * insert the new packet into any packet tree).
         */
        Script(const Script&) = default;

        /**
         * Sets this to be a copy of the given script packet.
         *
         * Like all packet types, this only copies the script content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * or change this packet's location in any packet tree).
         *
         * \param src the script packet whose contents should be copied.
         * \return a reference to this packet.
         */
        Script& operator = (const Script& src);

        /**
         * Swaps the contents of this and the given script packet.
         *
         * Like all packet types, this only swaps the script content, not
         * the packet infrastructure (e.g., it will not swap packet labels,
         * or change either packet's location in any packet tree).
         *
         * \param other the script packet whose contents should be swapped
         * with this.
         */
        void swap(Script& other);

        /**
         * Returns the complete text of this script.
         *
         * Variables are not considered part of the text; you can get
         * and set them through other member functions (see below).
         *
         * \return the complete text of this script.
         */
        const std::string& text() const;

        /**
         * Replaces the complete text of this script with the given
         * string.
         *
         * Variables are not considered part of the text; you can get
         * and set them through other member functions (see below).
         *
         * \param newText the new text for this script.
         */
        void setText(const std::string& newText);

        /**
         * Adds the given text to the end of this script.
         *
         * \param extraText the text to add.
         */
        void append(const std::string& extraText);

        /**
         * Returns the number of variables associated with this script.
         *
         * \return the number of variables.
         */
        size_t countVariables() const;
        /**
         * Returns the name of the requested variable associated with
         * this script.
         *
         * \param index the index of the requested variable; this must
         * be between 0 and countVariables()-1 inclusive.
         * \return the name of the requested variable.
         */
        const std::string& variableName(size_t index) const;
        /**
         * Returns the index of the variable stored with the given name.
         *
         * \param name the name of the requested variable; note that
         * names are case sensitive.
         * \return the index of the requested variable as an integer
         * between 0 and countVariables()-1 inclusive, or -1 if
         * there is no variable with the given name.
         */
        long variableIndex(const std::string& name) const;
        /**
         * Returns the value of the requested variable associated with
         * this script.  Variables may take the value \c null.
         *
         * \param index the index of the requested variable; this must
         * be between 0 and countVariables()-1 inclusive.
         * \return the value of the requested variable.
         */
        std::shared_ptr<Packet> variableValue(size_t index) const;
        /**
         * Returns the value of the variable stored with the given
         * name.  Variables may take the value \c null.
         *
         * If no variable is stored with the given name, then \c null
         * will likewise be returned.
         *
         * \param name the name of the requested variable; note that
         * names are case sensitive.
         * \return the value of the requested variable.
         */
        std::shared_ptr<Packet> variableValue(const std::string& name) const;

        /**
         * Changes the name of an existing variable associated with
         * this script.
         *
         * \warning This may change the indices of this and other
         * variables, since (at present) variables are kept stored in
         * sorted order by name.
         *
         * \param index the index of the variable whose name should change;
         * this must be between 0 and countVariables()-1 inclusive.
         * \param name the new name to assign to the variable.
         */
        void setVariableName(size_t index, const std::string& name);
        /**
         * Changes the value of an existing variable associated with
         * this script.
         *
         * \param index the index of the variable whose value should change;
         * this must be between 0 and countVariables()-1 inclusive.
         * \param value the new value to assign to the variable.  This is
         * allowed to be a null pointer, and if the argument is omitted then a
         * null pointer will be used.
         */
        void setVariableValue(size_t index, std::weak_ptr<Packet> value = {});

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
         * \param name the name of the new variable.
         * \param value the value of the new variable.  This is allowed
         * to be a null pointer, and if the argument is omitted then a
         * null pointer will be used.
         * \return \c true if the variable was successfully added, or
         * \c false if a variable with the given name was already stored.
         */
        bool addVariable(const std::string& name,
            std::weak_ptr<Packet> value = {});
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
         * \param name the string upon which the new variable name will
         * be based.
         * \param value the value of the new variable.  This is allowed
         * to be a null pointer, and if the argument is omitted then a
         * null pointer will be used.
         * \return the name of the variable that was added; this might
         * or might not be equal to \a name.
         */
        const std::string& addVariableName(const std::string& name,
            std::weak_ptr<Packet> value = {});
        /**
         * Removes the variable stored with the given name.
         * If no variable is stored with the given name, this routine
         * will do nothing.
         *
         * \warning This may change the indices of other variables, since
         * (at present) variables are kept stored in sorted order by name.
         *
         * \param name the name of the variable to remove; note that
         * names are case sensitive.
         */
        void removeVariable(const std::string& name);
        /**
         * Removes the variable stored at the given index.
         *
         * \warning This may change the indices of other variables, since
         * (at present) variables are kept stored in sorted order by name.
         *
         * \param index the index of the variable to remove;
         * this must be between 0 and countVariables()-1 inclusive.
         */
        void removeVariable(size_t index);
        /**
         * Removes all variables associated with this script.
         */
        void removeAllVariables();
        /**
         * Registers the given packet listener to listen for events on
         * all of packets identified by this script's variables.
         *
         * This is a one-off operation: if the script variables should
         * change later on, the given listener will not be registered and/or
         * deregistered to reflect the new variable values.
         *
         * This routine is safe to call if some variables have null values,
         * and/or if the same packet is identified by multiple variables.
         *
         * \param listener the listener to register with all packets
         * identified by the script variables.
         */
        void listenVariables(PacketListener* listener);
        /**
         * Deregisters the given packet listener from listening for events on
         * all of packets identified by this script's variables.
         *
         * Like listenVariables(), this is a one-off operation: it will not
         * make any further adjustments if the script variables should change
         * later on.
         *
         * This routine is safe to call if some variables have null values,
         * and/or if the same packet is identified by multiple variables.
         *
         * \param listener the listener to deregister from all packets
         * identified by the script variables.
         */
        void unlistenVariables(PacketListener* listener);

        /**
         * Determines if this and the given script are identical.
         *
         * Here _identical_ means that both scripts contain exactly the
         * same text, and they have the same set of variables.  For two
         * variables to be considered the same, they must have the same
         * variable name, and their values must be either both \c null or
         * both pointers to the same packet.
         *
         * \param other the script to compare with this.
         * \return \c true if and only if this and the given script are
         * identical.
         */
        bool operator == (const Script& other) const;

        /**
         * Determines if this and the given script are not identical.
         *
         * Here _identical_ means that both scripts contain exactly the
         * same text, and they have the same set of variables.  For two
         * variables to be considered the same, they must have the same
         * variable name, and their values must be either both \c null or
         * both pointers to the same packet.
         *
         * \param other the script to compare with this.
         * \return \c true if and only if this and the given script are
         * not identical.
         */
        bool operator != (const Script& other) const;

        void writeTextShort(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    protected:
        std::shared_ptr<Packet> internalClonePacket() const override;
        void writeXMLPacketData(std::ostream& out, FileFormat format,
            bool anon, PacketRefs& refs) const override;
        void addPacketRefs(PacketRefs& refs) const override;
};

/**
 * Swaps the contents of the given script packets.
 *
 * This global routine simply calls Script::swap(); it is provided so that
 * Script meets the C++ Swappable requirements.
 *
 * \param a the first script packet whose contents should be swapped.
 * \param b the second script packet whose contents should be swapped.
 *
 * \ingroup packet
 */
void swap(Script& a, Script& b);

// Inline functions for Script

inline const std::string& Script::text() const {
    return text_;
}
inline void Script::setText(const std::string& newText) {
    if (text_ == newText)
        return; // No change event fired.

    ChangeEventSpan span(*this);
    text_ = newText;
}

inline void Script::append(const std::string& extraText) {
    if (extraText.empty())
        return; // No change event fired.

    ChangeEventSpan span(*this);
    text_ += extraText;
}

inline size_t Script::countVariables() const {
    return variables_.size();
}

inline bool Script::addVariable(const std::string& name,
        std::weak_ptr<Packet> value) {
    ChangeEventSpan span(*this);
    return variables_.emplace(name, std::move(value)).second;
}
inline void Script::removeAllVariables() {
    ChangeEventSpan span(*this);
    variables_.clear();
}

inline void Script::listenVariables(PacketListener* listener) {
    for (const auto& v : variables_)
        if (auto shared = v.second.lock())
            shared->listen(listener);
}

inline void Script::unlistenVariables(PacketListener* listener) {
    for (const auto& v : variables_)
        if (auto shared = v.second.lock())
            shared->unlisten(listener);
}

inline void Script::addPacketRefs(PacketRefs& refs) const {
    for (const auto& v : variables_)
        if (auto shared = v.second.lock())
            refs.insert({ shared.get(), false });
}

inline bool Script::operator != (const Script& other) const {
    return ! ((*this) == other);
}

inline void swap(Script& a, Script& b) {
    a.swap(b);
}

} // namespace regina

#endif

