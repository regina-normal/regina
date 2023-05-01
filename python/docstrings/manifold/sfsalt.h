/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SFSAlt
static const char *SFSAlt =
R"doc(Provides an alternative representation of a single bounded Seifert
fibred space. Such alternatives are made possible by altering the
curves made by the fibre and base orbifold on a boundary torus.

This class is designed to help in finding simple representations of
graph manifolds (or, indeed, any non-geometric manifolds containing
Seifert fibred blocks).

Each alternative comes with its own representation of the original
Seifert fibred space, along with instructions for converting
fibre/base curves on the boundary tori between the original and
alternative spaces.

The alternative representation will generally be as simple as possible
(and indeed the hope is that it will be simpler than the original). In
particular, each alternative space is guaranteed to have obstruction
constant zero. The base orbifold may be changed entirely (for
instance, an orientable Seifert fibred space over the Mobius band with
no exceptional fibres will be converted to a Seifert fibred space over
the disc with two exceptional fibres).

The conversions between boundary curves are described by a conversion
matrix *M* as follows. Consider the first boundary torus. Let *f_old*
and *o_old* be directed curves on this boundary representing the fibre
and base orbifold of the original space, and let *f_alt* and *o_alt*
be directed curves on this same boundary representing the fibre and
base orbifold of the new alternative space. Then

```
    [f_alt]         [f_old]
    [     ]  =  M * [     ].
    [o_alt]         [o_old]
```

Note that this _only_ applies to the first boundary torus! If the
Seifert fibred space has more than one boundary, then for the
remaining boundaries the unoriented fibre and base curves remain the
same. More specifically, the directed fibre remains identical, and the
directed curve representing the base orbifold is reversed if and only
if a reflection was used in creating the alternative space, as
returned by reflected().

See the page on notation for Seifert fibred spaces for details on some
of the terminology used above.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that SFSAlt still requires a non-trivial (but constant sized)
amount of data to be copied even in a move operation.)doc";

namespace SFSAlt_ {

// Docstring regina::python::doc::SFSAlt_::__copy
static const char *__copy = R"doc(Creates a new copy of the given alternative.)doc";

// Docstring regina::python::doc::SFSAlt_::__eq
static const char *__eq =
R"doc(Determines whether this and the given alternative representation have
identical presentations.

To be considered _identical_, the two alternatives must have equal
alternative SFSpace representations, equal conversion matrices, and
either both must have used a reflection or both must have not used a
reflection. In other words, this is equivalent to testing all of
alt(), conversion() and reflected() for equality.

Parameter ``other``:
    the alternative to compare against this.

Returns:
    ``True`` if and only if this and the given alternative have
    identical presentations, as described above.)doc";

// Docstring regina::python::doc::SFSAlt_::__init
static const char *__init =
R"doc(Creates a "basic" alternative representation for the given Seifert
fibred space. This will:

* reduce the parameters via SFSpace::reduce(), without reflecting;

* add twists to the first boundary torus so that the obstruction
  constant *b* in the Seifert fibred space becomes zero;

* if the Seifert fibred space is over ``M/n2`` with no exceptional
  fibres (where ``M`` represents the Mobius band), it will switch the
  fibre and orbifold curves to give a Seifert fibred space over the
  disc with two exception fibres.

Precondition:
    The given Seifert fibred space has at least one torus boundary.

Parameter ``original``:
    the original Seifert fibred space for which we are creating a set
    of alternative representations.)doc";

// Docstring regina::python::doc::SFSAlt_::__init_2
static const char *__init_2 =
R"doc(Creates a new alternative representation from the given alternative
representation. The changes will include:

* if *reflect* is ``True``, then this will reflect the Seifert fibred
  space;

* if *negate* is ``True``, then this will attempt to replace all
  fibres (*alpha*, *beta*) with (*alpha*, *alpha* - *beta*), _without_
  any further reflections (but see below for conditions on when this
  is possible);

* in all cases, this will reduce the parameters via SFSpace::reduce()
  and add twists to give obstruction constant zero.

Asking for reflection is always valid. However, the option to negate
without reflection is possible only if:

* it is possible to negate an exceptional fibre by sliding it around
  the base orbifold (as in SFSpace::fibreNegating()); and

* the number of exceptional fibres is odd.

For convenience, you can test these conditions using the static
function canNegate().

Precondition:
    If *negate* is ``True``, then the underlying Seifert fibred space
    satisfies the requirements outlined above, or equivalently,
    ``canNegate(base.sfs())`` is ``True``.

.. note::
    This constructor only makes sense if at least one of ``reflect``
    or ``negate`` is ``True``, since *base* should already be in a
    reduced form with zero obstruction constant.

Parameter ``base``:
    the Seifert fibred space representation that will be used as a
    starting point for this new alternative.

