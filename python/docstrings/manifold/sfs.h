/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SFSFibre
static const char *SFSFibre =
R"doc(Represents an exceptional (*alpha*, *beta*) fibre in a Seifert fibred
space.

The first parameter *alpha* must be strictly positive, and the two
parameters *alpha* and *beta* must be coprime.

Note that we allow regular fibres with *alpha* = 1, and we do not
impose range limits on *beta* (thus *beta* may be negative, or it may
be larger than *alpha*). This is to allow more flexibility in routines
such as SFSpace::insertFibre().

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

.. warning::
    In Regina 4.2.1 and earlier, this class was named
    NExceptionalFibre. The new SFSFibre class was introduced with
    Regina 4.3, and has changed its behaviour (in particular, the
    natural ordering of fibres has changed). Code that was written to
    work with the old NExceptionalFibre class should be looked at
    closely before being adapted to the new SFSFibre class (i.e., it
    may require more than just substituting class names).)doc";

// Docstring regina::python::doc::SFSpace
static const char *SFSpace =
R"doc(Represents a general Seifert fibred space, which may be orientable or
non-orientable. Punctures and reflector boundaries in the base
orbifold are supported.

A Seifert fibred space whose base orbifold has no punctures or
reflector boundaries can be placed into one of the six classes ``o1``,
``o2``, ``n1``, ``n2``, ``n3`` and ``n4``, as detailed on page 88 of
"Seifert Manifolds", Peter Orlik, Springer-Verlag, 1972. These classes
describe whether this base surface is orientable, as well as how many
of its generators give fibre-reversing paths in the 3-manifold.

In the case where the base orbifold has punctures and/or reflector
boundaries, we use the five simplified classes ``bo1``, ``bo2``,
``bn1``, ``bn2`` and ``bn3``. These classes are not standard
terminology (i.e., they have been created explicitly for Regina), and
generally they do not provide enough information to uniquely identify
the 3-manifold. They do however identify whether or not the base
orbifold is orientable, and whether or not it contains any fibre-
reversing paths.

When describing punctures and reflector boundaries, a _twisted_
boundary is one that gives a fibre-reversing path, and an _untwisted_
boundary is one around which the direction of fibres is preserved.

Exceptional fibres are sorted first by *alpha* (the index) and then by
*beta*. The obstruction constant *b* is stored separately, though in
output routines such as name() and structure() it is merged in with
the exceptional fibres. Specifically, it is merged in with the *beta*
of the final exceptional fibre (replacing it with ``beta + b.alpha``),
or if there are no exceptional fibres then it is presented as a single
(1,b) fibre.

The Manifold routines homology() and construct() are only implemented
in some cases. The homology() routine is implemented if and only if
the base orbifold has no punctures. The construct() routine is
implemented only for lens spaces and Seifert fibred spaces over the
2-sphere without punctures or reflector boundaries.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that SFSpace still requires a non-trivial (but constant
sized) amount of data to be copied even in a move operation.

.. warning::
    In Regina 4.2.1 and earlier, this class was named NSFS. As of
    Regina 4.3, this class was renamed due to significant changes of
    behaviour (it became more general, and also now keeps the
    obstruction parameter *b* separate). Code that was written to work
    with the old NSFS class should be looked at closely before being
    adapted to the new SFSpace class (i.e., it may require more than
    just substituting class names).)doc";

namespace SFSFibre_ {

// Docstring regina::python::doc::SFSFibre_::__copy
static const char *__copy =
R"doc(Creates a new exceptional fibre that is a clone of the given fibre.

Parameter ``cloneMe``:
    the exceptional fibre to clone.)doc";

// Docstring regina::python::doc::SFSFibre_::__default
static const char *__default = R"doc(Creates a new uninitialised exceptional fibre.)doc";

// Docstring regina::python::doc::SFSFibre_::__eq
static const char *__eq =
R"doc(Determines if this and the given exceptional fibre are identical. This
requires both fibres to have the same values for *alpha* and the same
values for *beta*.

Parameter ``compare``:
    the fibre with which this will be compared.

Returns:
    ``True`` if and only if this and the given fibre are identical.)doc";

// Docstring regina::python::doc::SFSFibre_::__init
static const char *__init =
R"doc(Creates a new exceptional fibre with the given parameters.

