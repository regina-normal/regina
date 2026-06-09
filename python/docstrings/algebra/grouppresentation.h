/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct GroupExpression {

// Docstring regina::python::doc::GroupExpression::__class
static constexpr const char __class[] =
R"doc(Represents an expression involving generators from a group
presentation or a free group. An expression is represented as word,
i.e, a sequence of powers of generators all of which are multiplied in
order. Each power of a generator corresponds to an individual
GroupExpressionTerm.

For instance, the expression ``g1^2 g3^-1 g6`` contains the three
terms ``g1^2``, ``g3^-1`` and ``g6^1`` in that order.

Note that generators are indexed starting from 0 (so, for example,
``g3`` represents the _fourth_ generator in the group presentation,
not the third).

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::GroupExpression::__copy
static constexpr const char __copy[] = R"doc(Creates a new copy of the given expression.)doc";

// Docstring regina::python::doc::GroupExpression::__default
static constexpr const char __default[] = R"doc(Creates a new expression with no terms.)doc";

// Docstring regina::python::doc::GroupExpression::__eq
static constexpr const char __eq[] =
R"doc(Tests whether this and the given expression contain the same terms in
the same order.

Parameter ``other``:
    the expression to compare with this.

Returns:
    ``True`` if and only if this and the given expression are
    identical.)doc";

// Docstring regina::python::doc::GroupExpression::__init
static constexpr const char __init[] =
R"doc(Creates a new expression containing a single term.

Parameter ``term``:
    the term to use as the new expression.)doc";

// Docstring regina::python::doc::GroupExpression::__init_2
static constexpr const char __init_2[] =
R"doc(Creates a new expression containing a single term.

Parameter ``generator``:
    the number of the generator to use in the term.

Parameter ``exponent``:
    the exponent to which the given generator is raised in the term.)doc";

// Docstring regina::python::doc::GroupExpression::__init_3
static constexpr const char __init_3[] =
R"doc(Attempts to interpret the given input string as a word in a group.
Regina can recognise strings in the following four basic forms:

* ``a^7b^-2``

* ``aaaaaaaBB``

* ``a^7B^2``

* ``g0^7g1^-2``

The string may contain whitespace, which will simply be ignored. The
empty string will be treated as an expression with no terms.

Note that generators are numbered starting from 0. This means, for
example, that ``a``, ``b`` and ``c`` correspond to ``g0``, ``g1`` and
``g2`` respectively.

If the optional argument *nGens* is passed and is positive, then this
constructor will explicitly check that the given string only uses
generators 0,...,(*nGens*-1).

Exception ``InvalidArgument``:
    The given string could not be interpreted as a group expression,
    or else *nGens* was positive and the given string contains an out-
    of-range generator.

Parameter ``word``:
    the input string that is to be interpreted as a word.

Parameter ``nGens``:
    the number of generators in the group presentation. If this is 0
    (the default), then this argument will be ignored and this
    constructor will not check whether generators are within range.)doc";

// Docstring regina::python::doc::GroupExpression::__init_4
static constexpr const char __init_4[] =
R"doc(Attempts to interpret the given input string as a word in a group.
Regina can recognise strings in the following four basic forms:

* ``a^7b^-2``

* ``aaaaaaaBB``

* ``a^7B^2``

* ``g0^7g1^-2``

The string may contain whitespace, which will simply be ignored. The
empty string will be treated as an expression with no terms.

Note that generators are numbered starting from 0. This means, for
example, that ``a``, ``b`` and ``c`` correspond to ``g0``, ``g1`` and
``g2`` respectively.

If the optional argument *nGens* is passed and is positive, then this
constructor will explicitly check that the given string only uses
generators 0,...,(*nGens*-1).

Exception ``InvalidArgument``:
    The given string could not be interpreted as a group expression,
    or else *nGens* was positive and the given string contains an out-
    of-range generator.

Parameter ``word``:
    the input string that is to be interpreted as a word.

Parameter ``nGens``:
    the number of generators in the group presentation. If this is 0
    (the default), then this argument will be ignored and this
    constructor will not check whether generators are within range.)doc";

// Docstring regina::python::doc::GroupExpression::addTermFirst
static constexpr const char addTermFirst[] =
R"doc(Adds the given term to the beginning of this expression.

Parameter ``term``:
    the term to add.)doc";

// Docstring regina::python::doc::GroupExpression::addTermFirst_2
static constexpr const char addTermFirst_2[] =
R"doc(Adds the given term to the beginning of this expression.

Parameter ``generator``:
    the number of the generator corresponding to the new term.

Parameter ``exponent``:
    the exponent to which the given generator is raised.)doc";

// Docstring regina::python::doc::GroupExpression::addTermLast
static constexpr const char addTermLast[] =
R"doc(Adds the given term to the end of this expression.

Parameter ``term``:
    the term to add.)doc";

// Docstring regina::python::doc::GroupExpression::addTermLast_2
static constexpr const char addTermLast_2[] =
R"doc(Adds the given term to the end of this expression.

Parameter ``generator``:
    the number of the generator corresponding to the new term.

Parameter ``exponent``:
    the exponent to which the given generator is raised.)doc";

// Docstring regina::python::doc::GroupExpression::addTermsFirst
static constexpr const char addTermsFirst[] =
R"doc(Multiplies this expression on the left by the given word. This
expression will be modified directly.

Parameter ``word``:
    the word to multiply with this expression.)doc";

// Docstring regina::python::doc::GroupExpression::addTermsLast
static constexpr const char addTermsLast[] =
R"doc(Multiplies this expression on the right by the given word. This
expression will be modified directly.

Parameter ``word``:
    the word to multiply with this expression.)doc";

// Docstring regina::python::doc::GroupExpression::clear
static constexpr const char clear[] =
R"doc(Erases all terms from this word. This effectively turns this word into
the identity element.)doc";

// Docstring regina::python::doc::GroupExpression::countTerms
static constexpr const char countTerms[] =
R"doc(Returns the number of terms in this expression.

For instance, the expression ``g1^2 g3^-1 g6`` contains three terms.
See also wordLength().

Returns:
    the number of terms.)doc";

// Docstring regina::python::doc::GroupExpression::cycleLeft
static constexpr const char cycleLeft[] =
R"doc(Cycles this word by moving the rightmost term around to the leftmost.
All other terms shift one step to the right.

If the word is of the form ``g_i1^j1 g_i2^j2 ... g_in^jn``, this
converts it into the word ``g_in^jn g_i1^j1 g_i1^j1 ... g_in-1^jn-1``.)doc";

// Docstring regina::python::doc::GroupExpression::cycleRight
static constexpr const char cycleRight[] =
R"doc(Cycles this word by moving the leftmost term around to the rightmost.
All other terms shift one step to the left.

If the word is of the form ``g_i1^j1 g_i2^j2 ... g_in^jn``, this
converts it into the word ``g_i2^j2 ... g_in^jn g_i1^j1``.)doc";

