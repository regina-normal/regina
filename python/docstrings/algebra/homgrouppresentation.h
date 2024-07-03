/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::HomGroupPresentation
static const char *HomGroupPresentation =
R"doc(Represents a homomorphism between groups which are described via
finite presentations.

Some homomorphisms may be _declared isomorphisms_. This means that the
user (or some other function in Regina) has proven that this is an
isomorphism and has explicitly provided the inverse map. To provide
the inverse map, you should call the four-argument constructor
HomGroupPresentation(const GroupPresentation&, const
GroupPresentation&, const std::vector<GroupExpression>&, const
std::vector<GroupExpression>&).

You can test for a declared isomorphism by calling knowsInverse().
Even if a homomorphism is not a declared isomorphism, it might still
be an isomorphism; this just means that no inverse map was explicitly
provided.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

namespace HomGroupPresentation_ {

// Docstring regina::python::doc::HomGroupPresentation_::__copy
static const char *__copy = R"doc(Creates a clone of the given homomorphism.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::__eq
static const char *__eq =
R"doc(Determines whether this and the given homomorphism have identical
presentations.

This routine does _not_ test whether the two homomorphisms are equal
in the sense that each element of the domain maps to the same group
element of the codomain - in general this is an undecidable problem.

Instead, this routine tests whether the two homomorphisms map the
*i*th generator of the domain to precisely the same _word_ in the
codomain, for each *i*.

This routine will not test whether the domains and codomains have
identical presentations, or whether either homomorphism is declared to
be an isomorphism. However, if the two domains have different numbers
of generators then the two homomorphisms will compare as not equal
(since it will be impossible to compare the words that the
corresponding generators map to).

Parameter ``other``:
    the homomorphism to compare with this.

Returns:
    ``True`` if and only if this and the given homomorphisms have
    identical presentations.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::__init
static const char *__init =
R"doc(Creates a new homomorphism from the given data.

Parameter ``domain``:
    the domain of the homomorphism.

Parameter ``codomain``:
    the codomain of the homomorphism.

Parameter ``map``:
    a vector of length *g*, where *g* is the number of generators of
    the domain, and where this homomorphism sends the *i*th generator
    of the domain to the element ``map[i]`` of the codomain.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::__init_2
static const char *__init_2 =
R"doc(Creates a declared isomorphism from the given data. Here you must
provide both a map from the domain to codomain, and the inverse map
from the codomain to domain.

Precondition:
    The argument *inv* is indeed the inverse of *map*.

Parameter ``domain``:
    the domain of the homomorphism.

Parameter ``codomain``:
    the codomain of the homomorphism.

Parameter ``map``:
    a vector of length *g*, where *g* is the number of generators of
    the domain, and where this homomorphism sends the *i*th generator
    of the domain to the element ``map[i]`` of the codomain.

Parameter ``inv``:
    a vector of length *k* where *k* is the number of generators of
    the codomain, and where the inverse homomorphism sends the *i*th
    generator of the codomain to the element ``inv[i]`` of the domain.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::__init_3
static const char *__init_3 =
R"doc(Creates a new identity homomorphism for the given group.

This will be a declared isomorphism (see the HomGroupPresentation
class notes for details).

Parameter ``groupForIdentity``:
    both the domain and codomain of the new identity homomorphism.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::__mul
static const char *__mul =
R"doc(Composes this homomorphism with the given homomorphism.

Evaluating the composition on some group element *x* is the same as
evaluating ``this(rhs(x))``. In other words, in this composition,
*rhs* is evaluated first and then the output of that is evaluated by
this homomorphism.

If both of the given homomorphisms are declared isomorphisms, then the
return value will be a declared isomoprhism also.

Precondition:
    the codomain of *rhs* must be the same as the domain of this
    homomorphism.

Parameter ``rhs``:
    the homomorphism to compose this with.

Returns:
    the composition of both homomorphisms.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::__mul_2
static const char *__mul_2 =
R"doc(Composes this homomorphism with the given homomorphism.

Evaluating the composition on some group element *x* is the same as
evaluating ``this(rhs(x))``. In other words, in this composition,
*rhs* is evaluated first and then the output of that is evaluated by
this homomorphism.

If both of the given homomorphisms are declared isomorphisms, then the
return value will be a declared isomoprhism also.

Precondition:
    the codomain of *rhs* must be the same as the domain of this
    homomorphism.

Parameter ``rhs``:
    the homomorphism to compose this with.

Returns:
    the composition of both homomorphisms.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::__ne
static const char *__ne =
R"doc(Determines whether this and the given homomorphism do not have
identical presentations.

This routine does _not_ test whether the two homomorphisms are equal
in the sense that each element of the domain maps to the same group
element of the codomain - in general this is an undecidable problem.

Instead, this routine tests whether the two homomorphisms map the
*i*th generator of the domain to precisely the same _word_ in the
codomain, for each *i*.

This routine will not test whether the domains and codomains have
identical presentations, or whether either homomorphism is declared to
be an isomorphism. However, if the two domains have different numbers
of generators then the two homomorphisms will compare as not equal
(since it will be impossible to compare the words that the
corresponding generators map to).

Parameter ``other``:
    the homomorphism to compare with this.

Returns:
    ``True`` if and only if this and the given homomorphisms do not
    have identical presentations.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::codomain
static const char *codomain =
R"doc(The codomain of the map.

Returns:
    a reference to the codomain.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::domain
static const char *domain =
R"doc(The domain of the map.

Returns:
    a reference to the domain.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::evaluate
static const char *evaluate =
R"doc(Evaluate the homomorphism at an element of the domain.

Parameter ``arg``:
    an element of the domain.

Returns:
    the image of this element in the codomain.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::evaluate_2
static const char *evaluate_2 =
R"doc(Evaluate the homomorphism at a generator of the domain.

Parameter ``i``:
    the index of a generator in the domain.

Returns:
    the image of the *i*th generator in the codomain.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given homomorphisms.

This global routine simply calls HomGroupPresentation::swap(); it is
provided so that HomGroupPresentation meets the C++ Swappable
requirements.

Parameter ``lhs``:
    the homomorphism whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the homomorphism whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::intelligentNielsen
static const char *intelligentNielsen =
R"doc(Deprecated alias for nielsen(), which simplifies the domain and
codomain using only Nielsen moves, keeping track of the resulting map
in the progress.

.. deprecated::
    This routine has been renamed to nielsen(). See nielsen() for
    further details.

Returns:
    ``True`` if and only if either presentation and/or the map was
    changed.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::intelligentSimplify
static const char *intelligentSimplify =
R"doc(Deprecated alias for simplify(), which simultaneously simplifies the
presentation of the domain, the presentation of the codomain, and the
description of the map.

.. deprecated::
    This routine has been renamed to simplify(). See simplify() for
    further details.

Returns:
    ``True`` if and only if either presentation and/or the map was
    changed.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::invEvaluate
static const char *invEvaluate =
R"doc(Evaluate the isomorphisms's inverse at an element of the codomain.

Precondition:
    This homomorphism is in fact a declared isomorphism. See the
    HomGroupPresentation class notes for details on what this means.

Parameter ``arg``:
    an element of the codomain.

Returns:
    the image of this element in the domain.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::invEvaluate_2
static const char *invEvaluate_2 =
R"doc(Evaluate the isomorphism at a generator of the codomain.

Precondition:
    This homomorphism is in fact a declared isomorphism. See the
    HomGroupPresentation class notes for details on what this means.

Parameter ``i``:
    the index of a generator in the codomain.

Returns:
    the image of this generator in the domain.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::invert
static const char *invert =
R"doc(Inverts the homomorphism.

This is only possible if the homomorphism is in fact a declared
isomorphism (which means that the inverse map is already stored
internally). See the HomGroupPresentation class notes for further
details on declared isomorphisms.

If this is not a declared isomorphism then this routine will do
nothing and simply return ``False``.

This operation is (very) fast constant time.

Returns:
    ``True`` if and only if the inversion operation was successful
    (i.e., if this is a declared isomorphism).)doc";

