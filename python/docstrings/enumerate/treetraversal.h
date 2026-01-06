/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::TautEnumeration
static const char *TautEnumeration =
R"doc(The main entry point for the tree traversal algorithm to enumerate all
taut angle structures in a 3-manifold triangulation. For the analogous
algorithm to enumerate vertex normal or almost normal surfaces, see
the class TreeEnumeration instead.

This class follows a similar structure to the enumeration of vertex
normal surfaces, as described in "A tree traversal algorithm for
decision problems in knot theory and 3-manifold topology", Burton and
Ozlen, Algorithmica 65:4 (2013), pp. 772-801.

To enumerate all taut angle structures on a given 3-manifold
triangulation, simply construct a TautEnumeration object and call
run().

Alternatively, you can have more fine-grained control over the search.
Instead of calling run(), you can construct a TautEnumeration object
and repeatedly call next() to step through each taut angle structure
one at a time. This allows you to pause and resume the search as you
please.

By using appropriate template parameters *Constraint* and/or *Ban*, it
is possible to impose additional linear constraints on the angle
structure solution space, and/or explicitly force particular angles to
be zero. See the LPConstraintAPI and BanConstraintBase documentation
for details.

Note that some constraint classes may cause the TautEnumeration class
constructor to throw an exception; see the constructor documentation
for details.

The template argument *IntType* indicates the integer type that will
be used throughout the underlying linear programming machinery. Unless
you have a good reason to do otherwise, you should use the arbitrary-
precision Integer class (in which integers can grow arbitrarily large,
and overflow can never occur).

This class is designed to manage the execution of a significant
enumeration operation, and so it does not support copying, moving or
swapping.

Python:
    This is a heavily templated class; however, the only *Constraint*
    and *Ban* options currently offered for angle structures are the
    default LPConstraintNone and BanNone. Therefore Python offers just
    one instance of this class (with all template arguments set to
    their defaults), under the name ``TautEnumeration``.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::TreeEnumeration
static const char *TreeEnumeration =
R"doc(The main entry point for the tree traversal algorithm to enumerate all
vertex normal or almost normal surfaces in a 3-manifold triangulation.
For the analogous algorithm to enumerate taut angle structures, see
the class TautEnumeration instead.

This class essentially implements the algorithm from "A tree traversal
algorithm for decision problems in knot theory and 3-manifold
topology", Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801.

To enumerate all vertex surfaces for a given 3-manifold triangulation,
simply construct a TreeEnumeration object and call run().

Alternatively, you can have more fine-grained control over the search.
Instead of calling run(), you can construct a TreeEnumeration object
and repeatedly call next() to step through each vertex surface one at
a time. This allows you to pause and resume the search as you please.

If you simply wish to detect a single non-trivial solution under
additional constraints (such as positive Euler characteristic), then
use the class TreeSingleSoln instead, which is optimised for this
purpose.

This tree traversal can only enumerate surfaces in quadrilateral
normal coordinates (NormalCoords::Quad), standard normal coordinates
(NormalCoords::Standard), quadrilateral-octagon almost normal
coordinates (NormalCoords::QuadOct), or standard almost normal
coordinates (NormalCoords::AlmostNormal). For almost normal surfaces,
we allow any number of octagons (including zero), but we only allow at
most one octagon _type_ in the entire triangulation. No coordinate
systems other than these are supported.

By using appropriate template parameters *Constraint* and/or *Ban*, it
is possible to impose additional linear constraints on the normal
surface solution cone, and/or explicitly force particular normal
coordinates to zero. In this case, the notion of "vertex surface" is
modified to mean a normal surface whose coordinates lie on an extreme
ray of the restricted solution cone under these additional constraints
(and whose coordinates are integers with no common divisor). See the
LPConstraintAPI and BanConstraintBase documentation for details.

Note that some constraint classes may cause the TreeEnumeration class
constructor to throw an exception; see the constructor documentation
for details.

The template argument *IntType* indicates the integer type that will
be used throughout the underlying linear programming machinery. Unless
you have a good reason to do otherwise, you should use the arbitrary-
precision Integer class (in which integers can grow arbitrarily large,
and overflow can never occur).

This class is designed to manage the execution of a significant
enumeration operation, and so it does not support copying, moving or
swapping.

.. warning::
    Although the tree traversal algorithm can run in standard normal
    or almost normal coordinates, this is not recommended: it is
    likely to be _much_ slower than in quadrilateral or quadrilateral-
    octagon coordinates respectively. Instead you should enumerate
    vertex solutions using quadrilateral or quadrilateral-octagon
    coordinates, and then use the "transform constructor"
    ``NormalSurfaces(...,
    NormalTransform::ConvertReducedToStandard)``.