// Docstring regina::python::doc::GroupExpression::empty
static constexpr const char empty[] =
R"doc(Tests whether this is the empty (unit) word.

No attempt is made to remove redundant terms (so the word ``g g^-1``
will be treated as non-empty).

Returns:
    ``True`` if and only if this is the empty word.)doc";

// Docstring regina::python::doc::GroupExpression::erase
static constexpr const char erase[] =
R"doc(Deprecated function that erases all terms from this word.

.. deprecated::
    This routine has been renamed to clear(), for consistency with the
    standard C++ container interface. See clear() for further details.)doc";

// Docstring regina::python::doc::GroupExpression::exponent
static constexpr const char exponent[] =
R"doc(Returns the exponent corresonding to the term at the given index in
this expression. Index 0 represents the first term, index 1 represents
the second term and so on.

.. warning::
    This routine is O(*n*) where *n* is the number of terms in this
    expression.

Parameter ``index``:
    the index of the term to return; this must be between 0 and
    countTerms()-1 inclusive.

Returns:
    the requested exponent.)doc";

// Docstring regina::python::doc::GroupExpression::generator
static constexpr const char generator[] =
R"doc(Returns the generator corresonding to the term at the given index in
this expression. Index 0 represents the first term, index 1 represents
the second term and so on.

.. warning::
    This routine is O(*n*) where *n* is the number of terms in this
    expression.

Parameter ``index``:
    the index of the term to return; this must be between 0 and
    countTerms()-1 inclusive.

Returns:
    the number of the requested generator.)doc";

// Docstring regina::python::doc::GroupExpression::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the two given expressions.

This global routine simply calls GroupExpression::swap(); it is
provided so that GroupExpression meets the C++ Swappable requirements.

Parameter ``lhs``:
    the expression whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the expression whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::GroupExpression::inverse
static constexpr const char inverse[] =
R"doc(Returns the inverse of this expression. The terms will be reversed and
the exponents negated.

Returns:
    the inverse of this expression.)doc";

// Docstring regina::python::doc::GroupExpression::invert
static constexpr const char invert[] =
R"doc(Inverts this expression in-place. The terms will be reversed and the
exponents negated.)doc";

// Docstring regina::python::doc::GroupExpression::isTrivial
static constexpr const char isTrivial[] =
R"doc(Deprecated function that tests whether this is the empty (unit) word.

.. deprecated::
    This routine has been renamed to empty(), for consistency with the
    standard C++ container interface. See empty() for further details.

Returns:
    ``True`` if and only if this is the empty word.)doc";

// Docstring regina::python::doc::GroupExpression::power
static constexpr const char power[] =
R"doc(Returns this expression raised to the given power. The given exponent
may be positive, zero or negative.

Parameter ``exponent``:
    the power to which this expression should be raised.

Returns:
    this expression raised to the given power.)doc";

// Docstring regina::python::doc::GroupExpression::simplify
static constexpr const char simplify[] =
R"doc(Simplifies this expression. Adjacent powers of the same generator will
be combined, and terms with an exponent of zero will be removed. Note
that it is _not_ assumed that the underlying group is abelian.

You may declare that the expression is cyclic, in which case it is
assumed that terms may be moved from the back to the front and vice
versa. Thus expression ``g1 g2 g1 g2 g1`` simplifies to ``g1^2 g2 g1
g2`` if it is cyclic, but does not simplify at all if it is not
cyclic.

Parameter ``cyclic``:
    ``True`` if and only if the expression may be assumed to be
    cyclic.

Returns:
    ``True`` if and only if this expression was changed.)doc";

// Docstring regina::python::doc::GroupExpression::str
static constexpr const char str[] =
R"doc(Returns a short text representation of this group expression, with a
choice of either numbered generators or alphabetic generators.

If *alpha* is ``False``, the text representation will be of the form
``g2^4 g13^-5 g4``. If *alpha* is ``True``, this routine will assume
your word is in an alphabet of no more than 26 letters, and will
format the word using lower-case ASCII, i.e., ``c^4 n^-5 e``.

Note that there is also a zero-argument version of str(), inherited
through the ShortOutput base class. This zero-argument str() gives the
same output as ``str(false)``.

Note that generators are numbered starting from 0. This means, for
example, that ``a``, ``b`` and ``c`` correspond to ``g0``, ``g1`` and
``g2`` respectively.

Precondition:
    If *alpha* is ``True``, the number of generators in the
    corresponding group must be 26 or fewer.

Parameter ``alpha``:
    indicates whether to use numbered or alphabetic generators, as
    described above.

Returns:
    a short text representation of this group expression.)doc";

// Docstring regina::python::doc::GroupExpression::substitute
static constexpr const char substitute[] =
R"doc(Replaces every occurrence of the given generator with the given
replacement word. As expected, inverses of the given generator will be
replaced with inverses of the given word. If the given generator or
its inverse _was_ found, the expression will be simplified once the
substitution is complete.

Precondition:
    The argument *replacement* is not the same GroupExpression object
    as this.

Parameter ``generator``:
    the generator to be replaced.

Parameter ``replacement``:
    the substitute expression that will replace every occurrence of
    the given generator.

Parameter ``cyclic``:
    ``True`` if and only if the expression may be assumed to be
    cyclic; this is only relevant during the simplification stage.

Returns:
    ``True`` if and only if any substitutions were made.)doc";

// Docstring regina::python::doc::GroupExpression::substitute_2
static constexpr const char substitute_2[] =
R"doc(Replaces every generator in this expression with the corresponding
substitute expression from the given map.

Specifically, each generator *i* will be replaced with the expression
``replacements[i]``.

The expression will be simplified once all substitutions are complete.

Unlike the single-generator verison of substitute(), it is perfectly
fine if this GroupExpression object appears in the *replacements*
list, and/or if the same GroupExpression object appears several times
in the given list.

Precondition:
    The length of *replacements* is at least *g*+1, where *g* is the
    largest generator that appears in this expression. In other words,
    ``replacements[i]`` exists for every generator *i* that appears in
    this expression.

Parameter ``replacements``:
    the list of substitutes for all generators in this expression.

Parameter ``cyclic``:
    ``True`` if and only if the expression may be assumed to be
    cyclic; see simplify() for further details.)doc";

// Docstring regina::python::doc::GroupExpression::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given expression.

Parameter ``other``:
    the expression whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::GroupExpression::term
static constexpr const char term[] =
R"doc(Returns the term at the given index in this expression. Index 0
represents the first term, index 1 represents the second term and so
on.

.. warning::
    This routine is O(*n*) where *n* is the number of terms in this
    expression.

Parameter ``index``:
    the index of the term to return; this must be between 0 and
    countTerms()-1 inclusive.

Returns:
    the requested term.)doc";

// Docstring regina::python::doc::GroupExpression::term_2
static constexpr const char term_2[] =
R"doc(Returns a constant reference to the term at the given index in this
expression. Index 0 represents the first term, index 1 represents the
second term and so on.

