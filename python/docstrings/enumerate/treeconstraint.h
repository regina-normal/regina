/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::BanBoundary
static const char *BanBoundary =
R"doc(A class that bans normal disc types that meet the boundary of the
underlying triangulation. No disc types are marked at all.

This class is only for use with normal or almost normal surfaces, not
angle structures.

.. warning::
    This class only works as expected with vector encodings that
    explicitly include triangles (e.g., encodings for standard normal
    or almost normal coordinates). In quadrilateral or quadrilateral-
    octagon coordinates it will only ban quadrilaterals or octagons
    that touch the boundary, but it will still allow _triangles_ that
    meet the boundary (since triangle types are not counted in these
    coordinate systems). The supported() routine will only return
    ``True`` for encodings that include triangles.

See the BanConstraintBase class notes for details on all member
functions and structs.

These ban constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a ban constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Python:
    It is rare that you would need to access this class directly
    through Python. Instead, to use a ban constraint class, you would
    typically create a tree traversal object with the appropriate
    class suffix (e.g., one such Python class is
    ``TreeEnumeration_BanBoundary``). See the BanConstraintBase class
    notes for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::BanConstraintBase
static const char *BanConstraintBase =
R"doc(A base class for additional banning and marking constraints that we
can place on tree traversal algorithms. This is used with
TreeEnumeration, TreeSingleSoln and related algorithms for enumerating
and locating normal surfaces and angle structures in a 3-manifold
triangulation.

This class adds constraints of two types:

* _Banning_ constraints, which ensure that certain coordinates are set
  to zero;

* _Marking_ constraints, which are more flexible and can be used in
  different ways by different algorithms.

All of these constraints operate only on normal or angle structure
coordinates in the underlying tableaux (and in particular not the
additional variables introduced by additional linear constraints, as
described by LPConstraintBase and its subclasses).

Currently marking is used in the following ways:

* The TreeEnumeration and TautEnumeration algorithms do not use
  marking at all.

* In the TreeSingleSoln algorithm, marking affects what is considered
  a non-trivial normal surface. Normally, a non-trivial surface is
  defined to be one in which some triangle coordinate is zero. With
  marking, a non-trivial surface is redefined to be one in which some
  _unmarked_ triangle coordinate is zero. In other words, marked
  triangle types are effectively ignored when determining whether a
  surface is non-trivial or not.

At present, marking is not used at all for quadrilateral coordinates
or angle structures. However, marking is a very new feature, and this
concept may be expanded in future versions of Regina.

This class does not record disc types in the order of their normal or
angle structure coordinates; instead it records them in the order of
their columns in a tableaux for linear programming (as used in
LPInitialTableaux). This means that there is a little more work
required in setting up the initial lists of banned and marked columns,
but then these lists are easy to use on the fly during tree traversal
algorithms.

Bear in mind that the tableaux that these constraints are working with
will not necessarily use the same coordinates as the underlying
enumeration task (e.g., the tableaux will never include separate
columns for octagon coordinates). See LPInitialTableaux for a more
detailed discussion of this.

This base class provides limited functionality (as documented below).
Subclasses _must_ implement a constructor (which, like this base
class, takes an initial tableaux and determines which coordinates are
banned and/or marked), and must implement supported(), which indicates
which normal or angle structure coordinate system this constraint
class can work with. The constructor may take additional arguments
beyond the initial tableaux; if so, then the tree traversal classes
(mentioned below) will forward these arguments at runtime from their
own class constructors.

These ban constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a ban constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Python:
    This base class is not present, but all subclasses are available.
    However, as noted above, it is rare that you would need to access
    any of these ban constraint classes directly through Python.
    Instead, to use a ban constraint class, you would typically create
    a tree traversal object with the appropriate class suffix (e.g.,
    one such Python class is ``TreeEnumeration_BanBoundary``).

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::BanEdge
static const char *BanEdge =
R"doc(A class that bans normal disc types that meet a particular edge of the
underlying triangulation. No disc types are marked at all.

This class is only for use with normal or almost normal surfaces, not
angle structures.

.. warning::
    This class only works as expected with vector encodings that
    explicitly include triangles (e.g., encodings for standard normal
    or almost normal coordinates). In quadrilateral or quadrilateral-
    octagon coordinates it will only ban quadrilaterals or octagons
    that meet the given edge, but it will still allow _triangles_ that
    meet the edge (since triangle types are not counted in these
    coordinate systems). The supported() routine will only return
    ``True`` for encodings that include triangles.

