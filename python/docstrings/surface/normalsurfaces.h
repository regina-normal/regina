/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::NormalSurfaces
static const char *NormalSurfaces =
R"doc(A collection of normal surfaces in a 3-manifold triangulation.

There are some important changes to this class as of Regina 7.0:

* A normal surface list does _not_ need to be a child packet of the
  underlying triangulation, and indeed does not need to interact with
  the packet tree at all.

* You are welcome to modify or even destroy the original
  triangulation; if you do then this list will automatically make a
  private copy of the original triangulation as an ongoing reference.
  Different normal surface lists (and angle structure lists) can all
  share the same private copy, so this is not an expensive process.

* You should now create normal surface lists using the class
  constructor (but which, unlike the old enumerate(), does not insert
  the list into the packet tree). There is no need to use enumerate()
  any more.

Since Regina 7.0, this is no longer a "packet type" that can be
inserted directly into the packet tree. Instead a normal surface list
is now a standalone mathematatical object, which makes it slimmer and
faster for ad-hoc use. The consequences of this are:

* If you create your own NormalSurfaces object, it will not have any
  of the usual packet infrastructure. You cannot add it into the
  packet tree, and it will not support a label, tags, child/parent
  packets, and/or event listeners.

* To include an NormalSurfaces object in the packet tree, you must
  create a new PacketOf<NormalSurfaces>. This _is_ a packet type, and
  supports labels, tags, child/parent packets, and event listeners. It
  derives from NormalSurfaces, and so inherits the full NormalSurfaces
  interface.

* If you are adding new functions to this class that edit the list,
  you must still remember to create a PacketChangeSpan. This will
  ensure that, if the list is being managed by a
  PacketOf<NormalSurfaces>, then the appropriate packet change events
  will be fired. All other events (aside from packetToBeChanged() and
  packetWasChanged() are managed directly by the
  PacketOf<NormalSurfaces> wrapper class.

See the NormalSurface class notes for details of what to do when
introducing a new coordinate system.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::SurfaceExport
static const char *SurfaceExport =
R"doc(Used to describe a field, or a set of fields, that can be exported
alongside a normal surface list. This enumeration type is used with
export routines such as NormalSurfaces::saveCSVStandard() or
NormalSurfaces::saveCSVEdgeWeight().

This type describes fields to export _in addition_ to normal
coordinates, not the normal coordinates themselves (which are always
exported). Each field describes some property of a single normal
surface, and corresponds to a single column in a table of normal
surfaces.

This enumeration names individual fields, as well as some common
combinations of fields (such as ``None`` and ``All``). Fields can be
combined using the bitwise OR operator (resulting in an object of type
``Flags<SurfaceExport>``). In particular, if a surface export function
takes an argument of type ``Flags<SurfaceExport>``, then you can pass
a single SurfaceExport constant, or a bitwise combination of such
constants ``(field1 | field2)``, or empty braces ``{}`` to indicate no
fields at all.

The list of available fields may grow with future releases of Regina.)doc";

// Docstring regina::python::doc::__bor
static const char *__bor =
R"doc(Returns the bitwise OR of the two given flags.

Parameter ``lhs``:
    the first flag to combine.

Parameter ``rhs``:
    the second flag to combine.

Returns:
    the combination of both flags.)doc";

// Docstring regina::python::doc::makeEmbeddedConstraints
static const char *makeEmbeddedConstraints =
R"doc(Generates the validity constraints representing the condition that
normal surfaces be embedded. The validity constraints will be
expressed relative to the given coordinate system.

For some coordinate systems, these will include additional constraints
of a similar nature (i.e., restricting which combinations of
coordinates may be non-zero). For instance, in almost normal
coordinates, there will typically be an extra constraint insisting
that at most one octagon type is non-zero across the entire
triangulation.

These are the constraints that will be used when enumerating embedded
surfaces in the given coordinate system (i.e., when the default flag
NormalList::EmbeddedOnly is used). They will not be used when the
enumeration allows for immersed and/or singular surfaces.

