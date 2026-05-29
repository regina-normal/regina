/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Census {

// Docstring regina::python::doc::Census::__class
static constexpr const char __class[] =
R"doc(Searches for triangulations and link diagrams across Regina's in-built
census databases.

This class consists of static routines only. The main entry point (and
typically the only way that you would use this class) is via the
various static routines lookup() and lookupAs().

This class does not initialise the list of databases until the first
time they are needed by a lookup routine; moreover, it initialises the
3-manifold databases independently from the link databases. This means
that you can call GlobalDirs::setDirs() or GlobalDirs::deduceDirs() if
you need to fix the database locations; however, you must do this
_before_ the first census lookup.

.. warning::
    This class is not thread-safe, since (as noted above) it performs
    global initialisation the first time that a 3-manifold lookup is
    performed, and also the first time that a link lookup is
    performed. If you need thread safety, you can always call lookup()
    with an empty string when initialising your program, before
    spawning any other threads.)doc";

// Docstring regina::python::doc::Census::lookup
static constexpr const char lookup[] =
R"doc(Searches for the given triangulation or link diagram within all of
Regina's in-built census databases.

Internally, the databases store isomorphism signatures and knot/link
signature (not fully fleshed-out triangulations and links). If you
already have a signature for your object then you can call the variant
``lookupAs<ObjectType>(const std::string&)`` instead, which (assuming
your signature is of the right generation) will be faster since it
avoids some extra overhead.

Note that there may be many hits (possibly from multiple databases,
and in some cases possibly even within the same database). Therefore a
_list_ of hits will be returned, which you can iterate through the
individual matches. Even if there are no matches at all, a list will
still be returned; you can call empty() on this list to test whether
any matches were found.

This routine is fast: it first computes the relevant signature of the
object, and then performs a logarithmic-time lookup in each relevant
database (here "logarithmic" means logarithmic in the size of the
database).

Exception ``FileError``:
    An error occurred within one of the databases. Typically this
    would indicate that some database could not be opened (e.g., it
    might not be installed correctly on the system).

Template parameter ``ObjectType``:
    the type of object that you are searching for. At present, this
    must be ``Triangulation<3>`` or ``Link``.

Parameter ``object``:
    the triangulation or link diagram that you wish to search for.

Returns:
    a list of all database matches.)doc";

// Docstring regina::python::doc::Census::lookup_2
static constexpr const char lookup_2[] =
R"doc(Searches for any triangulation or link diagram with the given
signature within all of Regina's in-built census databases, without
knowing in advance the original object type.

This routine is similar to lookupAs(), except that you do not need to
provide an object type in advance. Instead it will search _all_
databases, including triangulations _and_ link diagrams, and will
return all hits that it finds. It is possible (though unlikely) that
the hits will include a mix of different types of object.

As with lookupAs(), the given signature may be either second-
generation (as returned by ``Triangulation<dim>::neoSig()`` or
``Link::neoSig()``), or first-generation (as returned by
``Triangulation<dim>::isoSig()`` or ``Link::knotSig()``).

See lookupAs() for further information.

Exception ``FileError``:
    An error occurred within one of the databases. Typically this
    would indicate that some database could not be opened (e.g., it
    might not be installed correctly on the system).

Parameter ``sig``:
    an isomorphism signature or knot/link signature that you wish to
    search for; this may be either first-generation or second-
    generation.

Returns:
    a list of all database matches.)doc";

// Docstring regina::python::doc::Census::lookupAs
static constexpr const char lookupAs[] =
R"doc(Searches for the triangulation or link diagram with the given
signature within all of Regina's in-built census databases.

This routine assumes you know what kind of object you are searching
for (i.e., whether it is a 3-manifold triangulation or a link
diagram). You specify the type of object through the template argument
*ObjectType*, and you specify the object itself by passing its
signature (either an isomorphism signature for a triangulation, or a
knot/link signature for a link diagram). The signature may be either
second-generation (from ``Triangulation<dim>::neoSig()`` or
``Link::neoSig()``), or first-generation (from
``Triangulation<dim>::isoSig()`` or ``Link::knotSig()``); either
generation of signature will yield the same results.