Python:
    This is a heavily templated class; nevertheless, many variants are
    now made available to Python users. Each class name is of the form
    TreeEnumeration_*Constraint*_*Ban*, where the suffixes
    *Constraint* and *Ban* are abbreviated versions of the
    corresponding template parameters; these suffixes are omitted
    entirely for the common cases LPConstraintNone and BanNone. As an
    example, to enumerate non-spun normal surfaces in an ideal
    3-manifold triangulation, you would use the Python class
    ``TreeEnumeration_NonSpun``. You are encouraged to look through
    the Regina namespace to see which combinations of constraint
    classes are supported under Python. In all cases, the *IntType*
    parameter is taken to be regina::Integer.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::TreeSingleSoln
static const char *TreeSingleSoln =
R"doc(The main entry point for the tree traversal / branching algorithm to
locate a single non-trivial normal surface satisfying given
constraints within a 3-manifold triangulation. The constraints are
passed using a combination of the template arguments *Constraint* and
*Ban*. Note that some constraint classes may cause the TreeSingleSoln
class constructor to throw an exception; see the constructor
documentation for details.

A common application of this algorithm is to find a surface of
positive Euler characteristic, using the template argument
LPConstraintEulerPositive. This is useful for tasks such as
0-efficiency testing and prime decomposition (when this is done in
standard normal coordinates), and also 3-sphere recognition (when this
is done in standard almost normal coordinates). Indeed, the underlying
algorithm is optimised for precisely this application.

By a "non-trivial" surface, we mean that at least one triangle
coordinate is zero. Philosophically this is to avoid vertex linking
surfaces, though if the triangulation has more than one vertex then
this takes on a different meaning. See the warning on this matter
below.

Be warned that this routine does not eliminate the zero vector, and so
the template argument *Constraint* should include at least one
constraint that eliminates the zero vector (e.g., positive Euler
characteristic). Otherwise this algorithm may simply return the zero
vector, and the information gained will not be very useful.

For any given normal coordinate, this routine will always try setting
that coordinate to zero before it tries setting it to non-zero. In
other words, if it does find a surface satisfying the given
constraints, then it is guaranteed that the set of non-zero coordinate
positions will be minimal (though not necessary a global _minimum_).
In many settings (such as when using LPConstraintEulerPositive), this
guarantees that the final surface (if it exists) will be a vertex
normal or almost normal surface.

The underlying algorithm is described in "A fast branching algorithm
for unknot recognition with experimental polynomial-time behaviour",
Burton and Ozlen, arXiv:1211.1079, and uses significant material from
"A tree traversal algorithm for decision problems in knot theory and
3-manifold topology", Burton and Ozlen, Algorithmica 65:4 (2013), pp.
772-801.

To use this class, i.e., to locate a non-trivial normal or almost
normal surface under the given constraints or to prove that no such
surface exists, you can simply construct a TreeSingleSoln object and
call find(). You can then call buildSurface() to extract the details
of the surface that was found.

If you wish to enumerate _all_ vertex surfaces in a 3-manifold
triangulation (instead of finding just one), you should use the class
TreeEnumeration instead.

This tree traversal can only enumerate surfaces in quadrilateral
normal coordinates (NormalCoords::Quad), standard normal coordinates
(NormalCoords::Standard), quadrilateral-octagon almost normal
coordinates (NormalCoords::QuadOct), or standard almost normal
coordinates (NormalCoords::AlmostNormal). For almost normal surfaces,
we allow any number of octagons (including zero), but we only allow at
most one octagon _type_ in the entire triangulation. No coordinate
systems other than these are supported.

The template argument *IntType* indicates the integer type that will
be used throughout the underlying linear programming machinery. Unless
you have a good reason to do otherwise, you should use the arbitrary-
precision Integer class (in which integers can grow arbitrarily large,
and overflow can never occur).

This class is designed to manage the execution of a significant search
operation, and so it does not support copying, moving or swapping.

.. warning::
    Typically one should only use this class with _one-vertex_
    triangulations (since otherwise, setting at least one triangle
    coordinate to zero is not enough to rule out trivial vertex
    linking surfaces). Of course there may be settings in which
    multiple vertices make sense (for instance, in ideal
    triangulations with multiple cusps, or when using ban
    constraints), and in such settings this class will still work
    precisely as described.

.. warning::
    If you examine the type vector (e.g., by calling typeString() or
    dumpTypes()), be aware that this class merges the old types 0 and
    1 together into a single branch of the search tree. This means
    that type 0 never appears, and that type 1 could indicate _either_
    positive quadrilaterals in the first position, or else no
    quadrilaterals at all.

