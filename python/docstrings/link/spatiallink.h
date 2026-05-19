/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SpatialLink
inline constexpr const char SpatialLink[] =
R"doc(Represents a specific embedding of a directed knot or link in real
3-dimensional space.

This class SpatialLink is a "purely geometric" representation of a
link, as opposed to the Link class which is a "purely combinatorial"
representation (holding the combintorics of a 2-dimensional link
diagram, with no geometric information at all about the specific
placements of strands or crossings).

One caveat with using the SpatialLink class is that it uses floating
point arithmetic. This makes it good for visualisation purposes, but
makes it susceptible to floating point errors. If you need to perform
exact computations (for example, of link invariants), use the Link
class instead.

This class supports links with any number of components (including
zero). Each component is made up of a non-empty sequence of nodes,
which are points in 3-dimensional space represented by objects of type
Vector3D. The nodes in each component are connected by straight line
segments to form a closed loop.

It is assumed that this indeed forms an embedding (i.e., no two nodes
are equal, no node meets any other line segment beyond the two that it
sits between on its link component, and no two line segments meet
beyond the expect cases of two adjacent segments touching at their
common endpoint). This is _not_ checked, and indeed the use of
floating point arithmetic makes it difficult to check this precisely.
Note that, as a consequence of forming an embedding, each link
component must contain at least three nodes.

It is assumed that the underlying coordinate system is right-handed.

Like the regular Link and Triangulation classes, SpatialLink is not a
packet type that can be inserted directly into the packet tree.
Instead it is a standalone mathematatical object, which makes it
slimmer and faster for ad-hoc use. Therefore:

* If you create your own SpatialLink, it will not have any of the
  usual packet infrastructure. You cannot add it into the packet tree,
  and it will not support a label, tags, child/parent packets, and/or
  event listeners.

* To include a SpatialLink in the packet tree, you must create a new
  PacketOf<SpatialLink>. This _is_ a packet type, and supports labels,
  tags, child/parent packets, and event listeners. It derives from
  SpatialLink, and so inherits the full SpatialLink interface.

If you are adding new functions to this class that edit the internal
data structures of the link, you must remember to surround these
changes with a ChangeAndClearSpan. This manages bookkeeping such as
clearing computed properties, and (if this link _does_ belong to a
packet) firing packet change events.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace SpatialLink_ {

// Docstring regina::python::doc::SpatialLink_::__copy
inline constexpr const char __copy[] = R"doc(Constructs a new copy of the given link.)doc";

// Docstring regina::python::doc::SpatialLink_::__default
inline constexpr const char __default[] = R"doc(Constructs an empty link. This will have zero components.)doc";

// Docstring regina::python::doc::SpatialLink_::__eq
inline constexpr const char __eq[] =
R"doc(Determines if this link is identical to the given link.

Here "identical" means that both links follow exactly the same paths
through 3-dimensional space, with their components and nodes stored in
exactly the same order.

If any rendering radii have been fixed (e.g., via setRadius()), these
will be ignored for the purpose of this comparison.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating point comparisons.

Parameter ``other``:
    the link to compare with this.

Returns:
    ``True`` if and only if the two links are identical.)doc";

// Docstring regina::python::doc::SpatialLink_::__init
inline constexpr const char __init[] =
R"doc(Creates a new link whose components are given by sequences of points
in 3-space.

The input is presented as a sequence of sequences:

* Each element of the "outer" sequence (defined by the *begin* and
  *end* iterator arguments to this routine) should be an "inner"
  sequence representing a single link component.

* Each "inner" sequence (i.e., each individual link component) should
  be a sequence of nodes (i.e., points in 3-space). These are the
  points that will be stored directly in the Component structure,
  which means that to form the actual geometry of the link component:

* each node in the sequence is joined by a straight line segment to
  the node that follows it, and likewise, the last node is joined to
  the first;

* the orientation of the link component follows the path in order from
  the first node to the last (and then cycling back to the front of
  the sequence again).

* In particular, each "inner" sequence must contain at least three
  nodes (the minimum required for an embedded piecewise-linear cycle
  in 3-space).

