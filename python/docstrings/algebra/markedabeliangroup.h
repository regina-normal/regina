/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::HomMarkedAbelianGroup
static const char *HomMarkedAbelianGroup =
R"doc(Represents a homomorphism of finitely generated abelian groups.

One initializes such a homomorphism by providing:

* two finitely generated abelian groups, which act as domain and
  codomain;

* a matrix describing the linear map between the free abelian groups
  in the centres of the respective chain complexes that were used to
  define the domain and codomain. If the abelian groups are computed
  via homology with coefficients, the codomain coefficients must be a
  quotient of the domain coefficients.

So for example, if the domain was initialized by the chain complex
``Z^a --A--> Z^b --B--> Z^c`` with mod p coefficients, and the
codomain was initialized by ``Z^d --D--> Z^e --E--> Z^f`` with mod q
coefficients, then the matrix needs to be an e-by-b matrix.
Furthermore, you only obtain a well-defined homomorphism if this
matrix extends to a cycle map, which this class assumes but which the
user can confirm with isCycleMap(). Moreover, *q* should divide *p:*
this allows for *q* > 0 and *p* = 0, which means the domain has Z
coefficients and the codomain has mod *q* coefficients.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Author:
    Ryan Budney)doc";

// Docstring regina::python::doc::MarkedAbelianGroup
static const char *MarkedAbelianGroup =
R"doc(Represents a finitely generated abelian group given by a chain
complex.

This class is initialized with a chain complex. The chain complex is
given in terms of two integer matrices *M* and *N* such that M*N=0.
These matrices should be thought of as acting on column vectors: this
means for example that the product ``B*A`` applies the linear
transformation *A*, then the linear transformation *B*. This is
consistent with the convention that Regina uses for for multiplying
permutations.

The abelian group that this class computes is the kernel of *M* modulo
the image of *N*. In other words, we compute the homology of the chain
complex ``Z^a --N--> Z^b --M--> Z^c``, where *a* = N.columns(), *b* =
M.columns() = N.rows(), and *c* = M.rows(). An additional constructor
allows you to take the homology with coefficients in an arbitrary
cyclic group.

Like the simpler class AbelianGroup, this group will be isomorphic to
some ``Z_{d0} + ... + Z_{dk} + Z^r``, where:

* *r* is the number of free generators, as returned by rank();

* *d1*, ..., *dk* are the *invariant factors* that describe the
  torsion elements of the group, where 1 < *d1* | *d2* | ... | *dk*.

This class allows you to retrieve the invariant factors, the rank, and
the corresponding vectors in the kernel of *M*. Moreover, given a
vector in the kernel of *M*, it decribes the homology class of the
vector (the free part, and its position in the invariant factors).

The purpose of this class is to not just represent homology groups,
but also to support coordinates on the group allowing for the
construction of homomorphisms, and keeping track of subgroups.

This routine makes frequent use of two coordinate systems:

* *Chain complex coordinates* describe vectors of length *b* (using
  the notation above); that is, elements of the domain of *M*, or
  equivalently the codomain of *N*.

* *SNF (or Smith normal form) coordinates* describe elements of this
  abelian group in terms of the torsion and free generators. A vector
  in SNF coordinates has length (*k* + *r*), again using the notation
  above, where the first *k* elements store coefficients for the
  generators of the torsion components Z_{d1}, ..., Z_{dk}, and the
  remaining *r* elements store coefficients for the *r* free
  generators. You can see how the torsion and free generators appear
  in chain complex coordinates by calling torsionRep() or freeRep()
  respectively.

Be aware that the choice of torsion and free generators is typically
not unique, and this will affect the results of many member functions
of this class. These choices are subject to change between different
versions of Regina; in particular, they depend upon the particular
algorithm used for computing Smith normal forms.

Some routines in this class refer to the *internal presentation
matrix*. This is a proper presentation matrix for the abelian group;
if you are looking at the implementation details, this refers to the
matrix *pres*, created by taking the product ``MRi_ * N`` and then
removing the first *rankM_* rows.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Author:
    Ryan Budney and B.B.)doc";

