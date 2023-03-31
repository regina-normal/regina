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
R"doc(Offers routines for constructing a variety of sample 3-dimensional
triangulations.

This is a specialisation of the generic Example class template; see
the generic Example template documentation for a general overview of
how the example triangulation classes work. In Python, you can read
this generic documentation by looking at a higher dimension: try
``help(Example5)``.

This 3-dimensional specialisation offers significant extra
functionality, by providing several more hard-coded and parameterised
constructions.)doc";

namespace Example_ {

// Docstring regina::python::doc::Example_::augTriSolidTorus
static const char *augTriSolidTorus =
R"doc(Returns an augmented triangular solid torus with the given parameters.
Almost all augmented triangular solid tori represent Seifert fibred
spaces with three or fewer exceptional fibres. Augmented triangular
solid tori are described in more detail in the AugTriSolidTorus class
notes.

The resulting Seifert fibred space will be SFS((*a1*, *b1*), (*a2*,
*b2*), (*a3*, *b3*), (1, 1)), where the parameters *a1*, ..., *b3* are
passed as arguments to this routine. The three layered solid tori that
are attached to the central triangular solid torus will be LST(|*a1*|,
|*b1*|, |-*a1*-*b1*|), ..., LST(|*a3*|, |*b3*|, |-*a3*-*b3*|).

There are no sign constraints on the parameters; in particular,
negative arguments are allowed.

The new tetrahedra will be inserted at the end of the list of
tetrahedra in the triangulation.

Precondition:
    gcd(*a1*, *b1*) = gcd(*a2*, *b2*) = gcd(*a3*, *b3*) = 1.

Parameter ``a1``:
    a parameter describing the first layered solid torus in the
    augmented triangular solid torus.

Parameter ``b1``:
    a parameter describing the first layered solid torus in the
    augmented triangular solid torus.

Parameter ``a2``:
    a parameter describing the second layered solid torus in the
    augmented triangular solid torus.

Parameter ``b2``:
    a parameter describing the second layered solid torus in the
    augmented triangular solid torus.

Parameter ``a3``:
    a parameter describing the third layered solid torus in the
    augmented triangular solid torus.

Parameter ``b3``:
    a parameter describing the third layered solid torus in the
    augmented triangular solid torus.)doc";

// Docstring regina::python::doc::Example_::bingsHouse
static const char *bingsHouse =
R"doc(Returns the two-tetrahedron triangulation of the 3-sphere that is dual
to Bing's house with two rooms.

Returns:
    a 3-sphere triangulation dual to Bing's house.)doc";

// Docstring regina::python::doc::Example_::cuspedGenusTwoTorus
static const char *cuspedGenusTwoTorus =
R"doc(Returns a triangulation of a solid genus two torus with a cusped
boundary. This triangulation has one internal finite vertex and one
genus two ideal vertex.

Returns:
    the solid genus two torus with cusped boundary.)doc";

// Docstring regina::python::doc::Example_::figureEight
static const char *figureEight =
R"doc(Returns a two-tetrahedron ideal triangulation of the figure eight knot
complement.

Returns:
    the figure eight knot complement.)doc";

// Docstring regina::python::doc::Example_::gieseking
static const char *gieseking =
R"doc(Returns the one-tetrahedron ideal triangulation of the non-orientable
Gieseking manifold.

Returns:
    the Gieseking manifold.)doc";

// Docstring regina::python::doc::Example_::handlebody
static const char *handlebody =
R"doc(Returns a triangulation of the orientable handlebody with the given
genus.

For positive genus, this routine uses a minimal layered triangulation
of the orientable handlebody. This is constructed by starting with a
one-vertex triangulation of a once-punctured non-orientable surface
with the given genus, and layering a tetrahedron onto each internal
edge of this surface, yielding a (3*genus-2)-tetrahedron
triangulation. For genus greater than one, there are many choices for
how to do this; this routine makes an arbitrary choice.

For genus 0, this routine uses the one-tetrahedron 3-ball.

Parameter ``genus``:
    the genus of the handlebody.

Returns:
    the orientable handlebody with the given genus.

Author:
    Alex He)doc";

