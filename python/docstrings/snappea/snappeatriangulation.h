/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Cusp
static const char *Cusp =
R"doc(Used to return information about a single cusp of a SnapPea
triangulation. See SnapPeaTriangulation::cusp() and
SnapPeaTriangulation::cusps() for further details.

Cusp objects essentially package together information about a cusp as
a standalone read-only object. Unlike Triangulation<3>::Vertex and
other skeletal objects, a Cusp object does not uniquely define a cusp
(if you need a unique identifier, use Cusp::vertex() for this). You
can make many copies of the same Cusp object, and each copy will
contain the same information and point to the same vertex of the
underlying triangulation.

Cusp objects should be considered temporary only; you should not hold
onto references or pointers to them. If you need to hold on to
information about a cusp, you can simply copy the Cusp object by value
(an operation that is both cheap and safe).

Cusp objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

// Docstring regina::python::doc::SnapPeaFatalError
static const char *SnapPeaFatalError =
R"doc(An exception that is thrown when the SnapPea kernel encounters a fatal
error.

This is one of two types of exception that can be thrown from deep
within the SnapPea kernel. SnapPeaFatalError is used for informative
messages (the description includes a function and filename), whereas
SnapPeaMemoryFull is used for out-of-memory errors (which use a stock
description so they can be created without allocating further
resources).

Details of the error can be accessed through the inherited member
function what().)doc";

// Docstring regina::python::doc::SnapPeaMemoryFull
static const char *SnapPeaMemoryFull =
R"doc(An exception that is thrown when the SnapPea kernel finds that all
available memory has been exhausted.

This is one of two types of exception that can be thrown from deep
within the SnapPea kernel. SnapPeaFatalError is used for informative
messages (the description includes a function and filename), whereas
SnapPeaMemoryFull is used for out-of-memory errors (which use a stock
description so they can be created without allocating further
resources).

Details of the error can be accessed through the member function
what().)doc";

// Docstring regina::python::doc::SnapPeaTriangulation
static const char *SnapPeaTriangulation =
R"doc(Offers direct access to the SnapPea kernel from within Regina. An
object of this class represents a 3-manifold triangulation, stored
directly in the SnapPea kernel using SnapPea's internal format.

Regarding interaction with the SnapPea kernel:

* This class acts as the **sole C/C++ conduit** between the Regina
  calculation engine and Regina's inbuilt version of the SnapPea
  kernel. Regina should not interact with the SnapPea kernel at the
  C/C++ level other than through this class.

* Regina can, however, interact with SnapPy at the Python level.
  Regina's version of the SnapPea kernel lives within a separate
  namespace (regina::snappea), and so there should be no conflicts
  between Regina's copy and SnapPy's copy of the SnapPea kernel (which
  may even be different versions). You can pass triangulations back
  and forth between Regina and SnapPy using strings that contain the
  contents of a SnapPea data file (see for instance
  SnapPeaTriangulation::snapPea()).

Regarding the inherited Triangulation<3> interface:

* You can happily query this object using both SnapPea functions (such
  as SnapPeaTriangulation::volume(), and others specific to this
  class) and Regina's native triangulation functions (such as
  Triangulation<3>::homology(), and others inherited from
  Triangulation<3>). This is because an object of this class stores
  _two_ representations of the triangulation (SnapPea's and Regina's),
  which are always kept in sync.

* However, you may **only edit this object using the SnapPea functions
  specific to this class** (such as
  SnapPeaTriangulation::randomise()). This is essentially because the
  synchronisation is one-way only (from SnapPea to Regina, but not in
  the other direction).

* Any attempt to edit this triangulation via the inherited
  Triangulation<3> interface (for instance, by calling
  Triangulation<3>::pachner()) will automatically cause this to become
  a **null triangulation**, with no tetrahedra and no SnapPea data at
  all.

* In particular, if you wish to assign one SnapPea triangulation to
  another, or swap the contents of two SnapPea triangulations, then
  you _must_ cast both objects to SnapPeaTriangulation before
  performing the operation. If either argument is presented as the
  parent class Triangulation<3> then the inherited operation on
  Triangulation<3> will be called instead, which will (as above)
  nullify one or both SnapPea triangulations respectively.

Null triangulations appear more generally when Regina is unable to
represent data in SnapPea's native format. You can test for a null
triangulation by calling isNull(). Null triangulations can occur for
several reasons, such as (but not limited to):

* attempting to build a SnapPea triangulation from a Regina
  triangulation that is invalid, has boundary faces, or has higher
  genus vertex links (none of which SnapPea can handle);

* attempting to read a broken SnapPea data file;

* attempting to change a SnapPea triangulation using the inherited
  Triangulation<3> interface (as discussed above);

* attempting to import a SnapPea triangulation that uses unsupported
  (e.g., non-integer or non-coprime) filling coefficients, as
  discussed below).

Regarding fillings: SnapPea can store and manipulate Dehn fillings on
cusps, and the SnapPeaTriangulation class respects these where it can
(but with restrictions on the possible filling coefficients; see
below). However, Regina's own Triangulation<3> class knows nothing
about fillings at all. Therefore:

* Routines inherited through the Triangulation<3> interface will
  ignore fillings completely (so, for instance, homology() will return
  the first homology of the unfilled manifold, even if SnapPea has
  designated fillings on the cusps).

* Routines that are defined here in the SnapPeaTriangulation class are
  generally aware of fillings (so, for instance, homologyFilled() will
  return the first homology of the filled manifold). See the
  individual notes for each member function for details on how it
  handles fillings.

* In particular, if you are testing two triangulations for equality:
  if either triangulation is presented as the parent class
  Triangulation<3> then you will be calling the inherited
  Triangulation<3> comparision operators, which _only_ compare the
  tetrahedron labelling and gluings. If you wish to compare cusps and
  fillings as well, you must ensure that both triangulations are cast
  to SnapPeaTriangulation (in which case the SnapPeaTriangulation
  comparison operators will be used).

For now, SnapPeaTriangulation only supports the following types of
filling coefficients: on orientable cusps the filling coefficients
must be coprime integers, and non non-orientable cusps the filling
coefficients must be the integers (±1, 0). Any attempt to import a
triangulation from a SnapPea file with filling coefficients outside
these requirements will result in a null triangulation (as discussed
above).

There are many places in the SnapPea kernel where SnapPea throws a
fatal error. These fatal errors are converted into exceptions (in
particular, SnapPeaFatalError and SnapPeaMemoryFull), which can be
caught and handled politely. You should assume, unless you have good
reason to believe otherwise (e.g., you have read and traced through
the SnapPea source code), that any member function of this class that
uses the SnapPea kernel could throw either of these exceptions.

Regina uses the variant of the SnapPea kernel that is shipped with
SnapPy (standard precision), as well as some additional code written
explicitly for SnapPy. The header regina-config.h includes a macro
SNAPPY_VERSION that gives the exact version of SnapPy that is bundled
into Regina, and you can query this at runtime by calling Regina's
function regina::versionSnapPy().

Since Regina 7.0, SnapPeaTriangulation is no longer a "packet type"
that can be inserted directly into the packet tree. Instead a
SnapPeaTriangulation is now a standalone mathematatical object, which
makes it slimmer and faster for ad-hoc use. The consequences of this
are:

* If you create your own SnapPeaTriangulation, it will not have any of
  the usual packet infrastructure. You cannot add it into the packet
  tree, and it will not support a label, tags, child/parent packets,
  and/or event listeners.

* To include a SnapPeaTriangulation in the packet tree, you must
  create a new PacketOf<SnapPeaTriangulation>. This _is_ a packet
  type, and supports labels, tags, child/parent packets, and event
  listeners. It derives from SnapPeaTriangulation, and so inherits the
  full SnapPeaTriangulation interface.