See the BanConstraintBase class notes for details on all member
functions and structs.

These ban constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a ban constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Python:
    It is rare that you would need to access this class directly
    through Python. Instead, to use a ban constraint class, you would
    typically create a tree traversal object with the appropriate
    class suffix (e.g., one such Python class is
    ``TreeEnumeration_BanEdge``). See the BanConstraintBase class
    notes for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::BanNone
static const char *BanNone =
R"doc(A do-nothing class that bans no coordinates and marks no coordinates.

This is intended to act as a drop-in replacement for a "real"
BanConstraint class (i.e., a subclass of BanConstraintBase). However,
to avoid any overhead in this trivial case, BanNone does _not_ derive
from BanConstraintBase, and all of its routines do nothing at all.

See the BanConstraintBase class notes for details on the interface
that this class adheres to.

These ban constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a ban constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Python:
    It is rare that you would need to access this class directly
    through Python. Instead, to use this ban constraint class, you
    would typically create a tree traversal object with no ban
    constraint class suffix at all (since BanNone is the default
    behaviour). For example, all of the Python classes
    ``TreeEnumeration_NonSpun``, ``TreeSingleSoln_EulerPositive`` and
    ``TautEnumeration`` use this do-nothing BanNone class. See the
    BanConstraintBase class notes for further details on accessing
    other types of ban constraints from within Python.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::BanTorusBoundary
static const char *BanTorusBoundary =
R"doc(A class that bans and marks disc types associated with torus boundary
components. Here we refer exclusively to real torus boundary
components (not ideal vertices with torus cusps). Specifically:

* this class bans any normal triangle or quadrilateral that meets a
  torus boundary;

* this class marks any normal triangle in the link of a vertex on a
  torus boundary.

This class is only for use with normal or almost normal surfaces, not
angle structures.

.. warning::
    As with BanBoundary, this class only works as expected with vector
    encodings that explicitly include triangles (e.g., encodings for
    standard normal or almost normal coordinates). In quadrilateral or
    quadrilateral-octagon coordinates it will only ban quadrilaterals
    or octagons that touch torus boundaries, but it will still allow
    _triangles_ that meet torus boundaries (since triangle types are
    not counted in these coordinate systems). The supported() routine
    will only return ``True`` for encodings that include triangles.

See the BanConstraintBase class notes for details on all member
functions and structs.

These ban constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a ban constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Python:
    It is rare that you would need to access this class directly
    through Python. Instead, to use a ban constraint class, you would
    typically create a tree traversal object with the appropriate
    class suffix (e.g., one such Python class is
    ``TreeEnumeration_BanTorusBoundary``). See the BanConstraintBase
    class notes for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPConstraintBase
static const char *LPConstraintBase =
R"doc(A base class for additional linear constraints that we can add to the
tableaux of normal surface or angle structure matching equations. This
is used with TreeEnumeration, TreeSingleSoln and related algorithms
for enumerating and locating normal surfaces or angle structures in a
3-manifold triangulation. See the LPInitialTableaux class notes for
details on how these constraints interact with the tableaux of
matching equations.

The linear constraints may be equalities or inequalities, and there
may be more than one such constraint. If all constraints are
homogeneous equalities, the class should derive from
LPConstraintSubspace instead (not this base class).

In angle structure coordinates, these linear constraints must _not_
involve the scaling coordinate (the final coordinate that is used to
convert the angle structure polytope into a polyhedral cone). The
coefficient for the final scaling coordinate in each additional linear
constraint will be assumed to be zero.

Bear in mind that the tableaux that these constraints are working with
will not necessarily use the same coordinates as the underlying
enumeration task (e.g., the tableaux will never include separate
columns for octagon coordinates). See LPInitialTableaux for a more
detailed discussion of this.

This base class provides no functionality. For documentation's sake
only, the notes here describe the functionality that any subclass
_must_ implement. We note again that LPConstraintBase does not provide
any implementations at all, and subclasses are completely responsible
for their own implementations.

All constraint classes provide their functionality through static
routines: they do not contain any member data, and it is unnecessary
(but harmless) to construct them.

