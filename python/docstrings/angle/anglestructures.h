/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::AngleStructures
static const char *AngleStructures =
R"doc(A collection of angle structures on a 3-manifold triangulation.

There are some important changes to this class as of Regina 7.0:

* An angle structure list does _not_ need to be a child packet of the
  underlying triangulation, and indeed does not need to interact with
  the packet tree at all.

* You are welcome to modify or even destroy the original
  triangulation; if you do then this list will automatically make a
  private copy of the original triangulation as an ongoing reference.
  Different angle structure lists (and normal surface lists) can all
  share the same private copy, so this is not an expensive process.

* You should now create angle structure lists using the class
  constructor (but which, unlike the old enumerate(), does not insert
  the list into the packet tree). There is no need to use enumerate()
  any more.

Since Regina 7.0, this is no longer a "packet type" that can be
inserted directly into the packet tree. Instead an angle structure
list is now a standalone mathematatical object, which makes it slimmer
and faster for ad-hoc use. The consequences of this are:

* If you create your own AngleStructures object, it will not have any
  of the usual packet infrastructure. You cannot add it into the
  packet tree, and it will not support a label, tags, child/parent
  packets, and/or event listeners.

* To include an AngleStructures object in the packet tree, you must
  create a new PacketOf<AngleStructures>. This _is_ a packet type, and
  supports labels, tags, child/parent packets, and event listeners. It
  derives from AngleStructures, and so inherits the full
  AngleStructures interface.

* If you are adding new functions to this class that edit the list,
  you must still remember to create a PacketChangeSpan. This will
  ensure that, if the list is being managed by a
  PacketOf<AngleStructures>, then the appropriate packet change events
  will be fired. All other events (aside from packetToBeChanged() and
  packetWasChanged() are managed directly by the
  PacketOf<AngleStructures> wrapper class.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::makeAngleEquations
static const char *makeAngleEquations =
R"doc(Generates the set of angle structure equations for the given
triangulation.

These are the angle equations that will be used when enumerating angle
structures on the given triangulation.

Each equation will be represented as a row of the resulting matrix,
and each column will represent a coordinate in the underlying
coordinate system (which is described in the notes for
AngleStructure::vector()).

Parameter ``tri``:
    the triangulation upon which these angle structure equations will
    be based.

Returns:
    the resulting set of angle structure equations.)doc";

namespace AngleStructures_ {

// Docstring regina::python::doc::AngleStructures_::__array
static const char *__array =
R"doc(Returns the angle structure at the requested index in this list. This
is identical to calling structure().

Parameter ``index``:
    the index of the requested angle structure in this list; this must
    be between 0 and size()-1 inclusive.

Returns:
    the angle structure at the requested index.)doc";

// Docstring regina::python::doc::AngleStructures_::__copy
static const char *__copy = R"doc(Constructs a new copy of the given list.)doc";

// Docstring regina::python::doc::AngleStructures_::__eq
static const char *__eq =
R"doc(Determines whether this and the given list contain the same set of
angle structures.

The lists will be compared as multisets: the order of the angle
structures in each list does not matter; however, in the unusual
scenario where a list the same angle structure multiple times,
multiplicity does matter.

Like the comparison operators for AngleStructure, it does not matter
whether the two lists work with different triangulations:

* If the two triangulations have the same size, then this routine will
  compare angle structures as though they were transplanted into the
  same triangulation using the same tetrahedron numbering and the same
  angle coordinates.

* If the two triangulations have different sizes, then this comparison
  will return ``False``.

Parameter ``other``:
    the list to be compared with this list.

Returns:
    ``True`` if both lists represent the same multiset of angle
    structures, or ``False`` if not.)doc";

// Docstring regina::python::doc::AngleStructures_::__init
static const char *__init =
R"doc(A unified constructor for enumerating various classes of angle
structures on a given triangulation.

If *tautOnly* is ``False`` (the default), then this new list will be
filled with all vertices of the angle structure solution space. If
``tautOnly`` is ``True``, then the list will be filled with only the
taut angle structures (a subset of the vertex angle structures); these
are usually much faster to enumerate.

The *algHints* argument is a combination of flags that allows you to
control the underlying enumeration algorithm. These flags are treated
as hints only: if your selection of algorithm is invalid, unavailable
or unsupported then Regina will choose something more appropriate.
Unless you have some specialised need, the default AngleAlg::Default
(which makes no hints at all) will allow Regina to choose what it
thinks will be the most efficient method.

Unlike the old enumerate() function, the new angle structure list will
_not_ be inserted into the packet tree. Moreover, the given
triangulation may change or even be destroyed without causing
problems. See the class notes for details.

If a progress tracker is passed, this routine will declare and work
through a series of stages whose combined weights sum to 1; typically
this means that the given tracker must not have been used before.

This constructor will not return until the enumeration of angle
structures is complete, regardless of whether a progress tracker was
passed. If you need the behaviour of the old enumerate() (where
passing a progress tracker caused the enumeration to start in the
background), simply call this constructor in a new detached thread.
Note that this enumeration can be extremely slow for larger
triangulations, and so there could be good reasons to do this.

