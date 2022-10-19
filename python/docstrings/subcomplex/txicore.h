/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::TxICore
static const char *TxICore =
R"doc(Provides a triangulation of the product ``T x I`` (the product of the
torus and the interval). Generally these triangulations are only one
tetrahedron thick (i.e., a "thin I-bundle"), though this is not a
strict requirement of this class. Triangulations of this type are
generally used as components of larger triangulations (such as layered
surface bundles).

This product has two torus boundaries, called the *upper* and *lower*
boundaries. Each of these boundary tori must be formed from precisely
two triangles. This class tracks the mappings between parallel curves
on the upper and lower boundaries, as well as mappings from boundary
curves to specific tetrahedron edges.

For each of the two torus boundaries, two curves are chosen as
generators of the fundamental group; these curves are called α and β.
Note that there is no requirement that the upper α and β be parallel
to the lower α and β. The parallelReln() routine can be called to
establish the precise relationship between these upper and lower
curves.

Every object of this class contains a full copy of the triangulation
that it describes (so you should not create excessive objects of this
class without reason). This triangulation can be accessed through the
core() routine.

This is an abstract base class; however, the concrete subclasses offer
all of the usual copy, move and swap operations. See each subclass for
details.)doc";

// Docstring regina::python::doc::TxIDiagonalCore
static const char *TxIDiagonalCore =
R"doc(One of a family of thin ``T x I`` triangulations that typically appear
at the centres of layered torus bundles. Different triangulations in
this family use different numbers of tetrahedra, with the larger
triangulations producing more complicated relationships between the
upper and lower boundary curves.

Members of this family are parameterised by their size (the number of
tetrahedra) and an additional integer *k*, where 1 ≤ *k* ≤ *size* - 5.
Note that this means we must have *size* ≥ 6. The member of this
family of size *n* with additional parameter *k* is labelled
``T_n:k``.

It is worth noting that ``T_n:k`` is isomorphic to ``T_n:(n-4-k)``, so
in reality there are only [(*n*-4)/2] different triangulations for a
given size (rounded down).

A triangulation of this family is most easily defined in terms of its
central torus. Central surfaces are described in detail in "Structures
of small closed non-orientable 3-manifold triangulations", Benjamin A.
Burton, J. Knot Theory Ramifications 16 (2007), 545--574; in
particular, see the section on thin I-bundles.

The central torus begins with two triangles *u0* and *u1* (which
eventually provide the upper torus boundary), with a chain of
quadrilaterals *q1*, ..., *q*(*n*-5) descending diagonally beneath
them as illustrated in the diagram below.

(Image available in HTML docs)

We then distort quadrilateral *qk* and attach two more triangles *w0*
and *w1* to its side (these will eventually provide the lower torus
boundary). This is illustrated in the following diagram.

(Image available in HTML docs)

The entire central torus wraps from left to right (so the lower left
edges of most quadrilaterals *qi* are identified with the upper right
edges of *q*(*i*-1), and the left edge of *qk* is identified with the
right edge of *w1*). As an exception, the two uppermost edges are
identified with the two lowermost edges in a parallel fashion (so the
upper left edge of *u1* is identified with the lower right edge of
*q1*, and the adjacent edges at right angles to these are also
identified).

The four triangles in the central torus correspond to the four
tetrahedra in the triangulation that provide the boundary triangles.
The upper boundary is coned out from triangles *u0* and *u1*, and the
lower boundary is coned out from triangles *w0* and *w1*. In each
boundary, *u0* or *w0* gives the first boundary triangle and *u1* or
*w1* gives the second. The directions of the corresponding α and β
curves are illustrated below.

(Image available in HTML docs)

As a final illustration, the example below shows the central surface
in the case (*n*, *k*) = (9, 2).

(Image available in HTML docs)

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::TxIParallelCore
static const char *TxIParallelCore =
R"doc(A specific six-tetrahedron TxICore triangulation that does not fit
neatly into other families.

This triangulation contains the fewest possible number of tetrahedra
(TxICore triangulations are not seen below six tetrahedra). It is
referred to as ``T_6^1`` in the paper "Structures of small closed non-
orientable 3-manifold triangulations", Benjamin A. Burton, J. Knot
Theory Ramifications 16 (2007), 545--574. In Regina it is given the
name ``T_6*``, to avoid confusion with the different TxIDiagonalCore
triangulation ``T_6:1``.

The triangulations of the upper and lower boundary tori are completely
parallel (and in particular, the upper and lower α curves are
parallel, as are the upper and lower β curves).

