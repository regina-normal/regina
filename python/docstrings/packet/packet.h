/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::ChildIterator
static const char *ChildIterator =
R"doc(A forward iterator for iterating through all immediate children of a
given packet.

Each iterator will hold a std::shared_ptr to the packet whose children
it is iterating over. This guarantees that the packet will not be
destroyed mid-iteration, but it also means that you must ensure that
you dispose of your iterators once you are finished with them.

Template parameter ``const_``:
    Indicates whether this iterator should offer const or non-const
    access to the child packets.

Python:
    Instead of the C++ interface described here, in Python the classes
    PacketChildren and ChildIterator together implement the Python
    iterable/iterator interface. The class PacketChildren has just the
    single function ``__iter__()``, which returns a ChildIterator;
    then ChildIterator implements ``__next__()``, which either returns
    the next child packet in the iteration or else throws a
    ``StopException`` if there are no more children to return. All
    iteration in Python is non-const (i.e., Python exclusively uses
    the classes where *const_* is ``False``).)doc";

// Docstring regina::python::doc::Packet
static const char *Packet =
R"doc(Represents a packet of information that may be individually edited or
operated upon. Packets are stored in a tree structure, with
child/parent relationships; the root of the tree represents a complete
Regina data file.

There are two types of packets: *innate* packets, and *wrapped*
packets.

* *Innate* packets are only relevant within the context of a data
  file. Examples include containers (which are used to organise the
  packet tree), or scripts (which stores Python code with variables
  bound to other packets in the tree). Each innate packet type is
  represented by its own customised subclass of Packet (e.g.,
  Container or Script).

* *Wrapped* packets hold some other type, which can also act as a
  standalone mathematical object. Examples include packets that hold
  triangulations, links, and normal surface lists. Each wrapped packet
  type is represented by a class of the form PacketOf<Held>, where
  *Held* is the underlying mathematical type (e.g., Triangulation<3>,
  Link, or NormalSurfaces).

Since Regina 7.0, packets are *always* managed by std::shared_ptr.
There are *no* exceptions to this rule. The implication of this are:

* Every new packet *must* be wrapped in a std::shared_ptr immediately
  after construction. It is recommended that you create new packets
  using std::make_shared, not ``new``, so you do not forget this. Many
  of Regina's operations on packets will assume that such a
  std::shared_ptr exists, and will throw std::bad_weak_ptr exceptions
  if it does not.

* When given an existing raw packet pointer (e.g., as a function
  argument), you must *not* wrap it in a new std::shared_ptr. This
  would lead to two shared pointers "independently" claiming ownership
  of the packet (which means the packet would be destroyed earlier
  than expected). If you need to convert a raw Packet* into a
  std::shared_ptr, you can use the member function shared_from_this()
  (inherited from std::enable_shared_from_this).

Parent packets own their children, but children do not own their
parents. This has the following impliciations for destruction:

* If you have a shared pointer to the root of the packet tree, then
  this will keep the entire packet tree alive.

* To destroy a child packet but not its parent, just call makeOrphan()
  on the child. This will remove the child from the tree, and then
  destroy the child once your last shared pointer to it goes out of
  scope.

* If you destroy a parent but you are also holding another shared
  pointer to one of its children, then that child will *not* be
  destroyed. It will instead become orphaned, and will become the root
  of its own (smaller) packet tree.

The old Packet::safeDelete() function has been removed, since you
should not be manually destroying packets at all now (instead just
wait for the last std::shared_ptr to be destroyed).

There are different requirements when creating a new packet type.

To create a new innate packet type:

* Add a new type constant to the PacketType enum;

* Add corresponding cases to the routines in PacketInfo;

* Create a new subclass *C* of Packet, which begins with the
  REGINA_PACKET macro and implements all pure virtual functions
  (except for those already provided by REGINA_PACKET);

* Declare and implement a copy constructor, copy assignment operator,
  and member/global swap functions, whose arguments use the derived
  class *C* (not the base class Packet);

* Add an appropriate case to XMLPacketReader::startSubElement(), to
  support reading from file;

* For every routine in *C* that edits the packet contents, declare a
  Packet::ChangeEventSpan on the stack while the modification takes
  place so that listeners are notified (see the discussion below on
  event listeners).

To create a new wrapped packet type that holds an object of type
*Held:*

* Add a new type constant *T* to the PacketType enum;

* Add a specialisation of the template constant packetTypeHolds<Held>,
  which should take the value *T*;

* Add corresponding cases to the routines in PacketInfo;

* Add PacketData<Held> as a new base class for *Held* (this is very
  lightweight, just adding a single enum variable);

* Add specialisations that implement the routines in XMLWriter<Held>,
  to support writing to file;

* Add an appropriate case to XMLPacketReader::startSubElement(), to
  support reading from file;

* For every routine in *Held* that edits the packet contents, declare
  a Held::ChangeEventSpan on the stack while the modification takes
  place. This is again lightweight (if an object does not belong to a
  packet then the cost is just two integer comparisions), and it will
  ensure that if the object *does* belong to a packet then listeners
  are notified.

External objects can listen for events on packets, such as when
packets are changed or about to be destroyed. This is useful (for
example) when keeping a graphical user interface in sync with any
changes that might be happening within the engine and/or via users'
python scripts. See the PacketListener class notes for details.

Regina's packet types do not support C++ move semantics, since this
would interfere with the structure of the packet tree. They do support
copy construction, copy assignment and swaps, but only in the derived
packet classes (e.g., you cannot assign from the polymorphic base
class Packet). Moreover, these operations only copy/swap the
mathematical content, not the packet infrastructure (e.g., they do not
touch packet labels, or the packet tree, or event listeners).)doc";

// Docstring regina::python::doc::PacketChildren
static const char *PacketChildren =
R"doc(A lightweight object that gives access to all immediate children of a
given packet.

The purpose of this class is to support iteration through all children
of a packet *p* using C++11 range-based ``for`` loops:

```
std::shared_ptr<Packet> parent = ...;
for (Packet& child : parent->children()) { ... }
```

In Python, PacketChildren is an iterable object:

```
parent = ...
for child in parent.children():
    ...
```

Each object of this class will hold a std::shared_ptr to the packet
whose children it gives access to. This guarantees that the packet
will not be destroyed during iteration, but it also means that you
must ensure that you dispose of these objects once you are finished
with them.

These are lightweight objects, small enough to pass by value and swap
with std::swap(), with no need for any specialised move operations or
swap functions. Copies of a PacketChildren will iterate over the
children of the same underlying packet.

Template parameter ``const_``:
    Indicates whether this iterator should offer const or non-const
    access to the child packets.

Python:
    Instead of the C++ interface described here, in Python the classes
    PacketChildren and ChildIterator together implement the Python
    iterable/iterator interface. The class PacketChildren has just the
    single function ``__iter__()``, which returns a ChildIterator;
    then ChildIterator implements ``__next__()``, which either returns
    the next child packet in the iteration or else throws a
    ``StopException`` if there are no more children to return. All
    iteration in Python is non-const (i.e., Python exclusively uses
    the classes where *const_* is ``False``).)doc";

// Docstring regina::python::doc::PacketData
static const char *PacketData =
R"doc(A lightweight helper class that allows an object of type *Held* to
connect with the wrapped packet class that contains it.

For every wrapped packet type of the form PacketOf<Held>, the
corresponding class *Held* must derive from PacketData<Held>. See the
Packet class notes for more information about packets, and for what
else must be implemented for each wrapped packet type.

This base class is extremely lightweight: the only data that it
contains is a single PacketHeldBy enumeration value. All of the class
constructors set this value to HELD_BY_NONE; it is the responsibility
of subclasses (e.g., PacketOf<Held>) to change this where necessary.

Python:
    Not present, but the routines anonID() and packet() will be
    provided directly through the various subclasses.)doc";

// Docstring regina::python::doc::PacketDescendants
static const char *PacketDescendants =
R"doc(A lightweight object that gives access to all strict descendants of a
given packet.

The purpose of this class is to support iteration through all strict
descendants of a packet *p* using C++11 range-based ``for`` loops:

```
std::shared_ptr<Packet> parent = ...;
for (Packet& desc : parent->descendants()) { ... }
```

In Python, PacketDescendants is an iterable object:

```
parent = ...
for desc in parent.descendants():
    ...
```

Each object of this class will hold a std::shared_ptr to the packet
whose descendants it gives access to. This guarantees that the packet
will not be destroyed during iteration, but it also means that you
must ensure that you dispose of these objects once you are finished
with them.

These are lightweight objects, small enough to pass by value and swap
with std::swap(), with no need for any specialised move operations or
swap functions. Copies of a PacketDescendants will iterate over the
descendants of the same underlying packet.

Template parameter ``const_``:
    Indicates whether this iterator should offer const or non-const
    access to the packet tree.

