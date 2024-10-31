/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Crossing
static const char *Crossing =
R"doc(Represents a single crossing in a link diagram. The two strands of the
link that run over and under the crossing respectively can be accessed
through routines such as over(), under(), upper(), lower(), and
strand().

Each crossing has a sign, which is either positive (denoted by +1) or
negative (denoted by -1):

* In a positive crossing, the upper strand passes over the lower
  strand from left to right:

```
  -----\ /----->
        \
  -----/ \----->
```

* In a negative crossing, the upper strand passes over the lower
  strand from right to left:

```
  -----\ /----->
        /
  -----/ \----->
```

If a link has *n* crossings, then these are numbered 0,...,*n*-1. The
number assigned to this crossing can be accessed by calling index().
Note that crossings in a link may be reindexed when other crossings
are added or removed - if you wish to track a particular crossing
through such operations then you should use a pointer to the relevant
Crossing object instead.

Crossings do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Link to which they belong.)doc";

// Docstring regina::python::doc::Framing
static const char *Framing =
R"doc(Indicates one of the standard framings of a knot or link. Here a
_framing_ refers to a choice of normal vector field along the knot or
link. Equivalently, a framing refers to a choice of longitude on the
torus bounding each component of the link.)doc";

// Docstring regina::python::doc::Link
static const char *Link =
R"doc(Represents a combinatorial diagram of a directed knot or link in the
3-sphere.

This class Link is a "purely combinatorial" representation of a link,
as it represents the combinatorics of a 2-dimensional link diagram,
with no geometric information about the specific placement of strands
or crossings. This is as opposed to the SpatialLink class, which is
"purely geometric" (storing a specific embedding of the link in
3-dimensional space).

* For most purposes, you should use the Link class, which has a rich
  set of mathematical features and uses exact discrete algorithms.

* For visualisation, you may wish to use SpatialLink instead, with the
  caveat that SpatialLink is based on floating-point arithmetic and is
  therefore susceptible to floating-point errors.

This Link class supports links with any number of components
(including zero), and it also supports components with no crossings
(which form additional unknot components of the overall link).

Since Regina 7.0, this is no longer a "packet type" that can be
inserted directly into the packet tree. Instead a Link is now a
standalone mathematatical object, which makes it slimmer and faster
for ad-hoc use. The consequences of this are:

* If you create your own Link, it will not have any of the usual
  packet infrastructure. You cannot add it into the packet tree, and
  it will not support a label, tags, child/parent packets, and/or
  event listeners.

* To include a Link in the packet tree, you must create a new
  PacketOf<Link>. This _is_ a packet type, and supports labels, tags,
  child/parent packets, and event listeners. It derives from Link, and
  so inherits the full Link interface.

If you are adding new functions to this class that edit the internal
data structures of the link, you must remember to surround these
changes with a ChangeAndClearSpan. This manages bookkeeping such as
clearing computed properties, and (if this link _does_ belong to a
packet) firing packet change events.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::StrandRef
static const char *StrandRef =
R"doc(A reference to one of the two strands of a link that pass each other
at a crossing.

This class is a simple wrapper that stores (i) a pointer to the
relevant crossing; and (ii) an integer to denote whether this refers
to the upper strand (which passes over the crossing) or the lower
strand (which passes under the crossing).

A "null reference" is one whose crossing is the null pointer.

This class can also be used to refer to an _arc_ of a link; that is, a
section of the link that runs from one crossing to the next. When used
in this way:

* The arc referred to is the arc beginning at the given strand of the
  given crossing, and moving forwards along the orientation of the
  link to the next crossing.

* A null reference is used to refer to an entire zero-crossing unknot
  component (as opposed to a typical arc which has well-defined start
  and end points). Of course the link in question may not have such a
  component, or may have many such components; the behaviour of the
  code in such circumstances is documented in the individual routines
  that take arc references as arguments.

* The increment and decrement operators, as well as next() and prev(),
  behave as expected: they follow the link forward and backward
  respectively along its orientation.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

namespace Crossing_ {

// Docstring regina::python::doc::Crossing_::index
static const char *index =
R"doc(Returns the index of this crossing within the overall link. If the
link contains *n* crossings, then the index will be a number between 0
and *n*-1 inclusive.

.. warning::
    The index of this crossing might change if other crossings are
    added or removed.

Returns:
    the index of this crossing.)doc";

// Docstring regina::python::doc::Crossing_::lower
static const char *lower =
R"doc(Returns a reference to the strand running under this crossing. This is
equivalent to directly constructing StrandRef(``this``, 0).

Note that lower() and under() are synonyms.

Returns:
    a reference to the lower strand for this crossing.)doc";

// Docstring regina::python::doc::Crossing_::next
static const char *next =
R"doc(Returns the crossing reference that immediately follows this when
walking forward in the direction of the link along one of the two
strands that pass at this crossing. Which strand we follow is
indicated by the argument *strand*.

Note that for a crossing *c*, calling ``c.next(s)`` is equivalent to
calling ``c.strand(s).next()``.

Parameter ``strand``:
    either 1 to walk forward along the upper strand, or 0 to walk
    forward along the lower strand.

Returns:
    a reference to the next crossing after this along the given
    strand.)doc";

// Docstring regina::python::doc::Crossing_::over
static const char *over =
R"doc(Returns a reference to the strand running over this crossing. This is
equivalent to directly constructing StrandRef(``this``, 1).

Note that upper() and over() are synonyms.

Returns:
    a reference to the upper strand for this crossing.)doc";

// Docstring regina::python::doc::Crossing_::prev
static const char *prev =
R"doc(Returns the crossing reference that immediately precedes this when
walking backward against the direction of the link along one of the
two strands that pass at this crossing. Which strand we follow is
indicated by the argument *strand*.

Note that for a crossing *c*, calling ``c.prev(s)`` is equivalent to
calling ``c.strand(s).prev()``.

Parameter ``strand``:
    either 1 to walk backward along the upper strand, or 0 to walk
    backward along the lower strand.

Returns:
    a reference to the previous crossing before this along the given
    strand.)doc";

// Docstring regina::python::doc::Crossing_::sign
static const char *sign =
R"doc(Returns the sign of this crossing. This will be +1 for a positive
crossing, or -1 for a negative crossing.

See the Crossing class notes for diagrams of positive and negative
crossings

Returns:
    the sign of this crossing.)doc";

// Docstring regina::python::doc::Crossing_::strand
static const char *strand =
R"doc(Returns a reference to one of the two strands of the link that pass
each other at this crossing. This is equivalent to directly
constructing StrandRef(``this``, *which*).

Note that upper() and over() are synonyms for strand(1), and lower()
and under() are synonyms for strand(0).

Parameter ``which``:
    either 1 to indicate the upper strand, or 0 to indicate the lower
    strand.

Returns:
    a reference to the given strand at this crossing.)doc";

// Docstring regina::python::doc::Crossing_::under
static const char *under =
R"doc(Returns a reference to the strand running under this crossing. This is
equivalent to directly constructing StrandRef(``this``, 0).

Note that lower() and under() are synonyms.

Returns:
    a reference to the lower strand for this crossing.)doc";

// Docstring regina::python::doc::Crossing_::upper
static const char *upper =
R"doc(Returns a reference to the strand running over this crossing. This is
equivalent to directly constructing StrandRef(``this``, 1).

Note that upper() and over() are synonyms.

Returns:
    a reference to the upper strand for this crossing.)doc";

}

namespace Framing_ {

// Docstring regina::python::doc::Framing_::Blackboard
static const char *Blackboard =
R"doc(Indicates the _blackboard framing_, which is specific to the knot/link
projection.

For the blackboard framing, the normal vector field stays within the
projection plane. Equivalently, the blackboard framing chooses
longitudes whose projections do not intersect the original link
diagram.)doc";

// Docstring regina::python::doc::Framing_::Seifert
static const char *Seifert =
R"doc(Indicates the _Seifert framing_, which is defined algebraically and is
independent of the knot/link projection.

For each component of the link, draw a Seifert surface (i.e., an
orientable surface embedded in the 3-sphere that is bounded by the
corresponding knot). The Seifert framing is the vector field that
points into the corresponding surface.

Equivalently, for each component of the link, the Seifert framing
chooses the unique longitude for the corresponding knot that is
trivial in the homology of the knot complement.)doc";

}

namespace Link_ {

// Docstring regina::python::doc::Link_::__copy
static const char *__copy =
R"doc(Constructs a new copy of the given link.

This will clone any computed properties (such as Jones polynomial and
so on) of the given link also. If you want a "clean" copy that resets
all properties to unknown, you can use the two-argument copy
constructor instead.

Parameter ``copy``:
    the link to copy.)doc";

// Docstring regina::python::doc::Link_::__default
static const char *__default = R"doc(Constructs an empty link. This will have zero components.)doc";

// Docstring regina::python::doc::Link_::__eq
static const char *__eq =
R"doc(Determines if this link diagram is combinatorially identical to the
given link diagram.

Here "identical" means that:

* the link diagrams have the same number of crossings and the same
  number of components;

* the same numbered crossings are positive and negative in both
  diagrams;

* the same pairs of numbered crossings have their under/over-strands
  connected, with the same orientations;

* for each *i*, the starting strand for the *th* component is the same
  (under/over) strand of the same numbered crossing in both diagrams.

Parameter ``other``:
    the link diagram to compare with this.

Returns:
    ``True`` if and only if the two link diagrams are combinatorially
    identical.)doc";

// Docstring regina::python::doc::Link_::__init
static const char *__init =
R"doc(Constructs the unlink with the given number of components.

Parameter ``unknots``:
    the number of (unknotted) components in the new unlink.)doc";

// Docstring regina::python::doc::Link_::__init_2
static const char *__init_2 =
R"doc(Constructs a new copy of the given link, with the option of whether or
not to clone its computed properties also.

Parameter ``copy``:
    the link to copy.

Parameter ``cloneProps``:
    ``True`` if this should also clone any computed properties of the
    given link (such as Jones polynomial and so on), or ``False`` if
    the new link should have all properties marked as unknown.)doc";

// Docstring regina::python::doc::Link_::__init_3
static const char *__init_3 =
R"doc("Magic" constructor that tries to find some way to interpret the given
string as a link.

At present, Regina understands the following types of strings (and
attempts to parse them in the following order):

* knot/link signatures, as used by fromSig();

* oriented Gauss codes, as used by fromOrientedGauss();

* classical Gauss codes, as used by fromGauss();

* numeric or alphabetical Dowker-Thistlethwaite strings, as used by
  fromDT();

* planar diagram codes, as used by fromPD().

This list may grow in future versions of Regina.

Exception ``InvalidArgument``:
    Regina could not interpret the given string as representing a link
    using any of the supported string types.

Parameter ``description``:
    a string that describes a knot or link.)doc";

// Docstring regina::python::doc::Link_::bracket
static const char *bracket =
R"doc(Returns the Kauffman bracket polynomial of this link diagram.

Note that the bracket polynomial is not an invariant - it is preserved
under Reidemeister moves II and III, but not I.

If this is the empty link, then this routine will return the zero
polynomial.

Bear in mind that each time the link changes, all of its polynomials
will be deleted. Thus the reference that is returned from this routine
should not be kept for later use. Instead, bracket() should be called
again; this will be instantaneous if the bracket polynomial has
already been calculated.

If this polynomial has already been computed, then the result will be
cached and so this routine will be very fast (since it just returns
the previously computed result). Otherwise the computation could be
quite slow, particularly for larger numbers of crossings.

Since Regina 7.0, this routine will not return until the polynomial
computation is complete, regardless of whether a progress tracker was
passed. If you need the old behaviour (where passing a progress
tracker caused the computation to start in the background), simply
call this routine in a new detached thread.

.. warning::
    The naive algorithm can only handle a limited number of crossings
    (currently at most 63). If you pass Algorithm::Naive and you have
    too many crossings (which is not advised, since the naive
    algorithm requires 2^*n* time), then this routine will ignore your
    choice of algorithm and use the treewidth-based algorithm
    regardless.

Exception ``NotImplemented``:
    This link is *so* large that the maximum possible strand ID cannot
    fit into an ``int``. (On a typical machine where ``int`` is
    32-bit, this would require over a _billion_ crossings). Note that,
    if you have such a link, then this function (which is exponential
    time) would be intractably slow anyway.

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``alg``:
    the algorithm with which to compute the polynomial. If you are not
    sure, the default (Algorithm::Default) is a safe choice. If you
    wish to specify a particular algorithm, there are currently two
    choices: Algorithm::Naive is a slow algorithm that computes the
    Kauffman bracket by resolving all crossings in all possible ways,
    and Algorithm::Treewidth uses a fixed-parameter tractable
    treewidth-based algorithm.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    the bracket polynomial, or the zero polynomial if the calculation
    was cancelled via the given progress tracker.)doc";

// Docstring regina::python::doc::Link_::brief
static const char *brief =
R"doc(Outputs this link in Regina's own brief write-only format. This format
is concise, but contains enough information to manually reconstruct
the complete link diagram.

This format cannot (yet) be used to read links back into Regina, and
so it is not good for external storage, or for passing links between
different programs (or even different instances of Regina). It was
originally designed for use with the test suite, where it was used to
ensure that links with being created and/or manipulated correctly.

The output will contain the following elements, separated by single
spaces:

* a sequence of signs (``+`` or ``-``), concatenated together, giving
  the signs of the crossings in order from crossing 0 to crossing
  size()-1;

* a description of each component of the link, in order from component
  0 to component countComponents()-1. Each component will be written
  in the form ``( a b c ... )``, indicating the crossings that are
  encountered as we follow the component in the forward direction from
  its starting strand. Each element *a*, *b*, *c* and so on will be
  written in the format used by the StrandRef class: either ``^n``
  when passing over crossing *n*, or ``_n`` when passing under
  crossing *n*.

For example, the Whitehead link as returned by ExampleLink.whitehead()
will give the following brief output:

```
--++- ( ^0 _1 ^4 _3 ^2 _4 ) ( _0 ^1 _2 ^3 )
```

As a special case, if the link contains no crossings, then the output
will not begin with a space; instead it will simply be a sequence of
the form ``( ) ( ) ... ( )``.

The string will not end in a newline.

There is also a variant of brief() that writes directly to an output
stream.

Returns:
    a description of this link in Regina's brief format.)doc";

// Docstring regina::python::doc::Link_::change
static const char *change =
R"doc(Switches the upper and lower strands of the given crossing.

Parameter ``c``:
    the crossing to change.)doc";

// Docstring regina::python::doc::Link_::changeAll
static const char *changeAll =
R"doc(Switches the upper and lower strands of every crossing in the diagram.

This operation corresponds to reflecting the link diagram through the
plane on which it is drawn.)doc";

// Docstring regina::python::doc::Link_::complement
static const char *complement =
R"doc(Returns an ideal triangulation of the complement of this link in the
3-sphere.

The triangulation will have one ideal vertex for each link component.
Assuming you pass *simplify* as ``True`` (the default), there will
typically be no internal vertices; however, this is not guaranteed.

Initially, each tetrahedron will be oriented according to a right-hand
rule: the thumb of the right hand points from vertices 0 to 1, and the
fingers curl around to point from vertices 2 to 3. If you pass
*simplify* as ``True``, then Regina will attempt to simplify the
triangulation to as few tetrahedra as possible: this may relabel the
tetrahedra, though their orientations will be preserved.

This is the same triangulation that would be produced by passing this
link to the Triangulation<3> constructor.

Parameter ``simplify``:
    ``True`` if and only if the triangulation of the complement should
    be simplified to use as few tetrahedra as possible.

Returns:
    the complement of this link.)doc";

// Docstring regina::python::doc::Link_::component
static const char *component =
R"doc(Returns a strand in the given component of this link.

For each component of the link, this routine returns a "starting
strand". You can traverse the entire component by beginning at this
starting strand and repeatedly incrementing it through a routine such
as StrandRef::operator++ or StrandRef::next().

If a component has no crossings (which means it must be a separate
unknot component), then this routine will return a null reference
(i.e., StrandRef::crossing() will return ``None``).

Parameter ``index``:
    the index of the requested component. This must be between 0 and
    countComponents()-1 inclusive.

Returns:
    a "starting strand" for traversing the component at the given
    index, or a null reference if the requested component has no
    crossings.)doc";

// Docstring regina::python::doc::Link_::components
static const char *components =
R"doc(Returns an object that allows iteration through and random access to
all components of this link.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Each element of the list will be a starting
strand for some component; more precisely, iterating through this list
is equivalent to calling ``component(0)``, ``component(1)``, ...,
``component(countComponents()-1)`` in turn. As an example, your code
might look like:

```
for (const StrandRef& c : link.components()) { ... }
```

The object that is returned will remain up-to-date and valid for as
long as the link exists: even as components are added and/or removed,
it will always reflect the components that are currently in the link.
Nevertheless, it is recommended to treat this object as temporary
only, and to call components() again each time you need it.

Returns:
    access to the list of all components.)doc";

// Docstring regina::python::doc::Link_::componentsByStrand
static const char *componentsByStrand =
R"doc(Returns a sequence that maps strand IDs to link component numbers.

This sequence will have length ``2n``, where *n* is the number of
crossings in this link diagram. If *strand* is a non-null strand
reference, *map* is the sequence that is returned, and
``map[strand.id()] == c``, then this indicates that *strand* is part
of the link component defined by ``component(c)``.

Null strand references are not handled by this map: they have a
negative ID (which means calling ``map[strand.id()]`` is an error),
and they could refer to any 0-crossing unknot component (so the
specific component might not be uniquely determined).

The return type is deliberately not specified here. It is guaranteed
to be a container whose elements have type ``size_t``, with value
semantics, fast move construction and swap operations, an array index
operator, and random access iterators. It is _not_ guaranteed to have
a copy assignment operator (but it will support fast move assignment).
At present the specific implementation returns ``FixedArray<size_t>``,
though this is subject to change in future versions of Regina and so
end user code should always use ``auto``.

Python:
    This routine will return a Python list.

Returns:
    a sequence mapping strand IDs to component numbers.)doc";

// Docstring regina::python::doc::Link_::composeWith
static const char *composeWith =
R"doc(Forms the composition of this with the given link. This link will be
altered directly.

Specifically, the first component of the given link will be grafted
into the first component of this link, in a way that preserves
orientations and crossing signs. If the given link has any additional
components, then they will be copied into this link directly with no
modification.

This routine may be expanded in future versions of Regina to allow
more flexibility (in particular, to allow you to choose which
components of the two links to graft together, and/or at which strands
to graft them).

If either link is empty (i.e., contains no components at all), then
the result will simply be a clone of the other link (with no
composition operation performed).

It is allowed to pass this link as *other*.

Parameter ``other``:
    the link with which this should be composed.)doc";

// Docstring regina::python::doc::Link_::connected
static const char *connected =
R"doc(Determines whether the two given crossings are connected in the link
diagram, if we treat each crossing as a 4-way intersection.

This tests whether it is possible to travel between the two given
crossings by:

* following the link around its components, and/or;

* jumping between upper and lower strands at crossings.

In particular, two crossings may be connected in the diagram even if
they involve entirely different components of the link.

See isConnected() for further discussion on the connectivity of link
diagrams.

Note: for knots and empty links, this routine is constant time. For
multiple-component links, it is linear in the link size.

Parameter ``a``:
    the first of the two crossings to examine.

Parameter ``b``:
    the second of the two crossings to examine.

Returns:
    ``True`` if and only if the two given crossings are connected.)doc";

// Docstring regina::python::doc::Link_::countComponents
static const char *countComponents =
R"doc(Returns the number of components in this link.

Returns:
    the number of components.)doc";

// Docstring regina::python::doc::Link_::countTrivialComponents
static const char *countTrivialComponents =
R"doc(Returns the number of zero-crossing unknot components in this link.

Returns:
    the number of zero-crossing unknot components.)doc";

// Docstring regina::python::doc::Link_::crossing
static const char *crossing =
R"doc(Returns a pointer to the crossing at the given index within this link.

For a link with *n* crossings, the crossings are numbered from 0 to
*n*-1 inclusive.

.. warning::
    If some crossings are added or removed then the indices of other
    crossings might change. If you wish to track a particular crossing
    through such operations then you should use the pointer to the
    relevant Crossing object instead.

Parameter ``index``:
    the index of the requested crossing. This must be between 0 and
    size()-1 inclusive.

Returns:
    the crossing at the given index.)doc";

// Docstring regina::python::doc::Link_::crossings
static const char *crossings =
R"doc(Returns an object that allows iteration through and random access to
all crossings within this link.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Note that the elements of the list will be
pointers, so your code might look like:

```
for (Crossing* c : link.crossings()) { ... }
```

The object that is returned will remain up-to-date and valid for as
long as the link exists: even as crossings are added and/or removed,
it will always reflect the crossings that are currently in the link.
Nevertheless, it is recommended to treat this object as temporary
only, and to call crossings() again each time you need it.

Returns:
    access to the list of all crossings.)doc";

// Docstring regina::python::doc::Link_::diagramComponents
static const char *diagramComponents =
R"doc(Returns the connected components of this link diagram as individual
standalone links.

Here _connected components_ are not the same as _link components_. A
connected component means a portion of the link diagram that is
connected when we treat each crossing as a 4-way intersection. In
other words, one can travel around the connected component by
following the link around, and/or jumping between upper and lower
strands at crossings. A single connected component of the diagram may
contain multiple link components, and will always describe a sublink
for which isConnected() returns ``True``.

The connected components are a property of the diagram, not an
invariant of the link itself, since the locations of the crossings
matter. In particular:

* a diagram with multiple connected components _must_ describe a
  splittable link;

* a splittable link, however, could be represented by a diagram with
  multiple connected components or with just one connected component.

The connected components that are returned will be cloned from this
link (so even if this diagram is connected and there is just one
connected component, a deep copy will still take place). The total
number of crossings across all of the links that are returned will
equal size(), and the total number of _link_ components across all of
the links that are returned will equal countComponents().

If you simply wish to know whether this diagram is connected, you
should call isConnected() instead which is much more lightweight.

Returns:
    a list containing the individual connected components of this link
    diagram.)doc";

// Docstring regina::python::doc::Link_::dt
static const char *dt =
R"doc(Exports this knot in either numerical or alphabetical Dowker-
Thistlethwaite notation, returning a string.

Like classical Gauss codes, Dowker-Thistlethwaite notation essentially
describes the 4-valent graph of a knot but not the particular
embedding in the plane. It comes with two major restrictions:

* In general, it does not carry enough information to uniquely
  reconstruct a knot. For instance, both a knot and its reflection can
  be described by the same Dowker-Thistlethwaite notation; moreover,
  for composite knots, the same notation can describe inequivalent
  knots (even when allowing for reflections).

* Parsing Dowker-Thistlethwaite notation is complex, since it requires
  an embedding to be deduced using some variant of a planarity testing
  algorithm.

If you need a code that specifies the knot uniquely and/or that is
fast to parse, consider using the _oriented_ Gauss code instead, which
resolves both of these issues.

For an *n*-crossing knot, Regina supports two variants of Dowker-
Thistlethwaite notation:

* a _numerical_ variant (the default), which is a sequence of *n* even
  signed integers as described (amongst other places) in Section 2.2
  of C. C. Adams, "The knot book", W. H. Freeman & Co., 1994;

* an _alphabetical_ variant, which transforms the numerical notation
  into a sequence of letters by replacing positive integers
  (2,4,6,...) with lower-case letters (``a``,``b``,``c``,...), and
  replacing negative integers (-2,-4,-6,...) with upper-case letters
  (``A``,``B``,``C``,...). This alphabetical variant can only be used
  for knots with 26 crossings or fewer; for larger knots this routine
  will throw an exception if the alphabetical variant is requested.

As an example, you can describe the trefoil using numerical Dowker-
Thistlethwaite notation as:

```
4 6 2
```

In alphabetical Dowker-Thistlethwaite notation, this becomes:

```
bca
```

Currently Regina only supports Dowker-Thistlethwaite codes for knots,
not empty or multiple component links. If this link does not have
precisely one component, then this routine will throw an exception. It
is possible that in future versions of Regina, Dowker-Thistlethwaite
codes will be expanded to cover all possible link diagrams (hence the
choice of NotImplemented as the exception type).

For numerical Dowker-Thistlethwaite notation, this routine will format
the list of integers as a string. The integers will be separated by
single spaces, and there will be no newlines. For alphabetical Dowker-
Thistlethwaite notation, the string that is returned will not contain
any whitespace at all.

For the numerical variant, the routine dtData() returns this same data
in machine-readable format (as a C++ vector), instead of the human-
readable format used here (a string). There is also another variant of
dt() that can write either the numerical or the alphabetical variant
directly to an output stream.

Exception ``NotImplemented``:
    Either this link is empty or has multiple components, or *alpha*
    is true and it has more than 26 crossings.

Parameter ``alpha``:
    ``True`` to use alphabetical notation, or ``False`` (the default)
    to use numerical notation.

Returns:
    the Dowker-Thistlethwaite notation for this knot diagram.)doc";

// Docstring regina::python::doc::Link_::dtData
static const char *dtData =
R"doc(Exports this knot in numerical Dowker-Thistlethwaite notation,
returning a vector of integers.

See dt(bool) for a full description of Dowker-Thistlethwaite notation
as it is used in Regina, as well as its limitations.

Although Regina can work with both the numerical and alphabetical
variants of Dowker-Thistlethwaite notation, this dtData() routine
exports the numerical variant only. If you wish to export the
alphabetical variant, you can call ``dt(true)``.

This routine returns machine-readable data (as a C++ vector); in
contrast, calling ``dt()`` returns the same integer sequence in human-
readable format (as a string).

Exception ``NotImplemented``:
    This link is empty, or has multiple components, or has so many
    crossings that the Dowker-Thistlethwaite notation cannot be
    expressed using native C++ integers.

Returns:
    the numerical Dowker-Thistlethwaite notation in machine-readable
    form.)doc";

// Docstring regina::python::doc::Link_::dumpConstruction
static const char *dumpConstruction =
R"doc(Deprecated routine that returns C++ code to reconstruct this link.

.. deprecated::
    This is equivalent to calling ``source(Language::Cxx)``, for
    compatibility with older versions of Regina. In particular, it is
    _not_ equivalent to calling ``source()`` (which defaults to the
    programming language currently being used). See source() for
    further details.

Returns:
    the C++ code that was generated.)doc";

// Docstring regina::python::doc::Link_::fromDT
static const char *fromDT =
R"doc(Creates a new knot from either alphabetical or numerical Dowker-
Thistlethwaite notation, presented as a string.

Dowker-Thistlethwaite notation essentially describes the 4-valent
graph of a knot but not the particular embedding in the plane. As a
result, there can be topological ambiguities when a knot is
reconstructed from Dowker-Thistlethwaite notation; these are described
in the warnings below.

Dowker-Thistlethwaite notation comes in two forms: numerical and
alphabetical. For an *n*-crossing knot, the numerical form is a
sequence of *n* even signed integers, and the alphabetical form is a
sequence of *n* case-sensitive letters. As an example, you can
construct the trefoil using either of the following strings:

```
4 6 2
bca
```

See dt(bool) for a full description of Dowker-Thistlethwaite notation
as it is used in Regina, as well as its limitations.

There are two variants of this routine. This variant takes a single
string, which is either (i) the alphabetical notation, in which any
whitespace within the string will be ignored; or (ii) the numerical
notation, in which the integers are combined together and separated by
whitespace. The other variant of this routine is only for numerical
Dowker-Thistlethwaite notation, and it takes a sequence of integers
defined by a pair of iterators.

In this variant (the string variant), the string may contain
additional leading or trailing whitespace; moreover, for numerical
Dowker-Thistlethwaite notation, the exact form of the whitespace that
separates the integers does not matter.

.. warning::
    In general, Dowker-Thistlethwaite notation does not contain enough
    information to uniquely reconstruct a knot. For prime knots, both
    a knot and its reflection can be described by the same notation;
    for composite knots, the same notation can describe knots that are
    topologically inequivalent, even when allowing for reflection. If
    you need to reconstruct a knot uniquely, consider using the
    oriented Gauss code instead.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a knot
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given string was not a valid Dowker-Thistlethwaite code for a
    knot. As noted above, the checks performed here are not
    exhaustive.

Author:
    Much of the code for this routine is based on the Dowker-
    Thistlethwaite implementation in the SnapPea/SnapPy kernel.

Parameter ``str``:
    either the alphabetical or numerical Dowker-Thistlethwaite
    notation for a knot, as described above.

Returns:
    the reconstructed knot.)doc";

// Docstring regina::python::doc::Link_::fromDT_2
static const char *fromDT_2 =
R"doc(Creates a new knot from numerical Dowker-Thistlethwaite notation,
presented as an integer sequence.

See dt(bool) for a full description of Dowker-Thistlethwaite notation
as it is used in Regina, and see fromDT(const std::string&) for a
detailed discussion of how Regina reconstructs knots from such
notation.

This routine is a variant of fromDT(const std::string&) which, instead
of taking a human-readable string, takes a machine-readable sequence
of integers. This sequence is given by passing a pair of begin/end
iterators.

This variant of fromDT() can only work with _numerical_ Dowker-
Thistlethwaite notation. Regina does understand alphabetic Dowker-
Thistlethwaite notation, but for this you will need to use the string-
based variant of fromDT().

Precondition:
    *Iterator* is a random access iterator type, and dereferencing
    such an iterator produces a native C++ integer. (The specific
    native C++ integer type being used will be deduced from the type
    *Iterator*.)

.. warning::
    In general, Dowker-Thistlethwaite notation does not contain enough
    information to uniquely reconstruct a knot. For prime knots, both
    a knot and its reflection can be described by the same notation;
    for composite knots, the same notation can describe knots that are
    topologically inequivalent, even when allowing for reflection. If
    you need to reconstruct a knot uniquely, consider using the
    oriented Gauss code instead.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a knot
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given sequence was not a valid Dowker-Thistlethwaite code for
    a knot. As noted above, the checks performed here are not
    exhaustive.

Python:
    Instead of a pair of begin and past-the-end iterators, this
    routine takes a Python list of integers.

Author:
    Much of the code for this routine is based on the Dowker-
    Thistlethwaite implementation in the SnapPea/SnapPy kernel.

Parameter ``begin``:
    an iterator that points to the beginning of the sequence of
    integers for the Dowker-Thistlethwaite notation for a knot.

Parameter ``end``:
    an iterator that points past the end of the sequence of integers
    for the Dowker-Thistlethwaite notation for a knot.

Returns:
    the reconstructed knot.)doc";

// Docstring regina::python::doc::Link_::fromData
static const char *fromData =
R"doc(Creates a new link from information about its crossings and
components.

This routine is an analogue to the variant of fromData() that takes
C++ initialiser lists; however, here the input data may be constructed
at runtime (which makes it accessible to Python, amongst other
things).

For the purposes of this routine, we number the crossings 1, 2, ...,
*n*. The information that you must pass to this routine is the
following:

* The first iterator range (*beginSigns*, *endSigns*) encodes the
  signs of crossings 1, ..., *n* in order. Each iterator in this range
  must dereference to either +1 or -1.

* The second iterator range (*beginComponents*, *endComponents*)
  identifies the individual components of the link. Each iterator in
  this range must dereference to a container that has a size()
  function and supports range-based ``for`` loops (so standard C++
  container classes such as std::vector<int> and std::list<int> are
  fine).

* The container for each component must be filled with integers, which
  identify the crossings you visit in order when traversing the
  component. A positive entry *i* indicates that you pass over
  crossing *i*, and a negative entry -*i* indicates that you pass
  under crossing *i*.

* To encode a component with no crossings, you may use either an empty
  container or a container containing the single integer 0.

Be aware that, once the link has been constructed, the crossings 1,
..., *n* will have been reindexed as 0, ..., *n*-1 (since every Link
object numbers its crossings starting from 0).

As an example, Python users can construct the left-hand trefoil and
the Hopf link as follows:

```
trefoil = Link.fromData([ -1, -1, -1 ], [[ 1, -2, 3, -1, 2, -3 ]])
hopf = Link.fromData([ +1, +1 ], [[ 1, -2 ], [ -1, 2 ]])
```

.. warning::
    While this routine does some error checking on the input, it does
    _not_ test for planarity of the diagram. That is, if the input
    describes a link diagram that must be drawn on some higher-genus
    surface as opposed to the plane, this will not be detected. Of
    course such inputs are not allowed, and it is currently up to the
    user to enforce this.

Exception ``InvalidArgument``:
    A link could not be reconstructed from the given data.

Python:
    The signs should be passed as a single Python list of integers
    (not an iterator pair). Likewise, the components should be passed
    as a Python list of lists of integers (not an iterator pair). In
    the case of a knot (which has only one component), you are welcome
    to replace the list of lists ``[[...]]`` with a single list
    ``[...]``; however, be aware that a single empty list ``[ ]`` will
    be interpreted as an empty link (not a zero-crossing unknot).

Parameter ``beginSigns``:
    the beginning of the list of crossing signs.