* If you are adding new functions to this class that edit the
  triangulation, you must still remember to create a ChangeEventSpan.
  This will ensure that, if the triangulation is being managed by a
  PacketOf<SnapPeaTriangulation>, then the appropriate packet change
  events will be fired. All other events (aside from
  packetToBeChanged() and packetWasChanged() are managed directly by
  the PacketOf<SnapPeaTriangulation> wrapper class.

Regarding the packet interface, there is currently a deficiency when
listening for change events on a PacketOf<SnapPeaTriangulation>:

* As described above, if you edit the triangulation using the
  inherited Triangulation<3> interface, this will nullify the SnapPea
  triangulation.

* As expected, this will fire a pair of change events. However, the
  packetToBeChanged() event will be fired too late: specifically, it
  will be fired _after_ the Triangulation<3> change is made but
  _before_ the SnapPea triangulation is nullifed. In particular, it
  will already be too late to take a copy of the original SnapPea
  triangulation.

* Since fixing this will require non-trivial re-engineering of the
  code, and since users should not be modifying SnapPea triangulations
  via the inherited Triangulation<3> interface, this deficiency is
  being left to stay for the time being.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

The SnapPea kernel was originally written by Jeff Weeks. SnapPy, where
this kernel is now maintained, is primarily developed by Marc Culler,
Nathan Dunfield and Matthias Goerner, with contributions from many
people. SnapPy and the corresponding SnapPea kernel are distributed
under the terms of the GNU General Public License, version 2 or any
later version, as published by the Free Software Foundation.

See http://snappy.computop.org/ for further information on SnapPea and
its successor SnapPy.

Exception ``SnapPeaFatalError``:
    The SnapPea kernel detected a fatal error from which it could not
    recover. This could be thrown by any member function that uses the
    SnapPea kernel.

Exception ``SnapPeaMemoryFull``:
    The SnapPea kernel ran out of memory. This could be thrown by any
    member function that uses the SnapPea kernel.)doc";

namespace Cusp_ {

// Docstring regina::python::doc::Cusp_::__copy
static const char *__copy = R"doc(Creates a new copy of the given cusp information.)doc";

// Docstring regina::python::doc::Cusp_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object hold the same cusp
information.

Two Cusp objects are considered equal if they refer to the same vertex
number of the underlying triangulation, and they have the same filling
coefficients. Note that the vertex _pointers_ do not need to be the
same (i.e., it is meaningful to compare cusps from different
triangulations).

Parameter ``other``:
    the cusp information to compare with this.

Returns:
    ``True`` if and only this and the given object hold the same cusp
    information.)doc";

// Docstring regina::python::doc::Cusp_::__ne
static const char *__ne =
R"doc(Determines if this and the given object do not hold the same cusp
information.

Two Cusp objects are considered equal if they refer to the same vertex
number of the underlying triangulation, and they have the same filling
coefficients. Note that the vertex _pointers_ do not need to be the
same (i.e., it is meaningful to compare cusps from different
triangulations).

Parameter ``other``:
    the cusp information to compare with this.

Returns:
    ``True`` if and only this and the given object do not hold the
    same cusp information.)doc";

// Docstring regina::python::doc::Cusp_::complete
static const char *complete =
R"doc(Returns whether this cusp is complete.

SnapPy:
    In SnapPy, this field corresponds to querying
    ``Manifold.cusp_info("is_complete")[cusp_number]``.

Returns:
    ``True`` if this cusp is complete, or ``False`` if it is filled.)doc";

// Docstring regina::python::doc::Cusp_::l
static const char *l =
R"doc(Returns the second (longitude) filling coefficient on this cusp, or 0
if this cusp is complete.

SnapPy:
    In SnapPy, this field corresponds to querying
    ``Manifold.cusp_info("filling")[cusp_number][1]``.

Returns:
    the second filling coefficient.)doc";

// Docstring regina::python::doc::Cusp_::m
static const char *m =
R"doc(Returns the first (meridian) filling coefficient on this cusp, or 0 if
this cusp is complete.

SnapPy:
    In SnapPy, this field corresponds to querying
    ``Manifold.cusp_info("filling")[cusp_number][0]``.

Returns:
    the first filling coefficient.)doc";

// Docstring regina::python::doc::Cusp_::vertex
static const char *vertex =
R"doc(Returns the corresponding vertex of the Regina triangulation (i.e., of
the Triangulation<3> structure that is inherited by
SnapPeaTriangulation).

Note that cusp and vertex indexing might not be in sync; that is,
SnapPea's ``cusp(i)`` need not correspond to Regina's ``vertex(i)``.

This routine can be used to detect if/when cusp numbering and vertex
numbering fall out of sync, and to translate between them if/when this
happens.)doc";

}

namespace SnapPeaFatalError_ {

// Docstring regina::python::doc::SnapPeaFatalError_::__copy
static const char *__copy = R"doc(Creates a new copy of the given exception.)doc";

// Docstring regina::python::doc::SnapPeaFatalError_::__init
static const char *__init =
R"doc(Creates a new exception, indicating where in the SnapPea kernel the
error occurred.

Parameter ``fromFunction``:
    the function from the SnapPea kernel in which the error occurred.

Parameter ``fromFile``:
    the source file from the SnapPea kernel in which the error
    occurred.)doc";

}

namespace SnapPeaMemoryFull_ {

// Docstring regina::python::doc::SnapPeaMemoryFull_::__copy
static const char *__copy = R"doc(Creates a new copy of the given exception.)doc";

// Docstring regina::python::doc::SnapPeaMemoryFull_::__default
static const char *__default = R"doc(Creates a new exception.)doc";

// Docstring regina::python::doc::SnapPeaMemoryFull_::what
static const char *what =
R"doc(Returns a human-readable description of the error that occurred.

Returns:
    a description of the error.)doc";

}