Parameter ``triangulation``:
    the triangulation upon which these validity constraints will be
    based.

Parameter ``coords``:
    the coordinate system to be used.

Returns:
    the set of validity constraints.)doc";

// Docstring regina::python::doc::makeMatchingEquations
static const char *makeMatchingEquations =
R"doc(Generates the set of normal surface matching equations for the given
triangulation using the given coordinate system.

These are the matching equations that will be used when enumerating
normal surfaces in the coordinate system *coords*.

Each equation will be represented as a row of the resulting matrix.
Each column of the matrix represents a coordinate in the given
coordinate system.

Exception ``InvalidArgument``:
    The matching equations could not be created for the given
    triangulation in the given coordinate system, due to an error that
    should have been preventable with the right checks in advance.
    This can only happen in certain coordinate systems, and for all
    such coordinate systems this is explicitly described in the
    NormalCoords enum documentation.

Exception ``UnsolvedCase``:
    The matching equations could not be created for the given
    triangulation in the given coordinate system, due to an error that
    was "genuinely" unforseeable. Again this can only happen in
    certain coordinate systems, where this is explicitly described in
    the NormalCoords enum documentation.

Parameter ``triangulation``:
    the triangulation upon which these matching equations will be
    based.

Parameter ``coords``:
    the coordinate system to be used.

Returns:
    the resulting set of matching equations.)doc";

namespace NormalSurfaces_ {

// Docstring regina::python::doc::NormalSurfaces_::__array
static const char *__array =
R"doc(Returns the surface at the requested index in this list. This is
identical to calling surface().

Parameter ``index``:
    the index of the requested surface in this list; this must be
    between 0 and size()-1 inclusive.

Returns:
    the normal surface at the requested index in this list.)doc";

// Docstring regina::python::doc::NormalSurfaces_::__copy
static const char *__copy = R"doc(Constructs a new copy of the given list.)doc";

// Docstring regina::python::doc::NormalSurfaces_::__eq
static const char *__eq =
R"doc(Determines whether this and the given list contain the same set of
normal (or almost normal) surfaces.

The lists will be compared as multisets: the order of the surfaces in
each list does not matter; however, in the unusual scenario where a
list the same surface multiple times, multiplicity does matter.

Like the comparison operators for NormalSurface, it does not matter
whether the lists work with different triangulations, or different
encodings, or if one but not the other supports almost normal and/or
spun-normal surfaces. The individual surfaces will simply be compared
by examining or computing the number of discs of each type.

In particular, this routine is safe to call even if this and the given
list work with different triangulations:

* If the two triangulations have the same size, then this routine will
  compare surfaces as though they were transplanted into the same
  triangulation using the same tetrahedron numbering and the same disc
  types.

* If the two triangulations have different sizes, then this comparison
  will return ``False``.

Parameter ``other``:
    the list to be compared with this list.

Returns:
    ``True`` if both lists represent the same multiset of normal or
    almost normal surfaces, or ``False`` if not.)doc";

// Docstring regina::python::doc::NormalSurfaces_::__init
static const char *__init =
R"doc(A unified "enumeration constructor" for enumerating various classes of
normal surfaces within a given triangulation.

The NormalCoords argument allows you to specify an underlying
coordinate system in which to do the enumeration (e.g., standard
coordinates, quadrilateral coordinates or almost normal coordinates).
This choice of coordinate system will affect which surfaces are
produced, since vertex/fundamental surfaces in one system are not
necessarily vertex/fundamental in another.

The *whichList* argument is a combination of flags that allows you to
specify exactly which normal surfaces you require. This includes (i)
whether you want all vertex surfaces or all fundamental surfaces,
which defaults to NormalList::Vertex if you specify neither or both;
and (ii) whether you want only properly embedded surfaces or you also
wish to include immersed and/or singular surfaces, which defaults to
NormalList::EmbeddedOnly if you specify neither or both.