Parameter ``endSigns``:
    a past-the-end iterator indicating the end of the list of crossing
    signs.

Parameter ``beginComponents``:
    the beginning of the list of containers describing each link
    component.

Parameter ``endComponents``:
    a past-the-end iterator indicating the end of the list of link
    components.

Returns:
    the reconstructed link.)doc";

// Docstring regina::python::doc::Link_::fromGauss
static const char *fromGauss =
R"doc(Creates a new knot from a classical Gauss code, presented as a string.

Classical Gauss codes essentially describe the 4-valent graph of a
knot but not the particular embedding in the plane. As a result, there
can be topological ambiguities when a knot is reconstructed from a
gauss code; these are described in the warnings below.

The Gauss code for an *n*-crossing knot is described by a sequence of
2*n* positive and negative integers. As an example, you can construct
the trefoil using the code:

```
1 -2 3 -1 2 -3
```

See gauss() for a full description of classical Gauss codes as they
are used in Regina, as well as their limitations.

Regina imposes the following restrictions when reconstructing a knot
from a classical Gauss code:

* This can only be done for knots (i.e., links with exactly one
  component).

* The crossings of the knot must be labelled 1, 2, ..., *n* (i.e.,
  they cannot be arbitrary natural numbers with "gaps", and the
  numbering cannot use a different starting point).

Be aware that, once the knot has been constructed, the crossings 1,
..., *n* will have been reindexed as 0, ..., *n*-1 (since every Link
object numbers its crossings starting from 0).

There are two variants of this routine. This variant takes a single
string, where the integers have been combined together and separated
by whitespace. The other variant takes a sequence of integers, defined
by a pair of iterators.

In this variant (the string variant), the exact form of the whitespace
does not matter, and additional whitespace at the beginning or end of
the string is allowed.

.. warning::
    In general, the classical Gauss code does not contain enough
    information to uniquely reconstruct a knot. For prime knots, both
    a knot and its reflection can be described by the same Gauss code;
    for composite knots, the same Gauss code can describe knots that
    are topologically inequivalent, even when allowing for reflection.
    If you need to reconstruct a knot uniquely, consider using the
    _oriented_ Gauss code instead.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a knot
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given string was not a valid classical Gauss code for a knot.
    As noted above, the checks performed here are not exhaustive.

Author:
    Adam Gowty

Parameter ``str``:
    a classical Gauss code for a knot, as described above.

Returns:
    the reconstructed knot.)doc";

// Docstring regina::python::doc::Link_::fromGauss_2
static const char *fromGauss_2 =
R"doc(Creates a new knot from a classical Gauss code, presented as an
integer sequence.

See gauss() for a full description of classical Gauss codes as they
are used in Regina, and see fromGauss(const std::string&) for a
detailed discussion of how Regina reconstructs knots from such codes.

This routine is a variant of fromGauss(const std::string&) which,
instead of taking a human-readable string, takes a machine-readable
sequence of integers. This sequence is given by passing a pair of
begin/end iterators.

Precondition:
    *Iterator* is a random access iterator type, and dereferencing
    such an iterator produces a native C++ integer. (The specific
    native C++ integer type being used will be deduced from the type
    *Iterator*.)

.. warning::
    In general, the classical Gauss code does not contain enough
    information to uniquely reconstruct a knot. For prime knots, both
    a knot and its reflection can be described by the same Gauss code;
    for composite knots, the same Gauss code can describe knots that
    are topologically inequivalent, even when allowing for reflection.
    If you need to reconstruct a knot uniquely, consider using the
    _oriented_ Gauss code instead.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a knot
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given sequence was not a valid classical Gauss code for a
    knot. As noted above, the checks performed here are not
    exhaustive.

Python:
    Instead of a pair of begin and past-the-end iterators, this
    routine takes a Python list of integers.

Author:
    Adam Gowty

Parameter ``begin``:
    an iterator that points to the beginning of the sequence of
    integers for a classical Gauss code.

Parameter ``end``:
    an iterator that points past the end of the sequence of integers
    for a classical Gauss code.

Returns:
    the reconstructed knot.)doc";

// Docstring regina::python::doc::Link_::fromJenkins
static const char *fromJenkins =
R"doc(Creates a new link from Bob Jenkins' format, presented as a string.

Jenkins' format overcomes the limitations of classical Gauss codes by
encoding all of the data needed to quickly and correctly reconstruct a
link diagram. It can work with links as well as knots.

In Jenkins' format, a link is described by a sequence of integers. As
an example, you could construct the Hopf link using the sequence:

```
2
2   0 1   1 -1
2   0 -1   1 1
0 1   1 1
```

See jenkins() for a full description of Jenkins' format (and in
particular, what these integers represent).

There are three variants of this routine. This variant takes a single
string, where the integers have been combined together and separated
by whitespace. The other variants take (i) a sequence of integers
defined by a pair of iterators, or (ii) an input stream from which the
integers will be read.

In this variant (the string variant), the exact form of the whitespace
does not matter, and additional whitespace at the beginning or end of
the string is allowed.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a link
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given string was not a valid encoding of a link in Jenkins'
    format. As noted above, the checks performed here are not
    exhaustive.

Parameter ``str``:
    a string describing a link in Jenkins' format, as described above.

Returns:
    the reconstructed link.)doc";

// Docstring regina::python::doc::Link_::fromJenkins_2
static const char *fromJenkins_2 =
R"doc(Creates a new link from Bob Jenkins' format, presented as an integer
sequence.

See jenkins() for a full description of Bob Jenkins' format as it is
used in Regina, and see fromJenkins(const std::string&) for a detailed
discussion of how Regina reconstructs links from this format.

This routine is a variant of fromJenkins(const std::string&) which,
instead of taking a human-readable string, takes a machine-readable
sequence of integers. This sequence is given by passing a pair of
begin/end iterators.

Precondition:
    *Iterator* is a forward iterator type, and dereferencing such an
    iterator produces a native C++ integer. (The specific native C++
    integer type being used will be deduced from the type *Iterator*.)

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a link
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given sequence was not a valid encoding of a link in Jenkins'
    format. As noted above, the checks performed here are not
    exhaustive.

Python:
    Instead of a pair of begin and past-the-end iterators, this
    routine takes a Python list of integers.

Parameter ``begin``:
    an iterator that points to the beginning of the sequence of
    integers that describes a link.

Parameter ``end``:
    an iterator that points past the end of the sequence of integers
    that describes a link.

Returns:
    the reconstructed link.)doc";

// Docstring regina::python::doc::Link_::fromKnotSig
static const char *fromKnotSig =
R"doc(Alias for fromSig(), to recover a link diagram from its knot/link
signature.

This alias fromKnotSig() has been kept to reflect the fact that, in
older versions of Regina, these signatures were only available for
single-component knots; moreover the old name "knot signatures" can
still be found in the literature. While this routine is not
deprecated, it is recommended to use fromSig() in new code.

See fromSig() for further details.

Exception ``InvalidArgument``:
    The given string was not a valid knot/link signature.

Parameter ``sig``:
    the signature of the link diagram to construct. Note that
    signatures are case-sensitive.

Returns:
    the reconstructed link diagram.)doc";

// Docstring regina::python::doc::Link_::fromOrientedGauss
static const char *fromOrientedGauss =
R"doc(Creates a new knot from an "oriented" variant of the Gauss code,
presented as string.

Oriented gauss codes overcome the limitations of classical Gauss codes
by encoding all of the data needed to quickly and correctly
reconstruct a knot diagram.

The oriented Gauss code for an *n*-crossing knot is described by a
sequence of 2*n* string tokens. As an example, you can construct the
left-hand trefoil using the code:

```
+>1 -<2 +>3 -<1 +>2 -<3
```

See orientedGauss() for a full description of oriented Gauss codes as
they are used in Regina (and in particular, what these tokens
represent).

Regina imposes the following restrictions when reconstructing a knot
from an oriented Gauss code:

* This can only be done for knots (i.e., links with exactly one
  component).

* The crossings of the knot must be labelled 1, 2, ..., *n* (i.e.,
  they cannot be arbitrary natural numbers with "gaps", and the
  numbering cannot use a different starting point).

Be aware that, once the knot has been constructed, the crossings 1,
..., *n* will have been reindexed as 0, ..., *n*-1 (since every Link
object numbers its crossings starting from 0).

There are two variants of this routine. This variant takes a single
string, where the tokens have been combined together and separated by
whitespace. The other variant takes a sequence of tokens, defined by a
pair of iterators.

In this variant (the string variant), the exact form of the whitespace
does not matter, and additional whitespace at the beginning or end of
the string is allowed.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a knot
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given string was not a valid oriented Gauss code for a knot.
    As noted above, the checks performed here are not exhaustive.

Parameter ``str``:
    an "oriented" Gauss code for a knot, as described above.

Returns:
    the reconstructed knot.)doc";

// Docstring regina::python::doc::Link_::fromOrientedGauss_2
static const char *fromOrientedGauss_2 =
R"doc(Creates a new knot from an "oriented" variant of the Gauss code,
presented as a sequence of string tokens.

See orientedGauss() for a full description of oriented Gauss codes as
they are used in Regina, and see fromOrientedGauss(const std::string&)
for a detailed discussion of how Regina reconstructs knots from such
codes.

This routine is a variant of fromOrientedGauss(const std::string&)
which, instead of taking a human-readable string, takes a machine-
readable sequence of string tokens. This sequence is given by passing
a pair of begin/end iterators.

The tokens in the input sequence should be the individual tokens of
the form ``+<k``, ``-<k``, ``+>k`` or ``->k`` that would normally be
joined with whitespace to form a complete oriented Gauss code. For
example, to describe the left-hand trefoil, the input sequence could
be a vector containing the six tokens:

```
{ "+>1", "-<2", "+>3", "-<1", "+>2", "-<3" }
```

Each individual token should _not_ contain any whitespace; otherwise
this routine may fail to parse the token(s) and could throw an
exception as a result.

Precondition:
    *Iterator* is a random access iterator type.

Precondition:
    Dereferencing such an iterator produces either a C-style string
    (which can be cast to ``const char*``) or a C++-style string
    (which can be cast to ``const std::string&``).

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a knot
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given sequence was not a valid oriented Gauss code for a knot.
    As noted above, the checks performed here are not exhaustive.

Python:
    Instead of a pair of begin and past-the-end iterators, this
    routine takes a Python list of strings.

Parameter ``begin``:
    an iterator that points to the beginning of the sequence of tokens
    for an "oriented" Gauss code.

Parameter ``end``:
    an iterator that points past the end of the sequence of tokens for
    an "oriented" Gauss code.

Returns:
    the reconstructed knot.)doc";

// Docstring regina::python::doc::Link_::fromPD
static const char *fromPD =
R"doc(Creates a new link from a planar diagram code, presented as a string.

Planar diagram codes overcome the limitations of classical Gauss codes
by encoding the local information at each crossing, though they do
introduce their own (less severe) ambiguities and computational
difficulties, as described in the warnings below. They can work with
links as well as knots, though they cannot encode zero-crossing unknot
components.

A planar diagram code for an *n*-crossing link is formed from a
sequence of *n* 4-tuples of integers. An example, you can construct
the right-handed trefoil using the sequence:

```
[[1, 5, 2, 4], [3, 1, 4, 6], [5, 3, 6, 2]]
```

See pd() for a full description of planar diagram codes (and in
particular, what these integers represent).

Regina imposes the following restrictions when reconstructing a link
from a planar diagram code:

* The integers used in the input sequence (which denote the 2*n*
  strands in the link diagram) must be in the range 1, 2, ..., 2*n*.
  That is, they cannot be arbitrary natural numbers with "gaps", and
  the numbering of strands cannot use a different starting point.

When Regina builds the resulting link, it numbers the crossings and
components (but not the strands). It will do this as follows:

* Each 4-tuple in the given sequence represents a single crossing.
  Regina will number the crossings 0, 1, ..., *n* in the same order as
  the corresponding 4-tuples appear in the sequence.

* The integers in the given sequence represent strands in the link
  diagram. The strand numbered 1 will become the starting point of
  component 0 in the final link. Of the strands not in that component,
  the lowest numbered strand remaining will become the starting point
  of component 1, and so on.

* In particular be aware that StrandRef::id() will in general have no
  relation to the strand numbers used in the planar diagram code.

There are two variants of this routine. This variant takes a single
string containing all 4*n* integers (see below for how this string may
be formatted). The other variant takes a sequence of 4-tuples of
integers, defined by a pair of iterators.

In this variant (the string variant), the integers may be separated by
any combination of the following:

* any whitespace;

* commas;

* open or close round brackets, square brackets and/or braces;

* the special symbols ``PD``, ``X``, ``Xp``, ``Xm`` and ``P``, which
  are used by other sources (such as the Knot Atlas), but which are
  ignored here.

Thus the following strings all describe the same sequence:

```
[[1, 5, 2, 4], [3, 1, 4, 6], [5, 3, 6, 2]]
PD[X[1, 5, 2, 4], X[3, 1, 4, 6], X[5, 3, 6, 2]]
1 5 2 4 3 1 4 6 5 3 6 2
```

The string may containin separators (as defined above) at the
beginning and/or the end; these will be ignored.

Note that some sources (again, such as the Knot Atlas) use the special
symbols ``Xp``, ``Xm`` and ``P`` to change the meaning of the tuples.
Regina does _not_ attribute any meaning to these symbols, and will
treat them as nothing more than separators.

.. warning::
    If the link contains an unknotted loop that sits completely above
    all other link components (in other words, a link components that
    consists entirely of over-crossings), then the orientation of this
    loop might not be reconstructed correctly. This is unavoidable:
    the planar diagram code simply does not contain this information.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a link
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given string was not a valid planar diagram code. As noted
    above, the checks performed here are not exhaustive.

Parameter ``str``:
    a planar diagram code for a link, as described above.

Returns:
    the reconstructed link.)doc";

// Docstring regina::python::doc::Link_::fromPD_2
static const char *fromPD_2 =
R"doc(Creates a new link from a planar diagram code, presented as a sequence
of 4-tuples.

See pd() for a full description of planar diagram codes as they are
used in Regina, and see fromPD(const std::string&) for a detailed
discussion of how Regina reconstructs links from such codes.

This routine is a variant of fromPD(const std::string&) which, instead
of taking a human-readable string, takes a machine-readable sequence
of 4-tuples of integers. This sequence is given by passing a pair of
begin/end iterators.

Precondition:
    *Iterator* is a random access iterator type.

Precondition:
    If *it* is such an iterator, then ``(*it)[0]``, ``(*it)[1]``,
    ``(*it)[2]`` and ``(*it)[3]`` will give the elements of the
    corresponding 4-tuple, which can then be treated as native C++
    integers. (The specific native C++ integer type being used will be
    deduced from the type *Iterator*.)

.. warning::
    If the link contains an unknotted loop that sits completely above
    all other link components (in other words, a link components that
    consists entire of over-crossings), then the orientation of this
    loop might not be reconstructed correctly. This is unavoidable:
    the planar diagram code simply does not contain this information.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does _not_ test for
    planarity of the diagram. That is, if the input describes a link
    diagram that must be drawn on some higher-genus surface as opposed
    to the plane, this will not be detected. Of course such inputs are
    not allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The given sequence was not a valid planar diagram code. As noted
    above, the checks performed here are not exhaustive.

Python:
    Instead of a pair of begin and past-the-end iterators, this
    routine takes a Python list. Each element of the list should be
    convertible to a tuple of integers. In particular, a list of
    Python lists is fine, and a list of Python tuples is fine also.

Parameter ``begin``:
    an iterator that points to the beginning of the sequence of
    4-tuples for a planar diagram code.

Parameter ``end``:
    an iterator that points past the end of the sequence of 4-tuples
    for a planar diagram code.

Returns:
    the reconstructed link.)doc";

// Docstring regina::python::doc::Link_::fromSig
static const char *fromSig =
R"doc(Recovers a link diagram from its knot/link signature. See sig() for
more information on these signatures.

Calling sig() followed by fromSig() is not guaranteed to produce an
_identical_ knot diagram to the original, but it is guaranteed to
produce one that is related by relabelling, rotating connected
components of the diagram, and optionally (according to the arguments
that were passed to sig()) reflection of the entire diagram and/or
reversal of individual link components.

Exception ``InvalidArgument``:
    The given string was not a valid knot/link signature.

Parameter ``sig``:
    the signature of the link diagram to construct. Note that
    signatures are case-sensitive.

Returns:
    the reconstructed link diagram.)doc";

// Docstring regina::python::doc::Link_::gauss
static const char *gauss =
R"doc(Returns a classical Gauss code for this knot, presented as a string.

