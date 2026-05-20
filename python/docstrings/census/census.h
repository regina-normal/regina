/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::global_swap_CensusDB
inline constexpr const char global_swap_CensusDB[] =
R"doc(Swaps the contents of the given database references.

This global routine simply calls CensusDB::swap(); it is provided so
that CensusDB meets the C++ Swappable requirements.

Parameter ``a``:
    the first database reference whose contents should be swapped.

Parameter ``b``:
    the second database reference whose contents should be swapped.)doc";

// Docstring regina::python::doc::global_swap_CensusHit
inline constexpr const char global_swap_CensusHit[] =
R"doc(Swaps the contents of the given census hits.

This global routine simply calls CensusHit::swap(); it is provided so
that CensusHit meets the C++ Swappable requirements.

Parameter ``a``:
    the first census hit whose contents should be swapped.

Parameter ``b``:
    the second census hit whose contents should be swapped.)doc";

struct Census {

// Docstring regina::python::doc::Census::__class
static constexpr const char __class[] =
R"doc(A utility class used to search for triangulations across one or more
3-manifold census databases.

This class consists of static routines only. The main entry point (and
typically the only way that you would use this class) is via the
various static lookup() routines.

.. warning::
    This class is not thread-safe, in that it performs some global
    initialisation the first time one of the lookup() functions is
    called. If you need thread-safety, you can always call lookup()
    with an empty string when initialising your program, and ensure
    this has finished before you allow any subsequent "normal" calls
    to lookup() from other threads.)doc";

// Docstring regina::python::doc::Census::lookup
static constexpr const char lookup[] =
R"doc(Searches for the given triangulation through all of Regina's in-built
census databases.

Internally, the census databases store isomorphism signatures as
opposed to fully fleshed-out triangulations. If you already have an
isomorphism signature for the triangulation, then you can call the
variant lookup(const std::string&) instead, which (if your signature
is of the right generation) will be faster since it avoids some extra
overhead.

Note that there may be many hits (possibly from multiple databases,
and in some cases possibly even within the same database). Therefore a
_list_ of hits will be returned, which you can iterate through the
individual matches. Even if there are no matches at all, a list will
still be returned; you can call empty() on this list to test whether
any matches were found.

This routine is fast: it first computes the relevant isomorphism
signature of the triangulation, and then performs a logarithmic-time
lookup in each database (here "logarithmic" means logarithmic in the
size of the database).

Parameter ``tri``:
    the triangulation that you wish to search for.

Returns:
    a list of all database matches.)doc";

// Docstring regina::python::doc::Census::lookup_2
static constexpr const char lookup_2[] =
R"doc(Searches for the given triangulation through all of Regina's in-built
census databases.

For this routine you specify the triangulation by giving its
isomorphism signature. This may be either second-generation (as
returned by ``Triangulation<dim>::neoSig()``), or first-generation (as
returned by ``Triangulation<dim>::isoSig()``); either form of
signature will yield the same results.

Calling lookup() on an isomorphism signature _may_ be faster than
calling lookup() on the triangulation itself:

* If the signature is of the same generation as is used internally by
  the census databases, this will avoid some overhead (since the
  triangulation variant must compute the signature to use as a lookup
  key).

* If the signature is of a different generation from the one used
  internally by the census databases, then this will add overhead
  (since this routine will need to reconstruct the triangulation and
  then compute the generation of signature that it needs to perform
  the internal database lookups).

A general rule of thumb is this: if you already have an isomorphism
signature, you should call this string-based routine (regardless of
which generation of signature you have), since reconstruction is
reasonably fast. If you do not already have an isomorphism signature,
just call the triangulation-based lookup routine.

Note that there may be many hits (possibly from multiple databases,
and in some cases possibly even within the same database). Therefore a
_list_ of hits will be returned, which you can iterate through the
individual matches. Even if there are no matches at all, a list will
still be returned; you can call empty() on this list to test whether
any matches were found.

This routine is fast: it first recomputes the generation of
isomorphism signature that it needs (but only if the given signature
is not already of the correct generation), and then it performs a
logarithmic-time lookup in each database (where "logarithmic" means
logarithmic in the size of the database).

Parameter ``isoSig``:
    the isomorphism signature of the triangulation that you wish to
    search for; this may be either first-generation or second-
    generation.

