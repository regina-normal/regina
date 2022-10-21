/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Tangle
static const char *Tangle =
R"doc(Represents a 2-tangle in the 3-ball. Regina does not allow closed
components in a tangle; in other words, a tangle in Regina is a proper
embedding of exactly two arcs in the 3-ball with the corresponding
four endpoints attached to four marked points on the 3-ball boundary.

Regina stores tangles as projections, with the four endpoints fixed at
the top-left, top-right, bottom-left and bottom-right corners of the
diagram.

Each tangles has a *type*, indicating how the four endpoints are
connected. The three possible types are:

* *horizontal*, indicating that the two top endpoints are connected,
  and the two bottom endpoints are connected;

* *vertical*, indicating that the two left endpoints are connected,
  and the two right endpoints are connected;

* *diagonal*, indicating that the top-left and bottom-right endpoints
  are connected, and the bottom-left and top-right endpoints are
  connected.

Internally, Regina numbers the two strings 0 and 1: string 0 will
always be the one attached to the top-left endpoint. Regina also
assigns each string an orientation: for a horizontal or diagonal
tangle this will always be from left to right, and for a vertical
tangle this will always be from top to bottom.

When traversing a tangle, if you reach one of the endpoints of a
string then the corresponding return value of Crossing::next() or
Crossing::prev() (whichever is relevant) will be a null strand
reference.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace Tangle_ {

// Docstring regina::python::doc::Tangle_::__copy
static const char *__copy =
R"doc(Constructs a new copy of the given tangle.

Parameter ``copy``:
    the tangle to copy.)doc";

// Docstring regina::python::doc::Tangle_::__default
static const char *__default =
R"doc(Constructs the zero tangle.

This is the horizontal tangle with no crossings.)doc";

// Docstring regina::python::doc::Tangle_::__eq
static const char *__eq =
R"doc(Determines if this tangle is combinatorially identical to the given
tangle.

Here "identical" means that:

* the tangles are of the same type and have the same number of
  crossings;

* the same numbered crossings are positive and negative in both
  tangles;

* the corresponding strings in each tangle pass through the same
  under/over-strands of the same numbered crossings in the same order.

Parameter ``other``:
    the tangle to compare with this.

Returns:
    ``True`` if and only if the two tangles are combinatorially
    identical.)doc";

// Docstring regina::python::doc::Tangle_::__init
static const char *__init =
R"doc(Constructs a tangle from the given number of twists.

If *twists* is positive, then the new tangle will consist of *twists*
positive twists, stacked from left to right. If *twists* is negative,
then the new tangle will consist of -(*twists*) negative twists,
likewise stacked from left to right. If *twists* is zero, then the new
tangle will be a horizontal tangle with no crossings at all.

In all cases, this is equivalent to calling the rational tangle
constructor Tangle(*twists*, 1).

Parameter ``twists``:
    the number of twists to perform; this may be positive, negative or
    zero.)doc";

// Docstring regina::python::doc::Tangle_::__init_2
static const char *__init_2 =
R"doc(Constructs a rational tangle with the given parameters. Here we use
the following convention (following the description that Adams gives
in _The Knot Book_):

* the zero tangle is horizontal with no crossings;

* the infinity tangle is vertical with no crossings;

* the +1 tangle is diagonal with one crossing, where the upper string
  runs from bottom-left to top-right.

Precondition:
    The given arguments are coprime.

Parameter ``num``:
    the numerator of the rational number that describes this tangle.

Parameter ``den``:
    the denominator of the rational number that describes this tangle;
    this may be 0 (representing the infinity tangle).)doc";

// Docstring regina::python::doc::Tangle_::__init_3
static const char *__init_3 =
R"doc(Creates a tangle from two parallel copies of the given knot.

Specifically, the tangle will consist of two parallel copies of the
given knot diagram, which will be broken just before the starting
strand as returned by ``knot.component(0)``.

The two resulting endpoints that appear just before the starting
strand will form the top-left and bottom-left endpoints of this
tangle, and the endpoints on the other side of the break (which will
be just after the parallel copies of the final strand
``knot.component(0).prev()``) will form the top-right and bottom-right
endpoints of this tangle.

The tangle will contain ``4 * knot.size()`` crossings in total.

Precondition:
    The argument contains exactly one component (i.e., it is actually
    a knot, and not empty or a multiple-component link).

Parameter ``knot``:
    the knot to break and duplicate to form this tangle.)doc";

// Docstring regina::python::doc::Tangle_::__ne
static const char *__ne =
R"doc(Determines if this tangle is not combinatorially identical to the
given tangle.

Here "identical" means that:

* the tangles are of the same type and have the same number of
  crossings;

* the same numbered crossings are positive and negative in both
  tangles;

* the corresponding strings in each tangle pass through the same
  under/over-strands of the same numbered crossings in the same order.

Parameter ``other``:
    the tangle to compare with this.

Returns:
    ``True`` if and only if the two tangles are not combinatorially
    identical.)doc";

// Docstring regina::python::doc::Tangle_::add
static const char *add =
R"doc(Adds the given tangle to the right-hand side of this tangle.

In Conway's notation, if this tangle is *t*, then this routine
converts this into (*t* + *other*).

Specifically: this routine will attach the two right-hand endpoints of
this tangle to the two left-hand endpoints of a copy of *other*.

This tangle will be changed directly. The tangle *other* (passed as
the argumet) will be left unchanged.

It is allowed to pass this tangle as *other*.

Precondition:
    It is not the case that both this and *other* are vertical tangles
    (which would cause the addition to create a closed link
    component).

Parameter ``other``:
    the tangle to add to this.)doc";

// Docstring regina::python::doc::Tangle_::begin
static const char *begin =
R"doc(Returns the crossing closest to the beginning of the given string.

Recall from the class notes that string 0 is always attached to the
top-left endpoint. Recall also that strings are oriented from left-to-
right for a horizontal or diagonal tangle, and from top-to-bottom for
a vertical tangle.

Parameter ``string``:
    indicates which of the two strings in this tangle to query; this
    must be either 0 or 1.

Returns:
    the crossing closest to the beginning of the given string, or a
    null reference if the given string contains no crossings.)doc";

// Docstring regina::python::doc::Tangle_::box
static const char *box =
R"doc(Encloses this tangle with the four given tangles in a box
configuration.

The five tangles will be connected as shown, with this tangle in the
centre:

```
 \     /
  O---O
 / \ / \
 |  O  |
 \ / \ /
  O---O
 /     \
```

The top-left corner of the argument *topLeft* will become the top-left
corner of the resulting tangle, and so on for the other three corners.

This tangle will be changed directly. The other four other tangles
(passed as arguments) will be left unchanged.

You may use the same tangle for multiple arguments, and you may even
use this tangle for one or more arguments.

Precondition:
    Every string in all five tangles (the four arguments and this) has
    at least one crossing.

Precondition:
    None of the five tangles (the four arguments and this) have types
    that would result in a closed link component after this operation
    is performed.

Parameter ``topLeft``:
    the tangle to connect to the top-left corner of this.

Parameter ``topRight``:
    the tangle to connect to the top-right corner of this.

Parameter ``bottomLeft``:
    the tangle to connect to the bottom-left corner of this.

Parameter ``bottomRight``:
    the tangle to connect to the bottom-right corner of this.)doc";

// Docstring regina::python::doc::Tangle_::brief
static const char *brief =
R"doc(Outputs this tangle in Regina's own brief write-only format. This
format is concise, but contains enough information to manually
reconstruct the complete tangle.

This format cannot (yet) be used to read tangles back into Regina, and
so it is not good for external storage, or for passing tangles between
different programs (or even different instances of Regina). It was
originally designed for use with the test suite, where it was used to
ensure that tangles with being created and/or manipulated correctly.

The output will contain the following elements, separated by single
spaces:

* one of the symbols ``-``, ``|`` or ``x``, indicating that the tangle
  is of horizontal, vertical or diagonal type respectively (as
  described in the class notes);

* a sequence of signs (``+`` or ``-``), concatenated together, giving
  the signs of the crossings in order from crossing 0 to crossing
  size()-1;

* a description of string 0 and then string 1. Each string will be
  written in the form ``( a b c ... )``, indicating the crossings that
  are encountered as we follow the string in the forward direction
  from its starting endpoint. Each element *a*, *b*, *c* and so on
  will be written in the format used by the StrandRef class: either
  ``^n`` when passing over crossing *n*, or ``_n`` when passing under
  crossing *n*.

For example, the rational tangle 3/2 as returned by Tangle(3,2) will
give the following brief output:

```
| --+ ( _0 ^1 ) ( ^2 _1 ^0 _2 )
```

As a special case, if the tangle contains no crossings then the output
will contain just one space, not two consecutive spaces, between the
type symbol and the string descriptions (since the sequence of
crossing signs that would normally sit between them will be empty).

The string will not end in a newline.

There is also a variant of brief() that writes directly to an output
stream.

Returns:
    a description of this tangle in Regina's brief format.)doc";

// Docstring regina::python::doc::Tangle_::changeAll
static const char *changeAll =
R"doc(Switches the upper and lower strands of every crossing in the tangle.