Python:
    Instead of the C++ interface described here, in Python the classes
    PacketDescendants and SubtreeIterator together implement the
    Python iterable/iterator interface. The class PacketDescendants
    has just the single function ``__iter__()``, which returns a
    SubtreeIterator; then SubtreeIterator implements ``__next__()``,
    which either returns the next descendant packet in the iteration
    or else throws a ``StopException`` if there are no more children
    to return. All iteration in Python is non-const (i.e., Python
    exclusively uses the classes where *const_* is ``False``).)doc";

// Docstring regina::python::doc::PacketListener
static const char *PacketListener =
R"doc(An object that can be registered to listen for packet events.

A packet listener can be registered to listen for events on a packet
by calling Packet::listen().

Each time that one of the events listed in this class occurs, the
packet will call the appropriate callback routine for all registered
listeners.

These events come in future/past pairs: packetToBeChanged() and
packetWasChanged(), childToBeAdded() and childWasAdded(), and so on.
These event pairs are mutually exclusive: any event will cause at most
one pair of callback routines to be called for each (packet, listener)
pair. For instance, if a packet is renamed then packetToBeRenamed()
and packetWasRenamed() will be called but packetToBeChanged() and
packetWasChanged() will not.

As a special case, when a packet is destroyed there is only the one
event packetBeingDestroyed(), since this is called *during* the packet
destructor (at a time when the set of listeners is still available,
but some of the other packet data may have already been destroyed).

No guarantees are made as to the order in which the different packet
listeners are notified of an event.

When a listener is destroyed, it is automatically unregistered from
any packets to which it is currently listening. Similarly, when a
packet is destroyed all listeners are automatically unregistered.

To listen for packet events using your own callback routines, you
would typically implement a subclass of PacketListener that overrides
only those callbacks that you are interested in. Be aware that:

* Callbacks are called for each listener, one at a time, in the same
  thread in which the event occurred.

* Callbacks can safely add new packet listeners, but there is no
  guarantee as to whether or not the new listeners will be notified of
  the specific event currently being processed.

* Callbacks can safely remove other listeners, but they must *not*
  remove the listener whose callback is currently being called. The
  one exception to this is packetBeingDestroyed(), which will
  explicitly remove each listener *before* its callback is called
  (which means, for example, the listener can safely delete itself).

.. warning::
    Subclass authors should be aware of the default copy semantics
    that this base class provides. In particular, this base class
    provides a protected copy constructor and copy assignment operator
    that will change which packets are being listened to (in the
    "obvious" way). As a subclass author, you should understand this
    inherited behaviour if your subclass constructors and/or
    assignment operators use these base class operations implicitly.

.. warning::
    At the time of writing (admittedly long ago now), Qt has only
    limited support for multithreading. When working with an existing
    packet tree in a new thread (not the main thread), the *only*
    modification that you may make is to insert new packets.
    Modifications of any other type (such as changing, renaming,
    deleting or reordering existing packets) could lead to a crash
    within Qt or Xlib when running the GUI. Of course, a new thread
    may create, modify and delete its own temporary packet trees as it
    chooses (and it may in fact insert them into a pre-existing packet
    tree once all modifications are completed). Assuming these
    restrictions are respected, packet listeners may assume that no
    routines other than childWasAdded() will be called from a non-main
    thread.

Python:
    You can happily make a pure Python subclass of PacketListener, and
    packets will call whichever functions you override when events
    occur, just as they would for a native C++ subclass.)doc";

// Docstring regina::python::doc::PacketOf
static const char *PacketOf =
R"doc(A packet that stores a mathematical object of type *Held*.

This is the class used for all of Regina's *wrapped packet types*. See
the Packet class notes for general information about packets, and
about the differences between *wrapped* and *innate* packet types.

You can use a PacketOf<Held> in much the same way as you can use a
"raw" object of type *Held*. This class inherits the full interface
from *Held:* you can query and manipulate its objects in the same way,
and (using the std::in_place constructors) you can create them in the
same way also.

There are some important differences, however:

* The *Held* class will typically support full value semantics, with
  full support for copying, moving, and swapping (as is now common
  across most of Regina's API). In contrast, due the constraints of
  working with the packet tree, PacketOf<Held> objects are typically
  passed by pointer, copies and swaps do not touch the tree structure,
  and moves are not supported at all.

* The *Held* class will typically *not* be polymorphic. In contrast,
  PacketOf<Held> aquires polymorphism through its inherited Packet
  interface.

Like all packet types, this class does not support C++ move semantics
since this would interfere with the structure of the packet tree. It
does support copy construction, copy assignment and swaps; however,
these operations only copy/swap the mathematical content, not the
packet infrastructure (e.g., they do not touch packet labels, or the
packet tree, or event listeners).

Python:
    Since Python does not support templates, this class will have a
    name of the form PacketOfHeld. For example, the C++ class Link is
    wrapped by the Python class ``PacketOfLink``, and the C++ class
    Triangulation<3> is wrapped by the Python class
    ``PacketOfTriangulation3``.)doc";

// Docstring regina::python::doc::PacketShell
static const char *PacketShell =
R"doc(Gives access to the final remains of a packet that is in the process
of being destroyed. The main use of this class is to pass packet
details to the callback function
PacketListener::packetBeingDestroyed().

All functions in this class mirror the corresponding Packet functions,
and are safe to call during PacketListener::packetBeingDestroyed().

This class works with raw Packet pointers, not std::shared_ptr,
because it typically only becomes relevant when a Packet is already in
the process of being destroyed (and so it is improper for a new shared
pointer to suddenly appear and try to claim ownership of the packet
again).

These are lightweight objects, small enough to pass by value and swap
with std::swap(), with no need for any specialised move operations or
swap functions. Copies of a PacketShell will give access to the
remains of the same underlying packet.)doc";

// Docstring regina::python::doc::SubtreeIterator
static const char *SubtreeIterator =
R"doc(A forward iterator for iterating through the entire packet subtree
rooted at a given packet.

The order of iteration is depth-first, where a parent packet is always
processed before its descendants.

Each iterator will hold a std::shared_ptr to the packet whose subtree
it is iterating over. This guarantees that the packet will not be
destroyed mid-iteration, but it also means that you must ensure that
you dispose of your iterators once you are finished with them.

Template parameter ``const_``:
    Indicates whether this iterator should offer const or non-const
    access to the packet tree.

Python:
    Instead of the C++ interface described here, in Python this class
    implements the Python iterable/iterator interface. It implements
    the function ``__iter__()``, which returns the iterator object
    itself; it also implements ``__next__()``, which either returns
    the next packet in the subtree iteration or else throws a
    ``StopException`` if there are no more packets to return. All
    iteration in Python is non-const (i.e., Python exclusively uses
    the classes where *const_* is ``False``).)doc";

// Docstring regina::python::doc::__eq
static const char *__eq =
R"doc(Identifies if the given shell refers to the given packet.

This test is also available the other way around (with PacketShell on
the left); this reversed test is defined as a member function of
PacketShell.

Parameter ``packet``:
    the packet to test against; this may be ``null``.

Parameter ``shell``:
    the packet shell to test against.

Returns:
    ``True`` if and only if the given shell refers to the given
    packet.)doc";

// Docstring regina::python::doc::__ne
static const char *__ne =
R"doc(Identifies if the given shell does not refer to the given packet.

This test is also available the other way around (with PacketShell on
the left); this reversed test is defined as a member function of
PacketShell.

Parameter ``packet``:
    the packet to test against; this may be ``null``.

Parameter ``shell``:
    the packet shell to test against.

Returns:
    ``True`` if and only if the given shell does not refer to the
    given packet.)doc";

// Docstring regina::python::doc::make_packet
static const char *make_packet =
R"doc(Converts a temporary *Held* object into a new wrapped packet, without
making a deep copy. The data will be moved out of *src* (using the
*Held* move constructor).

The packet that is returned will be newly created, and will have no
packet label.

.. note::
    This function is trivial (it just calls a single move
    constructor). Nevertheless, using this function is recommended
    since it makes it clear (and searchable) that you are correctly
    wrapping the new packet in a std::shared_ptr, as is required for
    all packets in Regina.

Python:
    The *src* argument is a const reference, and this routine makes a
    deep copy of *src*. This is because Python will still maintain a
    reference to *src*, and so it is not possible to move from *src*.

Parameter ``src``:
    the *Held* object that will be moved into the new packet; this
    will become unusable after this function returns.

Returns:
    the new wrapped packet.)doc";

// Docstring regina::python::doc::make_packet_2
static const char *make_packet_2 =
R"doc(Converts a temporary *Held* object into a new wrapped packet, without
making a deep copy. The data will be moved out of *src* (using the
*Held* move constructor).

The packet that is returned will be newly created, and will have the
given packet label.

