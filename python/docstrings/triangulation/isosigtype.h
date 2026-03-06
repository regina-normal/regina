/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::IsoSigClassic
constexpr const char *IsoSigClassic =
R"doc(The default signature type to use for isomorphism signatures of
triangulations.

This signature type is slower to use than others; its main benefit is
that it is consistent with the original isomorphism signatures that
were implemented in Regina 4.90.

See the IsoSigTypeAPI documentation for details on all member
functions, and on how isomorphism signature types work in general.

This classic signature type is trivial: it considers _all_ possible
"starting simplices" *s*, and all ``(dim+1)!`` possible "starting
labellings" *p*.

This class is designed to be used as a template parameter for
``Triangulation<dim>::isoSig()`` and
``Triangulation<dim>::isoSigDetail()``. Typical users would have no
need to create objects of this class or call any of its functions
directly.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., IsoSigClassic2 and
    IsoSigClassic3 for dimensions 2 and 3).)doc";

// Docstring regina::python::doc::IsoSigDegrees
constexpr const char *IsoSigDegrees =
R"doc(Defines an alternate type of isomorphism signature based on degree
sequences of *subdim*-faces.

This is a "proof of concept" type that shows how you might speed up
isomorphism signature computations. It requires that the signature
choose a starting simplex whose set of *subdim*-face degrees is
lexicographically minimal amongst all top-dimensional simplices. The
hope is that this eliminates a large number of potential starting
simplices without adding an enormous amount of computational overhead.

See the IsoSigTypeAPI documentation for details on all member
functions, and on how isomorphism signature types work in general.

This class is designed to be used as a template parameter for
``Triangulation<dim>::isoSig()`` and
``Triangulation<dim>::isoSigDetail()``. Typical users would have no
need to create objects of this class or call any of its functions
directly.

Python:
    Python does not support templates, and there are far too many of
    these classes to wrap. Currently Python supports only the cases
    where *subdim* is 1 or *dim*-2, using the type aliases
    IsoSigEdgeDegrees and IsoSigRidgeDegrees respectively (these cover
    all the variants that Regina currently uses internally). You can
    access the corresponding classes by appending the appending the
    dimension as a suffix to the type alias (e.g., you can use
    IsoSigEdgeDegrees3 to work with edge degrees in 3-manifold
    triangulations).)doc";

// Docstring regina::python::doc::IsoSigTypeAPI
constexpr const char *IsoSigTypeAPI =
R"doc(A documentation-only class describing the expected behaviour of types
of isomorphism signatures.

Regina supports different _types_ of isomomorphism signatures of
triangulations. Essentially, the job of a signature type is to help
Regina determine which labelling of a triangulation is "canonical".
Different signature types will make different trade-offs between
factors such as speed, accessibility, backward compatibility and so
on, typically resulting in different notions of "canonical" as a
consequence.

This IsoSigTypeAPI class is a documentation-only class (it is not
actually built into Regina). Its purpose is to describe in detail the
tasks that an isomorphism signature type is expected to perform, and
the interface that the corresponding C++ class should provide.

A signature type class such as this works with a single component *c*
of a *dim*-dimenensional triangulation. The sole task of a type class
is to iterate through a selection of combinations ``(s, p)``, each of
which identifies a "starting simplex" and a "starting labelling" of
its vertices. Here *s* is a top-dimensional simplex in *c* that will
act as the "starting simplex", and *p* is a permutation that maps the
vertices of *s* to the "starting labelling" ``0,1,...,dim``.

The properties that any signature type must satisfy are:

* The selection of combinations ``(s, p)`` is always non-empty.

* If we reorder the top-dimensional simplices of *c* and/or relabel
  their individual vertices, then the combinations ``(s, p)`` that
  this type class produces will be the same set, but modified
  according to this reordering/relabelling. In other words, the
  starting simplices and their starting labellings can in theory be
  completely deduced from an _unlabelled_ triangulation component.

An instance of a signature type class is like an iterator: it holds a
single candidate combination ``(s, p)``. The constructor must
initialise the instance to store the first candidate combination; you
can then query the current combination by calling simplex() and
perm(), and you can advance to the next combination by calling next().

End users should typically not need to create instances of isomorphism
signature type classes. Instead you would typically use such classes
as C++ template arguments for functions such as
``Triangulation<dim>::isoSig()`` and
``Triangulation<dim>::isoSigDetail()``.

Python:
    Whilst Regina's signature type classes are available, it is rare
    that you would need to access these directly through Python.
    Instead, to use an isomorphism signature type, you would typically
    call a modified form of ``Triangulation<dim>::isoSig()`` or
    ``Triangulation<dim>::isoSigDetail()``. See
    ``Triangulation<dim>::isoSig()`` for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

namespace IsoSigTypeAPI_ {

// Docstring regina::python::doc::IsoSigTypeAPI_::__init
constexpr const char *__init =
R"doc(Initialises this object to iterate through candidate "starting
simplices" *s* and "starting labellings" *p* for the given
triangulation component. See the IsoSigTypeAPI class documentation for
details.

This object will initially be set to hold the first candidate pair
``(s, p)``.

Parameter ``comp``:
    the triangulation component that we are examining.)doc";

// Docstring regina::python::doc::IsoSigTypeAPI_::next
constexpr const char *next =
R"doc(Advances this object to the next candidate pair ``(s, p)``.

See the IsoSigTypeAPI class documentation for further details.

Precondition:
    This object is holding a valid candidate pair ``(s, p)``; that is,
    next() has not yet returned ``False``.

Returns:
    ``True`` if this was successful, or ``False`` if there is no next
    candidate pair (i.e., the current candidate pair is the last).)doc";

// Docstring regina::python::doc::IsoSigTypeAPI_::perm
constexpr const char *perm =
R"doc(Returns the current starting labelling *p* of the vertices of the
current starting simplex.

See the IsoSigTypeAPI class documentation for further details.

Precondition:
    This object is holding a valid candidate pair ``(s, p)``; that is,
    next() has not yet returned ``False``.

Returns:
    the starting labelling, given as a permutation that maps the
    current vertex labels of the starting simplex *s* to the
    "canonical" labels ``0,1,...,dim``.)doc";

// Docstring regina::python::doc::IsoSigTypeAPI_::simplex
constexpr const char *simplex =
R"doc(Returns the current starting simplex *s*.

See the IsoSigTypeAPI class documentation for further details.

Precondition:
    This object is holding a valid candidate pair ``(s, p)``; that is,
    next() has not yet returned ``False``.

Returns:
    the index of the current starting simplex with respect to the
    triangulation component under consideration. Note that, for a
    disconnected triangulation, this is _not_ necessarily the same as
    Simplex::index() (which gives the index with respect to the
    overall triangulation).)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