namespace HomMarkedAbelianGroup_ {

// Docstring regina::python::doc::HomMarkedAbelianGroup_::__copy
static const char *__copy = R"doc(Creates a clone of the given homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::__init
static const char *__init =
R"doc(Constructs a homomorphism from two marked abelian groups and a matrix
that indicates where the generators are sent. The roles of the two
groups and the matrix are described in detail in the
HomMarkedAbelianGroup class overview.

The matrix must be given in the chain-complex coordinates.
Specifically, if the domain was defined via the chain complex ``Z^a
--N1--> Z^b --M1--> Z^c`` and the codomain was defined via ``Z^d
--N2--> Z^e --M2--> Z^f``, then *mat* is an e-by-b matrix that
describes a homomorphism from Z^b to Z^e.

In order for this to make sense as a homomorphism of the groups
represented by the domain and codomain respectively, one requires
img(mat*N1) to be a subset of img(N2). Similarly, ker(M1) must be sent
into ker(M2). These facts are not checked, but are assumed as
preconditions of this constructor.

Precondition:
    The matrix *mat* has the required dimensions e-by-b, gives
    img(mat*N1) as a subset of img(N2), and sends ker(M1) into
    ker(M2), as explained in the detailed notes above.

Parameter ``dom``:
    the domain group.

Parameter ``codom``:
    the codomain group.

Parameter ``mat``:
    the matrix that describes the homomorphism from *dom* to *ran*.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::__mul
static const char *__mul =
R"doc(Returns the composition of two homomorphisms.

Precondition:
    the homomorphisms must be composable, meaning that the codomain of
    X must have the same presentation matrices as the domain of this
    homomorphism.

Parameter ``X``:
    the homomorphism to compose this with.

Returns:
    the composite homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::__mul_2
static const char *__mul_2 =
R"doc(Returns the composition of two homomorphisms.

Precondition:
    the homomorphisms must be composable, meaning that the codomain of
    X must have the same presentation matrices as the domain of this
    homomorphism.

Parameter ``X``:
    the homomorphism to compose this with.

Returns:
    the composite homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::codomain
static const char *codomain =
R"doc(Returns the codomain of this homomorphism.

Returns:
    the codomain that was used to define the homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::cokernel
static const char *cokernel =
R"doc(Returns the cokernel of this homomorphism.

Returns:
    the cokernel of the homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::definingMatrix
static const char *definingMatrix =
R"doc(Returns the defining matrix for the homomorphism.

Returns:
    the matrix that was used to define the homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::domain
static const char *domain =
R"doc(Returns the domain of this homomorphism.

Returns:
    the domain that was used to define the homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::evalCC
static const char *evalCC =
R"doc(Evaluate the image of a vector under this homomorphism, using the
original chain complexes' coordinates. This involves multiplication by
the defining matrix.

Exception ``InvalidArgument``:
    The given vector was not in the original chain complex
    coordinates; that is, its length was not
    ``domain().M().columns()``.

Parameter ``input``:
    an input vector in the domain chain complex's coordinates, of
    length domain().M().columns().

Returns:
    the image of this vector in the codomain chain complex's
    coordinates, of length codomain().M().columns().)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::evalSNF
static const char *evalSNF =
R"doc(Evaluate the image of a vector under this homomorphism, using the
Smith normal form coordinates. This is just multiplication by the
reduced matrix.

.. warning::
    Smith normal form coordinates are sensitive to the implementation
    of the Smith Normal Form, i.e., they are not canonical.

Exception ``InvalidArgument``:
    The given vector was not in domain SNF coordinates; that is, its
    length was not ``domain().snfRank()``.

Parameter ``input``:
    an input vector in the domain SNF coordinates, of length
    domain().snfRank().

Returns:
    the image of this vector in the codomain chain complex's
    coordinates, of length codomain().snfRank().)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given homomorphisms.

This global routine simply calls HomMarkedAbelianGroup::swap(); it is
provided so that HomMarkedAbelianGroup meets the C++ Swappable
requirements.

Parameter ``lhs``:
    the homomorphism whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the homomorphism whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::image