namespace SnapPeaTriangulation_ {

// Docstring regina::python::doc::SnapPeaTriangulation_::CoverEnumerationType
static const char *CoverEnumerationType =
R"doc(Indicates which types of covers should be enumerated when calling
enumerateCovers().

This enumeration is identical to SnapPea's PermutationSubgroup enum,
though the values here are named differently. The enumeration is
declared again here because Regina code should not in general interact
directly with the SnapPea kernel. Values may be freely converted
between the two enumeration types by simple assignment and/or
typecasting.

.. warning::
    This enumeration must always be kept in sync with SnapPea's
    PermutationSubgroup enumeration.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::CoverType
static const char *CoverType =
R"doc(Indicates the different types of covers that can be produced by
enumerateCovers().

This enumeration is identical to SnapPea's own CoveringType enum; it
is declared again here because Regina code should not in general
interact directly with the SnapPea kernel. Values may be freely
converted between the two enumeration types by simple assignment
and/or typecasting.

.. warning::
    This enumeration must always be kept in sync with SnapPea's own
    CoveringType enumeration.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::SolutionType
static const char *SolutionType =
R"doc(Describes the different types of solution that can be found when
solving for a hyperbolic structure.

This enumeration is identical to SnapPea's own SolutionType enum; it
is declared again here because Regina code should not in general
interact directly with the SnapPea kernel. Values may be freely
converted between the two enumeration types by simple assignment
and/or typecasting.

.. warning::
    This enumeration must always be kept in sync with SnapPea's own
    SolutionType enumeration.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given SnapPea triangulation.

If *src* is a null triangulation then this will be a null
triangulation also. See isNull() for further details.

Parameter ``src``:
    the SnapPea triangulation to copy.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::__default
static const char *__default = R"doc(Creates a null triangulation, with no internal SnapPea data at all.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::__eq
static const char *__eq =
R"doc(Determines whether this and the given SnapPea triangulation are, at
some elementary level, the same.

This routine checks only those things that can be checked exactly,
without going through the SnapPea kernel and without requiring
floating-point comparisons.

In particular, it _does_ check whether:

* the tetrahedron numbers, vertex labels and gluings are the same in
  both triangulations (i.e., the tests performed by Regina's native
  Triangulation<3> comparison operators);

* the same numbered cusps correspond to the same numbered vertices of
  the triangulation;

* each pair of corresponding cusps are either both not filled, or both
  filled using the same coefficients.

It does _not_ check wehether corresponding tetrahedron shapes are the
same, or if the volumes are "sufficiently close", or even whether the
SnapPea kernel has produced the same solution type for both
triangulations.

Two null SnapPea triangulations will be considered equal.

Parameter ``other``:
    the SnapPea triangulation to compare with this.

Returns:
    ``True`` if and only this and the given triangulation are the
    same, according to the criteria described above.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::__init
static const char *__init =
R"doc(Creates a new SnapPea triangulation from the contents of SnapPea data
file. The argument may be the _name_ of a SnapPea file, or it may also
be the _contents_ of a SnapPea file (so the file itself need not
actually exist on the filesystem).

This routine uses the SnapPea kernel to read the data file, and so all
SnapPea-specific information will be preserved (including information
that Regina itself does not store, such as peripheral curves).

If this operation fails (e.g., if the given string does not represent
a valid SnapPea data file), then this routine will thrown an
exception; see below for details.

If this routine returns (as opposed to throwing an exception), then it
is guaranteed that this is not a null SnapPea triangulation. In this
successful scenario, this constructor will ask SnapPea to try to find
a hyperbolic structure before it returns.

.. note::
    This constructor can be used in a Python session to pass data from
    SnapPy through to Regina's copy of the SnapPea kernel (which is
    strictly separate from SnapPy's), without losing any of SnapPy's
    internal information.

.. warning::
    If (for some reason) you pass a filename that begins with "%
    Triangulation", then Regina will interpret this as the contents of
    a SnapPea file (not a filename).

Internationalisation:
    If the given argument is a filename, then this routine makes no
    assumptions about the character encoding used in the filename, and
    simply passes it through unchanged to low-level C/C++ file I/O
    routines. This routine assumes that the file _contents_, however,
    are in UTF-8 (the standard encoding used throughout Regina).

Exception ``FileError``:
    The SnapPea kernel could not read the given file, or could not
    parse the file contents (which could have been passed explicitly
    or could have been read from file).

Parameter ``fileNameOrContents``:
    either the name of a SnapPea data file, or the contents of a
    SnapPea data file (which need not actually exist on the
    filesystem).)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::__init_2
static const char *__init_2 =
R"doc(Converts the given Regina triangulation to a SnapPea triangulation.
This copy will be independent (i.e., this triangulation will not be
affected if *tri* is later changed or destroyed).

Since Regina works with more general kinds of trianguations than
SnapPea, not all Regina triangulations can be represented in SnapPea
format. If the conversion is unsuccessful, this will be marked as a
null triangulation. You should always test isNull() to determine
whether the conversion was successful.

Regarding the conversion:

* If *tri* is of the subclass SnapPeaTriangulation, then this
  effectively acts as a copy constructor: all SnapPea-specific
  information will be cloned directly through the SnapPea kernel. If
  *tri* is a null SnapPea triangulation then this copy will be a null
  triangulation also.

* If *tri* is of the parent class Triangulation<3>, then Regina will
  attempt to convert this triangulation to SnapPea format. If the
  conversion is successful, this constructor will immediately ask
  SnapPea to try to find a hyperbolic structure.

Regarding peripheral curves: native Regina triangulations do not store
or use peripheral curves themselves, and so this constructor makes a
default choice during the conversion process. Specifically:

* If solution_type() is geometric_solution or nongeometric_solution,
  then on each torus cusp the meridian and longitude are chosen to be
  the (shortest, second shortest) basis, and their orientations follow
  the convention used by the *SnapPy* kernel. Be warned, however, that
  this choice might not be unique for some cusp shapes, and the
  resolution of such ambiguities might be machine-dependent.

* If solution_type() is something else (e.g., degenerate or flat), or
  if SnapPea throws a fatal error when attempting to install the
  (shortest, second shortest) basis as described above, then Regina
  will accept whatever basis SnapPea installs by default. Be warned
  that this default basis may change (and indeed has changed in the
  past) across different versions of the SnapPea kernel.

Regarding internal vertices (i.e., vertices whose links are spheres):
SnapPea is designed to work only with triangulations where every
vertex is ideal. As a result:

* You may pass a closed triangulation to this constructor, but SnapPea
  will automatically convert this into a filling of a cusped manifold,
  using an ideal triangulation.

* You may also pass a triangulation that uses both ideal and internal
  vertices. In this case, SnapPea will retriangulate the manifold so
  that it uses ideal vertices only.

Even if SnapPea does not retriangulate the manifold (for the reasons
described above), it is possible that the tetrahedron and vertex
numbers might be changed in the new SnapPea triangulation. In
particular, if the given Regina triangulation is orientable but not
oriented, then you should _expect_ these numbers to change.

Parameter ``tri``:
    the Regina triangulation to clone.

Parameter ``ignored``:
    a legacy parameter that is now ignored. (This argument was once
    required if you wanted to pass a closed triangluation to SnapPea.))doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::__init_3
static const char *__init_3 =
R"doc(Creates a new ideal SnapPea triangulation representing the complement
of the given link in the 3-sphere.

This is _not_ the same triangulation that would be produced by calling
``SnapPeaTriangulation(link.complement())``. By calling
``link.complement()``, you through Regina's Triangulation<3> class and
therefore lose the peripheral curves. Although the
SnapPeaTriangulation constructor will install new peripheral curves,
there is no guarantee that these are the same curves as before; in
particular, there is no guarantee that these new curves will
correspond in any way to the original link diagram.

In contrast, by calling ``SnapPeaTriangulation(link)`` directly, the
link complement will be triangulated by the SnapPea kernel and not by
Regina. As a result, the peripheral curves installed by SnapPea will
be precisely the curves from the link diagram.

Exception ``InvalidArgument``:
    The given link is empty, or it has so many crossings and/or
    components that SnapPea cannot handle it. (The latter problem will
    only occur if the number of crossings and/or components does not
    fit into a native C++ ``int``.)

Parameter ``link``:
    the link whose complement we should build.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::__ne
static const char *__ne =
R"doc(Determines whether this and the given SnapPea triangulation are, at
some elementary level, different.

This routine checks only those things that can be checked exactly,
without going through the SnapPea kernel and without requiring
floating-point comparisons.

In particular, it _does_ check whether:

* the tetrahedron numbers, vertex labels and gluings are the same in
  both triangulations (i.e., the tests performed by Regina's native
  Triangulation<3> comparison operators);

* the same numbered cusps correspond to the same numbered vertices of
  the triangulation;

* each pair of corresponding cusps are either both not filled, or both
  filled using the same coefficients.

It does _not_ check wehether corresponding tetrahedron shapes are the
same, or if the volumes are "sufficiently close", or even whether the
SnapPea kernel has produced the same solution type for both
triangulations.

Two null SnapPea triangulations will be considered equal.

Parameter ``other``:
    the SnapPea triangulation to compare with this.

Returns:
    ``True`` if and only this and the given triangulation are
    different, according to the criteria described above.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::canonise
static const char *canonise =
R"doc(Constructs the canonical retriangulation of the canonical cell
decomposition.

Any fillings on the cusps of this SnapPea triangulation will be
ignored. In the resulting canonical triangulation (which is one of
Regina's native Triangulation<3> objects, not a SnapPea
triangulation), these fillings will be completely forgotten.

