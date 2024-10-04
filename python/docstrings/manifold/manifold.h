/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Manifold
static const char *Manifold =
R"doc(Represents a particular 3-manifold, independent of how it might be
triangulated. This is an abstract base class: its subclasses
correspond to different families of 3-manifolds.

Each subclass:

* must override all pure virtual functions (of course);

* may optionally override construct(), homology() and/or
  writeStructure(), if they are able to provide this functionality;

* must _not_ override writeTextShort() or writeTextLong(), since these
  routines are not virtual, and are provided directly by the Manifold
  base class;

* must provide value semantics (including at least a copy constructor
  and assignment operator);

* must provide comparison operators (== and !=);

* must provide member and global swap functions, for consistency
  across all Manifold subclasses.)doc";

namespace Manifold_ {

// Docstring regina::python::doc::Manifold_::__cmp
static const char *__cmp =
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

Parameter ``rhs``:
    the 3-manifold representation to compare this with.

Returns:
    A result that indicates how this and the given 3-manifold
    representation should be ordered with respect to each other.)doc";

// Docstring regina::python::doc::Manifold_::construct
static const char *construct =
R"doc(Returns a triangulation of this 3-manifold, if such a construction has
been implemented. For details of which types of 3-manifolds have
implemented this routine, see the class notes for each corresponding
subclasses of Manifold.

The default implemention of this routine just throws a NotImplemented
exception.

Exception ``NotImplemented``:
    Explicit construction has not yet been implemented for this
    particular 3-manifold.

Exception ``FileError``:
    The construction needs to be read from file (as opposed to
    computed on the fly), but the file is inaccessible or its contents
    cannot be read and parsed correctly. Currently this can only
    happen for the subclass SnapPeaCensusManifold, which reads its
    triangulations from the SnapPea census databases that are
    installed with Regina.

Returns:
    a triangulation of this 3-manifold, if this construction has been
    implemented.)doc";

// Docstring regina::python::doc::Manifold_::homology
static const char *homology =
R"doc(Returns the first homology group of this 3-manifold, if such a routine
has been implemented. For details of which types of 3-manifolds have
implemented this routine, see the class notes for each corresponding
subclasses of Manifold.

The default implemention of this routine just throws a NotImplemented
exception.

Exception ``NotImplemented``:
    Homology calculation has not yet been implemented for this
    particular 3-manifold.

Exception ``FileError``:
    The homology needs to be read from file (as opposed to computed),
    but the file is inaccessible or its contents cannot be read and
    parsed correctly. Currently this can only happen for the subclass
    SnapPeaCensusManifold, which reads its results from the SnapPea
    census databases that are installed with Regina.

Returns:
    the first homology group of this 3-manifold, if this functionality
    has been implemented.)doc";

// Docstring regina::python::doc::Manifold_::isHyperbolic
static const char *isHyperbolic =
R"doc(Returns whether or not this is a finite-volume hyperbolic manifold.

Returns:
    ``True`` if this is a finite-volume hyperbolic manifold, or
    ``False`` if not.)doc";

// Docstring regina::python::doc::Manifold_::name
static const char *name =
R"doc(Returns the common name of this 3-manifold as a human-readable string.

Returns:
    the common name of this 3-manifold.)doc";

// Docstring regina::python::doc::Manifold_::structure
static const char *structure =
R"doc(Returns details of the structure of this 3-manifold that might not be
evident from its common name. For instance, for an orbit space S³/G
this routine might return the full Seifert structure.

This routine may return the empty string if no additional details are
deemed necessary.

Returns:
    a string describing additional structural details.)doc";

// Docstring regina::python::doc::Manifold_::texName
static const char *texName =
R"doc(Returns the common name of this 3-manifold in TeX format. No leading
or trailing dollar signs will be included.

.. warning::
    The behaviour of this routine has changed as of Regina 4.3; in
    earlier versions, leading and trailing dollar signs were provided.

Returns:
    the common name of this 3-manifold in TeX format.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