.. warning::
    This routine is O(*n*) where *n* is the number of terms in this
    expression.

Parameter ``index``:
    the index of the term to return; this must be between 0 and
    countTerms()-1 inclusive.

Returns:
    the requested term.)doc";

// Docstring regina::python::doc::GroupExpression::terms
static constexpr const char terms[] =
R"doc(Returns the list of terms in this expression. These are the actual
terms stored internally; any modifications made to this list will show
up in the expression itself.

For instance, the expression ``g1^2 g3^-1 g6`` has list consisting of
three terms ``g1^2``, ``g3^-1`` and ``g6^1`` in that order.

Python:
    The list itself is not returned by reference (instead this routine
    returns a new Python list). However, the terms within this list
    are still returned by reference (i.e., you can use the elements of
    this list to modify each term individually).

Returns:
    the list of terms.)doc";

// Docstring regina::python::doc::GroupExpression::tex
static constexpr const char tex[] =
R"doc(Returns a TeX representation of this expression.

The text representation will be of the form ``g_2^4 g_{13}^{-5} g_4``.

Returns:
    a TeX representation of this expression.)doc";

// Docstring regina::python::doc::GroupExpression::utf8
static constexpr const char utf8[] =
R"doc(Returns a short text representation of this group expression using
unicode characters, with a choice of either numbered generators or
alphabetic generators.

This outputs a similar text representation to str(bool), except that
all exponents will be written using superscript characters encoded in
UTF-8. See str(bool) for further details.

Note that there is also a zero-argument version of utf8(), inherited
through the ShortOutput base class. This zero-argument utf8() gives
the same output as ``utf8(false)``.

Precondition:
    If *alpha* is ``True``, the number of generators in the
    corresponding group must be 26 or fewer.

Parameter ``alpha``:
    indicates whether to use numbered or alphabetic generators, as
    described above.

Returns:
    a short text representation of this group expression.)doc";

// Docstring regina::python::doc::GroupExpression::wordLength
static constexpr const char wordLength[] =
R"doc(Returns the length of the word, i.e. the number of letters with
exponent +1 or -1 for which this word is expressable as a product.

For instance, the expression ``g1^2 g3^-1 g6`` is a word of length
four. See also countTerms().

No attempt is made to remove redundant terms (so the word ``g g^-1``
will count as length two).

Returns:
    the length of the word.)doc";

// Docstring regina::python::doc::GroupExpression::writeXMLData
static constexpr const char writeXMLData[] =
R"doc(Writes a chunk of XML containing this expression.

Python:
    The argument *out* should be an open Python file object.

Parameter ``out``:
    the output stream to which the XML should be written.)doc";

}; // struct GroupExpression

struct GroupExpressionTerm {

// Docstring regina::python::doc::GroupExpressionTerm::__class
static constexpr const char __class[] =
R"doc(Represents a power of a generator in a group presentation.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

// Docstring regina::python::doc::GroupExpressionTerm::__cmp
static constexpr const char __cmp[] =
R"doc(Compares two terms lexicographically. Specifically, this operator
imposes a total order on terms by comparing them lexicographically as
(generator, exponent) pairs.

This generates all of the usual comparison operators, including ``<``,
``<=``, ``>``, and ``>=``.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Returns:
    the result of the lexicographical comparison between this and the
    given term.)doc";

// Docstring regina::python::doc::GroupExpressionTerm::__copy
static constexpr const char __copy[] = R"doc(Creates a new term initialised to the given value.)doc";

// Docstring regina::python::doc::GroupExpressionTerm::__default
static constexpr const char __default[] = R"doc(Creates a new uninitialised term.)doc";

// Docstring regina::python::doc::GroupExpressionTerm::__eq
static constexpr const char __eq[] =
R"doc(Determines whether this and the given term contain identical data.

Returns:
    ``True`` if and only if this and the given term have both the same
    generator and exponent.)doc";

// Docstring regina::python::doc::GroupExpressionTerm::__iadd
static constexpr const char __iadd[] =
R"doc(Attempts to merge this term with the given term. If both terms have
the same generator, the two exponents will be added and stored in this
term. If the generators are different, this routine will do nothing.

Note that this term might be changed but the given term will remain
unchanged.

Parameter ``other``:
    the term to merge with this term.

Returns:
    ``True`` if the two terms were merged into this term, or ``False``
    if the two terms have different generators.)doc";

// Docstring regina::python::doc::GroupExpressionTerm::__init
static constexpr const char __init[] =
R"doc(Creates a new term initialised to the given value.

Parameter ``gen``:
    the number that identifies the generator in the new term.

Parameter ``exp``:
    the exponent to which this generator is raised.)doc";

// Docstring regina::python::doc::GroupExpressionTerm::inverse
static constexpr const char inverse[] =
R"doc(Returns the inverse of this term. The inverse has the same generator
but a negated exponent.

Note that this term will remain unchanged.

Returns:
    the inverse of this term.)doc";

}; // struct GroupExpressionTerm

struct GroupPresentation {

// Docstring regina::python::doc::GroupPresentation::__class
static constexpr const char __class[] =
R"doc(Represents a finite presentation of a group.

A presentation consists of a number of generators and a set of
relations between these generators that together define the group.

If there are *g* generators, they will be numbered 0, 1, ..., *g*-1.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::GroupPresentation::__copy
static constexpr const char __copy[] =
R"doc(Creates a clone of the given group presentation.

Parameter ``src``:
    the group presentation to clone.)doc";

// Docstring regina::python::doc::GroupPresentation::__default
static constexpr const char __default[] = R"doc(Creates a new presentation with no generators and no relations.)doc";

// Docstring regina::python::doc::GroupPresentation::__eq
static constexpr const char __eq[] =
R"doc(Determines whether this and the given group presentation are
identical.

This routine does _not_ test for isomorphism (which in general is an
undecidable problem). Instead it tests whether this and the given
presentation use exactly the same generators and exactly the same
relations, presented in exactly the same order.

Parameter ``other``:
    the group presentation to compare with this.

Returns:
    ``True`` if and only if this and the given group presentation are
    identical.)doc";

// Docstring regina::python::doc::GroupPresentation::__init
static constexpr const char __init[] =
R"doc(Creates the free group on the given number of generators.

Parameter ``nGenerators``:
    the number of generators.)doc";

// Docstring regina::python::doc::GroupPresentation::__init_2
static constexpr const char __init_2[] =
R"doc(Constructor that allows you to directly pass an arbitrary number of
relators in string format.

The first argument *nGens* is the number of generators one wants the
group to have. The second argument *rels* is a vector of strings,
where each string gives a single relator. See the
GroupExpression::GroupExpression(const std::string&) constructor notes
for information on what format these strings can take.