Parameter ``newAlpha``:
    the first parameter (the index) of this exceptional fibre; this
    must be strictly positive.

Parameter ``newBeta``:
    the second parameter of this exceptional fibre; this must have no
    common factors with the first parameter *newAlpha*.)doc";

// Docstring regina::python::doc::SFSFibre_::__lt
static const char *__lt =
R"doc(Determines if this exceptional fibre is smaller than the given fibre.
Fibres are sorted by *alpha* and then by *beta*.

Parameter ``compare``:
    the fibre with which this will be compared.

Returns:
    ``True`` if and only if this is smaller than the given fibre.)doc";

// Docstring regina::python::doc::SFSFibre_::__ne
static const char *__ne =
R"doc(Determines if this and the given exceptional fibre are different. This
requires the fibres to have different values for *alpha* and/or to
have different values for *beta*.

Parameter ``compare``:
    the fibre with which this will be compared.

Returns:
    ``True`` if and only if this and the given fibre are different.)doc";

}

namespace SFSpace_ {

// Docstring regina::python::doc::SFSpace_::ClassType
static const char *ClassType =
R"doc(Lists the six classes ``o1``, ``o2``, ``n1``, ``n2``, ``n3``, ``n4``
for base orbifolds without boundaries, plus five classes ``bo1``,
``b02``, ``bn1``, ``bn2``, ``bn3`` for base orbifolds with boundaries.)doc";

// Docstring regina::python::doc::SFSpace_::__copy
static const char *__copy = R"doc(Creates a new copy of the given Seifert fibred space.)doc";

// Docstring regina::python::doc::SFSpace_::__default
static const char *__default =
R"doc(Creates a new Seifert fibred space with base orbifold the 2-sphere and
no exceptional fibres.)doc";

// Docstring regina::python::doc::SFSpace_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object contain precisely the
same presentations of the same Seifert fibred space.

This routine does _not_ test for homeomorphism. Instead it compares
the exact presentations, including the precise details of the base
orbifold and the exact parameters of the exceptional fibres, and
determines whether or not these _presentations_ are identical. If you
have two different presentations of the same Seifert fibred space,
they will be treated as not equal by this routine.

Parameter ``compare``:
    the presentation with which this will be compared.

Returns:
    ``True`` if and only if this and the given object contain
    identical presentations of the same Seifert fibred space.)doc";

// Docstring regina::python::doc::SFSpace_::__init
static const char *__init =
R"doc(Creates a new Seifert fibred space of the given class with the given
base orbifold and no exceptional fibres.

Precondition:
    If there are no punctures or reflector boundary components, then
    *useClass* is one of the six classes ``o1``, ``o2``, ``n1``,
    ``n2``, ``n3`` or ``n4``. Likewise, if there are punctures and/or
    reflector boundary components, then *useClass* is one of the five
    classes ``bo1``, ``bo2``, ``bn1``, ``bn2`` or ``bn3``.

Precondition:
    If there are any twisted punctures or reflector boundary
    components, then *useClass* is either ``bo2`` or ``bn3``.

Parameter ``useClass``:
    indicates whether the base orbifold is closed and/or orientable,
    and gives information about fibre-reversing paths in the
    3-manifold. See the SFSpace class notes and the ClassType
    enumeration notes for details.

Parameter ``genus``:
    the genus of the base orbifold (the number of tori or projective
    planes that it contains). Note that for non-orientable base
    surfaces, this is the non-orientable genus.

Parameter ``punctures``:
    the number of untwisted ordinary boundary components of the base
    orbifold. Here "ordinary" means that the puncture gives rise to a
    real 3-manifold boundary (i.e., this is not a reflector boundary
    of the base orbifold).

Parameter ``puncturesTwisted``:
    the number of twisted ordinary boundary components of the base
    orbifold. Here "ordinary" means that the puncture gives rise to a
    real 3-manifold boundary (i.e., this is not a reflector boundary
    of the base orbifold).

Parameter ``reflectors``:
    the number of untwisted reflector boundary components of the base
    orbifold. These are in addition to the ordinary boundary
    components described by *punctures*.

Parameter ``reflectorsTwisted``:
    the number of twisted reflector boundary components of the base
    orbifold. These are in addition to the ordinary boundary
    components described by *puncturesTwisted*.)doc";