.. note::
    This function is trivial (it just calls a single move constructor
    and then Packet::setLabel()). Nevertheless, using this function is
    recommended since it makes it clear (and searchable) that you are
    correctly wrapping the new packet in a std::shared_ptr, as is
    required for all packets in Regina.

Python:
    The *src* argument is a const reference, and this routine makes a
    deep copy of *src*. This is because Python will still maintain a
    reference to *src*, and so it is not possible to move from *src*.

Parameter ``src``:
    the *Held* object that will be moved into the new packet; this
    will become unusable after this function returns.

Parameter ``label``:
    the label to assign to the new packet.

Returns:
    the new wrapped packet.)doc";

// Docstring regina::python::doc::open
static const char *open =
R"doc(Reads a Regina data file, and returns the corresponding packet tree.
This uses Regina's native XML file format; it does not matter whether
the XML file is compressed or uncompressed.

If the file could not be opened or the top-level packet in the tree
could not be read, this routine will return ``null``. If some packet
deeper within the tree could not be read then that particular packet
(and its descendants, if any) will simply be ignored.

Internationalisation:
    This routine makes no assumptions about the i18n "character
    encoding" used in the given file *name*, and simply passes it
    through unchanged to low-level C/C++ file I/O routines.

Python:
    This function is not automatically imported into the global
    namespace when running regina-python, or when opening a Python
    console in the graphical user interface, or even when typing
    ``from regina import *``. This is to avoid overriding Python's own
    built-in open() function. You can access Regina's open() function
    by calling ``regina.open()``.

Parameter ``filename``:
    the pathname of the file to read from.

Returns:
    the packet tree read from file, or ``null`` on error.)doc";

namespace ChildIterator_ {

// Docstring regina::python::doc::ChildIterator_::__eq
static const char *__eq =
R"doc(Tests whether this and the given iterator are equal.

Returns:
    true if and only if the two iterators are equal.)doc";

// Docstring regina::python::doc::ChildIterator_::__ne
static const char *__ne =
R"doc(Tests whether this and the given iterator are different.

Returns:
    true if and only if the two iterators are different.)doc";

// Docstring regina::python::doc::ChildIterator_::__next__
static const char *__next__ =
R"doc(Returns the current child packet and increments this iterator.

Exception ``StopIteration``:
    The iterator is already past-the-end when this function is called.

Returns:
    the child packet that this iterator is pointing to, before the
    increment takes place.)doc";

}

namespace PacketChildren_ {

// Docstring regina::python::doc::PacketChildren_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object are designed to iterate
over children of the same parent packet.

Returns:
    ``True`` if and only if this object and *rhs* iterate over
    children of the same packet.)doc";

// Docstring regina::python::doc::PacketChildren_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over all immediate child packets.

Returns:
    an iterator over all immediate child packets.)doc";

// Docstring regina::python::doc::PacketChildren_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object are designed to iterate
over children of different parent packets.

Returns:
    ``True`` if and only if this object and *rhs* iterate over
    children of different packets.)doc";

}

namespace PacketData_ {

// Docstring regina::python::doc::PacketData_::__copy
static const char *__copy =
R"doc(Copy constructor that ignores its argument, and instead sets *heldBy_*
to HELD_BY_NONE. This is because *heldBy_* stores information about
the C++ type of *this* object, not the object being copied.

This constructor is provided so that *Held* can (if it wants) use an
implicitly-declared copy or move constructor.)doc";

// Docstring regina::python::doc::PacketData_::__default
static const char *__default = R"doc(Default constructor that sets *heldBy_* to HELD_BY_NONE.)doc";

// Docstring regina::python::doc::PacketData_::anonID
static const char *anonID =
R"doc(A unique string ID that can be used in place of a packet ID.

This is an alternative to Packet::internalID(), and is designed for
use when *Held* is not actually wrapped by a PacketOf<Held>. (An
example of such a scenario is when a normal surface list needs to
write its triangulation to file, but the triangulation is a standalone
object that is not stored in a packet.)

The ID that is returned will:

* remain fixed throughout the lifetime of the program for a given
  object, even if the contents of the object are changed;

* not clash with the anonID() returned from any other object, or with
  the internalID() returned from any packet of any type;

These IDs are *not* preserved when copying or moving one object to
another, and are not preserved when writing to a Regina data file and
then reloading the file contents.

.. warning::
    If this object *is* wrapped in a PacketOf<Held>, then anonID() and
    Packet::internalID() may return *different* values.

See Packet::internalID() for further details.

Returns:
    a unique ID that identifies this object.)doc";

// Docstring regina::python::doc::PacketData_::packet
static const char *packet =
R"doc(Returns the packet that holds this data, if there is one.

If this object is being held by a packet *p* of type PacketOf<Held>,
then that packet *p* will be returned. Otherwise, if this is a
"standalone" object of type Held, then this routine will return
``null``.

There is a special case when dealing with a packet *q* that holds a
SnapPea triangulation. Here *q* is of type
PacketOf<SnapPeaTriangulation>, and it holds a Triangulation<3>
"indirectly" in the sense that Packetof<SnapPeaTriangulation> derives
from SnapPeaTriangulation, which in turn derives from
Triangulation<3>. In this scenario:

* calling Triangulation<3>::packet() will return ``null``, since there
  is no "direct" PacketOf<Triangulation<3>>;

* calling SnapPeaTriangulation::packet() will return the enclosing
  packet *q*, since there is a PacketOf<SnapPeaTriangulation>;

* calling the special routine Triangulation<3>::inAnyPacket() will
  also return the "indirect" enclosing packet *q*.

The function inAnyPacket() is specific to Triangulation<3>, and is not
offered for other *Held* types.

Returns:
    the packet that holds this data, or ``null`` if this data is not
    (directly) held by a packet.)doc";

// Docstring regina::python::doc::PacketData_::packet_2
static const char *packet_2 =
R"doc(Returns the packet that holds this data, if there is one.

See the non-const version of this function for further details, and in
particular for how this functions operations in the special case of a
packet that holds a SnapPea triangulation.

Returns:
    the packet that holds this data, or ``null`` if this data is not
    (directly) held by a packet.)doc";

}

namespace PacketDescendants_ {

// Docstring regina::python::doc::PacketDescendants_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object are designed to iterate
over strict descendants of the same packet.

Returns:
    ``True`` if and only if this object and *rhs* iterate over
    descendants of the same packet.)doc";

// Docstring regina::python::doc::PacketDescendants_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over all strict descendant packets.

Returns:
    an iterator over all strict descendant packets.)doc";

// Docstring regina::python::doc::PacketDescendants_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object are designed to iterate
over strict descendants of different packets.

Returns:
    ``True`` if and only if this object and *rhs* iterate over
    descendants of different packets.)doc";

}

namespace PacketListener_ {

// Docstring regina::python::doc::PacketListener_::childToBeAdded
static const char *childToBeAdded =
R"doc(Called before a child packet is to be inserted directly beneath the
packet. Once the child is inserted, childWasAdded() will be called
also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.

Parameter ``child``:
    the child packet to be added.)doc";

// Docstring regina::python::doc::PacketListener_::childToBeRemoved
static const char *childToBeRemoved =
R"doc(Called before a child packet is to be removed from directly beneath
the packet. Once the child is removed, childWasRemoved() will be
called also.

Since Regina 7.0, this routine is no longer called from within either
the parent or child packet's destructor. In particular, when a parent
packet is destroyed, although it orphans all of its children as part
of the destruction process, it does not call childToBeRemoved() or
childWasRemoved when this happens.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to, or ``null`` if this routine is being
    called from within this packet's destructor.

Parameter ``child``:
    the child packet to be removed.)doc";

// Docstring regina::python::doc::PacketListener_::childToBeRenamed
static const char *childToBeRenamed =
R"doc(Called before one of this packet's immediate children has its label or
tags changed. Before this change, childToBeRenamed() will be called
also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.

Parameter ``child``:
    the child packet to be renamed.

See also:
    packetToBeRenamed())doc";

// Docstring regina::python::doc::PacketListener_::childWasAdded
static const char *childWasAdded =
R"doc(Called after a child packet has been inserted directly beneath the
packet. Before this child is added, childToBeAdded() will be called
also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.

Parameter ``child``:
    the child packet that was added.)doc";

// Docstring regina::python::doc::PacketListener_::childWasRemoved
static const char *childWasRemoved =
R"doc(Called after a child packet has been removed from directly beneath the
packet. Before the child is removed, childToBeRemoved() will be called
also.

Since Regina 7.0, this routine is no longer called from within either
the parent or child packet's destructor. In particular, when a parent
packet is destroyed, although it orphans all of its children as part
of the destruction process, it does not call childToBeRemoved() or
childWasRemoved when this happens.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to, or ``null`` if this routine is being
    called from within this packet's destructor.

Parameter ``child``:
    the child packet that was removed.)doc";