If you wish to create a group presentation from a hard-coded list of
relations, you can use this constructor with an initialiser list, via
syntax of the form ``GroupPresentation(nGens, { "rel1", "rel2", ...
})``.

Exception ``InvalidArgument``:
    One or more of the given strings could not be interpreted as a
    group expression, and/or contains an out-of-range generator.

Parameter ``nGens``:
    the number of generators.

Parameter ``rels``:
    a vector of relations each given in string form, as outlined
    above.)doc";

// Docstring regina::python::doc::GroupPresentation::abelianRank
static constexpr const char abelianRank[] =
R"doc(Computes the rank of the abelianisation of this group. This is the
number of *Z* summands in the abelianisation (i.e., ignoring any
torsion summands).

This is much less informative than computing the full abelianisation,
but in some cases it might be significantly faster (since it involves
just a matrix rank computation as opposed to a Smith normal form).

The result of this routine should be the same as the output of
``abelianisation().rank()``.

Returns:
    the rank of the abelianisation of this group.)doc";

// Docstring regina::python::doc::GroupPresentation::abelianisation
static constexpr const char abelianisation[] =
R"doc(Computes the abelianisation of this group.

Returns:
    the abelianisation of this group.)doc";

// Docstring regina::python::doc::GroupPresentation::addGenerator
static constexpr const char addGenerator[] =
R"doc(Adds one or more generators to the group presentation. If the new
presentation has *g* generators, the new generators will be numbered
*g*-1, *g*-2 and so on.

Parameter ``numToAdd``:
    the number of generators to add.

Returns:
    the number of generators in the new presentation.)doc";

// Docstring regina::python::doc::GroupPresentation::addRelation
static constexpr const char addRelation[] =
R"doc(Adds the given relation to the group presentation. The relation must
be of the form ``expression = 1``.

.. warning::
    This routine does not check whether or not your relation is a word
    only in the generators of this group. In other words, it does not
    stop you from using generators beyond the countGenerators() bound.

Parameter ``rel``:
    the expression that the relation sets to 1; for instance, if the
    relation is ``g1^2 g2 = 1`` then this parameter should be the
    expression ``g1^2 g2``.)doc";

// Docstring regina::python::doc::GroupPresentation::compact
static constexpr const char compact[] =
R"doc(Returns a compact one-line representation of this group presentation,
including details of all generators and relations.

The output will be of the form ``< generators | relators >``. The full
relations will be included, and the entire output will be written on a
single line. There will be no final newline.

Currently str() and compact() are identical functions, though the
output from str() may change in future versions of Regina.

Returns:
    a compact representation of this group presentation.)doc";

// Docstring regina::python::doc::GroupPresentation::countGenerators
static constexpr const char countGenerators[] =
R"doc(Returns the number of generators in this group presentation.

Returns:
    the number of generators.)doc";

// Docstring regina::python::doc::GroupPresentation::countRelations
static constexpr const char countRelations[] =
R"doc(Returns the number of relations in this group presentation.

Returns:
    the number of relations.)doc";

// Docstring regina::python::doc::GroupPresentation::enumerateCovers
static constexpr const char enumerateCovers[] =
R"doc(Enumerates all transitive representations of this group into the
symmetric group *S(k)*. Each representation is produced exactly once
up to conjugacy.

Each such representation corresponds to an index *k* subgroup, and the
multiset of the abelianisations of all these subgroups is a group
invariant that (for small enough *k*) can be computed in reasonable
time.

If this is the fundamental group of a manifold, then each such
representation also corresponds to a connected *k*-sheeted cover.

For each representation that is produced, this routine will call
*action* (which must be a function or some other callable type).

* The first argument to *action* must be a group presentation. This
  will be the index *k* subgroup corresponding to the representation.
  This argument will be passed as an rvalue; a typical action could
  (for example) take it by const reference and query it, or take it by
  value and modify it, or take it by rvalue reference and move it into
  more permanent storage.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* The return value of *action* will be ignored; typically it would
  return ``void``.

* It is completely safe for *action* to (if you wish) make changes to
  the original presentation (i.e., the group presentation upon which
  enumerateCovers() is being called). This will not interfere with the
  enumeration or change the results in any way.

This routine produces a constant stream of output (i.e., it calls
*action* as soon as each representation is found).

.. warning::
    The running time is ``(k!)^g``, where *k* is the subgroup index
    described above, and *g* is the number of generators of this group
    presentation. In particular, the running time grows _extremely_
    quickly with *k*. Moreover, for larger indices this routine may
    precompute some tables that will never be released (but which only
    need to be computed the first time that index is used); for index
    10 these tables will consume roughly 30MB, and for index 11 they
    will consume around 320MB.

.. warning::
    This routine does _not_ simplify the group presentation before it
    runs. You should make sure that you have simplified the
    presentation, either using Regina or some other tool, before
    running this routine, since (as noted above) the running time is
    exponential in the number of generators.

.. warning::
    Likewise, this routine does not simplify the subgroup
    presentations before passing them to *action*. These presentations
    can be quite large, and (for example) if all you care about is
    their abelianisations then you are better off using the _abelian_
    group simplification / computation instead (which is much faster).

Precondition:
    Arrays on this system can be large enough to store ``2⋅(n-2)!``
    objects. This is a technical condition on the bit-size of
    ``size_t`` that will be explicitly checked (with an exception
    thrown if it fails). On a 64-bit system this condition will be
    true for all supported *n*, but on a 32-bit system or smaller it
    will mean that enumerateCovers() cannot be used for larger values
    of *n*.

Exception ``FailedPrecondition``:
    A signed integer of the same bit-size as ``size_t`` cannot hold
    ``2⋅(n-2)!``. See the precondition above for further discussion on
    this constraint.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.

Python:
    There are two versions of this function available in Python. The
    first form is ``enumerateCovers(index, action)``, which mirrors
    the C++ function: it takes *action* which may be a pure Python
    function, it returns the number of representations found, but it
    does _not_ take an addition argument list (*args*). The second
    form is ``enumerateCovers(index)``, which returns a Python list
    containing all of the corresponding subgroups, each given as a
    GroupPresentation. In both forms, the template parameter *index*
    becomes the first argument to the Python function.

Template parameter ``index``:
    the number *k* in the description above; in other words, the index
    of the resulting subgroups. Currently we limit this to the range
    ``2 ≤ k ≤ 11`` because some of the cached precomputations can
    consume a _lot_ of space for larger indices.

Parameter ``action``:
    a function (or other callable type) to call for each
    representation that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial subgroup presentation argument.

Returns:
    the total number of representations found.)doc";

// Docstring regina::python::doc::GroupPresentation::gap
static constexpr const char gap[] =
R"doc(Returns a sequence of GAP commands that create this group.

GAP is a widely-used computational algebra system, and can be
downloaded from http://gap-system.org/ .

Other than the variable for the group itself, the commands returned
will not use or modify any other GAP variables with the current GAP
scope.