Returns:
    a list of all database matches.)doc";

}; // struct Census

struct CensusDB {

// Docstring regina::python::doc::CensusDB::__class
static constexpr const char __class[] =
R"doc(Stores the location and description of one of Regina's in-built census
databases.

A census database stores a list of key-value pairs. The keys are
isomorphism signatures of triangulations (currently second-generation
signatures as returned by ``Triangulation<dim>::neoSig()``, but see
the notes below). The values are human-readable names (typically the
names of the triangulations and/or the names of the underlying
manifolds). A key may appear multiple times (associated with different
human-readable names) within the same database.

Ordinary users should not need to interact with CensusDB directly;
instead you would typically use one of the high-level Census::lookup()
routines, which searches all of Regina's in-built databases using the
correct type of search key(s). There are two reasons for this:

* The _keys_ used for census databases are subject to change in future
  versions of Regina. Currently (as of Regina 8.0) these keys are
  second-generation isomorphism signatures.

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

Two databases are considered the same if they have identical filenames
(as returned by the filename() function). The database descriptions
are irrelevant here.

Parameter ``rhs``:
    the database to compare this against.

Returns:
    ``True`` if and only if this and the given object represent the
    same database.)doc";

// Docstring regina::python::doc::CensusDB::__init
static constexpr const char __init[] =
R"doc(Creates a new reference to one of Regina's census databases.

This constructor will not run any checks (e.g., it will not verify
that the database exists, or that it is stored in the correct format).
Note that even if the database does not exist, the lookupKey() routine
will fail gracefully.

Parameter ``filename``:
    the filename where the database is stored.

Parameter ``desc``:
    a human-readable description of the database. See the desc()
    routine for further information on how this description might be
    used.)doc";

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

The argument *key* should be a *g*th-generation isomorphism signature,
where *g* is passed as a template argument. Only one value of *g* is
allowed: the same generation of signature that the database uses
internally for its keys. This means that the way you call lookupKey()
will change if/when the database key type changes; this is by design,
since you will of course need to change what you pass as an argument
also.

If you are using this routine yourself, you will need to include the
extra header census/census-impl.h (which is _not_ automatically
included from this file). Typically, however, end users can simply use
the catch-all Census::lookup() routines and will not need to call this
more fine-grained routine.

Python:
    This function is available in Python, and the *action* argument
    may be a pure Python function. Since Python does not support C++
    templates, the generation should be passed as an initial argument
    at runtime: ``lookupKey(generation, isoSig, action)``. If
    *generation* does not match the one allowed value (i.e., the
    format used internally by the database), then this routine will
    throw an InvalidArgument exception.

Template parameter ``generation``:
    the generation of isomorphism signature that you are passing in
    the argument *isoSig*. Currently *generation* _must_ be 2, since
    Regina's databases currently use second-generation isomorphism
    signatures as their keys.

Parameter ``isoSig``:
    the isomorphism signature to search for; this must be of the same
    generation that is passed as a template parameter.

Parameter ``action``:
    a function (or other callable type) that will be called for each
    match in the database.

Returns:
    ``True`` if the lookup was correctly performed, or ``False`` if
    some error occurred (e.g., the database could not be opened). Note
    in particular that if there were no matches but no errors, then
    the return value will be ``True``.)doc";

// Docstring regina::python::doc::CensusDB::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given database reference.

Parameter ``other``:
    the database reference whose contents are to be swapped with this.)doc";

}; // struct CensusDB

struct CensusHit {

// Docstring regina::python::doc::CensusHit::__class
static constexpr const char __class[] =
R"doc(Stores a single "hit" indicating that some given triangulation has
been located in one of Regina's in-built census databases.

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
R"doc(Returns details of the census database in which the triangulation was
found.

Returns:
    the database for this hit.)doc";

// Docstring regina::python::doc::CensusHit::name
static constexpr const char name[] =
R"doc(Returns the human-readable name associated with the triangulation in
the database. This typically contains the name of the triangulation
and/or the name of the underlying manifold.

Returns:
    the human-readable name for this hit.)doc";

// Docstring regina::python::doc::CensusHit::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given census hit.

Parameter ``other``:
    the census hit whose contents are to be swapped with this.)doc";

}; // struct CensusHit

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