static const char *image =
R"doc(Returns the image of this homomorphism.

Returns:
    the image of the homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::inverseHom
static const char *inverseHom =
R"doc(Returns the inverse to a HomMarkedAbelianGroup. If this homomorphism
is not invertible, this routine returns the zero homomorphism.

If you are computing with mod-p coefficients, this routine will
further require that this invertible map preserves the UCT splitting
of the group, i.e., it gives an isomorphism of the tensor product
parts and the TOR parts. At present this suffices since we're only
using this to construct maps between homology groups in different
coordinate systems.

Returns:
    the inverse homomorphism, or the zero homomorphism if this is not
    invertible.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::isChainMap
static const char *isChainMap =
R"doc(Determines whether this and the given homomorphism together form a
chain map.

Given two HomMarkedAbelianGroups, you have two diagrams:

```
Z^a --N1--> Z^b --M1--> Z^c   Z^g --N3--> Z^h --M3--> Z^i
                  ^                             ^
                  |this.matrix                  |other.matrix
Z^d --N2--> Z^e --M2--> Z^f   Z^j --N4--> Z^k --M4--> Z^l
```

If c=g and f=j and M1=N3 and M2=N4, you can ask if these maps commute,
i.e., whether you have a map of chain complexes.

Parameter ``other``:
    the other homomorphism to analyse in conjunction with this.

Returns:
    true if and only if c=g, M1=N3, f=j, M2=N4, and the diagram
    commutes.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::isCycleMap
static const char *isCycleMap =
R"doc(Is this at least a cycle map? If not, pretty much any further
computations you try with this class will be give you nothing more
than carefully-crafted garbage. Technically, this routine only checks
that cycles are sent to cycles, since it only has access to three of
the four maps you need to verify you have a cycle map.

Returns:
    ``True`` if and only if this is a chain map.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::isEpic
static const char *isEpic =
R"doc(Is this an epic homomorphism?

Returns:
    true if this homomorphism is epic.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::isIdentity
static const char *isIdentity =
R"doc(Is this the identity automorphism?

Returns:
    true if and only if the domain and codomain are defined via the
    same chain complexes and the induced map on homology is the
    identity.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::isIsomorphism
static const char *isIsomorphism =
R"doc(Is this an isomorphism?

Returns:
    true if this homomorphism is an isomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::isMonic
static const char *isMonic =
R"doc(Is this a monic homomorphism?

Returns:
    true if this homomorphism is monic.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::isZero
static const char *isZero =
R"doc(Is this the zero map?

Returns:
    true if this homomorphism is the zero map.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::kernel
static const char *kernel =
R"doc(Returns the kernel of this homomorphism.

Returns:
    the kernel of the homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::reducedMatrix
static const char *reducedMatrix =
R"doc(Returns the internal reduced matrix representing the homomorphism.
This is where the rows/columns of the matrix represent first the
torsion summands in the order of the invariant factors, and then the
free generators:

Z_{d0} + ... + Z_{dk} + Z^r where:

* *r* is the number of free generators, as returned by rank();

* *d1*, ..., *dk* are the invariant factors that describe the torsion
  elements of the group, where 1 < *d1* | *d2* | ... | *dk*.

Returns:
    a copy of the internal representation of the homomorphism.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::summary
static const char *summary =
R"doc(Returns a very brief summary of the type of map. This will state some
basic properties of the homomorphism, such as:

* whether the map is the identity;

* whether the map is an isomorphism;

* whether the map is monic or epic;

* if it is not monic, describes the kernel;

* if it is not epic, describes the co-kernel;

* if it is neither monic nor epic, describes the image.

Returns:
    a brief summary.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given homomorphism.

Parameter ``other``:
    the homomorphism whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::HomMarkedAbelianGroup_::torsionSubgroup
static const char *torsionSubgroup =
R"doc(Returns a HomMarkedAbelianGroup representing the induced map on the
torsion subgroups.)doc";

}

