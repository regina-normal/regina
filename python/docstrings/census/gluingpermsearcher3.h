/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::ClosedPrimeMinSearcher
static const char *ClosedPrimeMinSearcher =
R"doc(A gluing permutation search class that offers a specialised search
algorithm for when (i) only closed prime minimal P2-irreducible
triangulations are required, and (ii) the given face pairing has order
at least three.

The search algorithm is significantly different from the default
algorithm provided by GluingPermSearcher<3>. It is heavily optimised
and takes advantage of a number of results regarding the underlying
face pairing graph.

Note that additional unwanted triangulations (e.g., non-prime or non-
minimal triangulations) may still be produced by this search. However,
significantly fewer unwanted triangulations will be produced when
using this class instead of GluingPermSearcher<3>.

This class is designed to manage the construction of a large census of
triangulations, and so it does not support copying, moving or
swapping.)doc";

// Docstring regina::python::doc::CompactSearcher
static const char *CompactSearcher =
R"doc(A gluing permutation search class that offers a specialised search
algorithm for when only compact (finite) 3-manifold triangulations are
required. The only constraints placed upon a triangulation are that
every edge must be valid (i.e., not identified with itself in
reverse), and that the link of every vertex must be a disk or a
sphere.

The search algorithm uses modified union-find structures on both edge
and vertex equivalence classes to prune searches that are guaranteed
to lead to bad edge or vertex links. For details see "Enumeration of
non-orientable 3-manifolds using face-pairing graphs and union-find",
Benjamin A. Burton, Discrete Comput. Geom. 38 (2007), no. 3, 527--571;
and "Detecting genus in vertex links for the fast enumeration of
3-manifold triangulations", Benjamin A. Burton, in "ISSAC 2011:
Proceedings of the 36th International Symposium on Symbolic and
Algebraic Computation", ACM, 2011, pp. 59-66.

No additional unwanted triangulations will be produced by this search
(in contrast to other search classes, such as ClosedPrimeMinSearcher).
That is, _only_ compact 3-manifolds will be produced.

This class is designed to manage the construction of a large census of
triangulations, and so it does not support copying, moving or
swapping.)doc";

// Docstring regina::python::doc::EulerSearcher
static const char *EulerSearcher =
R"doc(A gluing permutation search class that offers a specialised search
algorithm for when all vertex links must all have a given fixed Euler
characteristic. Examples might be Euler characteristic 2 (for closed
manifolds), or Euler characteristic 0 (for manifolds with torus and/or
Klein bottle cusps). In addition, we require that every edge must be
valid (i.e., not identified with itself in reverse).

Vertices on boundary triangles are treated a little differently. If
the underlying face pairing includes boundary triangles and the given
Euler characteristic is *E*, then boundary vertex links must have
Euler characteristic *E*-1, and must have exactly one puncture. For
instance, if *E* is 2 and the face pairing includes boundary faces,
then all vertex links must be either spheres (for internal vertices)
or discs (for boundary vertices).

The search algorithm uses modified union-find structures on both edge
and vertex equivalence classes to prune searches that are guaranteed
to lead to bad edge or vertex links. For details see "Enumeration of
non-orientable 3-manifolds using face-pairing graphs and union-find",
Benjamin A. Burton, Discrete Comput. Geom. 38 (2007), no. 3, 527--571;
and "Detecting genus in vertex links for the fast enumeration of
3-manifold triangulations", Benjamin A. Burton, in "ISSAC 2011:
Proceedings of the 36th International Symposium on Symbolic and
Algebraic Computation", ACM, 2011, pp. 59-66.

No additional unwanted triangulations will be produced by this search
(in contrast to other search classes, such as ClosedPrimeMinSearcher).
That is, _only_ 3-manifolds with the required vertex links will be
produced.

This class is designed to manage the construction of a large census of
triangulations, and so it does not support copying, moving or
swapping.)doc";

// Docstring regina::python::doc::GluingPermSearcher
static const char *GluingPermSearcher =
R"doc(A utility class for searching through all possible gluing permutation
sets that correspond to a given tetrahedron face pairing. Subclasses
of GluingPermSearcher<3> correspond to specialised (and heavily
optimised) search algorithms that may be used in sufficiently
constrained scenarios. The main class GluingPermSearcher<3> offers a
default (but slower) search algorithm that may be used in more general
contexts.