This routine does not insist on any specific types for the sequences,
as long as the outer and inner sequences all support iteration, and
the elements of the inner sequences can be converted to the node type
``Vector3D<double>``.

For example, your code might look like:

```
std::vector<std::vector<std::array<double, 3>>> data = ...;
SpatialLink link(data.begin(), data.end());
```

This constructor induces a deep copy of the given data.

Python:
    Instead of the iterators *begin* and *end*, this routine takes
    either (i) a Python list of lists of triples of real numbers, or
    (ii) a Python list of lists of Vector3D objects.

Parameter ``begin``:
    the beginning of the outer sequence of link components.

Parameter ``end``:
    a past-the-end iterator indicating the end of the outer sequence
    of components.)doc";

// Docstring regina::python::doc::SpatialLink_::clearRadius
inline constexpr const char clearRadius[] =
R"doc(Removes any user-specified radius to use when rendering this link.

Any subsequent calls to radius() will return a sensible default, as
computed by defaultRadius().)doc";

// Docstring regina::python::doc::SpatialLink_::component
inline constexpr const char component[] =
R"doc(Returns a reference to the component at the given index within this
link.

Parameter ``index``:
    the index of the requested component. This must be between 0 and
    countComponents()-1 inclusive.

Returns:
    the component at the given index.)doc";

// Docstring regina::python::doc::SpatialLink_::componentSize
inline constexpr const char componentSize[] =
R"doc(Returns the number of nodes that are stored for the given component of
this link.

This is equivalent to calling ``component[componentIndex].size()``.

Parameter ``componentIndex``:
    indicates the link component to query; this must be between 0 and
    ``countComponents() - 1`` inclusive.

Returns:
    the number of nodes stored for the requested component.)doc";

// Docstring regina::python::doc::SpatialLink_::components
inline constexpr const char components[] =
R"doc(Returns an object that allows iteration through and random access to
all components of this link.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be a lightweight view type from
the ``std::ranges`` library, which means it supports range-based
``for`` loops. Each element of the list will be a constant reference
to some component; more precisely, iterating through this list is
equivalent to calling ``component(0)``, ``component(1)``, ...,
``component(countComponents()-1)`` in turn. As an example, your code
might look like:

```
for (const SpatialLink::Component& c : link.components()) { ... }
```

The object that is returned will remain up-to-date and valid for as
long as the link exists: even as components are added and/or removed,
it will always reflect the components that are currently in the link.
Nevertheless, it is recommended to treat this object as temporary
only, and to call components() again each time you need it.

Returns:
    access to the list of all components.)doc";

// Docstring regina::python::doc::SpatialLink_::countComponents
inline constexpr const char countComponents[] =
R"doc(Returns the number of components in this link.

Returns:
    the number of components.)doc";

// Docstring regina::python::doc::SpatialLink_::defaultRadius
inline constexpr const char defaultRadius[] =
R"doc(Returns a sensible default radius to use when rendering the link.
Specifically, this is the radius to use for the balls and cylinders
used in the 3-D model.

As of Regina 8.0, this routine chooses a radius by finding the closest
non-adjacent pair of link nodes that are connected in the relative
neighbourhood graph. Thanks to Kate Turner for this excellent
suggestion. The current algorithm is best-case quadratic and worst-
case cubic in the total number of nodes.

This function is expensive to call the first time, but it caches its
value and so subsesquent calls are essentially instantaneous (until
the embedding of the link changes, at which point the cached value
will be cleared).

The choice of radius is subject to change in future versions of
Regina.

Returns:
    a sensible default radius to use for rendering.)doc";

// Docstring regina::python::doc::SpatialLink_::fromKnotPlot
inline constexpr const char fromKnotPlot[] =
R"doc(Creates a new link from a KnotPlot data file. Since KnotPlot files are
in a binary format, this routine takes a _filename_ (not the file
contents).

For further information on the KnotPlot file format, see
https://knotplot.com/manual/FileFormats.html .