// Docstring regina::python::doc::Example_::layeredLoop
static const char *layeredLoop =
R"doc(Returns a layered loop of the given length. Layered loops are
described in detail in the LayeredLoop class notes.

Parameter ``length``:
    the length of the layered loop to construct; this must be strictly
    positive.

Parameter ``twisted``:
    ``True`` if the layered loop should be twisted, or ``False`` if it
    should be untwisted.

Returns:
    the resulting layered loop.)doc";

// Docstring regina::python::doc::Example_::lens
static const char *lens =
R"doc(Returns a triangulation of the lens space ``L(p,q)``.

The triangulation uses a layered lens space, which is conjectured (but
not proven in all cases) to be the triangulation requiring the fewest
tetrahedra. A layered lens space is constructed by building a layered
solid torus and then joining together the two boundary triangles.

Precondition:
    *p* > *q* ≥ 0 unless (*p*,*q*) = (0,1).

Precondition:
    gcd(*p*, *q*) = 1.

Parameter ``p``:
    a parameter of the desired lens space.

Parameter ``q``:
    a parameter of the desired lens space.

Returns:
    the lens space ``L(p,q)``.)doc";

// Docstring regina::python::doc::Example_::lst
static const char *lst =
R"doc(Returns the layered solid torus ``LST(a,b,c)``.

This is a parameterised triangulation of the solid torus. It has two
boundary triangles and three boundary edges, and the meridional disc
of the solid torus cuts these boundary edges *a*, *b* and *c* times
respectively.

Only the parameters *a* and *b* are passed as arguments to this
routine. The third parameter *c* will be deduced automatically as *c*
= (*a* + *b*).

Precondition:
    gcd(*a*, *b*) = 1.

Parameter ``a``:
    the first parameter of the layered solid torus.

Parameter ``b``:
    the second parameter of the layered solid torus.

Returns:
    the layered solid torus ``LST(a,b,c)``.)doc";

// Docstring regina::python::doc::Example_::poincare
static const char *poincare =
R"doc(Returns the five-tetrahedron triangulation of the Poincare homology
sphere.

Returns:
    the Poincare homology sphere.)doc";

// Docstring regina::python::doc::Example_::rp2xs1
static const char *rp2xs1 =
R"doc(Returns a three-tetrahedron triangulation of the non-orientable
product space ``RP² x S¹``.

Returns:
    the product space ``RP² x S¹``.)doc";

// Docstring regina::python::doc::Example_::rp3rp3
static const char *rp3rp3 =
R"doc(Returns a triangulation of the connected sum ``RP³ # RP³``.

Returns:
    the connected sum ``RP³ # RP³``.)doc";

// Docstring regina::python::doc::Example_::s2xs1
static const char *s2xs1 =
R"doc(Returns a two-tetrahedron triangulation of the product space ``S² x
S¹``. This is identical to calling the generic routine sphereBundle().

Returns:
    the product space ``S² x S¹``.)doc";

// Docstring regina::python::doc::Example_::sfsOverSphere
static const char *sfsOverSphere =
R"doc(Returns a triangulation of the given orientable Seifert fibred space
over the sphere with at most three exceptional fibres.

The Seifert fibred space will be SFS((*a1*, *b1*), (*a2*, *b2*),
(*a3*, *b3*)), where the parameters *a1*, ..., *b3* are passed as
arguments to this routine.

The three pairs of parameters (*a*, *b*) do not need to be normalised,
i.e., the parameters can be positive or negative and *b* may lie
outside the range [0..*a*). There is no separate twisting parameter;
each additional twist can be incorporated into the existing parameters
by replacing some pair (*a*>, *b*) with the pair (*a*, *a* + *b*). For
Seifert fibred spaces with less than three exceptional fibres, some or
all of the parameter pairs may be (1, *k*) or even (1, 0).

If you wish to construct more complex Seifert fibred spaces (e.g.,
with more exceptional fibres, or with a different base orbifold), you
can use the more sophisticated SFSpace::construct().

Precondition:
    None of *a1*, *a2* or *a3* are 0.

Precondition:
    gcd(*a1*, *b1*) = gcd(*a2*, *b2*) = gcd(*a3*, *b3*) = 1.

