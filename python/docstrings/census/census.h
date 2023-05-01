/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Census
static const char *Census =
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

// Docstring regina::python::doc::CensusDB
static const char *CensusDB =
R"doc(Stores the location and description of one of Regina's in-built census
databases.

A census database stores a list of key-value pairs. The keys are
isomorphism signatures of triangulations (as returned by
Triangulation<3>::isoSig(), for instance), and the values are human-
readable names (typically the names of the triangulations and/or the
names of the underlying manifolds). An isomorphism signature may
appear multiple times (with different names) within the same database.

The format used to store census databases is an internal
implementation detail that may change in future releases of Regina.
End users should only search census databases using high-level
routines such as Census::lookup() and CensusDB::lookup().

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::CensusHit
static const char *CensusHit =
R"doc(Stores a single "hit" indicating that some given triangulation has
been located in one of Regina's in-built census databases.

You cannot construct or modify instances of this class yourself, other
than through the standard copy/move/swap operations. The only way to
create "genuinely" new objects of this class is via the various static
Census::lookup() routines.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace CensusDB_ {

// Docstring regina::python::doc::CensusDB_::__copy
static const char *__copy = R"doc(Creates a new clone of the given database reference.)doc";

// Docstring regina::python::doc::CensusDB_::__eq
static const char *__eq =
R"doc(Tests whether this and the given object represent the same database.

Two databases are considered the same if they have identical filenames
(as returned by the filename() function). The database descriptions
are irrelevant here.

Parameter ``rhs``:
    the database to compare this against.

Returns:
    ``True`` if and only if this and the given object represent the
    same database.)doc";

// Docstring regina::python::doc::CensusDB_::__init
static const char *__init =
R"doc(Creates a new reference to one of Regina's census databases.

This constructor will not run any checks (e.g., it will not verify
that the database exists, or that it is stored in the correct format).
Note that even if the database does not exist, the lookup() routine
will fail gracefully.

Parameter ``filename``:
    the filename where the database is stored.

Parameter ``desc``:
    a human-readable description of the database. See the desc()
    routine for further information on how this description might be
    used.)doc";

// Docstring regina::python::doc::CensusDB_::__ne
static const char *__ne =
R"doc(Tests whether this and the given object represent different databases.

Two databases are considered the same if they have identical filenames
(as returned by the filename() function). The database descriptions
are irrelevant here.

Parameter ``rhs``:
    the database to compare this against.

Returns:
    ``True`` if and only if this and the given object represent
    different databases.)doc";

// Docstring regina::python::doc::CensusDB_::desc
static const char *desc =
R"doc(Returns a human-readable description of this database.

The description could (for instance) be shown to users when giving a
list of all available databases, or when identifying in which
particular database a match was found.

Returns:
    the database description.)doc";

// Docstring regina::python::doc::CensusDB_::filename
static const char *filename =
R"doc(Returns the filename where this database is stored.

Returns:
    the database filename.)doc";

// Docstring regina::python::doc::CensusDB_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given database references.

This global routine simply calls CensusDB::swap(); it is provided so
that CensusDB meets the C++ Swappable requirements.

Parameter ``a``:
    the first database reference whose contents should be swapped.

Parameter ``b``:
    the second database reference whose contents should be swapped.)doc";

// Docstring regina::python::doc::CensusDB_::lookup
static const char *lookup =
R"doc(Searches for the given isomorphism signature in this database.

For each match that is found (if any), this routine will call *action*
(which must be a function or some other callable object). This action
should return ``void``, and must take exactly one CensusHit argument.
The argument will be passed as a prvalue, which means the argument
type for *action* could be any of (CensusHit), (const CensusHit&), or
(CensusHit&&).

Note that the database will be opened and closed every time this
routine is called.

If the given isomorphism signature is empty then this routine will
return ``True`` immediately (i.e., it will be treated as successful
with no hits but it will not actually search the database).

If you are using this routine yourself, you will need to include the
extra header census/census-impl.h (which is _not_ automatically
included from this file). Typically, however, end users can simply use
the catch-all Census::lookup() routines and will not need to call this
more fine-grained routine.

Python:
    This function is available in Python, and the *action* argument
    may be a pure Python function.

Parameter ``isoSig``:
    the isomorphism signature to search for.

Parameter ``action``:
    a function (or other callable object) that will be called for each
    match in the database.