This operation corresponds to reflecting the tangle through the plane
on which the diagram is drawn.)doc";

// Docstring regina::python::doc::Tangle_::crossing
static const char *crossing =
R"doc(Returns a pointer to the crossing at the given index within this
tangle.

For a tangle with *n* crossings, the crossings are numbered from 0 to
*n*-1 inclusive.

.. warning::
    If some crossings are added or removed then the indices of other
    crossings might change. If you wish to track a particular crossing
    through such operations then you should use the pointer to the
    relevant Crossing object instead.

Parameter ``index``:
    the index of the requested crossing. This must be between 0 and
    size()-1 inclusive.

Returns:
    the crossing at the given index.)doc";

// Docstring regina::python::doc::Tangle_::crossings
static const char *crossings =
R"doc(Returns an object that allows iteration through and random access to
all crossings within this tangle.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Note that the elements of the list will be
pointers, so your code might look like:

```
for (Crossing* c : tangle.crossings()) { ... }
```

The object that is returned will remain up-to-date and valid for as
long as the tangle exists: even as crossings are added and/or removed,
it will always reflect the crossings that are currently in the tangle.
Nevertheless, it is recommended to treat this object as temporary
only, and to call crossings() again each time you need it.

Returns:
    access to the list of all crossings.)doc";

// Docstring regina::python::doc::Tangle_::denClosure
static const char *denClosure =
R"doc(Forms the denominator closure of this tangle.

This is the link created by joining the two left endpoints of this
tangle, and also joining the two right endpoints.

Returns:
    the denominator closure of this tangle.)doc";

// Docstring regina::python::doc::Tangle_::end
static const char *end =
R"doc(Returns the crossing closest to the end of the given string.

Recall from the class notes that string 0 is always attached to the
top-left endpoint. Recall also that strings are oriented from left-to-
right for a horizontal or diagonal tangle, and from top-to-bottom for
a vertical tangle.

Parameter ``string``:
    indicates which of the two strings in this tangle to query; this
    must be either 0 or 1.

Returns:
    the crossing closest to the end of the given string, or a null
    reference if the given string contains no crossings.)doc";

// Docstring regina::python::doc::Tangle_::fromOrientedGauss
static const char *fromOrientedGauss =
R"doc(Creates a new tangle from an oriented Gauss code.

Oriented Gauss codes for tangles are an extension of oriented Gauss
codes for knots. Whilst oriented Gauss codes for knots are used
elsewhere (they are based on a format used by Andreeva et al.), these
codes for tangles are specific to Regina (so you should not expect
other software to understand them).

The format works as follows:

* Label the crossings arbitrarily as 1, 2, ..., *n*.

* Write one of the tokens ``-``, ``|`` or ``x`` to represent a
  horizontal, vertical or diagonal tangle respectively.

* Start at the top-left endpoint and follow this string to its other
  endpoint. At every crossing that you pass, write a token of the form
  ``+<k``, ``-<k``, ``+>k`` or ``->k``, where:

* the symbol ``+`` indicates that you are passing over the crossing
  labelled *k*, and the symbol ``-`` indicates that you are passing
  under the crossing labelled *k*;

* the symbol ``<`` indicates that the other strand of the crossing
  passes from right to left, and ``>`` indicates that the other strand
  passes from left to right;

* *k* is replaced with the integer crossing label.

* Write the token ``_`` to indicate that the first string has
  finished.

* Start at the beginning of the other string (for horizontal or
  diagonal tangles, this is the bottom-left endpoint, and for vertical
  tangles this is the top-right endpoint). As before, follow this
  string to its other endpoint, writing a token of the form ``+<k``,
  ``-<k``, ``+>k`` or ``->k`` at every crossing that you pass.

Be aware that, once the tangle has been constructed, the crossings 1,
..., *n* will have been reindexed as 0, ..., *n*-1 (since every Tangle
object numbers its crossings starting from 0).

As an example, you can construct the rational tangle -3/4 using the
following code:

```
| -<1 +>2 -<3 +>4 _ -<5 -<4 +>3 -<2 +>1 +>5
```

There are two variants of this routine. This variant takes a single
string, where the tokens have been combined together and separated by
whitespace. The other variant takes a sequence of tokens, defined by a
pair of iterators.

In this variant (the string variant), the given string may contain
additional leading or trailing whitespace.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does *not* test for
    the viability of the diagram (i.e., whether the given crossings
    with the given signs actually produce a tangle of the given type
    with the correct endpoints). Of course non-viable inputs are not
    allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The input was not a valid oriented Gauss code. As noted above, the
    checks performed here are not exhaustive.

