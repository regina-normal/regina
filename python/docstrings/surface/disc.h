/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::DiscSetSurfaceDataImpl
static const char *DiscSetSurfaceDataImpl =
R"doc(Stores a piece of data alongside every normal disc within a particular
normal surface.

End users should not refer to this class directly; instead use one of
the type aliases DiscSetSurfaceData<T> or DiscSetSurface.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Template parameter ``TetData``:
    This must be either (1) DiscSetTet, in which case there will be no
    additional data stored for each normal disc; or (2) a class of the
    form DiscSetTetData<T>, in which case there will be data of type
    *T* stored alongside each normal disc.

.. warning::
    This class converts the number of normal discs of a given type
    from LargeInteger to ``unsigned long``. See the precondition
    below.

Precondition:
    The number of normal discs of a particular type in a particular
    tetrahedron can be represented by a long integer.

Precondition:
    This class should only be used with *embedded* normal surfaces.

Python:
    The only instance of this class that is available through python
    is DiscSetSurface (i.e., the "vanilla" case where *TetData* is the
    type DiscSetTet).)doc";

// Docstring regina::python::doc::DiscSetTet
static const char *DiscSetTet =
R"doc(Represents a set of normal discs inside a single tetrahedron. The
numbers of discs of each type are stored in this structure, so
querying them is fast regardless of the underlying normal surface
coordinate system used.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

.. warning::
    This class converts the number of normal discs of a given type
    from LargeInteger to ``unsigned long``. See the precondition
    below.

Precondition:
    The number of normal discs of a particular type in a particular
    tetrahedron can be represented by a long integer.

Precondition:
    This class should only be used with *embedded* normal surfaces.)doc";

// Docstring regina::python::doc::DiscSpec
static const char *DiscSpec =
R"doc(Specifies a single normal disc in a normal surface.

There are 10 disc types. Types 0-3 represent triangles 0-3, types 4-6
represent quads 0-2 and types 7-9 represent octagons 0-2.

Discs of a specific type are assigned numbers from 0 upwards.
Triangular discs are numbered outwards from the vertex they surround.
Quad discs and octagonal discs are numbered outwards away from vertex
0 of the tetrahedron.

Note that, unlike DiscType in which the meaning of DiscType::type is
flexible, the meaning of DiscSpec::type is fixed as described above.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

.. warning::
    This class converts the indices of normal discs of a given type
    from LargeInteger to ``unsigned long``. See the precondition
    below.

Precondition:
    The number of normal discs of a particular type in a particular
    tetrahedron can be represented by a long integer.

Precondition:
    This class should only be used with *embedded* normal surfaces.)doc";

// Docstring regina::python::doc::DiscSpecIterator
static const char *DiscSpecIterator =
R"doc(A forward iterator used for running through all normal discs in a
normal surface.

This class is designed to work alongside
DiscSetSurfaceDataImpl<TetData>. Typically you would not need to
specify the template argument *TetData*; for copy initialisation you
can use the ``auto`` type, and when constructing from a
DiscSetSurfaceData<T> the template argument can be automatically
deduced.

.. warning::
    This class converts the indices of normal discs of a given type
    from LargeInteger to ``unsigned long``. See the precondition
    below.

Precondition:
    The number of normal discs of a particular type in a particular
    tetrahedron can be represented by a long integer.

Python:
    The only instance of this class that is available through python
    is the iterator for DiscSetSurface (i.e., the "vanilla" case where
    *TetData* is the type DiscSetTet). Moreover, instead of the C++
    interface described here, in Python DiscSetSurface and this class
    together implement the Python iterable/iterator interface. The
    DiscSetSurface class implements ``__iter__()``, which returns a
    DiscSpecIterator; then DiscSpecIterator implements ``next()``,
    which either returns the next normal disc in the set or else
    throws a ``StopException`` if there are no more discs to return.)doc";

// Docstring regina::python::doc::discOrientationFollowsEdge
static const char *discOrientationFollowsEdge =
R"doc(Determines whether or not the natural boundary orientation of a normal
disc of the given type follows the given directed normal arc. Natural
boundary orientation is defined by arrays regina::triDiscArcs,
regina::quadDiscArcs and regina::octDiscArcs.

Precondition:
    The given normal arc lies on a normal disc of the given type.