// Docstring regina::python::doc::PacketListener_::childWasRenamed
static const char *childWasRenamed =
R"doc(Called after one of this packet's immediate children has its label or
tags changed. Before this change, childToBeRenamed() will be called
also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.

Parameter ``child``:
    the child packet that was renamed.

See also:
    packetWasRenamed())doc";

// Docstring regina::python::doc::PacketListener_::childrenToBeReordered
static const char *childrenToBeReordered =
R"doc(Called before the child packets directly beneath the packet are to be
reordered. Once the reordering is done, childrenWereReordered() will
be called also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.)doc";

// Docstring regina::python::doc::PacketListener_::childrenWereReordered
static const char *childrenWereReordered =
R"doc(Called after the child packets directly beneath the packet have been
reordered. Before this reordering is done, childrenToBeReordered()
will be called also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.)doc";

// Docstring regina::python::doc::PacketListener_::isListening
static const char *isListening =
R"doc(Determines whether this object is listening for events on any packets
at all.

Returns:
    ``True`` if and only if this object is listening on at least one
    packet.)doc";

// Docstring regina::python::doc::PacketListener_::packetBeingDestroyed
static const char *packetBeingDestroyed =
R"doc(Called as the packet is being destroyed.

By the time this function is called, we are already inside the Packet
destructor, and so most Packet member functions are no longer safe to
call. Therefore the argument that is passed to this routine is a
PacketShell, which exposes only those member functions of Packet that
are still safe to call at this time. Importantly, you can safely
compare a PacketShell against a Packet pointer, in case you need to
identify which particular packet is being destroyed.

When a packet is destroyed, it will automatically unregister each
listener *before* calling packetBeingDestroyed() on that listener.
Therefore, for this (and only this) callback, it is safe for a
listener to unregister itself (since this will be a harmless operation
that does nothing). In particular, this makes it safe for a listener
to delete itself during this callback.

When an entire packet subtree is to be destroyed, child packets will
notify their listeners of the impending destruction before parent
packets will.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    gives access to the packet being listened to.)doc";

// Docstring regina::python::doc::PacketListener_::packetToBeChanged
static const char *packetToBeChanged =
R"doc(Called before the contents of the packet are to be changed. Once the
contents are changed, packetWasChanged() will be called also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.)doc";

// Docstring regina::python::doc::PacketListener_::packetToBeRenamed
static const char *packetToBeRenamed =
R"doc(Called before the packet label or tags are to be changed. Once the
label or tags are changed, packetWasRenamed() will be called also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.

See also:
    childToBeRenamed())doc";

// Docstring regina::python::doc::PacketListener_::packetWasChanged
static const char *packetWasChanged =
R"doc(Called after the contents of the packet have been changed. Before the
contents are changed, packetToBeChanged() will be called also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.)doc";

// Docstring regina::python::doc::PacketListener_::packetWasRenamed
static const char *packetWasRenamed =
R"doc(Called after the packet label or tags have been changed. Before the
label or tags are changed, packetToBeRenamed() will be called also.

The default implementation of this routine is to do nothing.

Parameter ``packet``:
    the packet being listened to.

See also:
    childWasRenamed())doc";

// Docstring regina::python::doc::PacketListener_::unlisten
static const char *unlisten =
R"doc(Unregisters this listener from all packets to which it is currently
listening.)doc";

}

namespace PacketOf_ {

// Docstring regina::python::doc::PacketOf_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given packet.

Like all packet types, this only copies the mathematical content, not
the packet infrastructure (e.g., it will not copy the packet label, it
will not clone the given packet's children, and it will not insert the
new packet into any packet tree).

Parameter ``src``:
    the packet whose contents should be copied.)doc";

// Docstring regina::python::doc::PacketOf_::__default
static const char *__default =
R"doc(Creates a new packet.

The *Held* object that it contains will be constructed using the
default *Held* constructor.

The packet will not be inserted into any packet tree, and will have an
empty packet label.)doc";

// Docstring regina::python::doc::PacketOf_::__init
static const char *__init =
R"doc(Creates a new packet containing a deep copy of the given data.

The packet will not be inserted into any packet tree, and will have an
empty packet label.

Parameter ``data``:
    the object to copy.)doc";

// Docstring regina::python::doc::PacketOf_::__init_2
static const char *__init_2 =
R"doc(Moves the given data into this new packet. This will typically be much
faster than a deep copy, since it uses the move constructor for
*Held*.

The packet will not be inserted into any packet tree, and will have an
empty packet label.

The object that is passed (*data*) will no longer be usable.

Parameter ``data``:
    the object to move.)doc";

// Docstring regina::python::doc::PacketOf_::__init_3
static const char *__init_3 =
R"doc(Creates a new packet using one of *Held*'s own constructors.

The given arguments *args* will be forwarded directly to the
appropriate *Held* constructor (using C++ perfect forwarding).

The initial argument should just be ``std::in_place``; this is so the
compiler can disambiguate between these "forwarding constructors" and
the other constructors for PacketOf<Held>.

The new packet will not be inserted into any packet tree, and will
have an empty packet label.

Python:
    The initial ``std::in_place`` argument is not present. Just use
    PacketOf(args...).

Parameter ``args``:
    the arguments to be forwarded to the appropriate *Held*
    constructor.)doc";

}

namespace PacketShell_ {

// Docstring regina::python::doc::PacketShell_::__copy
static const char *__copy =
R"doc(Creates a copy of the given shell. Both shells will refer to the same
underlying packet.

Parameter ``shell``:
    the shell to clone.)doc";

// Docstring regina::python::doc::PacketShell_::__eq
static const char *__eq =
R"doc(Identifies if this and the given shell refer to the same underlying
packet.

Parameter ``shell``:
    the shell to compare with this.

Returns:
    ``True`` if and only if both shells refer to the same packet.)doc";

// Docstring regina::python::doc::PacketShell_::__eq_2
static const char *__eq_2 =
R"doc(Identifies if this shell refers to the given packet.

This test is also available the other way around (with PacketShell on
the right); this reversed test is defined as a global function.

Parameter ``packet``:
    the packet to test against; this may be ``null``.

Returns:
    ``True`` if and only if this shell refers to the given packet.)doc";

// Docstring regina::python::doc::PacketShell_::__init
static const char *__init =
R"doc(Creates a new shell referring to the given packet.

Parameter ``packet``:
    the packet to refer to.)doc";

// Docstring regina::python::doc::PacketShell_::__ne
static const char *__ne =
R"doc(Identifies if this and the given shell refer to different underlying
packets.

Parameter ``shell``:
    the shell to compare with this.

Returns:
    ``True`` if and only if both shells refer to different packets.)doc";

// Docstring regina::python::doc::PacketShell_::__ne_2
static const char *__ne_2 =
R"doc(Identifies if this shell does not refer to the given packet.

This test is also available the other way around (with PacketShell on
the right); this reversed test is defined as a global function.

Parameter ``packet``:
    the packet to test against; this may be ``null``.

Returns:
    ``True`` if and only if this shell does not refer to the given
    packet.)doc";

// Docstring regina::python::doc::PacketShell_::hasTag
static const char *hasTag =
R"doc(Determines whether this packet has the given associated tag.

See Packet::tags() for further details on packet tags.

Parameter ``tag``:
    the tag to search for.

Returns:
    ``True`` if the given tag is found, ``False`` otherwise.)doc";

// Docstring regina::python::doc::PacketShell_::hasTags
static const char *hasTags =
R"doc(Determines whether this packet has any associated tags at all.

See Packet::tags() for further details on packet tags.

Returns:
    ``True`` if this packet has any tags, ``False`` otherwise.)doc";

// Docstring regina::python::doc::PacketShell_::humanLabel
static const char *humanLabel =
R"doc(Returns the label associated with this individual packet, adjusted if
necessary for human-readable output.

See Packet::label() and Packet::humanLabel() for further details on
packet labels.

Returns:
    this individual packet's label.)doc";

// Docstring regina::python::doc::PacketShell_::internalID
static const char *internalID =
R"doc(Returns a unique string ID that identifies this packet.

The user has no control over this ID and it is not human readable, but
it is guaranteed to be unique to this packet, and to remain fixed
throughout the lifetime of the program for this packet.

See Packet::internalID() for further details.

Returns:
    a unique ID that identifies this packet.)doc";

// Docstring regina::python::doc::PacketShell_::label
static const char *label =
R"doc(Returns the label associated with this individual packet.

See Packet::label() and Packet::humanLabel() for further details on
packet labels.

The reference that is returned should be only used as a temporary,
since the underlying packet (and therefore the string that is
referenced) is in the process of being destroyed.

Returns:
    this individual packet's label.)doc";

// Docstring regina::python::doc::PacketShell_::tags
static const char *tags =
R"doc(Returns the set of all tags associated with this packet.

See Packet::tags() for further details on packet tags.