The canonical cell decomposition is the one described in "Convex hulls
and isometries of cusped hyperbolic 3-manifolds", Jeffrey R. Weeks,
Topology Appl. 52 (1993), 127-149.

If the canonical cell decomposition is already a triangulation then we
leave it untouched. Otherwise, the canonical retriangulation
introduces internal (finite) vertices, and is defined as follows:

* within each 3-cell of the original complex we introduce a new
  internal vertex, and cone the 3-cell boundary to this new vertex;

* through each 2-cell of the original complex we insert the dual edge
  (joining the two new finite vertices on either side), and we replace
  the two cones on either side of the 2-cell with a ring of tetrahedra
  surrounding this dual edge.

See canonize_part_2.c in the SnapPea source code for details.

This routine discards the hyperbolic structure along with all SnapPea-
specific information (such as peripheral curves and fillings), and
simply returns one of Regina's native triangulations. If you need to
preserve SnapPea-specific information then you should call
protoCanonise() instead.

SnapPea is not always able to compute the canonical cell
decomposition: if it fails then then this routine will throw an
exception (see below for details).

SnapPy:
    The function ``canonise()`` means different things for SnapPy
    versus the SnapPea kernel. Here Regina follows the naming
    convention used in the SnapPea kernel. Specifically: Regina's
    routine SnapPeaTriangulation::protoCanonise() corresponds to
    SnapPy's ``Manifold.canonize()`` and the SnapPea kernel's
    ``proto_canonize(manifold)``. Regina's routine
    SnapPeaTriangulation::canonise() corresponds to the SnapPea
    kernel's ``canonize(manifold)``, and is not available through
    SnapPy at all.

.. warning::
    The SnapPea kernel does not always compute the canonical cell
    decomposition correctly. Sometimes it gives the wrong answer,
    although in such a case it still guarantees that the manifold it
    _does_ return is homeomorphic to the original. Sometimes it gives
    no answer at all, in which case this routine will throw an
    exception (see below).

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Exception ``UnsolvedCase``:
    The SnapPea kernel was unable to compute the canonical cell
    decomposition.

Returns:
    the canonical triangulation of the canonical cell decomposition.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::canonize
static const char *canonize =
R"doc(An alias for canonise(), which constructs the canonical
retriangulation of the canonical cell decomposition. See canonise()
for further details.

This alias is provided as "glue" between the British spelling used
throughout Regina and the American spelling used throughout the
SnapPea kernel.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Exception ``UnsolvedCase``:
    The SnapPea kernel was unable to compute the canonical cell
    decomposition.

Returns:
    the canonical triangulation of the canonical cell decomposition.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::countCompleteCusps
static const char *countCompleteCusps =
R"doc(Returns the total number of complete cusps (that is, unfilled cusps).

It is always true that ``countCompleteCusps() + countFilledCusps() ==
countCusps()``.

SnapPy:
    This has no corresponding routine in SnapPy, though the
    information is easily acessible via
    ``Manifold.cusp_info("is_complete")``.

Returns:
    the total number of complete cusps.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::countCusps
static const char *countCusps =
R"doc(Returns the total number of cusps (both filled and complete).

This returns the same value as the inherited function
Triangulation<3>::countBoundaryComponents().

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.num_cusps()``.

Returns:
    the total number of cusps.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::countFilledCusps
static const char *countFilledCusps =
R"doc(Returns the total number of filled cusps.

It is always true that ``countCompleteCusps() + countFilledCusps() ==
countCusps()``.

SnapPy:
    This has no corresponding routine in SnapPy, though the
    information is easily acessible via
    ``Manifold.cusp_info("is_complete")``.

Returns:
    the total number of filled cusps.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::cusp
static const char *cusp =
R"doc(Returns information about the given cusp of this manifold. This
information includes the filling coefficients (if any), along with
other combinatorial information.

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.cusp_info()[c]``, though the set of information
    returned about each cusp is different.

These Cusp objects should be considered temporary only; you should not
hold onto references or pointers to them. If you need to hold on to
information about a cusp, you can simply copy the Cusp object by value
(an operation that is both cheap and safe).

In older versions of Regina, this routine would explicitly check for a
null triangulation. Nowadays this is the responsibility of the user or
programmer.

.. warning::
    Be warned that cusp *i* might not correspond to vertex *i* of the
    triangulation. The Cusp::vertex() method (which is accessed
    through the cusp() routine) can help translate between SnapPea's
    cusp numbers and Regina's vertex numbers.

Parameter ``whichCusp``:
    the index of a cusp according to SnapPea; this must be between 0
    and countCusps()-1 inclusive.

Returns:
    information about the given cusp.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::cusps
static const char *cusps =
R"doc(Returns an object that allows iteration through and random access to
information about all of the cusps of this manifold. This information
includes the filling coefficients (if any), along with other
combinatorial information.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. The elements of the list will be read-only
objects of type Cusp. For example, your code might look like:

```
for (const Cusp& c : tri->cusps()) { ... }
```

These Cusp objects should be considered temporary only; you should not
hold onto references or pointers to them. If you need to hold on to
information about a cusp, you can simply copy the Cusp object by value
(an operation that is both cheap and safe).

.. warning::
    Be warned that cusp *i* might not correspond to vertex *i* of the
    triangulation. The Cusp::vertex() method (which is accessed
    through the cusp() routine) can help translate between SnapPea's
    cusp numbers and Regina's vertex numbers.

Returns:
    access to the list of all cusps of this manifold.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::disableKernelMessages
static const char *disableKernelMessages =
R"doc(Specifies that the SnapPea kernel should not write diagnostic messages
to standard output.

Calling this routine is equivalent to calling
enableKernelMessages(false).

Note that diagnostic messages are already disabled by default.

This routine (which interacts with static data) is thread-safe.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::enableKernelMessages
static const char *enableKernelMessages =
R"doc(Configures whether or not the SnapPea kernel should write diagnostic
messages to standard output.

By default such diagnostic messages are disabled.

This routine (which interacts with static data) is thread-safe.

Parameter ``enabled``:
    ``True`` if diagnostic messages should be enabled, or ``False``
    otherwise.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::enumerateCovers
static const char *enumerateCovers =
R"doc(Enumerates connected *k*-sheeted covers of the underlying manifold.
The number of sheets *k* is passed as the first argument to this
function.

Regina does this (with the help of SnapPea) by:

* enumerating all transitive representations of the fundamental group
  into either the symmetric group *S(k)* or the cyclic group *Z_k*
  (according to the parameter *type*), using the SnapPea function
  ``find_representations()``; and then

* building the cover that corresponds to each representation, using
  the SnapPea function ``construct_cover()``.

If you are only interested in the corresponding index *k* subgroups of
the fundamental group and not the triangulated covers themselves, then
you may wish to consider the native Regina function
GroupPresentation::enumerateCovers() instead. That function is highly
optimised, and should be considerably faster as *k* grows.

Each covering space is produced once up to equivalence; here
equivalent covers correspond to conjugate representations of the
fundamental group.

To enumerate _all_ *k*-sheeted covers up to equivalence, set *type* to
*all_covers*. Be warned, however, that this becomes enormously slow as
the number of sheets *k* grows. An alternative is to enumerate only
cyclic covers by setting *type* to *cyclic_covers*; this significantly
limits the set of covers produced but is also much faster for larger
*k*.

For each cover that is produced, this routine will call *action*
(which must be a function or some other callable object).

* The first argument to *action* must be a SnapPea triangulation; this
  will be the newly produced cover. This argument will be passed as an
  rvalue; a typical action could (for example) take it by const
  reference and query it, or take it by value and modify it, or take
  it by rvalue reference and move it into more permanent storage.