Parameter ``discType``:
    the normal disc type under consideration; this should be between 0
    and 9 inclusive, as described by the DiscSpec class notes.

Parameter ``vertex``:
    the vertex about which the normal arc runs.

Parameter ``edgeStart``:
    the start vertex of the edge to which the normal arc is parallel.

Parameter ``edgeEnd``:
    the end vertex of the edge to which the normal arc is parallel.)doc";

// Docstring regina::python::doc::numberDiscsAwayFromVertex
static const char *numberDiscsAwayFromVertex =
R"doc(Determines whether or not normal discs of the given type are numbered
away from the given vertex.

Parameter ``discType``:
    the normal disc type under consideration; this should be between 0
    and 9 inclusive, as described by the DiscSpec class notes.

Parameter ``vertex``:
    the vertex under consideration; this should be between 0 and 3
    inclusive.

Returns:
    ``True`` if normal discs of the given type are numbered away from
    the given vertex, or ``False`` if they are numbered towards the
    given vertex.)doc";

namespace DiscSetSurfaceDataImpl_ {

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given set of normal discs.

The data for each disc will be copied using the copy assignment
operator for type *T*.

Parameter ``src``:
    the disc set to copy.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::__eq
static const char *__eq =
R"doc(Determines whether this and the given set have the same number of
discs of each type in each tetrahedron, and contain the same data for
corresponding discs.

This routine does not consider whether the two underlying
triangulations are the same; it merely compares the disc counts and
associated data within each set. If the two disc sets come from
triangulations with different sizes, and/or surfaces with different
disc counts in one or more tetarhedra, then this comparison will
return ``False``.

The associated data (of type *T*) will be compared using the equality
operator (==).

Returns:
    ``True`` if and only if both sets are the same, as described
    above.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::__init
static const char *__init =
R"doc(Creates a new disc set corresponding to the discs of the given normal
surface. The data for each disc will be initialised using its default
constructor.

This disc set will be usable even if it outlives the given surface
and/or its underlying triangulation. This is because it takes a
snapshot of the necessary information as it appears right now (using
Regina's snapshotting machinery, which only takes a deep copy when
absolutely necessary).

Parameter ``surface``:
    the normal surface whose discs we shall use.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over all normal discs in the underlying
normal surface.

In Python, an object of this class can be treated as an iterable
object:

```
for disc in surfaceData:
    ...
```

Returns:
    an iterator over all normal discs.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::__ne
static const char *__ne =
R"doc(Determines whether this and the given set have different numbers of
discs of some type in some tetrahedron, or contain different data for
some pair of corresponding discs.

This routine does not consider whether the two underlying
triangulations are the same; it merely compares the disc counts and
associated data within each set. If the two disc sets come from
triangulations with different sizes, and/or surfaces with different
disc counts in one or more tetarhedra, then this comparison will
return ``True``.

The associated data (of type *T*) will be compared using the equality
operator (==).

Returns:
    ``True`` if and only if both sets are not the same, as described
    above.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::adjacentDisc
static const char *adjacentDisc =
R"doc(Determines which normal disc is adjacent to the given normal disc
along the given directed normal arc in the surface described by this
disc set.

A directed normal arc will be specified by a permutation *p*, where
the arc runs around vertex ``p[0]`` parallel to the directed edge from
vertex ``p[1]`` to ``p[2]``.

Parameter ``disc``:
    the given normal disc; this must be a disc in this disc set.

Parameter ``arc``:
    the given normal arc; this must actually be an arc on the boundary
    of the given normal disc (although it may run in either
    direction).

Returns:
    a pair (*adj*, *perm*), where *adj* is the normal disc adjacent to
    the given disc along the given arc, and *perm* represents the same
    directed normal arc that was passed but expressed in terms of the
    vertices of the adjacent tetrahedron. This will be no value if
    there is no adjacent disc.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given disc sets.

This global routine simply calls DiscSetSurfaceDataImpl::swap(); it is
provided so that DiscSetSurfaceDataImpl meets the C++ Swappable
requirements.

Parameter ``a``:
    the first disc set whose contents should be swapped.

Parameter ``b``:
    the second disc set whose contents should be swapped.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::nDiscs
static const char *nDiscs =
R"doc(Determines the number of discs of the given type inside the given
tetrahedron.

Parameter ``tetIndex``:
    the index in the triangulation of the tetrahedron to examine.

Parameter ``type``:
    the disc type to examine; this should be between 0 and 9
    inclusive. Disc types are outlined in the DiscSpec class notes.

Returns:
    the number of discs of the given type inside the given
    tetrahedron.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::nTets
static const char *nTets =
R"doc(Returns the number of tetrahedra in the underlying triangulation.

Returns:
    the number of tetrahedra.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given disc set.

Parameter ``other``:
    the disc set whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::DiscSetSurfaceDataImpl_::tetDiscs
static const char *tetDiscs =
R"doc(Returns the specific set of discs living inside the given tetrahedron.

Parameter ``tetIndex``:
    the index in the triangulation of the given tetrahedron.

Returns:
    the set of discs inside the given tetrahedron.)doc";

}