.. warning::
    The KnotPlot binary data format makes use of 32-bit and 64-bit
    floating-point numbers. For the time being, this means that this
    import will _only_ work on systems where ``float`` and ``double``
    are 32-bit and 64-bit respectively. This is at least true on
    ``x86_64`` chips (64-bit intel) and ``arm64`` chips (e.g., Apple
    Silicon). The sizes of the floating point types will be checked,
    and if this requirement fails to hold then this routine will throw
    a NotImplemented exception.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test
    whether the link is embedded. It is currently up to the user to
    enforce this.

Internationalisation:
    If the given argument is a filename, then this routine makes no
    assumptions about the character encoding used in the filename, and
    simply passes it through to low-level C/C++ file I/O routines.

Exception ``FileError``:
    The given file could not be read.

Exception ``InvalidInput``:
    The contents of the given file could not be interpreted as a
    KnotPlot data file, or the data file contains a structure that
    cannot be represented by a SpatialLink (e.g., a link that is cut
    open leaving free ends).

Exception ``NotImplemented``:
    The chipset on this machine uses floating-point types that are
    incompatible with KnotPlot's binary file format.

Parameter ``filename``:
    the name of a KnotPlot data file.

Returns:
    the reconstructed spatial link.)doc";

// Docstring regina::python::doc::SpatialLink_::global_swap
inline constexpr const char global_swap[] =
R"doc(Swaps the contents of the two given spatial links.

This global routine simply calls SpatialLink::swap(); it is provided
so that SpatialLink meets the C++ Swappable requirements.

See SpatialLink::swap() for more details.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both links which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``lhs``:
    the spatial link whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the spatial link whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::SpatialLink_::hasRadius
inline constexpr const char hasRadius[] =
R"doc(Indicates whether the user has set their own custom radius to use when
rendering this link.

Returns:
    ``True`` if a custom radius has been set (e.g., via setRadius()),
    or ``False`` if the default radius should be used (as computed by
    defaultRadius()).)doc";

// Docstring regina::python::doc::SpatialLink_::isEmpty
inline constexpr const char isEmpty[] =
R"doc(Determines whether this link is empty. An empty link is one with no
components at all.

Returns:
    ``True`` if and only if this link is empty.)doc";

// Docstring regina::python::doc::SpatialLink_::node
inline constexpr const char node[] =
R"doc(Returns a particular node belong to a particular component of this
link.

This is equivalent to calling
``component[componentIndex][nodeIndex]``.

Parameter ``componentIndex``:
    indicates the component of the link to which the requested node
    belongs; this must be between 0 and ``countComponents() - 1``
    inclusive.

Parameter ``nodeIndex``:
    indicates which node to return from the given component; this must
    be between 0 and ``componentSize(componentIndex) - 1`` inclusive.)doc";

// Docstring regina::python::doc::SpatialLink_::nodes
inline constexpr const char nodes[] =
R"doc(Returns a Python iterable object that iterates over all nodes in this
link. For example:

```
link = SpatialLink(...)
for n in link.nodes():
    ...
```

The order of iteration will be: all of the nodes in the first
component in order; then all of the nodes in the second component in
order; and so on.

Returns:
    an iterator over all nodes in this link.)doc";

// Docstring regina::python::doc::SpatialLink_::radius
inline constexpr const char radius[] =
R"doc(Returns the radius that should be used when rendering this link.
Specifically, this is the radius to use for the balls and cylinders
used in the 3-D model.

If the user has fixed their own radius (e.g., via setRadius()), then
that radius will be returned. Otherwise a sensible default (as
computed by defaultRadius()) will be returned.

Returns:
    the radius to use when rendering this link.)doc";

// Docstring regina::python::doc::SpatialLink_::range
inline constexpr const char range[] =
R"doc(Returns the range of coordinates that this link occupies.

Specifically, this routine returns a pair ``(min, max)``, where *min*
contains the minimum *x*, *y* and *z* coordinates over all nodes, and
*max* contains the maximum *x*, *y* and *z* coordinates over all
nodes.

Returns:
    the range of coordinates. If this link contains no nodes at all
    then this routine will return ``((0,0,0), (0,0,0))``.)doc";

// Docstring regina::python::doc::SpatialLink_::refine
inline constexpr const char refine[] =
R"doc(Adds additional nodes to make the embedding appear smoother.

