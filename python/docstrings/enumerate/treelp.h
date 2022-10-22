/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::LPData
static const char *LPData =
R"doc(Stores an intermediate tableaux for the dual simplex method, and
contains all of the core machinery for using the dual simplex method.

This class forms part of the tree traversal algorithms for enumerating
and locating normal surfaces, as described in "A tree traversal
algorithm for decision problems in knot theory and 3-manifold
topology", Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801,
and "A fast branching algorithm for unknot recognition with
experimental polynomial-time behaviour", Burton and Ozlen,
arXiv:1211.1079. It is also used for locating a single strict angle
structure, and for enumerating all taut angle structures.

This class is designed to represent a state partway through the tree
traversal algorithm, where the tableaux has been altered to constrain
some variables:

* Some variables have been "deactivated". This means we fix them to
  zero permanently, and pretend that the corresponding columns do not
  exist in the matrix. As a result, the rank of the matrix may smaller
  than it was when we began the tree traversal.

* Some variables have been constrained to be positive; as described in
  Burton and Ozlen, it is safe to do this using the non-strict
  inequality x_i ≥ 1 (instead of the strict inequality x_i > 0, which
  is more difficult to enforce). We enforce this constraing using a
  change of variable: we replace the variable x_i with (x_i - 1),
  which is then constrained to be non-negative as usual. The new
  variable (x_i - 1) uses the same column in the tableaux (we perform
  the actual change of variable by editing the tableaux itself using
  column operations). Be warned: as a result, when we arrive at a
  final solution and collect the values of the variables, we must
  remember to _increment_ the values of any such variables by one.

We do not store the full tableaux (which is dense and slow to work
with). Instead we store the matrix of row operations that were applied
to the original starting tableaux (in the notation of Burton and
Ozlen, we store the matrix M_β^{-1}, where M is the original matrix
stored in the class LPInitialTableaux, and β is the current basis).

If the system is infeasible (because the constraints on variables as
described above are too severe), then the contents of the internal
data members are undefined (other than the data member *feasible_*,
which is guaranteed to be ``False``). This is because the code is
optimised to abort any operation as soon as infeasibility is detected,
which may leave the data members in a broken state. If you are not
sure, you should always call isFeasible() before performing any other
query or operation on this tableaux.

This class is designed to be used in a backtracking search, which
means the API is cumbersome but we can quickly rewrite and copy data.
The rules are as follows:

* Before using an LPData object, you must reserve the necessary memory
  by calling reserve() and passing the original starting tableaux.

* After this, you can reset the data by calling one of the
  initialisation routines initStart() or initClone(), and you can call
  these initialisation routines as often as you like.

Like LPInitialTableaux, this class can enforce additional linear
constraints (such as positive Euler characteristic) through the
template parameter LPConstraint. If there are no such constraints,
simply use the template parameter LPConstraintNone.

In the context of normal surfaces (not angle structures): Although the
underlying coordinate system is based on quadrilaterals and
(optionally) triangles, this class has elementary support for octagons
also, as seen in _almost_ normal surface theory. For the purposes of
this class, an octagon is represented as a pair of quadrilaterals of
different types in the same tetrahedron: these meet the boundary of
the tetrahedron in the same arcs as a single octagon, and therefore
interact with the matching equations in the same way.

To declare that you will be using octagons in some tetrahedron, you
must call constrainOct(quad1, quad2), where *quad1* and *quad2* are
the two corresponding quadrilateral columns. This will have the
following effects, all of which may alter the tableaux:

* There will be some changes of variable. One of the two variables x_i
  will be replaced with (x_i - 1), forcing the number of octagons to
  be positive. The other variable x_j will be replaced with (x_j -
  x_i), which will be set to zero and deactivated. There is no
  guarantee as to which of the two variables *quad1* and *quad2* will
  be kept and which will be deactivated: this will depend on the
  layout of the tableaux when constrainOct() is called.

* If you are imposing additional constraints through the
  *LPConstraint* template parameter, the corresponding linear
  constraint functions may change their values (since the coefficients
  they use for octagon types need not be related to the coefficients
  for the two corresponding quadrilateral columns). Any such changes,
  if necessary, are encoded by the constant
  LPConstraint::octAdjustment.

This class has been optimised to ensure that you only have one octagon
type declared at any given time (which is consistent with the
constraints of almost normal surface theory).

All tableaux elements are of the integer class *IntType*, which is
supplied as a template argument. This same integer class will be used
as a template argument for *LPConstraint*.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. However, due to the unusual create-reserve-
initialise procedure, it does not support copying (either by copy
construction or copy assignment). Because of the move semantics, this
class avoids deep copies, even when passing or returning objects by
value.

Precondition:
    The template parameter LPConstraint must be one of the subclasses
    of LPConstraintBase. See the LPConstraintBase class notes for
    further details.

Precondition:
    The default constructor for the template class IntType must
    intialise each new integer to zero. The classes Integer and
    NativeInteger, for instance, have this property.

Python:
    This is a heavily templated class; nevertheless, many variants are
    now made available to Python users. Each class name is of the form
    LPData_*LPConstraint*, where the suffix *LPConstraint* is an
    abbreviated version of the *LPConstraint* template parameter; this
    suffix is omitted entirely for the common case LPConstraintNone.
    An example of such a Python class name is
    ``LPData_EulerPositive``. You are encouraged to look through the
    Regina namespace to see which constraint classes are supported
    under Python. In all cases, the IntType parameter is taken to be
    regina::Integer.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPInitialTableaux
static const char *LPInitialTableaux =
R"doc(Stores an adjusted matrix of homogeneous linear matching equations
based on a given triangulation, in sparse form. Typically these will
be the normal surface matching equations in some coordinate system, or
the angle structure equations.

This class forms part of the tree traversal algorithms for enumerating
and locating normal surfaces, as described in "A tree traversal
algorithm for decision problems in knot theory and 3-manifold
topology", Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801,
and "A fast branching algorithm for unknot recognition with
experimental polynomial-time behaviour", Burton and Ozlen,
arXiv:1211.1079. It is also used for locating a single strict angle
structure, and for enumerating all taut angle structures.

The adjustments (which are all carried out in the LPInitialTableaux
class constructor) are as follows:

* the rows of the matching equation matrix have been reordered so that
  the first rank() rows are full rank;

* the columns of the matching equation matrix have been reordered
  according to the permutation returned by columnPerm(). This is a
  heuristic reordering designed to improve the performance of the tree
  traversal algorithm; see columnPerm() for a list of constraints that
  such a reordering must satisfy.

There is also optional support for adding extra linear constraints
(such as a constraint on Euler characteristic for normal surfaces).
These extra constraints are supplied by the template parameter
*LPConstraint*, and will generate LPConstraint::nConstraints
additional rows and columns (used by the additional variables that
evaluate the corresponding linear functions). If there are no
additional constraints, simply use the template parameter
LPConstraintNone.

For some *LPConstraint* template arguments, Regina may discover at
runtime that it is impossible to add the corresponding extra linear
constraints (e.g., the constraints might require some preconditions on
the underlying triangulation that are not met). In this case, the
LPInitialTableaux class constructor will throw an exception, as noted
in the constructor documentation below.

This class is optimised for working with _columns_ of the matrix (in
particular, multiplying columns of this matrix by rows of some other
matrix).

This class works with a broad class of vector encodings for normal
surfaces or angle structures, as described by the LPSystem class, and
within that broad class it does not know _which_ particular encoding
or underlying coordinate system is being used. In particular, the
matching equations it uses will _always_ be one of the standard tri-
quad normal matching equations (if LPSystem::standard() is ``True``),
the quad normal matching equations (if LPSystem::quad() is ``True``),
or the homogeneous angle equations (if LPSystem::angles() is true). If
you need to add extra matching equations beyond these, use the
LPConstraint template argument as outlined above. If you need to
support more exotic vector encodings (e.g., for octagonal almost
normal surfaces), you will need to find a way to represent it using
one of these three broad classes; see the LPData class notes for how
this is done with octagons.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

.. warning::
    The implementation of this class relies on the fact that the sum
    of _absolute values_ of all coefficients in each column is at most
    four (not counting the rows for any optional extra constraints).
    If you are extending this class to work with more general matching
    equation matrices, you may need to change the implementation
    accordingly.

Precondition:
    The template parameter LPConstraint must be one of the subclasses
    of LPConstraintBase. See the LPConstraintBase class notes for
    further details.

Python:
    This is a heavily templated class; nevertheless, many variants are
    now made available to Python users. Each class name is of the form
    LPInitialTableaux_*LPConstraint*, where the suffix *LPConstraint*
    is an abbreviated version of the *LPConstraint* template
    parameter; this suffix is omitted entirely for the common case
    LPConstraintNone. An example of such a Python class name is
    ``LPInitialTableaux_NonSpun``. You are encouraged to look through
    the Regina namespace to see which constraint classes are supported
    under Python.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPMatrix
static const char *LPMatrix =
R"doc(A matrix class for use with linear programming.

This class is used in the tree traversal algorithms for enumerating
and locating vertex normal surfaces, as described in "A tree traversal
algorithm for decision problems in knot theory and 3-manifold
topology", Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801,
and "A fast branching algorithm for unknot recognition with
experimental polynomial-time behaviour", Burton and Ozlen,
arXiv:1211.1079. It is also used for locating a single strict angle
structure, and for enumerating all taut angle structures.

The operations on this matrix class are tailored and optimised
specifically for use with the dual simplex method in the context of a
repetitive backtracking search. As a result, the API is cumbersome and
highly specialised, which makes this matrix class inappropriate for
general use. If you just want a general-use integer matrix class, use
MatrixInt instead.

It is _critical_ that, before using an LPMatrix, you reserve space for
its elements, and then fix a specific size. A matrix for which both
tasks have been done will be called *initialised*. You can initialise
a matrix in one of two ways:

* by using the (*rows*, *columns*) constructor, which does everything
  for you;

* by using the default (no-arguments) constructor, then calling
  reserve(), and then calling one of the initialisation routines
  initClone() or initIdentity().

You may call the initialisation initClone() and initIdentity()
routines more than once (e.g., during a backtracking search), and you
may use different matrix sizes each time. However, you may never use
more elements than you originally reserved space for.

This matrix is stored in dense form. All elements are of the integer
class *IntType*, which is supplied as a template argument.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. However, due to the unusual create-reserve-
initialise procedure, it does not support copying (either by copy
construction or copy assignment). Because of the move semantics, this
class avoids deep copies, even when passing or returning objects by
value.

