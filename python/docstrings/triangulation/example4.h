/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Example
static const char *Example =
R"doc(Offers routines for constructing a variety of sample 4-dimensional
triangulations.

This is a specialisation of the generic Example class template; see
the generic Example template documentation for a general overview of
how the example triangulation classes work. In Python, you can read
this generic documentation by looking at a higher dimension: try
``help(Example5)``.

This 4-dimensional specialisation offers significant extra
functionality, by providing several more hard-coded and parameterised
constructions.)doc";

namespace Example_ {

// Docstring regina::python::doc::Example_::boundarySpin
static const char *boundarySpin =
R"doc(Returns a triangulation of the given 3-manifold spun around its
boundary.

Let ``M`` be the given 3-manifold, with real boundary ``∂M``. This
constructs a 4-manifold from ``M`` as follows:

* First we build the product ``M × S1`` in a similar way to
  s1Bundle(). That is: we build a tetrahedral prism for each original
  tetrahedron of *M*, glue the top and bottom tetrahedra of each prism
  together, and glue the walls of the prisms together according to the
  gluings between the original tetrahedra of *M*.

* Then, for each point ``b`` on the real boundary ``∂M``, we collapse
  the fibre ``b × S1`` to a single point. (Equivalently, we attach a
  copy of ``∂M × D2`` to the product ``M × S1`` so that, for each
  point ``b`` on the boundary ``∂M``, the fibre ``b × S1`` becomes the
  boundary of the corresponding disc ``b × D2``.) We implement this as
  follows: for each boundary facet of *M*, we fold the corresponding
  prism wall in half, so that the top half folds onto the bottom.

* If one or more of boundary facets of *M* are locked, then the
  corresponding prism walls will _not_ folded onto themselves; that
  is, the fibres over those parts of the boundary will _not_ be
  collapsed. See the section below on locks for further details.

Regarding real versus ideal boundary:

* The second step (collapsing fibres) only acts on _real_ boundary;
  that is, points ``b`` that lie on boundary triangles of ``M``. It
  ignores ideal boundary, in the sense that ideal vertices will just
  be transformed as part of the product ``M × S1`` (the first step),
  without the subsequent collapse/filling operation.

* As a result, any ideal vertices of ``M`` will produce invalid edges
  in the resulting 4-maifold triangulation (i.e., edges whose links
  are the same surfaces as the links of the original ideal vertices of
  ``M``).

Like s1Bundle(), the resulting triangulation will be very large: it
creates 82 pentachora for each original tetrahedron of *M*. It is
highly recommended that you call Triangulation<4>::simplify()
afterwards if you do not need to preserve the combinatorial structure.

This routine handles locks as follows:

* For any simplex in *base* that is locked, all of the pentachora and
  internal facets of the corresponding prism will also be locked, as
  well as the two tetrahedra at each end of the prism (which will be
  glued together, as explained above).

* For any internal triangular facet of *base* that is locked, all of
  the tetrahedral facets on the corresponding prism wall(s) will be
  locked.

* For any boundary facet of *base* that is locked, the corresponding
  prism wall will _not_ be folded onto itself (i.e, the corresponding
  fibres will _not_ be collapsed). Instead, all of the tetrahedral
  facets on that prism wall will be locked, and will remain as
  boundary facets of the final 4-dimensional triangulation.

* If *base* has a mix of locked and unlocked boundary facets, you
  should aim to ensure that the locked and unlocked regions are
  separated by embedded curves in the boundary of *M* (i.e., there are
  no "pinch points" where the local picture has two or more locked
  regions meeting two or more unlocked regions). Otherwise you may
  find that these pinch points create invalid edges in the resulting
  4-manifold triangulation (specifically, edges whose links are
  2-spheres with two or more punctures).

Note that the current construction does _not_ give an oriented
triangulation (due to the specific choice of labelling); this may
change in a future version of Regina.

Parameter ``base``:
    the 3-manifold triangulation *M*, as described above.

Returns:
    the 4-manifold obtained by spinning *M* around its boundary, as
    described above.)doc";

// Docstring regina::python::doc::Example_::bundleWithMonodromy
static const char *bundleWithMonodromy =
R"doc(Returns a bundle formed from a given 3-manifold and a given monodromy.

Specifically, let *M* be the given 3-manifold triangulation. This
routine builds the bundle ``M × I``, and then identifies the two
copies of *M* on the boundary according to the given homeomorphism
from *M* to itself. The homeomorphism must be expressed as a
combinatorial automorphism, which means that for a non-trivial
monodromy you may need to do some work to find a sufficiently
symmetric 3-manifold triangulation to begin with.

