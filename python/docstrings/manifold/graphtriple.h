/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::GraphTriple
static const char *GraphTriple =
R"doc(Represents a closed graph manifold formed by joining three bounded
Seifert fibred spaces along their torus boundaries.

There must be one Seifert fibred space at either end, each with a
single torus boundary (corresponding to a single puncture in the base
orbifold, with no fibre-reversing twist around this puncture). Each of
these end spaces is joined to the space in the centre, which has two
disjoint torus boundaries (corresponding to two punctures in the base
orbifold, again with no fibre-reversing twists around these
punctures).

This configuration is illustrated in the diagram below. The large
boxes represent the bounded Seifert fibred spaces, and the small
tunnels show how their boundaries are joined.

```
    /---------------\   /-----------------\   /---------------\
    |               |   |                 |   |               |
    |  End space 0   ---   Central space   ---   End space 1  |
    |                ---                   ---                |
    |               |   |                 |   |               |
    \---------------/   \-----------------/   \---------------/
```

The way in which each pair of spaces is joined is specified by a
2-by-2 matrix. This matrix expresses the locations of the fibres and
base orbifold of the corresponding end space in terms of the central
space. **Note that these are not the same matrices that appear in the
manifold name in the census data files!** See the warning below.

More specifically, consider the matrix *M* that describes the joining
of the central space and the first end space (marked above as end
space 0). Suppose that *f* and *o* are generators of the common
boundary torus, where *f* represents a directed fibre in the central
space and *o* represents the oriented boundary of the corresponding
base orbifold. Likewise, let *f0* and *o0* be generators of the common
boundary torus representing a directed fibre and the base orbifold of
the first end space. Then the curves *f*, *o*, *f0* and *o0* are
related as follows:

```
    [f0]       [f ]
    [  ] = M * [  ]
    [o0]       [o ]
```

Likewise, let matrix *M*' describe the joining of the central space
and the second end space (marked in the diagram above as end space 1).
Let *f*' and *o*' be curves on the common boundary torus representing
the fibres and the base orbifold of the central space, and let *f1*
and *o1* be curves on this same torus representing the fibres and the
base orbifold of the second end space. Then the curves *f*', *o*',
*f1* and *o1* are related as follows:

```
    [f1]        [f']
    [  ] = M' * [  ]
    [o1]        [o']
```

See the page on notation for Seifert fibred spaces for details on some
of the terminology used above.

The optional Manifold routine homology() is implemented, but the
optional routine construct() is not.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that GraphTriple still requires a non-trivial (but constant
sized) amount of data to be copied even in a move operation.

.. warning::
    The 2-by-2 matrices used in this class are _not_ the same matrices
    that appear in the manifold name returned by name() and texName()
    and seen in the census data files. The matrices used in this class
    work from the inside out, describing the boundary torus on each
    end space in terms of a boundary torus on the central space. The
    matrices used in the manifold name work from left to right in the
    diagram above, describing a boundary torus on the central space or
    rightmost end space in terms of a boundary torus on the leftmost
    end space or central space respectively. The upshot of all this is
    that **the first matrix becomes inverted** (and the second matrix
    remains unchanged). It is likely that future versions of Regina
    will replace this class with a more general class that (amongst
    other things) removes this inconsistency.)doc";

namespace GraphTriple_ {

// Docstring regina::python::doc::GraphTriple_::__copy
static const char *__copy = R"doc(Creates a clone of the given graph manifold.)doc";

// Docstring regina::python::doc::GraphTriple_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object contain precisely the
same presentations of the same graph manifold.

This routine does _not_ test for homeomorphism. Instead it compares
the exact presentations, including the matching matrices and the
specific presentations of the bounded Seifert fibred spaces, and
determines whether or not these _presentations_ are identical. If you
have two different presentations of the same graph manifold, they will
be treated as not equal by this routine.

Parameter ``compare``:
    the presentation with which this will be compared.

Returns:
    ``True`` if and only if this and the given object contain
    identical presentations of the same graph manifold.)doc";