Returns:
    ``True`` if the lookup was correctly performed, or ``False`` if
    some error occurred (e.g., the database could not be opened). Note
    in particular that if there were no matches but no errors, then
    the return value will be ``True``.)doc";

// Docstring regina::python::doc::CensusDB_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given database reference.

Parameter ``other``:
    the database reference whose contents are to be swapped with this.)doc";

}

namespace CensusHit_ {

// Docstring regina::python::doc::CensusHit_::__copy
static const char *__copy = R"doc(Creates a new copy of the given census hit.)doc";

// Docstring regina::python::doc::CensusHit_::__eq
static const char *__eq =
R"doc(Tests whether this and the given object represent the same census hit.

Two census hits are considered the same if they have the same human-
readable name and also come from the same database (as identified by
the CensusDB comparison operators).

Parameter ``rhs``:
    the census hit to compare this against.

Returns:
    ``True`` if and only if this and the given hit are the same.)doc";

// Docstring regina::python::doc::CensusHit_::__ne
static const char *__ne =
R"doc(Tests whether this and the given object represent different census
hits.

Two census hits are considered the same if they have the same human-
readable name and also come from the same database (as identified by
the CensusDB comparison operators).

Parameter ``rhs``:
    the census hit to compare this against.

Returns:
    ``True`` if and only if this and the given hit are different.)doc";

// Docstring regina::python::doc::CensusHit_::db
static const char *db =
R"doc(Returns details of the census database in which the triangulation was
found.

Returns:
    the database for this hit.)doc";

// Docstring regina::python::doc::CensusHit_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given census hits.

This global routine simply calls CensusHit::swap(); it is provided so
that CensusHit meets the C++ Swappable requirements.

Parameter ``a``:
    the first census hit whose contents should be swapped.

Parameter ``b``:
    the second census hit whose contents should be swapped.)doc";

// Docstring regina::python::doc::CensusHit_::name
static const char *name =
R"doc(Returns the human-readable name associated with the triangulation in
the database. This typically contains the name of the triangulation
and/or the name of the underlying manifold.

Returns:
    the human-readable name for this hit.)doc";

// Docstring regina::python::doc::CensusHit_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given census hit.

Parameter ``other``:
    the census hit whose contents are to be swapped with this.)doc";

}

namespace Census_ {

// Docstring regina::python::doc::Census_::lookup
static const char *lookup =
R"doc(Searches for the given triangulation through all of Regina's in-built
census databases.

Internally, the census databases store isomorphism signatures as
opposed to fully fleshed-out triangulations. If you already have the
isomorphism signature of the triangulation, then you can call the
variant lookup(const std::string&) instead, which will be faster since
it avoids some extra overhead.

Note that there may be many hits (possibly from multiple databases,
and in some cases possibly even within the same database). Therefore a
_list_ of hits will be returned, which you can iterate through the
individual matches. Even if there are no matches at all, a list will
still be returned; you can call empty() on this list to test whether
any matches were found.

This routine is fast: it first computes the isomorphism signature of
the triangulation, and then performs a logarithmic-time lookup in each
database (here "logarithmic" means logarithmic in the size of the
database).

Parameter ``tri``:
    the triangulation that you wish to search for.

Returns:
    a list of all database matches.)doc";

// Docstring regina::python::doc::Census_::lookup_2
static const char *lookup_2 =
R"doc(Searches for the given triangulation through all of Regina's in-built
census databases.

For this routine you specify the triangulation by giving its
isomorphism signature, as returned by Triangulation<3>::isoSig(). This
is faster than the variant lookup(const Triangulation<3>&), since
Regina's census databases store isomorphism signatures internally. If
you do not already know the isomorphism signature, it is fine to just
call lookup(const Triangulation<3>&) instead.

Note that there may be many hits (possibly from multiple databases,
and in some cases possibly even within the same database). Therefore a
_list_ of hits will be returned, which you can iterate through the
individual matches. Even if there are no matches at all, a list will
still be returned; you can call empty() on this list to test whether
any matches were found.

This routine is fast: it first computes the isomorphism signature of
the triangulation, and then performs a logarithmic-time lookup in each
database (here "logarithmic" means logarithmic in the size of the
database).

Parameter ``isoSig``:
    the isomorphism signature of the triangulation that you wish to
    search for.

Returns:
    a list of all database matches.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