Precondition:
    The default constructor for the template class IntType must
    intialise each new integer to zero. The classes Integer and
    NativeInteger, for instance, have this property.

Python:
    The template argument *IntType* is taken to be regina::Integer.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LPSystem
static const char *LPSystem =
R"doc(Indicates which broad class of vector encodings a particular tableaux
is designed to work with. This type is used by Regina's linear
programming machinery, and in particular by the LPInitialTableaux
class.

By "broad class of vector encodings", we allow only three options:

* _standard_ encodings, which cover all normal surface encodings that
  include triangle coordinates, and where the tableaux holds triangle
  and quadrilateral columns but nothing else;

* _quad_ encodings, which cover all normal surface encodings that do
  not include triangle coordinates, and where the tableaux holds
  quadrilateral columns but nothing else;

* _angle_ encodings, which cover angle structure encodings, and where
  the tableaux holds angle columns as well as a single scaling column.

When working with almost normal coordinate systems, we represent
octagons as pairs of intersecting quadrilaterals; see the LPData class
notes for more information on how this works. This means, for example,
that the coordinate system NS_AN_STANDARD will fall under the class of
standard encodings, and NS_AN_QUAD_OCT will fall under the class of
quad encodings.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

namespace LPData_ {

// Docstring regina::python::doc::LPData_::__default
static const char *__default =
R"doc(Constructs a new tableaux. You _must_ call reserve() before doing
anything else with this tableaux.)doc";

// Docstring regina::python::doc::LPData_::columns
static const char *columns =
R"doc(Returns the number of columns in this tableaux.

Note that, if we are imposing extra constraints through the template
parameter LPConstraint, then there will be extra variables to enforce
these, and so the number of columns will be larger than in the
original matching equation matrix.

Returns:
    the number of columns.)doc";

// Docstring regina::python::doc::LPData_::constrainOct
static const char *constrainOct =
R"doc(Declares that two quadrilateral coordinates within a tetrahedron are
to be combined into a single octagon coordinate, for use with almost
normal surfaces, and constrains the system accordingly.

This constrains the system in several ways, as discussed in detail in
the LPData class notes. In theory, we set the two quadrilateral
coordinates to be equal, and also insist that the number of octagons
be strictly positive. In practice, we do this through several changes
of variable; see the LPData class notes for a detailed discussion of
precisely how the variables and tableaux will change.

This routine will work even if one of the given quadrilateral
variables has already been deactivated, but in this case the routine
will immediately set the system to infeasible and return.

This routine is not used with angle structure coordinates.

Precondition:
    This is the first time constrainOct() has been called on this
    tableaux. This is because this class can only handle one octagon
    type in the entire system.

Precondition:
    Variables *quad1* and *quad2* represent different quadrilateral
    coordinates in the same tetrahedron of the underlying
    triangulation.

.. warning::
    If you have previously called constrainPositive() or
    constrainOct() on one of the given variables, then these prior
    routines will have performed a change of variable. Any new call to
    constrainOct() involving this same variable will constrain the
    _new_ variable, not the original, and so might not have the
    intended effect.

Parameter ``quad1``:
    one of the two quadrilateral types that we combine to form the new
    octagon type. This should be a column index with respect to this
    tableaux (i.e., it must take into account any permutation of
    columns from the original matching equations).

Parameter ``quad2``:
    the other of the two quadrilateral types that we combine to form
    the new octagon type. Again this should be a column index with
    respect to this tableaux.)doc";

// Docstring regina::python::doc::LPData_::constrainPositive
static const char *constrainPositive =
R"doc(Constrains this system further by constraining the given variable to
be strictly positive. We do this using a change of variable that
effectively replaces x_pos with the new variable x'_pos = x_pos - 1
(which we simply constrain to be non-negative as usual). See the
LPData class notes for details.

This routine will work even if the given variable has already been
deactivated, but in this case the routine will immediately set the
system to infeasible and return.

.. warning::
    If you have previously called constrainPositive() or
    constrainOct() on this variable, then these prior routines will
    have performed a change of variable. Any new call to
    constrainPositive() on this same variable will constrain the _new_
    variable, not the original, and so might not have the intended
    effect.

Parameter ``pos``:
    the index of the variable that is to be constrained as positive.
    This must be between 0 and origTableaux_->columns()-1 inclusive.
    The index should be with respect to this tableaux (i.e., it must
    take into account any permutation of columns from the original
    matching equations).)doc";

// Docstring regina::python::doc::LPData_::constrainZero
static const char *constrainZero =
R"doc(Constrains this system further by setting the given variable to zero
and deactivating it. See the LPData class notes for details.

This routine will work even if the given variable has already been
deactivated (and it will do nothing in this case).

.. warning::
    If you have previously called constrainPositive() or
    constrainOct() on this variable, then these prior routines will
    have performed a change of variable. Any new call to
    constraintZero() on this same variable will constraint the _new_
    variable, not the original, and so might not have the intended
    effect.

Parameter ``pos``:
    the index of the variable that is to be set to zero. This must be
    between 0 and origTableaux_->columns()-1 inclusive. The index
    should be with respect to this tableaux (i.e., it must take into
    account any permutation of columns from the original matching
    equations).)doc";

// Docstring regina::python::doc::LPData_::coordinateColumns
static const char *coordinateColumns =
R"doc(Returns the number of columns in this tableaux that correspond to
normal coordinates or angle structure coordinates. This is precisely
the number of columns in the original matrix of matching equations.