The *algHints* argument is a combination of flags that allows you to
control the underlying enumeration algorithm. These flags are treated
as hints only: if your selection of algorithm is invalid, unavailable
or unsupported then Regina will choose something more appropriate.
Unless you have some specialised need, the default NormalAlg::Default
(which makes no hints at all) will allow Regina to choose what it
thinks will be the most efficient method.

The enumerated surfaces will be stored in this new normal surface
list, and their representations will be scaled down to use the
smallest possible integer coordinates.

Unlike the old enumerate() function, the new normal surface list will
_not_ be inserted into the packet tree. Moreover, the given
triangulation may change or even be destroyed without causing
problems. See the class notes for details.

If a progress tracker is passed, this routine will declare and work
through a series of stages whose combined weights sum to 1; typically
this means that the given tracker must not have been used before.

This constructor will not return until the enumeration of surfaces is
complete, regardless of whether a progress tracker was passed. If you
need the behaviour of the old enumerate() (where passing a progress
tracker caused the enumeration to start in the background), simply
call this constructor in a new detached thread. Note that this
enumeration can be extremely slow for larger triangulations, and so
there could be good reasons to do this.

If an error occurs, then this routine will thrown an exception. In
this case, no normal surface list will be created, and the progress
tracker (if passed) will be marked as finished. See the exception
specifications below for details.

Exception ``InvalidArgument``:
    The matching equations could not be created for the given
    triangulation in the given coordinate system, due to an error that
    should have been preventable with the right checks in advance.
    This can only happen in certain coordinate systems, and for all
    such coordinate systems this is explicitly described in the
    NormalCoords enum documentation.

Exception ``UnsolvedCase``:
    The list of hypersurfaces could not be be created for the given
    triangulation in the given coordinate system, due to an error that
    was "genuinely" unforseeable. Currently there are two scenarios in
    which this could happen: (i) the matching equations could not be
    constructed, which can only happen in certain coordinate systems
    where this is explicitly described in the NormalCoords enum
    documentation; or (ii) the arguments require enumerating
    _fundamental_ normal surfaces using the primal Hilbert basis
    algorithm, and Normaliz was unable to complete its portion of the
    task, which in theory should never happen at all.

Python:
    The global interpreter lock will be released while this
    constructor runs, so you can use it with Python-based
    multithreading.

Parameter ``triangulation``:
    the triangulation upon which this list of normal surfaces will be
    based.

Parameter ``coords``:
    the coordinate system to be used. This must be one of the system
    that Regina is able to use for enumeration; this is documented
    alongside each NormalCoords enum value.

Parameter ``whichList``:
    indicates which normal surfaces should be enumerated.

Parameter ``algHints``:
    passes requests to Regina for which specific enumeration algorithm
    should be used.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.)doc";

// Docstring regina::python::doc::NormalSurfaces_::__init_2
static const char *__init_2 =
R"doc(A unified "transform constructor" for transforming one normal surface
list into another.

The available transformations include:

* conversions between vertex surfaces in different coordinate systems
  (e.g., between the set of all standard vertex surfaces and the
  typically smaller set of all quad vertex surfaces);

* filters that select a subset of surfaces (e.g., only surfaces that
  have a locally compatible or disjoint partner, or only surfaces that
  could potentially be incompressible).

Each transformation comes with its own set of preconditions, as
documented alongside the various NormalTransform enumeration
constants. These preconditions will be checked, and if any of them
fails then this constructor will throw an exception (see below).

Unlike the old conversion and filter functions, this constructor will
_not_ insert the new normal surface list into the packet tree.

Exception ``FailedPrecondition``:
    The preconditions for the given transformation were not met. See
    each NormalTransform enum constant for the corresponding set of
    preconditions.

Parameter ``src``:
    the normal surface list that we wish to transform; this will not
    be modified.

Parameter ``transform``:
    the specific transformation to apply.)doc";

// Docstring regina::python::doc::NormalSurfaces_::__init_3
static const char *__init_3 =
R"doc(A "filter constructor" that creates a new list filled with those
surfaces from the given list that pass the given filter.