namespace MarkedAbelianGroup_ {

// Docstring regina::python::doc::MarkedAbelianGroup_::__copy
static const char *__copy = R"doc(Creates a clone of the given group.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::__eq
static const char *__eq =
R"doc(Determines whether this and the given group were formed from identical
chain complex constructions.

This is *not* an isomorphism test. For this comparison to return
``True``, both groups must have been constructed from identical
matrices *M* and *N*, using homology with the same coefficients.

Parameter ``other``:
    the group with which this should be compared.

Returns:
    ``True`` if and only if the this and the given group have
    identical chain complex definitions.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::__init
static const char *__init =
R"doc(Creates a marked abelian group from a chain complex. This constructor
assumes you are interested in homology with integer coefficents.

The abelian group is the kernel of *M* modulo the image of *N*. See
the class notes for further details.

Precondition:
    M.columns() = N.rows(). This condition will be tested, and an
    exception will be thrown if it does not hold.

Precondition:
    The product M*N = 0. This condition will *not* be tested (for
    efficiency reasons); this is left to the user/programmer to
    ensure.

Exception ``InvalidArgument``:
    The number of columns in *M* does not match the number of rows in
    *N*.

Parameter ``M``:
    the `right' matrix in the chain complex; that is, the matrix that
    one takes the kernel of when computing homology.

Parameter ``N``:
    the `left' matrix in the chain complex; that is, the matrix that
    one takes the image of when computing homology.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::__init_2
static const char *__init_2 =
R"doc(Creates a marked abelian group from a chain complex with coefficients
in Z_p.

The abelian group is the kernel of *M* modulo the image of *N*. See
the class notes for further details.

Precondition:
    M.columns() = N.rows(). This condition will be tested, and an
    exception will be thrown if it does not hold.

Precondition:
    The product M*N = 0. This condition will *not* be tested (for
    efficiency reasons); this is left to the user/programmer to
    ensure.

Exception ``InvalidArgument``:
    The number of columns in *M* does not match the number of rows in
    *N*.

Parameter ``M``:
    the `right' matrix in the chain complex; that is, the matrix that
    one takes the kernel of when computing homology.

Parameter ``N``:
    the `left' matrix in the chain complex; that is, the matrix that
    one takes the image of when computing homology.

Parameter ``pcoeff``:
    specifies the coefficient ring, Z_pcoeff. This must be non-
    negative; a value of 0 indicates that you are using integer
    coefficients (in which case it is more efficient to use the
    constructor that just takes two matrices).)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::__init_3
static const char *__init_3 =
R"doc(Creates a free Z_p-module of a given rank using the direct sum of the
standard chain complex ``0 --> Z --p--> Z --> 0``. This group is
isomorphic to ``n Z_p``. Moreover, if constructed using the matrices-
with-coefficients constructor, *M* would be zero and *N* would be
diagonal and square with *p* down the diagonal.

Parameter ``rank``:
    the rank of the group as a Z_p-module. That is, if the group is
    ``n Z_p``, then *rank* should be *n*.

Parameter ``p``:
    describes the type of ring that we use to talk about the "free"
    module.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::__ne
static const char *__ne =
R"doc(Determines whether this and the given group were formed from different
chain complex constructions.

This is *not* an isomorphism test. For this comparison to return
``False`` (i.e., for the chain complex constructions to be considered
identical), both groups must have been constructed from identical
matrices *M* and *N*, using homology with the same coefficients.

Parameter ``other``:
    the group with which this should be compared.

Returns:
    ``True`` if and only if the this and the given group do not have
    identical chain complex definitions.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::asBoundary
static const char *asBoundary =
R"doc(Expresses the given vector as a boundary in the chain complex.

.. warning::
    If you are using mod *p* coefficients and if your element projects
    to a non-trivial element of TOR, then N*v != bdry as elements of
    TOR are not in the image of *N*. In this case, (*bdry* - N*v)
    represents the projection to TOR.

.. warning::
    The return value is not unique. The specific choice of vector may
    change between different versions of Regina.

Exception ``InvalidArgument``:
    The given vector is not a boundary.

Parameter ``bdry``:
    a boundary vector, given in chain complex coordinates.