Python:
    This is a heavily templated class; nevertheless, many variants are
    now made available to Python users. Each class name is of the form
    TreeSingleSoln_*Constraint*_*Ban*, where the suffixes *Constraint*
    and *Ban* are abbreviated versions of the corresponding template
    parameters; these suffixes are omitted entirely for the common
    cases LPConstraintNone and BanNone. As an example, to find a
    normal disc or sphere in a 3-manifold triangulation, you would use
    the Python class ``TreeSingleSoln_EulerPositive``. You are
    encouraged to look through the Regina namespace to see which
    combinations of constraint classes are supported under Python. In
    all cases, the *IntType* parameter is taken to be regina::Integer.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::TreeTraversal
static const char *TreeTraversal =
R"doc(A base class for searches that employ the tree traversal algorithm for
enumerating and locating vertex normal surfaces and taut angle
structures. Users should not use this base class directly; instead use
one of the subclasses TreeEnumeration (for enumerating all vertex
normal surfaces), TautEnumeration (for enumerating all taut angle
structures), or TreeSingleSoln (for locating a single non-trivial
solution under additional constraints, such as positive Euler
characteristic).

For normal surfaces, the full algorithms are described respectively in
"A tree traversal algorithm for decision problems in knot theory and
3-manifold topology", Burton and Ozlen, Algorithmica 65:4 (2013), pp.
772-801, and "A fast branching algorithm for unknot recognition with
experimental polynomial-time behaviour", Burton and Ozlen,
arXiv:1211.1079.

This base class provides the infrastructure for the search tree, and
the subclasses handle the mechanics of the moving through the tree
according to the backtracking search. The domination test is handled
separately by the class TypeTrie, and the feasibility test is handled
separately by the class LPData.

This class holds the particular state of the tree traversal at any
point in time, as described by the current _level_ (indicating our
current depth in the search tree) and _type vector_ (indicating which
branches of the search tree we have followed). For details on these
concepts, see the Algorithmica paper cited above. The key details are
summarised below; throughout this discussion *n* represents the number
of tetrahedra in the underlying triangulation.

* In quadrilateral coordinates, the type vector is a sequence of *n*
  integers, each equal to 0, 1, 2 or 3, where the *i*th integer
  describes the choice of quadrilateral location in the *i*th
  tetrahedron.

* In standard coordinates, the type vector begins with the *n*
  quadrilateral choices outlined above. This is then followed by an
  additional 4*n* integers, each equal to 0 or 1; these correspond to
  the 4*n* triangle coordinates, and indicate whether each coordinate
  is zero or non-zero.

* In angle structure coordinates, this class identifies taut angle
  structures only. Here the type vector is a sequence of *n* integers,
  each equal to 1, 2 or 3, where the *i*th integer describes the
  location of the two π angles in the *i*th tetrahedron.

In the original Algorithmica paper, we choose types in the order
type_[0], type_[1] and so on, working from the root of the tree down
to the leaves. Here we support a more flexible system: there is an
internal permutation *typeOrder_*, and we choose types in the order
type_[typeOrder_[0]], type_[typeOrder_[1]] and so on. This permutation
may mix quadrilateral and triangle processing, and may even change as
the algorithm runs.

This class can also support octagon types in almost normal surfaces.
However, we still do our linear programming in standard or
quadrilateral coordinates, where we represent an octagon using two
conflicting quadrilaterals in the same tetrahedron (which meet the
tetrahedron boundary in the same set of arcs as a single octagon
would). As with the almost normal coordinate systems in
NormalSurfaces, we allow multiple octagons of the same type, but only
one octagon type in the entire tetrahedron. In the type vector,
octagons are indicated by setting a quadrilateral type to 4, 5 or 6.

There is optional support for adding extra linear constraints (such as
a constraint on Euler characteristic), supplied by the template
parameter *Constraint*. If there are no additional constraints, simply
use the template parameter LPConstraintNone. Note that some constraint
classes may cause the TreeTraveral class constructor to throw an
exception; see the constructor documentation for details.

Also, there is optional support for banning coordinates (i.e.,
insisting that certain coordinates must be set to zero), and/or
marking coordinates (for normal and almost normal surfaces this
affects what is meant by a "non-trivial" surface for the
TreeSingleSoln algorithm; the concept of marking may be expanded
further in future versions of Regina). These options are supplied by
the template parameter *BanConstraint*. If there are no coordinates to
ban or mark, simply use the template parameter *BanNone*.