The string that is returned will be presented as a single (possibly
very long) GAP function call, and will not contain any newlines.

Parameter ``groupVariable``:
    the name of the GAP variable to which this group will be assigned.

Returns:
    a sequence of commands to create this group in GAP.)doc";

// Docstring regina::python::doc::GroupPresentation::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the two given group presentations.

This global routine simply calls GroupPresentation::swap(); it is
provided so that GroupPresentation meets the C++ Swappable
requirements.

Parameter ``lhs``:
    the presentation whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the presentation whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::GroupPresentation::homologicalAlignment
static constexpr const char homologicalAlignment[] =
R"doc(Rewrites the presentation so that generators of the group map to
generators of the abelianisation, with any left-over generators
mapping to zero (if possible). Consider this a _homological-alignment_
of the presentation.

If the abelianisation of this group has rank *N* and *M* invariant
factors ``d0 | d2 | ... | d(M-1)``, this routine applies Nielsen moves
to the presentation to ensure that under the markedAbelianisation()
routine, generators 0 through *M*-1 are mapped to generators of the
relevant ``Z_di`` group. Similarly, generators *M* through *M*+*N*-1
are mapped to ±1 in the appropriate factor. All further generators
will be mapped to zero.

If this routine does return a homomorphism (because the presentation
was changed), then this homomorphsm will in fact be a declared
isomorphism. See the HomGroupPresentation class notes for details on
what this means.

.. note::
    If you all care about is whether the presentation changed, you can
    simply cast the return value to a ``bool``. This will then mirror
    the behaviour of homologicalAlignment() from Regina 6.0 and
    earlier, when the return type was simply ``bool``.

Exception ``UnsolvedCase``:
    It was not possible to rewrite the presentation as described, due
    to an integer overflow. This can (in theory) occur because
    AbelianGroup uses arbitrary precision integers for coefficients
    and invariant factors, whereas GroupPresentation uses native C++
    long integers for exponents in relations and other group
    expressions. Be warned that, if this exception is thrown, the
    presentation might have already been rewritten in some way (it
    should still be a correct presentation of the group, just not the
    one we are aiming for).

Returns:
    an isomorphism describing the reduction map from the original
    presentation to the new presentation, or ``None`` if this
    presentation was not changed.)doc";

// Docstring regina::python::doc::GroupPresentation::identifyAbelian
static constexpr const char identifyAbelian[] =
R"doc(Attempts to determine if the group is abelian.

A return value of ``True`` indicates that this routine successfully
certified that the group is abelian. A return value of ``False``
indicates an inconclusive result: either the group is non-abelian, or
the group is abelian but this routine could not prove so.

If the group is abelian, then markedAbelianization() is the easiest
way to see precisely which abelian group it is, and how the generators
sit in that group.

You will have better results from this algorithm if the presentation
has been simplified, since this algorithm uses small cancellation
theory in an attempt to reduce the commutators of all pairs of
generators.

.. warning::
    If you have not adequately simplified this presentation this
    routine will most likely return ``False``. Consider running
    simplify(), possibly in concert with proliferateRelators(), in
    order to discover adequately many commutators.

Returns:
    ``True`` if the group is shown to be abelian, or ``False`` if the
    result is inconclusive.)doc";

// Docstring regina::python::doc::GroupPresentation::identifySimplyIsomorphicTo
static constexpr const char identifySimplyIsomorphicTo[] =
R"doc(Attempts to prove that this and the given group presentation are
_simply isomorphic_.

A _simple isomorphism_ is an isomorphism where each generator *g_{i}*
of this presentation is sent to some generator *g_{j}*^{±1} of the
other presentation. Moreover, at present this routine only looks for
maps where both presentations have the same number of generators, and
where distinct generators *g_{i}* of this presentation correspond to
distinct generators *g_{j}* of the other presentation (possibly with
inversion, as noted above).

If this routine returns ``True``, it means that the two presentations
are indeed simply isomorphic.

If this routine returns ``False``, it could mean one of many things:

* The groups are not isomorphic;

* The groups are isomorphic, but not simply isomorphic;

* The groups are simply isomorphic but this routine could not prove
  it, due to difficulties with the word problem.

Parameter ``other``:
    the group presentation to compare with this.

Returns:
    ``True`` if this routine could certify that the two group
    presentations are simply isomorphic, or ``False`` if it could not.)doc";

// Docstring regina::python::doc::GroupPresentation::incidence
static constexpr const char incidence[] =
R"doc(Returns a matrix indicating which generators are used by which
relations.

The rows of the matrix correspond to the relations 0,1,..., and the
columns correspond to the generators 0,1,.... The matrix entry in row
*r*, column *g* will be ``True`` if and only if relation *r* uses
generator *g*.

Precondition:
    The numbers of generators and relations are both non-zero.

Returns:
    the incidence matrix between relators and generators.)doc";

// Docstring regina::python::doc::GroupPresentation::intelligentNielsen
static constexpr const char intelligentNielsen[] =
R"doc(Deprecated alias for nielsen(), which looks for Nielsen moves that
will simplify the presentation.

.. deprecated::
    This routine has been renamed to nielsen(). See nielsen() for
    further details.

Returns:
    an isomorphism describing the map from the original presentation
    to the new presentation, or ``None`` if this presentation was not
    changed.)doc";

// Docstring regina::python::doc::GroupPresentation::intelligentSimplify
static constexpr const char intelligentSimplify[] =
R"doc(Deprecated alias for simplify(), which attempts to simplify this group
presentation as intelligently as possible without further input.

.. deprecated::
    This routine has been renamed to simplify(). See simplify() for
    further details.

Returns:
    an isomorphism describing the reduction map from the original
    presentation to the new presentation, or ``None`` if this
    presentation was not changed.)doc";

// Docstring regina::python::doc::GroupPresentation::isValid
static constexpr const char isValid[] =
R"doc(Tests whether all of the relations for the group are indeed words in
the generators. This routine returns ``False`` if at least one relator
uses an out-of-bound generator, and ``True`` otherwise.

This routine is intended only for sanity checking: you should never
have an invalid group presentation in the first place.

Returns:
    ``True`` if and only if all of the relations are words in the
    generators.)doc";

// Docstring regina::python::doc::GroupPresentation::markedAbelianisation
static constexpr const char markedAbelianisation[] =
R"doc(Computes the abelianisation of this group. The coordinates in the
chain complex correspond to the generators and relators for this
group.

Returns:
    the abelianisation of this group.)doc";

// Docstring regina::python::doc::GroupPresentation::nielsen
static constexpr const char nielsen[] =
R"doc(Looks for Nielsen moves that will simplify the presentation. Performs
one of the most-effective moves, if it can find any.

If this routine does return a homomorphism (because some move was
performed), then this homomorphsm will in fact be a declared
isomorphism. See the HomGroupPresentation class notes for details on
what this means.

This routine is guaranteed to be deterministic: within the same
version of Regina, simplifying identical group presentations will give
identical results. These results could, however, change between
different versions of Regina.