Specifically, each adjacent pair of nodes will have one new node
inserted between them (thereby doubling the number of nodes and arcs
overall). This new node is _not_ added at the midpoint of line segment
between the two original nodes (which would not help with smoothing);
instead it is calculated to lie on a Catmull-Rom spline defined by the
original nodes. This spline is configured to have tension τ=0.5.

See also refine(int), which allows for many new nodes to be inserted
between each adjacent pair of original nodes. Calling ``refine()`` is
equivalent to calling ``refine(2)`` (but uses a more streamlined
implementation).

.. warning::
    In the current implementation, there is no guarantee that this
    operation will not inadvertently pass one strand through another.
    (This could happen, for instance, if two parts of the link with
    very tight curvature pass very close to one another). The hope is
    to explicitly prevent this in a later implementation.)doc";

// Docstring regina::python::doc::SpatialLink_::refine_2
inline constexpr const char refine_2[] =
R"doc(Adds a configurable number of additional nodes to make the embedding
appear smoother.

Specifically, each adjacent pair of nodes will have ``sub - 1`` new
nodes inserted between them (thereby multiplying the number of nodes
and arcs by *sub* overall). The new nodes are _not_ added along the
line segments joining the original nodes (since this would not help
with smoothing); instead they are calculated to lie on Catmull-Rom
splines defined by the original nodes. These splines are configured to
have tension τ=0.5.

See also refine(), which allows for many new nodes to be inserted
between each adjacent pair of original nodes. Calling ``refine()`` is
equivalent to calling ``refine(2)`` (but uses a more streamlined
implementation).

.. warning::
    In the current implementation, there is no guarantee that this
    operation will not inadvertently pass one strand through another.
    (This could happen, for instance, if two parts of the link with
    very tight curvature pass very close to one another). The hope is
    to explicitly prevent this in a later implementation.

Parameter ``sub``:
    the number of pieces that each original arc (i.e., line segment)
    should be subdivided into. This must be at least 2.)doc";

// Docstring regina::python::doc::SpatialLink_::reflect
inline constexpr const char reflect[] =
R"doc(Reflects the link in plane perpendicular to the given axis.

Specifically:

* if *axis* is 0 then all *x* coordinates will be negated;

* if *axis* is 1 then all *y* coordinates will be negated;

* if *axis* is 2 then all *z* coordinates will be negated.

Exception ``InvalidInput``:
    The argument *axis* was not 0, 1 or 2.

Parameter ``axis``:
    indicates the axis of reflection, as described above.)doc";

// Docstring regina::python::doc::SpatialLink_::scale
inline constexpr const char scale[] =
R"doc(Scales the entire link by the given factor.

Specifically, all coordinates of all nodes will be multiplied by
*factor*.

The rendering radius, if this has been fixed, will be scaled also.

Parameter ``factor``:
    the scaling factor; this must not be zero.)doc";

// Docstring regina::python::doc::SpatialLink_::setRadius
inline constexpr const char setRadius[] =
R"doc(Indicates that the given radius should be used when rendering this
link.

The given value will be returned by any subsequent calls to radius().

Parameter ``useRadius``:
    the radius to use when rendering this link; this must be strictly
    positive.)doc";

// Docstring regina::python::doc::SpatialLink_::size
inline constexpr const char size[] =
R"doc(Returns the total number of nodes in this spatial link.

.. warning::
    This is not a constant time operation, since it sums the sizes of
    the individual components.

Returns:
    the total number of nodes.)doc";

// Docstring regina::python::doc::SpatialLink_::swap
inline constexpr const char swap[] =
R"doc(Swaps the contents of this and the given link.

All crossings that belong to this link will be moved to *other*, and
all crossings that belong to *other* will be moved to this link.

This routine will behave correctly if *other* is in fact this link.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both links which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``other``:
    the link whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::SpatialLink_::translate
inline constexpr const char translate[] =
R"doc(Translates the entire link by the given vector.

Specifically, the *x*, *y* and *z* coordinates of all nodes will be
incremented by ``vector.x``, ``vector.y`` and ``vector.z``
respectively.

Parameter ``vector``:
    holds the three constants that should be added to the *x*, *y* and
    *z* coordinates of every node.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