Classical Gauss codes essentially describe the 4-valent graph of a
knot but not the particular embedding in the plane. These codes come
with two major restrictions:

* In general, they do not carry enough information to uniquely
  reconstruct a knot. For instance, both a knot and its reflection can
  be described by the same Gauss code; moreover, for composite knots,
  the Gauss code can describe inequivalent knots (even when allowing
  for reflections).

* Parsing a Gauss code is complex, since it requires an embedding to
  be deduced using some variant of a planarity testing algorithm.

If you need a code that specifies the knot uniquely and/or that is
fast to parse, consider using the _oriented_ Gauss code instead, which
resolves both of these issues.

A Gauss code for an *n*-crossing knot is described by a sequence of
2*n* positive and negative integers, representing strands that pass
over and under crossings respectively. The code is constructed as
follows:

* Label the crossings arbitrarily as 1, 2, ..., *n*.

* Start at some point on the knot and follow it around. Whenever you
  pass crossing *k*, write the integer ``k`` if you pass over the
  crossing, or ``-k`` if you pass under the crossing.

As an example, you can represent the trefoil using the code:

```
1 -2 3 -1 2 -3
```

Currently Regina only supports Gauss codes for knots, not empty or
multiple component links. If this link does not have precisely one
component, then this routine will throw an exception. It is possible
that in future versions of Regina, Gauss codes will be expanded to
cover all possible link diagrams (hence the choice of NotImplemented
as the exception type).

This routine formats the list of integers as a string. The integers
will be separated by single spaces, and there will be no newlines.

The routine gaussData() returns this same data in machine-readable
format (as a C++ vector), instead of the human-readable format used
here (a string). There is also another variant of gauss() that writes
directly to an output stream.

Exception ``NotImplemented``:
    This link is empty or has multiple components.

Returns:
    a classical Gauss code as described above.)doc";

// Docstring regina::python::doc::Link_::gaussData
static const char *gaussData =
R"doc(Returns a classical Gauss code for this knot, presented as a vector of
integers.

See gauss() for a full description of classical Gauss codes as they
are used in Regina, as well as their limitations.

This routine returns machine-readable data (as a C++ vector); in
contrast, gauss() returns the same data in human-readable format (as a
string).

Exception ``NotImplemented``:
    This link is empty, or has multiple components, or has so many
    crossings that the Gauss code cannot be expressed using native C++
    integers.

Returns:
    a classical Gauss code for this knot in machine-readable form.)doc";

// Docstring regina::python::doc::Link_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given links.

This global routine simply calls Link::swap(); it is provided so that
Link meets the C++ Swappable requirements.

See Link::swap() for more details.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both links which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``lhs``:
    the link whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the link whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::Link_::graph
static const char *graph =
R"doc(Returns the 4-valent planar graph that models this link.

Any zero-component unknot components of this link will be ignored.

The nodes of the resulting graph will be numbered in the same way as
the crossings of this link. For each node, arc 0 will represent the
outgoing lower strand of the corresponding crossing.

Calling graph() is identical to passing this link to the
ModelLinkGraph constructor.

Returns:
    the graph that models this link.)doc";

// Docstring regina::python::doc::Link_::group
static const char *group =
R"doc(Returns the group of this link; that is, the fundamental group of the
link exterior.

This routine builds the Wirtinger presentation, where all relations
are some variant of the form ``xy=yz``.

If you pass *simplify* as ``False``, it will leave the presentation in
exactly this form (i.e., the Wirtinger presentation), and not simplify
it further. If you pass *simplify* as ``True`` (the default), this
routine will attempt to simplify the group presentation before
returning.

.. note::
    If you are finding the resulting group presentation too large for
    your liking even after simplification, then you could also try
    calling complement() and computing the fundamental group of the
    resulting 3-manifold triangulation. Sometimes the presentation
    obtained via the complement is better, and sometimes it is worse.

Currently this group is _not_ cached; instead it is reconstructed
every time this function is called. This behaviour may change in
future versions of Regina.

Returns:
    the group of this link.)doc";

// Docstring regina::python::doc::Link_::hasReducingPass
static const char *hasReducingPass =
R"doc(Tests whether this link has a pass move that will reduce the number of
crossings.

A _pass_ move involves taking a section of some link component that
involves only over-crossings (or only under-crossings), and then
lifting that section above (or beneath respectively) the diagram and
placing it back again in a different location. In particular, this
routine searches for a different location that will involve fewer
crossings than the original location.

This routine does not actually _perform_ the pass move; it simply
determines whether one exists.

The running time is cubic in the number of crossings.

Returns:
    ``True`` if and only if there is a pass move that reduces the
    number of crossings.)doc";

// Docstring regina::python::doc::Link_::homfly
static const char *homfly =
R"doc(Returns the HOMFLY polynomial of this link, as a polynomial in *alpha*
and *z*.

This routine is simply an alias for homflyAZ(). See the documentation
for homflyAZ() for further details.

To pretty-print this polynomial for human consumption, you can call
``Laurent2::str(Link::homflyVarX, Link::homflyVarY)``.

Bear in mind that each time the link changes, all of its polynomials
will be deleted. Thus the reference that is returned from this routine
should not be kept for later use. Instead, homfly() should be called
again; this will be instantaneous if the HOMFLY polynomial has already
been calculated.

Exception ``NotImplemented``:
    This link is *so* large that the maximum possible strand ID cannot
    fit into an ``int``. (On a typical machine where ``int`` is
    32-bit, this would require over a _billion_ crossings). Note that,
    if you have such a link, then this function (which is exponential
    time) would be intractably slow anyway.

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``alg``:
    the algorithm with which to compute the polynomial. If you are not
    sure, the default (Algorithm::Default) is a safe choice. If you
    wish to specify a particular algorithm, there are currently two
    choices: Algorithm::Backtrack will use Kauffman's skein-template
    algorithm, and Algorithm::Treewidth will use a fixed-parameter
    tractable treewidth-based algorithm.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    the HOMFLY polynomial, or the zero polynomial if the calculation
    was cancelled via the given progress tracker.)doc";

// Docstring regina::python::doc::Link_::homflyAZ
static const char *homflyAZ =
R"doc(Returns the HOMFLY polynomial of this link, as a polynomial in *alpha*
and *z*.

This variant of the HOMFLY polynomial is described (amongst other
places) in G. Gouesbet et al., "Computer evaluation of Homfly
polynomials by using Gauss codes, with a skein-template algorithm",
Applied Mathematics and Computation 105 (1999), 271-289.

The (*alpha*, *z*) and (*l*, *m*) variants of the HOMFLY polynomial
are related by a simple transformation: *alpha* = *l* *i* and *z* =
-*m* *i*, where *i* represents (as usual) a square root of -1.

This routine returns a Laurent polynomial in the two variables *alpha*
and *z* (which are represented by *x* and *y* respectively in the
class Laurent2).

If this is the empty link, then this routine will return the zero
polynomial.

To pretty-print this polynomial for human consumption, you can call
``Laurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)``.

The default implementation uses Kauffman's skein-template algorithm;
see L. H. Kauffman, "State models for link polynomials",
L'enseignement mathematique 36 (1990), 1-37, or for a more recent
summary see G. Gouesbet et al., "Computer evaluation of Homfly
polynomials by using Gauss codes, with a skein-template algorithm",
Applied Mathematics and Computation 105 (1999), 271-289.

Bear in mind that each time the link changes, all of its polynomials
will be deleted. Thus the reference that is returned from this routine
should not be kept for later use. Instead, homflyAZ() should be called
again; this will be instantaneous if the HOMFLY polynomial has already
been calculated.

If the HOMFLY polynomial has already been computed (either in terms of
*alpha* and *z* or in terms of *l* and *m*), then the result will be
cached and so this routine will be very fast (since it just returns
the previously computed result). Otherwise the computation could be
quite slow, particularly for larger numbers of crossings.

Since Regina 7.0, this routine will not return until the polynomial
computation is complete, regardless of whether a progress tracker was
passed. If you need the old behaviour (where passing a progress
tracker caused the computation to start in the background), simply
call this routine in a new detached thread.

Exception ``NotImplemented``:
    This link is *so* large that the maximum possible strand ID cannot
    fit into an ``int``. (On a typical machine where ``int`` is
    32-bit, this would require over a _billion_ crossings). Note that,
    if you have such a link, then this function (which is exponential
    time) would be intractably slow anyway.

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``alg``:
    the algorithm with which to compute the polynomial. If you are not
    sure, the default (Algorithm::Default) is a safe choice. If you
    wish to specify a particular algorithm, there are currently two
    choices: Algorithm::Backtrack will use Kauffman's skein-template
    algorithm, and Algorithm::Treewidth will use a fixed-parameter
    tractable treewidth-based algorithm.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    the HOMFLY polynomial, or the zero polynomial if the calculation
    was cancelled via the given progress tracker.)doc";

// Docstring regina::python::doc::Link_::homflyAZtoLM
static const char *homflyAZtoLM =
R"doc(Converts between the (*alpha*, *z*) and (*l*, *m*) representations of
the HOMFLY polynomial.

The (*alpha*, *z*) and (*l*, *m*) variants are related by a simple
transformation: *alpha* = *l* *i* and *z* = -*m* *i*, where *i*
represents (as usual) a square root of -1.

See homflyAZ() and homflyLM() for further details.

Parameter ``homflyAZ``:
    the HOMFLY polynomial of a link as a polynomial in *alpha* and
    *z*, where (*alpha*, *z*) are represented by (*x*, *y*) in the
    class Laurent2<Integer>.

Returns:
    the HOMFLY polynomial of the same link as a polynomial in *l* and
    *m*, where (*l*, *m*) are represented by (*x*, *y*) in the class
    Laurent2<Integer>.)doc";

// Docstring regina::python::doc::Link_::homflyLM
static const char *homflyLM =
R"doc(Returns the HOMFLY polynomial of this link, as a polynomial in *l* and
*m*.

This variant of the HOMFLY polynomial is described (amongst other
places) in C. C. Adams, "The knot book", W. H. Freeman & Co., 1994.

The (*alpha*, *z*) and (*l*, *m*) variants of the HOMFLY polynomial
are related by a simple transformation: *alpha* = *l* *i* and *z* =
-*m* *i*, where *i* represents (as usual) a square root of -1.

This routine returns a Laurent polynomial in the two variables *l* and
*m* (which are represented by *x* and *y* respectively in the class
Laurent2).

If this is the empty link, then this routine will return the zero
polynomial.

To pretty-print this polynomial for human consumption, you can call
``Laurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)``.

The default implementation uses Kauffman's skein-template algorithm;
see L. H. Kauffman, "State models for link polynomials",
L'enseignement mathematique 36 (1990), 1-37, or for a more recent
summary see G. Gouesbet et al., "Computer evaluation of Homfly
polynomials by using Gauss codes, with a skein-template algorithm",
Applied Mathematics and Computation 105 (1999), 271-289.

Bear in mind that each time the link changes, all of its polynomials
will be deleted. Thus the reference that is returned from this routine
should not be kept for later use. Instead, homflyLM() should be called
again; this will be instantaneous if the HOMFLY polynomial has already
been calculated.

If the HOMFLY polynomial has already been computed (either in terms of
*alpha* and *z* or in terms of *l* and *m*), then the result will be
cached and so this routine will be very fast (since it just returns
the previously computed result). Otherwise the computation could be
quite slow, particularly for larger numbers of crossings.

Since Regina 7.0, this routine will not return until the polynomial
computation is complete, regardless of whether a progress tracker was
passed. If you need the old behaviour (where passing a progress
tracker caused the computation to start in the background), simply
call this routine in a new detached thread.

Exception ``NotImplemented``:
    This link is *so* large that the maximum possible strand ID cannot
    fit into an ``int``. (On a typical machine where ``int`` is
    32-bit, this would require over a _billion_ crossings). Note that,
    if you have such a link, then this function (which is exponential
    time) would be intractably slow anyway.

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``alg``:
    the algorithm with which to compute the polynomial. If you are not
    sure, the default (Algorithm::Default) is a safe choice. If you
    wish to specify a particular algorithm, there are currently two
    choices: Algorithm::Backtrack will use Kauffman's skein-template
    algorithm, and Algorithm::Treewidth will use a fixed-parameter
    tractable treewidth-based algorithm.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    the HOMFLY polynomial, or the zero polynomial if the calculation
    was cancelled via the given progress tracker.)doc";

// Docstring regina::python::doc::Link_::insertLink
static const char *insertLink =
R"doc(Inserts a copy of the given link into this link.

The crossings and components of *source* will be copied into this
link, and placed after any pre-existing crossings and components.
Specifically, if the original number of crossings in this link was
*N*, then crossing number *i* of *source* will be copied to a new
crosssing ``N+i`` of this link; likewise for components.

This routine behaves correctly when *source* is this link.

Parameter ``source``:
    the link whose copy will be inserted.)doc";

// Docstring regina::python::doc::Link_::insertTorusLink
static const char *insertTorusLink =
R"doc(Inserts a new (*p*, *q*) torus link into this link.

The parameters *p* and *q* must be non-negative, but they do not need
to be coprime.

All of the crossings in the new torus link component(s) will be
positive if the argument *positive* is ``True``, or negative
otherwise.

The new crossings and components will be inserted at the end of the
respective lists in this link.

If your aim is to create a new torus link (as opposed to inserting one
into an existing link), it is simpler to just call
ExampleLink::torus().

Parameter ``p``:
    the first parameter of the new torus link; this must be non-
    negative.

Parameter ``q``:
    the second parameter of the new torus link; this must also be non-
    negative.

Parameter ``positive``:
    ``True`` if the crossings in the new torus link should be
    positive, or ``False`` if they should be negative.)doc";

// Docstring regina::python::doc::Link_::intelligentSimplify
static const char *intelligentSimplify =
R"doc(Deprecated alias for simplify(), which attempts to simplify this link
diagram as intelligently as possible using fast and greedy heuristics.

.. deprecated::
    This routine has been renamed to simplify(). See simplify() for
    further details.

Returns:
    ``True`` if and only if the link diagram was successfully
    simplified.)doc";

// Docstring regina::python::doc::Link_::isAlternating
static const char *isAlternating =
R"doc(Returns whether this link diagram is alternating.

Note that this routine cannot tell whether the _link_ is alternating
(i.e., whether there _exists_ an alternating diagram). Instead, it
simply returns whether this specific diagram is alternating or not.

The empty diagram and any zero-crossing unknot components will be
considered alternating.

Returns:
    ``True`` if this is an alternating diagram, or ``False`` if this
    is a non-alternating diagram.)doc";

// Docstring regina::python::doc::Link_::isConnected
static const char *isConnected =
R"doc(Determines whether this link diagram is connected, if we treat each
crossing as a 4-way intersection.

This tests whether it is possible to travel from any part of the link
to any other part of the link by:

* following the link around its components, and/or;

* jumping between upper and lower strands at crossings.

In particular, the link diagram may be connected even if the link has
multiple components.

Connectivity is a property of the diagram, not an invariant of the
link itself, since the locations of the crossings matter. In
particular:

* a disconnected diagram _must_ describe a splittable link;

* a splittable link, however, could be represented by either a
  connected or disconnected link diagram.

This is almost, but not quite, equivalent to testing whether the
underlying 4-valent graph of the link diagram is connected.
Specifically, where ``link.isConnected()`` and
``link.graph().isConnected()`` differ is in cases where the link has
zero-crossing components (i.e., unknotted circles disjoint from the
rest of the diagram). Zero-crossing components are considered here in
``Link.isConnected()`` but _not_ in ``ModelLinkGraph.isConnected()``,
since such components cannot be represented by a 4-valent graph (and
so the ModelLinkGraph class ignores them completely).

For the purposes of this routine, an empty link is considered to be
connected.

Note: for knots and empty links, this routine is constant time. For
multiple-component links, it is linear in the link size.

See also diagramComponents(), which extracts the connected components
of a link diagram as individual Link objects.

Returns:
    ``True`` if and only if this link diagram is connected.)doc";

// Docstring regina::python::doc::Link_::isEmpty
static const char *isEmpty =
R"doc(Determines whether this link is empty. An empty link is one with no
components at all.

Returns:
    ``True`` if and only if this link is empty.)doc";

// Docstring regina::python::doc::Link_::jenkins
static const char *jenkins =
R"doc(Exports this link using Bob Jenkins' text format, returning a single
string.

Jenkins' format is lengthy. However, in contrast to classical Gauss
codes or Dowker-Thistlethwaite notation, there are no topological
ambiguities in the format, and reconstructing a link from Jenkins'
format is simple. Moreover, the format is suitable for links with any
number of components.

