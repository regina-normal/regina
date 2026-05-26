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
R"doc(An abstract base class for various well-understood 3-manifolds.

Each subclass of Manifold describes a particular type of well-
understood 3-manifold. A single subclass could describe one particular
manifold (e.g., the 3-sphere), or an entire parameterised family
(e.g., lens spaces). Manifolds do _not_ encode specific
triangulations; instead they describe the topology of a manifold,
independent of how it might be triangulated.

Importantly, these subclasses do not aim to classify all 3-manifolds:
there are many 3-manifolds that are not represented by any subclass of
Manifold at all.

When defining a new subclass of Manifold:

* you must override the pure virtual functions writeName(),
  writeTeXName(), and isHyperbolic();

* optionally, you can override construct(), homology() and/or
  writeStructure() if your class is able to provide this
  functionality;

* you must _not_ override writeTextShort() or writeTextLong(), since
  these routines are not virtual, and are provided directly by the
  Manifold base class;

* you must provide value semantics (including at least a copy
  constructor and assignment operator);

* you must provide comparison operators (== and !=);

* optionally, you may incorporate your subclass into the global
  ordering on manifolds (operator <=>);

* you must provide member and global swap functions, for consistency
  across all Manifold subclasses.)doc";

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

Parameter ``rhs``:
    the 3-manifold representation to compare this with.

Returns:
    a result that indicates how this and the given 3-manifold
    representation should be ordered with respect to each other.)doc";

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
    Explicit construction is not yet implemented for this particular
    manifold.

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

// Docstring regina::python::doc::Manifold::homology
static constexpr const char homology[] =
R"doc(Returns the first homology group of this manifold, if this is known to
Regina.

Subclasses of Manifold may choose whether or not to implement this
routine. Moreover, if a subclass describes a parameterised family of
manifolds, it may choose to implement this only for some members of
the family. If homology has not been implemented for this particular
manifold, then this routine should throw a NotImplemented exception.

Individual subclasses of Manifold should explain in their class notes
whether they implement homology(). The default implementation provided
by this base class just throws a NotImplemented exception.

It is expected that over time, more subclasses of Manifold will
implement homology() in future releases of Regina.

Exception ``NotImplemented``:
    Homology calculation is not yet implemented for this particular
    manifold.

Exception ``FileError``:
    The homology needs to be read from file (as opposed to computed),
    but the file is inaccessible or its contents cannot be read and
    parsed correctly. Currently this can only happen for the subclass
    SnapPeaCensusManifold, which reads its results from the SnapPea
    census databases that are installed with Regina.

Returns:
    the first homology group, if this is implemented for this
    particular manifold.)doc";

// Docstring regina::python::doc::Manifold::isHyperbolic
static constexpr const char isHyperbolic[] =
R"doc(Returns whether or not this is a finite-volume hyperbolic manifold.

Returns:
    ``True`` if this is a finite-volume hyperbolic manifold, or
    ``False`` if not.)doc";

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

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