The reference that is returned should be only used as a temporary,
since the underlying packet (and therefore the set that is referenced)
is in the process of being destroyed.

Python:
    This routine returns a python set.

Returns:
    the set of all tags associated with this packet.)doc";

}

namespace Packet_ {

// Docstring regina::python::doc::Packet_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over the packets in the subtree rooted at
this packet.

Subtree iteration is depth-first, where a parent packet is always
processed before its descendants. In particular, this packet (as the
root of the subtree) will be processed first.

In Python, each packet can be treated as an iterable object, with the
effect of iterating through the corresponding subtree:

```
subtree = ...
for p in subtree:
    ...
```

Regina also supplies Python users with a member function
``Packet.subtree()``, which returns an iterable object. Iterating over
``Packet.subtree()`` is exactly the same as iterating the packet
itself; the ``subtree()`` function is offered because the intention
may be clearer for readers.

See also descendants() for iterating through just the strict
descendants in the subtree (i.e., excluding this packet itself), and
children() for iterating just through the immediate children of this
packet (not the full subtree).

Returns:
    an iterator over the subtree rooted at this packet.)doc";

// Docstring regina::python::doc::Packet_::addTag
static const char *addTag =
R"doc(Associates the given tag with this packet.

Each packet can have an arbitrary set of string tags associated with
it. The tags are not used by this calculation engine; the feature is
provided for whatever use a developer or user chooses to make of it.

Tags are case-sensitive. Tags associated with a single packet must be
distinct, i.e., a particular tag cannot be associated more than once
with the same packet.

Precondition:
    The given tag is not the empty string.

Parameter ``tag``:
    the tag to add.

Returns:
    ``True`` if the given tag was successfully added, or ``False`` if
    the given tag was already present beforehand.)doc";

// Docstring regina::python::doc::Packet_::adornedLabel
static const char *adornedLabel =
R"doc(Returns the label of this packet adorned with the given string.

An adornment typically shows how a packet has been created and/or
modified. For instance, the *adornment* argument might be "Filled", or
"Summand #1".

The way in which the packet label is adorned depends upon the label
itself (in particular, an empty packet label will be handled in a
sensible way). The way in which the packet label is adorned is subject
to change in future versions of Regina.

Note that, whilst this routine returns a modified version of the
packet label, the label itself will not be permamently changed.

Parameter ``adornment``:
    the string that will be used to adorn this packet label. The
    adornment should just be a piece of English, ideally beginning
    with an upper-case letter. It should not contain any surrounding
    punctuation such as brackets or a dash (this will be added
    automatically by this routine as required).

Returns:
    a copy of the packet label with the given adornment.)doc";

// Docstring regina::python::doc::Packet_::append
static const char *append =
R"doc(Inserts the given packet as the last child of this packet.

This packet will take ownership of *child*, in the sense that every
parent packet stores (either directly or indirectly) a shared_ptr to
every one of its descendants in the packet tree.

In Regina 7.0 and earlier, this routine was called insertChildLast().

This routine takes small constant time.

Precondition:
    The given child has no parent packet.

Precondition:
    This packet is not a descendant of the given child.

Exception ``InvalidArgument``:
    The argument *child* already has a parent packet. Note that,
    although this tests *one* of our preconditions, there are other
    preconditions that are *not* tested, and for which no exceptions
    are thrown.

Parameter ``child``:
    the child to insert.)doc";

// Docstring regina::python::doc::Packet_::children
static const char *children =
R"doc(Returns a lightweight object for iterating through the immediate
children of this packet.

This routine allows you to iterate through the immediate children of a
given packet using C++11 range-based ``for`` loops:

```
std::shared_ptr<Packet> parent = ...;
for (Packet& child : parent->children()) { ... }
```

In Python, this routine returns an iterable object:

```
parent = ...
for child in parent.children():
    ...
```

This function returns a lightweight object in the sense that it does
not generate a full list of children in advance, but instead just
returns a small iterator that visits each child as required. In
particular, this routine has small constant time and memory.

Since Regina 7.0, the return type is templated in order to support
both const and non-const iteration. It is recommended that you just
use ``auto`` if you need to store a local copy of the returned object.

See begin() and end(), as well as descendants(), for iterating through
the subtree rooted at this packet (not just the immediate children).

Returns:
    an object for iterating through the children of this packet.)doc";

// Docstring regina::python::doc::Packet_::children_2
static const char *children_2 =
R"doc(Returns a lightweight object for iterating through the immediate
children of this packet.

This routine allows you to iterate through the immediate children of a
given packet using C++11 range-based ``for`` loops:

```
std::shared_ptr<const Packet> parent = ...;
for (const Packet& child : parent->children()) { ... }
```

This function returns a lightweight object in the sense that it does
not generate a full list of children in advance, but instead just
returns a small iterator that visits each child as required. In
particular, this routine has small constant time and memory.

Since Regina 7.0, the return type is templated in order to support
both const and non-const iteration. It is recommended that you just
use ``auto`` if you need to store a local copy of the returned object.

See begin() and end(), as well as descendants(), for iterating through
the subtree rooted at this packet (not just the immediate children).

Returns:
    an object for iterating through the children of this packet.)doc";

// Docstring regina::python::doc::Packet_::cloneAsSibling
static const char *cloneAsSibling =
R"doc(Clones this packet (and possibly its descendants), assigns to it a
suitable unused label and inserts the clone into the tree as a sibling
of this packet.

Note that any string tags associated with this packet will *not* be
cloned.

If this packet has no parent in the tree structure, no clone will be
created and ``null`` will be returned.

In Regina 6.0.1 and earlier, this function was called clone(). It was
renamed in Regina 7.0 to emphasise that this is not just a deep copy,
and is not guaranteed to succeed.

.. note::
    Since Regina 7.0, if a normal surface/hypersurface or angle
    structure list is cloned, then the new clone will refer back to
    the *original* triangulation, even if we are cloning an entire
    packet tree. This is because there is no guarantee that the
    original triangulation was cloned also (it could live outside the
    cloned subtree, or might not be part of a packet tree at all).

Parameter ``cloneDescendants``:
    ``True`` if the descendants of this packet should also be cloned
    and inserted as descendants of the new packet. If this is passed
    as ``False`` (the default), only this packet will be cloned.

Parameter ``end``:
    ``True`` if the new packet should be inserted at the end of the
    parent's list of children (the default), or ``False`` if the new
    packet should be inserted as the sibling immediately after this
    packet.

Returns:
    the newly inserted packet, or ``null`` if this packet has no
    parent.)doc";

// Docstring regina::python::doc::Packet_::countChildren
static const char *countChildren =
R"doc(Returns the number of immediate children of this packet. Grandchildren
and so on are not counted.

Returns:
    the number of immediate children.)doc";

// Docstring regina::python::doc::Packet_::countDescendants
static const char *countDescendants =
R"doc(Returns the total number of strict descendants of this packet. This
includes children, grandchildren and so on. This packet is not
included in the count.

Returns:
    the total number of strict descendants.)doc";

// Docstring regina::python::doc::Packet_::descendants
static const char *descendants =
R"doc(Returns a lightweight object for iterating through all strict
descendants of this packet in the packet tree.

The order of iteration is exactly the same as when iterating over the
full subtree rooted at this packet (as offered by Packet::begin() and
Packet::end()), except that the iteration *excludes* this packet
itself. In particular, the iteration is depth-first, and each packet
in the subtree is processed before its own descendants.

This routine allows you to iterate through all strict descendants of a
given packet using C++11 range-based ``for`` loops:

```
std::shared_ptr<Packet> parent = ...;
for (Packet& desc : parent->descendants()) { ... }
```

In Python, this routine returns an iterable object:

```
parent = ...
for desc in parent.descendants():
    ...
```

This function returns a lightweight object in the sense that it does
not generate a full list of descendants in advance, but instead just
returns a small iterator that visits each descendant as required. In
particular, this routine has small constant time and memory.

Since Regina 7.0, the return type is templated in order to support
both const and non-const iteration. It is recommended that you just
use ``auto`` if you need to store a local copy of the returned object.

See also begin() and end() for iterating through the entire subtree
*including* this packet, and children() for iterating over just this
packet's immediate children.

Returns:
    an object for iterating through the strict descendants of this
    packet.)doc";

// Docstring regina::python::doc::Packet_::descendants_2
static const char *descendants_2 =
R"doc(Returns a lightweight object for iterating through all strict
descendants of this packet in the packet tree.

The order of iteration is exactly the same as when iterating over the
full subtree rooted at this packet (as offered by Packet::begin() and
Packet::end()), except that the iteration *excludes* this packet
itself. In particular, the iteration is depth-first, and each packet
in the subtree is processed before its own descendants.

This routine allows you to iterate through all strict descendants of a
given packet using C++11 range-based ``for`` loops:

```
std::shared_ptr<const Packet> parent = ...;
for (const Packet& desc : parent->descendants()) { ... }
```

This function returns a lightweight object in the sense that it does
not generate a full list of descendants in advance, but instead just
returns a small iterator that visits each descendant as required. In
particular, this routine has small constant time and memory.

Since Regina 7.0, the return type is templated in order to support
both const and non-const iteration. It is recommended that you just
use ``auto`` if you need to store a local copy of the returned object.

See also begin() and end() for iterating through the entire subtree
*including* this packet, and children() for iterating over just this
packet's immediate children.

Returns:
    an object for iterating through the strict descendants of this
    packet.)doc";

// Docstring regina::python::doc::Packet_::findPacketLabel
static const char *findPacketLabel =
R"doc(Finds the packet with the requested label in the tree or subtree for
which this packet is matriarch. Note that label comparisons are case
sensitive.

Parameter ``label``:
    the label to search for.

Returns:
    the packet with the requested label, or ``null`` if there is no
    such packet.)doc";

// Docstring regina::python::doc::Packet_::findPacketLabel_2
static const char *findPacketLabel_2 =
R"doc(Finds the packet with the requested label in the tree or subtree for
which this packet is matriarch. Note that label comparisons are case
sensitive.

Parameter ``label``:
    the label to search for.

Returns:
    the packet with the requested label, or ``null`` if there is no
    such packet.)doc";

// Docstring regina::python::doc::Packet_::firstChild
static const char *firstChild =
R"doc(Determines the first child of this packet in the tree structure.

This routine takes small constant time.

Returns:
    the first child packet, or ``null`` if there is none.)doc";

// Docstring regina::python::doc::Packet_::firstTreePacket
static const char *firstTreePacket =
R"doc(Finds the first packet of the requested type in a complete depth-first
iteration of the tree structure. Note that this packet **must** be the
matriarch of the entire tree.

A parent packet is always reached before its children. The tree
matriarch will be the first packet visited in a complete depth-first
iteration.

Parameter ``type``:
    the type of packet to search for.

Returns:
    the first such packet, or ``null`` if there are no packets of the
    requested type.)doc";

// Docstring regina::python::doc::Packet_::firstTreePacket_2
static const char *firstTreePacket_2 =
R"doc(Finds the first packet of the requested type in a complete depth-first
iteration of the tree structure. Note that this packet **must** be the
matriarch of the entire tree.

A parent packet is always reached before its children. The tree
matriarch will be the first packet visited in a complete depth-first
iteration.

Parameter ``type``:
    the type of packet to search for.

Returns:
    the first such packet, or ``null`` if there are no packets of the
    requested type.)doc";

// Docstring regina::python::doc::Packet_::fullName
static const char *fullName =
R"doc(Returns a descriptive text string for the packet. The string is of the
form *label (packet-type)*.

The packet label will be adjusted for human-readable output according
to the behaviour of humanLabel().

Returns:
    the descriptive text string.)doc";

// Docstring regina::python::doc::Packet_::hasParent
static const char *hasParent =
R"doc(Determines if this packet has a parent in the tree structure.

This is equivalent to, but slightly faster than, testing whether
parent() returns a null pointer.

Returns:
    ``if`` and only if this packet has a parent.)doc";

// Docstring regina::python::doc::Packet_::hasTag
static const char *hasTag =
R"doc(Determines whether this packet has the given associated tag.

Each packet can have an arbitrary set of string tags associated with
it. The tags are not used by this calculation engine; the feature is
provided for whatever use a developer or user chooses to make of it.

Tags are case-sensitive. Tags associated with a single packet must be
distinct, i.e., a particular tag cannot be associated more than once
with the same packet.

Parameter ``tag``:
    the tag to search for.

Returns:
    ``True`` if the given tag is found, ``False`` otherwise.)doc";

// Docstring regina::python::doc::Packet_::hasTags
static const char *hasTags =
R"doc(Determines whether this packet has any associated tags at all.

Each packet can have an arbitrary set of string tags associated with
it. The tags are not used by this calculation engine; the feature is
provided for whatever use a developer or user chooses to make of it.

Tags are case-sensitive. Tags associated with a single packet must be
distinct, i.e., a particular tag cannot be associated more than once
with the same packet.

Returns:
    ``True`` if this packet has any tags, ``False`` otherwise.)doc";

// Docstring regina::python::doc::Packet_::humanLabel
static const char *humanLabel =
R"doc(Returns the label associated with this individual packet, adjusted if
necessary for human-readable output.

In particular, if the packet has no label assigned then this routine
will return "(no label)", not the empty string.

.. warning::
    The method by which this routine adjusts packet labels is subject
    to change in future versions of Regina.

Returns:
    this individual packet's label.)doc";

// Docstring regina::python::doc::Packet_::insert
static const char *insert =
R"doc(Inserts the given packet as a child of this packet at the given
location in this packet's child list.

This packet will take ownership of *child*, in the sense that every
parent packet stores (either directly or indirectly) a shared_ptr to
every one of its descendants in the packet tree.

In Regina 7.0 and earlier, this routine was called insertChildAfter().

This routine takes small constant time.

Precondition:
    Parameter *newChild* has no parent packet.

Precondition:
    This packet is already the parent of *prevChild*.

Precondition:
    This packet is not a descendant of *newChild*.

Exception ``InvalidArgument``:
    The argument *newChild* already has a parent packet, and/or the
    argument *prevChild* is non-null and does not have this packet as
    its parent. Note that, although this tests *some* of our
    preconditions, there are other preconditions that are *not*
    tested, and for which no exceptions are thrown.

Parameter ``newChild``:
    the child to insert.

Parameter ``prevChild``:
    the preexisting child of this packet after which *newChild* will
    be inserted, or ``null`` if *newChild* is to be the first child of
    this packet.)doc";

// Docstring regina::python::doc::Packet_::insertChildAfter
static const char *insertChildAfter =
R"doc(Deprecated routine that inserts the given packet as a child of this
packet at the given location in this packet's child list.

.. deprecated::
    This routine has been renamed to insert(). See insert() for
    further details.

Precondition:
    Parameter *newChild* has no parent packet.

Precondition:
    This packet is already the parent of *prevChild*.

Precondition:
    This packet is not a descendant of *newChild*.

Exception ``InvalidArgument``:
    The argument *newChild* already has a parent packet, and/or the
    argument *prevChild* is non-null and does not have this packet as
    its parent. Note that, although this tests *some* of our
    preconditions, there are other preconditions that are *not*
    tested, and for which no exceptions are thrown.

Parameter ``newChild``:
    the child to insert.

Parameter ``prevChild``:
    the preexisting child of this packet after which *newChild* will
    be inserted, or ``null`` if *newChild* is to be the first child of
    this packet.)doc";

// Docstring regina::python::doc::Packet_::insertChildFirst
static const char *insertChildFirst =
R"doc(Deprecated routine that inserts the given packet as the first child of
this packet.

.. deprecated::
    This routine has been renamed to prepend(). See prepend() for
    further details.

Precondition:
    The given child has no parent packet.

Precondition:
    This packet is not a descendant of the given child.

Exception ``InvalidArgument``:
    The argument *child* already has a parent packet. Note that,
    although this tests *one* of our preconditions, there are other
    preconditions that are *not* tested, and for which no exceptions
    are thrown.

Parameter ``child``:
    the child to insert.)doc";

// Docstring regina::python::doc::Packet_::insertChildLast
static const char *insertChildLast =
R"doc(Deprecated routine that inserts the given packet as the last child of
this packet.

.. deprecated::
    This routine has been renamed to append(). See append() for
    further details.

Precondition:
    The given child has no parent packet.

Precondition:
    This packet is not a descendant of the given child.

Exception ``InvalidArgument``:
    The argument *child* already has a parent packet. Note that,
    although this tests *one* of our preconditions, there are other
    preconditions that are *not* tested, and for which no exceptions
    are thrown.

Parameter ``child``:
    the child to insert.)doc";

// Docstring regina::python::doc::Packet_::internalID
static const char *internalID =
R"doc(Returns a unique string ID that identifies this packet.

The user has no control over this ID, and it is not human readable. It
is guaranteed to remain fixed throughout the lifetime of the program
for a given packet, and it is guaranteed not to clash with the ID of
any other packet.

If you change the contents of a packet, its ID will not change.

If you clone a packet, the new clone will receive a different ID. If
you save and then load a packet to/from file, the ID will change.
These behaviours are necessary to ensure that IDs remain unique
(since, for instance, you could load several copies of the same data
file into memory simultaneously).

The ID is implemented as an encoding of the underlying C++ pointer.
This encoding is subject to change in later versions of Regina.

Returns:
    a unique ID that identifies this packet.)doc";

// Docstring regina::python::doc::Packet_::isAncestorOf
static const char *isAncestorOf =
R"doc(Determines if this packet is equal to or an ancestor of the given
packet in the tree structure.