Returns:
    a vector *v* such that ``N*v=bdry``.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::boundaryOf
static const char *boundaryOf =
R"doc(Computes the differential of the given vector in the chain complex
whose kernel is the cycles. In other words, this routine returns
``M*chain``, where *M* is the "right" matrix passed to the class
constructor.

Exception ``InvalidArgument``:
    The given vector is not in chain complex coordinates; that is, its
    length is not M.columns().

Parameter ``chain``:
    any vector in chain complex coordinates.

Returns:
    the differential, expressed as a vector of length M.rows().)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::ccRank
static const char *ccRank =
R"doc(Returns the rank of the chain complex supporting the homology
computation.

This is the dimension of a vector in chain complex coordinates. In the
class notes, this is given by M.columns() and N.rows(), where *M* and
*N* are the matrices used to define the chain complex.

Returns:
    the rank of the chain complex.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::ccRep
static const char *ccRep =
R"doc(A combination of freeRep and torsionRep, this routine takes a group
element expressed in SNF coordinates and returns a corresponding
vector in the original chain complex.

This routine is, in some sense, an inverse to snfRep().

.. warning::
    The return value is not unique. The specific choice of vector may
    change between different versions of Regina.

Exception ``InvalidArgument``:
    The size of the given vector was not precisely snfRank().

Parameter ``snf``:
    any vector in SNF coordinates.

Returns:
    a corresponding vector in chain complex coordinates.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::ccRep_2
static const char *ccRep_2 =
R"doc(A combination of freeRep() and torsionRep() that expresses a group
element as a vector in the original chain complex.

This is similar to the variant of ccRep() that takes a vector as
input, but it assumes that your input is a standard basis vector from
SNF coordinates. Calling ccRep(snfGen) is equivalent to passing the
(*snfGen*)th unit vector to ccRep().

.. warning::
    The return value is not unique. The specific choice of vector may
    change between different versions of Regina.

Exception ``InvalidArgument``:
    The given index was greater than or equal to the number of
    generators in SNF coordinates (i.e., greater than or equal to
    snfRank()).

Parameter ``snfGen``:
    specifies which standard basis vector to use from SNF coordinates;
    this must be between 0 and snfRank()-1 inclusive.

Returns:
    a corresponding vector in chain complex coordinates.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::coefficients
static const char *coefficients =
R"doc(Returns the coefficients used for the computation of homology. That
is, this routine returns the integer *p* where we use coefficients in
Z_p. If we use coefficients in the integers Z, then this routine
returns 0.

Returns:
    the coefficients used in the homology calculation.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::countInvariantFactors
static const char *countInvariantFactors =
R"doc(Returns the number of invariant factors that describe the torsion
elements of this group. This is the minimal number of torsion
generators. See the MarkedAbelianGroup class notes for further
details.

Returns:
    the number of invariant factors.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::cycleGen
static const char *cycleGen =
R"doc(Returns the requested generator of the cycles, i.e., the kernel of the
"right" matrix *M* in the chain complex.

.. warning::
    The return value is not unique. The specific choice of vector may
    change between different versions of Regina.

Exception ``InvalidArgument``:
    The argument *index* was out of range (i.e., greater than or equal
    to cycleRank()).

Parameter ``index``:
    indicates which generator to return; this must be between 0 and
    cycleRank()-1 inclusive.

Returns:
    the (*index*)th generator of the cycles, expressed in chain
    complex coordinates.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::cycleProjection
static const char *cycleProjection =
R"doc(Projects an element of the chain complex to the subspace of cycles.

.. warning::
    The return value is not unique. The specific choice of vector may
    change between different versions of Regina.

Exception ``InvalidArgument``:
    The length of the given vector was not the dimension of the chain
    complex (i.e., the number of chain complex coordinates).

Parameter ``ccelt``:
    any vector in chain complex coordinates.

Returns:
    a corresponding vector, also in the chain complex coordinates.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::cycleProjection_2
static const char *cycleProjection_2 =
R"doc(Projects a standard basis vector of the chain complex to the subspace
of cycles.