Returns:
    the number of normal or angle structure coordinate columns.)doc";

// Docstring regina::python::doc::LPData_::extractSolution
static const char *extractSolution =
R"doc(Extracts the values of the individual variables from the current
basis, with some modifications (as described below). The values of the
variables will be returned in vector form.

The modifications are as follows:

* We extract variables that correspond to the original matching
  equations obtained from the underlying triangulation, _not_ the
  current tableaux and _not_ even the original starting tableaux
  stored in origTableaux_. In other words, when we fill the resulting
  vector, we undo the column permutation described by
  LPInitialTableaux::columnPerm(), and we undo any changes of variable
  that were caused by calls to constrainPositive() and/or
  constrainOct().

* To ensure that the variables are all integers, we scale the
  resulting vector by the smallest positive rational multiple for
  which all elements of the vector are integers.

This routine is not used as an internal part of the tree traversal
algorithm; instead it is offered as a helper routine for
reconstructing the normal surfaces or angle structures that result.

Precondition:
    No individual coordinate column has had more than one call to
    either of constrainPositive() or constrainOct() (otherwise the
    coordinate will not be correctly reconstructed). Any additional
    columns arising from LPConstraint are exempt from this
    requirement.

Precondition:
    The precision of integers in *RayClass* is at least as large as
    the precision of *IntType* (as used by LPData).

Template parameter ``RayClass``:
    the class used to hold the output vector. This should be Vector<T>
    where *T* is one of Regina's own integer types (Integer,
    LargeInteger or NativeInteger). In particular, this ensures that
    all elements of a newly-created output vector will be
    automatically initialised to zero.

Python:
    The type vector should be passed as a Python list of integers (for
    example, in the enumeration of normal surfaces, there would be one
    integer per tetrahedron, each equal to 0, 1, 2 or 3). The
    *RayClass* argument is taken to be Vector<Integer>.

Parameter ``type``:
    the type vector corresponding to the current state of this
    tableaux, indicating which variables were previously fixed as
    positive via calls to constrainPositive(). This is necessary
    because LPData does not keep such historical data on its own. The
    order of these types should be with respect to the permuted
    columns (i.e., it should reflect the columns as they are stored in
    this tableaux, not the original matching equations). As a special
    case, when extracting a strict angle structure one may pass *type*
    = ``None``, in which case this routine will assume that _every_
    coordinate was constrained as positive.

Returns:
    a vector containing the values of all the variables. This vector
    will have length origTableaux_->coordinateColumns().)doc";

// Docstring regina::python::doc::LPData_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given tableaux.

This global routine simply calls LPData<LPConstraint,
IntType>::swap(); it is provided so that LPData<LPConstraint, IntType>
meets the C++ Swappable requirements.

Parameter ``a``:
    the first tableaux whose contents should be swapped.

Parameter ``b``:
    the second tableaux whose contents should be swapped.)doc";

// Docstring regina::python::doc::LPData_::initClone
static const char *initClone =
R"doc(Initialises this tableaux to be a clone of the given tableaux. This is
used in the tree traversal algorithm as we work our way down the
search tree, and child nodes "inherit" tableaux from their parent
nodes.

Precondition:
    reserve() has already been called.

Parameter ``parent``:
    the tableaux to clone.)doc";

// Docstring regina::python::doc::LPData_::initStart
static const char *initStart =
R"doc(Initialises this tableaux by beginning at the original starting
tableaux and working our way to any feasible basis.

This routine also explicitly enforces the additional constraints from
the template parameter LPConstraint (i.e., this routine is responsible
for forcing the corresponding linear function(s) to be zero or
strictly positive as appropriate).

It is possible that a feasible basis cannot be found; you should test
isFeasible() after running this routine to see whether this is the
case.

Precondition:
    reserve() has already been called.)doc";

// Docstring regina::python::doc::LPData_::isActive
static const char *isActive =
R"doc(Determines whether the given variable is currently active. See the
LPData class notes for details.

Parameter ``pos``:
    the index of the variable to query. This must be between 0 and
    origTableaux_->columns()-1 inclusive. The index should be with
    respect to this tableaux (i.e., it must take into account any
    permutation of columns from the original matching equations).)doc";

// Docstring regina::python::doc::LPData_::isFeasible
static const char *isFeasible =
R"doc(Returns whether or not this system is feasible.

A system may become infeasible when we add too many extra constraints
on the variables (such as forcing them to be positive, or setting them
to zero); see the LPData class notes for details on these constraints.

.. warning::
    As explained in the class notes, if this system is infeasible then
    any queries or operations (other than calling isFeasible() itself)
    are undefined.

Returns:
    ``True`` if this system is feasible, or ``False`` if it is
    infeasible.)doc";

// Docstring regina::python::doc::LPData_::reserve
static const char *reserve =
R"doc(Reserves enough memory for this tableaux to work with. You _must_ call
this routine before doing anything else with this tableaux.

The data in this tableaux will not be initialised, and the contents
and behaviour of this tableaux will remain undefined until you call
one of the initialisation routines initStart() or initClone().

Parameter ``origTableaux``:
    the original starting tableaux that holds the adjusted matrix of
    matching equations, before the tree traversal algorithm began.)doc";