Unlike the old filter() function, this constructor will _not_ insert
the new normal surface list into the packet tree.

For this new filtered list, which() will include the
NormalList::Custom flag, and algorithm() will include the
NormalAlg::Custom flag.

Parameter ``src``:
    the normal surface list that we wish to filter; this will not be
    modified.

Parameter ``filter``:
    the filter to apply to the given list.)doc";

// Docstring regina::python::doc::NormalSurfaces_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over the normal surfaces in this list.

In Python, a normal surface list can be treated as an iterable object:

```
list = NormalSurfaces(...)
for s in list:
    ...
```

Returns:
    an iterator over the normal surfaces in this list.)doc";

// Docstring regina::python::doc::NormalSurfaces_::algorithm
static const char *algorithm =
R"doc(Returns details of the algorithm that was used to enumerate this list.

These may not be the same algorithm flags that were passed to the
class constructor. In particular, default values will have been
explicitly filled in, invalid and/or redundant values will have been
removed, and unavailable and/or unsupported combinations of algorithm
flags will be replaced with whatever algorithm was actually used.

Returns:
    details of the algorithm used to enumerate this list.)doc";

// Docstring regina::python::doc::NormalSurfaces_::allowsAlmostNormal
static const char *allowsAlmostNormal =
R"doc(Determines if the coordinate system that was used for enumeration
allows for almost normal surfaces.

This does not test whether any of the surfaces in this list actually
contain octagons: it simply returns a basic property of the coordinate
system that was used for enumeration.

Returns:
    ``True`` if and only if almost normal surfaces are supported.)doc";

// Docstring regina::python::doc::NormalSurfaces_::allowsNonCompact
static const char *allowsNonCompact =
R"doc(Determines if the coordinate system that was used for enumeration
allows for non-compact normal surfaces.

This does not test whether any of the surfaces in this list are
actually non-compact: it simply returns a basic property of the
coordinate system that was used for enumeration.

Returns:
    ``True`` if and only if non-compact normal surfaces are supported.)doc";

// Docstring regina::python::doc::NormalSurfaces_::coords
static const char *coords =
R"doc(Returns the coordinate system that was originally used to enumerate
the surfaces in this list.

Returns:
    the coordinate system used.)doc";

// Docstring regina::python::doc::NormalSurfaces_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given lists.

This global routine simply calls NormalSurfaces::swap(); it is
provided so that NormalSurfaces meets the C++ Swappable requirements.

See NormalSurfaces::swap() for more details.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both lists which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``lhs``:
    the list whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the list whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::NormalSurfaces_::isEmbeddedOnly
static const char *isEmbeddedOnly =
R"doc(Returns whether this list was constructed to contain only properly
embedded surfaces.

If this returns ``False``, it does not guarantee that immersed and/or
singular surfaces are present; it merely indicates that they were not
deliberately excluded (for instance, the quadrilateral constraints
were not enforced).

Returns:
    ``True`` if this list was constructed to contain only properly
    embedded surfaces, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::NormalSurfaces_::recreateMatchingEquations
static const char *recreateMatchingEquations =
R"doc(Returns the matching equations that were used to create this normal
surface list. The matrix is not cached: multiple calls to this routine
will result in the construction of multiple matrices. This routine in
fact merely calls makeMatchingEquations() with the appropriate
parameters.

The format of the matrix is identical to that returned by
makeMatchingEquations().

Note that there are situations in which makeMatchingEquations() throws
an exception (because the triangulation is not supported by the chosen
coordinate system). However, this routine will always succeed, because
if makeMatchingEquations() had failed then this normal surface list
would not have been created in the first place.

Returns:
    the matching equations used to create this normal surface list.)doc";

// Docstring regina::python::doc::NormalSurfaces_::saveCSVEdgeWeight
static const char *saveCSVEdgeWeight =
R"doc(Exports the given list of normal surfaces as a plain text CSV (comma-
separated value) file, using edge weight coordinates. CSV files are
human-readable and human-editable, and are suitable for importing into
spreadsheets and databases.