.. warning::
    The return value is not unique. The specific choice of vector may
    change between different versions of Regina.

Exception ``InvalidArgument``:
    The index *ccindx* was greater than or equal to the dimension of
    the chain complex (i.e., ccRank()).

Parameter ``ccindx``:
    the index of the standard basis vector in chain complex
    coordinates.

Returns:
    the resulting projection, in the chain complex coordinates.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::cycleRank
static const char *cycleRank =
R"doc(Returns the number of generators of the kernel of *M*, where *M* is
the "right" matrix used to define the chain complex.

Returns:
    the number of generators of ker(*M*).)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::freeRep
static const char *freeRep =
R"doc(Returns the requested free generator of this group, represented in the
original chain complex defining the group.

The generator will be presented in chain complex coordinates.

.. warning::
    The return value is not unique. The specific choice of vector may
    change between different versions of Regina.

Exception ``InvalidArgument``:
    The argument *index* is out of range (i.e., greater than or equal
    to rank()).

Parameter ``index``:
    specifies which free generator to look up; this must be between 0
    and rank()-1 inclusive.

Returns:
    the (*index*)th free generator, presented in chain complex
    coordinates.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given abelian groups.

This global routine simply calls MarkedAbelianGroup::swap(); it is
provided so that MarkedAbelianGroup meets the C++ Swappable
requirements.

Parameter ``lhs``:
    the group whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the group whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::invariantFactor
static const char *invariantFactor =
R"doc(Returns the given invariant factor describing the torsion elements of
this group. See the MarkedAbelianGroup class notes for further
details.

If the invariant factors are *d0*|*d1*|...|*dn*, this routine will
return *di* where *i* is the value of parameter *index*.

Parameter ``index``:
    the index of the invariant factor to return; this must be between
    0 and countInvariantFactors()-1 inclusive.

Returns:
    the requested invariant factor.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::isBoundary
static const char *isBoundary =
R"doc(Determines whether the given vector represents a boundary in the chain
complex.

Parameter ``chain``:
    any vector in chain complex coordinates.

Returns:
    ``True`` if and only if the given vector represents a boundary.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::isCycle
static const char *isCycle =
R"doc(Determines whether the given vector represents a cycle in the chain
complex.

Parameter ``chain``:
    any vector in chain complex coordinates.

Returns:
    ``True`` if and only if the given vector represents a cycle.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::isIsomorphicTo
static const char *isIsomorphicTo =
R"doc(Determines whether this and the given abelian group are isomorphic.

Parameter ``other``:
    the group with which this should be compared.

Returns:
    ``True`` if and only if the two groups are isomorphic.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::isTrivial
static const char *isTrivial =
R"doc(Determines whether this is the trivial (zero) group.

Returns:
    ``True`` if and only if this is the trivial group.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::isZ
static const char *isZ =
R"doc(Determines whether this is the infinite cyclic group (Z).

Returns:
    ``True`` if and only if this is the infinite cyclic group.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::m
static const char *m =
R"doc(Returns the "right" matrix that was used to define the chain complex.
Our group was defined as the kernel of *M* modulo the image of *N*.
This is the matrix *M*.

This is the matrix *M* that was originally passed to the class
constructor. See the class overview for further details on matrices
*M* and *N* and their roles in defining the chain complex.

Returns:
    a reference to the defining matrix *M*.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::n
static const char *n =
R"doc(Returns the "left" matrix that was used to define the chain complex.
Our group was defined as the kernel of *M* modulo the image of *N*.
This is the matrix *N*.

This is the matrix *N* that was originally passed to the class
constructor. See the class overview for further details on matrices
*M* and *N* and their roles in defining the chain complex.

Returns:
    a reference to the defining matrix *N*.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::rank
static const char *rank =
R"doc(Returns the rank of the group. This is the number of included copies
of *Z*.

Equivalently, the rank is the maximum number of linearly independent
elements, and it indicates the size of the largest free abelian
subgroup. The rank effectively ignores all torsion elements.

Returns:
    the rank of the group.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::snfRank
static const char *snfRank =
R"doc(Returns the minimum number of generators for this group.