// Docstring regina::python::doc::LPData_::sign
static const char *sign =
R"doc(Returns the sign of the given variable under the current basis. This
does _not_ attempt to "undo" any changes of variable caused by prior
calls to constrainPositive() or constrainOct(); it simply tests the
sign of the variable in the given column of the tableaux in its
current form.

Specifically: if the given variable is inactive or non-basic, this
routine returns zero. If the given variable is in the basis, this
routine returns the sign of the corresponding integer on the right-
hand side of the tableaux.

Parameter ``pos``:
    the index of the variable to query. This must be between 0 and
    origTableaux_->columns()-1 inclusive. The index should be with
    respect to this tableaux (i.e., it must take into account any
    permutation of columns from the original matching equations).

Returns:
    the sign of the variable as described above; this will be either
    1, 0 or -1.)doc";

// Docstring regina::python::doc::LPData_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given tableaux.

It does not matter if the two tableaux have different sizes, or if one
or both is unintialised; if so then these properties will be swapped
also.

Parameter ``other``:
    the tableaux whose contents should be swapped with this.)doc";

}

namespace LPInitialTableaux_ {

// Docstring regina::python::doc::LPInitialTableaux_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given matrix.

Parameter ``src``:
    the matrix to copy.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::__init
static const char *__init =
R"doc(Construts this adjusted sparse matrix of matching equations.

Note that LPInitialTableaux does not copy the given triangulation; it
merely keeps a reference to it. The triangulation should not change
during the lifespan of this object.

Precondition:
    The given triangulation is non-empty.

Exception ``InvalidArgument``:
    It was not possible to add the extra constraints from the
    LPConstraint template argument, due to an error which should have
    been preventable with the right checks in advance. Such exceptions
    are generated by the *LPConstraint* class, and so you should
    consult the class documentation for your chosen *LPConstraint*
    template argument to see if this is a possibility.

Exception ``InvalidArgument``:
    It was not possible to add the extra constraints from the
    LPConstraint template argument, due to an error that was
    "genuinely" unforseeable. Again, such exceptions are generated by
    your chosen *LPConstraint* class, and you should consult its
    documentation to see if this is a possibility.

Parameter ``tri``:
    the underlying 3-manifold triangulation.

Parameter ``enc``:
    the normal surface vector encoding that we are using for our
    enumeration task. This may be any valid NormalEncoding object,
    including the special angle structure encoding.

Parameter ``enumeration``:
    ``True`` if we should optimise the tableaux for a full enumeration
    of vertex surfaces or taut angle structures, or ``False`` if we
    should optimise the tableaux for an existence test (such as
    searching for a non-trivial normal disc or sphere, or a strict
    angle structure).)doc";

// Docstring regina::python::doc::LPInitialTableaux_::columnPerm
static const char *columnPerm =
R"doc(Returns the permutation that describes how the columns of the matching
equation matrix were reordered. This permutation maps column numbers
in this adjusted matching equation matrix to column numbers in the
original (unmodified) matching equation matrix that was originally
derived from the triangulation.

The permutation is returned as an array of columns() integers, such
that column *i* of this adjusted matrix corresponds to column
``columnPerm()[i]`` of the original matrix.

If you are imposing additional constraints through the template
parameter LPConstraint, then the corresponding extra variables will be
included in the permutation; however, these are never moved and will
always remain the rightmost variables in this system (i.e., the
columns of highest index).

As well as the requirement that this is a genuine permutation of
0,...,columns()-1, this array will also adhere to the following
constraints. In the following discussion, *n* refers to the number of
tetrahedra in the underlying triangulation.

* The quadrilateral coordinate columns must appear as the first 3*n*
  columns of the adjusted matrix. In particular, when working in the
  7*n*-dimensional standard normal coordinate system, the remaining
  4*n* triangle coordinate columns must appear last.

* The quadrilateral coordinate columns must be grouped by tetrahedron
  and ordered by quadrilateral type. In other words, for each *i* =
  0,...,*n*-1, there will be some tetrahedron *j* for which the three
  columns 3*i*, 3*i*+1 and 3*i*+2 refer to the quadrilaterals in
  tetrahedron *j* of types 0, 1 and 2 respectively. Phrased loosely,
  we are allowed to reorder the tetrahedra, but not the quadrilateral
  coordinates within each tetrahedron.

* The triangle coordinate columns (if we are working in standard
  normal coordinates) must likewise be grouped by tetrahedron, and
  these tetrahedra must appear in the same order as for the
  quadrilateral types. In other words, for each *i* = 0,...,*n*-1, the
  quadrilateral columns 3*i*, 3*i*+1 and 3*i*+2 and the triangle
  columns 3*n*+4*i*, 3*n*+4*i*+1, 3*n*+4*i*+2 and 3*n*+4*i*+3 all
  refer to the same tetrahedron.

* For angle structure coordinates, the constraints are analogous to
  those for quadrilateral coordinates: the angle coordinates must be
  grouped by tetrahedron and ordered by angle type, and the final
  scaling coordinate must remain last.

Python:
    This routine returns a Python list.

Returns:
    details of the permutation describing how columns were reordered.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::columns
static const char *columns =
R"doc(Returns the number of columns in this matrix.

Note that, if we are imposing extra constraints through the template
parameter LPConstraint, then there will be extra variables to enforce
these, and so the number of columns will be larger than in the
original matching equation matrix.

Returns:
    the number of columns.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::coordinateColumns
