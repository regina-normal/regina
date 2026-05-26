/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Manifold {

// Docstring regina::python::doc::Manifold::__class
static constexpr const char __class[] =
R"doc(An abstract base class for various well-understood *dim*-dimensional
-manifolds.

Each subclass of Manifold describes a particular type of well-
understood *dim*-manifold. A single subclass could describe one
particular manifold (e.g., the *dim*-dimensional sphere), or an entire
parameterised family (e.g., lens spaces). Manifolds do _not_ encode
specific triangulations; instead they describe the topology of a
manifold, independent of how it might be triangulated.

Importantly, these subclasses do not aim to classify all manifolds:
there are many manifolds that are not represented by any subclass of
Manifold at all.

When defining a new subclass of `Manifold<dim>`:

* you must override the pure virtual functions writeName() and
  writeTeXName();

* optionally, you can implement the extra features offered through
  ``ManifoldOptions<dim>`` (such as isHyperbolic() and homology() in
  dimension 3);

* optionally, you can override construct() and/or writeStructure() if
  your class is able to provide this functionality;

* you must _not_ override writeTextShort() or writeTextLong(), since
  these routines are not virtual, but are provided directly by the
  Manifold base class;

* you must provide value semantics (including at least a copy
  constructor and assignment operator);

* you must provide comparison operators (== and !=) that compare two
  objects of your subclass;

* you must provide member and global swap functions, for consistency
  across all Manifold subclasses;

* optionally, for dimension 3, you may incorporate your subclass into
  the global ordering on 3-manifolds (operator <=>).

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., Manifold3 for
    dimensions 3).

Template parameter ``dim``:
    the dimension of manifold under consideration. Currently this is
    restricted to dimension 3 only, but the intent is to support other
    dimensions in future versions of Regina.)doc";

// Docstring regina::python::doc::Manifold::__cmp
static constexpr const char __cmp[] =
R"doc(Compares representations of two 3-manifolds according to an aesthetic
ordering.

The only purpose of this routine is to implement a consistent ordering
of 3-manifold representations. The specific ordering used is purely
aesthetic on the part of the author, and is subject to change in
future versions of Regina.

It does not matter whether the two 3-manifolds are homeomorphic; this
routine compares the specific _representations_ of these manifolds
(and so in particular, different representations of the same
3-manifold might well be ordered differently).

This operator generates all of the usual comparison operators,
including ``<``, ``<=``, ``>``, and ``>=``.

.. warning::
    Currently this routine is only implemented in full for closed
    3-manifolds. For most classes of bounded 3-manifolds, this routine
    simply compares the strings returned by name(). For this reason,
    the return value is currently marked as a weak ordering, since it
    is possible that different representations of the same 3-manifold
    will produce the same string name.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``x``:
    the first 3-manifold representation to compare.

Parameter ``y``:
    the second 3-manifold representation to compare.

Returns:
    a result that indicates how the representations of *x* and *y*
    should be ordered with respect to each other.)doc";

// Docstring regina::python::doc::Manifold::construct
static constexpr const char construct[] =
R"doc(Returns a triangulation of this manifold, if such a construction has
been implemented.

Subclasses of Manifold may choose whether or not to implement this
routine. Moreover, if a subclass describes a parameterised family of
manifolds, it may choose to implement this only for some members of
the family. If construction has not been implemented for this
particular manifold, then this routine should throw a NotImplemented
exception.

Individual subclasses of Manifold should explain in their class notes
whether they implement construct(). The default implementation
provided by this base class just throws a NotImplemented exception.

It is expected that over time, more subclasses of Manifold will
implement construct() in future releases of Regina.

Exception ``NotImplemented``:
    Triangulation is not yet implemented for this particular manifold.

Exception ``FileError``:
    The construction needs to be read from file (as opposed to
    computed on the fly), but the file is inaccessible or its contents
    cannot be read and parsed correctly. Currently this can only
    happen for the subclass SnapPeaCensusManifold, which reads its
    triangulations from the SnapPea census databases that are
    installed with Regina.

Returns:
    a triangulation of this manifold, if this construction has been
    implemented.)doc";

// Docstring regina::python::doc::Manifold::name
static constexpr const char name[] =
R"doc(Returns the common name of this manifold as a human-readable string.

Returns:
    the common name of this manifold.)doc";

// Docstring regina::python::doc::Manifold::structure
static constexpr const char structure[] =
R"doc(Returns details of the structure of this manifold that might not be
evident from its common name. For instance, for an orbit space S³/G
this routine might return the full Seifert structure.