namespace DiscSetTet_ {

// Docstring regina::python::doc::DiscSetTet_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given set of normal discs.

Parameter ``src``:
    the disc set to copy.)doc";

// Docstring regina::python::doc::DiscSetTet_::__eq
static const char *__eq =
R"doc(Determines whether this and the given set have the same number of
discs of each type.

This routine does not consider whether the two underlying tetrahedra
are the same; it merely compares the ten disc counts in each set.

Returns:
    ``True`` if and only if both sets are the same, as described
    above.)doc";

// Docstring regina::python::doc::DiscSetTet_::__init
static const char *__init =
R"doc(Creates a new set of normal discs corresponding to the discs of the
given normal surface that lie within the given tetrahedron.

Parameter ``surface``:
    the normal surface whose discs we shall use.

Parameter ``tetIndex``:
    the index in the triangulation of the tetrahedron that our discs
    must lie in; this must be between 0 and ``tri.size()-1``
    inclusive, where ``tri`` is the triangulation containing the given
    normal surface.)doc";

// Docstring regina::python::doc::DiscSetTet_::__init_2
static const char *__init_2 =
R"doc(Creates a new set of normal discs where the number of discs of each
type is explicitly given.

Parameter ``tri0``:
    the number of triangular discs surrounding vertex 0.

Parameter ``tri1``:
    the number of triangular discs surrounding vertex 1.

Parameter ``tri2``:
    the number of triangular discs surrounding vertex 2.

Parameter ``tri3``:
    the number of triangular discs surrounding vertex 3.

Parameter ``quad0``:
    the number of quadrilateral discs of type 0.

Parameter ``quad1``:
    the number of quadrilateral discs of type 1.

Parameter ``quad2``:
    the number of quadrilateral discs of type 2.

Parameter ``oct0``:
    the number of octahedral discs of type 0.

Parameter ``oct1``:
    the number of octahedral discs of type 1.

Parameter ``oct2``:
    the number of octahedral discs of type 2.)doc";

// Docstring regina::python::doc::DiscSetTet_::__ne
static const char *__ne =
R"doc(Determines whether this and the given set do not have the same number
of discs of each type.

This routine does not consider whether the two underlying tetrahedra
are the same; it merely compares the ten disc counts in each set.

Returns:
    ``True`` if and only if both sets are not the same, as described
    above.)doc";

// Docstring regina::python::doc::DiscSetTet_::arcFromDisc
static const char *arcFromDisc =
R"doc(Determines which normal arc of a given type on a given face of this
tetrahedron corresponds to the given normal disc.

Precondition:
    The given normal disc actually meets a normal arc of the given
    type on the given face.

Parameter ``arcFace``:
    the face of this tetrahedron containing the normal arc (between 0
    and 3 inclusive).

Parameter ``arcVertex``:
    the vertex of this tetrahedron about which the normal arc runs
    (between 0 and 3 inclusive); *arcFace* and *arcVertex* should not
    be the same.

Parameter ``discType``:
    the disc type of the given normal disc; this should be between 0
    and 9 inclusive, as described in the DiscSpec class notes.

Parameter ``discNumber``:
    indicates which normal disc of the given disc type is referred to
    (between 0 and ``nDiscs(discType)-1`` inclusive).

Returns:
    the number of the normal arc of the given type that belongs to the
    given normal disc. Arcs of a given type (where *arcFace* and
    *arcVertex* together define the arc type) are numbered starting at
    0 from the tetrahedron vertex outwards.)doc";

// Docstring regina::python::doc::DiscSetTet_::discFromArc
static const char *discFromArc =
R"doc(Determines which normal disc in this tetrahedron meets the given
normal arc on the given face.