These linear constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a linear constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Python:
    This base class is not present, but all of the "real" linear
    constraint subclasses are available. However, as noted above, it
    is rare that you would need to access any of these constraint
    classes directly through Python. Instead, to use a linear
    constraint class, you would typically create a tree traversal
    object with the appropriate class suffix (e.g., one such Python
    class is ``TreeEnumeration_NonSpun``).

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPConstraintEulerPositive
static const char *LPConstraintEulerPositive =
R"doc(A class that constraints the tableaux of normal surface matching
equations to ensure that Euler characteristic is strictly positive.

There are many ways of writing Euler characteritic as a linear
function. The function constructed here has integer coefficients, but
otherwise has no special properties of note.

This constraint can work with either normal or almost normal
coordinates. In the case of almost normal coordinates, the function is
modified to measure Euler characteristic minus the number of octagons
(a technique of Casson, also employed by Jaco and Rubinstein, that is
used to ensure we do not have more than two octagons when searching
for a normal or almost normal sphere in the 3-sphere recognition
algorithm).

See the LPConstraintBase class notes for details on all member
functions.

These linear constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a linear constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Precondition:
    We are working with a normal or almost normal vector encoding that
    includes triangle coordinates (i.e., the encoding for standard
    normal or standard almost normal coordinates).

Python:
    It is rare that you would need to access this class directly
    through Python. Instead, to use a linear constraint class, you
    would typically create a tree traversal object with the
    appropriate class suffix (e.g., one such Python class is
    ``TreeSingleSolution_EulerPositive``). See the LPConstraintBase
    class notes for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPConstraintEulerZero
static const char *LPConstraintEulerZero =
R"doc(A class that constraints the tableaux of normal surface matching
equations to ensure that Euler characteristic is zero.

There are many ways of writing Euler characteritic as a linear
function. The function constructed here has integer coefficients, but
otherwise has no special properties of note.

This constraint currently only works with normal (and _not_ almost
normal) coordinates.

See the LPConstraintBase class notes for details on all member
functions.

These linear constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a linear constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Precondition:
    We are working with a normal vector encoding that includes
    triangle coordinates, and that does _not_ include octagon
    coordinates (i.e, the encoding for standard normal coordinates).

Python:
    It is rare that you would need to access this class directly
    through Python. Instead, to use a linear constraint class, you
    would typically create a tree traversal object with the
    appropriate class suffix (e.g., one such Python class is
    ``TreeEnumeration_EulerZero``). See the LPConstraintBase class
    notes for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPConstraintNonSpun
static const char *LPConstraintNonSpun =
R"doc(A class that constraints the tableaux of normal surface matching
equations to ensure that normal surfaces in an ideal triangulation are
compact (thereby avoiding spun normal surfaces with infinitely many
triangles).

At present this class can only work with oriented triangulations that
have precisely one vertex, which is ideal with torus link. Moreover,
it uses the SnapPea kernel for some of its computations, and so
SnapPea must be able to work directly with the given triangulation.
See below for details on the exceptions that addRows() can throw if
these requirements are not met.

Also, at present this class can only work with quadrilateral normal
coordinates (and cannot handle almost normal coordinates at all). This
is _not_ explicitly checked; instead it appears as a precondition (see
below).

See the LPConstraintBase class notes for details on all member
functions.

These linear constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a linear constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Precondition:
    We are working with a normal or almost normal vector encoding that
    does not include triangle coordinates (i.e., the encoding for quad
    or quad-oct normal coordinates).

Exception ``InvalidArgument``:
    Thrown by addRows() if the underlying triangulation is not
    oriented with precisely one vertex, which must have a torus link.

Exception ``UnsolvedCase``:
    Thrown by addRows() if SnapPea retriangulates the underlying
    triangulation or produces a null triangulation, or if the
    coefficients of the slope equations are too large to store in a
    native C++ long integer.

Python:
    It is rare that you would need to access this class directly
    through Python. Instead, to use a linear constraint class, you
    would typically create a tree traversal object with the
    appropriate class suffix (e.g., one such Python class is
    ``TreeEnumeration_NonSpun``). See the LPConstraintBase class notes
    for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPConstraintNone
static const char *LPConstraintNone =
R"doc(A do-nothing class that imposes no additional linear constraints on
the tableaux of normal surface or angle structure matching equations.

