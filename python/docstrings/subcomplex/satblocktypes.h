/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SatCube
static const char *SatCube =
R"doc(A saturated block that is a six-tetrahedron cube.

There are several ways of triangulating a cube with six tetrahedra;
the specific method used here is illustrated in the diagram below
(where the top face of the cube is identified with the bottom).

(Image available in HTML docs)

Note that none of the four tetrahedra that meet the boundary annuli
touch each other, and that each of these four boundary tetrahedra meet
both central tetrahedra. Note also that (unlike other triangulations)
this cube cannot be split vertically into two triangular prisms.

This class does not support value semantics: it cannot be copied,
swapped or manually constructed. Its memory is managed by the
SatRegion class (or for independently constructed models, the
SatBlockModel class), and blocks' locations in memory define them. See
SatRegion for further details.)doc";

// Docstring regina::python::doc::SatLST
static const char *SatLST =
R"doc(A saturated block that is a layered solid torus. See the
LayeredSolidTorus class for details.

The three boundary edges of the layered solid torus are attached to
the vertical, horizontal and diagonal edges of the boundary annulus;
see the SatAnnulus class notes for details on precisely what vertical,
horizontal and diagonal mean.

This class does not support value semantics: it cannot be copied,
swapped or manually constructed. Its memory is managed by the
SatRegion class, and blocks' locations in memory define them. See
SatRegion for further details.)doc";

// Docstring regina::python::doc::SatLayering
static const char *SatLayering =
R"doc(A degenerate saturated block that is a single tetrahedron wrapped
around so that two opposite edges touch. This forms a degenerate one-
tetrahedron solid torus that is pinched along a single meridinal
curve.

The four faces of this tetrahedron form two boundary annuli, and the
tetrahedron is effectively layered onto each boundary annulus. See the
Layering class notes for more discussion on layerings in general.

Although this block is degenerate (the fibres are all pinched together
where the opposite edges of the tetrahedron meet), it can be used
without problems as long as the entire Seifert fibred space is not
formed from degenerate blocks. In other words, using such blocks is
fine as long as they eventually meet a real (non-degenerate) block,
which will give room for the fibres to separate so that they are no
longer pinched together.

The SatAnnulus class notes describe horizontal and diagonal edges of a
saturated annulus. This block may be one of two types, according to
how the tetrahedron is layered onto the boundary annuli. Either the
tetrahedron can be layered over the horizontal edge of each annulus
(with the fibres pinched together between the two diagonal edges), or
the tetrahedron can be layered over the diagonal edge of each annulus
(with the fibres pinched together between the two horizontal edges).

This class does not support value semantics: it cannot be copied,
swapped or manually constructed. Its memory is managed by the
SatRegion class, and blocks' locations in memory define them. See
SatRegion for further details.)doc";

// Docstring regina::python::doc::SatMobius
static const char *SatMobius =
R"doc(A degenerate zero-tetrahedron saturated block that corresponds to
attaching a Mobius band to a single annulus boundary.

This is a degenerate case of the layered solid torus (see the class
SatLST), where instead of joining a solid torus to an annulus boundary
we join a Mobius band. The Mobius band can be thought of as a zero-
tetrahedron solid torus with two boundary triangles, which in fact are
opposite sides of the same triangle. By attaching a zero-tetrahedron
Mobius band to an annulus boundary, we are effectively joining the two
triangles of the annulus together.

The meridinal disc of this zero-tetrahedron solid torus meets the
three edges of the annulus in 1, 1 and 2 places, so it is in fact a
degenerate (1,1,2) layered solid torus. Note that the weight 2 edge is
the boundary edge of the Mobius strip.

This class does not support value semantics: it cannot be copied,
swapped or manually constructed. Its memory is managed by the
SatRegion class, and blocks' locations in memory define them. See
SatRegion for further details.)doc";

// Docstring regina::python::doc::SatReflectorStrip
static const char *SatReflectorStrip =
R"doc(A saturated block that is a reflector strip.

A reflector strip is a ring of triangular prisms arranged end-to-end,
as illustrated in the diagram below. The top rectangle of each prism
is identified with the bottom in an orientation-reversing fashion (the
back edge moves to the front and vice versa), and the prisms are
joined in a loop from left to right. The Seifert fibres run vertically
in the diagram, with each saturated boundary annulus shaded at the
rear of each prism.

(Image available in HTML docs)

The effect of a reflector strip is to create a reflector boundary in
the base orbifold of the surrounding Seifert fibred space. Each prism
provides a segment of this reflector boundary.

A reflector strip may have arbitrary length, and it may also include a
twist as the ring of prisms wraps back around to meet itself. Note
that a twisted reflector strip will have a twisted ring of boundary
annuli, as described by SatBlock::twistedBoundary().