Jenkins' format is described in his HOMFLY polynomial software, which
is available online from http://burtleburtle.net/bob/knot/homfly.html.
The format consists of a sequence of integers separated by whitespace,
constructed as follows:

* Label the crossings arbitrarily as 0, 1, ..., *n*-1.

* Write the number of components in the link.

* Next, for each link component:

  * write the number of times you pass a crossing when traversing the
    component (i.e., the length of the component);

  * write two integers for each crossing that you pass in such a
    traversal: the crossing label, and then +1 if you pass over the
    crossing or -1 if you pass under the crossing.

* Finally, for each crossing:

  * write the crossing label;

  * write the sign of the crossing (either +1 or -1).

As an example, you could describe the left-hand trefoil using the
following sequence:

```
1
6   0 1   1 -1   2 1   0 -1   1 1   2 -1
0 -1   1 -1   2 -1
```

Another example is the Hopf link, which you could describe using the
following sequence:

```
2
2   0 1   1 -1
2   0 -1   1 1
0 1   1 1
```

The string that is returned will contain multiple lines, and will end
in a newline. The specific choice of whitespace (i.e., the
"formatting" of the sequence) may change in future versions of Regina.

The routine jenkinsData() returns this same data in machine-readable
format (as a C++ vector), instead of the human-readable format used
here (a string). There is also another variant of jenkins() that
writes directly to an output stream.

Returns:
    a description of this link using Jenkins' text format.)doc";

// Docstring regina::python::doc::Link_::jenkinsData
static const char *jenkinsData =
R"doc(Exports this link using Bob Jenkins' text format, returning a vector
of integers.

See jenkins() for a full description of Jenkins' format as it is used
in Regina.

This routine returns machine-readable data (as a C++ vector); in
contrast, jenkins() returns the same data in human-readable format (as
a string).

Exception ``NotImplemented``:
    This link has so many crossings and/or components that its
    description in Jenkins' format cannot be expressed using native
    C++ integers.

Returns:
    a description of this link using Jenkins' format in machine-
    readable form.)doc";

// Docstring regina::python::doc::Link_::jones
static const char *jones =
R"doc(Returns the Jones polynomial of this link, but with all exponents
doubled.

By "all exponents doubled", we are indicating that the Jones
polynomial is in fact a Laurent polynomial in the square root of *t*.
So, for example:

* The right-hand trefoil has Jones polynomial ``1/t + 1/t^3 - 1/t^4``,
  and so this routine returns the Laurent polynomial ``x^-2 + x^-6 -
  x^-8``.

* The Hopf link has Jones polynomial ``-1/sqrt(x) - 1/sqrt(x^5)``, and
  so this routine returns the Laurent polynomial ``-x^-1 - x^-5``.

If this is the empty link, then this routine will return the zero
polynomial.

Regina follows the conventions described in C. C. Adams, "The knot
book", W. H. Freeman & Co., 1994. If you wish to convert to the
conventions used by Khovanov as described in Dror Bar-Natan, "On
Khovanov's categorifiction of the Jones polynomial", Algebraic &
Geometric Topology 2 (2002), 337-370, you can simply take the
polynomial returned by this routine and replace the variable *x*
(which represents the square root of *t*) with the expression -*q*.

To pretty-print this polynomial for human consumption, you can call
``Laurent::str(Link::jonesVar)``.

Bear in mind that each time the link changes, all of its polynomials
will be deleted. Thus the reference that is returned from this routine
should not be kept for later use. Instead, jones() should be called
again; this will be instantaneous if the Jones polynomial has already
been calculated.

If this polynomial has already been computed, then the result will be
cached and so this routine will be very fast (since it just returns
the previously computed result). Otherwise the computation could be
quite slow, particularly for larger numbers of crossings.

Since Regina 7.0, this routine will not return until the polynomial
computation is complete, regardless of whether a progress tracker was
passed. If you need the old behaviour (where passing a progress
tracker caused the computation to start in the background), simply
call this routine in a new detached thread.

.. warning::
    The naive algorithm can only handle a limited number of crossings
    (currently at most 63). If you pass Algorithm::Naive and you have
    too many crossings (which is not advised, since the naive
    algorithm requires 2^*n* time), then this routine will ignore your
    choice of algorithm and use the treewidth-based algorithm
    regardless.

Exception ``NotImplemented``:
    This link is *so* large that the maximum possible strand ID cannot
    fit into an ``int``. (On a typical machine where ``int`` is
    32-bit, this would require over a _billion_ crossings). Note that,
    if you have such a link, then this function (which is exponential
    time) would be intractably slow anyway.

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``alg``:
    the algorithm with which to compute the polynomial. If you are not
    sure, the default (Algorithm::Default) is a safe choice. If you
    wish to specify a particular algorithm, there are currently two
    choices: Algorithm::Naive is a slow algorithm that computes the
    Kauffman bracket by resolving all crossings in all possible ways,
    and Algorithm::Treewidth uses a fixed-parameter tractable
    treewidth-based algorithm.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    the Jones polynomial, or the zero polynomial if the calculation
    was cancelled via the given progress tracker.)doc";

// Docstring regina::python::doc::Link_::knotSig
static const char *knotSig =
R"doc(Alias for sig(), which constructs the signature for this knot or link
diagram.

This alias knotSig() has been kept to reflect the fact that, in older
versions of Regina, these signatures were only available for single-
component knots; moreover the old name "knot signatures" can still be
found in the literature. While this routine is not deprecated, it is
recommended to use sig() in new code.

See sig() for further details.

Exception ``NotImplemented``:
    This link diagram has 64 or more link components.

Parameter ``allowReflection``:
    ``True`` if reflecting the entire link diagram should preserve the
    signature, or ``False`` if the signature should distinguish
    between a diagram and its reflection (unless of course there is a
    symmetry).

Parameter ``allowReversal``:
    ``True`` if reversing some or all link components should preserve
    the signature, or ``False`` if the signature should distinguish
    between different orientations (again, unless of course there are
    symmetries).

Returns:
    the signature for this link diagram.)doc";

// Docstring regina::python::doc::Link_::knowsBracket
static const char *knowsBracket =
R"doc(Is the Kauffman bracket polynomial of this link diagram already known?
See bracket() for further details.

If this property is already known, future calls to bracket() will be
very fast (simply returning the precalculated value).

Returns:
    ``True`` if and only if this property is already known.)doc";

// Docstring regina::python::doc::Link_::knowsHomfly
static const char *knowsHomfly =
R"doc(Is the HOMFLY polynomial of this link diagram already known? See
homflyAZ() and homflyLM() for further details.

If this property is already known, future calls to homfly(),
homflyAZ() and homflyLM() will all be very fast (simply returning the
precalculated values).

Returns:
    ``True`` if and only if this property is already known.)doc";

// Docstring regina::python::doc::Link_::knowsJones
static const char *knowsJones =
R"doc(Is the Jones polynomial of this link diagram already known? See
jones() for further details.

If this property is already known, future calls to jones() will be
very fast (simply returning the precalculated value).

Returns:
    ``True`` if and only if this property is already known.)doc";

// Docstring regina::python::doc::Link_::linking
static const char *linking =
R"doc(Returns the linking number of this link.

This is an invariant of the link, computed as half the sum of the
signs of all crossings that involve different link components.

The algorithm to compute linking number is linear time.

Returns:
    the linking number.)doc";

// Docstring regina::python::doc::Link_::makeAlternating
static const char *makeAlternating =
R"doc(Changes a subset of crossings to convert this into an alternating link
diagram. Here, "changing" a crossing means switching its upper and
lower strands (so this operation may change this into a topologically
different link).

The empty diagram and any zero-crossing unknot components will be
considered alternating.

For each connected piece of the link diagram (which may incorporate
several link components), there is a unique alternating diagram up to
reflection through the plane in which the diagram is drawn. The
reflection that is chosen will be the one that preserves the sign of
the lowest-index crossing in that piece of the diagram.

Returns:
    ``True`` if the link diagram was changed, or *false* if it was
    already alternating to begin with.)doc";

// Docstring regina::python::doc::Link_::moveContentsTo
static const char *moveContentsTo =
R"doc(Moves the contents of this link into the given destination link,
leaving this link empty but otherwise usable.

The crossings and components of this link will be moved directly into
*dest*, and placed after any pre-existing crossings and components.
Specifically, if the original number of crossings in *dest* was *N*,
then crossing number *i* of this link will become crosssing ``N+i`` of
*dest*; likewise for components.

This link will become empty as a result, but it will otherwise remain
a valid and usable Link object. Any strand references or crossing
pointers that referred to either this link or *dest* will remain valid
(and will all now refer to *dest*), though if they originally referred
to this link then they will now return different crossing indices and
strand IDs.

Calling ``link.moveContentsTo(dest)`` is similar to calling
``dest.insertLink(std::move(link))``; it is a little slower but it
comes with the benefit of leaving this link in a usable state.

Precondition:
    *dest* is not this link.

Parameter ``dest``:
    the link into which the contents of this link should be moved.)doc";

// Docstring regina::python::doc::Link_::niceTreeDecomposition
static const char *niceTreeDecomposition =
R"doc(Returns a nice tree decomposition of the planar 4-valent multigraph
formed by this link diagram. This can (for example) be used in
implementing algorithms that are fixed-parameter tractable in the
treewidth of this graph.

See TreeDecomposition for further details on tree decompositions, and
see TreeDecomposition::makeNice() for details on what it means to be a
_nice_ tree decomposition.

This routine is fast: it will use a greedy algorithm to find a tree
decomposition with (hopefully) small width, but with no guarantees
that the width of this tree decomposition is the smallest possible.

The tree decomposition will be cached, so that if this routine is
called a second time (and the underlying link has not been changed)
then the same tree decomposition will be returned immediately.

If you wish to supply your own tree decomposition (as opposed to
relying on the greedy heuristics that Regina implements), then you can
supply it by calling useTreeDecomposition().

Returns:
    a nice tree decomposition of this link diagram.)doc";

// Docstring regina::python::doc::Link_::orientedGauss
static const char *orientedGauss =
R"doc(Returns an oriented Gauss code for this knot, presented as a string.

The oriented Gauss code, based on a format used by Andreeva et al., is
an extension of the classical Gauss code with additional characters to
describe the orientation of the other strand passing by at each
crossing. This extra information removes both the topological
ambiguities and the complexity in the reconstruction procedure for
classical Gauss codes.

This "oriented" format is described at
http://www.javaview.de/services/knots/doc/description.html#gc, and it
works as follows:

* Label the crossings arbitrarily as 1, 2, ..., *n*.

* Start at some point on the knot and follow it around. At every
  crossing that you pass, write a token of the form ``+<k``, ``-<k``,
  ``+>k`` or ``->k``, where:

* the symbol ``+`` indicates that you are passing over the crossing
  labelled *k*, and the symbol ``-`` indicates that you are passing
  under the crossing labelled *k*;

* the symbol ``<`` indicates that the other strand of the crossing
  passes from right to left, and ``>`` indicates that the other strand
  passes from left to right;

* *k* is replaced with the integer crossing label.

As an example, you can represent the left-hand trefoil using the code:

```
+>1 -<2 +>3 -<1 +>2 -<3
```

Currently Regina only supports Gauss codes for knots, not empty or
multiple component links. If this link does not have precisely one
component, then this routine will throw an exception. It is possible
that in future versions of Regina, Gauss codes will be expanded to
cover all possible link diagrams (hence the choice of NotImplemented
as the exception type).

This routine joins the tokens together as a single string. The tokens
will be separated by single spaces, and there will be no newlines.

The routine orientedGaussData() returns this same data in machine-
readable format (as a C++ vector of string tokens), instead of the
human-readable format used here (a single long string). There is also
another variant of orientedGauss() that writes directly to an output
stream.

Exception ``NotImplemented``:
    This link is empty or has multiple components.

Returns:
    an oriented Gauss code as described above.)doc";

// Docstring regina::python::doc::Link_::orientedGaussData
static const char *orientedGaussData =
R"doc(Returns an oriented Gauss code for this knot, presented as a vector of
string tokens.

See orientedGauss() for a full description of oriented Gauss codes as
they are used in Regina, as well as their limitations.

For an *n*-crossing knot, the elements of the returned vector will be
the 2*n* individual tokens of the form ``+<k``, ``-<k``, ``+>k`` or
``->k`` that would normally be joined with whitespace to form a
complete oriented Gauss code. For example, for the left-hand trefoil,
the vector might contain the six tokens:

```
{ "+>1", "-<2", "+>3", "-<1", "+>2", "-<3" }
```

This routine returns machine-readable data (as a C++ vector); in
contrast, orientedGauss() returns the same data in human-readable
format (as a string).

Exception ``NotImplemented``:
    This link is empty or has multiple components.

Returns:
    an oriented Gauss code for this knot in machine-readable form.)doc";

// Docstring regina::python::doc::Link_::overForComponent
static const char *overForComponent =
R"doc(Locates an over-crossing within the same link component as the given
strand. The choice of _which_ over-crossing is returned will be
arbitrary (i.e., it might not be the _first_ over-crossing).

Parameter ``component``:
    a strand reference in this link, which may be a null reference
    (indicating a zero-crossing component).

Returns:
    an over-crossing in the same link component, or a null reference
    if the given link component contains only under-crossings (i.e.,
    it is a zero-crossing unknot placed beneath the rest of the
    diagram).)doc";

// Docstring regina::python::doc::Link_::pace
static const char *pace =
R"doc(Returns a text representation of the underlying planar 4-valent
multigraph, using the PACE text format. This text format is described
in detail at https://pacechallenge.wordpress.com/pace-2016/track-a-
treewidth/.

In summary, the PACE text representation will consist of several lines
of text:

* The first line will be of the form ``p tw <num_vertices>
  <num_edges>``. Note that, since the underlying graph comes from a
  link diagram, we will always have *num_edges* equal to twice
  *num_vertices*.

* Following this will be *num_edges* lines, one for each edge, each of
  the form ``<u> <v>``, indicating an edge from vertex number *u* to
  vertex number *v*. In this format, vertices are numbered
  1,2,...,*num_vertices*.

An example of this text format is as follows:

```
p tw 4 8
1 2
1 4
1 2
2 3
3 4
1 3
3 4
2 4
```

If you are writing this text representation to an output stream then
you should call writePACE() instead, which is more efficient.

Returns:
    the PACE text representation of the underlying 4-valent
    multigraph, as outlined above.

See also:
    https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/)doc";

// Docstring regina::python::doc::Link_::parallel
static const char *parallel =
R"doc(Returns *k* cables of this link, all parallel to each other using the
given framing.

This routine creates a new link by:

* treating each component of this link as a ribbon, using the given
  framing;

* creating *k* parallel copies of the original link, following each
  other side-by-side along these ribbons.

This link will not be modified.

Parameter ``k``:
    the number of parallel copies to create. This must be non-
    negative.

Parameter ``framing``:
    the framing under which these copies will be parallel.

Returns:
    *k* parallel copies of this link.)doc";

// Docstring regina::python::doc::Link_::pd
static const char *pd =
R"doc(Returns a planar diagram code for this link, presented as a string.

Planar diagram codes encode the local information at each crossing,
and present this information as a list of 4-tuples. These codes are
available for links as well as knots, but they do come with some minor
restrictions:

* They cannot encode zero-crossing unknot components (i.e., components
  for which the component() function returns a null strand). Any such
  components will simply be omitted from the code. You can detect such
  components by calling countTrivialComponents().

* If a link has any components that consist entirely of over-crossings
  (which must be unknots "placed on top of" the link diagram), a
  planar diagram code does not carry enough data to reconstruct the
  _orientation_ of these components. The topology will be preserved,
  but in general the combinatorics of such a link diagram cannot be
  reconstructed faithfully. You can detect such components by calling
  pdAmbiguous().

If you need a text code that can work with these types of link
diagrams, you can always use Jenkins' format instead.

Regina adheres to a tight specification for the planar diagram codes
that it outputs, in order to ensure compatibility with other software.
In particular, Regina's codes are compatible with the Knot Atlas, as
seen at http://katlas.org/wiki/Planar_Diagrams.

In detail: a planar diagram code for an *n*-crossing link is formed
from a sequence of *n* 4-tuples of integers. Regina constructs this
sequence as follows:

* Throw away any zero-crossing unknot components.

* Let *n* denote the number of crossings.

* Number the strands from 1 to 2*n* in order as we walk along each
  component, in order from the first component to the last.

* For each crossing *c*, construct a 4-tuple that lists the four
  strands that meet at that *c*, in counter-clockwise order, beginning
  from the incoming lower strand.