Parameter ``str``:
    an oriented Gauss code for a tangle, as described above.

Returns:
    the resulting tangle.)doc";

// Docstring regina::python::doc::Tangle_::fromOrientedGauss_2
static const char *fromOrientedGauss_2 =
R"doc(Creates a new tangle from an oriented Gauss code.

Oriented Gauss codes for tangles are an extension of oriented Gauss
codes for knots. Whilst oriented Gauss codes for knots are used
elsewhere (they are based on a format used by Andreeva et al.), these
codes for tangles are specific to Regina (so you should not expect
other software to understand them).

See fromOrientedGauss(const std::string&) for a detailed description
of this format as it is used in Regina.

There are two variants of this routine. The other variant
(fromOrientedGauss(const std::string&), which offers more detailed
documentation) takes a single string, where the tokens have been
combined together and separated by whitespace. This variant takes a
sequence of tokens, defined by a pair of iterators.

Precondition:
    *Iterator* is a random access iterator type.

Precondition:
    Dereferencing such an iterator produces either a C-style string
    (which can be cast to ``const char*``) or a C++-style string
    (which can be cast to ``const std::string&``).

Precondition:
    The tokens in the input sequence do not contain any whitespace.

.. warning::
    While this routine does some error checking on the input, these
    checks are not exhaustive. In particular, it does *not* test for
    the viability of the diagram (i.e., whether the given crossings
    with the given signs actually produce a tangle of the given type
    with the correct endpoints). Of course non-viable inputs are not
    allowed, and it is currently up to the user to enforce this.

Exception ``InvalidArgument``:
    The input did not describe a valid oriented Gauss code. As noted
    above, the checks performed here are not exhaustive.

Python:
    Instead of a pair of begin and past-the-end iterators, this
    routine takes a Python list of strings.

Parameter ``begin``:
    an iterator that points to the beginning of the sequence of tokens
    for an oriented Gauss code.

Parameter ``end``:
    an iterator that points past the end of the sequence of tokens for
    an oriented Gauss code.

Returns:
    the resulting tangle.)doc";

// Docstring regina::python::doc::Tangle_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given tangles.

This global routine simply calls Tangle::swap(); it is provided so
that Tangle meets the C++ Swappable requirements.

See Tangle::swap() for more details.

Parameter ``lhs``:
    the tangle whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the tangle whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::Tangle_::negate
static const char *negate =
R"doc(Reflects this tangle through the diagonal axis running from the top-
left to bottom-right corners of the diagram.

In Conway's notation, this negates the tangle.)doc";

// Docstring regina::python::doc::Tangle_::numClosure
static const char *numClosure =
R"doc(Forms the numerator closure of this tangle.

This is the link created by joining the two top endpoints of this
tangle, and also joining the two bottom endpoints.

Returns:
    the numerator closure of this tangle.)doc";

// Docstring regina::python::doc::Tangle_::orientedGauss
static const char *orientedGauss =
R"doc(Outputs an oriented Gauss code for this tangle.

Oriented Gauss codes for tangles are an extension of oriented Gauss
codes for knots. Whilst oriented Gauss codes for knots are used
elsewhere (they are based on a format used by Andreeva et al.), these
codes for tangles are specific to Regina (so you should not expect
other software to understand them).

For a full explanation of how oriented Gauss codes work for tangles,
see the documentation for fromOrientedGauss(const std::string&), which
imports tangles in this format.

The string that is returned will not contain any newlines.

.. note::
    There is another variant of this routine that, instead of
    returning a string, writes directly to an output stream.

Returns:
    an oriented Gauss code for this tangle.)doc";

// Docstring regina::python::doc::Tangle_::r1
static const char *r1 =
R"doc(Tests for and/or performs a type I Reidemeister move to remove a
crossing.

Unlike links, which implement the full suite of Reidemeister moves,
tangles (at present) only offer the simplifying versions of
Reidemeister moves I and II.

The behaviour of this routine is identical to the r1() routine in the
Link class; see Link::r1() for further details.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    The given crossing is either a null pointer, or else some crossing
    in this tangle.

Parameter ``crossing``:
    identifies the crossing to be removed.

Parameter ``check``:
    ``True`` if we are to check whether the move can be performed at
    the given location.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the move can be performed. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Tangle_::r2
static const char *r2 =
R"doc(Tests for and/or performs a type II Reidemeister move to remove two
crossings.

Unlike links, which implement the full suite of Reidemeister moves,
tangles (at present) only offer the simplifying versions of
Reidemeister moves I and II.

The behaviour of this routine is identical to the r2() routine in the
Link class; see Link::r2() for further details.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    The given strand reference is either a null reference, or else
    refers to some strand of some crossing in this tangle.