See the LPConstraintBase class notes for details on all member
functions.

These linear constraint classes are designed mainly to act as C++
template arguments, and end users will typically not need to construct
their own object of these classes. Instead, to use a linear constraint
class, pass it as a template parameter to one of the tree traversal
subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
TautEnumeration).

Python:
    It is rare that you would need to access this class directly
    through Python. Instead, to use this do-nothing constraint class,
    you would typically create a tree traversal object with no linear
    constraint class suffix at all (since LPConstraintNone is the
    default behaviour). For example, the Python classes
    ``TreeEnumeration``, ``TreeSingleSoln_BanBoundary`` and
    ``TautEnumeration`` all use this do-nothing LPConstraintNone
    class. See the LPConstraintBase class notes for further details on
    accessing other types of linear constraints from within Python.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

namespace BanBoundary_ {

// Docstring regina::python::doc::BanBoundary_::__init
static const char *__init =
R"doc(Constructs a new set of banning and marking constraints.

This base class constructor will construct the *banned_* and *marked_*
arrays to be the correct size based on the given tableaux, and will
initialise their contents to ban disc types that meet the
triangulation boundary.

No disc types will be marked.

Parameter ``init``:
    the original starting tableaux being used for this enumeration
    task. This tableaux must work with normal or almost normal surface
    coordinates (not angle structure coordinates).)doc";

}

namespace BanConstraintBase_ {

// Docstring regina::python::doc::BanConstraintBase_::__eq
static const char *__eq =
R"doc(Determines if this and the given object ban and mark the same tableaux
coordinates as each other.

Even if this and the given object are of different subclasses of
BanConstraintBase, as long as they ban the same coordinates and mark
the same coordinates, they will compare as equal.

It does not matter whether the two objects use the same underlying
tableaux. However, if the underlying tableaux use triangulations of
different sizes and/or different broad classes of vector encodings (as
described by LPSystem), then these two objects will compare as not
equal.

Parameter ``other``:
    the object to compare with this.

Returns:
    ``True`` if and only if this and the object ban and mark the same
    tableaux coordinates, as described above.)doc";

// Docstring regina::python::doc::BanConstraintBase_::__init
static const char *__init =
R"doc(Constructs a new set of banning and marking constraints.

This base class constructor will create *banned_* and *marked_* arrays
of the correct size, and will initialise their contents to be entirely
``False``. This means that there will be no banned or marked disc
types at all.

Subclass constructors should identify which coordinates to ban and
mark, and adjust the contents of the *banned_* and *marked_* arrays
accordingly.

Parameter ``init``:
    the original starting tableaux being used for this enumeration
    task.)doc";

// Docstring regina::python::doc::BanConstraintBase_::__ne
static const char *__ne =
R"doc(Determines if this and the given object do not ban and mark the same
tableaux coordinates as each other.

Even if this and the given object are of different subclasses of
BanConstraintBase, as long as they ban the same coordinates and mark
the same coordinates, they will compare as equal (i.e., this
inequality comparison will return ``False``).

It does not matter whether the two objects use the same underlying
tableaux. However, if the underlying tableaux use triangulations of
different sizes and/or different broad classes of vector encodings (as
described by LPSystem), then these two objects will compare as not
equal.

Parameter ``other``:
    the object to compare with this.

Returns:
    ``True`` if and only if this and the object do not ban and mark
    the same tableaux coordinates, as described above.)doc";

// Docstring regina::python::doc::BanConstraintBase_::enforceBans
static const char *enforceBans =
R"doc(Enforces all bans described by this class in the given tableaux.
Specifically, for each banned coordinate, this routine calls
LPData::constrainZero() on the corresponding coordinate column.

Parameter ``lp``:
    the tableaux in which to enforce the bans.)doc";

// Docstring regina::python::doc::BanConstraintBase_::marked
static const char *marked =
R"doc(Identifies whether the given column of the tableaux corresponds to a
marked coordinate (e.g., a marked normal disc type).

Parameter ``column``:
    a column of the tableaux. This must be one of the columns
    corresponding to a normal or angle structure coordinate, not one
    of the extra columns induced by an LPConstraint parameter for the
    tree traversal class.

Returns:
    ``True`` if and only if the given column corresponds to a marked
    coordinate.)doc";