The surfaces will be exported in edge weight coordinates. Thus there
will be one coordinate for each edge of the underlying triangulation;
each such coordinate will become a separate field in the CSV file.

As well as the normal surface coordinates, additional properties of
the normal surfaces (such as Euler characteristic, orientability, and
so on) can be included as extra fields in the export. Users can select
precisely which properties to include by passing a bitwise OR
combination of constants from the regina::SurfaceExport enumeration
type.

The CSV format used here begins with a header row, and uses commas as
field separators. Text fields with arbitrary contents are placed
inside double quotes, and the double quote character itself is
represented by a pair of double quotes. Thus the string ``my "normal"
surface's name`` would be stored as ``"my ""normal"" surface's
name"``.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file _name_, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. Any user strings
    such as surface names will be written in UTF-8.

Parameter ``filename``:
    the name of the CSV file to export to.

Parameter ``additionalFields``:
    a bitwise OR combination of constants from regina::SurfaceExport
    indicating which additional properties of surfaces should be
    included in the export.

Returns:
    ``True`` if the export was successful, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::NormalSurfaces_::saveCSVStandard
static const char *saveCSVStandard =
R"doc(Exports this list of normal surfaces as a plain text CSV (comma-
separated value) file, using standard coordinates. CSV files are
human-readable and human-editable, and are suitable for importing into
spreadsheets and databases.

The surfaces will be exported in standard coordinates (tri-quad
coordinates for normal surfaces, or tri-quad-oct coordinates for
almost normal surfaces). Each coordinate will become a separate field
in the CSV file.

As well as the normal surface coordinates, additional properties of
the normal surfaces (such as Euler characteristic, orientability, and
so on) can be included as extra fields in the export. Users can select
precisely which properties to include by passing a bitwise OR
combination of constants from the regina::SurfaceExport enumeration
type.

The CSV format used here begins with a header row, and uses commas as
field separators. Text fields with arbitrary contents are placed
inside double quotes, and the double quote character itself is
represented by a pair of double quotes. Thus the string ``my "normal"
surface's name`` would be stored as ``"my ""normal"" surface's
name"``.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file _name_, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. Any user strings
    such as surface names will be written in UTF-8.

Parameter ``filename``:
    the name of the CSV file to export to.

Parameter ``additionalFields``:
    a bitwise OR combination of constants from regina::SurfaceExport
    indicating which additional properties of surfaces should be
    included in the export.

Returns:
    ``True`` if the export was successful, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::NormalSurfaces_::size
static const char *size =
R"doc(Returns the number of surfaces stored in this list.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the number of surfaces.)doc";

// Docstring regina::python::doc::NormalSurfaces_::sort
static const char *sort =
R"doc(Sorts the surfaces in this list according to the given criterion.

This sort is stable, i.e., surfaces that are equivalent under the
given criterion will remain in the same relative order.

The implementation of this routine uses std::stable_sort.

Python:
    This is available in Python, and *comp* may be a pure Python
    function.

Parameter ``comp``:
    a binary function (or other callable type) that accepts two const
    NormalSurface references, and returns ``True`` if and only if the
    first surface should appear before the second in the sorted list.)doc";

// Docstring regina::python::doc::NormalSurfaces_::surface
static const char *surface =
R"doc(Returns the surface at the requested index in this list. This is
identical to using the square bracket operator.

Parameter ``index``:
    the index of the requested surface in this list; this must be
    between 0 and size()-1 inclusive.

Returns:
    the normal surface at the requested index in this list.)doc";

// Docstring regina::python::doc::NormalSurfaces_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given list.

This routine will behave correctly if *other* is in fact this list.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both lists which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``other``:
    the list whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::NormalSurfaces_::triangulation
static const char *triangulation =
R"doc(Returns the triangulation in which these normal surfaces live.