This is the dimension of a vector in SNF coordinates. It will always
be equal to rank() + countInvariantFactors().

Returns:
    the minimum number of generators.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::snfRep
static const char *snfRep =
R"doc(Expresses the given cycle as a combination of free and torsion
generators.

This routine takes a single argument *cycle*, which should be a cycle
in chain complex coordinates. This routine then returns this cycle as
a group element, expressed in SNF coordinates. See the class notes for
a full explanation of what these concepts mean.

In the vector that is returned, the coordinates that hold coefficients
for the torsion generators will be non-negative integers modulo the
corresponding invariant factors *d1*, ..., *dk*.

Specifically, using the notation from the class notes, suppose *cycle*
belongs to ker(*M*) and snfRep(*cycle*) returns the vector (*b1*, ...,
*bk*, *a1*, ..., *ar*). Suppose furthermore that the free generators
returned by freeRep(0..(r-1)) are *f1*, ..., *fr* respectively, and
that the torsion generators returned by torsionRep(0..(k-1)) are *t1*,
..., *tk* respectively. Then *cycle* = *b1*.t1 + ... + *bk*.tk +
*a1*.f1 + ... + *ar*.fr modulo img(*N*).

.. warning::
    The return value is not unique. The specific choice of vector may
    change between different versions of Regina.

Exception ``InvalidArgument``:
    The given vector was the wrong size, or is not a cycle (i.e., not
    in the kernel of *M*).

Parameter ``cycle``:
    a cycle, presented in chain complex coordinates.

Returns:
    the group element corresponding to *cycle*, expressed in SNF
    coordinates.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given group.

Parameter ``other``:
    the group whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::torsionInclusion
static const char *torsionInclusion =
R"doc(Returns a map representing the inclusion of the torsion subgroup into
this group.

Returns:
    the inclusion map for the torsion subgroup.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::torsionRank
static const char *torsionRank =
R"doc(Returns the rank in the group of the torsion term of given degree. If
the given degree is *d*, this routine will return the largest *m* for
which *m*Z_*d* is a subgroup of this group.

For instance, if this group is Z_6+Z_12, the torsion term of degree 2
has rank 2 (one occurrence in Z_6 and one in Z_12), and the torsion
term of degree 4 has rank 1 (one occurrence in Z_12).

Precondition:
    The given degree is at least 2.

Parameter ``degree``:
    the degree of the torsion term to query.

Returns:
    the rank in the group of the given torsion term.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::torsionRank_2
static const char *torsionRank_2 =
R"doc(Returns the rank in the group of the torsion term of given degree. If
the given degree is *d*, this routine will return the largest *m* for
which *m*Z_*d* is a subgroup of this group.

For instance, if this group is Z_6+Z_12, the torsion term of degree 2
has rank 2 (one occurrence in Z_6 and one in Z_12), and the torsion
term of degree 4 has rank 1 (one occurrence in Z_12).

Precondition:
    The given degree is at least 2.

Parameter ``degree``:
    the degree of the torsion term to query.

Returns:
    the rank in the group of the given torsion term.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::torsionRep
static const char *torsionRep =
R"doc(Returns the requested generator of the torsion subgroup, represented
in the original chain complex defining the group.

The generator will be presented in chain complex coordinates.

.. warning::
    The return value is not unique. The specific choice of vector may
    change between different versions of Regina.

Exception ``InvalidArgument``:
    The argument *index* is out of range (i.e., greater than or equal
    to countInvariantFactors()).

Parameter ``index``:
    specifies which generator in the torsion subgroup; this must be
    between 0 and countInvariantFactors()-1 inclusive.

Returns:
    the (*index*)th torsion generator, presented in chain complex
    coordinates.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::torsionSubgroup
static const char *torsionSubgroup =
R"doc(Returns the torsion subgroup of this group.

Returns:
    the torsion subgroup.)doc";

// Docstring regina::python::doc::MarkedAbelianGroup_::unmarked
static const char *unmarked =
R"doc(Returns just the underlying abelian group, without any of the chain
complex information.

Returns:
    the abelian group.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