* Return the resulting list of *n* 4-tuples.

An example, you can represent the right-hand trefoil using the code:

```
[[1, 5, 2, 4], [3, 1, 4, 6], [5, 3, 6, 2]]
```

Some points to be aware of:

* When building the list of 4-tuples, Regina orders the crossings as
  follows: again we walk along each component, in order from the first
  component to the last, and process each crossing when we enter it at
  the lower strand.

* When building each individual 4-tuple, some sources order the
  strands clockwise instead of counter-clockwise. Regina follows the
  same counter-clockwise convention that is used by the Knot Atlas and
  SnapPy.

This routine formats the list of 4-tuples as a string, in a way that
is consistent with the description in the Knot Atlas at
http://katlas.org/wiki/Planar_Diagrams.

In particular, each 4-tuple will be formatted with square brackets,
commas, and the prefix ``X``, and the main list will be formatted with
square brackets, commas, and the prefix ``PD``. An example (for the
right-handed trefoil) is:

```
PD[X[1, 5, 2, 4], X[3, 1, 4, 6], X[5, 3, 6, 2]]
```

The routine pdData() returns this same data in machine-readable format
(as a C++ vector of 4-tuples of integers), instead of the human-
readable format used here (a single string). There is also another
variant of pd() that writes directly to an output stream.

Returns:
    the planar diagram code, as described above.)doc";

// Docstring regina::python::doc::Link_::pdAmbiguous
static const char *pdAmbiguous =
R"doc(Determines whether this link has any components whose orientations
cannot be recovered from a planar diagram code.

Such components must have at least one crossing, and must consist
_entirely_ of over-crossings. These are essentially unknotted loops
that are "placed on top of" the remainder of the link diagram.

Note that planar diagrams have another limitation, which is that they
cannot represent zero-crossing components at all (any such components
are omitted from planar diagram codes entirely). Zero-crossing
components are _not_ recognised by this routine, but can be recognised
instead by calling countTrivialComponents().

Returns:
    ``True`` if and only if some component of this link has at least
    one crossing and consists entirely of over-crossings.)doc";

// Docstring regina::python::doc::Link_::pdData
static const char *pdData =
R"doc(Returns a planar diagram code for this link, presented as vector of
4-tuples.

See pd() for a full description of planar diagram codes as they are
used in Regina, as well as their limitations.

This routine returns machine-readable data (as a C++ vector); in
contrast, pd() returns the same data in human-readable format (as a
string).

Exception ``NotImplemented``:
    This link has so many crossings that the planar diagram code
    cannot be expressed using native C++ integers.

Returns:
    the planar diagram code in machine-readable form.)doc";

// Docstring regina::python::doc::Link_::r1
static const char *r1 =
R"doc(Tests for and/or performs a type I Reidemeister move to remove a
crossing.

There are two boolean arguments that control the behaviour of this
routine: *check* and *perform*.

* If *check* and *perform* are both ``True`` (the default), then this
  routine will first check whether this move can be performed at the
  given location. If so, it will perform the move and return ``True``.
  If not, it will do nothing and return ``False``.

* If *check* is ``True`` but *perform* is ``False``, then this routine
  will simply check whether this move can be performed at the given
  location and return ``True`` or ``False`` accordingly.

* If *check* is ``False`` but *perform* is ``True``, then this routine
  will perform the move without any prior checks, and will always
  return ``True``. In this case, it must be known in advance that the
  move can be performed at the given location.

* If *check* and *perform* are both ``False``, then this routine does
  nothing and just returns ``True``. (There is no reason to use this
  combination of arguments.)

The location of this move is specified by the argument *crossing*,
which indicates the crossing that will be removed. Specifically, this
move involves undoing a trivial twist at the given crossing.

You may pass a null pointer for *crossing*. However, in this case the
move cannot be performed, which means (i) *check* must be ``True``,
and therefore (ii) this routine will do nothing and return ``False``.

.. warning::
    A side-effect of this move is that, because one crossing is being
    removed, the other crossings in the link may be reindexed.
    However, no crossings other than the one involved in this move
    will be destroyed.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    The given crossing is either a null pointer, or else some crossing
    in this link.

Parameter ``crossing``:
    identifies the crossing to be removed.

Parameter ``check``:
    ``True`` if we are to check whether the move can be performed at
    the given location.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the move can be performed. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Link_::r1_2
static const char *r1_2 =
R"doc(Tests for and/or performs a type I Reidemeister move to add a new
crossing.

There are two boolean arguments that control the behaviour of this
routine: *check* and *perform*.

* If *check* and *perform* are both ``True`` (the default), then this
  routine will first check whether this move can be performed at the
  given location. If so, it will perform the move and return ``True``.
  If not, it will do nothing and return ``False``.

* If *check* is ``True`` but *perform* is ``False``, then this routine
  will simply check whether this move can be performed at the given
  location and return ``True`` or ``False`` accordingly.

* If *check* is ``False`` but *perform* is ``True``, then this routine
  will perform the move without any prior checks, and will always
  return ``True``. In this case, it must be known in advance that the
  move can be performed at the given location.

* If *check* and *perform* are both ``False``, then this routine does
  nothing and just returns ``True``. (There is no reason to use this
  combination of arguments.)

The location of this move is specified by the argument *arc*.
Specifically, this move involves adding a trivial twist to the given
arc; the arguments *side* and *sign* indicate on which side of the arc
and with which orientation the new twist will be made. See the
StrandRef documentation for the convention on how arcs are represented
using StrandRef objects.

If *arc* is a null reference, then the new twist will be added to a
zero-crossing unknot component; it will be assumed that this unknot
component is oriented clockwise. If *arc* is null but there is no
zero-crossing component then the move cannot be performed, and if
*arc* is null but there are multiple zero-crossing components then the
first such component will be used.

This move is almost always able to be performed: the only situation in
which it _cannot_ be performed is if *arc* is a null reference but
this link contains no zero-crossing components, as discussed above.

The existing crossings in this link will keep the same indices, and
the new crossing will be given the next index that is available.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    The given strand reference is either a null reference, or else
    refers to some strand of some crossing in this link.

Parameter ``arc``:
    identifies the arc of the link in which the new twist will be
    introduced, as described above.

Parameter ``side``:
    0 if the twist should be introduced on the left of the arc (when
    walking along the arc in the forward direction), or 1 if the twist
    should be introduced on the right of the arc.

Parameter ``sign``:
    the sign of the new crossing that will be introduced as part of
    the twist; this must be +1 or -1.

Parameter ``check``:
    ``True`` if we are to check whether the move can be performed at
    the given location.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the move can be performed. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Link_::r2
static const char *r2 =
R"doc(Tests for and/or performs a type II Reidemeister move to remove two
crossings.

There are two variants of this routine: one that takes an arc, and one
that takes a crossing. This variant, which takes an arc, is more
flexible (since either of the two arcs involved in this move can be
passed). The other variant, which takes a crossing, offers a canonical
way of performing the move (since for each move there is exactly one
crossing that describes it).

There are two boolean arguments that control the behaviour of this
routine: *check* and *perform*.

* If *check* and *perform* are both ``True`` (the default), then this
  routine will first check whether this move can be performed at the
  given location. If so, it will perform the move and return ``True``.
  If not, it will do nothing and return ``False``.

* If *check* is ``True`` but *perform* is ``False``, then this routine
  will simply check whether this move can be performed at the given
  location and return ``True`` or ``False`` accordingly.

* If *check* is ``False`` but *perform* is ``True``, then this routine
  will perform the move without any prior checks, and will always
  return ``True``. In this case, it must be known in advance that the
  move can be performed at the given location.

* If *check* and *perform* are both ``False``, then this routine does
  nothing and just returns ``True``. (There is no reason to use this
  combination of arguments.)

The location of this move is specified by the argument *arc*.
Specifically, this move involves pulling apart two arcs of the link
that surround a bigon; the given arc must be one of these two arcs.
See the StrandRef documentation for the convention on how arcs are
represented using StrandRef objects.

You may pass a null reference for *arc*. However, in this case the
move cannot be performed, which means (i) *check* must be ``True``,
and therefore (ii) this routine will do nothing and return ``False``.

.. warning::
    A side-effect of this move is that, because two crossings are
    being removed, the other crossings in the link may be reindexed.
    However, no crossings other than the two involved in this move
    will be destroyed.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    The given strand reference is either a null reference, or else
    refers to some strand of some crossing in this link.

Parameter ``arc``:
    identifies one of the arcs of the bigon about which the move will
    be performed, as described above.

Parameter ``check``:
    ``True`` if we are to check whether the move is legal.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the requested move is legal. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Link_::r2_2
static const char *r2_2 =
R"doc(Tests for and/or performs a type II Reidemeister move to remove two
crossings.

There are two variants of this routine: one that takes an arc, and one
that takes a crossing. The other variant, which takes an arc, is more
flexible (since either of the two arcs involved in this move can be
passed). This variant, which takes a crossing, offers a canonical way
of performing the move (since for each move there is exactly one
crossing that describes it).

There are two boolean arguments that control the behaviour of this
routine: *check* and *perform*.

* If *check* and *perform* are both ``True`` (the default), then this
  routine will first check whether this move can be performed at the
  given location. If so, it will perform the move and return ``True``.
  If not, it will do nothing and return ``False``.

* If *check* is ``True`` but *perform* is ``False``, then this routine
  will simply check whether this move can be performed at the given
  location and return ``True`` or ``False`` accordingly.

* If *check* is ``False`` but *perform* is ``True``, then this routine
  will perform the move without any prior checks, and will always
  return ``True``. In this case, it must be known in advance that the
  move can be performed at the given location.

* If *check* and *perform* are both ``False``, then this routine does
  nothing and just returns ``True``. (There is no reason to use this
  combination of arguments.)

The location of this move is specified by the argument *crossing*,
Specifically, this move involves pulling apart two arcs of the link
(one upper, one lower) that both run between the same pair of
crossings. The given crossing should be the start point of the upper
arc; that is, when following the upper arc forwards, *crossing* should
be the first of the two crossings that we encounter. Note that
*crossing* is one of the two crossings that will be removed by this
move.

You may pass a null pointer for *crossing*. However, in this case the
move cannot be performed, which means (i) *check* must be ``True``,
and therefore (ii) this routine will do nothing and return ``False``.

.. warning::
    A side-effect of this move is that, because two crossings are
    being removed, the other crossings in the link may be reindexed.
    However, no crossings other than the two involved in this move
    will be destroyed.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    The given crossing is either a null pointer, or else some crossing
    in this link.

Parameter ``crossing``:
    identifies the crossing at the beginning of the "upper" arc that
    features in this move, as described above.

Parameter ``check``:
    ``True`` if we are to check whether the move is legal.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the requested move is legal. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Link_::r2_3
static const char *r2_3 =
R"doc(Tests for and/or performs a type II Reidemeister move to add two new
crossings.

There are two boolean arguments that control the behaviour of this
routine: *check* and *perform*.

* If *check* and *perform* are both ``True`` (the default), then this
  routine will first check whether this move can be performed at the
  given location. If so, it will perform the move and return ``True``.
  If not, it will do nothing and return ``False``.

* If *check* is ``True`` but *perform* is ``False``, then this routine
  will simply check whether this move can be performed at the given
  location and return ``True`` or ``False`` accordingly.

* If *check* is ``False`` but *perform* is ``True``, then this routine
  will perform the move without any prior checks, and will always
  return ``True``. In this case, it must be known in advance that the
  move can be performed at the given location.

* If *check* and *perform* are both ``False``, then this routine does
  nothing and just returns ``True``. (There is no reason to use this
  combination of arguments.)

The location of this move is specified by the arguments *upperArc*,
*upperSide*, *lowerArc* and *lowerSide*. Specifically, this move
involves taking the arc *upperArc* and pushing it over *lowerArc* so
that the two arcs overlap. The arguments *upperSide* and *lowerSide*
indicate on which side of each arc the overlap takes place. See the
StrandRef documentation for the convention on how arcs are represented
using StrandRef objects.

If either *upperArc* or *lowerArc* is a null reference, then the move
will be performed upon a zero-crossing unknot component; it will be
assumed that this unknot component is oriented clockwise. If one of
these arguments is a null reference but there is no zero-crossing
component then the move cannot be performed, and if there are multiple
zero-crossing components then the first such component will be used.

Likewise, if _both_ arcs are null references, then the move will be
performed upon two _different_ zero-crossing unknot components. In
this case, if there are fewer than two such components then the move
cannot be performed, and otherwise *upperArc* will be the first such
component and *lowerArc* will be the second.

Currently, Regina cannot perform the move when *upperArc* and
*lowerArc* represent the same arc (or the same zero-crossing unknot
component). In this case there is a workaround: you can achieve the
same effect by performing two type I Reidemeister moves (i.e., by
adding two twists).

The existing crossings in this link will keep the same indices, and
the two new crossings will be given the next two indices that are
available.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    Each of the given strand references is either a null reference, or
    else refers to some strand of some crossing in this link.

.. warning::
    The check for this move is expensive (linear time), since it
    includes testing whether both sides-of-arcs belong to the same
    2-cell of the knot diagram.

Parameter ``upperArc``:
    identifies the arc of the link which will be passed over the
    other, as described above.

Parameter ``upperSide``:
    0 if the new overlap should take place on the left of *upperArc*
    (when walking along *upperArc* in the forward direction), or 1 if
    the new overlap should take place on the right of *upperArc*.

Parameter ``lowerArc``:
    identifies the arc of the link which will be passed beneath the
    other, as described above.

Parameter ``lowerSide``:
    0 if the new overlap should take place on the left of *lowerArc*
    (when walking along *lowerArc* in the forward direction), or 1 if
    the new overlap should take place on the right of *lowerArc*.

Parameter ``check``:
    ``True`` if we are to check whether the move can be performed at
    the given location.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the move can be performed. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Link_::r3
static const char *r3 =
R"doc(Tests for and/or performs a type III Reidemeister move.

There are two variants of this routine: one that takes an arc, and one
that takes a crossing. This variant, which takes an arc, is more
flexible (since any of the three arcs involved in this move can be
passed). The other variant, which takes a crossing, offers a canonical
way of performing the move (since for each move there is exactly one
crossing that describes it).

There are two boolean arguments that control the behaviour of this
routine: *check* and *perform*.

* If *check* and *perform* are both ``True`` (the default), then this
  routine will first check whether this move can be performed at the
  given location. If so, it will perform the move and return ``True``.
  If not, it will do nothing and return ``False``.

* If *check* is ``True`` but *perform* is ``False``, then this routine
  will simply check whether this move can be performed at the given
  location and return ``True`` or ``False`` accordingly.

* If *check* is ``False`` but *perform* is ``True``, then this routine
  will perform the move without any prior checks, and will always
  return ``True``. In this case, it must be known in advance that the
  move can be performed at the given location.

* If *check* and *perform* are both ``False``, then this routine does
  nothing and just returns ``True``. (There is no reason to use this
  combination of arguments.)

The location of this move is specified by the arguments *arc* and
*side*. Specifically, this move takes place around a triangle; the
given arc must form one of the three edges of this triangle. The
argument *side* indicates on which side of the arc the third crossing
is located. See the StrandRef documentation for the convention on how
arcs are represented using StrandRef objects.

You may pass a null reference for *arc*. However, in this case the
move cannot be performed, which means (i) *check* must be ``True``,
and therefore (ii) this routine will do nothing and return ``False``.

All crossings in this link will keep the same indices, and no
crossings will be created or destroyed. Instead, the three crossings
involved in this move will simply be reordered along the various
segments of the link.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    The given strand reference is either a null reference, or else
    refers to some strand of some crossing in this link.

Parameter ``arc``:
    identifies one of the arcs of the triangle about which the move
    will be performed, as described above.

Parameter ``side``:
    0 if the third crossing of the triangle is located to the left of
    the arc (when walking along the arc in the forward direction), or
    1 if the third crossing is located on the right of the arc.

Parameter ``check``:
    ``True`` if we are to check whether the move can be performed at
    the given location.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the move can be performed. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Link_::r3_2
static const char *r3_2 =
R"doc(Tests for and/or performs a type III Reidemeister move.

There are two variants of this routine: one that takes an arc, and one
that takes a crossing. The other variant, which takes an arc, is more
flexible (since any of the three arcs involved in this move can be
passed). This variant, which takes a crossing, offers a canonical way
of performing the move (since for each move there is exactly one
crossing that describes it).

There are two boolean arguments that control the behaviour of this
routine: *check* and *perform*.

