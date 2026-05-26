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

Each subclass of StandardSubcomplex describes a particular type of
well-understood combinatorial subcomplex. A single subclass could
describe one particular subcomplex (e.g., a one-tetrahedron snapped
ball), or an entire parameterised family (e.g., layered solid tori).
These subcomplexes could describe smaller parts of triangulations
(e.g., layered solid tori, which appear in many different larger
3-dimensional constructions), or they could describe entire
triangulations (e.g., the infinite family of layered lens spaces).

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

When defining a new subclass of `StandardSubcomplex<dim>`:

* you must override the pure virtual functions writeName() and
  writeTeXName();

* optionally, you can implement the extra features offered through
  ``StandardSubcomplexOptions<dim>`` (such as manifold() and
  homology() in dimension 3);

* optionally, you can override writeTextShort() and writeTextLong(),
  though this base class provides sensible default implementations
  based on writeName();

* you must provide value semantics (including at least a copy
  constructor and assignment operator);

* you must provide member and global swap functions, for consistency
  across all StandardSubcomplex subclasses.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., StandardSubcomplex3
    and StandardSubcomplex4 for dimensions 3 and 4).

Template parameter ``dim``:
    the dimension of triangulation in which the subcomplexes live.)doc";

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

struct StandardSubcomplexOptions {

// Docstring regina::python::doc::StandardSubcomplexOptions::__class
static constexpr const char __class[] =
R"doc(Optional features that standard subcomplex classes may choose to
implement.

As described in the StandardSubcomplex class notes, each subclass of
``StandardSubcomplex<dim>`` describes a particular type of well-
understood combinatorial subcomplex within a *dim*-dimensional
triangulation.

Every such subclass _must_ implement basic features, such as writing
the subcomplex name in English and in TeX. However, there are other
features that are optional for subclasses to implement: examples in
dimension 3 include returning the underlying manifold, or the expected
first homology.

This base class ``StandardSubcomplexOptions<dim>`` describes those
optional features as virtual functions, and provides default
implementations that indicate to the user in some appropriate way that
the feature is not implemented (e.g., by returning ``None``, or
throwing an exception). Each subclass of ``StandardSubcomplex<dim>``
may, if desired, override any of these virtual functions to provide a
real implementation. If a subclass describes a parameterised family of
subcomplexes, it may even choose to implement these features only for
some members of the family (falling back to the default "not
impemented" behaviour for the others).

These optional features are specific to the dimension, and so this
class should be specialised where necessary (for example, the virtual
functions ``manifold()`` and ``homology()`` as discussed earlier are
provided by the specialisation ``StandardSubcomplexOptions<3>``). The
generic implementation of StandardSubcomplexOptions has no functions
at all.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g.,
    StandardSubcomplexOptions3 and StandardSubcomplexOptions4 for
    dimensions 3 and 4).

Template parameter ``dim``:
    the dimension of triangulation in which the subcomplexes live.)doc";

}; // struct StandardSubcomplexOptions

struct StandardSubcomplexOptions3 {

// Docstring regina::python::doc::StandardSubcomplexOptions3::__class
static constexpr const char __class[] =
R"doc(Describes optional features that standard 3-dimensional subcomplex
classes may choose to implement. See the generic
StandardSubcomplexOptions class notes for an overview of how such
optional features work.

This class provides these optional features as virtual functions, with
default implementations that indicate to the user in some appropriate
way that the feature is not implemented (e.g., by returning ``None``,
or throwing an exception). Each subclass of ``StandardSubcomplex<3>``
may, if desired, override any of these virtual functions to provide a
real implementation.

Individual subcomplex classes should explain in their class notes
which of these optional features they implement.

Python:
    This class is available to Python users under the name
    StandardSubcomplexOptions3.)doc";

// Docstring regina::python::doc::StandardSubcomplexOptions3::homology
static constexpr const char homology[] =
R"doc(Returns the first homology group of this subcomplex, if this is known
to Regina.

This is an optional feature that subcomplex classes may choose whether
to implement; see the StandardSubcomplexOptions class notes for an
overview of how such optional features work.

If homology has not been implemented for this particular subcomplex,
this routine should return throw a NotImplemented exception (which is
what the default implementation does).

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

It is expected that over time, more subcomplex classes will implement
homology() in future releases of Regina.

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

// Docstring regina::python::doc::StandardSubcomplexOptions3::manifold
static constexpr const char manifold[] =
R"doc(Returns the 3-manifold represented by this subcomplex, if this is
known to Regina.

This is an optional feature that subcomplex classes may choose whether
to implement; see the StandardSubcomplexOptions class notes for an
overview of how such optional features work.

If 3-manifold recognition has not been implemented for this particular
subcomplex, this routine should return ``None`` (which is what the
default implementation does).

It is expected that over time, more subcomplex classes will implement
manifold() in future releases of Regina.

Returns:
    the underlying manifold, or ``None`` if manifold recognition is
    not yet implemented for this particular subcomplex.)doc";

}; // struct StandardSubcomplexOptions3

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

