/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::LayeredTorusBundle
static const char *LayeredTorusBundle =
R"doc(Describes a layered torus bundle. This is a triangulation of a torus
bundle over the circle formed as follows.

We begin with a thin I-bundle over the torus, i.e,. a triangulation of
the product ``T x I`` that is only one tetrahedron thick. This is
referred to as the *core*, and is described by an object of type
TxICore.

We then identify the upper and lower torus boundaries of this core
according to some homeomorphism of the torus. This may be impossible
due to incompatible boundary edges, and so we allow a layering of
tetrahedra over one of the boundari tori in order to adjust the
boundary edges accordingly. Layerings are described in more detail in
the Layering class.

Given the parameters of the core ``T x I`` and the specific layering,
the monodromy for this torus bundle over the circle can be calculated.
The manifold() routine returns details of the corresponding
3-manifold.

All optional StandardTriangulation routines are implemented for this
class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace LayeredTorusBundle_ {

// Docstring regina::python::doc::LayeredTorusBundle_::__copy
static const char *__copy = R"doc(Creates a new copy of this structure.)doc";

// Docstring regina::python::doc::LayeredTorusBundle_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of layered torus bundle.

Specifically, two layered torus bundles will compare as equal if and
only if their core ``T x I`` triangulations have the same
combinatorial parameters, and their layering relations are the same.

In particular, if you invert a layered torus bundle (which means the
layering relation becomes its inverse matrix), the resulting layered
torus bundle will generally *not* compare as equal.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since this test does not
recognise inversion and also does not recognise symmetries within the
``T x I`` core).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of layered torus bundle.)doc";

// Docstring regina::python::doc::LayeredTorusBundle_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure represent different
types of layered torus bundle.

Specifically, two layered torus bundles will compare as equal if and
only if their core ``T x I`` triangulations have the same
combinatorial parameters, and their layering relations are the same.

In particular, if you invert a layered torus bundle (which means the
layering relation becomes its inverse matrix), the resulting layered
torus bundle will generally *not* compare as equal.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since this test does not
recognise inversion and also does not recognise symmetries within the
``T x I`` core).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent
    different types of layered torus bundle.)doc";

// Docstring regina::python::doc::LayeredTorusBundle_::core
static const char *core =
R"doc(Returns the ``T x I`` triangulation at the core of this layered torus
bundle. This is the product ``T x I`` whose boundaries are joined
(possibly via some layering of tetrahedra).

Note that the triangulation returned by TxICore::core() (that is,
LayeredTorusBundle::core().core()) may well use different tetrahedron
and vertex numbers. That is, an isomorphic copy of it appears within
this layered surface bundle but the individual tetrahedra and vertices
may have been permuted. For a precise mapping from the TxICore::core()
triangulation to this triangulation, see the routine coreIso().

Returns:
    the core ``T x I`` triangulation.)doc";

// Docstring regina::python::doc::LayeredTorusBundle_::coreIso
static const char *coreIso =
R"doc(Returns the isomorphism describing how the core ``T x I`` appears as a
subcomplex of this layered torus bundle.

As described in the core() notes, the core ``T x I`` triangulation
returned by TxICore::core() appears within this layered torus bundle,
but not necessarily with the same tetrahedron or vertex numbers.

This routine returns an isomorphism that maps the tetrahedra and
vertices of the core ``T x I`` triangulation (as returned by
LayeredTorusBundle::core().core()) to the tetrahedra and vertices of
this overall layered torus bundle.

Returns:
    the isomorphism from the core ``T x I`` to this layered torus
    bundle.)doc";

// Docstring regina::python::doc::LayeredTorusBundle_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls LayeredTorusBundle::swap(); it is
provided so that LayeredTorusBundle meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::LayeredTorusBundle_::layeringReln
static const char *layeringReln =
R"doc(Returns a 2-by-2 matrix describing how the layering of tetrahedra
relates curves on the two torus boundaries of the core ``T x I``.

The TxICore class documentation describes generating *alpha* and
*beta* curves on the two torus boundaries of the core ``T x I`` (which
are referred to as the *upper* and *lower* boundaries). The two
boundary tori are parallel in two directions: through the core, and
through the layering. It is desirable to know the parallel
relationship between the two sets of boundary curves in each
direction.

The relationship through the core is already described by
TxICore::parallelReln(). This routine describes the relationship
through the layering.

Let *a_u* and *b_u* be the *alpha* and *beta* curves on the upper
boundary torus, and let *a_l* and *b_l* be the *alpha* and *beta*
curves on the lower boundary torus. Suppose that the upper *alpha* is
parallel to *w*.*a_l* + *x*.*b_l*, and that the upper *beta* is
parallel to *y*.*a_l* + *z*.*b_l*. Then the matrix returned will be

```
    [ w  x ]
    [      ] .
    [ y  z ]
```

In other words,

```
    [ a_u ]                      [ a_l ]
    [     ]  =  layeringReln() * [     ] .
    [ b_u ]                      [ b_l ]
```

It can be observed that this matrix expresses the upper boundary
curves in terms of the lower, whereas TxICore::parallelReln()
expresses the lower boundary curves in terms of the upper. This means
that the monodromy describing the overall torus bundle over the circle
can be calculated as

```
    M  =  layeringReln() * core().parallelReln()
```

or alternatively using the similar matrix

```
    M'  =  core().parallelReln() * layeringReln() .
```

Note that in the degenerate case where there is no layering at all,
this matrix is still perfectly well defined; in this case it describes
a direct identification between the upper and lower boundary tori.

Returns:
    the relationship through the layering between the upper and lower
    boundary curves of the core ``T x I``.)doc";

// Docstring regina::python::doc::LayeredTorusBundle_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation is a layered torus bundle.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``tri``:
    the triangulation to examine.

Returns:
    a structure containing details of the layered torus bundle, or
    ``null`` if the given triangulation is not a layered torus bundle.)doc";

// Docstring regina::python::doc::LayeredTorusBundle_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