Calling lookupAs() on a signature will yield the same results as
calling lookup() on the corresponding triangulation or link diagram,
but it offers different performance:

* If the signature is of the _same_ generation as is used internally
  by the census databases, then passing a signature to lookupAs() will
  avoid some overhead (since the variant of lookup() that takes a
  triangulation or link diagram must otherwise compute the signature
  to use as a lookup key).

* If the signature is of a _different_ generation from the one used
  internally by the census databases, then lookupAs() will _add_
  overhead (since it will need to reconstruct the triangulation or
  link diagram and _then_ compute the generation of signature that it
  needs to perform the internal database lookups).

A general rule of thumb is this: if you already have a signature, you
should call this string-based routine (regardless of which generation
of signature you have), since reconstruction is reasonably fast. If
you do not already have a signature, just call the triangulation-based
or link-based lookup().

Note that there may be many hits (possibly from multiple databases,
and in some cases possibly even within the same database). Therefore a
_list_ of hits will be returned, which you can iterate through the
individual matches. Even if there are no matches at all, a list will
still be returned; you can call empty() on this list to test whether
any matches were found.

This routine is fast: it first recomputes the generation of signature
that it needs (but only if the given signature is not already of the
correct generation), and then it performs a logarithmic-time lookup in
each database (where "logarithmic" means logarithmic in the size of
the database).

Python:
    Python does not support C++ templates. Instead you should pass the
    object type at runtime, using the argument order
    ``lookupAs(objectType, sig)``. An example that uses the signature
    of the figure eight knot is ``lookupAs(Link, "eputWe")``.

Exception ``FileError``:
    An error occurred within one of the databases. Typically this
    would indicate that some database could not be opened (e.g., it
    might not be installed correctly on the system).

Template parameter ``ObjectType``:
    the type of object that you are searching for. At present, this
    must be ``Triangulation<3>`` or ``Link``.

Parameter ``sig``:
    the isomorphism signature or knot/link signature of the
    triangulation or link diagram that you wish to search for; this
    may be either first-generation or second-generation.

Returns:
    a list of all database matches.)doc";

}; // struct Census

struct CensusCollection {

// Docstring regina::python::doc::CensusCollection::__class
static constexpr const char __class[] =
R"doc(Holds references to all of Regina's in-built census databases for a
particular type of topological object.

Ordinary users should not need to interact with CensusCollection
directly; instead you would typically use one of the high-level
Census::lookup() routines, which searches all of Regina's in-built
databases using the correct type of search key(s).

This class is essentially a halfway point between the CensusDB (which
manages a single database) and Census (which offers high-level lookup
routines for end users). See those two classes for further
information.

This class does not initialise its list of databases until the first
time they are needed by a lookup routine. This means that you can call
GlobalDirs::setDirs() or GlobalDirs::deduceDirs() if you need to fix
the database locations; however, you must do this _before_ the first
census lookup.

.. warning::
    This class is not thread-safe, since (as noted above) it performs
    global initialisation the first time that a 3-manifold lookup is
    performed, and also the first time that a link lookup is
    performed. If you need thread safety, you can always call lookup()
    with an empty string when initialising your program, before
    spawning any other threads.

Template parameter ``ObjectType``:
    the type of object stored in this collection of databases. At
    present, this must be ``Triangulation<3>`` or ``Link``.

Python:
    Python does not support C++ templates. For triangulations, you
    should append the dimension as a suffix to the type name (e.g.,
    ``CensusCollection3`` for the object type ``Triangulation<3>``);
    for links you should use the type name ``CensusCollectionLink``.)doc";

// Docstring regina::python::doc::CensusCollection::lookup
static constexpr const char lookup[] =
R"doc(Searches for the given object within all of the databases in this
collection.

Exception ``FileError``:
    An error occurred within one of the databases. Typically this
    would indicate that some database could not be opened (e.g., it
    might not be installed correctly on the system).