The template argument *IntType* indicates the integer type that will
be used throughout the underlying linear programming machinery. Unless
you have a good reason to do otherwise, you should use the arbitrary-
precision Integer class (in which integers can grow arbitrarily large,
and overflow can never occur).

Subclasses of TreeTraversal are designed to manage the execution of
significant enumeration and search operations, and so this class does
not support copying, moving or swapping.

Python:
    This is a heavily templated class; moreover, it only serves as a
    base class, and you will most likely not need to access this class
    directly. Instead see the subclasses TreeEnumeration,
    TautEnumeration and TreeSingleSoln, each of which offers a more
    useful interface for solving different type of problems. The
    variants of this TreeTraversal base class that are available in
    Python have Python names of the form
    TreeTraversal_*Constraint*_*Ban*, where the suffixes *Constraint*
    and *Ban* are abbreviated versions of the corresponding template
    parameters; these suffixes are omitted entirely for the common
    cases LPConstraintNone and BanNone. In all cases, the *IntType*
    parameter is taken to be regina::Integer.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

namespace TautEnumeration_ {

// Docstring regina::python::doc::TautEnumeration_::__init
static const char *__init =
R"doc(Creates a new object for running the tree traversal algorithm.

This prepares the algorithm; in order to run the algorithm and
enumerate taut angle structures, you can either:

* call run(), which enumerates all taut angle structures with a single
  function call;

* repeatedly call next(), which will step to the next taut angle
  struture surface each time you call it.

Precondition:
    The given triangulation is non-empty.

Precondition:
    The trianglation adheres to any preconditions required by the
    template parameters *Constraint* and *Ban*.

Exception ``InvalidArgument``:
    It was not possible to add the extra constraints from the
    LPConstraint template argument, due to an error which should have
    been preventable with the right checks in advance. Such exceptions
    are generated by the *Constraint* class, and so you should consult
    the class documentation for your chosen *Constraint* template
    argument to see if this is a possibility.

Exception ``InvalidArgument``:
    It was not possible to add the extra constraints from the
    LPConstraint template argument, due to an error that was
    "genuinely" unforseeable. Again, such exceptions are generated by
    your chosen *Constraint* class, and you should consult its
    documentation to see if this is a possibility.

Parameter ``tri``:
    the triangulation in which we wish to enumerate taut angle
    structures.

Parameter ``banArgs``:
    any additional arguments to be passed to the constructor for
    *Ban*, after the initial starting tableaux. For most ban
    constraint classes, this list of arguments is empty.)doc";

// Docstring regina::python::doc::TautEnumeration_::next
static const char *next =
R"doc(An incremental step in the enumeration algorithm that runs forward
until it finds the next solution. Specifically: this continues the
enumeration from the current point until either it finds the next taut
angle structure (in which case it returns ``True``), or until the
enumeration algorithm is completely finished with no more solutions to
be found (in which case it returns ``False``).

If you simply wish to find and process all taut angle structures, you
may wish to consider the all-in-one routine run() instead. By using
next() to step through one solution at a time however, you obtain more
fine-grained control: for instance, you can "pause" and restart the
search, or have tighter control over multithreading.

If next() does return ``True`` because it found a solution, you can
extract details of the solution directly from this enumeration object:
for instance, you can dump the type vector using dumpTypes(), or you
can reconstruct the full taut angle structure using buildStructure()
and perform some other operations upon it.

An optional progress tracker may be passed. If so, this routine will
update the percentage progress and poll for cancellation requests. It
will be assumed that an appropriate stage has already been declared
via ProgressTracker::newStage() before this routine is called, and
that ProgressTracker::setFinished() will be called after this routine
returns (and presumably not until the entire search tree is
exhausted). The percentage progress will be given in the context of a
complete enumeration of the entire search tree (i.e., it will
typically start at a percentage greater than 0, and end at a
percentage less than 100).

Precondition:
    The enumeration algorithm has not yet finished. That is, you have
    not called run() before, and if you have called next() then it has
    always returned ``True`` (indicating that it has not yet finished
    the search).

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    ``True`` if we found another vertex surface, or ``False`` if the
    search has now finished and no more taut angle strutures were
    found.)doc";

// Docstring regina::python::doc::TautEnumeration_::run
static const char *run =
R"doc(Runs the complete tree traversal algorithm to enumerate all taut angle
structures.

For each taut angle structure that is found, this routine will call
*action* (which must be a function or some other callable type).

* The first argument passed to *action* will a const reference to this
  TautEnumeration object.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* The tree traversal algorithm will block until *action* returns.

* *action* must return a ``bool``. A return value of ``False``
  indicates that run() should continue the enumeration, and a return
  value of ``True`` indicates that run() should terminate the search
  immediately.