// Docstring regina::python::doc::HomGroupPresentation_::knowsInverse
static const char *knowsInverse =
R"doc(Returns whether or not this is a declared isomorphism.

A _declared isomorphism_ is a isomorphism for which the user has
explicitly provided the inverse map. See the HomGroupPresentation
class notes for details.

Returns:
    ``True`` if and only if this is a declared isomorphism, i.e, the
    inverse map was explicitly provided.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::markedAbelianisation
static const char *markedAbelianisation =
R"doc(Computes the induced map on the abelianizations of the domain and
codomain.

Returns:
    the induced map on the abelianizations.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::nielsen
static const char *nielsen =
R"doc(Simplifies the domain and codomain using only Nielsen moves, keeping
track of the resulting map in the progress.

Returns:
    ``True`` if and only if either presentation and/or the map was
    changed.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::simplify
static const char *simplify =
R"doc(Simultaneously simplifies the presentation of the domain, the
presentation of the codomain, and the description of the map.

Uses the underlying GroupPresentation::simplify(). See that routine
for details.

Returns:
    ``True`` if and only if either presentation and/or the map was
    changed.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::smallCancellation
static const char *smallCancellation =
R"doc(Simplifies the domain and codomain using only small cancellation
theory.

Returns:
    ``True`` if and only if either presentation and/or the map was
    changed.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given homomorphism.

Parameter ``other``:
    the homomorphism whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::verify
static const char *verify =
R"doc(Verifies the map is a valid homomorphism. Specifically, this routine
runs through all the relators in the domain, evaluates the
homomorphism on the relators and checks that they simplify to 1 in the
codomain.

This routine does not guarantee a conclusive result (since the word
problem is, in general, undecidable). If this routine returns ``True``
then this proves that the homomorphism is indeed valid. If this
routine returns ``False``, then the result is inconclusive (i.e., it
might still be valid but Regina was not able to prove this).

This routine is intended for sanity checking only: any homomorphism
that you construct in Regina should always be valid in this sense.

Returns:
    ``True`` if Regina is able to verify that this is a homomorphism,
    or ``False`` if the result is inconclusive.)doc";

// Docstring regina::python::doc::HomGroupPresentation_::verifyIsomorphism
static const char *verifyIsomorphism =
R"doc(Attempts to verify that a declared isomorphism is, indeed, an
isomorphism.

This routine works by attempting to verify that ``f^-1(f(x))x^-1``
simplifes to 1 for all generators *x* in the domain, and likewise for
the codomain.

This routine does not guarantee a conclusive result. If this routine
returns ``True`` then this proves that this is indeed an isomorphism.
If this routine returns ``False`` then the result is inconclusive
(i.e., it might still be an isomorphism but Regina was not able to
prove this).

You probably only want to run this on good presentations for small
cancellation theory - an automorphism of a poorly-presented group
likely will not be noticed.

This routine is intended for sanity checking only: any homomorphism
that you construct as a declared isomorphism should always be an
isomorphism.

Precondition:
    This homomorphism is in fact a declared isomorphism. See the
    HomGroupPresentation class notes for details on what this means.

Returns:
    ``True`` if it is verified that this is an isomorphism, or
    ``False`` if the result is inconclusive.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