Parameter ``object``:
    the object that you wish to search for.

Returns:
    a list of all database matches.)doc";

// Docstring regina::python::doc::CensusCollection::lookup_2
static constexpr const char lookup_2[] =
R"doc(Searches for the object with the given signature within all of the
databases in this collection.

The given signature may be either second-generation or first-
generation; either will yield the same results.

Calling lookup() on a signature will yield the same results as calling
lookup() on the corresponding object (i.e., the triangulation or link
diagram, but it offers different performance:

* If the signature is of the _same_ generation as is used internally
  by the census databases, then passing a signature directly will
  avoid the overhead of computing it.

* If the signature is of a _different_ generation from the one used
  internally by the census databases, then passing a signature will
  _add_ overhead (since it must reconstruct the object and then
  compute the generation of signature that the databases need).

Exception ``FileError``:
    An error occurred within one of the databases. Typically this
    would indicate that some database could not be opened (e.g., it
    might not be installed correctly on the system).

Parameter ``sig``:
    the isomorphism signature or knot/link signature of the object
    that you wish to search for; this may be either first-generation
    or second-generation.

Returns:
    a list of all database matches.)doc";

}; // struct CensusCollection

struct CensusDB {

// Docstring regina::python::doc::CensusDB::__class
static constexpr const char __class[] =
R"doc(Stores the location and other identifying information for one of
Regina's in-built census databases.

A census database stores a list of key-value pairs. The keys are
isomorphism signatures of triangulations or knot/link signatures
(currently second-generation signatures as returned by
``Triangulation<dim>::neoSig()`` or ``Link::neoSig()``, but see the
notes below). The values are human-readable names (typically the names
of the triangulations, the links, and/or the underlying manifolds). A
key may appear multiple times (associated with different human-
readable names) within the same database.

Ordinary users should not need to interact with CensusDB directly;
instead you would typically use one of the high-level Census::lookup()
routines, which searches all of Regina's in-built databases using the
correct type of search key(s). There are two reasons for this:

* The _keys_ used for census databases are subject to change in future
  versions of Regina. Currently (as of Regina 8.0) these keys are
  second-generation signatures.

* The _format_ used to store census databases is an internal
  implementation detail, also subject to change in future releases of
  Regina. Even if you are accessing a specific database via CensusDB,
  you should only search a database using the high-level routine
  CensusDB::lookupKey() and not attempt to open the database file
  directly.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::CensusDB::__copy
static constexpr const char __copy[] = R"doc(Creates a new clone of the given database reference.)doc";

// Docstring regina::python::doc::CensusDB::__eq
static constexpr const char __eq[] =
R"doc(Tests whether this and the given object represent the same database.

Two databases are considered the same if and only if they have
identical filenames (as returned by the filename() function). The
database descriptions and tags are irrelevant here, as are the maximum
size arguments passed to the class constructor.

Parameter ``rhs``:
    the database to compare this against.

Returns:
    ``True`` if and only if this and the given object represent the
    same database.)doc";

// Docstring regina::python::doc::CensusDB::__init
static constexpr const char __init[] =
R"doc(Creates a new reference to a census database.

The database should use the same format as Regina's in-built census
databases (and this format depends upon your build configuration);
however, it does not actually need to be one of those in-built
databases, and it may be located anywhere on the filesystem.

This constructor will not run any checks (e.g., it will not verify
that the database exists, that it is stored in the correct format, or
that the *maxSize* argument is correct).

Parameter ``filename``:
    the filename where the database is stored.

Parameter ``desc``:
    a human-readable description of the database. See the desc()
    routine for further information on how this description might be
    used.

Parameter ``tag``:
    a short human-readable string that identifies this database, or
    the empty string if no such tag is necessary. See tag() for
    further information on how tags are used.

Parameter ``maxSize``:
    the maximum number of top-dimensional simplices and/or crossings
    for any entry in the database, or 0 (the default) if this is not
    known. This can be used to optimise database lookups.)doc";

// Docstring regina::python::doc::CensusDB::desc
static constexpr const char desc[] =
R"doc(Returns a human-readable description of this database.

