/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


namespace detail {

// Docstring regina::python::doc::detail::ExampleBase
constexpr const char *ExampleBase =
R"doc(Provides core functionality for constructing example *dim*-dimensional
triangulations.

These example triangulations are made available through the class
Example<dim>, which uses this as a base class. End users should not
need to refer to ExampleBase directly.

See the Example class notes for further information.

Python:
    This base class is not present, but the "end user" class
    Example<dim> is.

Template parameter ``dim``:
    the dimension of the example triangulations to construct. This
    must be between 2 and 15 inclusive.)doc";

// Docstring regina::python::doc::detail::ExampleFromLowDim
constexpr const char *ExampleFromLowDim =
R"doc(Helper class that builds various *dim*-dimensional triangulations from
(*dim*-1)-dimensional triangulations.

Python:
    This base class is not present, but the "end user" class
    Example<dim> is.

Template parameter ``dim``:
    the dimension of the example triangulations to construct. This
    must be between 2 and 15 inclusive.

Template parameter ``available``:
    ``True`` if Regina supports (*dim*-1)-dimensional triangulations,
    or ``False`` if not (in which case this class will be empty).)doc";

}

namespace detail::ExampleBase_ {

// Docstring regina::python::doc::detail::ExampleBase_::ball
constexpr const char *ball =
R"doc(Returns a one-simplex triangulation of the *dim*-ball.

Returns:
    a one-simplex *dim*-ball.)doc";

// Docstring regina::python::doc::detail::ExampleBase_::ballBundle
constexpr const char *ballBundle =
R"doc(Returns a triangulation of the product space ``B^(*dim*-1) x S^1``.
This will use one simplex in odd dimensions, or two simplices in even
dimensions.

Returns:
    the product ``B^(*dim*-1) x S^1``.)doc";

// Docstring regina::python::doc::detail::ExampleBase_::simplicialSphere
constexpr const char *simplicialSphere =
R"doc(Returns the standard (*dim*+2)-simplex triangulation of the
*dim*-sphere as the boundary of a (*dim*+1)-simplex.

Returns:
    the standard simplicial *dim*-sphere.)doc";

// Docstring regina::python::doc::detail::ExampleBase_::sphere
constexpr const char *sphere =
R"doc(Returns a two-simplex triangulation of the *dim*-sphere.

Returns:
    a two-simplex *dim*-sphere.)doc";

// Docstring regina::python::doc::detail::ExampleBase_::sphereBundle
constexpr const char *sphereBundle =
R"doc(Returns a two-simplex triangulation of the product space ``S^(*dim*-1)
x S^1``.

Returns:
    the product ``S^(*dim*-1) x S^1``.)doc";

// Docstring regina::python::doc::detail::ExampleBase_::twistedBallBundle
constexpr const char *twistedBallBundle =
R"doc(Returns a triangulation of the twisted product space ``B^(*dim*-1) x~
S^1``. This will use one simplex in even dimensions, or two simplices
in odd dimensions.

Returns:
    the twisted product ``B^(*dim*-1) x~ S^1``.)doc";

// Docstring regina::python::doc::detail::ExampleBase_::twistedSphereBundle
constexpr const char *twistedSphereBundle =
R"doc(Returns a two-simplex triangulation of the twisted product space
``S^(*dim*-1) x~ S^1``.

Returns:
    the twisted product ``S^(*dim*-1) x~ S^1``.)doc";

}

namespace detail::ExampleFromLowDim_ {

// Docstring regina::python::doc::detail::ExampleFromLowDim_::doubleCone
constexpr const char *doubleCone =
R"doc(Returns a double cone over the given (*dim-1*)-dimensional
triangulation.

If the given triangulation represents the manifold ``M``, then this
returns an ideal triangulation of the product ``M x I`` (with two
ideal boundary components). A copy of the original triangulation
*base* can be found at the centre of this construction, formed from
the *dim*-simplices that sit between the two ideal vertices.

Note that, as a special case, if ``M`` is either a sphere or a ball,
then this routine returns a (*dim*)-sphere or a (*dim*)-ball (since
"ideal spheres" and "ideal balls" just become regular internal and
boundary vertices respectively).

This construction is essentially the suspension of the triangulation
*base*. We do not call it this however, since from a topological point
of view, to form the ideal triangulation of ``M x I`` we "remove" the
vertices at the apex of each cone.

.. warning::
    If the given (*dim*-1)-dimensional triangulation has any boundary
    whatsoever (either real or ideal), then unless it is a
    (*dim*-1)-ball, you will obtain an invalid *dim*-manifold
    triangulation as a result.

Returns:
    a double cone over the given triangulation.)doc";

// Docstring regina::python::doc::detail::ExampleFromLowDim_::singleCone
constexpr const char *singleCone =
R"doc(Returns a single cone over the given (*dim-1*)-dimensional
triangulation.

If the given triangulation represents the manifold ``M``, then this
returns a triangulation of the product ``M x I`` that has one real
boundary component and one ideal boundary component. The triangulation
of the real boundary component will be identical to the original
(*dim-1*)-dimensional triangulation *base*.

.. warning::
    If the given (*dim*-1)-dimensional triangulation has any boundary
    whatsoever (either real or ideal), then unless it is a
    (*dim*-1)-ball, you will obtain an invalid *dim*-manifold
    triangulation as a result.

Returns:
    a single cone over the given triangulation.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

