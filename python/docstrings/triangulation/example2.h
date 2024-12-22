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
R"doc(Offers routines for constructing a variety of sample 2-dimensional
triangulations.

This is a specialisation of the generic Example class template; see
the generic Example template documentation for a general overview of
how the example triangulation classes work. In Python, you can read
this generic documentation by looking at a higher dimension: try
``help(Example5)``.

This 2-dimensional specialisation offers significant extra
functionality, by providing several more hard-coded constructions.)doc";

namespace Example_ {

// Docstring regina::python::doc::Example_::annulus
static const char *annulus =
R"doc(Returns a two-triangle annulus. This is identical to the triangulation
returned by the generic routine ballBundle().

Returns:
    the annulus.)doc";

// Docstring regina::python::doc::Example_::disc
static const char *disc =
R"doc(Returns a one-triangle disc. This is identical to the triangulation
returned by the generic routine ball().

Returns:
    the disc.)doc";

// Docstring regina::python::doc::Example_::kb
static const char *kb =
R"doc(Returns a two-triangle Klein bottle. This is identical to the
triangulation returned by the generic routine twistedSphereBundle().

Returns:
    the Klein bottle.)doc";

// Docstring regina::python::doc::Example_::mobius
static const char *mobius =
R"doc(Returns a one-triangle Mobius band. This is identical to the
triangulation returned by the generic routine twistedBallBundle().

Returns:
    the Mobius band.)doc";

// Docstring regina::python::doc::Example_::nonOrientable
static const char *nonOrientable =
R"doc(Returns a triangulation of the given non-orientable surface.

If the number of punctures is 0 or 1, then the resulting triangulation
will be minimal (which, with the exception of the projective plane,
means there is exactly one vertex).

Parameter ``genus``:
    the non-orientable genus of the surface, i.e., the number of
    crosscaps that it contains; this must be greater than or equal to
    one.

Parameter ``punctures``:
    the number of punctures in the surface; this must be greater than
    or equal to zero.

Returns:
    the requested non-orientable surface.

Author:
    Alex He, B.B.)doc";

// Docstring regina::python::doc::Example_::orientable
static const char *orientable =
R"doc(Returns a triangulation of the given orientable surface.

If the number of punctures is 0, then the resulting triangulation will
be minimal (which, for positive genus, means there is exactly one
vertex).

Parameter ``genus``:
    the genus of the surface; this must be greater than or equal to
    zero.

Parameter ``punctures``:
    the number of punctures in the surface; this must be greater than
    or equal to zero.

Returns:
    the requested orientable surface.)doc";

// Docstring regina::python::doc::Example_::rp2
static const char *rp2 =
R"doc(Returns a two-triangle projective plane.

Returns:
    the projective plane.)doc";

// Docstring regina::python::doc::Example_::sphereOctahedron
static const char *sphereOctahedron =
R"doc(Returns the eight-triangle 2-sphere formed from the boundary of an
octahedron.

Returns:
    the octahedral sphere.)doc";

// Docstring regina::python::doc::Example_::sphereTetrahedron
static const char *sphereTetrahedron =
R"doc(Returns the four-triangle 2-sphere formed from the boundary of a
tetrahedron. This is identical to the triangulation returned by the
generic routine simplicialSphere().

Returns:
    the tetrahedral sphere.)doc";

// Docstring regina::python::doc::Example_::torus
static const char *torus =
R"doc(Returns a two-triangle torus. This is identical to the triangulation
returned by the generic routine sphereBundle().

Returns:
    the torus.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