* The second argument to *action* must be of type
  *SnapPeaTriangulation::CoverType*. This will indicate the type of
  cover that has been constructed; see the
  SnapPeaTriangulation::CoverType documentation for details. In the
  same call to enumerateCovers() you may observe different types of
  covers being produced (i.e., this value is computed individually for
  each cover). You should, however, never see the value
  *unknown_cover*.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return ``void``.

* *action* must not make changes to this original triangulation (i.e.,
  the SnapPeaTriangulation upon which enumerateCovers() is being
  called).

Be aware that this routine does the bulk of its work before any covers
are produced. Specifically, because of the design of the SnapPea
kernel, this routine first enumerates all requested representations of
the fundamental group (which is the slow part), and only then does it
construct the cover and call *action* for each representation, one at
a time.

Precondition:
    This is not a null triangulation.

.. warning::
    If you are enumerating all covers, then the argument *sheets*
    should be _very_ small.

.. warning::
    The covers that are produced will typically use far more
    tetrahedra than necessary. If size is important then you should
    take a copy using Regina's native type Triangulation<3> and call
    intelligentSimplify() on that. Note that you cannot call
    intelligentSimplify() directly on a SnapPeaTriangulation, since
    (like all modifying functions inherited from Triangulation<3>)
    this will nullify the SnapPeaTriangulation. See the class notes
    for further explanation.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.

Python:
    There are two versions of this function available in Python. The
    first form is ``enumerateCovers(sheets, type, action)``, which
    mirrors the C++ function: it takes *action* which may be a pure
    Python function, it returns the number of covers found, but it
    does _not_ take an addition argument list (*args*). The second
    form is ``enumerateCovers(sheets, type)``, which returns a Python
    list containing all of the triangulated covers, each given as a
    pair (SnapPeaTriangulation, SnapPeaTriangulation::CoverType).

Parameter ``sheets``:
    the number of sheets in the covers to produce (i.e., the number
    *k* in the description above); this must be a positive integer.

Parameter ``type``:
    indicates whether to enumerate all covers (up to equivalence) or
    only cyclic covers.

Parameter ``action``:
    a function (or other callable object) to call for each cover that
    is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial triangulation and type arguments.

Returns:
    the total number of covers found.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::fill
static const char *fill =
R"doc(Assigns a Dehn filling to the given cusp. This routine will
automatically ask SnapPea to update the hyperbolic structure according
to the new filling coefficients.

The triangulation itself will not change; this routine will simply ask
SnapPea to store the given filling coefficients alongside the cusp, to
be used in operations such as computing hyperbolic structures. If you
wish to retriangulate to permanently fill the cusp, call filledAll()
or filledPartial() instead.

For orientable cusps only coprime filling coefficients are allowed,
and for non-orientable cusps only (±1, 0) fillings are allowed.
Although SnapPea can handle more general fillings, Regina will enforce
these conditions; if they are not satisfied then it will do nothing
and simply return ``False``.

As a special case however, you may pass (0, 0) as the filling
coefficients, in which case this routine will behave identically to
unfill().

It is possible that, if the given integers are extremely large,
SnapPea cannot convert the filling coefficients to its own internal
floating-point representation. If this happens then this routine will
again do nothing and simply return ``False``.

.. warning::
    Be warned that cusp *i* might not correspond to vertex *i* of the
    triangulation. The Cusp::vertex() method (which is accessed
    through the cusp() routine) can help translate between SnapPea's
    cusp numbers and Regina's vertex numbers.

Parameter ``m``:
    the first (meridional) filling coefficient.

Parameter ``l``:
    the second (longitudinal) filling coefficient.

Parameter ``whichCusp``:
    the index of the cusp to fill according to SnapPea; this must be
    between 0 and countCusps()-1 inclusive.

Returns:
    ``True`` if and only if the filling coefficients were accepted
    (according to the conditions outlined above).)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::filledAll
static const char *filledAll =
R"doc(Retriangulates to permanently fill all cusps. This uses the current
Dehn filling coefficients on the cusps, as set by fill(). The result
will be a closed triangulation, of type Triangulation<3> (not
SnapPeaTriangulation).

This routine requires every cusp to be non-complete. If one or more
cusps is complete (i.e., has no filling coefficients assigned), then
this routine will throw an exception.

This replaces the old filledTriangulation() routines from Regina 6.0.1
and earlier, which decided at runtime whether to return a
Triangulation<3> or a SnapPeaTriangulation. Since this routine
explicitly fills all cusps, it is able to guarantee an explicit return
type of Triangulation<3> at compile time.

Precondition:
    All cusps of this manifold are non-complete (i.e., have filling
    coefficients assigned). This will be checked, and an exception
    will be thrown if this requirement is not met.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Exception ``FailedPrecondition``:
    Some cusp of this manifold is complete.

Returns:
    the filled triangulation.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::filledPartial
static const char *filledPartial =
R"doc(Retriangulates to permanently fill the given cusp. This uses the
current Dehn filling coefficients on the cusp, as set by fill(). The
result will be another instance of SnapPeaTriangulation, with at least
one cusp remaining.

This routine requires the manifold to have at least two cusps (i.e.,
the cusp being filled, plus at least one other). This is because most
of the SnapPea kernel routines require its triangulations to have at
least one cusp.

The error conditions are as follows:

* If the given cusp has no filling coefficients assigned (i.e., it is
  complete), then it cannot be filled: instead this routine will throw
  an exception.

* If the manifold has only one cusp, then this routine will likewise
  throw an exception. For such scenarios you should call filledAll()
  instead, which will return a closed Regina triangulation.

* Otherwise, if there is at least one other cusp, the result will be a
  SnapPeaTriangulation with one fewer cusp. The remaining cusps might
  be reindexed, and all Cusp structures will be destroyed and rebuilt.
  Auxiliary information on the remaining cusps (such as peripheral
  curves) will be preserved, and SnapPea will automatically attempt to
  compute a hyperbolic structure on the new triangulation.

This replaces the old filledTriangulation() routines from Regina 6.0.1
and earlier, which decided at runtime whether to return a
Triangulation<3> or a SnapPeaTriangulation. Since this routine
explicitly does not fill all cusps, it is able to guarantee an
explicit return type of SnapPeaTriangulation at compile time.

Precondition:
    The given cusp is non-complete (i.e., has filling coefficients
    assigned), and the manifold has at least one other cusp. These
    preconditions will be checked, and an exception will be thrown if
    they are not met.

.. warning::
    Be warned that cusp *i* might not correspond to vertex *i* of the
    triangulation. The Cusp::vertex() method (which is accessed
    through the cusp() routine) can help translate between SnapPea's
    cusp numbers and Regina's vertex numbers.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Exception ``FailedPrecondition``:
    The given cusp is complete, and/or it is the only cusp.

Parameter ``whichCusp``:
    the index of the cusp to permanently fill according to SnapPea;
    this must be between 0 and countCusps()-1 inclusive.

Returns:
    the filled triangulation.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::filledPartial_2
static const char *filledPartial_2 =
R"doc(Retriangulates to permanently fill some but not all cusps. This uses
the current Dehn filling coefficients on the cusps, as set by fill().
The result will be another instance of SnapPeaTriangulation, with at
least one cusp remaining.

This routine requires at least one cusp to be complete (i.e., to have
no filling coefficients assigned), since most of the SnapPea kernel
requires its triangulations to have at least one cusp.

* If all cusps are complete, then the result will simply be a clone of
  this triangulation.

* If some but not all cusps are complete, then the result will be a
  SnapPeaTriangulation with fewer cusps. The remaining cusps might be
  reindexed, and all Cusp structures will be destroyed and rebuilt.
  Auxiliary information on the remaining cusps (such as peripheral
  curves) will be preserved, and SnapPea will automatically attempt to
  compute a hyperbolic structure on the new triangulation.