Parameter ``reflect``:
    ``True`` if we should reflect the Seifert fibred space.

Parameter ``negate``:
    ``True`` if we should attempt to negate all exceptional fibres
    without reflecting, as described above.)doc";

// Docstring regina::python::doc::SFSAlt_::__ne
static const char *__ne =
R"doc(Determines whether this and the given alternative representation do
not have identical presentations.

To be considered _identical_, the two alternatives must have equal
alternative SFSpace representations, equal conversion matrices, and
either both must have used a reflection or both must have not used a
reflection. In other words, this is equivalent to testing all of
alt(), conversion() and reflected() for equality.

Parameter ``other``:
    the alternative to compare against this.

Returns:
    ``True`` if and only if this and the given alternative do not have
    identical presentations, as described above.)doc";

// Docstring regina::python::doc::SFSAlt_::alt
static const char *alt =
R"doc(Returns the alternative representation of the original Seifert fibred
space.

Returns:
    the alternative representation.)doc";

// Docstring regina::python::doc::SFSAlt_::alt_2
static const char *alt_2 =
R"doc(Returns a non-const reference to the alternative representation of the
original Seifert fibred space.

Returns:
    the alternative representation.)doc";

// Docstring regina::python::doc::SFSAlt_::alt_3
static const char *alt_3 =
R"doc(Returns an rvalue reference to the alternative representation of the
original Seifert fibred space.

Returns:
    the alternative representation.)doc";

// Docstring regina::python::doc::SFSAlt_::altSet
static const char *altSet =
R"doc(Returns a set of alternatives for the given Seifert fibred space.
These will consist of (1) the alternative *base* created by passing
the original Seifert fibred space to the SFSAlt constructor; and (2)
all alternatives created by passing *base* with appropriate reflection
and negation arguments to the SFSAlt constructor. The combinations of
reflection and negation arguments that are used (and hence the size of
the set that is returned) will depend on the properties of the
original space.

Note that in general, none of the alternatives will have a
representation identical to the original (generally these alternative
representations will be simpler if possible).

It is guaranteed that the set that is returned will be non-empty.

Precondition:
    The given Seifert fibred space has at least one torus boundary.

Parameter ``sfs``:
    the original Seifert fibred space.

Returns:
    the resulting set of alternative representations for *sfs*.)doc";

// Docstring regina::python::doc::SFSAlt_::canNegate
static const char *canNegate =
R"doc(Determines whether the SFSAlt class constructor will be able to negate
all exceptional fibres without reflecting the underlying Seifert
fibred space.

See the constructor SFSAlt(const SFSAlt&, bool, bool) for further
details.

Precondition:
    The given Seifert fibred space has at least one torus boundary.

Parameter ``sfs``:
    the Seifert fibred space that we are attempting to represent.

Returns:
    ``True`` if and only if it is possible to set the negation
    argument to ``True`` in the SFSAlt class constructor.)doc";

// Docstring regina::python::doc::SFSAlt_::conversion
static const char *conversion =
R"doc(Returns the conversion matrix for this alternative space. This matrix
describes the fibre and base curves of the alternative space on the
first boundary torus in terms of the fibre and base curves of the
original space (which was passed to the SFSAlt constructor). See the
class notes above for details.

Note that this conversion matrix applies _only_ to the first boundary
torus! If there is more than one boundary, the remaining boundary
conversions are simpler and depend only on whether a reflection has
been used or not. See reflected() or the class notes for details.

Returns:
    the conversion matrix for this alternative space.)doc";

// Docstring regina::python::doc::SFSAlt_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given alternative representations of
Seifert fibred spaces.

This global routine simply calls SFSAlt::swap(); it is provided so
that SFSAlt meets the C++ Swappable requirements.

Parameter ``a``:
    the first alternative whose contents should be swapped.

Parameter ``b``:
    the second alternative whose contents should be swapped.)doc";

// Docstring regina::python::doc::SFSAlt_::reflected
static const char *reflected =
R"doc(Returns whether or not a reflection was used when creating this
alternative space. This determines the conversion between boundary
curves for all boundary tori after the first.

More specifically, if no reflection was used then the directed fibre
and base curves are identical for the original and alternative spaces.
If a reflection was used, then the directed fibres are identical but
the directed base curves are reversed.

The conversion between curves on the first boundary torus is generally
more complex, and is returned as a matrix by the conversion() routine.

You can also test whether a reflection was used by examining whether
the conversion matrix has determinant 1 or -1. However, calling
reflected() is both simpler and a little faster.

Returns:
    ``True`` if a reflection was used in creating this alternative
    space, or ``False`` if no reflection was used.)doc";

// Docstring regina::python::doc::SFSAlt_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given alternative.

Parameter ``other``:
    the alternative whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

