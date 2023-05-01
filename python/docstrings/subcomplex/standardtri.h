/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::StandardTriangulation
static const char *StandardTriangulation =
R"doc(Describes a triangulation or subcomplex of a triangulation whose
structure is well-understood. This is an abstract base class: its
subclasses correspond to different combinatorial constructions
(typically parameterised families of triangulations or subcomplexes).

A StandardTriangulation is generally identified with a concrete
triangulation, i.e., a Triangulation<3> object, which it describes
some portion of (or possibly all of).

In general StandardTriangulation objects cannot be constructed
directly, but are instead created through static identification
routines such as StandardTriangulation::recognise(const
Triangulation<3>&).

Each subclass of StandardTriangulation:

* must override all pure virtual functions (of course);

* may optionally override manifold() and/or homology(), if they are
  able to provide this functionality;

* may optionally override writeTextShort() and writeTextLong(), though
  this class provides sensible default implementations which use
  writeName() (which subclasses always override because writeName() is
  pure virtual);

* provide value semantics (including at least a copy constructor and
  assignment operator);

* provide member and global swap functions, for consistency across all
  StandardTriangulation subclasses.)doc";

namespace StandardTriangulation_ {

// Docstring regina::python::doc::StandardTriangulation_::homology
static const char *homology =
R"doc(Returns the expected first homology group of this triangulation, if
such a routine has been implemented.

This routine does not work by calling Triangulation<3>::homology() on
the associated real triangulation. Instead the homology is calculated
directly from the known properties of this standard triangulation.

This means that homology() needs to be implemented separately for each
class of standard triangulation. See the class notes for each subclass
of StandardTriangulation for details on whether homology has been
implemented for that particular subclass. The default implementation
of this routine just throws a NotImplemented exception.

Most users will not need this routine, since presumably you already
have an explicit Triangulation<3> available and so you can just call
Triangulation<3>::homology() instead (which, unlike this routine, _is_
always implemented). This StandardTriangulation::homology() routine
should be seen as more of a verification/validation tool for the
Regina developers.

If this StandardTriangulation describes an entire Triangulation<3>
(and not just a part thereof) then the results of this routine should
be identical to the homology group obtained by calling
Triangulation<3>::homology() upon the associated real triangulation.

Exception ``NotImplemented``:
    Homology calculation has not yet been implemented for this
    particular type of standard triangulation.

Exception ``FileError``:
    The homology needs to be read from file (as opposed to computed),
    but the file is inaccessible or its contents cannot be read and
    parsed correctly. Currently this can only happen for the subclass
    SnapPeaCensusTri, which reads its results from the SnapPea census
    databases that are installed with Regina.

Returns:
    the first homology group of this triangulation, if this
    functionality has been implemented.)doc";

// Docstring regina::python::doc::StandardTriangulation_::manifold
static const char *manifold =
R"doc(Returns the 3-manifold represented by this triangulation, if such a
recognition routine has been implemented. If the 3-manifold cannot be
recognised then this routine will return ``None``.

The details of which standard triangulations have 3-manifold
recognition routines can be found in the notes for the corresponding
subclasses of StandardTriangulation. The default implementation of
this routine returns ``None``.

It is expected that the number of triangulations whose underlying
3-manifolds can be recognised will grow between releases.

Returns:
    the underlying 3-manifold.)doc";

// Docstring regina::python::doc::StandardTriangulation_::name
static const char *name =
R"doc(Returns the name of this specific triangulation as a human-readable
string.

Returns:
    the name of this triangulation.)doc";

// Docstring regina::python::doc::StandardTriangulation_::recognise
static const char *recognise =
R"doc(Determines whether the given component represents one of the standard
triangulations understood by Regina. The list of recognised
triangulations is expected to grow between releases.

If the standard triangulation returned has boundary triangles then the
given component must have the same corresponding boundary triangles,
i.e., the component cannot have any further identifications of these
boundary triangles with each other.

Note that the triangulation-based routine recognise(const
Triangulation<3>&) may recognise more triangulations than this
routine, since passing an entire triangulation allows access to more
information.

Parameter ``component``:
    the triangulation component under examination.

Returns:
    the details of the standard triangulation if the given component
    is recognised, or ``None`` otherwise.)doc";

// Docstring regina::python::doc::StandardTriangulation_::recognise_2
static const char *recognise_2 =
R"doc(Determines whether the given triangulation represents one of the
standard triangulations understood by Regina. The list of recognised
triangulations is expected to grow between releases.

If the standard triangulation returned has boundary triangles then the
given triangulation must have the same corresponding boundary
triangles, i.e., the triangulation cannot have any further
identifications of these boundary triangles with each other.

This routine may recognise more triangulations than the component-
based recognise(Component<3>*), since passing an entire triangulation
allows access to more information.

Parameter ``tri``:
    the triangulation under examination.

Returns:
    the details of the standard triangualation if the given
    triangulation is recognised, or ``None`` otherwise.)doc";

// Docstring regina::python::doc::StandardTriangulation_::texName
static const char *texName =
R"doc(Returns the name of this specific triangulation in TeX format. No
leading or trailing dollar signs will be included.

.. warning::
    The behaviour of this routine has changed as of Regina 4.3; in
    earlier versions, leading and trailing dollar signs were provided.

Returns:
    the name of this triangulation in TeX format.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