This will be a snapshot frozen in time of the triangulation that was
originally passed to the NormalSurfaces constructor.

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
    packet tree as Packet::parent(). This is because normal surface
    lists can now be kept anywhere in the packet tree, or can be kept
    as standalone objects outside the packet tree entirely.

Returns:
    a reference to the underlying triangulation.)doc";

// Docstring regina::python::doc::NormalSurfaces_::vectors
static const char *vectors =
R"doc(Returns a Python iterable object that iterates over the raw vectors
for all surfaces in this list. For example:

```
list = NormalSurfaces(...)
for v in list.vectors():
    ...
```

Returns:
    an iterator over the normal surfaces in this list.)doc";

// Docstring regina::python::doc::NormalSurfaces_::which
static const char *which =
R"doc(Returns details of which normal surfaces this list represents within
the underlying triangulation.

These may not be the same list flags that were passed to the class
constructor. In particular, default values will have been explicitly
filled in (such as NormalList::Vertex and/or
NormalList::EmbeddedOnly), and invalid and/or redundant values will
have been removed.

Returns:
    details of what this list represents.)doc";

}

namespace SurfaceExport_ {

// Docstring regina::python::doc::SurfaceExport_::All
static const char *All =
R"doc(Indicates that all available fields should be exported, including the
user-assigned surface name. Since the list of available fields may
grow with future releases, the numerical value of this constant may
change as a result.)doc";

// Docstring regina::python::doc::SurfaceExport_::AllButName
static const char *AllButName =
R"doc(Indicates that all available fields should be exported, except for the
user-assigned surface name. Since the list of available fields may
grow with future releases, the numerical value of this constant may
change as a result.)doc";

// Docstring regina::python::doc::SurfaceExport_::Bdry
static const char *Bdry =
R"doc(Represents the calculated property of whether a surface is bounded. In
most cases, this will be one of the strings "closed", "real bdry" or
"infinite" (where "infinite" indicates a surface with infinitely many
discs). For spun-normal surfaces in certain ideal triangulations, this
string will be followed by the boundary slopes of the surface at the
cusps: these written as a list of pairs (*p*, *q*), one for each cusp,
indicating that the boundary curves of the surface run *p* times
around the meridian and *q* times around the longitude. See
NormalSurface::boundaryIntersections() for further information on
interpreting these values.)doc";

// Docstring regina::python::doc::SurfaceExport_::Euler
static const char *Euler =
R"doc(Represents the calculated Euler characteristic of a surface. This will
be an integer, and will be left empty if the Euler characteristic
cannot be computed.)doc";

// Docstring regina::python::doc::SurfaceExport_::Link
static const char *Link =
R"doc(Represents whether a surface is a single vertex link or a thin edge
link. See NormalSurface::isVertexLink() and
NormalSurface::isThinEdgeLink() for details. This will be written as a
human-readable string.)doc";

// Docstring regina::python::doc::SurfaceExport_::Name
static const char *Name = R"doc(Represents the user-assigned surface name.)doc";

// Docstring regina::python::doc::SurfaceExport_::None
static const char *None =
R"doc(Indicates that no fields should be exported (except for the normal
coordinates, which are always exported).

Python:
    This constant is called ``Nil``, since ``None`` is a reserved word
    in Python.)doc";

// Docstring regina::python::doc::SurfaceExport_::Orient
static const char *Orient =
R"doc(Represents the calculated property of whether a surface is orientable.
This will be the string ``TRUE`` or ``FALSE``, or will be left empty
if the orientability cannot be computed.)doc";

// Docstring regina::python::doc::SurfaceExport_::Sides
static const char *Sides =
R"doc(Represents the calculated property of whether a surface is one-sided
or two-sided. This will be the integer 1 or 2, or will be left empty
if the "sidedness" cannot be computed.)doc";

// Docstring regina::python::doc::SurfaceExport_::Type
static const char *Type =
R"doc(Represents any additional high-level properties of a surface, such as
whether it is a splitting surface or a central surface. This will be
written as a human-readable string. This field is somewhat arbitrary,
and the precise properties it describes are subject to change in
future releases of Regina.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