.. note::
    If you all care about is whether the presentation changed, you can
    simply cast the return value to a ``bool``. This will then mirror
    the behaviour of intelligentNielsen() from Regina 6.0 and earlier,
    when the return type was simply ``bool``.

Returns:
    an isomorphism describing the map from the original presentation
    to the new presentation, or ``None`` if this presentation was not
    changed.)doc";

// Docstring regina::python::doc::GroupPresentation::nielsenCombine
static constexpr const char nielsenCombine[] =
R"doc(Replaces a generator ``gi`` by either ``(gi)(gj)^k`` or ``(gj)^k(gi)``
in the presentation. It it is the third type of Nielsen move one can
apply to a presentation.

This means that, if the new generator ``Gi`` is the old ``(gi)(gj)^k``
or ``(gj)^k(gi)``, then we can construct the new presentation from the
old by replacing occurrences of ``Gi`` by ``(Gi)(gj)^(-k)`` or
``(gj)^(-k)(Gi)`` respectively.

Precondition:
    Both *i* and *j* are strictly less than countGenerators().

Parameter ``i``:
    indicates the generator to replace.

Parameter ``j``:
    indicates the generator to combine with ``gi``.

Parameter ``k``:
    indicates the power to which we raise ``gj`` when performing the
    replacement; this may be positive or negative (or zero, but this
    will have no effect).

Parameter ``rightMult``:
    ``True`` if we should replace ``gi`` by ``(gi)(gj)^k``, or
    ``False`` if we should replace ``gi`` by ``(gj)^k(gi)``.

Returns:
    ``True`` if and only if the nielsen automorphism had an effect on
    at least one relation.)doc";

// Docstring regina::python::doc::GroupPresentation::nielsenInvert
static constexpr const char nielsenInvert[] =
R"doc(Replaces a generator in a presentation by its inverse, and recomputes
the appropriate presentation. This is the second generator type of the
automorphism group of a free group.

Precondition:
    *i* is strictly less than countGenerators().

Parameter ``i``:
    indicates the generator to invert.

Returns:
    ``True`` if and only if the Nielsen automorphism had an effect on
    at least one relation.)doc";

// Docstring regina::python::doc::GroupPresentation::nielsenTransposition
static constexpr const char nielsenTransposition[] =
R"doc(Switches the generators in the presentation indexed by *i* and *j*
respectively, and recomputes the appropriate presentation. It is one
of the standard Nielsen moves, which is the first of three generator
types of the automorphism group of a free group.

Precondition:
    Both *i* and *j* are strictly less than countGenerators().

Parameter ``i``:
    indicates the first of the two generators to switch.

Parameter ``j``:
    indicates the second of the two generators to switch.

Returns:
    ``True`` if and only if the Nielsen automorphism had an effect on
    at least one relation.)doc";

// Docstring regina::python::doc::GroupPresentation::prettyRewriting
static constexpr const char prettyRewriting[] =
R"doc(An entirely cosmetic rewriting of the presentation, which is fast and
superficial.

1. If there are any length 1 relators, those generators are deleted,
   and the remaining relators simplified.

2. It sorts the relators by number of generator indices that appear,
   followed by relator numbers (lexico) followed by relator length.

3. It inverts relators if the net sign of the generators is negative.

4. Given each generator, it looks for the smallest word where that
   generator appears with non-zero weight. If negative weight, it
   inverts that generator.

5. It cyclically permutes relators to start with smallest gen.

If this routine does return a homomorphism (because the choice of
generators was changed), then this homomorphsm will in fact be a
declared isomorphism. See the HomGroupPresentation class notes for
details on what this means.

This routine is guaranteed to be deterministic: within the same
version of Regina, simplifying identical group presentations will give
identical results. These results could, however, change between
different versions of Regina.

.. note::
    If you all care about is whether the presentation changed, you can
    simply cast the return value to a ``bool``. This will then mirror
    the behaviour of prettyRewriting() from Regina 6.0 and earlier,
    when the return type was simply ``bool``.

Returns:
    an isomorphism describing the map from the original presentation
    to the new presentation, or ``None`` if this presentation was not
    changed.)doc";

// Docstring regina::python::doc::GroupPresentation::proliferateRelators
static constexpr const char proliferateRelators[] =
R"doc(A routine to help escape local wells when simplifying presentations,
which may be useful when small cancellation theory can't find the
simplest relators.

Given a presentation ``<g_i | r_i>``, this routine appends
consequences of the relators ``{r_i}`` to the presentation that are of
the form ``ab``, where both ``a`` and ``b`` are cyclic permutations of
relators from the collection ``{r_i}``.

Passing depth=1 means it will only form products of two relators.
Depth=2 means products of three, etc. Depth=4 is typically the last
depth before the exponential growth of the operation grows out of
hand. It also conveniently trivializes all the complicated trivial
group presentations that we've come across so far.

.. warning::
    Do not call this routine with depth n before having called it at
    depth n-1 first. Depth≤0 is invalid, and depth=1 should be your
    first call to this routine. This routine gobbles up an exponential
    amount of memory (exponential in your presentation size times n).
    So do be careful when using it.

Parameter ``depth``:
    controls the depth of the proliferation, as described above; this
    must be strictly positive.)doc";

// Docstring regina::python::doc::GroupPresentation::recogniseGroup
static constexpr const char recogniseGroup[] =
R"doc(Attempts to recognise the group corresponding to this presentation.
This routine is much more likely to be successful if you have already
called simplify().

Currently, the groups this routine recognises include: the trivial
group, abelian groups, free groups, extensions over the integers, and
free products of any group the algorithm can recognise (inductively).

The string returned from this routine may use some unicode characters,
which will be encoding using UTF-8. If *moreUtf8* is passed as
``False`` then unicode will be used sparingly; if *moreUtf8* is
``True`` then unicode will be use more liberally, resulting in strings
that look nicer but require more complex fonts to be available on the
user's machine.

Examples of the format of the returned string are:

* ``0`` for the trivial group;

* ``Z_n`` for cyclic groups with *n* > 1;

* ``Free(n)`` for free groups with *n* > 1 generators - see
  AbelianGroup::str() for how abelian groups are presented;

* ``FreeProduct(G1, G2, ... , Gk)`` for free products, where one
  replaces *G1* through *Gk* by text strings representing the free
  summands;

* ``Z~G w/ monodromy H`` for extensions over Z, where *G* is a
  description of the kernel of the homomorphism to the integers, and
  *H* is a text string representing the monodromy - see
  HomMarkedAbelianGroup.str() for details on how these are presented.

Returns:
    a simple string representation of the group if it is recognised,
    or an empty string if the group is not recognised.)doc";

// Docstring regina::python::doc::GroupPresentation::relation
static constexpr const char relation[] =
R"doc(Returns the relation at the given index in this group presentation.
The relation will be of the form ``expresson = 1``.