The simplest way of performing a search through all possible gluing
permutations is by calling the static method findAllPerms(). This will
examine the search parameters and ensure that the best possible
algorithm is used. For finer control over the program flow, the static
method bestSearcher() can be used to create a search manager of the
most suitable class and then runSearch() can be called on this object
directly. For absolute control, a specific algorithm can be forced by
explicitly constructing an object of the corresponding class (and
again calling runSearch() on that object directly).

This class is designed to manage the construction of a large census of
triangulations, and so it does not support copying, moving or
swapping.)doc";

// Docstring regina::python::doc::HyperbolicMinSearcher
static const char *HyperbolicMinSearcher =
R"doc(A gluing permutation search class that offers a specialised search
algorithm for when only minimal ideal triangulations of cusped finite-
volume hyperbolic 3-manifolds are required. Here every vertex link
will be a torus or Klein bottle.

Note that this searches for _any_ triangulations of such hyperbolic
manifolds, not just _geometric_ triangulations.

Note that additional unwanted triangulations (e.g., non-hyperbolic or
non-minimal triangulations) may still be produced by this search.
However, significantly fewer unwanted triangulations will be produced
when using this class instead of GluingPermSearcher<3>.

This class is designed to manage the construction of a large census of
triangulations, and so it does not support copying, moving or
swapping.)doc";

namespace ClosedPrimeMinSearcher_ {

// Docstring regina::python::doc::ClosedPrimeMinSearcher_::__init
static const char *__init =
R"doc(Creates a new search manager for use when (i) only closed prime
minimal P2-irreducible triangulations are required, and (ii) the given
face pairing has order at least three. Note that other unwanted
triangulations may still be produced (e.g., non-prime or non-minimal
triangulations), but there will be far fewer of these than when using
the GluingPermSearcher<3> class directly.

For details on how a search manager is used, see the
GluingPermSearcher<3> documentation. Note in particular that this
class will be automatically used by
GluingPermSearcher<3>::findAllPerms() if possible, so there is often
no need for an end user to instantiate this class directly.

All constructor arguments are the same as for the
GluingPermSearcher<3> constructor, though some arguments (such as
*finiteOnly* and *purge*) are not needed here since they are already
implied by the specialised search context.

Precondition:
    The given face pairing is connected, i.e., it is possible to reach
    any tetrahedron from any other tetrahedron via a series of matched
    face pairs.

Precondition:
    The given face pairing is in canonical form as described by
    FacetPairing<3>::isCanonical(). Note that all face pairings
    constructed by FacetPairing<3>::findAllPairings() are of this
    form.

Precondition:
    The given face pairing has no boundary faces and has at least
    three tetrahedra.)doc";

}

namespace CompactSearcher_ {

// Docstring regina::python::doc::CompactSearcher_::__init
static const char *__init =
R"doc(Creates a new search manager for use when only compact 3-manifold
triangulations are required.

For details on how a search manager is used, see the
GluingPermSearcher<3> documentation. Note in particular that this
class will be automatically used by
GluingPermSearcher<3>::findAllPerms() if possible, so there is often
no need for an end user to instantiate this class directly.

All constructor arguments are the same as for the
GluingPermSearcher<3> constructor, though some arguments (such as
*finiteOnly*) are not needed here since they are already implied by
the specialised search context.

Precondition:
    The given face pairing is connected, i.e., it is possible to reach
    any tetrahedron from any other tetrahedron via a series of matched
    face pairs.

Precondition:
    The given face pairing is in canonical form as described by
    FacetPairing<3>::isCanonical(). Note that all face pairings
    constructed by FacetPairing<3>::findAllPairings() are of this
    form.)doc";

}

