/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct BanBoundary {

// Docstring regina::python::doc::BanBoundary::__class
static constexpr const char __class[] =
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

// Docstring regina::python::doc::BanBoundary::__init
static constexpr const char __init[] =
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

}; // struct BanBoundary

struct BanConstraintBase {

// Docstring regina::python::doc::BanConstraintBase::__class
static constexpr const char __class[] =
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
described in the LPConstraint concept documentation).

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

// Docstring regina::python::doc::BanConstraintBase::__eq
static constexpr const char __eq[] =
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

// Docstring regina::python::doc::BanConstraintBase::__init
static constexpr const char __init[] =
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

// Docstring regina::python::doc::BanConstraintBase::enforceBans
static constexpr const char enforceBans[] =
R"doc(Enforces all bans described by this class in the given tableaux.
Specifically, for each banned coordinate, this routine calls
LPData::constrainZero() on the corresponding coordinate column.

Parameter ``lp``:
    the tableaux in which to enforce the bans.)doc";

// Docstring regina::python::doc::BanConstraintBase::marked
static constexpr const char marked[] =
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

// Docstring regina::python::doc::BanConstraintBase::supported
static constexpr const char supported[] =
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

}; // struct BanConstraintBase

struct BanEdge {

// Docstring regina::python::doc::BanEdge::__class
static constexpr const char __class[] =
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

// Docstring regina::python::doc::BanEdge::__init
static constexpr const char __init[] =
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

}; // struct BanEdge

struct BanNone {

// Docstring regina::python::doc::BanNone::__class
static constexpr const char __class[] =
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

}; // struct BanNone

struct BanTorusBoundary {

// Docstring regina::python::doc::BanTorusBoundary::__class
static constexpr const char __class[] =
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

// Docstring regina::python::doc::BanTorusBoundary::__init
static constexpr const char __init[] =
R"doc(Constructs a new set of banning and marking constraints.

This base class constructor will construct the *banned_* and *marked_*
arrays to be the correct size based on the given tableaux, and will
initialise their contents to ban and mark disc types associated with
torus boundary components, as described in the class notes.

Parameter ``init``:
    the original starting tableaux being used for this enumeration
    task. This tableaux must work with normal or almost normal surface
    coordinates (not angle structure coordinates).)doc";

}; // struct BanTorusBoundary

struct LPConstraintEulerPositive {

// Docstring regina::python::doc::LPConstraintEulerPositive::__class
static constexpr const char __class[] =
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

This constraint type cannot be used with angle structure coordinate
systems.

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
    ``TreeSingleSolution_EulerPositive``). See the LPConstraint
    concept documentation for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPConstraintEulerPositive::addRows
static constexpr const char addRows[] =
R"doc(Explicitly builds equations for the linear function(s) constrained by
this class. See the LPConstraint concept documentation for more
precise details on what this function does.

Precondition:
    For all columns in the array *col*, the members LPCol::extra have
    all been initialised to zero.

Python:
    The array *columnPerm* should be presented as a Python list of
    integers. Moreover, the argument *col* is not present at all,
    since LPCol is an internal class, not for general use. Instead,
    this routine returns a Python list of constraints, where each
    constraint is presented as a Python list of coefficients. Each of
    these inner lists will have size equal to the number of columns.

Parameter ``col``:
    the array of columns as stored in the initial tableaux, presented
    as a C-style array.

Parameter ``tri``:
    the underlying triangulation.

Parameter ``columnPerm``:
    the permutation that indicates which columns of the initial
    tableaux correspond to which normal coordinates, presented as a
    C-style array. This is the same permutation returned by
    ``LPInitialTableaux<...>::columnPerm()``.)doc";

// Docstring regina::python::doc::LPConstraintEulerPositive::supported
static constexpr const char supported[] =
R"doc(Indicates whether the given vector encoding is supported by this
constraint class.

Parameter ``enc``:
    the vector encoding being queried, which must already be known to
    be supported by the generic TreeTraversal infrastructure.

Returns:
    ``True`` if and only if the given encoding is supported by this
    specific constraint class.)doc";

// Docstring regina::python::doc::LPConstraintEulerPositive::verify_surface
static constexpr const char verify_surface[] =
R"doc(Ensures that the given normal surface satisfies the extra constraints
described by this class. For LPConstraintEulerPositive, this simply
tests whether *s* has positive Euler characteristic.