For reference, the central torus of this triangulation is depicted
below. The left and right sides of the diagram are identified, as are
the top and bottom. The four triangles *u0*, *u1*, *w0* and *w1*
provide the boundary triangles of the overall triangulation, with the
upper boundary coned out from triangles *u0* and *u1* and the lower
boundary coned out from triangles *w0* and *w1*. In each boundary,
*u0* or *w0* gives the first boundary triangle and *u1* or *w1* gives
the second. The directions of the corresponding α and β curves are are
also included.

(Image available in HTML docs)

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace TxICore_ {

// Docstring regina::python::doc::TxICore_::__eq
static const char *__eq =
R"doc(Determines if this and the given ``T x I`` triangulation are of the
same type and have the same parameters.

If this returns ``True``, then the triangulations returned by core()
should also be combinatorially identical.

Parameter ``other``:
    the ``T x I`` triangulation to compare with this.

Returns:
    ``True`` if and only if this and the given triangulation are of
    the same type and have the same parameters.)doc";

// Docstring regina::python::doc::TxICore_::__ne
static const char *__ne =
R"doc(Determines if this and the given ``T x I`` triangulation are of
different types and/or have different parameters.

If this returns ``False`` (i.e., both objects compare as equal), then
the triangulations returned by core() should also be combinatorially
identical.

Parameter ``other``:
    the ``T x I`` triangulation to compare with this.

Returns:
    ``True`` if and only if this and the given triangulation are of
    different types and/or have different parameters.)doc";

// Docstring regina::python::doc::TxICore_::bdryReln
static const char *bdryReln =
R"doc(Returns a 2-by-2 matrix describing the α and β curves on a torus
boundary in terms of specific tetrahedron edges.

Consider the first triangle of the given boundary. Let *t* be the
tetrahedron returned by bdryTet(*whichBdry*, 0) and let *p* be the
permutation returned by bdryRoles(*whichBdry*, 0).

Let *edge01* be the directed edge from vertex *p*[0] to *p*[1] of
tetrahedron *t*, and let *edge02* be the directed edge from vertex
*p*[0] to *p*[2] of tetrahedron *t*. Then the matrix returned by this
routine describes how the directed edges *edge01* and *edge02* relate
to the α and β curves on the given boundary. Specifically:

```
    [ α ]                  [ edge01 ]
    [   ]  =  bdryReln() * [        ] .
    [ β ]                  [ edge02 ]
```

It is guaranteed that this matrix has determinant +1 or -1.

Parameter ``whichBdry``:
    0 if the upper boundary should be examined, or 1 if the lower
    boundary should be examined.

Returns:
    the relationship between the boundary curves and tetrahedron
    edges.)doc";

// Docstring regina::python::doc::TxICore_::bdryRoles
static const char *bdryRoles =
R"doc(Describes which tetrahedron vertices play which roles in the upper and
lower boundary triangles.

Each boundary torus contains two triangles, whose vertices can be
numbered 0, 1 and 2 according to the following diagram. This diagram
is completely symmetric, in that edges 1-2 are no more special than
edges 0-2 or 0-1. The important observations are that edges 1-2 and
2-1 of each triangle are identified, edges 0-2 and 2-0 of each
triangle are identified and edges 0-1 and 1-0 of each triangle are
identified.

```
          *--->>--*
          |0  2 / |
  First   |    / 1|  Second
 triangle v   /   v triangle
          |1 /    |
          | / 2  0|
          *--->>--*
```

This routine returns a permutation that maps these integers 0,1,2 to
real tetrahedron vertices. Let *t* be the tetrahedron returned by
bdryTet(*whichBdry*, *whichTri*) and let *p* be the permutation
returned by bdryRoles(*whichBdry*, *whichTri*). Then vertices *p*[0],
*p*[1] and *p*[2] of tetrahedron *t* correspond to the markings 0, 1
and 2 respectively in the diagram above (and therefore the boundary
triangle is face *p*[3] of the tetrahedron).

The arguments to this routine affect whether we examine the upper or
lower boundary and whether we examine the first or second triangle of
this boundary

Parameter ``whichBdry``:
    0 if the upper boundary should be examined, or 1 if the lower
    boundary should be examined.

Parameter ``whichTri``:
    0 if the first boundary triangle should be examined, or 1 if the
    second boundary triangle should be examined.

Returns:
    the permutation mapping roles 0, 1 and 2 in the diagram above to
    real tetrahedron vertex numbers.)doc";

// Docstring regina::python::doc::TxICore_::bdryTet
static const char *bdryTet =
R"doc(Determines which tetrahedron provides the requested boundary triangle.

Recall that the ``T x I`` triangulation has two torus boundaries, each
consisting of two boundary triangles. This routine returns the
specific tetrahedron that provides the given triangle of the given
torus boundary.