Parameter ``descendant``:
    the other packet whose relationships we are examining.

Returns:
    ``True`` if and only if this packet is equal to or an ancestor of
    ``descendant``.)doc";

// Docstring regina::python::doc::Packet_::isListening
static const char *isListening =
R"doc(Determines whether the given packet listener is currently listening
for events on this packet. See the PacketListener class notes for
details.

Parameter ``listener``:
    the listener to search for.

Returns:
    ``True`` if the given listener is currently registered with this
    packet, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Packet_::label
static const char *label =
R"doc(Returns the label associated with this individual packet. An example
is ``MyTriangulation``.

Returns:
    this individual packet's label.)doc";

// Docstring regina::python::doc::Packet_::lastChild
static const char *lastChild =
R"doc(Determines the last child of this packet in the tree structure.

This routine takes small constant time.

Returns:
    the last child packet, or ``null`` if there is none.)doc";

// Docstring regina::python::doc::Packet_::levelsDownTo
static const char *levelsDownTo =
R"doc(Counts the number of levels between this packet and its given
descendant in the tree structure. If ``descendant`` is this packet,
the number of levels is zero.

Precondition:
    This packet is equal to ``descendant``, or can be obtained from
    ``descendant`` using only child-to-parent steps.

Exception ``FailedPrecondition``:
    The argument *descendant* is not equal to or a descendant of this
    packet.

Parameter ``descendant``:
    the packet whose relationship with this packet we are examining.

Returns:
    the number of levels difference.)doc";

// Docstring regina::python::doc::Packet_::levelsUpTo
static const char *levelsUpTo =
R"doc(Counts the number of levels between this packet and its given ancestor
in the tree structure. If ``ancestor`` is this packet, the number of
levels is zero.

Precondition:
    This packet is equal to ``ancestor``, or can be obtained from
    ``ancestor`` using only parent-to-child steps.

Exception ``FailedPrecondition``:
    This packet is not equal to or a descendant of the argument
    *descendant*.

Parameter ``ancestor``:
    the packet whose relationship with this packet we are examining.

Returns:
    the number of levels difference.)doc";

// Docstring regina::python::doc::Packet_::listen
static const char *listen =
R"doc(Registers the given packet listener to listen for events on this
packet. See the PacketListener class notes for details.

Parameter ``listener``:
    the listener to register.

Returns:
    ``True`` if the given listener was successfully registered, or
    ``False`` if the given listener was already registered beforehand.)doc";

// Docstring regina::python::doc::Packet_::makeOrphan
static const char *makeOrphan =
R"doc(Cuts this packet away from its parent in the tree structure and
instead makes it the root of its own tree. The tree information for
both this packet and its parent will be updated.

The old parent will relinquish ownership of this packet. This means
that, even if the old parent remains alive, once the last shared_ptr
to this packet is destroyed then this packet itself will be destroyed
also.

This makeOrphan() function is the preferred way to "delete" a packet
*p* from a larger packet tree: simply orphan *p* and then dispose of
any outstanding shared pointers to it (if you have any).

Even if you are not holding a shared_ptr to this packet yourself, this
routine is still safe to use: it will keep an internal shared_ptr as a
"guard" until makeOrphan() has completed its work, at which point the
packet will be safely destroyed.

This routine takes small constant time. It is safe to use regardless
of whether this packet currently has a parent or not.)doc";

// Docstring regina::python::doc::Packet_::moveDown
static const char *moveDown =
R"doc(Moves this packet the given number of steps towards the end of its
sibling list. If the number of steps is larger than the greatest
possible movement, the packet will be moved to the very end of its
sibling list.

This routine takes time proportional to the number of steps.

Precondition:
    The given number of steps is strictly positive.

Parameter ``steps``:
    the number of steps down to move.)doc";

// Docstring regina::python::doc::Packet_::moveToFirst
static const char *moveToFirst =
R"doc(Moves this packet to be the first in its sibling list.

This routine takes small constant time.)doc";

// Docstring regina::python::doc::Packet_::moveToLast
static const char *moveToLast =
R"doc(Moves this packet to be the last in its sibling list.

This routine takes small constant time.)doc";

// Docstring regina::python::doc::Packet_::moveUp
static const char *moveUp =
R"doc(Moves this packet the given number of steps towards the beginning of
its sibling list. If the number of steps is larger than the greatest
possible movement, the packet will be moved to the very beginning of
its sibling list.

This routine takes time proportional to the number of steps.

Precondition:
    The given number of steps is strictly positive.

Parameter ``steps``:
    the number of steps up to move.)doc";

// Docstring regina::python::doc::Packet_::nextSibling
static const char *nextSibling =
R"doc(Determines the next sibling of this packet in the tree structure. This
is the child of the parent that follows this packet.

This routine takes small constant time.

Returns:
    the next sibling of this packet, or ``null`` if there is none.)doc";

// Docstring regina::python::doc::Packet_::nextTreePacket
static const char *nextTreePacket =
R"doc(Finds the next packet after this in a complete depth-first iteration
of the entire tree structure to which this packet belongs. Note that
this packet need not be the tree matriarch.

A parent packet is always reached before its children. The tree
matriarch will be the first packet visited in a complete depth-first
iteration.

Returns:
    the next packet, or ``null`` if this is the last packet in such an
    iteration.)doc";

// Docstring regina::python::doc::Packet_::nextTreePacket_2
static const char *nextTreePacket_2 =
R"doc(Finds the next packet after this in a complete depth-first iteration
of the entire tree structure to which this packet belongs. Note that
this packet need not be the tree matriarch.

A parent packet is always reached before its children. The tree
matriarch will be the first packet visited in a complete depth-first
iteration.

Returns:
    the next packet, or ``null`` if this is the last packet in such an
    iteration.)doc";

// Docstring regina::python::doc::Packet_::nextTreePacket_3
static const char *nextTreePacket_3 =
R"doc(Finds the next packet after this of the requested type in a complete
depth-first iteration of the entire tree structure. Note that this
packet need not be the tree matriarch. The order of tree searching is
described in firstTreePacket().

Parameter ``type``:
    the type of packet to search for.

Returns:
    the next such packet, or ``null`` if this is the last packet of
    the requested type in such an iteration.)doc";

// Docstring regina::python::doc::Packet_::nextTreePacket_4
static const char *nextTreePacket_4 =
R"doc(Finds the next packet after this of the requested type in a complete
depth-first iteration of the entire tree structure. Note that this
packet need not be the tree matriarch. The order of tree searching is
described in firstTreePacket().

Parameter ``type``:
    the type of packet to search for.

Returns:
    the next such packet, or ``null`` if this is the last packet of
    the requested type in such an iteration.)doc";

// Docstring regina::python::doc::Packet_::parent
static const char *parent =
R"doc(Determines the parent packet in the tree structure.

This routine takes small constant time.

Returns:
    the parent packet, or ``null`` if there is none.)doc";

// Docstring regina::python::doc::Packet_::prepend
static const char *prepend =
R"doc(Inserts the given packet as the first child of this packet.

This packet will take ownership of *child*, in the sense that every
parent packet stores (either directly or indirectly) a shared_ptr to
every one of its descendants in the packet tree.

In Regina 7.0 and earlier, this routine was called insertChildFirst().

This routine takes small constant time.

Precondition:
    The given child has no parent packet.

Precondition:
    This packet is not a descendant of the given child.

Exception ``InvalidArgument``:
    The argument *child* already has a parent packet. Note that,
    although this tests *one* of our preconditions, there are other
    preconditions that are *not* tested, and for which no exceptions
    are thrown.

Parameter ``child``:
    the child to insert.)doc";

// Docstring regina::python::doc::Packet_::prevSibling
static const char *prevSibling =
R"doc(Determines the previous sibling of this packet in the tree structure.
This is the child of the parent that precedes this packet.

This routine takes small constant time.

Returns:
    the previous sibling of this packet, or ``null`` if there is none.)doc";

// Docstring regina::python::doc::Packet_::removeAllTags
static const char *removeAllTags =
R"doc(Removes all associated tags from this packet.

Each packet can have an arbitrary set of string tags associated with
it. The tags are not used by this calculation engine; the feature is
provided for whatever use a developer or user chooses to make of it.

Tags are case-sensitive. Tags associated with a single packet must be
distinct, i.e., a particular tag cannot be associated more than once
with the same packet.)doc";

// Docstring regina::python::doc::Packet_::removeTag
static const char *removeTag =
R"doc(Removes the association of the given tag with this packet.

Each packet can have an arbitrary set of string tags associated with
it. The tags are not used by this calculation engine; the feature is
provided for whatever use a developer or user chooses to make of it.

Tags are case-sensitive. Tags associated with a single packet must be
distinct, i.e., a particular tag cannot be associated more than once
with the same packet.

