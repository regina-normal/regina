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
static const char *SpatialLink =
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
zero). Each component is made up of a sequence of points in
3-dimensional space, each represented by a SpatialLink::Node, which
are connected by straight line segments to form a closed loop.

It is assumed that this indeed forms an embedding (i.e., no two nodes
are equal, no node meets any other line segment beyond the two that it
sits between on its link component, and no two line segments meet
beyond the expect cases of two adjacent segments touching at their
common endpoint). This is _not_ checked, and indeed the use of
floating point arithmetic makes it difficult to check this precisely.

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
changes with a ChangeAndClearSpan. This manages bookkeeping, such as
(if this link _does_ belong to a packet) firing packet change events.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace SpatialLink_ {

// Docstring regina::python::doc::SpatialLink_::Node
static const char *Node =
R"doc(Represents a single point on the path that a link component takes
through three-dimensional space.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

// Docstring regina::python::doc::SpatialLink_::__copy
static const char *__copy = R"doc(Constructs a new copy of the given link.)doc";

// Docstring regina::python::doc::SpatialLink_::__default
static const char *__default = R"doc(Constructs an empty link. This will have zero components.)doc";

// Docstring regina::python::doc::SpatialLink_::__eq
static const char *__eq =
R"doc(Determines if this link is identical to the given link.

Here "identical" means that both links follow exactly the same paths
through 3-dimensional space, with their components and nodes stored in
exactly the same order.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating point comparisons.

Parameter ``other``:
    the link to compare with this.

Returns:
    ``True`` if and only if the two links are identical.)doc";

// Docstring regina::python::doc::SpatialLink_::__init
static const char *__init =
R"doc(Creates a new link whose components are supplied by the given
sequences of points in 3-space.

Each element of the given sequence should represent a separate link
component. Each component should be given as a sequence of points in
3-space (any reasonable container type will do; see the requirements
for the *iterator* type below). These are the points that will be
stored directly in the Component structure, which means that to form
the actual geometry of the link component:

* each node in the sequence is joined by a straight line segment to
  the node that follows it (and likewise, the last node is joined to
  the first);

* the orientation of the link component follows the path in order from
  the first node to the last (and then cycling back to the front of
  the sequence again).

This constructor induces a deep copy of the given data.

Python:
    Instead of the iterators *begin* and *end*, this routine takes
    either (i) a Python list of lists of triples of real numbers, or
    (ii) a Python list of lists of SpatialLink::Node objects.

Template parameter ``iterator``:
    the iterator type used to access the full sequence of nodes in
    each link component. This must satisfy the following requirements:
    (i) when dereferenced, the resulting object (which represents a
    single link component) has appropriate ``begin()`` and ``end()``
    functions; and (ii) when _those_ iterators are dereferenced, the
    resulting object (which represents an individual point along some
    link component) is convertible to a SpatialLink::Node object.

Parameter ``begin``:
    the beginning of the sequence of link components.

Parameter ``end``:
    a past-the-end iterator indicating the end of the sequence of
    components.)doc";

// Docstring regina::python::doc::SpatialLink_::__ne
static const char *__ne =
R"doc(Determines if this link is not identical to the given link.

Here "identical" means that both links follow exactly the same paths
through 3-dimensional space, with their components and nodes stored in
exactly the same order.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating point comparisons.

Parameter ``other``:
    the link to compare with this.

Returns:
    ``True`` if and only if the two links are not identical.)doc";

// Docstring regina::python::doc::SpatialLink_::component
static const char *component =
R"doc(Returns a reference to the component at the given index within this
link.

Parameter ``index``:
    the index of the requested component. This must be between 0 and
    countComponents()-1 inclusive.

Returns:
    the component at the given index.)doc";

// Docstring regina::python::doc::SpatialLink_::componentSize
static const char *componentSize =
R"doc(Returns the number of nodes that are stored for the given component of
this link.

This is equivalent to calling ``component[componentIndex].size()``.

Parameter ``componentIndex``:
    indicates the link component to query; this must be between 0 and
    ``countComponents() - 1`` inclusive.

Returns:
    the number of nodes stored for the requested component.)doc";

// Docstring regina::python::doc::SpatialLink_::components
static const char *components =
R"doc(Returns an object that allows iteration through and random access to
all components of this link.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Each element of the list will be a constant
reference to some component; more precisely, iterating through this
list is equivalent to calling ``component(0)``, ``component(1)``, ...,
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
static const char *countComponents =
R"doc(Returns the number of components in this link.

Returns:
    the number of components.)doc";

// Docstring regina::python::doc::SpatialLink_::defaultRadius
static const char *defaultRadius =
R"doc(Returns a sensible radius to use when rendering the link.
Specifically, this is the radius to use for the balls and cylinders
used in the 3-D model.

Currently this routine makes a "barely educated" decision: it looks
only at the scale of the embedding, without studying the complexity of
the knot or the closeness of the strands. Specifically, it chooses
some fixed fraction of the minimum range amongst the *x*, *y* and *z*
dimensions.

Eventually this will be replaced with something intelligent that
factors in how far apart the strands are, and will (as a result)
guarantee that the renderings of no-adjacent strands will not collide.

