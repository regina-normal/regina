/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::TorusBundle
static const char *TorusBundle =
R"doc(Represents a torus bundle over the circle. This is expressed as the
product of the torus and the interval, with the two torus boundaries
identified according to some specified monodromy.

The monodromy is described by a 2-by-2 matrix *M* as follows. Let *a*
and *b* be generating curves of the upper torus boundary, and let *p*
and *q* be the corresponding curves on the lower torus boundary (so
that *a* and *p* are parallel and *b* and *q* are parallel). Then we
identify the torus boundaries so that, in additive terms:

```
    [a]       [p]
    [ ] = M * [ ]
    [b]       [q]
```

All optional Manifold routines except for construct() are implemented
for this class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
manifold classes.)doc";

namespace TorusBundle_ {

// Docstring regina::python::doc::TorusBundle_::__copy
static const char *__copy = R"doc(Creates a new copy of the given torus bundle.)doc";

// Docstring regina::python::doc::TorusBundle_::__default
static const char *__default =
R"doc(Creates a new trivial torus bundle over the circle. In other words,
this routine creates a torus bundle with the identity monodromy.)doc";

// Docstring regina::python::doc::TorusBundle_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object contain precisely the
same presentations of the same torus bundle.

This routine does _not_ test for homeomorphism; instead it compares
the specific monodromies. If you have two objects that represent same
torus bundle using two different monodromies, they will be treated as
not equal by this routine.

Parameter ``compare``:
    the presentation with which this will be compared.

Returns:
    ``True`` if and only if this and the given object contain
    identical presentations of the same torus bundle.)doc";

// Docstring regina::python::doc::TorusBundle_::__init
static const char *__init =
R"doc(Creates a new torus bundle over the circle using the given monodromy.

Precondition:
    The given matrix has determinant +1 or -1.

Exception ``InvalidArgument``:
    The given monodromy does not have determinant ±1.

Parameter ``monodromy``:
    describes precisely how the upper and lower torus boundaries are
    identified. See the class notes for details.)doc";

// Docstring regina::python::doc::TorusBundle_::__init_2
static const char *__init_2 =
R"doc(Creates a new torus bundle over the circle using the given monodromy.
The four elements of the monodromy matrix are passed separately. They
combine to give the full monodromy matrix *M* as follows:

```
          [ mon00  mon01 ]
    M  =  [              ]
          [ mon10  mon11 ]
```

Precondition:
    The monodromy matrix formed from the given parameters has
    determinant +1 or -1.

Exception ``InvalidArgument``:
    The given monodromy does not have determinant ±1.

Parameter ``mon00``:
    the (0,0) element of the monodromy matrix.

Parameter ``mon01``:
    the (0,1) element of the monodromy matrix.

Parameter ``mon10``:
    the (1,0) element of the monodromy matrix.

Parameter ``mon11``:
    the (1,1) element of the monodromy matrix.)doc";

// Docstring regina::python::doc::TorusBundle_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given torus bundles.

This global routine simply calls TorusBundle::swap(); it is provided
so that TorusBundle meets the C++ Swappable requirements.

Parameter ``a``:
    the first torus bundle whose contents should be swapped.

Parameter ``b``:
    the second torus bundle whose contents should be swapped.)doc";

// Docstring regina::python::doc::TorusBundle_::monodromy
static const char *monodromy =
R"doc(Returns the monodromy describing how the upper and lower torus
boundaries are identified. See the class notes for details.

Returns:
    the monodromy for this torus bundle.)doc";

// Docstring regina::python::doc::TorusBundle_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given torus bundle.

Parameter ``other``:
    the torus bundle whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