// Docstring regina::python::doc::BanConstraintBase_::supported
static const char *supported =
R"doc(Indicates whether the given coordinate system is supported by this
constraint class.

This routine assumes that the given system is already known to be
supported by the generic tree traversal infrastructure, and only
returns ``False`` if there are additional prerequisites imposed by
this particular constraint class that the given system does not
satisfy. If this constraint class does not impose any of its own
additional conditions, this routine may simply return ``True``.

The only features of the encoding that this routine should examine are
what coordinates are stored (e.g., NormalEncoding::storesTriangles()).
In particular, this routine will not look at any "semantic guarantees"
(e.g. NormalEncoding::couldBeNonCompact()).

Parameter ``enc``:
    the vector encoding being queried. This must be one of the vector
    encodings known to be supported by the generic TreeTraversal
    infrastructure, and in particular it may be the special angle
    structure encoding.

Returns:
    ``True`` if and only if this vector encoding is also supported by
    this specific constraint class.)doc";

}

namespace BanEdge_ {

// Docstring regina::python::doc::BanEdge_::__init
static const char *__init =
R"doc(Constructs a new set of banning and marking constraints.

This base class constructor will construct the *banned_* and *marked_*
arrays to be the correct size based on the given tableaux, and will
initialise their contents to ban disc types that meet the given edge.

No disc types will be marked.

Parameter ``init``:
    the original starting tableaux being used for this enumeration
    task. This tableaux must work with normal or almost normal surface
    coordinates (not angle structure coordinates).

Parameter ``edge``:
    the specific edge that our normal discs must not meet.)doc";

}

namespace BanTorusBoundary_ {

// Docstring regina::python::doc::BanTorusBoundary_::__init
static const char *__init =
R"doc(Constructs a new set of banning and marking constraints.

This base class constructor will construct the *banned_* and *marked_*
arrays to be the correct size based on the given tableaux, and will
initialise their contents to ban and mark disc types associated with
torus boundary components, as described in the class notes.

Parameter ``init``:
    the original starting tableaux being used for this enumeration
    task. This tableaux must work with normal or almost normal surface
    coordinates (not angle structure coordinates).)doc";

}

namespace LPConstraintBase_ {

// Docstring regina::python::doc::LPConstraintBase_::addRows
static const char *addRows =
R"doc(Explicitly constructs equations for the linear function(s) constrained
by this class. Specifically, this routine takes an array of columns in
the initial tableaux and fills in the necessary coefficient data.

More precisely: recall that, for each linear function, the initial
tableaux acquires one new variable *x_i* that evaluates this linear
function f(x). This routine must create the corresponding row that
sets ``f(x) - x_i = 0``. Thus it must construct the coefficients of
f(x) in the columns corresponding to normal coordinates, and it must
also set a coefficient of -1 in the column for the corresponding new
variable.

As described in the LPInitialTableaux class notes, it might not be
possible to construct the linear functions (since the underlying
triangulation might not satisfy the necessary requirements). In such
cases this routine should throw an exception, as described below, and
the corresponding constraint class _must_ mention this possibility in
its class documentation.

If you are implementing this routine in a subclass that works with
angle structure coordinates, remember that your linear constraints
must not interact with the scaling coordinate (the final angle
structure coordinate that is used to projectivise the angle structure
polytope into a polyhedral cone). Your implementation of this routine
_must_ ensure that your linear constraints all have coefficient zero
in this column.

The precise form of the linear function(s) will typically depend upon
the underlying triangulation, as well as the permutation that
indicates which columns of the initial tableaux correspond to which
normal or angle structure coordinates. All of this information is read
from the given initial tableaux *init*.

Note that the tableaux *init* may still be under construction (and
indeed, the column array *col* to be filled will typically be the
internal column array from *init* itself). This routine should not
read any of the tableaux entries; it should only access the underlying
triangulation (LPInitialTableaux.tri()) and the permutation of columns
(LPInitialTableaux.columnPerm()).

For each subclass *Sub* of LPConstraintBase, the array *col* must be
an array of objects of type LPCol<Sub>, and the tableaux *init* must
be of type LPInitialTableaux<Sub>.

This routine should only write to the coefficients stored in
LPCol::extra. You may assume that these coefficients have all been
initialised to zero by the LPCol constructor.

Precondition:
    For all columns in the array *col*, the members LPCol::extra have
    all been initialised to zero.

Exception ``InvalidArgument``:
    It was not possible to create the linear functions for these
    constraints, due to an error which should have been preventable
    with the right checks in advance. Any constraint class that could
    throw exceptions in this way _must_ describe this behaviour in its
    own class documentation.

Exception ``UnsolvedCase``:
    It was not possible to create the linear functions for these
    constraints, due to an error that was "genuinely" unforseeable.
    Again, any constraint class that could throw exceptions in this
    way _must_ describe this behaviour in its own class documentation.

Python:
    The argument *col* is not present, since LPCol is only designed to
    be used as part of the internal data storage for
    LPInitialTableaux. Instead, this routine returns a Python list of
    constraints, where each constraint is presented as a Python list
    of coefficients. Each of these inner lists will have size
    init.columns().

Parameter ``col``:
    the array of columns as stored in the initial tableaux (i.e., the
    data member LPInitialTableaux::col_).

Parameter ``init``:
    the tableaux through which this routine can acces the underlying
    triangulation and permutation of columns. Typically this will be
    the tableaux holding the column array *col*.)doc";