static const char *coordinateColumns =
R"doc(Returns the number of columns that correspond to normal coordinates or
angle structure coordinates. This is precisely the number of columns
in the original matrix of matching equations.

Returns:
    the number of normal or angle structure coordinate columns.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::fillInitialTableaux
static const char *fillInitialTableaux =
R"doc(Fills the given matrix with the contents of this matrix. This
effectively copies this sparse but highly specialised matrix
representation into a dense but more flexible matrix representation.

Precondition:
    The given matrix has already been initialised to size rank() *
    columns(), and all of its elements have already been set to zero.
    Note that this can all be arranged by calling the constructor
    LPMatrix::LPMatrix(size_t, size_t).

Parameter ``m``:
    the matrix to fill.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given matrices.

This global routine simply calls LPInitialTableaux<IntType>::swap();
it is provided so that LPInitialTableaux<IntType> meets the C++
Swappable requirements.

Parameter ``a``:
    the first matrix whose contents should be swapped.

Parameter ``b``:
    the second matrix whose contents should be swapped.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::multColByRow
static const char *multColByRow =
R"doc(Computes the inner product of (i) the given row of the given matrix
with (ii) the given column of this matrix.

This routine is optimised to use the sparse representation of columns
in this matrix.

Precondition:
    The given matrix *m* has precisely rank() columns.

Parameter ``m``:
    the matrix whose row we will use in the inner product.

Parameter ``mRow``:
    the row of the matrix *m* to use in the inner product.

Parameter ``thisCol``:
    the column of this matrix to use in the inner product.

Returns:
    the resulting inner product.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::multColByRowOct
static const char *multColByRowOct =
R"doc(A variant of multColByRow() that takes into account any adjustments to
the tableaux that are required when this is a quadrilateral column
being used to represent an octagon type.

The LPData class offers support for octagonal almost normal surfaces,
in which exactly one tetrahedron is allowed to have exactly one
octagon type. We represent such an octagon as a _pair_ of incompatible
quadrilaterals within the same tetrahedron. See the LPData class notes
for details on how this works.

In some settings where we are using additional constraints through the
template parameter LPConstraint, these extra constraints behave
differently in the presence of octagons (i.e., the coefficient of the
octagon type is not just the sum of coefficients of the two
constituent quadrilateral types). This routine effectively allows us
to adjust the tableaux accordingly.

Specifically: this routine computes the inner product of (i) the given
row of the given matrix with (ii) the given column of this matrix. We
assume that the given column of this matrix describes one of the two
quadrilateral coordinates in some tetrahedron that together form an
octagon type, and (via the information given by
LPConstraint::octAdjustment) we implicitly adjust the coefficients of
our extra constraints accordingly.

This routine is optimised to use the sparse representation of columns
in this matrix.

This routine is not used with angle structure coordinates.

Precondition:
    The given matrix *m* has precisely rank() columns.

Precondition:
    Column *thisCol* of this matrix describes one of the two
    quadrilateral coordinates that are being combined to form an
    octagon type within some tetrahedron.

Parameter ``m``:
    the matrix whose row we will use in the adjusted inner product.

Parameter ``mRow``:
    the row of the matrix *m* to use in the adjusted inner product.

Parameter ``thisCol``:
    the column of this matrix to use in the adjusted inner product.

Returns:
    the resulting adjusted inner product.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::rank
static const char *rank =
R"doc(Returns the rank of this matrix.

Note that, if we are imposing extra constraints through the template
parameter LPConstraint, then there will be extra variables to enforce
these, and so the rank will be larger than the rank of the original
matching equation matrix.

Returns:
    the matrix rank.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given matrix.

It does not matter if the two matrices have different sizes, and/or
work with different vector encodings; if so then these properties will
be swapped also.

Parameter ``other``:
    the matrix whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::system
static const char *system =
R"doc(Returns the broad class of vector encodings that this tableaux works
with. This broad class is deduced from the vector encoding that was
passed to the class constructor, and it completely determines which
matching equations were generated as a result.

See the LPInitialTableaux class notes for more information on these
three broad classes and how they affect the tableaux.

Returns:
    the class of vector encodings used by this tableaux.)doc";

// Docstring regina::python::doc::LPInitialTableaux_::tri
static const char *tri =
R"doc(Returns the underlying 3-manifold triangulation from which the
matching equations were derived.

Returns:
    the underlying triangulation.)doc";

}

namespace LPMatrix_ {

// Docstring regina::python::doc::LPMatrix_::__default
static const char *__default =
R"doc(Creates an uninitialised matrix with no memory storage.

You _must_ call reserve() and then either initClone() or
initIdentity() before this matrix will become initialised.)doc";

// Docstring regina::python::doc::LPMatrix_::__eq
static const char *__eq =
R"doc(Determines whether this and the given matrix are equal.

Two matrices are equal if and only if their dimensions are the same,
and the corresponding elements of each matrix are equal.

It is safe to compare matrices of different dimensions, and it is safe
to compare matrices that might not yet be initialised. Two
uninitialised matrices will compare as equal.

Parameter ``other``:
    the matrix to compare with this.

Returns:
    ``True`` if and only if the two matrices are equal.)doc";

// Docstring regina::python::doc::LPMatrix_::__init
static const char *__init =
R"doc(Creates a fully initialised *rows* by *cols* matrix with all elements
set to zero.