Precondition:
    The given normal arc actually exists in the normal surface with
    which this *DiscSetTet* object was created.

Parameter ``arcFace``:
    the face of this tetrahedron containing the normal arc (between 0
    and 3 inclusive).

Parameter ``arcVertex``:
    the vertex of this tetrahedron about which the normal arc runs
    (between 0 and 3 inclusive); *arcFace* and *arcVertex* should not
    be the same.

Parameter ``arcNumber``:
    indicates which normal arc of the given type is referred to. Arcs
    of a given type (where *arcFace* and *arcVertex* together define
    the arc type) are numbered starting at 0 from the tetrahedron
    vertex outwards.

Returns:
    a pair (*discType*, *discNumber*), where *discType* gives the disc
    type of the normal disc that meets the given normal arc (between 0
    and 9 inclusive), and *discNumber* indicates which normal disc of
    the returned disc type (``discType``) meets the given normal arc
    (between 0 and ``nDiscs(discType)-1`` inclusive).)doc";

// Docstring regina::python::doc::DiscSetTet_::nDiscs
static const char *nDiscs =
R"doc(Determines the number of discs of the given type inside this
tetrahedron.

Parameter ``type``:
    the disc type to examine; this should be between 0 and 9
    inclusive. Disc types are outlined in the DiscSpec class notes

Returns:
    the number of discs of the given type inside this tetrahedron.)doc";

}

namespace DiscSpecIterator_ {

// Docstring regina::python::doc::DiscSpecIterator_::__eq
static const char *__eq =
R"doc(Determines if this and the given iterator are equal.

Two iterators are considered equal if (i) they were constructed from
the same DiscSetSurface object (not two different DiscSetSurface
objects with identical contents), and (ii) they point to the same disc
of the same tetrahedron.

Parameter ``other``:
    the iterator to compare with this.

Returns:
    ``True`` if and only if this and the given iterator are equal.)doc";

// Docstring regina::python::doc::DiscSpecIterator_::__ne
static const char *__ne =
R"doc(Determines if this and the given iterator are different.

Two iterators are considered equal if (i) they were constructed from
the same DiscSetSurface object (not two different DiscSetSurface
objects with identical contents), and (ii) they point to the same disc
of the same tetrahedron.

Parameter ``other``:
    the iterator to compare with this.

Returns:
    ``True`` if and only if this and the given iterator are equal.)doc";

// Docstring regina::python::doc::DiscSpecIterator_::__next__
static const char *__next__ =
R"doc(Returns the current DiscSpec and increments this iterator.

Exception ``StopIteration``:
    The iterator is already past-the-end when this function is called.

Returns:
    a reference to the disc that this iterator is pointing to, before
    the increment takes place.)doc";

}

namespace DiscSpec_ {

// Docstring regina::python::doc::DiscSpec_::__copy
static const char *__copy =
R"doc(Creates a new disc specifier that is a clone of the given specifier.

Parameter ``cloneMe``:
    the disc specifier to clone.)doc";

// Docstring regina::python::doc::DiscSpec_::__eq
static const char *__eq =
R"doc(Determines if this and the given disc specifier contain identical
information.

Parameter ``other``:
    the disc specifier to compare with this.

Returns:
    ``True`` if and only if this and the given disc specifier contain
    identical information.)doc";

// Docstring regina::python::doc::DiscSpec_::__init
static const char *__init = R"doc(Creates a new uninitialised disc specifier.)doc";

// Docstring regina::python::doc::DiscSpec_::__init_2
static const char *__init_2 =
R"doc(Creates a new disc specifier containing the given values.

Parameter ``newTetIndex``:
    the index in the triangulation of the tetrahedron containing the
    disc.

Parameter ``newType``:
    the disc type; this is between 0 and 9 inclusive, as described in
    the *DiscSpec* class notes.

Parameter ``newNumber``:
    specifies which disc of the particular type in the particular
    tetrahedron is being referred to; discs are numbered as described
    in the *DiscSpec* class notes.)doc";

// Docstring regina::python::doc::DiscSpec_::__ne
static const char *__ne =
R"doc(Determines if this and the given disc specifier contain different
information.

Parameter ``other``:
    the disc specifier to compare with this.

Returns:
    ``True`` if and only if this and the given disc specifier contain
    different information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