namespace EulerSearcher_ {

// Docstring regina::python::doc::EulerSearcher_::__init
static const char *__init =
R"doc(Creates a new search manager that restricts Euler characteristic on
the vertex links, as described in the class overview.

For details on how a search manager is used, see the
GluingPermSearcher<3> documentation. Note in particular that this
class will be automatically used by
GluingPermSearcher<3>::findAllPerms() if possible, so there is often
no need for an end user to instantiate this class directly.

All constructor arguments except for *euler* are the same as for the
GluingPermSearcher<3> constructor, though some arguments (such as
*finiteOnly*) are not needed here since they are already implied by
the specialised search context.

Precondition:
    The given Euler characteristic is at most 2.

Precondition:
    The given face pairing is connected, i.e., it is possible to reach
    any tetrahedron from any other tetrahedron via a series of matched
    face pairs.

Precondition:
    The given face pairing is in canonical form as described by
    FacetPairing<3>::isCanonical(). Note that all face pairings
    constructed by FacetPairing<3>::findAllPairings() are of this
    form.

Parameter ``useEuler``:
    the Euler characteristic that vertex links must have. For boundary
    vertices, this is the Euler characteristic of the closed surface
    that would be obtained if the puncture in the vertex link were
    filled.)doc";

}

namespace GluingPermSearcher_ {

// Docstring regina::python::doc::GluingPermSearcher_::__init
static const char *__init =
R"doc(Initialises a new search for gluing permutation sets. The search is
started by calling runSearch(). Note that the static method
findAllPerms() handles both construction and searching, and is the
preferred entry point for end users.

The arguments to this constructor describe the search parameters in
detail.

Parameter *purge* may be used to avoid constructing permutation sets
that correspond to triangulations satisfying certain constraints (such
as non-minimality). The use of this parameter, combined with
parameters *orientableOnly* and *finiteOnly*, can significantly speed
up the permutation set generation. For some combinations of these
parameters entirely different algorithms are used.

Note that not all permutation sets described by parameter *purge* will
be avoided (i.e., you may get gluing permutation sets that you did not
want). It is guaranteed however that every permutation set whose
corresonding triangulation does _not_ satisfy the *purge* constraints
will be generated.

Similarly, even if *finiteOnly* is set to ``True``, some non-finite
triangulations might still slip through the net (since the full vertex
links are not always constructed). However, like *purge*, setting
*finiteOnly* to ``True`` allow the census algorithm to take shortcuts
and therefore run faster. The resulting triangulations may be tested
for finiteness (and other properties) by calling triangulate().

Precondition:
    The given face pairing is connected, i.e., it is possible to reach
    any tetrahedron from any other tetrahedron via a series of matched
    face pairs.

Precondition:
    The given face pairing is in canonical form as described by
    FacetPairing<3>::isCanonical(). Note that all face pairings
    constructed by FacetPairing<3>::findAllPairings() are of this
    form.

Parameter ``pairing``:
    the specific pairing of tetrahedron faces that the generated
    permutation sets will complement.

Parameter ``autos``:
    the collection of isomorphisms that define equivalence of
    permutation sets. These are used by runSearch(), which produces
    each permutation set precisely once up to equivalence. These
    isomorphisms must all be automorphisms of the given face pairing,
    and will generally be the set of all such automorphisms (which you
    can generate via ``pairing.findAutomorphisms()``).

Parameter ``orientableOnly``:
    ``True`` if only gluing permutations corresponding to orientable
    triangulations should be generated, or ``False`` if no such
    restriction should be imposed.

Parameter ``finiteOnly``:
    ``True`` if only gluing permutations corresponding to finite
    triangulations are required, or ``False`` if there is no such
    requirement. Note that regardless of this value, some non-finite
    triangulations might still be produced; see the notes above for
    details.

Parameter ``purge``:
    specifies which permutation sets we may avoid constructing (see
    the function notes above for details). This should be a bitwise OR
    of constants from the CensusPurge enumeration, or else
    ``CensusPurge::None`` (or just empty braces ``{}``) if we should
    simply generate every possible permutation set. If several purge
    constants are bitwise ORed together, then permutation sets whose
    triangulation satisfies _any_ of these constraints might be
    avoided. Note that not _all_ such permutation sets will be
    avoided, but enough are avoided that the performance increase is
    noticeable.)doc";

// Docstring regina::python::doc::GluingPermSearcher_::bestSearcher
static const char *bestSearcher =
R"doc(Constructs a search manager of the best possible class for the given
search parameters. Different subclasses of GluingPermSearcher<3>
provide optimised search algorithms for different types of search.

Calling this routine and then calling runSearch() on the result has
the same effect as the all-in-one routine findAllPerms(). Unless you
have specialised requirements (such as partial searching), you are
probably better calling findAllPerms() instead.

