/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::PluggedTorusBundle
static const char *PluggedTorusBundle =
R"doc(Describes a triangulation of a graph manifold formed by joining a
bounded saturated region with a thin I-bundle over the torus, possibly
with layerings in between.

The thin I-bundle must be untwisted, so that it forms the product ``T
× I`` with two boundary tori. Moreover, it must be isomorphic to some
existing instance of the class TxICore.

The saturated region is described by an object of the class SatRegion.
This region must have precisely two boundary annuli. These may be two
separate torus boundaries (each formed from its own saturated
annulus). Alternatively, the saturated region may have a single
boundary formed from both saturated annuli, where this boundary is
pinched together so that each annulus becomes its own two-sided torus.

Either way, the saturated region effectively has two torus boundaries,
each formed from two triangles of the triangulation. These boundaries
are then joined to the two torus boundaries of the thin I-bundle,
possibly with layerings in between (for more detail on layerings, see
the Layering class). This is illustrated in the following diagram,
where the small tunnels show where the torus boundaries are joined
(possibly via layerings).

```
   /--------------------\     /-----------------\
   |                     -----                  |
   |                     -----                  |
   |  Saturated region  |     |  Thin I-bundle  |
   |                     -----                  |
   |                     -----                  |
   \--------------------/     \-----------------/
```

The effect of the thin I-bundle and the two layerings is essentially
to join the two boundaries of the saturated region according to some
non-trivial homeomorphism of the torus. This homeomorphism is
specified by a 2-by-2 matrix *M* as follows.

Suppose that *f0* and *o0* are directed curves on the first boundary
torus and *f1* and *o1* are directed curves on the second boundary
torus, where *f0* and *f1* represent the fibres of the saturated
region and *o0* and *o1* represent the base orbifold (see the page on
notation for Seifert fibred spaces for terminology). Then the torus
boundaries of the saturated region are identified by the thin I-bundle
and layerings according to the following relation:

```
    [f1]       [f0]
    [  ] = M * [  ]
    [o1]       [o0]
```

Note that the routines writeName() and writeTeXName() do _not_ offer
enough information to uniquely identify the triangulation, since this
essentially requires 2-dimensional assemblings of saturated blocks.
For more detail, writeTextLong() may be used instead.

The optional StandardTriangulation routine manifold() is implemented
for this class, but homology() is not.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that the only way to create objects of this class (aside from
copying or moving) is via the static member function recognise().)doc";

namespace PluggedTorusBundle_ {

// Docstring regina::python::doc::PluggedTorusBundle_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given structure. This will induce a deep
copy of *src*.

Parameter ``src``:
    the structure to copy.)doc";

// Docstring regina::python::doc::PluggedTorusBundle_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of plugged torus bundle.

Specifically, two structures will compare as equal if and only if:

* both structures use the same type of thin I-bundle with the same
  parameters (as tested by the TxICore comparison operators);

* both structures use saturated regions with the same combinatorial
  presentation (as tested by the SatRegion comparison operators);

* the layerings that connect the thin I-bundle and saturated region in
  each structure are the same (as tested by the Layering comparison
  operators), and use the same attaching matrices.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since this test does not
account for the many possible symmetries in a plugged torus bundle).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of plugged torus bundle.)doc";

// Docstring regina::python::doc::PluggedTorusBundle_::bundle
static const char *bundle =
R"doc(Returns an isomorphic copy of the thin I-bundle that forms part of
this triangulation. Like all objects of class TxICore, the thin
I-bundle that is returned is an external object with its own separate
triangulation of the product ``T × I``. For information on how the
thin I-bundle is embedded within this triangulation, see the routine
bundleIso().

Returns:
    the an isomorphic copy of the thin I-bundle within this
    triangulation.)doc";

// Docstring regina::python::doc::PluggedTorusBundle_::bundleIso
static const char *bundleIso =
R"doc(Returns an isomorphism describing how the thin I-bundle forms a
subcomplex of this triangulation.

The thin I-bundle returned by bundle() does not directly refer to
tetrahedra within this triangulation. Instead it contains its own
isomorphic copy of the thin I-bundle triangulation (as is usual for
objects of class TxICore).

The isomorphism returned by this routine is a mapping from the
triangulation bundle().core() to this triangulation, showing how the
thin I-bundle appears as a subcomplex of this structure.

Returns:
    an isomorphism from the thin I-bundle described by bundle() to the
    tetrahedra of this triangulation.)doc";

// Docstring regina::python::doc::PluggedTorusBundle_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls PluggedTorusBundle::swap(); it is
provided so that PluggedTorusBundle meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::PluggedTorusBundle_::matchingReln
static const char *matchingReln =
R"doc(Returns the matrix describing how the two torus boundaries of the
saturated region are joined by the thin I-bundle and layerings. See
the class notes above for details.

Returns:
    the matching relation between the two region boundaries.)doc";

// Docstring regina::python::doc::PluggedTorusBundle_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation is a saturated region joined to
a thin I-bundle via optional layerings, as described in the class
notes above.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``tri``:
    the triangulation to examine.

Returns:
    an object containing details of the structure that was found, or
    ``None`` if the given triangulation is not of the form described
    by this class.)doc";

// Docstring regina::python::doc::PluggedTorusBundle_::region
static const char *region =
R"doc(Returns the saturated region that forms part of this triangulation.
The region refers directly to tetrahedra within this triangulation (as
opposed to the thin I-bundle, which refers to a separate external
triangulation).

Returns:
    the saturated region.)doc";

// Docstring regina::python::doc::PluggedTorusBundle_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

