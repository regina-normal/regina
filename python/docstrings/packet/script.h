/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Script
static const char *Script =
R"doc(A packet representing a Python script that can be run.

A script consists of two parts: (i) the _text_, which contains the
Python code; and (ii) a set of _variables_, which refer to packets in
your packet tree. When running a script, the variables should be
instantiated in the default namespace before the script is run.

The way variables are stored has changed as of Regina 7.0:

* the value of each variable is now stored as a std::weak_ptr<Packet>;

* scripts do not listen to packet events on their variables.

This means that:

* If a packet is stored as the value of some script variable, this
  does not prevent the packet from being destroyed (since the script
  only holds weak pointers).

* If such a packet changes its packet label or is destroyed, the
  script will not notify its _own_ listeners of the change. If a user
  interface needs to know about this change (e.g., because it shows
  packet labels of script variables in a visual table), it will need
  to set up packet listeners for the individual variables - it is no
  longer enough just to listen on the script itself. Note that packet
  labels are completely independent of the variable names stored in
  the script.

* If such a packet is destroyed, the script will behave correctly: any
  subsequent request to retrieve the value of the script variable will
  return a null pointer (not an invalid pointer to a non-existent
  packet).

For historical reference, the previous behaviour was:

* In Regina 6.0.1 and earlier (before Regina switched to widespread
  use of std::shared_ptr for packets), scripts would notify their own
  listeners when a variable packet was destroyed or changed its packet
  label. This was implemented by having each script listen for packet
  events on its own variables.

* In Regina 4.94 and earlier (before the file format supported cross-
  referencing between packets), script variables were stored purely by
  their packet labels (not pointers to the packets themselves).

Like all packet types, this class does not support C++ move semantics
since this would interfere with the structure of the packet tree. It
does support copy construction, copy assignment and swaps; however,
these operations only copy/swap the mathematical content, not the
packet infrastructure (e.g., they do not touch packet labels, or the
packet tree, or event listeners).)doc";

namespace Script_ {

// Docstring regina::python::doc::Script_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given script packet.

Like all packet types, this only copies the script content, not the
packet infrastructure (e.g., it will not copy the packet label, it
will not clone the given packet's children, and it will not insert the
new packet into any packet tree).)doc";

// Docstring regina::python::doc::Script_::__default
static const char *__default = R"doc(Initialises to a script with no text and no variables.)doc";

// Docstring regina::python::doc::Script_::__eq
static const char *__eq =
R"doc(Determines if this and the given script are identical.

Here _identical_ means that both scripts contain exactly the same
text, and they have the same set of variables. For two variables to be
considered the same, they must have the same variable name, and their
values must be either both ``None`` or both pointers to the same
packet.

Parameter ``other``:
    the script to compare with this.

Returns:
    ``True`` if and only if this and the given script are identical.)doc";

// Docstring regina::python::doc::Script_::addVariable
static const char *addVariable =
R"doc(Attempts to add a new variable to be associated with this script. If a
variable with the given name is already stored, this routine will do
nothing.

If you need to ensure that a new variable is always added, even if the
variable name needs to change, see the routine addVariableName()
instead.

.. warning::
    The index of the new variable might not be countVariables()-1, and
    this operation may change the indices of other variables also.
    This is because (at present) variables are kept stored in sorted
    order by name.

Parameter ``name``:
    the name of the new variable.

Parameter ``value``:
    the value of the new variable. This is allowed to be a null
    pointer, and if the argument is omitted then a null pointer will
    be used.

Returns:
    ``True`` if the variable was successfully added, or ``False`` if a
    variable with the given name was already stored.)doc";

// Docstring regina::python::doc::Script_::addVariableName
static const char *addVariableName =
R"doc(Adds a new variable to be associated with this script, changing its
name if necessary. If the given variable name does not already exist
as a variable name in this script, then it will be used without
modification. Otherwise a new variable name will be constructed by
appending additional characters to *name*.

.. warning::
    The index of the new variable might not be countVariables()-1, and
    this operation may change the indices of other variables also.
    This is because (at present) variables are kept stored in sorted
    order by name.

Parameter ``name``:
    the string upon which the new variable name will be based.

Parameter ``value``:
    the value of the new variable. This is allowed to be a null
    pointer, and if the argument is omitted then a null pointer will
    be used.

Returns:
    the name of the variable that was added; this might or might not
    be equal to *name*.)doc";

// Docstring regina::python::doc::Script_::append
static const char *append =
R"doc(Adds the given text to the end of this script.

Parameter ``extraText``:
    the text to add.)doc";

// Docstring regina::python::doc::Script_::countVariables
static const char *countVariables =
R"doc(Returns the number of variables associated with this script.

Returns:
    the number of variables.)doc";

// Docstring regina::python::doc::Script_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given script packets.

This global routine simply calls Script::swap(); it is provided so
that Script meets the C++ Swappable requirements.