Parameter ``index``:
    the index of the desired relation; this must be between 0 and
    countRelations()-1 inclusive.

Returns:
    the expression that the requested relation sets to 1; for
    instance, if the relation is ``g1^2 g2 = 1`` then this will be the
    expression ``g1^2 g2``.)doc";

// Docstring regina::python::doc::GroupPresentation::relations
static constexpr const char relations[] =
R"doc(Returns the list of all relations in this group presentation.

Python:
    The list itself is not returned by reference (instead this routine
    returns a new Python list). However, the relations within this
    list are still returned by reference. This means that in theory
    you could use the references in this list to modify each relation
    individually, although since this is a const function (which
    Python does not understand or respect), you should _not_ do this.

Returns:
    the list of relations.)doc";

// Docstring regina::python::doc::GroupPresentation::relatorLength
static constexpr const char relatorLength[] =
R"doc(The sum of the word lengths of the relators. Word lengths are
computing using GroupExpression::wordLength(). Used as a coarse
measure of the complexity of the presentation.

Returns:
    the sum of word lengths.)doc";

// Docstring regina::python::doc::GroupPresentation::simplify
static constexpr const char simplify[] =
R"doc(Attempts to simplify this group presentation as intelligently as
possible without further input.

The current simplification method uses a combination of small
cancellation theory and Nielsen moves.

If this routine does return a homomorphism (because the presentation
was changed), then this homomorphsm will in fact be a declared
isomorphism. See the HomGroupPresentation class notes for details on
what this means.

This routine is guaranteed to be deterministic: within the same
version of Regina, simplifying identical group presentations will give
identical results. These results could, however, change between
different versions of Regina.

.. note::
    If you all care about is whether the presentation changed, you can
    simply cast the return value to a ``bool``. This will then mirror
    the behaviour of intelligentSimplify() from Regina 6.0 and
    earlier, when the return type was simply ``bool``.

Returns:
    an isomorphism describing the reduction map from the original
    presentation to the new presentation, or ``None`` if this
    presentation was not changed.)doc";

// Docstring regina::python::doc::GroupPresentation::simplifyAndConjugate
static constexpr const char simplifyAndConjugate[] =
R"doc(Uses small cancellation theory to reduce the input word, modulo
conjugation, using the current presentation of the group. The input
word will be modified directly.

By "modulo conjugation", we mean: if *w* represents the input word,
then this routine might (as part of the reduction process) transform
*w* into a different group element of the form ``g w g^-1``.

In Regina 7.2 and earlier, this routine was called simplifyWord(). It
was renamed to simplifyAndConjugate() in Regina 7.3 to make it clear
to the user that conjugation might take place. Note that, even in
older versions of Regina, this routine could always potentially
conjugate.

.. warning::
    This routine is only as good as the relator table for the group.
    You might want to consider running simplify(), possibly in concert
    with proliferateRelators(), before using this routine for any
    significant tasks.

Parameter ``word``:
    the word you would like to simplify (modulo conjugation). This
    must be a word in the generators of this group.

Returns:
    ``True`` if and only if the input word was modified.)doc";

// Docstring regina::python::doc::GroupPresentation::smallCancellation
static constexpr const char smallCancellation[] =
R"doc(Attempts to simplify the group presentation using small cancellation
theory. The simplification method is based on the Dehn algorithm for
hyperbolic groups, i.e. small cancellation theory. This means we look
to see if part of one relator can be used to simplify others. If so,
make the substitution and simplify. We continue until no more
presentation-shortening substitutions are available. We follow that by
killing any available generators using words where generators appear a
single time.

If this routine does return a homomorphism (because the presentation
was changed), then this homomorphsm will in fact be a declared
isomorphism. See the HomGroupPresentation class notes for details on
what this means.

This routine is guaranteed to be deterministic: within the same
version of Regina, simplifying identical group presentations will give
identical results. These results could, however, change between
different versions of Regina.

.. note::
    If you all care about is whether the presentation changed, you can
    simply cast the return value to a ``bool``. This will then mirror
    the behaviour of smallCancellation() from Regina 6.0 and earlier,
    when the return type was simply ``bool``.

Returns:
    an isomorphism describing the reduction map from the original
    presentation to the new presentation, or ``None`` if this
    presentation was not changed.)doc";

// Docstring regina::python::doc::GroupPresentation::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given group presentation.

Parameter ``other``:
    the group presentation whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::GroupPresentation::tex
static constexpr const char tex[] =
R"doc(Returns a TeX representation of this group presentation.

The output will be of the form ``< generators | relators >``. There
will be no final newline.

Returns:
    a TeX representation of this group presentation.)doc";

// Docstring regina::python::doc::GroupPresentation::writeXMLData
static constexpr const char writeXMLData[] =
R"doc(Writes a chunk of XML containing this group presentation.

Python:
    The argument *out* should be an open Python file object.

Parameter ``out``:
    the output stream to which the XML should be written.)doc";

}; // struct GroupPresentation

struct SplayedExpression {

// Docstring regina::python::doc::SplayedExpression::__array
static constexpr const char __array[] =
R"doc(Returns a read-write reference to the integer at the given index in
the underlying sequence.

Parameter ``index``:
    the index of the integer to access. This must be between 0 and
    ``size()-1`` inclusive.

Returns:
    a reference to the requested integer.)doc";

// Docstring regina::python::doc::SplayedExpression::__array_2
static constexpr const char __array_2[] =
R"doc(Returns a read-only reference to the integer at the given index in the
underlying sequence.

Parameter ``index``:
    the index of the integer to access. This must be between 0 and
    ``size()-1`` inclusive.

Returns:
    a reference to the requested integer.)doc";

// Docstring regina::python::doc::SplayedExpression::__class
static constexpr const char __class[] =
R"doc(Represents a group expression expanded so that each term uses a single
generator or its inverse.

The expansion is presented as a sequence of integers. If *g*
represents generator *i* in a group presentation (``i ≥ 0``), then a
SplayedExpression represents the generator *g* using the positive
integer ``g+1``, and represents the inverse of *g* using the negative
integer ``-(g+1)``. The integer 0 should never appear in a
SplayedExpression at all.

As an example, the group expression ``g1^2 g3^-2 g6`` would be appear
in splayed form as the integer sequence ``2,2,-4,-4,7``.

A SplayedExpression is typically longer than its corresponding
GroupExpression. Its advantage is that some operations (e.g., word
length, searching and surgery) become simpler in a splayed setting.

Currently the API for SplayedExpression is fairly limited (just what
is needed for internal group presentation algorithms); the available
operations may expand in future releases of Regina.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::SplayedExpression::__copy
static constexpr const char __copy[] = R"doc(Creates a new copy of the given splayed expression.)doc";

// Docstring regina::python::doc::SplayedExpression::__default
static constexpr const char __default[] = R"doc(Creates a new splayed expression with no terms.)doc";