The _length_ of a reflector strip is defined to be the number of
prisms that are joined together, or equivalently the number of
saturated annuli on the boundary.

This class does not support value semantics: it cannot be copied,
swapped or manually constructed. Its memory is managed by the
SatRegion class (or for independently constructed models, the
SatBlockModel class), and blocks' locations in memory define them. See
SatRegion for further details.)doc";

// Docstring regina::python::doc::SatTriPrism
static const char *SatTriPrism =
R"doc(A saturated block that is a three-tetrahedron triangular prism.

Such a prism may be of major type or of minor type. In a _major_ type
prism, the horizontal edges of the boundary annuli are all major
(degree three) edges of the prism. Likewise, in a _minor_ type prism,
the horizontal boundary edges are all minor (degree two) edges of the
prism. See the SatAnnulus class notes for a definition of "horizontal"
and the TriSolidTorus class notes for further details regarding
"major" and "minor".

This class does not support value semantics: it cannot be copied,
swapped or manually constructed. Its memory is managed by the
SatRegion class (or for independently constructed models, the
SatBlockModel class), and blocks' locations in memory define them. See
SatRegion for further details.)doc";

namespace SatCube_ {

// Docstring regina::python::doc::SatCube_::model
static const char *model =
R"doc(Creates a new model of a cube block.

Returns:
    a full triangulated model of this type of block.)doc";

}

namespace SatLST_ {

// Docstring regina::python::doc::SatLST_::lst
static const char *lst =
R"doc(Returns details of the layered solid torus that this block represents.

Returns:
    details of the layered solid torus.)doc";

// Docstring regina::python::doc::SatLST_::roles
static const char *roles =
R"doc(Describes how the layered solid torus is attached to the boundary
annulus.

The LayeredSolidTorus class notes describe top-level edge groups 0, 1
and 2 for a layered solid torus. On the other hand, the SatAnnulus
class notes define vertical, horizontal and diagonal edges on the
boundary annulus.

Suppose that the permutation returned by this routine is *r*. This
indicates that:

* edge group *r*[0] is attached to the vertical annulus edges;

* edge group *r*[1] is attached to the horizontal annulus edges;

* edge group *r*[2] is attached to the diagonal annulus edges.

Returns:
    a description of how the layered solid torus is attached to the
    boundary annulus.)doc";

}

namespace SatLayering_ {

// Docstring regina::python::doc::SatLayering_::overHorizontal
static const char *overHorizontal =
R"doc(Does this describe a layering over the horizontal edge of the boundary
annulus, or a layering over the diagonal edge?

See the SatAnnulus class notes for definitions of horizontal and
diagonal in this context.)doc";

}

namespace SatMobius_ {

// Docstring regina::python::doc::SatMobius_::position
static const char *position =
R"doc(Describes how the Mobius band is attached to the boundary annulus.

The class notes discuss the weight two edge of the Mobius band (or
equivalently the boundary edge of the Mobius band). The return value
of this routine indicates which edge of the boundary annulus this
weight two edge is joined to.

In the SatAnnulus class notes, the three edges of the annulus are
denoted vertical, horizontal and boundary, and the vertices of each
triangle are given markings 0, 1 and 2.

The return value of this routine takes the value 0, 1 or 2 as follows:

* 0 means that the weight two edge is joined to the diagonal edge of
  the annulus (markings 1 and 2);

* 1 means that the weight two edge is joined to the horizontal edge of
  the annulus (markings 0 and 2);

* 2 means that the weight two edge is joined to the vertical edge of
  the annulus (markings 0 and 1).

Returns:
    the value 0, 1 or 2 as described above.)doc";

}

namespace SatReflectorStrip_ {

// Docstring regina::python::doc::SatReflectorStrip_::model
static const char *model =
R"doc(Creates a new model of a reflector strip block.

Parameter ``length``:
    the length of the new reflector strip, i.e., the number of
    boundary annuli; this must be strictly positive.

Parameter ``twisted``:
    ``True`` if the new reflector strip should be twisted (causing its
    ring of boundary annuli to be twisted also), or ``False`` if the
    new strip should not be twisted.

Returns:
    a full triangulated model of this type of block.)doc";

}

namespace SatTriPrism_ {

// Docstring regina::python::doc::SatTriPrism_::isMajor
static const char *isMajor =
R"doc(Is this prism of major type or minor type? See the class notes for
further details.

Note that this routine cannot be called major(), since on some
compilers that name clashes with a macro for isolating major/minor
bytes.

Returns:
    ``True`` if this prism is of major type, or ``False`` if it is of
    minor type.)doc";

// Docstring regina::python::doc::SatTriPrism_::model
static const char *model =
R"doc(Creates a new model of a triangular prism block.

Parameter ``major``:
    ``True`` if a block of major type should be created, or ``False``
    if a block of minor type should be created.

Returns:
    a full triangulated model of this type of block.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