// Docstring regina::python::doc::LPConstraintBase_::constrain
static const char *constrain =
R"doc(Explicitly constraints each of these linear functions to an equality
or inequality in the underlying tableaux. This will typically consist
of a series of calls to LPData::constrainZero() and/or
LPData::constrainPositive().

The variables for these extra linear functions are stored in columns
``numCols - nConstraints``, ..., ``numCols - 1`` of the given
tableaux, and so your calls to LPData::constrainZero() and/or
LPData::constrainPositive() should operate on these (and only these)
columns.

Precondition:
    These column coefficients belong to the initial starting tableaux
    (LPInitialTableaux) from which the given tableaux is derived.

Parameter ``lp``:
    the tableaux in which to constrain these linear functions.

Parameter ``numCols``:
    the number of columns in the given tableaux.)doc";

// Docstring regina::python::doc::LPConstraintBase_::supported
static const char *supported =
R"doc(Indicates whether the given vector encoding is supported by this
constraint class.

This routine assumes that the given encoding is already known to be
supported by the generic tree traversal infrastructure, and only
returns ``False`` if there are additional prerequisites imposed by
this particular constraint class that the given encoding does not
satisfy. If this constraint class does not impose any of its own
additional conditions, this routine may simply return ``True``.

The only features of the encoding that this routine should examine are
what coordinates are stored (e.g., NormalEncoding::storesTriangles()).
In particular, this routine will not look at any "semantic guarantees"
(e.g. NormalEncoding::couldBeNonCompact()).

Parameter ``enc``:
    the vector encoding being queried. This must be one of the vector
    encodings known to be supported by the generic TreeTraversal
    infrastructure, and in particular it may be the special angle
    structure encoding.

Returns:
    ``True`` if and only if this vector encoding is also supported by
    this specific constraint class.)doc";

// Docstring regina::python::doc::LPConstraintBase_::verify
static const char *verify =
R"doc(Ensures that the given normal surface satisfies the extra constraints
described by this class.

Ideally this test is not based on explicitly recomputing the linear
function(s), but instead runs independent tests. For instance, if this
class is used to constraint Euler characteristic, then ideally this
routine would call s.eulerChar() and test the return value of that
routine instead.

If these linear constraints work with angle structure coordinates (not
normal or almost normal surfaces), then this routine should return
``False``.

Parameter ``s``:
    the surface to test.

Returns:
    ``True`` if the given surface satisfies these linear constraints,
    or ``False`` if it does not.)doc";

// Docstring regina::python::doc::LPConstraintBase_::verify_2
static const char *verify_2 =
R"doc(Ensures that the given angle structure satisfies the extra constraints
described by this class.

Ideally this test is not based on explicitly recomputing the linear
function(s), but instead runs independent tests; see the related
routine verify(const NormalSurface&) for examples.

If these linear constraints work with normal or almost normal surfaces
(not angle structure coordinates), then this routine should return
``False``.

Parameter ``s``:
    the angle structure to test.

Returns:
    ``True`` if the given angle structure satisfies these linear
    constraints, or ``False`` if it does not.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