Your action can extract details of the solution directly from the
enumeration object: for instance, it can dump the type vector using
dumpTypes(), or it can reconstruct the full taut angle structure using
buildStructure() and perform some other operations upon it.

The usual way of using this routine is to construct an TautEnumeration
object and then immediately call run(). However, if you prefer, you
may call run() after one or more calls to next(). In this case, run()
will continue the search from the current point and run it to its
completion. In other words, run() will locate and call *action* for
all taut angle structures that had not yet been found, but it will not
call *action* for those solutions that had previously been found
during earlier calls to next().

Precondition:
    The enumeration algorithm has not yet finished. That is, you have
    not called run() before, and if you have called next() then it has
    always returned ``True`` (indicating that it has not yet finished
    the search).

Python:
    This function is available, and *action* may be a pure Python
    function. However, *action* cannot take any additional arguments
    beyond the initial TautEnumeration object (and therefore the
    additional *args* list is omitted here).

Parameter ``action``:
    a function (or some other callable type) to call for each taut
    angle structure that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial tree enumeration argument.

Returns:
    ``True`` if *action* ever terminated the search by returning
    ``True``, or ``False`` if the search was allowed to run to
    completion.)doc";

// Docstring regina::python::doc::TautEnumeration_::solutions
static const char *solutions =
R"doc(Returns the total number of taut angle structures found thus far in
the tree traversal search.

If you called run(), then this will simply be the total number of taut
angle structures that were found. If you are calling next() one
surface at time, this will be the partial count of how many taut angle
structures have been found until now.

Returns:
    the number of solutions found so far.)doc";

// Docstring regina::python::doc::TautEnumeration_::writeStructure
static const char *writeStructure =
R"doc(A callback function that writes to standard output the full angle
structure coordinates of the taut angle structure at the current point
in the given tree traversal search. You can use this as the callback
*action* that is passed to run().

The angle structure coordinates will be written on a single line, with
spaces and punctuation separating them, a prefix indicating which
solution we are up to, and a final newline appended. The final scaling
coordinate (used to projectivise the angle structure polytope) will
also be written. This output format is subject to change in future
versions of Regina.

Precondition:
    The given tree traversal is at a point in the search where it has
    reached the deepest level of the search tree and found a feasible
    solution that represents a taut angle structure. This is always
    the case any time after next() returns ``True``, or any time that
    run() calls its callback function.

Python:
    This function is available and can be used directly, but you
    should not use it as a callback with run(). Currently this causes
    a crash in Python, most likely coming from some confusion in
    passing a C++ function as a C++ callback via a Python wrapper.
    Instead you can use a pure python function *f* as a callback,
    where ``f(tree)`` just calls ``tree.writeStructure()``.

Parameter ``tree``:
    the tree traversal object from which we are extracting the current
    taut angle structure.

Returns:
    ``False`` (which indicates to run() that we should continue the
    tree traversal).)doc";

// Docstring regina::python::doc::TautEnumeration_::writeTypes
static const char *writeTypes =
R"doc(A callback function that writes to standard output the type vector at
the current point in the given tree traversal search. You can use this
as the callback *action* that is passed to run().

The type vector will be written on a single line, with no spaces
between types, with a prefix indicating which solution we are up to,
and with a final newline appended. This output format is subject to
change in future versions of Regina.

Precondition:
    The given tree traversal is at a point in the search where it has
    reached the deepest level of the search tree and found a feasible
    solution that represents a taut angle structure. This is always
    the case any time after next() returns ``True``, or any time that
    run() calls its callback function.

Parameter ``tree``:
    the tree traversal object from which we are extracting the current
    type vector.

Returns:
    ``False`` (which indicates to run() that we should continue the
    tree traversal).)doc";

}