// Docstring regina::python::doc::SFSpace_::__lt
static const char *__lt =
R"doc(Determines in a fairly ad-hoc fashion whether this representation of
this space is "smaller" than the given representation of the given
space.

The ordering imposed on Seifert fibred space representations is purely
aesthetic on the part of the author, and is subject to change in
future versions of Regina. It also depends upon the particular
representation, so that different representations of the same space
may be ordered differently.

All that this routine really offers is a well-defined way of ordering
Seifert fibred space representations.

Parameter ``compare``:
    the representation with which this will be compared.

Returns:
    ``True`` if and only if this is "smaller" than the given Seifert
    fibred space representation.)doc";

// Docstring regina::python::doc::SFSpace_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object do not contain precisely
the same presentations of the same Seifert fibred space.

This routine does _not_ test for homeomorphism. Instead it compares
the exact presentations, including the precise details of the base
orbifold and the exact parameters of the exceptional fibres, and
determines whether or not these _presentations_ are identical. If you
have two different presentations of the same Seifert fibred space,
they will be treated as not equal by this routine.

Parameter ``compare``:
    the presentation with which this will be compared.

Returns:
    ``True`` if and only if this and the given object do not contain
    identical presentations of the same Seifert fibred space.)doc";

// Docstring regina::python::doc::SFSpace_::addCrosscap
static const char *addCrosscap =
R"doc(Inserts a new crosscap into the base orbifold.

This makes the base orbifold non-orientable, and increases its non-
orientable genus by one. It is equivalent to removing a disc from the
base orbifold and replacing it with a Mobius band.

Note that this operation may alter which of the classes described by
ClassType this space belongs to.

The exceptional fibres and the obstruction constant *b* are not
modified by this routine.

Parameter ``fibreReversing``:
    ``True`` if the generator of the new crosscap should give a fibre-
    reversing curve in the overall 3-manifold, or ``False`` (the
    default) if it should preserve the directions of the fibres.)doc";

// Docstring regina::python::doc::SFSpace_::addHandle
static const char *addHandle =
R"doc(Inserts a new handle into the base orbifold.

This increases the orientable genus of the base orbifold by one, or
the non-orientable genus by two. It is equivalent to removing a disc
from the base orbifold and replacing it with a punctured torus.

Note that this operation may alter which of the classes described by
ClassType this space belongs to.

The exceptional fibres and the obstruction constant *b* are not
modified by this routine.

Parameter ``fibreReversing``:
    ``True`` if one or both generators of the new handle should give
    fibre-reversing curves in the overall 3-manifold, or ``False``
    (the default) if both generators should preserve the directions of
    the fibres.)doc";

// Docstring regina::python::doc::SFSpace_::addPuncture
static const char *addPuncture =
R"doc(Inserts one or more new punctures into the base orbifold. The
punctures may be twisted or untwisted.

Each puncture insertion is equivalent to removing a disc from the base
orbifold. In the untwisted case this results in a new torus boundary
for the 3-manifold, and in the twisted case it results in a new Klein
bottle boundary.

The exceptional fibres and the obstruction constant *b* are not
modified by this routine.

Parameter ``twisted``:
    ``True`` if the new punctures should be twisted (i.e., their
    boundaries should be fibre-reversing), or ``False`` if the new
    punctures should be untwisted.

Parameter ``nPunctures``:
    the number of new punctures to insert.)doc";

// Docstring regina::python::doc::SFSpace_::addReflector
static const char *addReflector =
R"doc(Adds one or more new reflector boundary components to the base
orbifold. The new reflector boundaries may be twisted or untwisted.

Each addition of a reflector boundary component is equivalent to
removing a disc from the base orbifold and replacing it with an
annulus with one reflector boundary.

In the untwisted case, it has the effect of removing a trivially
fibred solid torus from the overall 3-manifold and replacing it with
an appropriately fibred twisted I-bundle over the torus.

The exceptional fibres and the obstruction constant *b* are not
modified by this routine.

Parameter ``twisted``:
    ``True`` if the new reflector boundaries should be twisted (i.e.,
    the boundaries should be fibre-reversing), or ``False`` if the new
    reflector boundaries should be untwisted.

Parameter ``nReflectors``:
    the number of new reflector boundaries to add.)doc";