This routine may return the empty string if no additional details are
deemed necessary.

Returns:
    a string describing additional structural details.)doc";

// Docstring regina::python::doc::Manifold::texName
static constexpr const char texName[] =
R"doc(Returns the common name of this manifold in TeX format.

The TeX will assume that we are within math mode, and no leading or
trailing dollar signs will be included.

Returns:
    the common name of this manifold in TeX format.)doc";

}; // struct Manifold

struct ManifoldOptions {

// Docstring regina::python::doc::ManifoldOptions::__class
static constexpr const char __class[] =
R"doc(Optional features that manifold classes may choose to implement.

As described in the Manifold class notes, each subclass of
``Manifold<dim>`` describes a particular type of well-understood
*dim*-dimensional manifold.

Every such subclass _must_ implement basic features, such as writing
the manifold name in English and in TeX. However, there are other
features that are optional for subclasses to implement: examples in
dimension 3 include hyperbolicity testing and first homology.

This base class ``ManifoldOptions<dim>`` describes those optional
features as virtual functions, and provides default implementations
that indicate to the user in some appropriate way that the feature is
not implemented (e.g., by returning ``None``, or throwing an
exception). Each subclass of ``Manifold<dim>`` may, if desired,
override any of these virtual functions to provide a real
implementation. If a subclass describes a parameterised family of
manifolds, it may even choose to implement these features only for
some members of the family (falling back to the default "not
impemented" behaviour for the others).

These optional features are specific to the dimension, and so this
class should be specialised where necessary (for example, the virtual
functions ``isHyperbolic()`` and ``homology()`` as discussed earlier
are provided by the specialisation ``ManifoldOptions<3>``). The
generic implementation of ManifoldOptions has no functions at all.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., ManifoldOptions3 for
    dimension 3).

Template parameter ``dim``:
    the dimension of manifold under consideration.)doc";

}; // struct ManifoldOptions

struct ManifoldOptions3 {

// Docstring regina::python::doc::ManifoldOptions3::__class
static constexpr const char __class[] =
R"doc(Describes optional features that 3-manifold classes may choose to
implement. See the generic ManifoldOptions class notes for an overview
of how such optional features work.

This class provides these optional features as virtual functions, with
default implementations that indicate to the user in some appropriate
way that the feature is not implemented (e.g., by returning ``None``,
or throwing an exception). Each subclass of ``Manifold<3>`` may, if
desired, override any of these virtual functions to provide a real
implementation.

Individual manifold classes should explain in their class notes which
of these optional features they implement.

Python:
    This class is available to Python users under the name
    ManifoldOptions3.)doc";

// Docstring regina::python::doc::ManifoldOptions3::homology
static constexpr const char homology[] =
R"doc(Returns the first homology group of this 3-manifold, if this is known
to Regina.

This is an optional feature that 3-manifold classes may choose whether
to implement; see the ManifoldOptions class notes for an overview of
how such optional features work.

If homology has not been implemented for this particular 3-manifold,
this routine should return throw a NotImplemented exception (which is
what the default implementation does).

It is expected that over time, more 3-manifold classes will implement
homology() in future releases of Regina.

Exception ``NotImplemented``:
    Homology calculation is not yet implemented for this particular
    3-manifold.

Exception ``FileError``:
    The homology needs to be read from file (as opposed to computed),
    but the file is inaccessible or its contents cannot be read and
    parsed correctly. Currently this can only happen for the subclass
    SnapPeaCensusManifold, which reads its results from the SnapPea
    census databases that are installed with Regina.

Returns:
    the first homology group, if this is implemented for this
    particular manifold.)doc";

// Docstring regina::python::doc::ManifoldOptions3::isHyperbolic
static constexpr const char isHyperbolic[] =
R"doc(Returns whether or not this is a finite-volume hyperbolic 3-manifold,
if this is known to Regina.

This is an optional feature that 3-manifold classes may choose whether
to implement; see the ManifoldOptions class notes for an overview of
how such optional features work.

If hyperbolicity testing has not been implemented for this particular
3-manifold, this routine should throw a NotImplemented exception
(which is what the default implementation does).

As of Regina 8.0, _all_ 3-manifold classes implement isHyperbolic()
(though it is possible that the future there will be new manifold
classes that do not).

Exception ``NotImplemented``:
    Hyperbolicity testing is not yet implemented for this particular
    3-manifold.

Returns:
    ``True`` if this is a finite-volume hyperbolic 3-manifold, or
    ``False`` if not.)doc";

}; // struct ManifoldOptions3

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

