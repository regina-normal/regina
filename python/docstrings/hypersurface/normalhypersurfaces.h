/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::NormalHypersurfaces
static const char *NormalHypersurfaces =
R"doc(A collection of normal hypersurfaces in a 4-manifold triangulation.

There are some important changes to this class as of Regina 7.0:

* A normal hypersurface list does *not* need to be a child packet of
  the underlying triangulation, and indeed does not need to interact
  with the packet tree at all.

* You are welcome to modify or even destroy the original
  triangulation; if you do then this list will automatically make a
  private copy of the original triangulation as an ongoing reference.
  Different normal hypersurface lists can all share the same private
  copy, so this is not an expensive process.

* You should now create normal hypersurface lists using the class
  constructor (but which, unlike the old enumerate(), does not insert
  the list into the packet tree). There is no need to use enumerate()
  any more.

Since Regina 7.0, this is no longer a "packet type" that can be
inserted directly into the packet tree. Instead a normal hypersurface
list is now a standalone mathematatical object, which makes it slimmer
and faster for ad-hoc use. The consequences of this are:

* If you create your own NormalHypersurfaces object, it will not have
  any of the usual packet infrastructure. You cannot add it into the
  packet tree, and it will not support a label, tags, child/parent
  packets, and/or event listeners.

* To include an NormalHypersurfaces object in the packet tree, you
  must create a new PacketOf<NormalHypersurfaces>. This *is* a packet
  type, and supports labels, tags, child/parent packets, and event
  listeners. It derives from NormalHypersurfaces, and so inherits the
  full NormalHypersurfaces interface.

* If you are adding new functions to this class that edit the list,
  you must still remember to create a ChangeEventSpan. This will
  ensure that, if the list is being managed by a
  PacketOf<NormalHypersurfaces>, then the appropriate packet change
  events will be fired. All other events (aside from
  packetToBeChanged() and packetWasChanged() are managed directly by
  the PacketOf<NormalHypersurfaces> wrapper class.

See the NormalHypersurface class notes for details of what to do when
introducing a new coordinate system.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::makeEmbeddedConstraints
static const char *makeEmbeddedConstraints =
R"doc(Generates the validity constraints representing the condition that
normal hypersurfaces be embedded. The validity constraints will be
expressed relative to the given coordinate system.

These are the constraints that will be used when enumerating embedded
hypersurfaces in the given coordinate system (i.e., when the default
HS_EMBEDDED_ONLY flag is used). They will not be used when the
enumeration allows for immersed and/or singular hypersurfaces.

Parameter ``triangulation``:
    the triangulation upon which these validity constraints will be
    based.

Parameter ``coords``:
    the coordinate system to be used; this must be one of the
    predefined coordinate system constants in NormalHypersurfaces.

Returns:
    the set of validity constraints.)doc";

// Docstring regina::python::doc::makeMatchingEquations
static const char *makeMatchingEquations =
R"doc(Generates the set of normal hypersurface matching equations for the
given triangulation using the given coordinate system.

These are the matching equations that will be used when enumerating
normal hypersurfaces in the coordinate system *coords*.

Each equation will be represented as a row of the resulting matrix.
Each column of the matrix represents a coordinate in the given
coordinate system.

Exception ``InvalidArgument``:
    The matching equations could not be created for the given
    triangulation in the given coordinate system, due to an error that
    should have been preventable with the right checks in advance.
    This can only happen in certain coordinate systems, and for all
    such coordinate systems this is explicitly described in the
    HyperCoords enum documentation.

Exception ``UnsolvedCase``:
    The matching equations could not be created for the given
    triangulation in the given coordinate system, due to an error that
    was "genuinely" unforseeable. Again this can only happen in
    certain coordinate systems, where this is explicitly described in
    the HyperCoords enum documentation.

Parameter ``triangulation``:
    the triangulation upon which these matching equations will be
    based.

Parameter ``coords``:
    the coordinate system to be used.

Returns:
    the resulting set of matching equations.)doc";