What is returned is the index number of the tetrahedron within the
triangulation. To access the tetrahedron itself, you may call
``core().tetrahedron(bdryTet(...))``.

Note that the same tetrahedron may provide more than one boundary
triangle.

Parameter ``whichBdry``:
    0 if the upper boundary should be examined, or 1 if the lower
    boundary should be examined.

Parameter ``whichTri``:
    0 if the first boundary triangle should be examined, or 1 if the
    second boundary triangle should be examined.)doc";

// Docstring regina::python::doc::TxICore_::core
static const char *core =
R"doc(Returns a full copy of the ``T x I`` triangulation that this object
describes.

Successive calls to this routine will return a reference to the same
triangulation (i.e., it is not recreated each time this function is
called).

Returns:
    the full triangulation.)doc";

// Docstring regina::python::doc::TxICore_::name
static const char *name =
R"doc(Returns the name of this specific triangulation of ``T x I`` as a
human-readable string.

Returns:
    the name of this triangulation.)doc";

// Docstring regina::python::doc::TxICore_::parallelReln
static const char *parallelReln =
R"doc(Returns a 2-by-2 matrix describing the parallel relationship between
the upper and lower boundary curves.

Let *a_u* and *b_u* be the upper α and β boundary curves. Suppose that
the lower α is parallel to *w*.*a_u* + *x*.*b_u*, and that the lower β
is parallel to *y*.*a_u* + *z*.*b_u*. Then the matrix returned will be

```
    [ w  x ]
    [      ] .
    [ y  z ]
```

In other words, if *a_l* and *b_l* are the lower α and β curves
respectively, we have

```
    [ a_l ]                      [ a_u ]
    [     ]  =  parallelReln() * [     ] .
    [ b_l ]                      [ b_u ]
```

Returns:
    the relationship between the upper and lower boundary curves.)doc";

// Docstring regina::python::doc::TxICore_::texName
static const char *texName =
R"doc(Returns the name of this specific triangulation of ``T x I`` in TeX
format. No leading or trailing dollar signs will be included.

Returns:
    the name of this triangulation in TeX format.)doc";

}

namespace TxIDiagonalCore_ {

// Docstring regina::python::doc::TxIDiagonalCore_::__copy
static const char *__copy = R"doc(Creates a new copy of the given ``T x I`` triangulation.)doc";

// Docstring regina::python::doc::TxIDiagonalCore_::__init
static const char *__init =
R"doc(Creates a new ``T x I`` triangulation with the given parameters.

Parameter ``size``:
    the number of tetrahedra in this triangulation. This must be at
    least 6.

Parameter ``k``:
    the additional parameter *k* as described in the class notes. This
    must be between 1 and (*size* - 5) inclusive.)doc";

// Docstring regina::python::doc::TxIDiagonalCore_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given ``T x I`` triangulations.

This global routine simply calls TxIDiagonalCore::swap(); it is
provided so that TxIDiagonalCore meets the C++ Swappable requirements.

See TxIDiagonalCore::swap() for more details.

Parameter ``lhs``:
    the triangulation whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the triangulation whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::TxIDiagonalCore_::k
static const char *k =
R"doc(Returns the additional parameter *k* as described in the class notes.

Returns:
    the additional parameter *k*.)doc";

// Docstring regina::python::doc::TxIDiagonalCore_::size
static const char *size =
R"doc(Returns the total number of tetrahedra in this ``T x I``
triangulation.

Returns:
    the total number of tetrahedra.)doc";

// Docstring regina::python::doc::TxIDiagonalCore_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given ``T x I`` triangulation.

Parameter ``other``:
    the triangulation whose contents should be swapped with this.)doc";

}

namespace TxIParallelCore_ {

// Docstring regina::python::doc::TxIParallelCore_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given ``T x I`` triangulation.

Since there is only one triangulation of this type, the copy
constructor will give the same end result as the default constructor
(but using a different algorithm).)doc";

// Docstring regina::python::doc::TxIParallelCore_::__default
static const char *__default = R"doc(Creates a new copy of this ``T x I`` triangulation.)doc";

// Docstring regina::python::doc::TxIParallelCore_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given ``T x I`` triangulations.

This global routine simply calls TxIParallelCore::swap(); it is
provided so that TxIParallelCore meets the C++ Swappable requirements.

See TxIParallelCore::swap() for more details.

Parameter ``lhs``:
    the triangulation whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the triangulation whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::TxIParallelCore_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given ``T x I`` triangulation.

Parameter ``other``:
    the triangulation whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