The description could (for instance) be shown to users when giving a
list of all available databases, or when identifying in which
particular database a match was found.

Returns:
    the database description.)doc";

// Docstring regina::python::doc::CensusDB::filename
static constexpr const char filename[] =
R"doc(Returns the filename where this database is stored.

Returns:
    the database filename.)doc";

// Docstring regina::python::doc::CensusDB::global
static constexpr const char global[] =
R"doc(Returns a reference to one of Regina's in-built census databases,
stored in the standard census database location on the filesystem.

The database will be assumed to live in the directory
``GlobalDirs::census()``.

This variant of global() will not give the database an identifying
tag.

Parameter ``basename``:
    the base of the database filename, with no file extension and no
    directory information.

Parameter ``desc``:
    a human-readable description for the database.

Parameter ``maxSize``:
    the maximum number of top-dimensional simplices and/or crossings
    for any entry in the database, or 0 (the default) if this is not
    known. This can be used to optimise database lookups.

Returns:
    the resulting database reference.)doc";

// Docstring regina::python::doc::CensusDB::global_2
static constexpr const char global_2[] =
R"doc(Returns a reference to one of Regina's in-built census databases,
stored in the standard census database location on the filesystem.

The database will be assumed to live in the directory
``GlobalDirs::census()``.

Parameter ``basename``:
    the base of the database filename, with no file extension and no
    directory information.

Parameter ``desc``:
    a human-readable description for the database.

Parameter ``tag``:
    a short human-readable string that identifies this database; see
    tag() for further information on how tags are used.

Parameter ``maxSize``:
    the maximum number of top-dimensional simplices and/or crossings
    for any entry in the database, or 0 (the default) if this is not
    known. This can be used to optimise database lookups.

Returns:
    the resulting database reference.)doc";

// Docstring regina::python::doc::CensusDB::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the given database references.

This global routine simply calls CensusDB::swap(); it is provided so
that CensusDB meets the C++ Swappable requirements.

Parameter ``a``:
    the first database reference whose contents should be swapped.

Parameter ``b``:
    the second database reference whose contents should be swapped.)doc";

// Docstring regina::python::doc::CensusDB::lookupKey
static constexpr const char lookupKey[] =
R"doc(Searches for the given key in this database.

For each match that is found (if any), this routine will call *action*
(which must be a function or some other callable type). This action
must take exactly one CensusHit argument, which will be passed as a
prvalue (so the argument type for *action* could be any of
``CensusHit``, ``const CensusHit&``, or ``CensusHit&&``). The return
value of *action* will be ignored (typically *action* would return
``void``).

Note that the database will be opened and closed every time this
routine is called.

The argument *key* should be a *g*th-generation isomorphism signature
or knot/link signature, where the generation *g* is passed as a
template argument. Only one value of *g* is allowed: the same
generation of signature that the database uses internally for its
keys. This means that the way you call lookupKey() will change if/when
the database key type changes; this is by design, since you will of
course need to change what you pass as an argument also.

If you are using this routine yourself, you will need to include the
extra header census/census-impl.h (which is _not_ automatically
included from this file). Typically, however, end users can simply use
the catch-all Census::lookup() routines and will not need to call this
more fine-grained routine.

Python:
    This function is available in Python, and the *action* argument
    may be a pure Python function. Since Python does not support C++
    templates, the generation should be passed as an initial argument
    at runtime: ``lookupKey(generation, sig, action)``. If
    *generation* does not match the one allowed value (i.e., the
    format used internally by the database), then this routine will
    throw an InvalidArgument exception.

Template parameter ``generation``:
    the generation of isomorphism signature or knot/link signature
    that you are passing in the argument *sig*. Currently *generation*
    _must_ be 2, since Regina's databases currently use second-
    generation signatures as their keys.

Exception ``FileError``:
    An error occurred at the database level (e.g., the database could
    not be opened).

Parameter ``sig``:
    the isomorphism signature or knot/link signature to search for;
    this must be of the same generation that is passed as a template
    parameter.