// Docstring regina::python::doc::SFSpace_::baseClass
static const char *baseClass =
R"doc(Returns which of the eleven predefined classes this space belongs to.
The specific class indicates whether the base orbifold has punctures
and/or reflector boundaries, whether the base orbifold is orientable,
and gives information on fibre-reversing paths.

The class can be (indirectly) modified by calling addHandle(),
addCrosscap(), addPuncture() or addReflector().

For more information on the eleven predefined classes, see the SFSpace
class notes or the ClassType enumeration notes.

Returns:
    the particular class to which this space belongs.)doc";

// Docstring regina::python::doc::SFSpace_::baseGenus
static const char *baseGenus =
R"doc(Returns the genus of the base orbifold. All punctures and reflector
boundaries in the base orbifold are ignored (i.e., they are treated as
though they had been replaced with ordinary filled discs).

The genus is the number of tori or projective planes that the base
surface is formed from. In particular, if the base surface is non-
orientable then this is the non-orientable genus.

Returns:
    the genus of the base orbifold.)doc";

// Docstring regina::python::doc::SFSpace_::baseOrientable
static const char *baseOrientable =
R"doc(Returns whether or not the base surface is orientable. Reflector
boundary components of the base orbifold are not considered here.

The orientability of the base surface can be (indirectly) modified by
calling addCrosscap().

Returns:
    ``True`` if and only if the base surface is orientable.)doc";

// Docstring regina::python::doc::SFSpace_::complementAllFibres
static const char *complementAllFibres =
R"doc(Replaces each exceptional fibre of the form (*alpha*, *beta*) with a
fibre of the form (*alpha*, *alpha* - *beta*). The obstruction
constant *b* is not touched.)doc";

// Docstring regina::python::doc::SFSpace_::fibre
static const char *fibre =
R"doc(Returns the requested exceptional fibre. Fibres are stored in sorted
order by *alpha* (the index) and then by *beta*. See the SFSpace class
notes for details.

.. warning::
    This routine takes linear time (specifically, linear in the
    argument *which*).

Parameter ``which``:
    determines which fibre to return; this must be between 0 and
    getFibreCount()-1 inclusive.

Returns:
    the requested fibre.)doc";

// Docstring regina::python::doc::SFSpace_::fibreCount
static const char *fibreCount =
R"doc(Returns the number of exceptional fibres in this Seifert fibred space.

Note that the obstruction parameter *b* is not included in this count.
That is, any (1,k) fibres are ignored.

Returns:
    the number of exceptional fibres.)doc";

// Docstring regina::python::doc::SFSpace_::fibreNegating
static const char *fibreNegating =
R"doc(Returns whether or not we can negate an exceptional fibre by passing
it around the interior of the base orbifold. That is, this routine
determines whether a (*p*, *q*) exceptional fibre can become a (*p*,
-*q*) exceptional fibre simply by sliding it around.

This is possible if either

* the base orbifold has an orientation-reversing loop that does not
  reverse fibres in the 3-manifold, or

* the base orbifold has an orientation-preserving loop that does
  reverse fibres in the 3-manifold.

Note that reflector boundary components, whilst making the overall
3-manifold non-orientable, have no bearing on the outcome of this
routine.

Returns:
    ``True`` if and only an exceptional fibre can be reflected as
    described above.)doc";

// Docstring regina::python::doc::SFSpace_::fibreReversing
static const char *fibreReversing =
R"doc(Returns whether or not this space contains any fibre-reversing paths.

Returns:
    ``True`` if and only if a fibre-reversing path exists.)doc";

// Docstring regina::python::doc::SFSpace_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given Seifert fibred spaces.

This global routine simply calls SFSpace::swap(); it is provided so
that SFSpace meets the C++ Swappable requirements.

Parameter ``a``:
    the first space whose contents should be swapped.

Parameter ``b``:
    the second space whose contents should be swapped.)doc";

// Docstring regina::python::doc::SFSpace_::insertFibre
static const char *insertFibre =
R"doc(Adds the given fibre to this Seifert fibred space.

This may be an exceptional fibre (*alpha* > 1) or it may be a regular
fibre (*alpha* = 1). If it is a regular fibre, the obstruction
constant *b* will be adjusted according to the value of *beta*.