namespace TreeEnumeration_ {

// Docstring regina::python::doc::TreeEnumeration_::__init
static const char *__init =
R"doc(Creates a new object for running the tree traversal algorithm.

This prepares the algorithm; in order to run the algorithm and
enumerate vertex surfaces, you can either:

* call run(), which enumerates all vertex surfaces with a single
  function call;

* repeatedly call next(), which will step to the next vertex surface
  each time you call it.

.. warning::
    Although it is supported, it is highly recommended that you do
    _not_ run a full vertex enumeration in standard normal or almost
    normal coordinates (this is for performance reasons). See the
    class notes for further discussion and better alternatives. In
    normal circumstances you should run a full vertex enumeration in
    quadrilateral or quadrilateral-octagon coordinates only.

Precondition:
    The given triangulation is non-empty.

Precondition:
    Both the trianglation and the given vector encoding adhere to any
    preconditions required by the template parameters *Constraint* and
    *Ban*.

Exception ``InvalidArgument``:
    It was not possible to add the extra constraints from the
    LPConstraint template argument, due to an error which should have
    been preventable with the right checks in advance. Such exceptions
    are generated by the *Constraint* class, and so you should consult
    the class documentation for your chosen *Constraint* template
    argument to see if this is a possibility.

Exception ``InvalidArgument``:
    It was not possible to add the extra constraints from the
    LPConstraint template argument, due to an error that was
    "genuinely" unforseeable. Again, such exceptions are generated by
    your chosen *Constraint* class, and you should consult its
    documentation to see if this is a possibility.

Parameter ``tri``:
    the triangulation in which we wish to enumerate vertex surfaces.

Parameter ``enc``:
    the normal (or almost normal) surface vector encoding that we are
    working with.

Parameter ``banArgs``:
    any additional arguments to be passed to the constructor for
    *Ban*, after the initial starting tableaux. For most ban
    constraint classes, this list of arguments is empty.)doc";

// Docstring regina::python::doc::TreeEnumeration_::next
static const char *next =
R"doc(An incremental step in the tree traversal algorithm that runs forward
until it finds the next solution. Specifically: this continues the
tree traversal from the current point until either it finds the next
vertex normal or almost normal surface (in which case it returns
``True``), or until the tree traversal is completely finished with no
more solutions to be found (in which case it returns ``False``).

If you simply wish to find and process all vertex surfaces, you may
wish to consider the all-in-one routine run() instead. By using next()
to step through one solution at a time however, you obtain more fine-
grained control: for instance, you can "pause" and restart the search,
or have tighter control over multithreading.

If next() does return ``True`` because it found a solution, you can
extract details of the solution directly from this tree enumeration
object: for instance, you can dump the type vector using dumpTypes(),
or you can reconstruct the full normal or almost normal surface using
buildSurface() and perform some other operations upon it.

An optional progress tracker may be passed. If so, this routine will
update the percentage progress and poll for cancellation requests. It
will be assumed that an appropriate stage has already been declared
via ProgressTracker::newStage() before this routine is called, and
that ProgressTracker::setFinished() will be called after this routine
returns (and presumably not until the entire search tree is
exhausted). The percentage progress will be given in the context of a
complete enumeration of the entire search tree (i.e., it will
typically start at a percentage greater than 0, and end at a
percentage less than 100).

Precondition:
    The tree traversal algorithm has not yet finished. That is, you
    have not called run() before, and if you have called next() then
    it has always returned ``True`` (indicating that it has not yet
    finished the search).

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    ``True`` if we found another vertex surface, or ``False`` if the
    search has now finished and no more vertex surfaces were found.)doc";

// Docstring regina::python::doc::TreeEnumeration_::run
static const char *run =
R"doc(Runs the complete tree traversal algorithm to enumerate vertex normal
or almost normal surfaces.

For each vertex surface that is found, this routine will call *action*
(which must be a function or some other callable type).

* The first argument passed to *action* will a const reference to this
  TreeEnumeration object.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* The tree traversal algorithm will block until *action* returns.

* *action* must return a ``bool``. A return value of ``False``
  indicates that run() should continue the tree traversal, and a
  return value of ``True`` indicates that run() should terminate the
  search immediately.

Your action can extract details of the solution directly from the tree
enumeration object: for instance, it can dump the type vector using
dumpTypes(), or it can reconstruct the full normal or almost normal
surface using buildSurface() and perform some other operations upon
it.

The usual way of using this routine is to construct a TreeEnumeration
object and then immediately call run(). However, if you prefer, you
may call run() after one or more calls to next(). In this case, run()
will continue the search from the current point and run it to its
completion. In other words, run() will locate and call *action* for
all vertex surfaces that had not yet been found, but it will not call
*action* on those surfaces that had previously been found during
earlier calls to next().

Precondition:
    The tree traversal algorithm has not yet finished. That is, you
    have not called run() before, and if you have called next() then
    it has always returned ``True`` (indicating that it has not yet
    finished the search).

Python:
    This function is available, and *action* may be a pure Python
    function. However, *action* cannot take any additional arguments
    beyond the initial TreeEnumeration object (and therefore the
    additional *args* list is omitted here).

Parameter ``action``:
    a function (or some other callable type) to call for each vertex
    surface that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial tree enumeration argument.

Returns:
    ``True`` if *action* ever terminated the search by returning
    ``True``, or ``False`` if the search was allowed to run to
    completion.)doc";