This routine reserves space for precisely *rows* * *cols* elements. In
other words, you may later re-initialise the matrix to become smaller
if you like, but you cannot re-initialise the matrix to become larger.

Parameter ``rows``:
    the number of rows in the new matrix. This must be strictly
    positive.

Parameter ``cols``:
    the number of columns in the new matrix. This must be strictly
    positive.)doc";

// Docstring regina::python::doc::LPMatrix_::__ne
static const char *__ne =
R"doc(Determines whether this and the given matrix are not equal.

Two matrices are equal if and only if their dimensions are the same,
and the corresponding elements of each matrix are equal.

It is safe to compare matrices of different dimensions, and it is safe
to compare matrices that might not yet be initialised. Two
uninitialised matrices will compare as equal.

Parameter ``other``:
    the matrix to compare with this.

Returns:
    ``True`` if and only if the two matrices are not equal.)doc";

// Docstring regina::python::doc::LPMatrix_::columns
static const char *columns =
R"doc(Returns the number of columns in this matrix. This relates to the
currently assigned matrix size, not the total amount of memory that
was originally reserved.

Returns:
    the number of columns.)doc";

// Docstring regina::python::doc::LPMatrix_::combRow
static const char *combRow =
R"doc(Applies a particular row operation to this matrix.

Specifically, row *dest* will be replaced with the linear combination:
(*destCoeff* * row *dest* - *srcCoeff* * row *src*) / *div*.

Precondition:
    *dest* and *src* are not equal.

Precondition:
    It is known in advance that every integer in (*destCoeff* * row
    *dest* - *srcCoeff* * row *src*) will be divisible by *div*. In
    other words, it is known in advance that we can use exact integer
    division without remainders.

Parameter ``destCoeff``:
    the coefficient applied to row *dest* in the linear combination.

Parameter ``dest``:
    the index of the row to replace. This must be between 0 and
    rows()-1 inclusive.

Parameter ``srcCoeff``:
    the coefficient applied to row *src* in the linear combination.

Parameter ``src``:
    the index of the other row used in this linear combination. This
    must be between 0 and rows()-1 inclusive.

Parameter ``div``:
    the integer to divide the final row by. This must be non-zero.)doc";

// Docstring regina::python::doc::LPMatrix_::combRowAndNorm
static const char *combRowAndNorm =
R"doc(Applies a particular row operation to this matrix, and then
normalises.

Specifically, row *dest* will be replaced with the linear combination:
(*destCoeff* * row *dest* - *srcCoeff* * row *src*); then, if row
*dest* is non-zero, it will be normalised by dividing through by the
gcd of its elements. Note that this gcd is always taken to be positive
(i.e., the final normalisation will never change the signs of the
elements in the row).

Precondition:
    *dest* and *src* are not equal.

Parameter ``destCoeff``:
    the coefficient applied to row *dest* in the linear combination.

Parameter ``dest``:
    the index of the row to replace. This must be between 0 and
    rows()-1 inclusive.

Parameter ``srcCoeff``:
    the coefficient applied to row *src* in the linear combination.

Parameter ``src``:
    the index of the other row used in this linear combination. This
    must be between 0 and rows()-1 inclusive.

Returns:
    the positive gcd that row *dest* was scaled down by, or 0 if row
    *dest* is entirely zero.)doc";

// Docstring regina::python::doc::LPMatrix_::entry
static const char *entry =
R"doc(Returns a read-write reference to the given element of this matrix.

Python:
    The entry() routine gives direct read-write access to matrix
    elements, but does not allow them to be set using the assignment
    operator. In other words, code such as ``matrix.entry(r,
    c).negate()`` will work, but ``matrix.entry(r, c) = value`` will
    not. To assign values to matrix elements, you should instead use
    the syntax ``matrix.set(row, column, value)``. This set() routine
    returns nothing, and is provided for python only (i.e., it is not
    part of the C++ calculation engine).

Parameter ``row``:
    the row of the requested element. This must be between 0 and
    rows()-1 inclusive.

Parameter ``col``:
    the column of the requested element. This must be between 0 and
    columns()-1 inclusive.)doc";

// Docstring regina::python::doc::LPMatrix_::entry_2
static const char *entry_2 =
R"doc(Returns a read-only reference to the given element of this matrix.

Parameter ``row``:
    the row of the requested element. This must be between 0 and
    rows()-1 inclusive.

Parameter ``col``:
    the column of the requested element. This must be between 0 and
    columns()-1 inclusive.)doc";

// Docstring regina::python::doc::LPMatrix_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given matrices.

This global routine simply calls LPMatrix<IntType>::swap(); it is
provided so that LPMatrix<IntType> meets the C++ Swappable
requirements.

Parameter ``a``:
    the first matrix whose contents should be swapped.

Parameter ``b``:
    the second matrix whose contents should be swapped.)doc";

// Docstring regina::python::doc::LPMatrix_::initClone
static const char *initClone =
R"doc(Initialises this matrix to a copy of the given matrix.

This matrix does not yet need to be initialised, but it does need to
have enough space reserved.

You may call this routine on an already-initialised matrix, and you
may use this routine to assign it a different size (as long as enough
space was originally reserved).

Precondition:
    If this matrix has not been initialised before, then reserve()
    must have already been called.

Precondition:
    This matrix has enough space reserved for at least clone.rows() *
    clone.columns() elements.