See the GluingPermSearcher<3> constructor for documentation on the
arguments to this routine.

Precondition:
    The given face pairing is connected, i.e., it is possible to reach
    any tetrahedron from any other tetrahedron via a series of matched
    face pairs.

Precondition:
    The given face pairing is in canonical form as described by
    FacetPairing<3>::isCanonical(). Note that all face pairings
    constructed by FacetPairing<3>::findAllPairings() are of this
    form.

Returns:
    the new search manager.)doc";

// Docstring regina::python::doc::GluingPermSearcher_::data
static const char *data =
R"doc(Returns all internal data in a plain text format. This object can be
recreated from this text data by calling the input stream constructor
for the appropriate class.

This routine may be useful for transferring objects from one processor
to another.

If subclasses override this function, they should write subclass data
after superclass data. This means it is safe to dump data from a
subclass and then recreate a new superclass object from that data
(though subclass-specific information will be lost).

This routine returns the same information that dumpData() writes.

The key difference between data() and taggedData() is that
taggedData() preserves all internal information even if this object
belongs to a subclass of GluingPermSearcher, whereas data() only
writes information pertaining to this base class.

.. warning::
    The data format is liable to change between Regina releases. Data
    in this format should be used on a short-term temporary basis
    only.

Python:
    This routine is available, but the matching input stream
    constructor is not. Python users should use taggedData() and
    fromTaggedData() instead.

Returns:
    all of this object's internal data in plain text format.)doc";

// Docstring regina::python::doc::GluingPermSearcher_::findAllPerms
static const char *findAllPerms =
R"doc(The main entry routine for running a search for all gluing permutation
sets that complement a given face pairing.

This routine examines the search parameters, chooses the best possible
search algorithm, constructs an object of the corresponding subclass
of GluingPermSearcher<3> and then calls runSearch().

See the GluingPermSearcher<3> constructor for documentation on the
arguments to this routine. See the runSearch() method for
documentation on how the search runs and returns its results via
*action* and *args*..

Precondition:
    The given face pairing is connected, i.e., it is possible to reach
    any tetrahedron from any other tetrahedron via a series of matched
    face pairs.

Precondition:
    The given face pairing is in canonical form as described by
    FacetPairing<3>::isCanonical(). Note that all face pairings
    constructed by FacetPairing<3>::findAllPairings() are of this
    form.

Python:
    This function is available, and *action* may be a pure Python
    function. However, *action* cannot take any additional arguments
    beyond the initial gluing permutation set (and therefore the
    additional *args* list is omitted here).)doc";

// Docstring regina::python::doc::GluingPermSearcher_::fromTaggedData
static const char *fromTaggedData =
R"doc(Creates a new search manager based on tagged data stored in the given
string. This may be a new search or a partially completed search.

The tagged data should be in the format returned by taggedData(). The
precise class of the search manager will be determined from the tagged
data, and does not need to be known in advance. This is in contrast to
dumpData() and the input stream constructors, where the class of the
data being read must be known at compile time.

.. warning::
    The data format is liable to change between Regina releases. Data
    in this format should be used on a short-term temporary basis
    only.

Exception ``InvalidArgument``:
    The data found in the given string is invalid, incomplete, or
    incorrectly formatted.

Parameter ``data``:
    the tagged data from which to reconstruct a search manager.

Returns:
    the new search manager, or ``None`` if the data in the given
    string was invalid or incorrectly formatted.)doc";

// Docstring regina::python::doc::GluingPermSearcher_::isComplete
static const char *isComplete =
R"doc(Determines whether this search manager holds a complete gluing
permutation set or just a partially completed search state.

This may assist the *action* routine when running partial depth-based
searches. See partialSearch() for further details.

Returns:
    ``True`` if a complete gluing permutation set is held, or
    ``False`` otherwise.)doc";

// Docstring regina::python::doc::GluingPermSearcher_::partialSearch
static const char *partialSearch =
R"doc(Runs a partial search for all possible gluing permutations that
satisfy the search criteria, branching only to the given depth and no
further.

This routine essentially does some but not all of the work of
runSearch(). See the runSearch() documentation for a detailed overview
of what the full search aims to achieve.