// Docstring regina::python::doc::GraphTriple_::__init
static const char *__init =
R"doc(Creates a new graph manifold from three bounded Seifert fibred spaces,
as described in the class notes. The three Seifert fibred spaces and
both 2-by-2 matching matrices are passed separately.

Precondition:
    Each of the given matrices has determinant +1 or -1.

Exception ``InvalidArgument``:
    One of the spaces *end0* and *end1* does not have precisely one
    torus boundary corresponding to a single untwisted puncture in its
    base orbifold, and/or the space *centre* does not have precisely
    two disjoint torus boundaries corresponding to two untwisted
    punctures in its base orbifold.

Parameter ``end0``:
    the first end space, as described in the class notes.

Parameter ``centre``:
    the central space, as described in the class notes.

Parameter ``end1``:
    the second end space, as described in the class notes.

Parameter ``matchingReln0``:
    the 2-by-2 matching matrix that specifies how spaces *end0* and
    *centre* are joined.

Parameter ``matchingReln1``:
    the 2-by-2 matching matrix that specifies how spaces *end1* and
    *centre* are joined.)doc";

// Docstring regina::python::doc::GraphTriple_::__init_2
static const char *__init_2 =
R"doc(Creates a new graph manifold from three bounded Seifert fibred spaces,
which are moved instead of copied.

Other than its use of move semantics, this behaves identically to the
other constructor that takes the Seifert fibred spaces by const
reference.

Precondition:
    Each of the given matrices has determinant +1 or -1.

Exception ``InvalidArgument``:
    One of the spaces *end0* and *end1* does not have precisely one
    torus boundary corresponding to a single untwisted puncture in its
    base orbifold, and/or the space *centre* does not have precisely
    two disjoint torus boundaries corresponding to two untwisted
    punctures in its base orbifold.

Parameter ``end0``:
    the first end space, as described in the class notes.

Parameter ``centre``:
    the central space, as described in the class notes.

Parameter ``end1``:
    the second end space, as described in the class notes.

Parameter ``matchingReln0``:
    the 2-by-2 matching matrix that specifies how spaces *end0* and
    *centre* are joined.

Parameter ``matchingReln1``:
    the 2-by-2 matching matrix that specifies how spaces *end1* and
    *centre* are joined.)doc";

// Docstring regina::python::doc::GraphTriple_::__lt
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

// Docstring regina::python::doc::GraphTriple_::centre
static const char *centre =
R"doc(Returns a reference to the central space. This is the Seifert fibred
space with two boundary components, to which the two end spaces are
joined. See the class notes for further discussion.

Returns:
    a reference to the requested Seifert fibred space.)doc";

// Docstring regina::python::doc::GraphTriple_::end
static const char *end =
R"doc(Returns a reference to one of the two end spaces. These are the
Seifert fibred spaces with just one boundary component, to be joined
to the central space. See the class notes for further discussion.

Parameter ``which``:
    0 if the first end space is to be returned, or 1 if the second end
    space is to be returned.

Returns:
    a reference to the requested Seifert fibred space.)doc";

// Docstring regina::python::doc::GraphTriple_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given graph manifolds.

This global routine simply calls GraphTriple::swap(); it is provided
so that GraphTriple meets the C++ Swappable requirements.

Parameter ``a``:
    the first graph manifold whose contents should be swapped.

Parameter ``b``:
    the second graph manifold whose contents should be swapped.)doc";

// Docstring regina::python::doc::GraphTriple_::matchingReln
static const char *matchingReln =
R"doc(Returns a reference to the 2-by-2 matrix describing how the two
requested bounded Seifert fibred spaces are joined together. See the
class notes for details on precisely how these matrices are
represented.

The argument *which* indicates which particular join should be
examined. A value of 0 denotes the join between the central space and
the first end space (corresponding to matrix *M* in the class notes),
whereas a value of 1 denotes the join between the central space and
the second end space (corresponding to matrix *M*' in the class
notes).

Parameter ``which``:
    indicates which particular join should be examined; this should be
    0 or 1 as described above.

Returns:
    a reference to the requested matching matrix.)doc";

// Docstring regina::python::doc::GraphTriple_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given graph manifold.

Parameter ``other``:
    the graph manifold whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