* If no cusps are complete (i.e., all cusps have filling
  coefficients), then this routine will throw an exception. For such
  scenarios you should call filledAll() instead.

This replaces the old filledTriangulation() routines from Regina 6.0.1
and earlier, which decided at runtime whether to return a
Triangulation<3> or a SnapPeaTriangulation. Since this routine
explicitly does not fill all cusps, it is able to guarantee an
explicit return type of SnapPeaTriangulation at compile time.

Precondition:
    At least one cusp of this manifold is complete (i.e., has no
    filling coefficients assigned). This will be checked, and an
    exception will be thrown if this requirement is not met.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Exception ``FailedPrecondition``:
    All cusps of this manifold are non-complete.

Returns:
    the filled triangulation.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::fundamentalGroupFilled
static const char *fundamentalGroupFilled =
R"doc(Returns the fundamental group of the manifold with respect to the
current Dehn filling (if any). Any complete cusps (without fillings)
will be treated as though they had been truncated.

This is different from the inherited group() routine (and its alias
fundamentalGroup()) from the parent Triangulation<3> class:

* This routine fundamentalGroupFilled() respects Dehn fillings, and
  directly uses SnapPea's code to compute fundamental groups.

* The inherited group() routine uses only Regina's code, and works
  purely within Regina's parent Triangulation<3> class. Since
  Triangulation<3> knows nothing about SnapPea or fillings, this means
  that any fillings on the cusps (which are specific to SnapPea
  triangulations) will be ignored.

Note that each time the triangulation changes, the fundamental group
will be deleted. Thus the reference that is returned from this routine
should not be kept for later use. Instead, fundamentalGroupFilled()
should be called again; this will be instantaneous if the group has
already been calculated.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Parameter ``simplifyPresentation``:
    ``True`` if SnapPea should attempt to simplify the group
    presentation, or ``False`` if it should be left unsimplified. Even
    if *simplifyPresentation* is ``False``, this routine will always
    eliminate adjacent (x, x^-1) pairs.

Parameter ``fillingsMayAffectGenerators``:
    ``True`` if SnapPea's choice of generators is allowed to depend on
    the Dehn fillings, or ``False`` if the choice of generators should
    be consistent across different fillings.

Parameter ``minimiseNumberOfGenerators``:
    ``True`` if SnapPea's group simplification code should try to
    reduce the number of generators at the expense of increasing the
    total length of the relations, or ``False`` if it should do the
    opposite.

Parameter ``tryHardToShortenRelators``:
    ``True`` if SnapPea's group simplification code should try to
    reduce the length of the relations by inserting one relation into
    another. In general this is a good thing, but it can be very
    costly for large presentations.

Returns:
    the fundamental group of the filled manifold.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given SnapPea triangulations.

This global routine simply calls SnapPeaTriangulation::swap(); it is
provided so that SnapPeaTriangulation meets the C++ Swappable
requirements.

See SnapPeaTriangulation::swap() for more details and caveats.

.. warning::
    If you wish to swap the contents of two SnapPea triangulations,
    you _must_ cast both to SnapPeaTriangulation before calling
    swap(). If either argument is presented as the parent class
    Triangulation<3>, then the Triangulation<3> swap() will be called
    instead; the result will be that (just like when you call any of
    the Triangulation<3> edit routines) both SnapPea triangulations
    will be reset to null triangulations. See the SnapPeaTriangulation
    class notes for further discussion.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events which may in turn call arbitrary code via any
    registered packet listeners.

Parameter ``lhs``:
    the triangulation whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the triangulation whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::gluingEquations
static const char *gluingEquations =
R"doc(Returns a matrix describing Thurston's gluing equations. This will be
with respect to the current Dehn filling (if any).

Each row of this matrix will describe a single equation. The first
countEdges() rows will list the edge equations, and the following 2 *
countCompleteCusps() + countFilledCusps() rows will list the cusp
equations.

The edge equations will be ordered arbitrarily. The cusp equations
will be presented in pairs ordered by cusp index (as stored by
SnapPea); within each pair the meridian equation will appear before
the longitude equation. The Cusp::vertex() method (which is accessed
through the cusp() routine) can help translate between SnapPea's cusp
numbers and Regina's vertex numbers.

The matrix will contain ``3 * size()`` columns. The first three
columns represent shape parameters ``z``, ``1/(1-z)`` and ``(z-1)/z``
for the first tetrahedron; the next three columns represent shape
parameters ``z``, ``1/(1-z)`` and ``(z-1)/z`` for the second
tetrahedron, and so on. By Regina's edge numbering conventions, ``z``
corresponds to edges 0 and 5 of the tetrahedron, ``1/(1-z)``
corresponds to edges 1 and 4 of the tetrahedron, and ``(z-1)/z``
corresponds to edges 2 and 3 of the tetrahedron.

More specifically, a row of the form ``a b c d e f ...`` describes an
equation with left hand side ``a log(z0) + b log(1/(1-z0)) + c
log((z0-1)/z) + d log(z1) + ...``, and with right hand side ``2πi``
for an edge equation or 0 for a cusp equation.

See also gluingEquationsRect(), which returns the gluing equations in
a more streamlined form.

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.gluing_equations()``.

Precondition:
    This is not a null triangulation.

Returns:
    a matrix with (*number_of_rows* + *number_of_cusps*) rows and (3 *
    *number_of_tetrahedra*) columns as described above.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::gluingEquationsRect
static const char *gluingEquationsRect =
R"doc(Returns a matrix describing Thurston's gluing equations in a
streamlined form. This will be with respect to the current Dehn
filling (if any).

Each row of this matrix will describe a single equation. The rows
begin with the edge equations (in arbitrary order) followed by the
cusp equations (ordered by cusp index); for precise details see the
documentation for gluingEquations(), which uses the same ordering.

The matrix will contain ``2 * size() + 1`` columns. Let *k* =
size()-1, and suppose the shape parameters for tetrahedra 0, 1, ..., k
are *z0*, *z1*, ..., *zk* (here each shape parameter corresponds to
edges 0 and 5 of the corresponding tetrahedron). Then a row of the
form ``a0 a1 ... ak b0 b1 ... bk c`` describes the equation ``z0^a0
z1^a1 ... zk^ak (1-z0)^b0 (1-z1)^b1 ... (1-zk)^bk = c``, where *c*
will always be 1 or -1.

See also gluingEquations(), which returns the gluing equations in a
more transparent term-by-term form.

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.gluing_equations(form="rect")``.

Precondition:
    This is not a null triangulation.

Returns:
    a matrix with (*number_of_rows* + *number_of_cusps*) rows and (2 *
    *number_of_tetrahedra* + 1) columns as described above.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::homologyFilled
static const char *homologyFilled =
R"doc(Returns the first homology group of the manifold with respect to the
current Dehn filling (if any). Any complete cusps (without fillings)
will be treated as though they had been truncated.

This is different from the inherited homology() routine from the
parent Triangulation<3> class:

* This routine homologyFilled() respects Dehn fillings, and uses a
  combination of both SnapPea's and Regina's code to compute homology
  groups. There may be situations in which the SnapPea kernel cannot
  perform its part of the computation (see below), in which case this
  routine will throw a SnapPeaUnsolvedCase exception.

* The inherited homology() routine uses only Regina's code, and works
  purely within Regina's parent Triangulation<3> class. Since
  Triangulation<3> knows nothing about SnapPea or fillings, this means
  that any fillings on the cusps (which are specific to SnapPea
  triangulations) will be ignored. The homology() routine will always
  return a solution.

This routine uses exact arithmetic, and so you are guaranteed that -
if it returns a result at all - that this result does not suffer from
integer overflows. Essentially, the process is this:

* SnapPea constructs a filled relation matrix using machine integer
  arithmetic, but detects overflow (in which case this routine will
  throw an exception);