// Docstring regina::python::doc::SplayedExpression::__eq
static constexpr const char __eq[] =
R"doc(Tests whether this and the given splayed expression contain the same
terms in the same order.

Parameter ``other``:
    the expression to compare with this.

Returns:
    ``True`` if and only if this and the given expression are
    identical.)doc";

// Docstring regina::python::doc::SplayedExpression::__init
static constexpr const char __init[] =
R"doc(Creates a new splayed expression that expands the given group
expression.

Parameter ``word``:
    the group expression to expand.)doc";

// Docstring regina::python::doc::SplayedExpression::__init_2
static constexpr const char __init_2[] =
R"doc(Creates a new splayed expression that expands the given group
expression, where the word length is known in advance.

The reason for having this routine is that
GroupExpression::wordLength() requires linear time to compute, and so
if you already know it then you can pass it here to avoid an expensive
re-computation.

Precondition:
    The argument *wordLength* is precisely ``word.wordLength()``.

Parameter ``word``:
    the group expression to expand.

Parameter ``wordLength``:
    the word length of *word*.)doc";

// Docstring regina::python::doc::SplayedExpression::__iter__
static constexpr const char __iter__[] =
R"doc(Returns a Python iterator over the elements of the underlying integer
sequence.

Returns:
    an iterator over the elements of the underlying integer sequence.)doc";

// Docstring regina::python::doc::SplayedExpression::desplay
static constexpr const char desplay[] =
R"doc(Repacks this as a group expression. This will combine adjacent terms
that use the same generator, and will apply any simplifications that
arise. So, for example, the splayed integer sequence ``1 3 2 -2 -3 5
5`` will be repacked as ``g0 g4^2``.

The expression is not assumed to be cyclic (i.e., this routine will
not attempt to cancel terms from the front and back of the word).

Returns:
    the repacked group expression.)doc";

// Docstring regina::python::doc::SplayedExpression::empty
static constexpr const char empty[] =
R"doc(Tests whether this is the empty word.

No attempt is made to remove redundant terms (so the word ``g g^-1``
will be treated as non-empty).

Returns:
    ``True`` if and only if this is the empty word.)doc";

// Docstring regina::python::doc::SplayedExpression::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the two given expressions.

This global routine simply calls SplayedExpression::swap(); it is
provided so that SplayedExpression meets the C++ Swappable
requirements.

Parameter ``lhs``:
    the expression whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the expression whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::SplayedExpression::inverse
static constexpr const char inverse[] =
R"doc(Returns the inverse of this expression. The terms will be reversed,
and generators will be replaced with their inverses (and vice versa).

Returns:
    the inverse of this expression.)doc";

// Docstring regina::python::doc::SplayedExpression::invert
static constexpr const char invert[] =
R"doc(Inverts this expression in-place. The terms will be reversed, and
generators will be replaced with their inverses (and vice versa).)doc";

// Docstring regina::python::doc::SplayedExpression::simplify
static constexpr const char simplify[] =
R"doc(Simplifies this expression. Adjacent terms of the form ``g g^-1`` or
``g^-1 g`` will be cancelled, and this will propagate as required if
new cancellation opportunities open up as a result. It is _not_
assumed that the underlying group is abelian.

You may declare that the expression is cyclic, in which case terms
from the front can also cancel with inverse terms from the back.

Parameter ``cyclic``:
    ``True`` if and only if the expression may be assumed to be
    cyclic.

Returns:
    ``True`` if and only if this expression was changed.)doc";

// Docstring regina::python::doc::SplayedExpression::size
static constexpr const char size[] =
R"doc(Returns the length of this splayed expression. This is the number of
individual terms (i.e., the length of the corresponding integer
sequence), and it corresponds to GroupExpression::wordLength().

No attempt is made to remove redundant terms (so the word ``g g^-1``
will count as length two).

Returns:
    the length of this expression.)doc";

// Docstring regina::python::doc::SplayedExpression::str
static constexpr const char str[] =
R"doc(Returns a short text representation of this splayed expression, with a
choice of either numbered generators or alphabetic generators.

If *alpha* is ``False``, the text representation will be of the form
``g2 g13^-1 g4``. If *alpha* is ``True``, this routine will assume
your word comes from a group with at most 26 generators, and will
format the word using lower-case ASCII for generators and upper-case
ASCII for their inverses, i.e., ``cNe``.

Note that there is also a zero-argument version of str(), inherited
through the ShortOutput base class. This zero-argument str() gives the
same output as ``str(false)``.

In the output, generators will be numbered starting from 0 (i.e.,
following the same numbering used by GroupExpression and
GroupPresentation). This means, for example, that the output ``g2
g13^-1 g4`` or ``cNe`` would come from the splayed integer sequence
``3 -14 5``.

Precondition:
    If *alpha* is ``True``, the number of generators in the
    corresponding group must be at most 26.

Parameter ``alpha``:
    indicates whether to use numbered or alphabetic generators, as
    described above.

Returns:
    a short text representation of this splayed expression.)doc";

// Docstring regina::python::doc::SplayedExpression::substitute
static constexpr const char substitute[] =
R"doc(Replaces every occurrence of the given generator with the given
replacement word. As expected, inverses of the given generator will be
replaced with inverses of the given word. If the given generator or
its inverse _was_ found, the expression will be simplified once the
substitution is complete.

Be aware that the argument *generator* is zero-based, not one-based
(that is, we use the same generator numbering as GroupExpression and
GroupPresentation). This means, for example, that calling
``substitute(2, ...)`` will replace all occurrences of ±3 in the
underlying integer sequence.

Precondition:
    The argument *replacement* is not the same SplayedExpression
    object as this.

Parameter ``generator``:
    the generator to be replaced, numbered from 0 upwards.

Parameter ``replacement``:
    the substitute expression that will replace every occurrence of
    the given generator.

Parameter ``cyclic``:
    ``True`` if and only if the expression may be assumed to be
    cyclic; this is only relevant during the simplification stage.

Returns:
    ``True`` if and only if any substitutions were made.)doc";

// Docstring regina::python::doc::SplayedExpression::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given splayed expression.

Parameter ``other``:
    the expression whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::SplayedExpression::utf8
static constexpr const char utf8[] =
R"doc(Returns a short text representation of this splayed expression using
unicode characters, with a choice of either numbered generators or
alphabetic generators.

This outputs a similar text representation to str(bool), except that
exponents for inverses will be written using superscript characters
encoded in UTF-8. See str(bool) for further details.

Note that there is also a zero-argument version of utf8(), inherited
through the ShortOutput base class. This zero-argument utf8() gives
the same output as ``utf8(false)``.

Precondition:
    If *alpha* is ``True``, the number of generators in the
    corresponding group must be at most 26.

Parameter ``alpha``:
    indicates whether to use numbered or alphabetic generators.

Returns:
    a short text representation of this splayed expression.)doc";

}; // struct SplayedExpression

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