If runSearch() enumerates an entire search tree, then you can think of
partialSearch() as only enumerating the first *maxDepth* levels of
this search tree. Rather than producing complete gluing permutation
sets, this search will produce a series of partially-constructed
permutation sets. A partial search can be continued by calling
runSearch() again on the underlying GluingPermSearcher (perhaps after
being frozen, or passed on to a different processor via taggedData()
and fromTaggedData()). If necessary, the *action* routine may call
isComplete() to distinguish between a complete set of gluing
permutations and a partial search state.

Note that a restarted search will never drop below its initial depth.
That is, calling runSearch() with a fixed depth can be used to
subdivide the overall search space into many branches, and then
calling runSearch() on each resulting partial search will complete
each of these branches without overlap.

If the search tree is shallow enough (or if *maxDepth* is large
enough), it is possible that this routine will produce complete gluing
permutation sets.

Python:
    This function is available, and *action* may be a pure Python
    function. However, *action* cannot take any additional arguments
    beyond the initial gluing permutation set (and therefore the
    additional *args* list is omitted here).

Parameter ``maxDepth``:
    the depth of the partial search to run. A negative number
    indicates that a full search should be run.

Parameter ``action``:
    a function (or other callable object) to call for each permutation
    set (partial or complete) that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial permutation set argument.)doc";

// Docstring regina::python::doc::GluingPermSearcher_::runSearch
static const char *runSearch =
R"doc(Generates all possible gluing permutation sets that satisfy the
current search criteria. The search criteria are specified in the
class constructor, or through the static method findAllPerms().

Each set of gluing permutations will be produced precisely once up to
equivalence, where equivalence is defined by the given set of
automorphisms of the given face pairing.

For each permutation set that is generated, this routine will call
*action* (which must be a function or some other callable object).

* The first argument to *action* must be a const reference to a
  GluingPerms<3>. This will be the permutation set that was found. If
  *action* wishes to keep the permutation set, it should take a deep
  copy (not a reference), since the permutation set may be changed and
  reused after *action* returns.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return ``void``.

It is possible to run only a partial search, branching to a given
depth but no further; for this you should use the separate routine
partialSearch(), not runSearch().

Python:
    This function is available, and *action* may be a pure Python
    function. However, *action* cannot take any additional arguments
    beyond the initial gluing permutation set (and therefore the
    additional *args* list is omitted here).

Parameter ``action``:
    a function (or other callable object) to call for each permutation
    set that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial permutation set argument.)doc";

// Docstring regina::python::doc::GluingPermSearcher_::taggedData
static const char *taggedData =
R"doc(Returns all internal data in a plain text format, along with a marker
to signify which precise class the data belongs to. This routine can
be used with fromTaggedData() to transport objects from place to place
whose precise class is unknown.

This routine returns the same information that dumpTaggedData()
writes.

The key difference between data() and taggedData() is that
taggedData() preserves all internal information even if this object
belongs to a subclass of GluingPermSearcher, whereas data() only
writes information pertaining to this base class.

.. warning::
    The data format is liable to change between Regina releases. Data
    in this format should be used on a short-term temporary basis
    only.

Returns:
    all of this object's internal data in plain text format.)doc";

}

namespace HyperbolicMinSearcher_ {

// Docstring regina::python::doc::HyperbolicMinSearcher_::__init
static const char *__init =
R"doc(Creates a new search manager for use when only minimal ideal
triangulations of cusped finite-volume hyperbolic 3-manifolds are
required. Here every vertex link will be a torus or Klein bottle. Note
that other unwanted triangulations may still be produced (e.g., non-
hyperbolic or non-minimal triangulations), but there will be far fewer
of these than when using the GluingPermSearcher<3> class directly.

For details on how a search manager is used, see the
GluingPermSearcher<3> documentation.

All constructor arguments are the same as for the
GluingPermSearcher<3> constructor, though some arguments (such as
*finiteOnly* and *purge*) are not needed here since they are already
implied by the specialised search context.

Precondition:
    The given face pairing is connected, i.e., it is possible to reach
    any tetrahedron from any other tetrahedron via a series of matched
    face pairs.

Precondition:
    The given face pairing is in canonical form as described by
    FacetPairing<3>::isCanonical(). Note that all face pairings
    constructed by FacetPairing<3>::findAllPairings() are of this
    form.

Precondition:
    The given face pairing has no boundary faces.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