* Regina then uses exact integer arithmetic to solve for the abelian
  group invariants (i.e., Smith normal form).

Note that each time the triangulation changes, the homology group will
be deleted. Thus the reference that is returned from this routine
should not be kept for later use. Instead, homologyFilled() should be
called again; this will be instantaneous if the group has already been
calculated.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Exception ``SnapPeaUnsolvedCase``:
    SnapPea detected an overflow when attempting to create the filled
    relation matrix.

Returns:
    the first homology group of the filled manifold.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::isNull
static const char *isNull =
R"doc(Determines whether this triangulation contains valid SnapPea data.

A null SnapPea triangulation can occur (for instance) when converting
unusual types of Regina triangulation into SnapPea format, or when
reading broken SnapPea data files. See the SnapPeaTriangulation class
notes for details.

Returns:
    ``True`` if this is a null triangulation, or ``False`` if this
    triangulation contains valid SnapPea data.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::kernelMessagesEnabled
static const char *kernelMessagesEnabled =
R"doc(Returns whether or not the SnapPea kernel writes diagnostic messages
to standard output.

By default such diagnostic messages are disabled. To enable them, call
enableKernelMessages().

This routine (which interacts with static data) is thread-safe.

Returns:
    ``True`` if and only if diagonstic messages are enabled.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::minImaginaryShape
static const char *minImaginaryShape =
R"doc(Returns the minimum imaginary part found amongst all tetrahedron
shapes, with respect to the Dehn filled hyperbolic structure.

Tetrahedron shapes are given in rectangular form using a fixed
coordinate system, as described in the documentation for shape().

If this is a null triangulation, or if solutionType() is no_solution
or not_attempted (i.e., we did not or could not solve for a hyperbolic
structure), then this routine will simply return zero.

SnapPy:
    This has no corresponding routine in SnapPy, though the
    information is easily acessible via
    ``Manifold.tetrahedra_shapes(part="rect")``.

Returns:
    the minimum imaginary part amongst all tetrahedron shapes.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::name
static const char *name =
R"doc(Returns SnapPea's internal name for this triangulation.

This is the manifold name stored in the SnapPea kernel, which is
typically different from the packet label assigned by Regina.

If this is a null triangulation then the empty string will be
returned.

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.name()``.

Returns:
    SnapPea's name for this triangulation.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::nullify
static const char *nullify = R"doc(Sets this to be a null SnapPea triangulation.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::protoCanonise
static const char *protoCanonise =
R"doc(Constructs the canonical cell decomposition, using an arbitrary
retriangulation if this decomposition contains non-tetrahedron cells.

Any fillings on the cusps of this SnapPea triangulation will be
ignored for the purposes of canonisation, though they will be copied
over to the new SnapPea triangulation that is returned.

The canonical cell decomposition is the one described in "Convex hulls
and isometries of cusped hyperbolic 3-manifolds", Jeffrey R. Weeks,
Topology Appl. 52 (1993), 127-149.

If the canonical cell decomposition is already a triangulation then we
leave it untouched, and otherwise we triangulate it arbitrarily.
Either way, we preserve the hyperbolic structure.

If you need a canonical triangulation (as opposed to an arbitrary
retriangulation), then you should call canonise() instead.

SnapPea is not always able to triangulate the canonical cell
decomposition: if it fails then then this routine will throw an
exception (see below for details).

SnapPy:
    The function ``canonise()`` means different things for SnapPy
    versus the SnapPea kernel. Here Regina follows the naming
    convention used in the SnapPea kernel. Specifically: Regina's
    routine SnapPeaTriangulation::protoCanonise() corresponds to
    SnapPy's ``Manifold.canonize()`` and the SnapPea kernel's
    ``proto_canonize(manifold)``. Regina's routine
    SnapPeaTriangulation::canonise() corresponds to the SnapPea
    kernel's ``canonize(manifold)``, and is not available through
    SnapPy at all.

.. warning::
    The SnapPea kernel does not always compute the canonical cell
    decomposition correctly. Sometimes it gives the wrong answer,
    although in such a case it still guarantees that the manifold it
    _does_ return is homeomorphic to the original. Sometimes it gives
    no answer at all, in which case this routine will throw an
    exception (see below).

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Exception ``UnsolvedCase``:
    The SnapPea kernel was unable to triangulate the canonical cell
    decomposition.

Returns:
    a triangulation of the canonical cell decomposition.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::protoCanonize
static const char *protoCanonize =
R"doc(An alias for protoCanonise(), which constructs the canonical cell
decomposition using an arbitrary retriangulation if necessary. See
protoCanonise() for further details.

This alias is provided as "glue" between the British spelling used
throughout Regina and the American spelling used throughout the
SnapPea kernel.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Exception ``UnsolvedCase``:
    The SnapPea kernel was unable to triangulate the canonical cell
    decomposition.

Returns:
    a triangulation of the canonical cell decomposition.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::randomise
static const char *randomise =
R"doc(Asks SnapPea to randomly retriangulate this manifold, using local
moves that preserve the topology. This can help when SnapPea is having
difficulty finding a hyperbolic structure.

This routine uses SnapPea's own internal retriangulation code.

After randomising, this routine will immediately ask SnapPea to try to
find a hyperbolic structure.

If this is a null SnapPea triangulation, this routine does nothing.

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.randomize()``.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::randomize
static const char *randomize =
R"doc(An alias for randomise(), which asks SnapPea to randomly retriangulate
this manifold. See randomise() for further details.

This alias is provided as "glue" between the British spelling used
throughout Regina and the American spelling used throughout the
SnapPea kernel.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::saveSnapPea
static const char *saveSnapPea =
R"doc(Writes this triangulation to the given file using SnapPea's native
file format.

Unlike Triangulation<3>::saveSnapPea(), this routine uses the SnapPea
kernel to produce the file contents. This means it will include not
just the tetrahedron gluings, but also other SnapPea-specific
information that Regina does not use (e.g., peripheral curves).

If this is a null triangulation, then the file will not be written and
this routine will return ``False``.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file _name_, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. The _contents_ of
    the file will be written using UTF-8.

Parameter ``filename``:
    the name of the SnapPea file to which to write.

Returns:
    ``True`` if and only if the file was successfully written.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::shape
static const char *shape =
R"doc(Returns the shape of the given tetrahedron, with respect to the Dehn
filled hyperbolic structure.