// Docstring regina::python::doc::TreeEnumeration_::solutions
static const char *solutions =
R"doc(Returns the total number of vertex normal or almost normal surfaces
found thus far in the tree traversal search.

If you called run(), then this will simply be the total number of
vertex surfaces that were found. If you are calling next() one surface
at time, this will be the partial count of how many vertex surfaces
have been found until now.

Returns:
    the number of solutions found so far.)doc";

// Docstring regina::python::doc::TreeEnumeration_::writeSurface
static const char *writeSurface =
R"doc(A callback function that writes to standard output the full triangle-
quadrilateral coordinates of the vertex normal or almost normal
surface at the current point in the given tree traversal search. You
can use this as the callback *action* that is passed to run().

The normal surface coordinates will be written on a single line, with
spaces and punctuation separating them, a prefix indicating which
solution we are up to, and a final newline appended. This output
format is subject to change in future versions of Regina.

Precondition:
    The given tree traversal is at a point in the search where it has
    reached the deepest level of the search tree and found a feasible
    solution that represents a vertex normal or almost normal surface.
    This is always the case any time after next() returns ``True``, or
    any time that run() calls its callback function.

Python:
    This function is available and can be used directly, but you
    should not use it as a callback with run(). Currently this causes
    a crash in Python, most likely coming from some confusion in
    passing a C++ function as a C++ callback via a Python wrapper.
    Instead you can use a pure python function *f* as a callback,
    where ``f(tree)`` just calls ``tree.writeSurface()``.

Parameter ``tree``:
    the tree traversal object from which we are extracting the current
    vertex normal or almost normal surface.

Returns:
    ``False`` (which indicates to run() that we should continue the
    tree traversal).)doc";

// Docstring regina::python::doc::TreeEnumeration_::writeTypes
static const char *writeTypes =
R"doc(A callback function that writes to standard output the type vector at
the current point in the given tree traversal search. You can use this
as the callback *action* that is passed to run().

The type vector will be written on a single line, with no spaces
between types, with a prefix indicating which solution we are up to,
and with a final newline appended. This output format is subject to
change in future versions of Regina.

Precondition:
    The given tree traversal is at a point in the search where it has
    reached the deepest level of the search tree and found a feasible
    solution that represents a vertex normal or almost normal surface.
    This is always the case any time after next() returns ``True``, or
    any time that run() calls its callback function.

Parameter ``tree``:
    the tree traversal object from which we are extracting the current
    type vector.

Returns:
    ``False`` (which indicates to run() that we should continue the
    tree traversal).)doc";

}

namespace TreeSingleSoln_ {

// Docstring regina::python::doc::TreeSingleSoln_::__init
static const char *__init =
R"doc(Creates a new object for running the tree traversal / branching
algorithm to locate a non-trivial surface that satisfies the chosen
constraints.

This constructor prepares the algorithm; in order to run the algorithm
you should call find(), which returns ``True`` or ``False`` according
to whether or not such a surface was found.

Precondition:
    The given triangulation is non-empty.

Precondition:
    Both the trianglation and the given vector encoding adhere to any
    preconditions required by the template parameters *Constraint* and
    *Ban*.

Exception ``InvalidArgument``:
    It was not possible to add the extra constraints from the
    LPConstraint template argument, due to an error which should have
    been preventable with the right checks in advance. Such exceptions
    are generated by the *Constraint* class, and so you should consult
    the class documentation for your chosen *Constraint* template
    argument to see if this is a possibility.

Exception ``InvalidArgument``:
    It was not possible to add the extra constraints from the
    LPConstraint template argument, due to an error that was
    "genuinely" unforseeable. Again, such exceptions are generated by
    your chosen *Constraint* class, and you should consult its
    documentation to see if this is a possibility.

Parameter ``tri``:
    the triangulation in which we wish to search for a non-trivial
    surface.

Parameter ``enc``:
    the normal (or almost normal) surface vector encoding that we are
    working with.

Parameter ``banArgs``:
    any additional arguments to be passed to the constructor for
    *Ban*, after the initial starting tableaux. For most ban
    constraint classes, this list of arguments is empty.)doc";

// Docstring regina::python::doc::TreeSingleSoln_::cancel
static const char *cancel =
R"doc(Cancels the current find() operation.

This may be called from another thread (it is thread-safe). If called,
it signals that if find() is currently running then it should be
cancelled at the earliest convenient opportunity.)doc";

// Docstring regina::python::doc::TreeSingleSoln_::find
static const char *find =
R"doc(Runs the tree traversal algorithm until it finds some non-trivial
surface that satisfies the chosen constraints, or else proves that no
such solution exists.