Parameter ``s``:
    the surface to test.

Returns:
    ``True`` if the given surface satisfies these linear constraints,
    or ``False`` if it does not.)doc";

}; // struct LPConstraintEulerPositive

struct LPConstraintEulerZero {

// Docstring regina::python::doc::LPConstraintEulerZero::__class
static constexpr const char __class[] =
R"doc(A class that constraints the tableaux of normal surface matching
equations to ensure that Euler characteristic is zero.

There are many ways of writing Euler characteritic as a linear
function. The function constructed here has integer coefficients, but
otherwise has no special properties of note.

This constraint currently only works with normal (and _not_ almost
normal) coordinates. It cannot be used with angle structure coordinate
systems.

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
    ``TreeEnumeration_EulerZero``). See the LPConstraint concept
    documentation for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPConstraintEulerZero::addRows
static constexpr const char addRows[] =
R"doc(Explicitly builds equations for the linear function(s) constrained by
this class. See the LPConstraint concept documentation for more
precise details on what this function does.

Precondition:
    For all columns in the array *col*, the members LPCol::extra have
    all been initialised to zero.

Python:
    The array *columnPerm* should be presented as a Python list of
    integers. Moreover, the argument *col* is not present at all,
    since LPCol is an internal class, not for general use. Instead,
    this routine returns a Python list of constraints, where each
    constraint is presented as a Python list of coefficients. Each of
    these inner lists will have size equal to the number of columns.

Parameter ``col``:
    the array of columns as stored in the initial tableaux, presented
    as a C-style array.

Parameter ``tri``:
    the underlying triangulation.

Parameter ``columnPerm``:
    the permutation that indicates which columns of the initial
    tableaux correspond to which normal coordinates, presented as a
    C-style array. This is the same permutation returned by
    ``LPInitialTableaux<...>::columnPerm()``.)doc";

// Docstring regina::python::doc::LPConstraintEulerZero::supported
static constexpr const char supported[] =
R"doc(Indicates whether the given vector encoding is supported by this
constraint class.

Parameter ``enc``:
    the vector encoding being queried, which must already be known to
    be supported by the generic TreeTraversal infrastructure.

Returns:
    ``True`` if and only if the given encoding is supported by this
    specific constraint class.)doc";

// Docstring regina::python::doc::LPConstraintEulerZero::verify_surface
static constexpr const char verify_surface[] =
R"doc(Ensures that the given normal surface satisfies the extra constraints
described by this class. For LPConstraintEulerZero, this simply tests
whether *s* has zero Euler characteristic.

Parameter ``s``:
    the surface to test.

Returns:
    ``True`` if the given surface satisfies these linear constraints,
    or ``False`` if it does not.)doc";

}; // struct LPConstraintEulerZero

struct LPConstraintNonSpun {

// Docstring regina::python::doc::LPConstraintNonSpun::__class
static constexpr const char __class[] =
R"doc(A class that constraints the tableaux of normal surface matching
equations to ensure that normal surfaces in an ideal triangulation are
compact (thereby avoiding spun normal surfaces with infinitely many
triangles).

At present this class can only work with oriented triangulations that
have precisely one vertex, which is ideal with torus link. Moreover,
it uses the SnapPea kernel for some of its computations, and so
SnapPea must be able to work directly with the given triangulation.
See the addRows() notes below for details on the exceptions that will
be thrown if these requirements are not met.

Also, at present this class can only work with quadrilateral normal
coordinates (and cannot handle almost normal coordinates at all). This
is _not_ explicitly checked; instead it appears as a precondition (see
below).

This constraint type cannot be used with angle structure coordinate
systems.

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
    ``TreeEnumeration_NonSpun``). See the LPConstraint concept
    documentation for further details.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPConstraintNonSpun::addRows
static constexpr const char addRows[] =
R"doc(Explicitly builds equations for the linear function(s) constrained by
this class. See the LPConstraint concept documentation for more
precise details on what this function does.

Precondition:
    For all columns in the array *col*, the members LPCol::extra have
    all been initialised to zero.

Exception ``InvalidArgument``:
    The underlying triangulation is not oriented with precisely one
    vertex, which must have a torus link.

Exception ``UnsolvedCase``:
    SnapPea retriangulated the underlying triangulation or produced a
    null triangulation, or the coefficients of the slope equations
    were found to be too large to store in a native C++ long integer.