Tetrahedron shapes are given in rectangular form, and using a fixed
coordinate system (fixed alignment, in SnapPea's terminology).

If this is a null triangulation, or if solutionType() is no_solution
or not_attempted (i.e., we did not or could not solve for a hyperbolic
structure), then this routine will simply return zero.

This routine is fast constant time (unlike in SnapPea, where the
corresponding routine *get_tet_shape* takes linear time). Therefore
you can happily call this routine repeatedly without a significant
performance penalty.

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.tetrahedra_shapes(part="rect")[tet]``.

Parameter ``tet``:
    the index of a tetrahedron; this must be between 0 and size()-1
    inclusive.

Returns:
    the shape of the given tetrahedron, in rectangular form.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::slopeEquations
static const char *slopeEquations =
R"doc(Returns a matrix for computing boundary slopes of spun-normal surfaces
at the cusps of the triangulation. This matrix includes a pair of rows
for each cusp in the triangulation: one row for determining the
algebraic intersection number with the meridian, followed by one row
for determining the algebraic intersection number with the longitude.

If the triangulation has more than one cusp, these pairs are ordered
by cusp index (as stored by SnapPea). You can examine
``cusp(cusp_number).vertex()`` to map these to Regina's vertex indices
if needed.

For the purposes of this routine, any fillings on the cusps of this
SnapPea triangulation will be ignored.

This matrix is constructed so that, if *M* and *L* are the rows for
the meridian and longitude at some cusp, then for any spun-normal
surface with quadrilateral coordinates *q*, the boundary curves have
algebraic intersection number *M.q* with the meridian and *L.q* with
the longitude. Equivalently, the boundary curves pass *L.q* times
around the meridian and *-M.q* times around the longitude. To compute
these slopes directly from a normal surface, see
NormalSurface::boundaryIntersections().

The orientations of the boundary curves of a spun-normal surface are
chosen so that _if_ meridian and longitude are a positive basis as
vieved from the cusp, then as one travels along an oriented boundary
curve, the spun-normal surface spirals into the cusp to one's right
and down into the manifold to one's left.

SnapPy:
    This has no corresponding routine in SnapPy.

Precondition:
    All vertex links in this triangulation must be tori.

.. warning::
    If this triangulation was constructed from a Regina triangulation
    (of class Triangulation<3>), then Regina will have no information
    about what meridian and longitude the user wishes to use (since
    Regina does not keep track of peripheral curves on cusps).
    Therefore Regina will give boundary slopes relative to the
    (shortest, second-shortest) basis, as described in the constructor
    SnapPeaTriangulation(const Triangulation<3>&, bool). This might
    not be what the user expects.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Author:
    William Pettersson and Stephan Tillmann

Returns:
    a matrix with (2 * *number_of_cusps*) rows and (3 *
    *number_of_tetrahedra*) columns as described above.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::snapPea
static const char *snapPea =
R"doc(Returns a string containing the full contents of a SnapPea data file
that describes this triangulation. In particular, this string can be
used in a Python session to pass the triangulation directly to SnapPy
(without writing to the filesystem).

Unlike Triangulation<3>::snapPea(), this routine uses the SnapPea
kernel to produce the file contents. This means it will include not
just the tetrahedron gluings, but also other SnapPea-specific
information that Regina does not use (e.g., peripheral curves).

If you wish to export a triangulation to a SnapPea _file_, you should
call saveSnapPea() instead (which has better performance, and does not
require you to construct an enormous intermediate string).

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Returns:
    a string containing the contents of the corresponding SnapPea data
    file.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::solutionType
static const char *solutionType =
R"doc(Returns the type of solution found when solving for a hyperbolic
structure, with respect to the current Dehn filling (if any).

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.solution_type()``.

Returns:
    the solution type.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given SnapPea triangulation.

All information contained in this triangulation, including both
Regina's and SnapPea's internal data as well as all cached properties,
will be moved to *other*. Likewise, all information contained in
*other* will be moved to this triangulation.

In particular, any pointers or references to Tetrahedron<3>, Face<3,
subdim> and/or Cusp objects will remain valid.

This routine will behave correctly if *other* is in fact this
triangulation.

.. warning::
    If you wish to swap the contents of two SnapPea triangulations,
    you _must_ cast both to SnapPeaTriangulation before calling
    swap(). If either argument is presented as the parent class
    Triangulation<3>, then the Triangulation<3> swap() will be called
    instead; the result will be that (just like when you call any of
    the Triangulation<3> edit routines) both SnapPea triangulations
    will be reset to null triangulations. See the SnapPeaTriangulation
    class notes for further discussion.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both triangulations which may in turn call
    arbitrary code via any registered packet listeners.

Parameter ``other``:
    the SnapPea triangulation whose contents should be swapped with
    this.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::unfill
static const char *unfill =
R"doc(Removes any filling on the given cusp. After removing the filling,
this routine will automatically ask SnapPea to update the hyperbolic
structure.

If the given cusp is already complete, then this routine safely does
nothing.

.. warning::
    Be warned that cusp *i* might not correspond to vertex *i* of the
    triangulation. The Cusp::vertex() method (which is accessed
    through the cusp() routine) can help translate between SnapPea's
    cusp numbers and Regina's vertex numbers.

Parameter ``whichCusp``:
    the index of the cusp to unfill according to SnapPea; this must be
    between 0 and countCusps()-1 inclusive.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::volume
static const char *volume =
R"doc(Computes the volume of the current solution to the hyperbolic gluing
equations. This will be with respect to the current Dehn filling (if
any).

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.volume()``.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Returns:
    the estimated volume of the underlying 3-manifold.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::volumeWithPrecision
static const char *volumeWithPrecision =
R"doc(Computes the volume of the current solution to the hyperbolic gluing
equations, and estimates the accuracy of the answer. This will be with
respect to the current Dehn filling (if any).

SnapPy:
    In SnapPy, this routine corresponds to calling
    ``Manifold.volume(accuracy=True)``.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Returns:
    a pair whose first element is the estimated volume of the
    underlying 3-manifold, and whose second element is an estimate of
    the number of decimal places of accuracy in this volume.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::volumeZero
static const char *volumeZero =
R"doc(Determines whether the current solution to the gluing equations has
volume approximately zero. This test is _not_ rigorous.

This requires (i) the volume itself to be very close to zero in an
absolute sense, (ii) the volume to be zero within SnapPea's own
estimated precision, and (iii) SnapPea's estimated precision to be
sufficiently good in an absolute sense.

Returns:
    ``True`` if and only if the volume of the current solution is
    approximately zero according to the constraints outlined above.)doc";

}

namespace SnapPeaTriangulation_::CoverEnumerationType_ {

// Docstring regina::python::doc::SnapPeaTriangulation_::CoverEnumerationType_::all_covers
static const char *all_covers =
R"doc(Indicates that all covers should be enumerated. This corresponds to
the SnapPea constant *permutation_subgroup_Sn*.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::CoverEnumerationType_::cyclic_covers
static const char *cyclic_covers =
R"doc(Indicates that only cyclic covers should be enumerated. This
corresponds to the SnapPea constant *permutation_subgroup_Zn*.)doc";

}

namespace SnapPeaTriangulation_::CoverType_ {

// Docstring regina::python::doc::SnapPeaTriangulation_::CoverType_::cyclic_cover
static const char *cyclic_cover = R"doc(Indicates a cyclic covering.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::CoverType_::irregular_cover
static const char *irregular_cover =
R"doc(Indicates a covering where there exist two lifts of a point in the
base manifold with no covering transformation that takes one to the
other.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::CoverType_::regular_cover
static const char *regular_cover =
R"doc(Indicates a covering that is not cyclic, and where for any two lifts
of a point in the base manfiold, there is a covering transformation
taking one to the other.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::CoverType_::unknown_cover
static const char *unknown_cover = R"doc(Indicates that SnapPea has not yet computed the covering type.)doc";

}

namespace SnapPeaTriangulation_::SolutionType_ {

// Docstring regina::python::doc::SnapPeaTriangulation_::SolutionType_::degenerate_solution
static const char *degenerate_solution = R"doc(At least one tetrahedron has shape 0, 1 or infinity.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::SolutionType_::externally_computed
static const char *externally_computed = R"doc(Tetrahedron shapes were inserted into the triangulation.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::SolutionType_::flat_solution
static const char *flat_solution = R"doc(All tetrahedra are flat, but none have shape 0, 1 or infinity.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::SolutionType_::geometric_solution
static const char *geometric_solution = R"doc(All tetrahedra are positively oriented.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::SolutionType_::no_solution
static const char *no_solution = R"doc(The gluing equations could not be solved.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::SolutionType_::nongeometric_solution
static const char *nongeometric_solution =
R"doc(The overall volume is positive, but some tetrahedra are flat or
negatively oriented. No tetrahedra have shape 0, 1 or infinity.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::SolutionType_::not_attempted
static const char *not_attempted = R"doc(A solution has not been attempted.)doc";

// Docstring regina::python::doc::SnapPeaTriangulation_::SolutionType_::other_solution
static const char *other_solution =
R"doc(The volume is zero or negative, but the solution is neither flat nor
degenerate.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