Note that there is no restriction on the range of the second parameter
*beta*. If it is out of the usual range 0 ≤ *beta* < *alpha*, it will
be pulled back into this range and the excess will be pushed into the
obstruction constant *b*.

Exception ``InvalidArgument``:
    *alpha* is zero.

Parameter ``fibre``:
    the fibre to insert. The first parameter of this fibre (i.e., its
    index) must be strictly positive, and the two parameters of this
    fibre must be coprime.)doc";

// Docstring regina::python::doc::SFSpace_::insertFibre_2
static const char *insertFibre_2 =
R"doc(Adds the given fibre to this Seifert fibred space.

This may be an exceptional fibre (*alpha* > 1) or it may be a regular
fibre (*alpha* = 1). If it is a regular fibre, the obstruction
constant *b* will be adjusted according to the value of *beta*.

Note that there is no restriction on the range of the second parameter
*beta*. If it is out of the usual range 0 ≤ *beta* < *alpha*, it will
be pulled back into this range and the excess will be pushed into the
obstruction constant *b*.

Exception ``InvalidArgument``:
    *alpha* is zero.

Parameter ``alpha``:
    the first parameter (i.e., the index) of the fibre to insert; this
    must be strictly positive.

Parameter ``beta``:
    the second parameter of the fibre to insert; this must have no
    common factors with the first parameter *alpha*.)doc";

// Docstring regina::python::doc::SFSpace_::isLensSpace
static const char *isLensSpace =
R"doc(Determines if this Seifert fibred space is a Lens space.

Returns:
    a structure containing the details of this Lens space, or no value
    if this is not a Lens space.)doc";

// Docstring regina::python::doc::SFSpace_::obstruction
static const char *obstruction =
R"doc(Returns the obstruction constant *b* for this Seifert fibred space.

The obstruction constant corresponds to the insertion of an additional
(1,*b*) fibre. It can be modified by calling insertFibre() with a
value of *alpha* = 1. It will also be modified whenever insertFibre()
is called with *beta* out of range (*beta* < 0 or *beta* ≥ *alpha*),
since each exceptional fibre must be stored in standard form (0 ≤
*beta* < *alpha*).

Returns:
    the obstruction constant *b*.)doc";

// Docstring regina::python::doc::SFSpace_::punctures
static const char *punctures =
R"doc(Returns the total number of punctures in the base orbifold. In other
words, this routine returns the total number of real torus or Klein
bottle boundary components in the overall 3-manifold.

Note that reflector boundaries on the base orbifold are _not_ counted
here; only the ordinary boundary components that give rise to real
3-manifold boundaries are included.

Both untwisted and twisted punctures (giving rise to torus and Klein
bottle boundaries respectively in the 3-manifold) are counted by this
routine.

Returns:
    the total number of punctures.)doc";

// Docstring regina::python::doc::SFSpace_::punctures_2
static const char *punctures_2 =
R"doc(Returns the number of punctures of the given type in the base
orbifold. In other words, this routine returns the number of real
boundary components of the given type in the overall 3-manifold.

This routine either counts only twisted punctures (which give rise to
Klein bottle boundaries), or only untwisted punctures (which give rise
to torus boundaries).

Either way, reflector boundaries on the base orbifold are _not_
counted here; only ordinary boundary components that give rise to real
3-manifold boundaries are considered.

Parameter ``twisted``:
    ``True`` if only twisted punctures should be counted (those that
    give fibre-reversing paths and Klein bottle boundaries), or
    ``False`` if only untwisted punctures should be counted (those
    that are fibre-preserving and give torus boundaries).

Returns:
    the number of punctures of the given type.)doc";

// Docstring regina::python::doc::SFSpace_::reduce
static const char *reduce =
R"doc(Reduces the parameters of this Seifert fibred space to a simpler form
if possible, without changing the underlying fibration.

In some cases the parameters of the Seifert fibred space may be
simplified by taking a mirror image of the entire 3-manifold. The
argument *mayReflect* signifies whether this is allowed.

This routine will not change the curves made by the fibres and the
base orbifold on any boundary components (i.e., boundaries caused by
punctures in the base orbifold).

.. warning::
    If *mayReflect* is ``True`` then the entire 3-manifold might be
    replaced with its mirror image, in which case any subsequent
    modifications (such as inserting additional fibres or altering the
    base orbifold) may give unexpected results.