Parameter ``a``:
    the first script packet whose contents should be swapped.

Parameter ``b``:
    the second script packet whose contents should be swapped.)doc";

// Docstring regina::python::doc::Script_::listenVariables
static const char *listenVariables =
R"doc(Registers the given packet listener to listen for events on all of
packets identified by this script's variables.

This is a one-off operation: if the script variables should change
later on, the given listener will not be registered and/or
deregistered to reflect the new variable values.

This routine is safe to call if some variables have null values,
and/or if the same packet is identified by multiple variables.

Parameter ``listener``:
    the listener to register with all packets identified by the script
    variables.)doc";

// Docstring regina::python::doc::Script_::removeAllVariables
static const char *removeAllVariables = R"doc(Removes all variables associated with this script.)doc";

// Docstring regina::python::doc::Script_::removeVariable
static const char *removeVariable =
R"doc(Removes the variable stored with the given name. If no variable is
stored with the given name, this routine will do nothing.

.. warning::
    This may change the indices of other variables, since (at present)
    variables are kept stored in sorted order by name.

Parameter ``name``:
    the name of the variable to remove; note that names are case
    sensitive.)doc";

// Docstring regina::python::doc::Script_::removeVariable_2
static const char *removeVariable_2 =
R"doc(Removes the variable stored at the given index.

.. warning::
    This may change the indices of other variables, since (at present)
    variables are kept stored in sorted order by name.

Parameter ``index``:
    the index of the variable to remove; this must be between 0 and
    countVariables()-1 inclusive.)doc";

// Docstring regina::python::doc::Script_::setText
static const char *setText =
R"doc(Replaces the complete text of this script with the given string.

Variables are not considered part of the text; you can get and set
them through other member functions (see below).

Parameter ``newText``:
    the new text for this script.)doc";

// Docstring regina::python::doc::Script_::setVariableName
static const char *setVariableName =
R"doc(Changes the name of an existing variable associated with this script.

.. warning::
    This may change the indices of this and other variables, since (at
    present) variables are kept stored in sorted order by name.

Parameter ``index``:
    the index of the variable whose name should change; this must be
    between 0 and countVariables()-1 inclusive.

Parameter ``name``:
    the new name to assign to the variable.)doc";

// Docstring regina::python::doc::Script_::setVariableValue
static const char *setVariableValue =
R"doc(Changes the value of an existing variable associated with this script.

Parameter ``index``:
    the index of the variable whose value should change; this must be
    between 0 and countVariables()-1 inclusive.

Parameter ``value``:
    the new value to assign to the variable. This is allowed to be a
    null pointer, and if the argument is omitted then a null pointer
    will be used.)doc";

// Docstring regina::python::doc::Script_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given script packet.

Like all packet types, this only swaps the script content, not the
packet infrastructure (e.g., it will not swap packet labels, or change
either packet's location in any packet tree).

Parameter ``other``:
    the script packet whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Script_::text
static const char *text =
R"doc(Returns the complete text of this script.

Variables are not considered part of the text; you can get and set
them through other member functions (see below).

Returns:
    the complete text of this script.)doc";

// Docstring regina::python::doc::Script_::unlistenVariables
static const char *unlistenVariables =
R"doc(Deregisters the given packet listener from listening for events on all
of packets identified by this script's variables.

Like listenVariables(), this is a one-off operation: it will not make
any further adjustments if the script variables should change later
on.

This routine is safe to call if some variables have null values,
and/or if the same packet is identified by multiple variables.

Parameter ``listener``:
    the listener to deregister from all packets identified by the
    script variables.)doc";

// Docstring regina::python::doc::Script_::variableIndex
static const char *variableIndex =
R"doc(Returns the index of the variable stored with the given name.

Parameter ``name``:
    the name of the requested variable; note that names are case
    sensitive.

Returns:
    the index of the requested variable as an integer between 0 and
    countVariables()-1 inclusive, or -1 if there is no variable with
    the given name.)doc";

// Docstring regina::python::doc::Script_::variableName
static const char *variableName =
R"doc(Returns the name of the requested variable associated with this
script.

Parameter ``index``:
    the index of the requested variable; this must be between 0 and
    countVariables()-1 inclusive.

Returns:
    the name of the requested variable.)doc";

// Docstring regina::python::doc::Script_::variableValue
static const char *variableValue =
R"doc(Returns the value of the requested variable associated with this
script. Variables may take the value ``None``.

Parameter ``index``:
    the index of the requested variable; this must be between 0 and
    countVariables()-1 inclusive.

Returns:
    the value of the requested variable.)doc";

// Docstring regina::python::doc::Script_::variableValue_2
static const char *variableValue_2 =
R"doc(Returns the value of the variable stored with the given name.
Variables may take the value ``None``.

If no variable is stored with the given name, then ``None`` will
likewise be returned.

Parameter ``name``:
    the name of the requested variable; note that names are case
    sensitive.

Returns:
    the value of the requested variable.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