* If *check* and *perform* are both ``True`` (the default), then this
  routine will first check whether this move can be performed at the
  given location. If so, it will perform the move and return ``True``.
  If not, it will do nothing and return ``False``.

* If *check* is ``True`` but *perform* is ``False``, then this routine
  will simply check whether this move can be performed at the given
  location and return ``True`` or ``False`` accordingly.

* If *check* is ``False`` but *perform* is ``True``, then this routine
  will perform the move without any prior checks, and will always
  return ``True``. In this case, it must be known in advance that the
  move can be performed at the given location.

* If *check* and *perform* are both ``False``, then this routine does
  nothing and just returns ``True``. (There is no reason to use this
  combination of arguments.)

The location of this move is specified by the arguments *crossing* and
*side*. Specifically, this move takes place around a triangle, and one
of the arcs of this triangle is _uppermost_ (in that it passes above
the other two arcs). The given crossing should be the start point of
this uppermost arc; that is, when following the arc forwards,
*crossing* should be the first of the two crossings that we encounter.
The additional argument *side* indicates on which side of the
uppermost arc the third crossing is located.

You may pass a null pointer for *crossing*. However, in this case the
move cannot be performed, which means (i) *check* must be ``True``,
and therefore (ii) this routine will do nothing and return ``False``.

All crossings in this link will keep the same indices, and no
crossings will be created or destroyed. Instead, the three crossings
involved in this move will simply be reordered along the various
segments of the link.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    The given crossing is either a null pointer, or else some crossing
    in this link.

Parameter ``crossing``:
    identifies the crossing at the beginning of the "uppermost" arc
    that features in this move, as described above.

Parameter ``side``:
    0 if the third crossing of the triangle is located to the left of
    the uppermost arc (when walking along the arc in the forward
    direction), or 1 if the third crossing is located on the right of
    the uppermost arc.

Parameter ``check``:
    ``True`` if we are to check whether the move can be performed at
    the given location.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the move can be performed. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Link_::reflect
static const char *reflect =
R"doc(Converts this link into its reflection.

This routine changes the sign of every crossing, but leaves the upper
and lower strands the same. This operation corresponds to reflecting
the link diagram about some axis in the plane.)doc";

// Docstring regina::python::doc::Link_::resolve
static const char *resolve =
R"doc(Resolves the given crossing. The two incoming strands will switch
connections with the two outgoing strands, with the result that the
given crossing is removed entirely.

.. note::
    The number of components in the link _will_ change as a result of
    this operation.

Parameter ``c``:
    the crossing to resolve.)doc";

// Docstring regina::python::doc::Link_::reverse
static const char *reverse =
R"doc(Reverses the orientation of every component of this link.

This routine preserves both the sign and the upper/lower positions at
every crossing, but switches all incoming strands with outgoing
strands and vice versa (so next() becomes prev(), and prev() becomes
next()).)doc";

// Docstring regina::python::doc::Link_::reverse_2
static const char *reverse_2 =
R"doc(Reverses the orientation of just the link component that contains the
given strand. Other components of the link will not be modified.

For knots, this routine is identical to calling reverse().

Parameter ``component``:
    a strand belonging to some component of this link. This need not
    be the starting strand for the component (i.e., it does not need
    to be the strand that is returned by ``component()``). This may be
    a null strand reference, in which case this routine will do
    nothing.)doc";

// Docstring regina::python::doc::Link_::rewrite
static const char *rewrite =
R"doc(Explores all link diagrams that can be reached from this via
Reidemeister moves, without exceeding a given number of additional
crossings.

As of Regina 7.4, this routine is now available for any connected link
diagram with fewer than 64 link components. If this link has 64 or
more components then this routine will throw an exception (as
described below).

This routine iterates through all link diagrams that can be reached
from this via Reidemeister moves, without ever exceeding *height*
additional crossings beyond the original number. With the current
implementation, these diagrams **could become reflected and/or
reversed**, and moreover each diagram will only be considered once up
to reflection and/or reversal; be aware that this behaviour could
change and/or become configurable in a future version of Regina.

For every such link diagram (including this starting diagram), this
routine will call *action* (which must be a function or some other
callable object).

* *action* must take the following initial argument(s). Either (a) the
  first argument must be a link (the precise type is discussed below),
  representing the link diagram that has been found; or else (b) the
  first two arguments must be of types const std::string& followed by
  a link, representing both the link diagram and its signature (as
  returned by sig()). The second form is offered in order to avoid
  unnecessarily recomputation within the *action* function. If there
  are any additional arguments supplied in the list *args*, then these
  will be passed as subsequent arguments to *action*.

* The link argument will be passed as an rvalue; a typical action
  could (for example) take it by const reference and query it, or take
  it by value and modify it, or take it by rvalue reference and move
  it into more permanent storage.

* *action* must return a boolean. If *action* ever returns ``True``,
  then this indicates that processing should stop immediately (i.e.,
  no more link diagrams will be processed).

* *action* may, if it chooses, make changes to this link (i.e., the
  original link upon which rewrite() was called). This will not affect
  the search: all link diagrams that this routine visits will be
  obtained via Reidemeister moves from the original link diagram,
  before any subsequent changes (if any) were made.

* *action* will only be called once for each link diagram (including
  this starting diagram). In other words, no link diagram will be
  revisited a second time in a single call to rewrite().

This routine can be very slow and very memory-intensive, since the
number of link diagrams it visits may be exponential in the number of
crossings, and it records every link diagram that it visits (so as to
avoid revisiting the same diagram again). It is highly recommended
that you begin with *height* = 1, and if necessary try increasing
*height* one at a time until this routine becomes too expensive to
run.

If *height* is negative, then there will be _no_ bound on the number
of additional crossings. This means that the routine will _never
terminate_, unless *action* returns ``True`` for some link diagram
that is passed to it.

Since Regina 7.0, this routine will not return until the exploration
of link diagrams is complete, regardless of whether a progress tracker
was passed. If you need the old behaviour (where passing a progress
tracker caused the enumeration to start in the background), simply
call this routine in a new detached thread.

To assist with performance, this routine can run in parallel
(multithreaded) mode; simply pass the number of parallel threads in
the argument *threads*. Even in multithreaded mode, this routine will
not return until processing has finished (i.e., either *action*
returned ``True``, or the search was exhausted). All calls to *action*
will be protected by a mutex (i.e., different threads will never be
calling *action* at the same time); as a corollary, the action should
avoid expensive operations where possible (otherwise it will become a
serialisation bottleneck in the multithreading).

Precondition:
    This link has fewer than 64 link components.

Exception ``FailedPrecondition``:
    This link has 64 or more link components. If a progress tracker
    was passed, it will be marked as finished before the exception is
    thrown.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.

Python:
    This function is available in Python, and the *action* argument
    may be a pure Python function. However, its form is more
    restricted: the arguments *tracker* and *args* are removed, so you
    simply call it as rewrite(height, threads, action). Moreover,
    *action* must take exactly two arguments (const std::string&,
    Link&&) representing the signature and the link diagram, as
    described in option (b) above.

Parameter ``height``:
    the maximum number of _additional_ crossings to allow beyond the
    number of crossings originally present in this link diagram, or a
    negative number if this should not be bounded.

Parameter ``threads``:
    the number of threads to use. If this is 1 or smaller then the
    routine will run single-threaded.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Parameter ``action``:
    a function (or other callable object) to call for each link
    diagram that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial link argument(s).

Returns:
    ``True`` if some call to *action* returned ``True`` (thereby
    terminating the search early), or ``False`` if the search ran to
    completion.)doc";

// Docstring regina::python::doc::Link_::rotate
static const char *rotate =
R"doc(Rotates this link diagram, converting it into a different diagram of
the same link.

This routine keeps the sign of each crossing fixed, but switches the
upper and lower strands. This operation corresponds to a 3-dimensional
rotation about some axis in the plane.)doc";

// Docstring regina::python::doc::Link_::seifertCircles
static const char *seifertCircles =
R"doc(Returns the number of Seifert circles for this link diagram. This is
the number of circles obtained when we smooth every crossing in a way
that respects the orientations of the strands.

In other words: this routine returns the number of link components
that would be obtained if we called resolve() on every crossing in the
diagram.

Returns:
    the number of Seifert circles.)doc";

// Docstring regina::python::doc::Link_::selfFrame
static const char *selfFrame =
R"doc(Adds trivial twists to this link to ensure that each component has
zero writhe. Here the _writhe_ of a component *c* is the sum of the
signs of all crossings at which *c* crosses itself.

Any component(s) that already have zero writhe will be left unchanged.

This link will be modified directly.

Returns:
    ``True`` if the link diagram was changed, or ``False`` if every
    component already had zero writhe to begin with.)doc";

// Docstring regina::python::doc::Link_::sig
static const char *sig =
R"doc(Constructs the _signature_ for this knot or link diagram.

A _signature_ is a compact text representation of a link diagram that
uniquely determines the diagram up to: relabelling; rotating connected
components of the diagram; and (optionally) reflecting the entire
diagram and/or reversing some or all link components.

Signatures are now supported for all link diagrams with fewer than 64
link components. Specifically:

* Regina 7.3 and earlier only offered signatures for knots. As of
  Regina 7.4, signatures are now supported for arbitrary link diagrams
  (but see the next point), and for knots the new signatures are
  identical to the old.

* The implementation uses bitmasks, and a side-effect of this is that
  it can only support fewer than 64 link components. However, since
  the running time is exponential in the number of components (if we
  allow reversal, which is the default) then it would be completely
  infeasible to use this routine in practice with _more_ components
  than this. If there are 64 or more link components then this routine
  will throw an exception.

The signature is constructed entirely of printable characters, and has
length proportional to ``n log n``, where *n* is the number of
crossings.

The routine fromSig() can be used to recover a link diagram from its
signature. The resulting diagram might not be identical to the
original, but it will be related by zero or more applications of
relabelling, rotating connected components of the diagram, and/or
(according to the arguments) reflection of the entire diagram and/or
reversal of individual link components.

The running time is quadratic in the number of crossings and (if we
allow reversal, which is the default) exponential in the number of
link components. For this reason, signatures should not be used for
links with a large number of components.

This routine runs in quadratic time.

Exception ``NotImplemented``:
    This link diagram has 64 or more link components.

Parameter ``allowReflection``:
    ``True`` if reflecting the entire link diagram should preserve the
    signature, or ``False`` if the signature should distinguish
    between a diagram and its reflection (unless of course there is a
    symmetry).

Parameter ``allowReversal``:
    ``True`` if reversing some or all link components should preserve
    the signature, or ``False`` if the signature should distinguish
    between different orientations (again, unless of course there are
    symmetries).

Returns:
    the signature for this link diagram.)doc";

// Docstring regina::python::doc::Link_::simplify
static const char *simplify =
R"doc(Attempts to simplify this link diagram as intelligently as possible
using fast and greedy heuristics. Specifically, this routine tries
combinations of Reidemeister moves with the aim of reducing the number
of crossings.

Currently this routine uses simplifyToLocalMinimum() in combination
with random type III Reidemeister moves.

Although simplify() often works well, it can sometimes get stuck. If
this link is a knot (i.e., it has precisely one component), then in
such cases you can try the more powerful but (much) slower
simplifyExhaustive() instead.

This routine will never reflect or reverse the link.

.. warning::
    Running this routine multiple times upon the same link may return
    different results, since the implementation makes random
    decisions. More broadly, the implementation of this routine (and
    therefore its results) may change between different releases of
    Regina.

.. note::
    For long-term users of Regina: this is the routine that was for a
    long time called intelligentSimplify(). It was renamed to
    simplify() in Regina 7.4.

Returns:
    ``True`` if and only if the link diagram was successfully
    simplified.)doc";

// Docstring regina::python::doc::Link_::simplifyExhaustive
static const char *simplifyExhaustive =
R"doc(Attempts to simplify this link diagram using a slow but exhaustive
search through the Reidemeister graph. This routine is more powerful
but much slower than simplify().

Unlike simplify(), this routine **could potentially reflect or reverse
the link**.

As of Regina 7.4, this routine is now available for any connected link
diagram with fewer than 64 link components. If this link has 64 or
more components then this routine will throw an exception (as
described below).

This routine will iterate through all link diagrams that can be
reached from this via Reidemeister moves, without ever exceeding
*height* additional crossings beyond the original number.

If at any stage it finds a diagram with _fewer_ crossings than the
original, then this routine will call simplify() to simplify the
diagram further if possible and will then return ``True``. If it
cannot find a diagram with fewer crossings then it will leave this
link diagram unchanged and return ``False``.

This routine can be very slow and very memory-intensive: the number of
link diagrams it visits may be exponential in the number of crossings,
and it records every diagram that it visits (so as to avoid revisiting
the same diagram again). It is highly recommended that you begin with
*height* = 1, and if this fails then try increasing *height* one at a
time until either you find a simplification or the routine becomes too
expensive to run.

If *height* is negative, then there will be _no_ bound on the number
of additional crossings. This means that the routine will not
terminate until a simpler diagram is found. If no simpler diagram
exists then the only way to terminate this function is to cancel the
operation via a progress tracker (read on for details).

If you want a _fast_ simplification routine, you should call
simplify() instead. The benefit of simplifyExhaustive() is that, for
very stubborn link diagrams where simplify() finds itself stuck at a
local minimum, simplifyExhaustive() is able to "climb out" of such
wells.

Since Regina 7.0, this routine will not return until either the link
diagram is simplified or the exhaustive search is complete, regardless
of whether a progress tracker was passed. If you need the old
behaviour (where passing a progress tracker caused the exhaustive
search to start in the background), simply call this routine in a new
detached thread.

To assist with performance, this routine can run in parallel
(multithreaded) mode; simply pass the number of parallel threads in
the argument *threads*. Even in multithreaded mode, this routine will
not return until processing has finished (i.e., either the diagram was
simplified or the search was exhausted), and any change to this link
diagram will happen in the calling thread.

If this routine is unable to simplify the link diagram, then this link
diagram will not be changed.

Precondition:
    This link has at most 64 link components.

Exception ``FailedPrecondition``:
    This link has 64 or more link components. If a progress tracker
    was passed, it will be marked as finished before the exception is
    thrown.

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``height``:
    the maximum number of _additional_ crossings to allow beyond the
    number of crossings originally present in this diagram, or a
    negative number if this should not be bounded.

Parameter ``threads``:
    the number of threads to use. If this is 1 or smaller then the
    routine will run single-threaded.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    ``True`` if and only if this diagram was successfully simplified
    to fewer crossings.)doc";

// Docstring regina::python::doc::Link_::simplifyToLocalMinimum
static const char *simplifyToLocalMinimum =
R"doc(Uses type I and II Reidemeister moves to reduce the link monotonically
to some local minimum number of crossings.

End users will probably not want to call this routine. You should call
simplify() if you want a fast (and usually effective) means of
simplifying a link. If this link is a knot (i.e., it has precisely one
component), then you can also call simplifyExhaustive() if you are
still stuck and you want to try a slower but more powerful method
instead.

Type III Reidemeister moves (which do not reduce the number of
crossings) are not used in this routine. Such moves do however feature
in simplify().

This routine will never reflect or reverse the link.

.. warning::
    The implementation of this routine (and therefore its results) may
    change between different releases of Regina.

Parameter ``perform``:
    ``True`` if we are to perform the simplifications, or ``False`` if
    we are only to investigate whether simplifications are possible
    (defaults to ``True``).

Returns:
    if *perform* is ``True``, this routine returns ``True`` if and
    only if the link was changed to reduce the number of crossings; if
    *perform* is ``False``, this routine returns ``True`` if and only
    if it determines that it is capable of performing such a change.)doc";

// Docstring regina::python::doc::Link_::size
static const char *size =
R"doc(Returns the number of crossings in this link.

Note that a link can have more components than crossings (since it may
contain additional zero-crossing unknot components).

Returns:
    the number of crossings.)doc";

// Docstring regina::python::doc::Link_::source
static const char *source =
R"doc(Returns C++ or Python source code that can be used to reconstruct this
link.

This code will call Link::fromData(), passing a series of hard-coded
C++ initialiser lists or Python lists (depending on the requested
language).

The main purpose of this routine is to generate these hard-coded
lists, which can be tedious and error-prone to write by hand.

Parameter ``language``:
    the language in which the source code should be written.

Returns:
    the source code that was generated.)doc";

// Docstring regina::python::doc::Link_::strand
static const char *strand =
R"doc(Returns the strand in the link with the given integer ID.

Each strand ID is of the form 2*c*+*s*, where *c* is the index of the
crossing, and *s* is 0 or 1 for the lower or upper strand
respectively. A null strand reference (as used to indicate 0-crossing
unknot components) has an ID of -1.