Parameter ``a1``:
    a parameter describing the first exceptional fibre.

Parameter ``b1``:
    a parameter describing the first exceptional fibre.

Parameter ``a2``:
    a parameter describing the second exceptional fibre.

Parameter ``b2``:
    a parameter describing the second exceptional fibre.

Parameter ``a3``:
    a parameter describing the third exceptional fibre.

Parameter ``b3``:
    a parameter describing the third exceptional fibre.

Returns:
    the triangulated Seifert fibred space.)doc";

// Docstring regina::python::doc::Example_::smallClosedNonOrblHyperbolic
static const char *smallClosedNonOrblHyperbolic =
R"doc(Returns the eleven-tetrahedron closed non-orientable hyperbolic
3-manifold with volume 2.02988321.

Returns:
    the closed non-orientable hyperbolic manifold described above.)doc";

// Docstring regina::python::doc::Example_::smallClosedOrblHyperbolic
static const char *smallClosedOrblHyperbolic =
R"doc(Returns the nine-tetrahedron closed orientable hyperbolic 3-manifold
with volume 0.94270736.

Returns:
    the closed orientable hyperbolic manifold described above.)doc";

// Docstring regina::python::doc::Example_::solidKleinBottle
static const char *solidKleinBottle =
R"doc(Returns a triangulation of the solid Klein bottle. This is isomorphic
to the triangulation returned by the generic routine
twistedBallBundle().

Returns:
    the solid Klein bottle.)doc";

// Docstring regina::python::doc::Example_::sphere600
static const char *sphere600 =
R"doc(Returns the boundary 3-sphere of the regular 600-cell. This is a
triangulation of the 3-sphere that is a simplicial complex, and in
which every edge has degree five.

The triangulation was extracted from the Benedetti-Lutz library of
triangulations. See: http://page.math.tu-
berlin.de/~lutz/stellar/library_of_triangulations.html

Returns:
    the boundary of the regular 600-cell.)doc";

// Docstring regina::python::doc::Example_::threeSphere
static const char *threeSphere =
R"doc(Returns a one-tetrahedron triangulation of the 3-sphere. This is
_different_ from the generic routine sphere(), which uses two
tetrahedra instead.

Returns:
    a one-tetrahedron 3-sphere.)doc";

// Docstring regina::python::doc::Example_::trefoil
static const char *trefoil =
R"doc(Returns a two-tetrahedron ideal triangulation of the trefoil knot
complement.

Returns:
    the trefoil knot complement.)doc";

// Docstring regina::python::doc::Example_::weberSeifert
static const char *weberSeifert =
R"doc(Returns a one-vertex triangulation of the Weber-Seifert dodecahedral
space.

This 3-manifold is described in "Die beiden Dodekaederraume", C. Weber
and H. Seifert, Math. Z. 37 (1933), no. 1, 237-253. The triangulation
returned by this routine (with 23 tetrahedra) is given in "The Weber-
Seifert dodecahedral space is non-Haken", Benjamin A. Burton, J. Hyam
Rubinstein and Stephan Tillmann, Trans. Amer. Math. Soc. 364:2 (2012),
pp. 911-932.

Returns:
    the Weber-Seifert dodecahedral space.)doc";

// Docstring regina::python::doc::Example_::weeks
static const char *weeks =
R"doc(Returns a nine-tetrahedron minimal triangulation of the Weeks
manifold. The Weeks manifold is the smallest-volume closed hyperbolic
3-manifold, with a volume of roughly 0.9427. Note that there are nine
minimal triangulations of the Weeks manifold (of course this routine
returns just one).

Returns:
    the Weeks manifold.)doc";

// Docstring regina::python::doc::Example_::whitehead
static const char *whitehead =
R"doc(Returns a four-tetrahedron ideal triangulation of the Whitehead link
complement.

Returns:
    the Whitehead link complement.)doc";

// Docstring regina::python::doc::Example_::whiteheadLink
static const char *whiteheadLink =
R"doc(Deprecated alias for whitehead(), which returns a four-tetrahedron
ideal triangulation of the Whitehead link complement.

.. deprecated::
    This routine has been renamed to whitehead().

Returns:
    the Whitehead link complement.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