namespace NormalHypersurfaces_ {

// Docstring regina::python::doc::NormalHypersurfaces_::__array
static const char *__array =
R"doc(Returns the hypersurface at the requested index in this list. This is
identical to calling hypersurface().

Parameter ``index``:
    the index of the requested hypersurface in this list; this must be
    between 0 and size()-1 inclusive.

Returns:
    the normal hypersurface at the requested index in this list.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::__eq
static const char *__eq =
R"doc(Determines whether this and the given list contain the same set of
normal hypersurfaces.

The lists will be compared as multisets: the order of the
hypersurfaces in each list does not matter; however, in the unusual
scenario where a list the same hypersurface multiple times,
multiplicity does matter.

Like the comparison operators for NormalHypersurface, it does not
matter whether the lists work with different triangulations, or
different encodings, or if one but not the other supports non-compact
hypersurfaces. The individual hypersurfaces will simply be compared by
examining or computing the number of normal pieces of each type.

In particular, this routine is safe to call even if this and the given
list work with different triangulations:

* If the two triangulations have the same size, then this routine will
  compare hypersurfaces as though they were transplanted into the same
  triangulation using the same pentachoron numbering and the same
  normal piece types.

* If the two triangulations have different sizes, then this comparison
  will return ``False``.

Parameter ``other``:
    the list to be compared with this list.

Returns:
    ``True`` if both lists represent the same multiset of normal
    hypersurfaces, or ``False`` if not.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::__init
static const char *__init =
R"doc(A unified constructor for enumerating various classes of normal
hypersurfaces within a given triangulation.

The HyperCoords argument allows you to specify an underlying
coordinate system in which to do the enumeration (e.g., standard
coordinates or prism coordinates). This choice of coordinate system
will affect which surfaces are produced, since vertex/fundamental
surfaces in one system are not necessarily vertex/fundamental in
another.

The HyperList argument is a combination of flags that allows you to
specify exactly which normal hypersurfaces you require. This includes
(i) whether you want all vertex hypersurfaces or all fundamental
hypersurfaces, which defaults to HS_VERTEX if you specify neither or
both; and (ii) whether you want only properly embedded surfaces or you
also wish to include immersed and/or singular hypersurfaces, which
defaults to HS_EMBEDDED_ONLY if you specify neither or both.

The HyperAlg argument is a combination of flags that allows you to
control the underlying enumeration algorithm. These flags are treated
as hints only: if your selection of algorithm is invalid, unavailable
or unsupported then Regina will choose something more appropriate.
Unless you have some specialised need, the default HS_ALG_DEFAULT
(which makes no hints at all) will allow Regina to choose what it
thinks will be the most efficient method.

The enumerated hypersurfaces will be stored in this new normal
hypersurface list, and their representations will be scaled down to
use the smallest possible integer coordinates.

Unlike the old enumerate() function, the new normal hypersurface list
will *not* be inserted into the packet tree. Moreover, the given
triangulation may change or even be destroyed without causing
problems. See the class notes for details.

If a progress tracker is passed, this routine will declare and work
through a series of stages whose combined weights sum to 1; typically
this means that the given tracker must not have been used before.

This constructor will not return until the enumeration of
hypersurfaces is complete, regardless of whether a progress tracker
was passed. If you need the behaviour of the old enumerate() (where
passing a progress tracker caused the enumeration to start in the
background), simply call this constructor in a new detached thread.
Note that this enumeration can be extremely slow for larger
triangulations, and so there could be good reasons to do this.

If an error occurs, then this routine will thrown an exception. In
this case, no normal hypersurface list will be created, and the
progress tracker (if passed) will be marked as finished. See the
exception specifications below for details.

Exception ``InvalidArgument``:
    The matching equations could not be created for the given
    triangulation in the given coordinate system, due to an error that
    should have been preventable with the right checks in advance.
    This can only happen in certain coordinate systems, and for all
    such coordinate systems this is explicitly described in the
    HyperCoords enum documentation.

Exception ``UnsolvedCase``:
    The list of hypersurfaces could not be be created for the given
    triangulation in the given coordinate system, due to an error that
    was "genuinely" unforseeable. Currently there are two scenarios in
    which this could happen: (i) the matching equations could not be
    constructed, which can only happen in certain coordinate systems
    where this is explicitly described in the HyperCoords enum
    documentation; or (ii) the arguments require enumerating
    *fundamental* normal surfaces using the primal Hilbert basis
    algorithm, and Normaliz was unable to complete its portion of the
    task, which in theory should never happen at all.

Python:
    The global interpreter lock will be released while this
    constructor runs, so you can use it with Python-based
    multithreading.

Parameter ``triangulation``:
    the triangulation upon which this list of normal hypersurfaces
    will be based.

Parameter ``coords``:
    the coordinate system to be used. This must be one of the system
    that Regina is able to use for enumeration; this is documented
    alongside each HyperCoords enum value.

Parameter ``which``:
    indicates which normal hypersurfaces should be enumerated.

Parameter ``algHints``:
    passes requests to Regina for which specific enumeration algorithm
    should be used.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``null`` if no progress reporting is required.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::__init_2
static const char *__init_2 = R"doc(Constructs a new copy of the given list.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over the normal hypersurfaces in this list.

In Python, a normal hypersurface list can be treated as an iterable
object:

```
list = NormalHypersurfaces(...)
for s in list:
    ...
```

Returns:
    an iterator over the normal hypersurfaces in this list.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::__ne
static const char *__ne =
R"doc(Determines whether this and the given list contain different sets of
normal hypersurfaces.

The lists will be compared as multisets: the order of the
hypersurfaces in each list does not matter; however, in the unusual
scenario where a list the same hypersurface multiple times,
multiplicity does matter.

Like the comparison operators for NormalHypersurface, it does not
matter whether the lists work with different triangulations, or
different encodings, or if one but not the other supports non-compact
hypersurfaces. The individual hypersurfaces will simply be compared by
examining or computing the number of normal pieces of each type.

In particular, this routine is safe to call even if this and the given
list work with different triangulations:

* If the two triangulations have the same size, then this routine will
  compare hypersurfaces as though they were transplanted into the same
  triangulation using the same pentachoron numbering and the same
  normal piece types.

* If the two triangulations have different sizes, then this comparison
  will return ``True`` (i.e., the lists will be considered different).

Parameter ``other``:
    the list to be compared with this list.

Returns:
    ``True`` if both lists do not represent the same multiset of
    normal hypersurfaces, or ``False`` if they do.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::algorithm
static const char *algorithm =
R"doc(Returns details of the algorithm that was used to enumerate this list.

These may not be the same HyperAlg flags that were passed to the class
constructor. In particular, default values will have been explicitly
filled in, invalid and/or redundant values will have been removed, and
unavailable and/or unsupported combinations of algorithm flags will be
replaced with whatever algorithm was actually used.

Returns:
    details of the algorithm used to enumerate this list.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::allowsNonCompact
static const char *allowsNonCompact =
R"doc(Determines if the coordinate system that was used for enumeration
allows for non-compact hypersurfaces.

This does not test whether any of the hypersurfaces in this list are
actually non-compact; it simply returns a basic property of the
coordinate system that was used for enumeration.

Returns:
    ``True`` if and only if non-compact normal hypersurfaces are
    supported.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::coords
static const char *coords =
R"doc(Returns the coordinate system that was originally used to enumerate
the hypersurfaces in this list.

Returns:
    the coordinate system used.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given lists.

This global routine simply calls NormalHypersurfaces::swap(); it is
provided so that NormalHypersurfaces meets the C++ Swappable
requirements.

See NormalHypersurfaces::swap() for more details.

.. note::
    This swap function is *not* marked ``noexcept``, since it fires
    change events on both lists which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``lhs``:
    the list whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the list whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::hypersurface
static const char *hypersurface =
R"doc(Returns the hypersurface at the requested index in this list. This is
identical to using the square bracket operator.

Parameter ``index``:
    the index of the requested hypersurface in this list; this must be
    between 0 and size()-1 inclusive.

Returns:
    the normal hypersurface at the requested index in this list.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::isEmbeddedOnly
static const char *isEmbeddedOnly =
R"doc(Returns whether this list is known to contain only embedded normal
hypersurfaces.

If this returns ``False``, it does not guarantee that immersed and/or
singular hypersurfaces are present; it merely indicates that they were
not deliberately excluded (for instance, the prism constraints were
not enforced).

Returns:
    ``True`` if this list was constructed to contain only properly
    embedded hypersurfaces, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::recreateMatchingEquations
static const char *recreateMatchingEquations =
R"doc(Returns the matching equations that were used to create this normal
hypersurface list. This matrix is not cached: multiple calls to this
routine will result in the construction of multiple matrices. This
routine in fact merely calls makeMatchingEquations() with the
appropriate parameters.

The format of the matrix is identical to that returned by
makeMatchingEquations().

Note that there are situations in which makeMatchingEquations() throws
an exception (because the triangulation is not supported by the chosen
coordinate system). However, this routine will always succeed, because
if makeMatchingEquations() had failed then this normal hypersurface
list would not have been created in the first place.

Returns:
    the matching equations used to create this normal hypersurface
    list.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::size
static const char *size =
R"doc(Returns the number of hypersurfaces stored in this list.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the number of hypersurfaces.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::sort
static const char *sort =
R"doc(Sorts the hypersurfaces in this list according to the given criterion.

This sort is stable, i.e., hypersurfaces that are equivalent under the
given criterion will remain in the same relative order.

The implementation of this routine uses std::stable_sort.

Python:
    This is available in Python, and *comp* may be a pure Python
    function.

Parameter ``comp``:
    a binary function (or other callable object) that accepts two
    const HyperSurface references, and returns ``True`` if and only if
    the first hypersurface should appear before the second in the
    sorted list.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given list.

This routine will behave correctly if *other* is in fact this list.

.. note::
    This swap function is *not* marked ``noexcept``, since it fires
    change events on both lists which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``other``:
    the list whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::triangulation
static const char *triangulation =
R"doc(Returns the triangulation in which these normal hypersurfaces live.

This will be a snapshot frozen in time of the triangulation that was
originally passed to the NormalHypersurfaces constructor.

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
    As of Regina 7.0, you *cannot* access this triangulation via the
    packet tree as Packet::parent(). This is because normal
    hypersurface lists can now be kept anywhere in the packet tree, or
    can be kept as standalone objects outside the packet tree
    entirely.

Returns:
    a reference to the underlying triangulation.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::vectors
static const char *vectors =
R"doc(Returns a Python iterable object that iterates over the raw vectors
for all hypersurfaces in this list. For example:

```
list = NormalHypersurfaces(...)
for v in list.vectors():
    ...
```

Returns:
    an iterator over the normal hypersurfaces in this list.)doc";

// Docstring regina::python::doc::NormalHypersurfaces_::which
static const char *which =
R"doc(Returns details of which normal hypersurfaces this list represents
within the underlying triangulation.

This may not be the same HyperList that was passed to the class
constructor. In particular, default values will have been explicitly
filled in (such as HS_VERTEX and/or HS_EMBEDDED_ONLY), and invalid
and/or redundant values will have been removed.

Returns:
    details of what this list represents.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