Parameter ``mayReflect``:
    ``True`` if we are allowed to take a mirror image of the entire
    3-manifold, or ``False`` if we are not.)doc";

// Docstring regina::python::doc::SFSpace_::reflect
static const char *reflect =
R"doc(Replaces this space with its mirror image. Specifically, all
exceptional fibres and the obstruction constant *b* will be negated.
Note that the obstruction constant will generally undergo further
change as the exceptional fibres are standardised into the usual 0 ≤
*beta* < *alpha* form.

This routine will not change the curves made by the fibres and the
base orbifold on any boundary components (i.e., boundaries caused by
punctures in the base orbifold), with the exception that each base
curve will be reflected.

.. warning::
    The space is _not_ reduced after reflecting. It may be that the
    space can be further simplified (especially in the case of non-
    orientable manifolds).)doc";

// Docstring regina::python::doc::SFSpace_::reflectors
static const char *reflectors =
R"doc(Returns the total number of reflector boundary components of the base
orbifold. This includes both twisted and untwisted reflector
boundaries.

Returns:
    the total number of reflector boundary components.)doc";

// Docstring regina::python::doc::SFSpace_::reflectors_2
static const char *reflectors_2 =
R"doc(Returns the number of reflector boundary components of the given type
in the base orbifold. This either counts only twisted reflector
boundaries, or only untwisted reflector boundaries.

Parameter ``twisted``:
    ``True`` if only twisted reflector boundaries should be counted
    (those that give fibre-reversing paths), or ``False`` if only
    untwisted reflector boundaries should be counted.

Returns:
    the number of reflector boundaries of the given type.)doc";

// Docstring regina::python::doc::SFSpace_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given Seifert fibred space.

Parameter ``other``:
    the space whose contents should be swapped with this.)doc";

}

namespace SFSpace_::ClassType_ {

// Docstring regina::python::doc::SFSpace_::ClassType_::bn1
static const char *bn1 =
R"doc(Indicates that the base orbifold contains punctures and/or reflector
boundaries, that it is non-orientable, and that it contains no fibre-
reversing paths.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::bn2
static const char *bn2 =
R"doc(Indicates that the base orbifold contains punctures and/or reflector
boundaries, that it is non-orientable, and that its fibre-reversing
paths correspond precisely to its orientation-reversing paths.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::bn3
static const char *bn3 =
R"doc(Indicates that the base orbifold contains punctures and/or reflector
boundaries, that it is non-orientable, that it contains at least one
fibre-reversing path, and that its fibre-reversing paths do not
correspond precisely to its orientation-reversing paths.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::bo1
static const char *bo1 =
R"doc(Indicates that the base orbifold contains punctures and/or reflector
boundaries, that it is orientable, and that it contains no fibre-
reversing paths.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::bo2
static const char *bo2 =
R"doc(Indicates that the base orbifold contains punctures and/or reflector
boundaries, that it is orientable, and that it contains at least one
fibre-reversing path.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::n1
static const char *n1 =
R"doc(Indicates that the base orbifold is non-orientable with no punctures
or reflector boundaries, and that none of its generators give fibre-
reversing paths.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::n2
static const char *n2 =
R"doc(Indicates that the base orbifold is non-orientable with no punctures
or reflector boundaries, and that all of its generators give fibre-
reversing paths.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::n3
static const char *n3 =
R"doc(Indicates that the base orbifold is non-orientable with no punctures
or reflector boundaries, that it has non-orientable genus at least
two, and that precisely one of its generators gives a fibre-reversing
path.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::n4
static const char *n4 =
R"doc(Indicates that the base orbifold is non-orientable with no punctures
or reflector boundaries, that it has non-orientable genus at least
three, and that precisely two of its generators give fibre-reversing
paths.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::o1
static const char *o1 =
R"doc(Indicates that the base orbifold is orientable with no punctures or
reflector boundaries, and that none of its generators give fibre-
reversing paths.)doc";

// Docstring regina::python::doc::SFSpace_::ClassType_::o2
static const char *o2 =
R"doc(Indicates that the base orbifold is orientable with no punctures or
reflector boundaries, and that all of its generators give fibre-
reversing paths.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