Returns:
    a sensible radius to use for rendering.)doc";

// Docstring regina::python::doc::SpatialLink_::fromKnotPlot
static const char *fromKnotPlot =
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
static const char *global_swap =
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

// Docstring regina::python::doc::SpatialLink_::isEmpty
static const char *isEmpty =
R"doc(Determines whether this link is empty. An empty link is one with no
components at all.

Returns:
    ``True`` if and only if this link is empty.)doc";

// Docstring regina::python::doc::SpatialLink_::node
static const char *node =
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

// Docstring regina::python::doc::SpatialLink_::range
static const char *range =
R"doc(Returns the range of coordinates that this link occupies.

Specifically, this routine returns a pair ``(min, max)``, where *min*
contains the minimum *x*, *y* and *z* coordinates over all nodes, and
*max* contains the maximum *x*, *y* and *z* coordinates over all
nodes.

Returns:
    the range of coordinates. If this link contains no nodes at all
    then this routine will return ``((0,0,0), (0,0,0))``.)doc";

// Docstring regina::python::doc::SpatialLink_::refine
static const char *refine =
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
static const char *refine_2 =
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
static const char *reflect =
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
static const char *scale =
R"doc(Scales the entire link by the given factor.

Specifically, all coordinates of all nodes will be multiplied by
*factor*.

Parameter ``factor``:
    the scaling factor; this must not be zero.)doc";

// Docstring regina::python::doc::SpatialLink_::size
static const char *size =
R"doc(Returns the total number of nodes in this spatial link.

.. warning::
    This is not a constant time operation, since it sums the sizes of
    the individual components.

Returns:
    the total number of nodes.)doc";

// Docstring regina::python::doc::SpatialLink_::swap
static const char *swap =
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
static const char *translate =
R"doc(Translates the entire link by the given vector.

Specifically, the *x*, *y* and *z* coordinates of all nodes will be
incremented by ``vector.x``, ``vector.y`` and ``vector.z``
respectively.

Parameter ``vector``:
    holds the three constants that should be added to the *x*, *y* and
    *z* coordinates of every node.)doc";

}

namespace SpatialLink_::Node_ {

// Docstring regina::python::doc::SpatialLink_::Node_::__add
static const char *__add =
R"doc(Returns the sum of this and the given node.

Here we use vector arithmetic: the resulting point will be the fourth
vertex of the parallelogram whose other vertices are the this node,
the origin, and the given node.

Parameter ``rhs``:
    the node to add to this node.

Returns:
    the sum of this and the given node.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::__copy
static const char *__copy = R"doc(Creates a new copy of the given point.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::__default
static const char *__default = R"doc(Creates a new point whose coordinates are uninitialised.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::__eq
static const char *__eq =
R"doc(Determines if this and the given point have the same coordinates.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating point comparisons.

Parameter ``other``:
    the point to compare with this.

Returns:
    ``True`` if and only if the two points are equal.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::__iadd
static const char *__iadd =
R"doc(Adds the coordinates of the given node to this node.

Here we use vector arithmetic: the new value of this node will be the
fourth vertex of the parallelogram whose other vertices are the given
node, the origin, and the original value of this node.

Parameter ``rhs``:
    the node whose coordinates should be added to this node.

Returns:
    a reference to this node.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::__imul
static const char *__imul =
R"doc(Scales this node by the given factor. Specifically, all coordinates of
this node will be multiplied by *scale*.

Parameter ``scale``:
    the scaling factor to apply.

Returns:
    a reference to this node.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::__init
static const char *__init =
R"doc(Creates a new point with the given 3-dimensional coordinates.

Parameter ``x``:
    the first (x) coordinate.

Parameter ``y``:
    the second (y) coordinate.

Parameter ``z``:
    the third (z) coordinate.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::__init_2
static const char *__init_2 =
R"doc(Creates a new point with the given 3-dimensional coordinates.

Parameter ``coordinates``:
    array whose three elements are the \z x, *y* and *z* coordinate
    respectively.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::__mul
static const char *__mul =
R"doc(Returns a copy of this node rescaled by the given factor.
Specifically, the coordinates of the node that is returned will be the
coordinates of this node multiplied by *scale*.

Parameter ``scale``:
    the scaling factor to apply.

Returns:
    a rescaled copy of this node.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::__ne
static const char *__ne =
R"doc(Determines if this and the given point have different coordinates.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating point comparisons.

Parameter ``other``:
    the point to compare with this.

Returns:
    ``True`` if and only if the two points are different.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::distance
static const char *distance =
R"doc(Returns the distance between this and the given node.

Returns:
    the distance between this and the given node.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::length
static const char *length =
R"doc(Returns the distance from this node to the origin.

Returns:
    the distance from this node to the origin.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::midpoint
static const char *midpoint =
R"doc(Returns the midpoint between this and the given node.

Returns:
    the midpoint between this and the given node.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::x
static const char *x = R"doc(The first (x) coordinate of the point.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::y
static const char *y = R"doc(The second (y) coordinate of the point.)doc";

// Docstring regina::python::doc::SpatialLink_::Node_::z
static const char *z = R"doc(The third (z) coordinate of the point.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