Parameter ``action``:
    a function (or other callable type) that will be called for each
    match in the database.)doc";

// Docstring regina::python::doc::CensusDB::maxSize
static constexpr const char maxSize[] =
R"doc(Returns the maximum number of top-dimensional simplices and/or
crossings for any entry in this database, or 0 if this information is
not known.

If this _is_ known, it can be used to optimise database lookups (in
particular, to avoid lookups entirely when it is known that the key
will not be found).

Returns:
    the maximum number of top-dimensional simplices and/or crossings.)doc";

// Docstring regina::python::doc::CensusDB::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given database reference.

Parameter ``other``:
    the database reference whose contents are to be swapped with this.)doc";

// Docstring regina::python::doc::CensusDB::tag
static constexpr const char tag[] =
R"doc(Returns a short human-readable string that identifies this database,
or the empty string if no such tag has been deemed necessary.

Tags may be used in scenarios where triangulations or link diagrams
are likely to appear in multiple databases, and where the name of a
triangulation or link may be confusing without knowledge of which
database it came from.

An example is Regina's virtual knot census versus Regina's classical
knot census, which use the tags ``virtual`` and ``classical``
respectively.

Returns:
    the database tag, or the empty string if there is no tag.)doc";

}; // struct CensusDB

struct CensusHit {

// Docstring regina::python::doc::CensusHit::__class
static constexpr const char __class[] =
R"doc(Stores a single "hit" indicating that some given triangulation or link
diagram has been located in one of Regina's in-built census databases.

You cannot construct or modify instances of this class yourself, other
than through the standard copy/move/swap operations. The only way to
create "genuinely" new objects of this class is via the various static
Census::lookup() routines.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::CensusHit::__copy
static constexpr const char __copy[] = R"doc(Creates a new copy of the given census hit.)doc";

// Docstring regina::python::doc::CensusHit::__eq
static constexpr const char __eq[] =
R"doc(Tests whether this and the given object represent the same census hit.

Two census hits are considered the same if they have the same human-
readable name and also come from the same database (as identified by
the CensusDB comparison operators).

Parameter ``rhs``:
    the census hit to compare this against.

Returns:
    ``True`` if and only if this and the given hit are the same.)doc";

// Docstring regina::python::doc::CensusHit::db
static constexpr const char db[] =
R"doc(Returns details of the census database in which the triangulation or
link diagram was found.

Returns:
    the database for this hit.)doc";

// Docstring regina::python::doc::CensusHit::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the given census hits.

This global routine simply calls CensusHit::swap(); it is provided so
that CensusHit meets the C++ Swappable requirements.

Parameter ``a``:
    the first census hit whose contents should be swapped.

Parameter ``b``:
    the second census hit whose contents should be swapped.)doc";

// Docstring regina::python::doc::CensusHit::name
static constexpr const char name[] =
R"doc(Returns the human-readable name associated with the triangulation or
link diagram in the database. This would typically contain the name of
the triangulation, the link, and/or the underlying manifold.

Returns:
    the human-readable name for this hit.)doc";

// Docstring regina::python::doc::CensusHit::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given census hit.

Parameter ``other``:
    the census hit whose contents are to be swapped with this.)doc";

}; // struct CensusHit

struct CensusSearchable {

// Docstring regina::python::doc::CensusSearchable::__concept
static constexpr const char __concept[] =
R"doc(A type of mathematical object that can be looked up within Regina's
in-built census databases.

This concept refers to the underlying mathematical types that the
databases encode (e.g., triangulation or link types), not the strings
that are actually used to encode these object within the databases.

Currently the only supported types here are Triangulation<3> and Link.
This list may grow as more census data ships in future versions of
Regina.

This concept includes ``SignatureEncodable<T>`` as an explicit
requirement. Although this requirement is redundant (since the full
list of supported types is already hard-coded directly in this
concept), it is kept as a reminder that census databases require
signatures to use as lookup keys.

Concepts:
    CensusSearchable is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct CensusSearchable

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