The product is created as follows. For each original tetrahedron of
*M*, we build a tetrahedral prism containing 82 pentachora. We then
glue these prisms together in a manner that follows the gluings of the
original tetrahedra. Moreover, we take the two copies of *M* that are
formed from the tetrahedra at the two ends of these prisms, and glue
these together according to the given monodromy. It is highly
recommended that you call Triangulation<4>::simplify() afterwards if
you do not need to preserve the combinatorial structure.

For any simplex in *base* that is locked, all of the pentachora and
internal facets of the corresponding prism will also be locked. For
any triangular facet of *base* that is locked, all of the tetrahedral
facets on the corresponding prism wall(s) will likewise be locked. The
two tetrahedra at the ends of each prism will _not_ be locked (these
are the tetrahedra along which the two copies of *M* are identified
using the given monodromy).

Note that the current construction does _not_ give an oriented
triangulation (due to the specific choice of labelling); this may
change in a future version of Regina.

Precondition:
    The given monodromy must be an isomorphism from *M* to itself;
    that is, a combinatorial automorphism.

.. warning::
    If the given 3-manifold triangulation has ideal boundary, then you
    will obtain an invalid 4-manifold triangulation as a result.

Parameter ``base``:
    the 3-manifold triangulation *M*, as described above.

Parameter ``monodromy``:
    the homeomorphism from *M* to itself, as described above.

Returns:
    the requested bundle.)doc";

// Docstring regina::python::doc::Example_::cappellShaneson
static const char *cappellShaneson =
R"doc(Returns a two-pentachoron triangulation of a Cappell-Shaneson 2-knot
complement in the 4-sphere. This triangulation is described and
analysed in "Triangulating a Cappell-Shaneson knot complement",
Budney, Burton and Hillman, Mathematical Research Letters 19 (2012),
no. 5, 1117-1126.

Returns:
    a Cappell-Shaneson 2-knot complement.)doc";

// Docstring regina::python::doc::Example_::cp2
static const char *cp2 =
R"doc(Returns a four-pentachoron triangulation of the standard complex
projective plane. This triangulation is minimal.

Under the orientation convention that we use for intersection forms,
this triangulation gives the "plain" ``CP²`` with intersection form
[1], not the reflected ``CP²`` with intersection form [-1].

Returns:
    the standard complex projective plane.)doc";

// Docstring regina::python::doc::Example_::fourSphere
static const char *fourSphere =
R"doc(Returns a two-pentachoron triangulation of the 4-sphere. This is
identical to calling the generic routine sphere().

Returns:
    a two-pentachoron 4-sphere.)doc";

// Docstring regina::python::doc::Example_::fourTorus
static const char *fourTorus =
R"doc(Returns a triangulation of the standard 4-torus; that is, the product
space ``T² × T²``.

Returns:
    the product space ``T² × T²``.)doc";

// Docstring regina::python::doc::Example_::iBundle
static const char *iBundle =
R"doc(Returns a triangulation of the product ``M × I``, where *M* is the
given 3-manifold triangulation.

The boundary of this product will consist of two copies of *M*, both
combinatorially isomorphic to the original triangulation. If *n* is
the number of tetrahedra in *M*, then the first copy of *M* on the
boundary is obtained by mapping vertices 0,1,2,3 of tetrahedron *i* of
*M* to vertices 0,1,2,3 of pentachoron *i*, and the second copy is
obtained by mapping vertices 0,1,2,3 of tetrahedron *i* of *M* to
vertices 0,1,2,3 of pentachoron *n*+i.

The product is created as follows: for each original tetrahedron of
*M* we build a tetrahedral prism containing 82 pentachora, and we then
glue these prisms together in a manner that follows the gluings of the
original tetrahedra. It is highly recommended that you call
Triangulation<4>::simplify() afterwards if you do not need to preserve
the combinatorial structure.

For any simplex in *base* that is locked, all of the pentachora and
internal facets of the corresponding prism will also be locked. For
any triangular facet of *base* that is locked, all of the tetrahedral
facets on the corresponding prism wall(s) will likewise be locked.

Note that the current construction does _not_ give an oriented
triangulation (due to the specific choice of labelling); this may
change in a future version of Regina.

.. warning::
    If the given 3-manifold triangulation has ideal boundary, then you
    will obtain an invalid 4-manifold triangulation as a result.

Parameter ``base``:
    the 3-manifold triangulation *M*, as described above.

Returns:
    the product ``M × I``.)doc";

// Docstring regina::python::doc::Example_::k3
static const char *k3 =
R"doc(Returns a triangulation of the standard K3 surface.

Be warned: this triangulation is fairly large.

Returns:
    the K3 surface.)doc";