Note that, if a solution is found, it will have a maximal (but not
necessarily maximum) set of zero coordinates, which in some settings
is enough to guarantee a vertex normal surface. See the TreeSingleSoln
class notes for details.

If find() does return ``True``, you can extract details of the
corresponding surface directly from this tree enumeration object: for
instance, you can dump the type vector using dumpTypes(), or you can
reconstruct the full surface using buildSurface(). Be warned that this
class defines the type vector in an unusual way (see the
TreeSingleSoln class notes for details).

Precondition:
    The algorithm has not yet been run, i.e., you have not called
    find() before.

Returns:
    ``True`` if we found a non-trivial solution as described in the
    class notes, or ``False`` if no such solution exists.)doc";

}

namespace TreeTraversal_ {

// Docstring regina::python::doc::TreeTraversal_::buildStructure
static const char *buildStructure =
R"doc(Reconstructs the full taut angle structure that is represented by the
type vector at the current stage of the search. This routine is for
use only with taut angle structures, not normal or almost normal
surfaces.

There will always be a unique taut angle structure corresponding to
this type vector (this follows from the preconditions below).

Precondition:
    This tree traversal is at a point in the search where it has found
    a feasible solution that represents a taut angle structure. This
    condition is always true after TautEnumeration::next() returns
    ``True``, or any time that TautEnumeration::run() calls its
    callback function.

Precondition:
    We are working with angle structure coordinates. This will be
    checked (see the exception details below).

Exception ``FailedPrecondition``:
    We are not working with angle structure coordinates (i.e., the
    coordinate system passed to the TreeTraversal constructor was not
    NormalCoords::Angle).

Returns:
    the taut angle structure that has been found at the current stage
    of the search.)doc";

// Docstring regina::python::doc::TreeTraversal_::buildSurface
static const char *buildSurface =
R"doc(Reconstructs the full normal surface that is represented by the type
vector at the current stage of the search. This routine is for use
only with normal (or almost normal) surfaces, not taut angle
structures.

If the current type vector does not represent a _vertex_ normal
surface (which may be the case when calling TreeSingleSoln::find()),
then there may be many normal surfaces all represented by the same
type vector; in this case there are no further guarantees about
_which_ of these normal surfaces you will get.

The surface that is returned will use the same vector encoding that
was passed to the TreeTraversal class constructor.

Precondition:
    This tree traversal is at a point in the search where it has found
    a feasible solution that represents a normal surface (though this
    need not be a vertex surface). This condition is always true after
    TreeEnumeration::next() or TreeSingleSoln::find() returns
    ``True``, or any time that TreeEnumeration::run() calls its
    callback function.

Precondition:
    We are working with normal or almost normal surfaces. This will be
    checked (see the exception details below).

Exception ``FailedPrecondition``:
    We are not working with normal or almost normal surfaces (i.e.,
    the coordinate system passed to the TreeTraversal constructor was
    NormalCoords::Angle).

Returns:
    a normal surface that has been found at the current stage of the
    search.)doc";

// Docstring regina::python::doc::TreeTraversal_::supported
static const char *supported =
R"doc(Indicates whether the given normal surface or angle structure vector
encoding is supported by this tree traversal infrastructure. Any
restrictions imposed by the LPConstraint and BanConstraint template
parameters will be taken into account.

Note that, even if an encoding is supported, this does not mean that
the underlying tableaux will use the same encoding internally. See
LPInitialTableaux for more details on this.

Parameter ``enc``:
    the vector encoding being queried. In particular, this may be the
    special angle structure encoding.

Returns:
    ``True`` if and only if the given vector encoding is supported.)doc";

// Docstring regina::python::doc::TreeTraversal_::typeString
static const char *typeString =
R"doc(Returns the current type vector in string form. There will be no
spaces between the types.

This routine returns the same information that dumpTypes() writes.

Returns:
    the type vector in string form.)doc";

// Docstring regina::python::doc::TreeTraversal_::visited
static const char *visited =
R"doc(Returns the total number of nodes in the search tree that we have
visited thus far in the tree traversal. This figure might grow much
faster than the number of solutions, since it also counts traversals
through "dead ends" in the search tree.

This counts all nodes that we visit, including those that fail any or
all of the domination, feasibility and zero tests. The precise way
that this number is calculated is subject to change in future versions
of Regina.

If you called an "all at once" routine such as TreeEnumeration::run()
or TreeSingleSoln::find(), then this will be the total number of nodes
that were visited in the entire tree traversal. If you are calling an
"incremental" routine such as TreeEnumeration::next() (i.e., you are
generating one solution at time), then this will be the partial count
of how many nodes have been visited so far.

Returns:
    the number of nodes visited so far.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

