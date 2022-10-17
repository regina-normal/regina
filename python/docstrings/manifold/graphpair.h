/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::GraphPair
static const char *GraphPair =
R"doc(Represents a closed graph manifold formed by joining two bounded
Seifert fibred spaces along a common torus.

Each Seifert fibred space must have just one boundary component,
corresponding to a puncture in the base orbifold (with no fibre-
reversing twist as one travels around this boundary).

The way in which the two spaces are joined is specified by a 2-by-2
matrix *M*. This matrix expresses the locations of the fibres and base
orbifold of the second Seifert fibred space in terms of the first.

More specifically, suppose that *f0* and *o0* are generators of the
common torus, where *f0* represents a directed fibre in the first
Seifert fibred space and *o0* represents the oriented boundary of the
corresponding base orbifold. Likewise, let *f1* and *o1* be generators
of the common torus representing a directed fibre and the base
orbifold of the second Seifert fibred space. Then the curves *f0*,
*o0*, *f1* and *o1* are related as follows:

```
    [f1]       [f0]
    [  ] = M * [  ]
    [o1]       [o0]
```

See the page on notation for Seifert fibred spaces for details on some
of the terminology used above.

The optional Manifold routine homology() is implemented, but the
optional routine construct() is not.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that GraphPair still requires a non-trivial (but constant
sized) amount of data to be copied even in a move operation.)doc";

namespace GraphPair_ {

// Docstring regina::python::doc::GraphPair_::__copy
static const char *__copy = R"doc(Creates a clone of the given graph manifold.)doc";

// Docstring regina::python::doc::GraphPair_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object contain precisely the
same presentations of the same graph manifold.

This routine does *not* test for homeomorphism. Instead it compares
the exact presentations, including the matching matrix and the
specific presentations of the bounded Seifert fibred spaces, and
determines whether or not these *presentations* are identical. If you
have two different presentations of the same graph manifold, they will
be treated as not equal by this routine.

Parameter ``compare``:
    the presentation with which this will be compared.

Returns:
    ``True`` if and only if this and the given object contain
    identical presentations of the same graph manifold.)doc";

// Docstring regina::python::doc::GraphPair_::__init
static const char *__init =
R"doc(Creates a new graph manifold as a pair of joined Seifert fibred
spaces. The two bounded Seifert fibred spaces and the four elements of
the 2-by-2 matching matrix are all passed separately. The elements of
the matching matrix combine to give the full matrix *M* as follows:

```
          [ mat00  mat01 ]
    M  =  [              ]
          [ mat10  mat11 ]
```

Precondition:
    The given matching matrix has determinant +1 or -1.

Exception ``InvalidArgument``:
    One of the given Seifert fibred spaces does not have precisely one
    torus boundary, corresponding to a single untwisted puncture in
    its base orbifold.

Parameter ``sfs0``:
    the first Seifert fibred space.

Parameter ``sfs1``:
    the second Seifert fibred space.

Parameter ``mat00``:
    the (0,0) element of the matching matrix.

Parameter ``mat01``:
    the (0,1) element of the matching matrix.

Parameter ``mat10``:
    the (1,0) element of the matching matrix.

Parameter ``mat11``:
    the (1,1) element of the matching matrix.)doc";

// Docstring regina::python::doc::GraphPair_::__init_2
static const char *__init_2 =
R"doc(Creates a new graph manifold from a pair of bounded Seifert fibred
spaces, which are moved instead of copied.

Other than its use of move semantics, this behaves identically to the
other constructor that takes the Seifert fibred spaces by const
reference. See that constructor for further details.

Precondition:
    The given matching matrix has determinant +1 or -1.

Exception ``InvalidArgument``:
    One of the given Seifert fibred spaces does not have precisely one
    torus boundary, corresponding to a single untwisted puncture in
    its base orbifold.

Parameter ``sfs0``:
    the first Seifert fibred space.

Parameter ``sfs1``:
    the second Seifert fibred space.

Parameter ``mat00``:
    the (0,0) element of the matching matrix.

Parameter ``mat01``:
    the (0,1) element of the matching matrix.