Parameter ``clone``:
    the matrix to copy.)doc";

// Docstring regina::python::doc::LPMatrix_::initIdentity
static const char *initIdentity =
R"doc(Initialises this matrix to the identity matrix of the given size.

This matrix does not yet need to be initialised, but it does need to
have enough space reserved.

You may call this routine on an already-initialised matrix, and you
may use this routine to assign it a different size (as long as enough
space was originally reserved).

Precondition:
    If this matrix has not been initialised before, then reserve()
    must have already been called.

Precondition:
    This matrix has enough space reserved for at least *size* * *size*
    elements.

Parameter ``size``:
    the number of rows, and also the number of columns, that will be
    assigned to this matrix. This must be strictly positive.)doc";

// Docstring regina::python::doc::LPMatrix_::negateRow
static const char *negateRow =
R"doc(Negates all elements in the given row of this matrix.

Parameter ``row``:
    the row whose elements should be negated. This must be between 0
    and rows()-1 inclusive.)doc";

// Docstring regina::python::doc::LPMatrix_::reserve
static const char *reserve =
R"doc(Reserves enough space to store the elements of a *maxRows* by
*maxCols* matrix. This is just an upper bound: your matrix may end up
using fewer elements than this, but it cannot use more.

This matrix will still not be initialised until you call either
initClone() or initIdentity(). See the class notes for details.

Precondition:
    This matrix was created using the default (no-argument)
    constructor, and you have not called any other routines on this
    matrix since.

.. warning::
    To elaborate on the precondition above: you can only call
    reserve() once, and if you did not use the default LPMatrix
    constructor then you cannot call it at all. Any additional calls
    to reserve() will result in a memory leak.

Parameter ``maxRows``:
    an upper bound on the number of rows that you will need for this
    matrix. This must be strictly positive.

Parameter ``maxCols``:
    an upper bound on the number of columns that you will need for
    this matrix. This must be strictly positive.)doc";

// Docstring regina::python::doc::LPMatrix_::rows
static const char *rows =
R"doc(Returns the number of rows in this matrix. This relates to the
currently assigned matrix size, not the total amount of memory that
was originally reserved.

Returns:
    the number of rows.)doc";

// Docstring regina::python::doc::LPMatrix_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given matrix.

It does not matter if the two matrices have different sizes, or if one
or both is unintialised; if so then these properties will be swapped
also.

Parameter ``other``:
    the matrix whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::LPMatrix_::swapRows
static const char *swapRows =
R"doc(Swaps the two given rows of this matrix. The two arguments *r1* and
*r2* may be equal (in which case the matrix will be left unchanged).

Parameter ``r1``:
    the index of the first row to swap. This must be between 0 and
    rows()-1 inclusive.

Parameter ``r2``:
    the index of the second row to swap. This must be between 0 and
    rows()-1 inclusive.)doc";

}

namespace LPSystem_ {

// Docstring regina::python::doc::LPSystem_::__copy
static const char *__copy = R"doc(Creates a new copy of the given class of vector encodings.)doc";

// Docstring regina::python::doc::LPSystem_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object represent the same class
of vector encodings.

Parameter ``other``:
    the object to compare with this.

Returns:
    ``True`` if and only if both objects represent the same class of
    encodings.)doc";

// Docstring regina::python::doc::LPSystem_::__init
static const char *__init =
R"doc(Identifies which class of vector encodings the given encoding falls
into.

Parameter ``enc``:
    a normal surface vector encoding; this may be any valid
    NormalEncoding object, including the special angle structure
    encoding.)doc";

// Docstring regina::python::doc::LPSystem_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object represent different
classes of vector encodings.

Parameter ``other``:
    the object to compare with this.

Returns:
    ``True`` if and only if both objects represent different classes
    of encodings.)doc";

// Docstring regina::python::doc::LPSystem_::angle
static const char *angle =
R"doc(Identifies whether this is the class of encodings that represent angle
structures.

Exactly one of normal() and angle() will return ``True``.

Returns:
    ``True`` if this is the class of angle encodings.)doc";

// Docstring regina::python::doc::LPSystem_::coords
static const char *coords =
R"doc(Returns the number of coordinate columns that a tableaux will use for
this class of vector encodings, with respect to a particular
triangulation.

Parameter ``nTet``:
    the number of tetrahedra in the triangulation.

Returns:
    the corresponding number of coordinate columns in the tableaux.)doc";

// Docstring regina::python::doc::LPSystem_::normal
static const char *normal =
R"doc(Identifies whether this is one of the two classes of encodings that
represent normal or almost normal surfaces.

This will be ``True`` if and only if either standard() or quad()
returns ``True``.

Exactly one of normal() and angle() will return ``True``.

Returns:
    ``True`` if this is a class of normal or almost normal surface
    encodings.)doc";

// Docstring regina::python::doc::LPSystem_::quad
static const char *quad =
R"doc(Identifies whether this is the class of quad encodings.

Exactly one of standard(), quad() and angle() will return ``True``.

Returns:
    ``True`` if this is the class of quad encodings.)doc";

// Docstring regina::python::doc::LPSystem_::standard
static const char *standard =
R"doc(Identifies whether this is the class of standard encodings.

Exactly one of standard(), quad() and angle() will return ``True``.

Returns:
    ``True`` if this is the class of standard encodings.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