Parameter ``id``:
    an integer between -1 and 2*size()-1 inclusive.

Returns:
    the strand of this link with the corresponding ID.

See also:
    StrandRef::id())doc";

// Docstring regina::python::doc::Link_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given link.

All crossings that belong to this link will be moved to *other*, and
all crossings that belong to *other* will be moved to this link.
Likewise, all cached properties (e.g., tree decompositions) will be
swapped.

In particular, any Crossing pointers or references and any StrandRef
objects will remain valid.

This routine will behave correctly if *other* is in fact this link.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both links which may in turn call arbitrary code
    via any registered packet listeners.

Parameter ``other``:
    the link whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Link_::translate
static const char *translate =
R"doc(Translates a crossing from some other link into the corresponding
crossing in this link.

Typically this routine would be used when the given crossing comes
from a link that is combinatorially identical to this, and you wish to
obtain the corresponding crossing in this link.

Specifically: if *other* refers to crossing number *k* of some other
link, then the return value will refer to crossing number *k* of this
link.

This routine behaves correctly even if *other* is a null pointer.

Precondition:
    This link contains at least as many crossings as the link
    containing *other* (though, as noted above, in typical scenarios
    both links would actually be combinatorially identical).

Parameter ``other``:
    the crossing to translate.

Returns:
    the corresponding crossing in this link.)doc";

// Docstring regina::python::doc::Link_::translate_2
static const char *translate_2 =
R"doc(Translates a strand reference from some other link into the
corresponding strand reference from this link.

Typically this routine would be used when the given strand comes from
a link that is combinatorially identical to this, and you wish to
obtain the corresponding strand in this link.

Specifically: if *other* refers to some strand (upper or lower) of
crossing number *k* of some other link, then the return value will
refer to the same strand (upper or lower) of crossing number *k* of
this link.

This routine behaves correctly even if *other* is a null reference.

Precondition:
    This link contains at least as many crossings as the link
    containing *other* (though, as noted above, in typical scenarios
    both links would actually be combinatorially identical).

Parameter ``other``:
    the strand reference to translate.

Returns:
    the corresponding strand reference for this link.)doc";

// Docstring regina::python::doc::Link_::tryR1
static const char *tryR1 =
R"doc(If possible, returns the diagram obtained by performing a type I
Reidemeister move at the given location to remove a crossing. If such
a move is not allowed, then this routine returns no value.

This link diagram will not be changed.

For more detail on type I moves and when they can be performed, see
r1(Crossing*, bool, bool).

Precondition:
    The given crossing is either a null pointer, or else some crossing
    in this link.

Parameter ``crossing``:
    identifies the crossing to be removed. See r1(Crossing*, bool,
    bool) for details on exactly how this will be interpreted.

Returns:
    The new link diagram obtained by performing the requested move, or
    no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Link_::tryR1_2
static const char *tryR1_2 =
R"doc(If possible, returns the diagram obtained by performing a type I
Reidemeister move at the given location to add a new crossing. If such
a move is not allowed, then this routine returns no value.

This link diagram will not be changed.

For more detail on type I moves and when they can be performed, see
r1(StrandRef, int, int, bool, bool).

Precondition:
    The given strand reference is either a null reference, or else
    refers to some strand of some crossing in this link.

Parameter ``arc``:
    identifies the arc of the link in which the new twist will be
    introduced. See r1(StrandRef, int, int, bool, bool) for details on
    exactly how this will be interpreted.

Parameter ``side``:
    0 if the twist should be introduced on the left of the arc (when
    walking along the arc in the forward direction), or 1 if the twist
    should be introduced on the right of the arc.

Parameter ``sign``:
    the sign of the new crossing that will be introduced as part of
    the twist; this must be +1 or -1.

Returns:
    The new link diagram obtained by performing the requested move, or
    no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Link_::tryR2
static const char *tryR2 =
R"doc(If possible, returns the diagram obtained by performing a type II
Reidemeister move at the given location to remove two crossings. If
such a move is not allowed, then this routine returns no value.

This link diagram will not be changed.

For more detail on type II moves and when they can be performed, see
r2(StrandRef, bool, bool).

Precondition:
    The given strand reference is either a null reference, or else
    refers to some strand of some crossing in this link.

Parameter ``arc``:
    identifies one of the arcs of the bigon about which the move will
    be performed. See r2(StrandRef, bool, bool) for details on exactly
    how this will be interpreted.

Returns:
    The new link diagram obtained by performing the requested move, or
    no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Link_::tryR2_2
static const char *tryR2_2 =
R"doc(If possible, returns the diagram obtained by performing a type II
Reidemeister move at the given location to remove two crossings. If
such a move is not allowed, then this routine returns no value.

This link diagram will not be changed.

For more detail on type II moves and when they can be performed, see
r2(Crossing*, bool, bool).

Precondition:
    The given crossing is either a null pointer, or else some crossing
    in this link.

Parameter ``crossing``:
    identifies the crossing at the beginning of the "upper" arc that
    features in this move. See r2(Crossing*, bool, bool) for details
    on exactly how this will be interpreted.

Returns:
    The new link diagram obtained by performing the requested move, or
    no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Link_::tryR2_3
static const char *tryR2_3 =
R"doc(If possible, returns the diagram obtained by performing a type II
Reidemeister move at the given location to add two new crossings. If
such a move is not allowed, then this routine returns no value.

This link diagram will not be changed.

For more detail on type II moves and when they can be performed, see
r2(StrandRef, int, StrandRef, int, bool, bool).

Precondition:
    Each of the given strand references is either a null reference, or
    else refers to some strand of some crossing in this link.

.. warning::
    The check for this move is expensive (linear time), since it
    includes testing whether both sides-of-arcs belong to the same
    2-cell of the knot diagram.

Parameter ``upperArc``:
    identifies which arc of the link will be passed over another. See
    r2(StrandRef, int, StrandRef, int, bool, bool) for details on
    exactly how this will be interpreted.

Parameter ``upperSide``:
    0 if the new overlap should take place on the left of *upperArc*
    (when walking along *upperArc* in the forward direction), or 1 if
    the new overlap should take place on the right of *upperArc*.

Parameter ``lowerArc``:
    identifies which arc of the link will be passed beneath another.
    See r2(StrandRef, int, StrandRef, int, bool, bool) for details on
    exactly how this will be interpreted.

Parameter ``lowerSide``:
    0 if the new overlap should take place on the left of *lowerArc*
    (when walking along *lowerArc* in the forward direction), or 1 if
    the new overlap should take place on the right of *lowerArc*.

Returns:
    The new link diagram obtained by performing the requested move, or
    no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Link_::tryR3
static const char *tryR3 =
R"doc(If possible, returns the diagram obtained by performing a type III
Reidemeister move at the given location. If such a move is not
allowed, then this routine returns no value.

This link diagram will not be changed.

For more detail on type III moves and when they can be performed, see
r3(StrandRef, int, bool, bool).

Precondition:
    The given strand reference is either a null reference, or else
    refers to some strand of some crossing in this link.

Parameter ``arc``:
    identifies one of the arcs of the triangle about which the move
    will be performed. See r3(StrandRef, int, bool, bool) for details
    on exactly how this will be interpreted.

Parameter ``side``:
    0 if the third crossing of the triangle is located to the left of
    the arc (when walking along the arc in the forward direction), or
    1 if the third crossing is located on the right of the arc.

Returns:
    The new link diagram obtained by performing the requested move, or
    no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Link_::tryR3_2
static const char *tryR3_2 =
R"doc(If possible, returns the diagram obtained by performing a type III
Reidemeister move at the given location. If such a move is not
allowed, then this routine returns no value.

This link diagram will not be changed.

For more detail on type III moves and when they can be performed, see
r3(Crossing*, int, bool, bool).

Precondition:
    The given crossing is either a null pointer, or else some crossing
    in this link.

Parameter ``crossing``:
    identifies the crossing at the beginning of the "uppermost" arc
    that features in this move. See r3(Crossing*, int, bool, bool) for
    details on exactly what this means.

Parameter ``side``:
    0 if the third crossing of the triangle is located to the left of
    the uppermost arc (when walking along the arc in the forward
    direction), or 1 if the third crossing is located on the right of
    the uppermost arc.

Returns:
    The new link diagram obtained by performing the requested move, or
    no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Link_::underForComponent
static const char *underForComponent =
R"doc(Locates an under-crossing within the same link component as the given
strand. The choice of _which_ under-crossing is returned will be
arbitrary (i.e., it might not be the _first_ under-crossing).

Parameter ``component``:
    a strand reference in this link, which may be a null reference
    (indicating a zero-crossing component).

Returns:
    an under-crossing in the same link component, or a null reference
    if the given link component contains only over-crossings (i.e., it
    is a zero-crossing unknot placed above the rest of the diagram).)doc";

// Docstring regina::python::doc::Link_::useTreeDecomposition
static const char *useTreeDecomposition =
R"doc(Instructs Regina to use the given tree decomposition as the starting
point whenever it needs a tree decomposition for this link.

For some link routines, including niceTreeDecomposition() as well as
computations such as jones() that support the option
Algorithm::Treewidth, Regina needs a tree decomposition of the planar
4-valent multigraph formed by this link diagram.

By default, Regina will compute (and then cache) such a tree
decomposition itself, using in-built greedy heuristics. This routine
allows you to supply your _own_ tree decomposition (which, for
example, might be a smaller-width tree decomposition that you found
using third-party software). By supplying your own tree decomposition
*td* through this routine, Regina will throw away any pre-computed
tree decomposition that it has cached, and will instead cache *td* for
future use instead.

Regina may modify the given tree decomposition for its purposes. In
particular, *td* does not need to be a _nice_ tree decomposition
(indeed, it does not need to have any special properties beyond the
definition of a tree decomposition). Regina will automatically create
a nice tree decomposition from it if *td* is not nice already.

Parameter ``td``:
    a tree decomposition of the planar 4-valent multigraph formed by
    this link diagram.)doc";

// Docstring regina::python::doc::Link_::writhe
static const char *writhe =
R"doc(Returns the writhe of this link diagram.

This is _not_ an invariant of the link; instead it depends on the
particular link diagram. It is computed as the sum of the signs of all
crossings. It is preserved under Reidemeister moves II and III, but
not I.

Returns:
    the writhe.)doc";

// Docstring regina::python::doc::Link_::writheOfComponent
static const char *writheOfComponent =
R"doc(Returns the writhe of a single component of this link diagram.

This is the writhe of the diagram when all _other_ components are
removed. It is computed as the sum of the signs of all crossings at
which the given component crosses itself.

In this version of writheOfComponent(), the component is indicated by
the argument *strand*, which may be any strand along the component. In
particular, *strand* does not need to be the "starting strand"
returned by component().

The given strand may be a null strand, in which case the return value
will be 0 (since Regina uses null strands to refer to zero-crossing
unknot components). This is always allowed, regardless of whether the
link actually contains any zero-crossing unknot components.

Parameter ``component``:
    any strand along the component of interest.

Returns:
    the writhe of the component containing the given strand, or 0 if
    the given strand is a null strand.)doc";

// Docstring regina::python::doc::Link_::writheOfComponent_2
static const char *writheOfComponent_2 =
R"doc(Returns the writhe of a single component of this link diagram.

This is the writhe of the diagram when all _other_ components are
removed. It is computed as the sum of the signs of all crossings at
which the given component crosses itself.

In this version of writheOfComponent(), the component is indicated by
its index. This function is equivalent to calling
``writheOfComponent(component(index))``.

Parameter ``index``:
    the index of the requested component. This must be between 0 and
    countComponents()-1 inclusive.

Returns:
    the writhe of the given component.)doc";

}

namespace StrandRef_ {

// Docstring regina::python::doc::StrandRef_::__as_bool
static const char *__as_bool =
R"doc(Tests whether this is a non-null reference.

Returns:
    ``True`` if this is not a null reference (i.e., crossing() does
    not return a null pointer), or ``False`` if this is a null
    reference.)doc";

// Docstring regina::python::doc::StrandRef_::__copy
static const char *__copy = R"doc(Creates a new copy of the given strand reference.)doc";

// Docstring regina::python::doc::StrandRef_::__dec
static const char *__dec =
R"doc(Moves this reference backward against the direction of the link until
it reaches the previous crossing. (Of course, if the link contains a
trivial twist then this may in fact return to the same crossing but
the other strand).

This is a postincrement operator: the object will be changed, but a
copy of the original reference will be returned.

Precondition:
    This is not a null reference, i.e., crossing() does not return
    ``None``.

Python:
    This routine is available under the name dec().

Returns:
    a copy of this object before the change took place.)doc";

// Docstring regina::python::doc::StrandRef_::__default
static const char *__default =
R"doc(Initialises this to a null reference.

The pointer returned by crossing() will be ``None``, and the integer
returned by strand() will be 0.)doc";

// Docstring regina::python::doc::StrandRef_::__eq
static const char *__eq =
R"doc(Tests whether this and the given reference are identical.

Two references are identical if and only if they return the same
values for both crossing() and strand().

.. warning::
    If you create a null reference by calling StrandRef(``None``, 1)
    then this will _not_ be considered equal to the null reference
    created by calling StrandRef(), since the latter is equivalent to
    calling StrandRef(``None``, 0).

Returns:
    ``True`` if and only if this and the given reference are
    identical.)doc";

// Docstring regina::python::doc::StrandRef_::__inc
static const char *__inc =
R"doc(Moves this reference forward along the direction of the link until it
reaches the next crossing. (Of course, if the link contains a trivial
twist then this may in fact return to the same crossing but the other
strand).

This is a postincrement operator: the object will be changed, but a
copy of the original reference will be returned.

Precondition:
    This is not a null reference, i.e., crossing() does not return
    ``None``.

Python:
    This routine is available under the name inc().

Returns:
    a copy of this object before the change took place.)doc";

// Docstring regina::python::doc::StrandRef_::__init
static const char *__init =
R"doc(Initialises this to the given strand of the given crossing.

The given crossing may be ``None``, in which case this will become a
null reference. If you are creating a null reference, then it is
highly recommended that you pass *strand* as 0, so that comparison
tests treat this null reference as equal to a null reference created
by the zero-argument constructor.

Parameter ``crossing``:
    the crossing being identified.

Parameter ``strand``:
    0 to denote the strand running under the crossing, or 1 to denote
    the strand running over the crossing.)doc";

// Docstring regina::python::doc::StrandRef_::crossing
static const char *crossing =
R"doc(The crossing that this reference points to.

The information returned by crossing() and strand() together pinpoint
exactly which strand of the link this reference points to.

Returns:
    the crossing, or ``None`` if this is a null reference.)doc";

// Docstring regina::python::doc::StrandRef_::id
static const char *id =
R"doc(An integer that uniquely identifies this strand within the link.

This integer will be 2*c*+*s*, where *c* is the index of the crossing,
and *s* is 0 or 1 for the lower or upper strand respectively.

If this is a null reference, then id() will return -1.

A strand can be restored from its ID by calling Link::strand().

Returns:
    the unique ID of this strand within the link.)doc";

// Docstring regina::python::doc::StrandRef_::jump
static const char *jump =
R"doc(Jumps to the other strand at the same crossing.

This reference will be changed directly. The crossing will remain the
same, but the strand will switch from lower to upper or vice versa.)doc";

// Docstring regina::python::doc::StrandRef_::next
static const char *next =
R"doc(Returns the crossing reference that comes immediately after this when
walking forward along the direction of the link.

Equivalently, this routine returns the reference that would be
obtained by calling the increment (++) operator (but, unlike the
increment operator, this routine does not actually change the current
reference).

Precondition:
    This is not a null reference, i.e., crossing() does not return
    ``None``.

Returns:
    the crossing reference that follows this.)doc";

// Docstring regina::python::doc::StrandRef_::prev
static const char *prev =
R"doc(Returns the crossing reference that comes immediately before this when
walking backward against the direction of the link.

Equivalently, this routine returns the reference that would be
obtained by calling the decrement (--) operator (but, unlike the
decrement operator, this routine does not actually change the current
reference).

Precondition:
    This is not a null reference, i.e., crossing() does not return
    ``None``.

Returns:
    the crossing reference that precedes this.)doc";

// Docstring regina::python::doc::StrandRef_::strand
static const char *strand =
R"doc(Indicates whether this reference points to the upper or lower strand
of the relevant crossing.

A value of 1 denotes the upper strand (which passes over the
crossing), and a value of 0 denotes the lower strand (which passes
under the crossing).

The information returned by crossing() and strand() together pinpoint
exactly which strand of the link this reference points to.

Returns:
    either 0 or 1 to indicate the strand.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