Parameter ``arc``:
    identifies one of the arcs of the bigon about which the move will
    be performed.

Parameter ``check``:
    ``True`` if we are to check whether the move is legal.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the requested move is legal. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Tangle_::r2_2
static const char *r2_2 =
R"doc(Tests for and/or performs a type II Reidemeister move to remove two
crossings.

Unlike links, which implement the full suite of Reidemeister moves,
tangles (at present) only offer the simplifying versions of
Reidemeister moves I and II.

The behaviour of this routine is identical to the r2() routine in the
Link class; see Link::r2() for further details.

Precondition:
    If *perform* is ``True`` but *check* is ``False``, then it must be
    known in advance that this move can be performed at the given
    location.

Precondition:
    The given crossing is either a null pointer, or else some crossing
    in this tangle.

Parameter ``crossing``:
    identifies the crossing at the beginning of the "upper" arc that
    features in this move.

Parameter ``check``:
    ``True`` if we are to check whether the move is legal.

Parameter ``perform``:
    ``True`` if we should actually perform the move.

Returns:
    If *check* is ``True``, this function returns ``True`` if and only
    if the requested move is legal. If *check* is ``False``, this
    function always returns ``True``.)doc";

// Docstring regina::python::doc::Tangle_::simplifyToLocalMinimum
static const char *simplifyToLocalMinimum =
R"doc(Uses type I and II Reidemeister moves to reduce the tangle
monotonically to some local minimum number of crossings. Type III
Reidemeister moves (which do not reduce the number of crossings) are
not used in this routine.

Unlike links, tangle do not (at present) offer stronger simplification
routines (such as the much better Link::intelligentSimplify() and
Link::simplifyExhaustive()).

.. warning::
    The implementation of this routine (and therefore its results) may
    change between different releases of Regina.

Parameter ``perform``:
    ``True`` if we are to perform the simplifications, or ``False`` if
    we are only to investigate whether simplifications are possible
    (defaults to ``True``).

Returns:
    if *perform* is ``True``, this routine returns ``True`` if and
    only if the link was changed to reduce the number of crossings; if
    *perform* is ``False``, this routine returns ``True`` if and only
    if it determines that it is capable of performing such a change.)doc";

// Docstring regina::python::doc::Tangle_::size
static const char *size =
R"doc(Returns the number of crossings in this tangle.

Returns:
    the number of crossings.)doc";

// Docstring regina::python::doc::Tangle_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given tangle.

All crossings that belong to this tangle will be moved to *other*, and
all crossings that belong to *other* will be moved to this tangle.
Likewise, all cached properties will be swapped.

In particular, any Crossing pointers or references and any StrandRef
objects will remain valid.

This routine will behave correctly if *other* is in fact this tangle.

Parameter ``other``:
    the tangle whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Tangle_::translate
static const char *translate =
R"doc(Translates a strand reference from some other tangle into the
corresponding strand reference from this tangle.

Typically this routine would be used when the given strand comes from
a tangle that is combinatorially identical to this, and you wish to
obtain the corresponding strand in this tangle.

Specifically: if *other* refers to some strand (upper or lower) of
crossing number *k* of some other tangle, then the return value will
refer to the same strand (upper or lower) of crossing number *k* of
this tangle.

This routine behaves correctly even if *other* is a null reference.

Precondition:
    This tangle contains at least as many crossings as the tangle
    containing *other* (though, as noted above, in typical scenarios
    both tangles would actually be combinatorially identical).

Parameter ``other``:
    the strand reference to translate.

Returns:
    the corresponding strand reference for this tangle.)doc";

// Docstring regina::python::doc::Tangle_::turn
static const char *turn =
R"doc(Rotates this tangle by 90 degrees.

Parameter ``direction``:
    either 1 if the tangle should be rotated clockwise, or -1 if the
    tangle should be rotated anticlockwise.)doc";

// Docstring regina::python::doc::Tangle_::twist
static const char *twist =
R"doc(Adds a twist to the right-hand end of this tangle.

Parameter ``sign``:
    either 1 if we should perform a positive twist (dragging the
    bottom-right endpoint up over the top-right endpoint), or -1 if we
    should perform a negative twist (dragging the bottom-right
    endpoint up beneath the top-right endpoint).)doc";

// Docstring regina::python::doc::Tangle_::type
static const char *type =
R"doc(Returns the type of this tangle.

This will be one of the characters ``-``, ``|`` or ``x``, indicating a
horizontal, vertical or diagonal type as described in the class notes.

Returns:
    the type of this crossing.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