Parameter ``tag``:
    the tag to remove.

Returns:
    ``True`` if the given tag was removed, or ``False`` if the given
    tag was not actually associated with this packet.)doc";

// Docstring regina::python::doc::Packet_::reparent
static const char *reparent =
R"doc(Cuts this packet away from its parent in the tree structure, and
inserts it as a child of the given packet instead.

This routine is essentially a combination of makeOrphan() followed by
either prepend() or append().

Even if you are not holding a shared_ptr to this packet yourself, this
routine is still safe to use: it will maintain a shared_ptr as a
"guard" so that this packet is not inadvertently destroyed during the
transfer.

You may pass *newParent* as ``null``, in which case this routine
behaves in the same way as makeOrphan() (and is similarly safe to use
even if there are no other shared pointers to this packet).

This routine takes small constant time. It is safe to use regardless
of whether this packet currently has a parent or not.

If you wish to reparent *all* of the children of a given packet, see
transferChildren() instead.

Precondition:
    The given parent is not a descendant of this packet.

Parameter ``newParent``:
    the new parent of this packet, i.e., the packet beneath which this
    packet will be inserted.

Parameter ``first``:
    ``True`` if this packet should be inserted as the first child of
    the given parent, or ``False`` (the default) if it should be
    inserted as the last child.)doc";

// Docstring regina::python::doc::Packet_::root
static const char *root =
R"doc(Determines the root of the tree to which this packet belongs.

Returns:
    the matriarch of the packet tree.)doc";

// Docstring regina::python::doc::Packet_::samePacket
static const char *samePacket =
R"doc(Determines whether this and the given object refer to the same packet.

This is exactly the same as testing whether the underlying Packet
pointers are equal, and so this routine is unnecessary for C++ users.

Instead, this routine is designed for Python users, since:

* the Python keyword ``is`` will not work, because there could be many
  different Python wrappers all pointing to the same C++ object;

* the Python equality test (==) will not work, since as of Regina 7.0
  this compares objects by value (i.e., it tests whether their
  contents are equal).

A use case for this function could be (for example) iterating through
a packet tree and identifying when a particular known packet has been
found.

Parameter ``other``:
    the packet to compare with this.

Returns:
    ``True`` if and only if this and the given object refer to the
    same underlying packet.)doc";

// Docstring regina::python::doc::Packet_::save
static const char *save =
R"doc(Saves the subtree rooted at this packet to the given Regina data file,
using Regina's native XML file format. The XML file may be optionally
compressed (Regina can happily read both compressed and uncompressed
XML).

This is the preferred way of saving a Regina data file. Typically this
will be called from the root of the packet tree, which will save the
entire packet tree to file.

Precondition:
    The given packet does not depend on its parent.

Internationalisation:
    This routine makes no assumptions about the i18n "character
    encoding" used in the given file *name*, and simply passes it
    through unchanged to low-level C/C++ file I/O routines. The
    *contents* of the file will be written using UTF-8.

Parameter ``filename``:
    the pathname of the file to write to.

Parameter ``compressed``:
    ``True`` if the XML data should be compressed, or ``False`` if it
    should be written as plain text.

Parameter ``format``:
    indicates which of Regina's XML file formats to write. You should
    use the default (REGINA_CURRENT_FILE_FORMAT) unless you need your
    file to be readable by older versions of Regina. This must not be
    REGINA_BINARY_GEN_1, which is no longer supported.

Returns:
    ``True`` if and only if the file was successfully written.)doc";

// Docstring regina::python::doc::Packet_::setLabel
static const char *setLabel =
R"doc(Sets the label associated with this individual packet.

Parameter ``label``:
    the new label to give this packet.)doc";

// Docstring regina::python::doc::Packet_::sortChildren
static const char *sortChildren =
R"doc(Sorts the immediate children of this packet according to their packet
labels. Note that this routine is not recursive (for instance,
grandchildren will not be sorted within each child packet).

This routine takes quadratic time in the number of immediate children
(and it's slow quadratic at that).)doc";

// Docstring regina::python::doc::Packet_::swapWithNextSibling
static const char *swapWithNextSibling =
R"doc(Swaps this packet with its next sibling in the sequence of children
beneath their common parent packet. Calling this routine is equivalent
to calling moveDown().

This routine takes small constant time.

If this packet has no next sibling then this routine safely does
nothing.)doc";

// Docstring regina::python::doc::Packet_::tags
static const char *tags =
R"doc(Returns the set of all tags associated with this packet.

Each packet can have an arbitrary set of string tags associated with
it. The tags are not used by this calculation engine; the feature is
provided for whatever use a developer or user chooses to make of it.

Tags are case-sensitive. Tags associated with a single packet must be
distinct, i.e., a particular tag cannot be associated more than once
with the same packet.

Python:
    This routine returns a python set.

Returns:
    the set of all tags associated with this packet.)doc";

// Docstring regina::python::doc::Packet_::totalTreeSize
static const char *totalTreeSize =
R"doc(Determines the total number of packets in the tree or subtree for
which this packet is matriarch. This packet is included in the count.

Returns:
    the total tree or subtree size.)doc";

// Docstring regina::python::doc::Packet_::transferChildren
static const char *transferChildren =
R"doc(Cuts all of this packet's children out of the packet tree, and
reinserts them as children of the given packet instead.

The children of this packet will be appended to the end of the new
parent's child list, in the same order as they were previously.

This is equivalent to calling reparent() on each child, but should be
somewhat faster if there are many children to move.

Even if you are not holding a shared_ptr to any of this packet's
children, this routine is still safe to use: it will ensure there is
always some shared_ptr to guard against any children being
inadvertently destroyed during the transfer.

You may pass *newParent* as ``null``, in which case this routine is
equivalent to calling makeOrphan() on each child (and is similarly
safe to use even if there are no other shared pointers to this
packet).

Precondition:
    The given parent is not a descendant of this packet.

Parameter ``newParent``:
    the new parent beneath which the children will be inserted.)doc";

// Docstring regina::python::doc::Packet_::type
static const char *type =
R"doc(Returns the unique integer ID representing this type of packet. This
is the same for all packets of this class.

Returns:
    the packet type ID.)doc";

// Docstring regina::python::doc::Packet_::typeName
static const char *typeName =
R"doc(Returns an English name for this type of packet. An example is
``Triangulation3``. This is the same for all packets of this class.

Returns:
    the packet type name.)doc";

// Docstring regina::python::doc::Packet_::unlisten
static const char *unlisten =
R"doc(Unregisters the given packet listener so that it no longer listens for
events on this packet. See the PacketListener class notes for details.

Parameter ``listener``:
    the listener to unregister.

Returns:
    ``True`` if the given listener was successfully unregistered, or
    ``False`` if the given listener was not registered in the first
    place.)doc";

// Docstring regina::python::doc::Packet_::writeXMLFile
static const char *writeXMLFile =
R"doc(Writes the subtree rooted at this packet to the given output stream in
Regina's native XML file format. Ths is similar to calling save(),
except that (i) the user has a more flexible choice of output stream,
and (ii) the XML will always be written in plain text (i.e., it will
not be compressed).

If you simply wish to save your data to a file on the filesystem, you
should call save() instead.

Typically this will be called from the root of the packet tree, which
will write the entire packet tree to the output stream.

Python:
    The argument *out* should be an open Python file object.

Parameter ``out``:
    the output stream to which the XML data file should be written.

Parameter ``format``:
    indicates which of Regina's XML file formats to write. You should
    use the default (REGINA_CURRENT_FILE_FORMAT) unless you need your
    file to be readable by older versions of Regina. This must not be
    REGINA_BINARY_GEN_1, which is no longer supported.)doc";

}

namespace SubtreeIterator_ {

// Docstring regina::python::doc::SubtreeIterator_::__eq
static const char *__eq =
R"doc(Tests whether this and the given iterator are equal.

This routine only compares the packets that each iterator is currently
pointing to. It does not compare the roots of the subtrees themselves.

Returns:
    true if and only if the two iterators are equal.)doc";

// Docstring regina::python::doc::SubtreeIterator_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over all members of the relevant packet
subtree.

Returns:
    an iterator over all members of the relevant packet subtree.)doc";

// Docstring regina::python::doc::SubtreeIterator_::__ne
static const char *__ne =
R"doc(Tests whether this and the given iterator are different.

This routine only compares the packets that each iterator is currently
pointing to. It does not compare the roots of the subtrees themselves.

Returns:
    true if and only if the two iterators are different.)doc";

// Docstring regina::python::doc::SubtreeIterator_::__next__
static const char *__next__ =
R"doc(Returns the current packet in the subtree and increments this
iterator.

Exception ``StopIteration``:
    The iterator is already past-the-end when this function is called.

Returns:
    the packet that this iterator is pointing to, before the increment
    takes place.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