Parameter ``mat10``:
    the (1,0) element of the matching matrix.

Parameter ``mat11``:
    the (1,1) element of the matching matrix.)doc";

// Docstring regina::python::doc::GraphPair_::__init_3
static const char *__init_3 =
R"doc(Creates a new graph manifold as a pair of joined Seifert fibred
spaces. The two bounded Seifert fibred spaces and the entire 2-by-2
matching matrix are each passed separately.

Precondition:
    The given matching matrix has determinant +1 or -1.

Exception ``InvalidArgument``:
    One of the given Seifert fibred spaces does not have precisely one
    torus boundary, corresponding to a single untwisted puncture in
    its base orbifold.

Parameter ``sfs0``:
    the first Seifert fibred space.

Parameter ``sfs1``:
    the second Seifert fibred space.

Parameter ``matchingReln``:
    the 2-by-2 matching matrix.)doc";

// Docstring regina::python::doc::GraphPair_::__init_4
static const char *__init_4 =
R"doc(Creates a new graph manifold from a pair of bounded Seifert fibred
spaces, which are moved instead of copied.

Other than its use of move semantics, this behaves identically to the
other constructor that takes the Seifert fibred spaces by const
reference. See that constructor for further details.

Precondition:
    The given matching matrix has determinant +1 or -1.

Exception ``InvalidArgument``:
    One of the given Seifert fibred spaces does not have precisely one
    torus boundary, corresponding to a single untwisted puncture in
    its base orbifold.

Parameter ``sfs0``:
    the first Seifert fibred space.

Parameter ``sfs1``:
    the second Seifert fibred space.

Parameter ``matchingReln``:
    the 2-by-2 matching matrix.)doc";

// Docstring regina::python::doc::GraphPair_::__lt
static const char *__lt =
R"doc(Determines in a fairly ad-hoc fashion whether this representation of
this space is "smaller" than the given representation of the given
space.

The ordering imposed on graph manifolds is purely aesthetic on the
part of the author, and is subject to change in future versions of
Regina. It also depends upon the particular representation, so that
different representations of the same space may be ordered
differently.

All that this routine really offers is a well-defined way of ordering
graph manifold representations.

Parameter ``compare``:
    the representation with which this will be compared.

Returns:
    ``True`` if and only if this is "smaller" than the given graph
    manifold representation.)doc";

// Docstring regina::python::doc::GraphPair_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object do not contain precisely
the same presentations of the same graph manifold.

This routine does *not* test for homeomorphism. Instead it compares
the exact presentations, including the matching matrix and the
specific presentations of the bounded Seifert fibred spaces, and
determines whether or not these *presentations* are identical. If you
have two different presentations of the same graph manifold, they will
be treated as not equal by this routine.

Parameter ``compare``:
    the presentation with which this will be compared.

Returns:
    ``True`` if and only if this and the given object do not contain
    identical presentations of the same graph manifold.)doc";

// Docstring regina::python::doc::GraphPair_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given graph manifolds.

This global routine simply calls GraphPair::swap(); it is provided so
that GraphPair meets the C++ Swappable requirements.

Parameter ``a``:
    the first graph manifold whose contents should be swapped.

Parameter ``b``:
    the second graph manifold whose contents should be swapped.)doc";

// Docstring regina::python::doc::GraphPair_::matchingReln
static const char *matchingReln =
R"doc(Returns a reference to the 2-by-2 matrix describing how the two
Seifert fibred spaces are joined together. See the class notes for
details on precisely how this matrix is represented.

Returns:
    a reference to the matching matrix.)doc";

// Docstring regina::python::doc::GraphPair_::sfs
static const char *sfs =
R"doc(Returns a reference to one of the two bounded Seifert fibred spaces
that are joined together.

Parameter ``which``:
    0 if the first Seifert fibred space is to be returned, or 1 if the
    second space is to be returned.

Returns:
    a reference to the requested Seifert fibred space.)doc";

// Docstring regina::python::doc::GraphPair_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given graph manifold.

Parameter ``other``:
    the graph manifold whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