Python:
    The global interpreter lock will be released while this
    constructor runs, so you can use it with Python-based
    multithreading.

Parameter ``triangulation``:
    the triangulation for which the vertex angle structures will be
    enumerated.

Parameter ``tautOnly``:
    ``True`` if only taut structures are to be enuemrated, or
    ``False`` if we should enumerate all vertices of the angle
    structure solution space.

Parameter ``algHints``:
    passes requests to Regina for which specific enumeration algorithm
    should be used.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.)doc";

// Docstring regina::python::doc::AngleStructures_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over the angle structures in this list.

In Python, an angle structure list can be treated as an iterable
object:

```
list = AngleStructures(...)
for s in list:
    ...
```

Returns:
    an iterator over the angle structures in this list.)doc";

// Docstring regina::python::doc::AngleStructures_::algorithm
static const char *algorithm =
R"doc(Returns details of the algorithm that was used to enumerate this list.

These may not be the same algorithm flags that were passed to the
class constructor. In particular, default values will have been
explicitly filled in, invalid and/or redundant values will have been
removed, and unavailable and/or unsupported combinations of algorithm
flags will be replaced with whatever algorithm was actually used.

Returns:
    details of the algorithm used to enumerate this list.)doc";

// Docstring regina::python::doc::AngleStructures_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given lists.

This global routine simply calls AngleStructures::swap(); it is
provided so that AngleStructures meets the C++ Swappable requirements.

See AngleStructures::swap() for more details.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both lists which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``lhs``:
    the list whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the list whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::AngleStructures_::isTautOnly
static const char *isTautOnly =
R"doc(Returns whether this list was produced by enumerating taut angle
structures only.

Returns:
    ``True`` if this list was produced by enumerating taut angle
    structures only, or ``False`` if the enumeration procedure allowed
    for any angle structures.)doc";

// Docstring regina::python::doc::AngleStructures_::size
static const char *size =
R"doc(Returns the number of angle structures stored in this list.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the number of angle structures.)doc";

// Docstring regina::python::doc::AngleStructures_::sort
static const char *sort =
R"doc(Sorts the angle structures in this list according to the given
criterion.

This sort is stable, i.e., angle structures that are equivalent under
the given criterion will remain in the same relative order.

The implementation of this routine uses std::stable_sort.

Python:
    This is available in Python, and *comp* may be a pure Python
    function.

Parameter ``comp``:
    a binary function (or other callable type) that accepts two const
    AngleStructure references, and returns ``True`` if and only if the
    first angle structure should appear before the second in the
    sorted list.)doc";

// Docstring regina::python::doc::AngleStructures_::spansStrict
static const char *spansStrict =
R"doc(Determines whether any convex combination of the angle structures in
this list is a strict angle structure. See AngleStructure::isStrict()
for details on strict angle structures.

Returns:
    ``True`` if and only if a strict angle structure can be produced.)doc";

// Docstring regina::python::doc::AngleStructures_::spansTaut
static const char *spansTaut =
R"doc(Determines whether any angle structure in this list is a taut
structure. Because taut structures always appear as vertices of the
angle structure solution space, this routine is equivalent to testing
whether any convex combination of the angle structures in this list is
a taut structure.

See AngleStructure::isTaut() for details on taut structures.

Returns:
    ``True`` if and only if a taut structure can be produced.)doc";

// Docstring regina::python::doc::AngleStructures_::structure
static const char *structure =
R"doc(Returns the angle structure at the requested index in this list. This
is identical to using the square bracket operator.

Parameter ``index``:
    the index of the requested angle structure in this list; this must
    be between 0 and size()-1 inclusive.

Returns:
    the angle structure at the requested index.)doc";

// Docstring regina::python::doc::AngleStructures_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given list.

This routine will behave correctly if *other* is in fact this list.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both lists which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``other``:
    the list whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::AngleStructures_::triangulation
static const char *triangulation =
R"doc(Returns the triangulation on which these angle structures lie.

This will be a snapshot frozen in time of the triangulation that was
originally passed to the AngleStructures constructor.

This will return a correct result even if the original triangulation
has since been modified or destroyed. However, in order to ensure this
behaviour, it is possible that at different points in time this
function may return references to different C++ objects.

The rules for using the triangulation() reference are:

* Do not keep the resulting reference as a long-term reference or
  pointer of your own, since in time you may find yourself referring
  to the wrong object (see above). Just call this function again.

* You must respect the read-only nature of the result (i.e., you must
  not cast the constness away). The snapshotting process detects
  modifications, and modifying the frozen snapshot may result in an
  exception being thrown.

.. warning::
    As of Regina 7.0, you _cannot_ access this triangulation via the
    packet tree as Packet::parent(). This is because angle structure
    lists can now be kept anywhere in the packet tree, or can be kept
    as standalone objects outside the packet tree entirely.

Returns:
    a reference to the underlying triangulation.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