// Docstring regina::python::doc::Example_::rp4
static const char *rp4 =
R"doc(Returns a four-pentachoron triangulation of real projective 4-space.

Returns:
    real projective 4-space.)doc";

// Docstring regina::python::doc::Example_::s1Bundle
static const char *s1Bundle =
R"doc(Returns a triangulation of the product ``M × S1``, where *M* is the
given 3-manifold triangulation. This simply calls iBundle() and then
glues together the two copies of *M* on the boundary.

The product is created as follows. For each original tetrahedron of
*M*, we build a tetrahedral prism containing 82 pentachora. We then
glue these prisms together in a manner that follows the gluings of the
original tetrahedra, and we also glue together the two tetrahedra at
each end of every prism. It is highly recommended that you call
Triangulation<4>::simplify() afterwards if you do not need to preserve
the combinatorial structure.

For any simplex in *base* that is locked, all of the pentachora and
internal facets of the corresponding prism will also be locked, as
well as the two tetrahedra at each end of the prism (which will be
glued together, as explained above). For any triangular facet of
*base* that is locked, all of the tetrahedral facets on the
corresponding prism wall(s) will likewise be locked.

Note that the current construction does _not_ give an oriented
triangulation (due to the specific choice of labelling); this may
change in a future version of Regina.

.. warning::
    If the given 3-manifold triangulation has ideal boundary, then you
    will obtain an invalid 4-manifold triangulation as a result.

Parameter ``base``:
    the 3-manifold triangulation *M*, as described above.

Returns:
    the product ``M × S1``.)doc";

// Docstring regina::python::doc::Example_::s2xs2
static const char *s2xs2 =
R"doc(Returns a six-pentachoron triangulation of the standard product ``S² ×
S²``. This triangulation is minimal.

Returns:
    the standard product of two 2-spheres.)doc";

// Docstring regina::python::doc::Example_::s2xs2Twisted
static const char *s2xs2Twisted =
R"doc(Returns a six-pentachoron triangulation of the twisted product ``S² x~
S²``. This manifold is diffeomorphic to ``CP² # -CP²``, where ``-CP²``
denotes ``CP²`` with its orientation reversed. This triangulation is
minimal.

Returns:
    the twisted product of two 2-spheres.)doc";

// Docstring regina::python::doc::Example_::s3xs1
static const char *s3xs1 =
R"doc(Returns a two-pentachoron triangulation of the product space ``S³ ×
S¹``. This is identical to calling the generic routine sphereBundle().

Returns:
    the product ``S³ × S¹``.)doc";

// Docstring regina::python::doc::Example_::s3xs1Twisted
static const char *s3xs1Twisted =
R"doc(Returns a two-pentachoron triangulation of the twisted product space
``S³ x~ S¹``. This is identical to calling the generic routine
twistedSphereBundle().

Returns:
    the twisted product ``S³ x~ S¹``.)doc";

// Docstring regina::python::doc::Example_::simplicialFourSphere
static const char *simplicialFourSphere =
R"doc(Returns the standard six-pentachoron triangulation of the 4-sphere as
the boundary of a 5-simplex. This is identical to calling the generic
routine simplicialSphere().

Returns:
    the standard simplicial 4-sphere.)doc";

// Docstring regina::python::doc::Example_::spun
static const char *spun =
R"doc(Returns an ideal triangulation of the complement of the 2-knot
obtained by spinning the given 1-knot (without twisting). The knot to
be spun is passed as the first argument; the second (optional)
argument allows you to specify where the knot should be broken open
when carrying out the spinning construction.

The spinning construction is described by Artin in "Zur Isotopie
zweidimensionaler Flächen im R_4", Abh. Math. Sem. Univ. Hamburg 4
(1925), no. 1, 174-177.

The final triangulation might (or might not) still contain internal
vertices, in addition to the one ideal vertex that represents the
2-knot itself.

.. warning::
    This routine could be slow, even when *knot* has very few
    crossings. This is because it typically goes via intermediate
    triangulations with thousands or even tens of thousands of
    pentachora, and simplifying such triangulations takes time.

Precondition:
    The argument *knot* is a classical knot diagram. That is, the link
    diagram is not virtual, and has exactly one link component.

Exception ``FailedPrecondition``:
    The given link diagram is empty, has multiple components, and/or
    is virtual (as opposed to classical).

Parameter ``knot``:
    the knot to be spun.

Parameter ``breakOpen``:
    indicates where to break open the given knot diagram when
    performing the spinning construction. See the StrandRef
    documentation for the convention on how arcs are represented using
    StrandRef objects. This may be a null reference (the default), in
    which case this routine will choose an arbitrary location to break
    the knot open.

Returns:
    an ideal triangulation of the resulting 2-knot.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

