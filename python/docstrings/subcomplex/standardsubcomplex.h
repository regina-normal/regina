/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct StandardSubcomplex {

// Docstring regina::python::doc::StandardSubcomplex::__class
static constexpr const char __class[] =
R"doc(An abstract base class for various types of well-understood
combinatorial subcomplexes within a *dim*-dimensional triangulation.

Each subclass describes a particular type of well-understood
combinatorial subcomplex. A single subclass could describe one
particular subcomplex (e.g., a one-tetrahedron snapped ball), or an
entire parameterised family of subcomplexes (e.g., layered solid
tori). These subcomplexes could describe smaller parts of
triangulations (e.g., layered solid tori, which appear in many
different larger 3-dimensional constructions), or they could describe
entire triangulations (e.g., the infinite family of layered lens
spaces).

A specific instance of a StandardSubcomplex would typically be
associated with a concrete triangulation, and would typically store
details of where the subcomplex is situated within the triangulation
(e.g., via pointers to top-dimensional simplices or lower-dimensional
faces).

End users typically cannot construct these subcomplex objects
directly. Instead you would typically obtain a StandardSubcomplex by
passing a triangulation (or part thereof) to a static recognition
routine such as ``StandardSubcomplex<dim>::recognise(const
Triangulation<dim>&)``.

When defining a new subclass of StandardSubcomplex<dim>:

* you must override the pure virtual functions writeName() and
  writeTeXName();

* optionally, you can override manifold() and/or homology() if your
  class is able to provide this functionality;

* optionally, you can override writeTextShort() and writeTextLong(),
  though this base class provides sensible default implementations
  based on writeName();

* you must provide value semantics (including at least a copy
  constructor and assignment operator);

* you must provide member and global swap functions, for consistency
  across all StandardSubcomplex subclasses.)doc";

// Docstring regina::python::doc::StandardSubcomplex::homology
static constexpr const char homology[] =
R"doc(Returns the first homology group of this subcomplex, if this is known.

This routine does _not_ call ``Triangulation<dim>::homology()`` on the
associated triangulation. Instead the homology is calculated directly
from the known properties of this subcomplex. This can (for example)
be used to assist with unit testing for subcomplex recognition
routines.

Most users will not need this routine, since it is only implemented
for some subcomplexes (as opposed to
``Triangulation<dim>::homology()`` which is implemented always).

For subclasses that describe an entire triangulation (as opposed to
just a part thereof), the results of this routine should match the
homology group obtained by calling ``Triangulation<dim>::homology()``.

Subclasses of StandardSubcomplex may choose whether or not to
implement this routine. Moreover, if a subclass describes a
parameterised family of subcomplexes, it may choose to implement this
only for some members of the family. If homology has not been
implemented for this particular subcomplex, then this routine should
throw a NotImplemented exception.

Individual subclasses of ``StandardSubcomplex<3>`` should explain in
their class notes whether they implement homology(). The default
implementation provided by this base class just throws a
NotImplemented exception.

It is expected that over time, more subclasses of StandardSubcomplex
will implement homology() in future releases of Regina.

Exception ``NotImplemented``:
    Homology calculation is not yet implemented for this particular
    subcomplex.

Exception ``FileError``:
    The homology needs to be read from file (as opposed to computed),
    but the file is inaccessible or its contents cannot be read and
    parsed correctly. Currently this can only happen for the subclass
    SnapPeaCensusTri, which reads its results from the SnapPea census
    databases that are installed with Regina.

Returns:
    the first homology group, if this is implemented for this
    particular subcomplex.)doc";

// Docstring regina::python::doc::StandardSubcomplex::manifold
static constexpr const char manifold[] =
R"doc(Returns the manifold represented by this subcomplex, if this is known.
Currently this feature is only available in dimension 3.

Subclasses of StandardSubcomplex may choose whether or not to
implement this routine. Moreover, if a subclass describes a
parameterised family of subcomplexes, it may choose to implement this
only for some members of the family. A return value of ``None``
indicates that recognition has not been implemented for this
particular subcomplex.

Individual subclasses of ``StandardSubcomplex<3>`` should explain in
their class notes whether they implement manifold(). The default
implementation provided by this base class simply returns ``None``.

It is expected that over time, more subclasses of StandardSubcomplex
will implement manifold() in future releases of Regina.

Returns:
    the underlying manifold, or ``None`` if manifold recognition is
    not yet implemented for this particular subcomplex.)doc";

// Docstring regina::python::doc::StandardSubcomplex::name
static constexpr const char name[] =
R"doc(Returns the name of this subcomplex as a human-readable string.

Returns:
    the name of this subcomplex.)doc";

// Docstring regina::python::doc::StandardSubcomplex::recognise
static constexpr const char recognise[] =
R"doc(Determines whether the given component is completely described by one
of the standard subcomplexes understood by Regina. It is anticipated
that over time, more standard subcomplexes will be recognised in
future releases of Regina.

If the subcomplex that is returned has boundary facets, then the given
component must have the same corresponding boundary facets. That is,
the component cannot have any further identifications of boundary
facets beyond those described by the subcomplex that is returned.

Note that the variant ``recognise(const Triangulation<dim>&)`` may
recognise more triangulations than this routine, since passing an
entire triangulation allows access to more information.

Parameter ``component``:
    the triangulation component under examination.

Returns:
    details of the standard subcomplex if the given component is
    recognised as such, or ``None`` otherwise.)doc";

// Docstring regina::python::doc::StandardSubcomplex::recognise_2
static constexpr const char recognise_2[] =
R"doc(Determines whether the given triangulation is completely described by
one of the standard subcomplexes understood by Regina. It is
anticipated that over time, more standard subcomplexes will be
recognised in future releases of Regina.

If the subcomplex that is returned has boundary facets, then the given
triangulation must have the same corresponding boundary facets. That
is, the triangulation cannot have any further identifications of
boundary facets beyond those described by the subcomplex that is
returned.

This routine may recognise more triangulations than the variant
``recognise(Component<dim>*)``, since passing an entire triangulation
allows access to more information.

Parameter ``tri``:
    the triangulation under examination.

Returns:
    details of the standard subcomplex if the given triangulation is
    recognised as such, or ``None`` otherwise.)doc";

// Docstring regina::python::doc::StandardSubcomplex::texName
static constexpr const char texName[] =
R"doc(Returns the name of this subcomplex in TeX format.

The TeX will assume that we are within math mode, and no leading or
trailing dollar signs will be included.

Returns:
    the name of this subcomplex in TeX format.)doc";

}; // struct StandardSubcomplex

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