Python:
    The array *columnPerm* should be presented as a Python list of
    integers. Moreover, the argument *col* is not present at all,
    since LPCol is an internal class, not for general use. Instead,
    this routine returns a Python list of constraints, where each
    constraint is presented as a Python list of coefficients. Each of
    these inner lists will have size equal to the number of columns.

Parameter ``col``:
    the array of columns as stored in the initial tableaux, presented
    as a C-style array.

Parameter ``tri``:
    the underlying triangulation.

Parameter ``columnPerm``:
    the permutation that indicates which columns of the initial
    tableaux correspond to which normal coordinates, presented as a
    C-style array. This is the same permutation returned by
    ``LPInitialTableaux<...>::columnPerm()``.)doc";

// Docstring regina::python::doc::LPConstraintNonSpun::supported
static constexpr const char supported[] =
R"doc(Indicates whether the given vector encoding is supported by this
constraint class.

Parameter ``enc``:
    the vector encoding being queried, which must already be known to
    be supported by the generic TreeTraversal infrastructure.

Returns:
    ``True`` if and only if the given encoding is supported by this
    specific constraint class.)doc";

// Docstring regina::python::doc::LPConstraintNonSpun::verify_surface
static constexpr const char verify_surface[] =
R"doc(Ensures that the given normal surface satisfies the extra constraints
described by this class. For LPConstraintNonSpun, this simply tests
whether *s* is compact.

Parameter ``s``:
    the surface to test.

Returns:
    ``True`` if the given surface satisfies these linear constraints,
    or ``False`` if it does not.)doc";

}; // struct LPConstraintNonSpun

struct LPConstraintNone {

// Docstring regina::python::doc::LPConstraintNone::__class
static constexpr const char __class[] =
R"doc(A do-nothing class that imposes no additional linear constraints on
the tableaux of normal surface or angle structure matching equations.
This constraint type can be used with both normal surface and angle
structure coordinate systems.

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
    class. See the LPConstraint concept documentation for further
    details on accessing other types of linear constraints from within
    Python.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPConstraintNone::addRows
static constexpr const char addRows[] =
R"doc(Explicitly builds equations for the linear function(s) constrained by
this class. See the LPConstraint concept documentation for more
precise details on what this function does.

For LPConstraintNone, since there are no constraints to add, this
function does nothing at all.

Precondition:
    For all columns in the array *col*, the members LPCol::extra have
    all been initialised to zero.

Python:
    The array *columnPerm* should be presented as a Python list of
    integers. Moreover, the argument *col* is not present at all,
    since LPCol is an internal class, not for general use. Instead,
    this routine returns a Python list of constraints, where each
    constraint is presented as a Python list of coefficients. Each of
    these inner lists will have size equal to the number of columns.

Parameter ``col``:
    the array of columns as stored in the initial tableaux, presented
    as a C-style array.

Parameter ``tri``:
    the underlying triangulation.

Parameter ``columnPerm``:
    the permutation that indicates which columns of the initial
    tableaux correspond to which normal or angle structure
    coordinates, presented as a C-style array. This is the same
    permutation returned by ``LPInitialTableaux<...>::columnPerm()``.)doc";

// Docstring regina::python::doc::LPConstraintNone::supported
static constexpr const char supported[] =
R"doc(Indicates whether the given vector encoding is supported by this
constraint class.

Parameter ``enc``:
    the vector encoding being queried, which must already be known to
    be supported by the generic TreeTraversal infrastructure.

Returns:
    ``True`` if and only if the given encoding is supported by this
    specific constraint class.)doc";

// Docstring regina::python::doc::LPConstraintNone::verify_structure
static constexpr const char verify_structure[] =
R"doc(Ensures that the given angle structure satisfies the extra constraints
described by this class. For LPConstraintNone, this function always
returns ``True``.

Parameter ``s``:
    the angle structure to test.

Returns:
    ``True`` if the given angle structure satisfies these linear
    constraints, or ``False`` if it does not.)doc";

// Docstring regina::python::doc::LPConstraintNone::verify_surface
static constexpr const char verify_surface[] =
R"doc(Ensures that the given normal surface satisfies the extra constraints
described by this class. For LPConstraintNone, this function always
returns ``True``.

Parameter ``s``:
    the surface to test.

Returns:
    ``True`` if the given surface satisfies these linear constraints,
    or ``False`` if it does not.)doc";

}; // struct LPConstraintNone

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

