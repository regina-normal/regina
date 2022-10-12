/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::GroupExpression
static const char *GroupExpression =
R"doc(Represents an expression involving generators from a group
presentation or a free group. An expression is represented as word,
i.e, a sequence of powers of generators all of which are multiplied in
order. Each power of a generator corresponds to an individual
GroupExpressionTerm.

For instance, the expression ``g1^2 g3^-1 g6`` contains the three
terms ``g1^2``, ``g3^-1`` and ``g6^1`` in that order.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::GroupExpressionTerm
static const char *GroupExpressionTerm =
R"doc(Represents a power of a generator in a group presentation.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

// Docstring regina::python::doc::GroupPresentation
static const char *GroupPresentation =
R"doc(Represents a finite presentation of a group.

A presentation consists of a number of generators and a set of
relations between these generators that together define the group.

If there are *g* generators, they will be numbered 0, 1, ..., *g*-1.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::swap
static const char *swap =
R"doc(Swaps the contents of the two given expressions.

This global routine simply calls GroupExpression::swap(); it is
provided so that GroupExpression meets the C++ Swappable requirements.

Parameter ``lhs``:
    the expression whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the expression whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::swap_2
static const char *swap_2 =
R"doc(Swaps the contents of the two given group presentations.

This global routine simply calls GroupPresentation::swap(); it is
provided so that GroupPresentation meets the C++ Swappable
requirements.

Parameter ``lhs``:
    the presentation whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the presentation whose contents should be swapped with *lhs*.)doc";

namespace GroupExpressionTerm_ {

// Docstring regina::python::doc::GroupExpressionTerm_::GroupExpressionTerm
static const char *GroupExpressionTerm = R"doc(Creates a new uninitialised term.)doc";

// Docstring regina::python::doc::GroupExpressionTerm_::GroupExpressionTerm_2
static const char *GroupExpressionTerm_2 =
R"doc(Creates a new term initialised to the given value.

Parameter ``newGen``:
    the number that identifies the generator in the new term.

Parameter ``newExp``:
    the exponent to which this generator is raised.)doc";

// Docstring regina::python::doc::GroupExpressionTerm_::GroupExpressionTerm_3
static const char *GroupExpressionTerm_3 = R"doc(Creates a new term initialised to the given value.)doc";

// Docstring regina::python::doc::GroupExpressionTerm_::__eq
static const char *__eq =
R"doc(Determines whether this and the given term contain identical data.

Parameter ``other``:
    the term with which this term will be compared.

Returns:
    ``True`` if and only if this and the given term have both the same
    generator and exponent.)doc";

// Docstring regina::python::doc::GroupExpressionTerm_::__iadd
static const char *__iadd =
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

// Docstring regina::python::doc::GroupExpressionTerm_::__lt
static const char *__lt =
R"doc(Imposes an ordering on terms. Terms are ordered lexigraphically as
(generator, exponent) pairs.

Parameter ``other``:
    the term to compare with this.

Returns:
    true if and only if this term is lexicographically smaller than
    *other*.)doc";

// Docstring regina::python::doc::GroupExpressionTerm_::__ne
static const char *__ne =
R"doc(Determines whether this and the given term do not contain identical
data.

Parameter ``other``:
    the term with which this term will be compared.

Returns:
    ``True`` if and only if this and the given term do not have both
    the same generator and exponent.)doc";

// Docstring regina::python::doc::GroupExpressionTerm_::inverse
static const char *inverse =
R"doc(Returns the inverse of this term. The inverse has the same generator
but a negated exponent.

Note that this term will remain unchanged.

Returns:
    the inverse of this term.)doc";

}

namespace GroupExpression_ {

// Docstring regina::python::doc::GroupExpression_::GroupExpression
static const char *GroupExpression = R"doc(Creates a new expression with no terms.)doc";

// Docstring regina::python::doc::GroupExpression_::GroupExpression_2
static const char *GroupExpression_2 =
R"doc(Creates a new expression containing a single term.

Parameter ``term``:
    the term to use as the new expression.)doc";

// Docstring regina::python::doc::GroupExpression_::GroupExpression_3
static const char *GroupExpression_3 =
R"doc(Creates a new expression containing a single term.

Parameter ``generator``:
    the number of the generator to use in the term.

Parameter ``exponent``:
    the exponent to which the given generator is raised in the term.)doc";

// Docstring regina::python::doc::GroupExpression_::GroupExpression_4
static const char *GroupExpression_4 = R"doc(Creates a new expression that is a clone of the given expression.)doc";

// Docstring regina::python::doc::GroupExpression_::GroupExpression_5
static const char *GroupExpression_5 =
R"doc(Attempts to interpret the given input string as a word in a group.
Regina can recognise strings in the following four basic forms:

* ``a^7b^-2``

* ``aaaaaaaBB``

* ``a^7B^2``

* ``g0^7g1^-2``

The string may contain whitespace, which will simply be ignored.

Exception ``InvalidArgument``:
    The given string could not be interpreted as a group expression.

Parameter ``input``:
    the input string that is to be interpreted.)doc";

// Docstring regina::python::doc::GroupExpression_::GroupExpression_6
static const char *GroupExpression_6 =
R"doc(Attempts to interpret the given input string as a word in a group.
Regina can recognise strings in the following four basic forms:

* ``a^7b^-2``

* ``aaaaaaaBB``

* ``a^7B^2``

* ``g0^7g1^-2``

The string may contain whitespace, which will simply be ignored.

Exception ``InvalidArgument``:
    The given string could not be interpreted as a group expression.

Parameter ``input``:
    the input string that is to be interpreted.)doc";

// Docstring regina::python::doc::GroupExpression_::__eq
static const char *__eq =
R"doc(Equality operator. Checks to see whether or not these two words
represent the same literal string.

Parameter ``comp``:
    the expression to compare against this.

Returns:
    ``True`` if this and the given string literal are identical.)doc";

// Docstring regina::python::doc::GroupExpression_::__ne
static const char *__ne =
R"doc(Inequality operator. Checks to see whether or not these two words
represent different literal strings.

Parameter ``comp``:
    the expression to compare against this.

Returns:
    ``True`` if this and the given string literal are not identical.)doc";

// Docstring regina::python::doc::GroupExpression_::addTermFirst
static const char *addTermFirst =
R"doc(Adds the given term to the beginning of this expression.

Parameter ``term``:
    the term to add.)doc";

// Docstring regina::python::doc::GroupExpression_::addTermFirst_2
static const char *addTermFirst_2 =
R"doc(Adds the given term to the beginning of this expression.

Parameter ``generator``:
    the number of the generator corresponding to the new term.

Parameter ``exponent``:
    the exponent to which the given generator is raised.)doc";

// Docstring regina::python::doc::GroupExpression_::addTermLast
static const char *addTermLast =
R"doc(Adds the given term to the end of this expression.

Parameter ``term``:
    the term to add.)doc";

// Docstring regina::python::doc::GroupExpression_::addTermLast_2
static const char *addTermLast_2 =
R"doc(Adds the given term to the end of this expression.

Parameter ``generator``:
    the number of the generator corresponding to the new term.

Parameter ``exponent``:
    the exponent to which the given generator is raised.)doc";

// Docstring regina::python::doc::GroupExpression_::addTermsFirst
static const char *addTermsFirst =
R"doc(Multiplies this expression on the left by the given word. This
expression will be modified directly.

Parameter ``word``:
    the word to multiply with this expression.)doc";

// Docstring regina::python::doc::GroupExpression_::addTermsLast
static const char *addTermsLast =
R"doc(Multiplies this expression on the right by the given word. This
expression will be modified directly.

Parameter ``word``:
    the word to multiply with this expression.)doc";

// Docstring regina::python::doc::GroupExpression_::countTerms
static const char *countTerms =
R"doc(Returns the number of terms in this expression.

For instance, the expression ``g1^2 g3^-1 g6`` contains three terms.
See also wordLength().

Returns:
    the number of terms.)doc";

// Docstring regina::python::doc::GroupExpression_::cycleLeft
static const char *cycleLeft =
R"doc(Cycles this word by moving the rightmost term around to the leftmost.
All other terms shift one step to the right.

If the word is of the form ``g_i1^j1 g_i2^j2 ... g_in^jn``, this
converts it into the word ``g_in^jn g_i1^j1 g_i1^j1 ... g_in-1^jn-1``.)doc";

// Docstring regina::python::doc::GroupExpression_::cycleRight
static const char *cycleRight =
R"doc(Cycles this word by moving the leftmost term around to the rightmost.
All other terms shift one step to the left.

If the word is of the form ``g_i1^j1 g_i2^j2 ... g_in^jn``, this
converts it into the word ``g_i2^j2 ... g_in^jn g_i1^j1``.)doc";

// Docstring regina::python::doc::GroupExpression_::erase
static const char *erase =
R"doc(Erases all terms from this this word. This effectively turns this word
into the identity element.)doc";

// Docstring regina::python::doc::GroupExpression_::exponent
static const char *exponent =
R"doc(Returns the exponent corresonding to the term at the given index in
this expression. Index 0 represents the first term, index 1 represents
the second term and so on.

.. warning::
    This routine is *O(n)* where *n* is the number of terms in this
    expression.

Parameter ``index``:
    the index of the term to return; this must be between 0 and
    countTerms()-1 inclusive.

Returns:
    the requested exponent.)doc";

// Docstring regina::python::doc::GroupExpression_::generator
static const char *generator =
R"doc(Returns the generator corresonding to the term at the given index in
this expression. Index 0 represents the first term, index 1 represents
the second term and so on.

.. warning::
    This routine is *O(n)* where *n* is the number of terms in this
    expression.

Parameter ``index``:
    the index of the term to return; this must be between 0 and
    countTerms()-1 inclusive.

Returns:
    the number of the requested generator.)doc";

// Docstring regina::python::doc::GroupExpression_::inverse
static const char *inverse =
R"doc(Returns the inverse of this expression. The terms will be reversed and
the exponents negated.

Returns:
    the inverse of this expression.)doc";

// Docstring regina::python::doc::GroupExpression_::invert
static const char *invert = R"doc(Inverts this expression. Does not allocate or deallocate anything.)doc";

// Docstring regina::python::doc::GroupExpression_::isTrivial
static const char *isTrivial =
R"doc(Tests whether this is the trivial (unit) word.

No attempt is made to remove redundant terms (so the word ``g g^-1``
will be treated as non-trivial).

Returns:
    ``True`` if and only if this is the trivial word.)doc";

// Docstring regina::python::doc::GroupExpression_::power
static const char *power =
R"doc(Returns this expression raised to the given power. The given exponent
may be positive, zero or negative.

Parameter ``exponent``:
    the power to which this expression should be raised.

Returns:
    this expression raised to the given power.)doc";

// Docstring regina::python::doc::GroupExpression_::simplify
static const char *simplify =
R"doc(Simplifies this expression. Adjacent powers of the same generator will
be combined, and terms with an exponent of zero will be removed. Note
that it is *not* assumed that the underlying group is abelian.

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

// Docstring regina::python::doc::GroupExpression_::str
static const char *str =
R"doc(Returns a short text representation of this group expression, with a
choice of either numbered generators or alphabetic generators.

If *alphaGen* is ``False``, the text representation will be of the
form ``g2^4 g13^-5 g4``. If *alphaGen* is ``True``, this routine will
assume your word is in an alphabet of no more than 26 letters, and
will format the word using lower-case ASCII, i.e., ``c^4 n^-5 e``.

Note that there is also a zero-argument version of str(), inherited
through the ShortOutput base class. This zero-argument str() gives the
same output as ``str(false)``.

Precondition:
    If *alphaGen* is ``True``, the number of generators in the
    corresponding group must be 26 or fewer.

Parameter ``alphaGen``:
    indicates whether to use numbered or alphabetic generators, as
    described above.

Returns:
    a short text representation of this group expression.)doc";

// Docstring regina::python::doc::GroupExpression_::substitute
static const char *substitute =
R"doc(Replaces every occurrence of the given generator with the given
substitute expression. If the given generator was found, the
expression will be simplified once the substitution is complete.

Precondition:
    The given expansion is not the same GroupExpression object as
    this.

Parameter ``generator``:
    the generator to be replaced.

Parameter ``expansion``:
    the substitute expression that will replace every occurrence of
    the given generator.

Parameter ``cyclic``:
    ``True`` if and only if the expression may be assumed to be
    cyclic; see simplify() for further details.

Returns:
    ``True`` if and only if any substitutions were made.)doc";

// Docstring regina::python::doc::GroupExpression_::substitute_2
static const char *substitute_2 =
R"doc(Replaces every generator in this expression with the corresponding
substitute expression from the given map.

Specifically, each generator *i* will be replaced with the expression
``expansions[i]``.

The expression will be simplified once all substitutions are complete.

Unlike the single-generator verison of substitute(), it is perfectly
fine if this GroupExpression object appears in the *expansions* list,
and/or if the same GroupExpression object appears several times in the
given list.

Precondition:
    The length of *expansions* is at least *g*+1, where *g* is the
    largest generator that appears in this expression. In other words,
    ``expansions[i]`` exists for every generator *i* that appears in
    this expression.

Parameter ``expansions``:
    the list of substitutes for all generators in this expression.

Parameter ``cyclic``:
    ``True`` if and only if the expression may be assumed to be
    cyclic; see simplify() for further details.)doc";

// Docstring regina::python::doc::GroupExpression_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given expression.

Parameter ``other``:
    the expression whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::GroupExpression_::term
static const char *term =
R"doc(Returns the term at the given index in this expression. Index 0
represents the first term, index 1 represents the second term and so
on.

.. warning::
    This routine is *O(n)* where *n* is the number of terms in this
    expression.

Parameter ``index``:
    the index of the term to return; this must be between 0 and
    countTerms()-1 inclusive.

Returns:
    the requested term.)doc";

// Docstring regina::python::doc::GroupExpression_::term_2
static const char *term_2 =
R"doc(Returns a constant reference to the term at the given index in this
expression. Index 0 represents the first term, index 1 represents the
second term and so on.

.. warning::
    This routine is *O(n)* where *n* is the number of terms in this
    expression.

Parameter ``index``:
    the index of the term to return; this must be between 0 and
    countTerms()-1 inclusive.

Returns:
    the requested term.)doc";

// Docstring regina::python::doc::GroupExpression_::terms
static const char *terms =
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

// Docstring regina::python::doc::GroupExpression_::terms_2
static const char *terms_2 =
R"doc(Returns a constant reference to the list of terms in this expression.

For instance, the expression ``g1^2 g3^-1 g6`` has list consisting of
three terms ``g1^2``, ``g3^-1`` and ``g6^1`` in that order.

Python:
    The list itself is not returned by reference (instead this routine
    returns a new Python list). However, the terms within this list
    are still returned by reference.

Returns:
    the list of terms.)doc";

// Docstring regina::python::doc::GroupExpression_::tex
static const char *tex =
R"doc(Returns a TeX representation of this expression. See writeTeX() for
details on how this is formed.

Returns:
    a TeX representation of this expression.)doc";

// Docstring regina::python::doc::GroupExpression_::utf8
static const char *utf8 =
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
    If *alphaGen* is ``True``, the number of generators in the
    corresponding group must be 26 or fewer.

Parameter ``alphaGen``:
    indicates whether to use numbered or alphabetic generators, as
    described above.

Returns:
    a short text representation of this group expression.)doc";

// Docstring regina::python::doc::GroupExpression_::wordLength
static const char *wordLength =
R"doc(Returns the length of the word, i.e. the number of letters with
exponent +1 or -1 for which this word is expressable as a product.

For instance, the expression ``g1^2 g3^-1 g6`` is a word of length
four. See also countTerms().

No attempt is made to remove redundant terms (so the word ``g g^-1``
will count as length two).

Returns:
    the length of the word.)doc";

// Docstring regina::python::doc::GroupExpression_::writeXMLData
static const char *writeXMLData =
R"doc(Writes a chunk of XML containing this expression.

Python:
    The argument *out* should be an open Python file object.

Parameter ``out``:
    the output stream to which the XML should be written.)doc";

}

namespace GroupPresentation_ {

// Docstring regina::python::doc::GroupPresentation_::GroupPresentation
static const char *GroupPresentation = R"doc(Creates a new presentation with no generators and no relations.)doc";

// Docstring regina::python::doc::GroupPresentation_::GroupPresentation_2
static const char *GroupPresentation_2 =
R"doc(Creates a clone of the given group presentation.

Parameter ``src``:
    the group presentation to clone.)doc";

// Docstring regina::python::doc::GroupPresentation_::GroupPresentation_3
static const char *GroupPresentation_3 =
R"doc(Creates the free group on the given number of generators.

Parameter ``nGenerators``:
    the number of generators.)doc";

// Docstring regina::python::doc::GroupPresentation_::GroupPresentation_4
static const char *GroupPresentation_4 =
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
    group expression.

Parameter ``nGens``:
    the number of generators.

Parameter ``rels``:
    a vector of relations each given in string form, as outlined
    above.)doc";

// Docstring regina::python::doc::GroupPresentation_::__eq
static const char *__eq =
R"doc(Determines whether this and the given group presentation are
identical.

This routine does *not* test for isomorphism (which in general is an
undecidable problem). Instead it tests whether this and the given
presentation use exactly the same generators and exactly the same
relations, presented in exactly the same order.

Parameter ``other``:
    the group presentation to compare with this.

Returns:
    ``True`` if and only if this and the given group presentation are
    identical.)doc";

// Docstring regina::python::doc::GroupPresentation_::__ne
static const char *__ne =
R"doc(Determines whether this and the given group presentation are not
identical.

This routine does *not* test for isomorphism (which in general is an
undecidable problem). Instead it tests whether this and the given
presentation use exactly the same generators and exactly the same
relations, presented in exactly the same order.

Parameter ``other``:
    the group presentation to compare with this.

Returns:
    ``True`` if and only if this and the given group presentation are
    not identical.)doc";

// Docstring regina::python::doc::GroupPresentation_::abelianRank
static const char *abelianRank =
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

// Docstring regina::python::doc::GroupPresentation_::abelianisation
static const char *abelianisation =
R"doc(Computes the abelianisation of this group.

Returns:
    the abelianisation of this group.)doc";

// Docstring regina::python::doc::GroupPresentation_::addGenerator
static const char *addGenerator =
R"doc(Adds one or more generators to the group presentation. If the new
presentation has *g* generators, the new generators will be numbered
*g*-1, *g*-2 and so on.

Parameter ``numToAdd``:
    the number of generators to add.

Returns:
    the number of generators in the new presentation.)doc";

// Docstring regina::python::doc::GroupPresentation_::addRelation
static const char *addRelation =
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

// Docstring regina::python::doc::GroupPresentation_::compact
static const char *compact =
R"doc(Returns a compact one-line representation of this group presentation,
including details of all generators and relations. See
writeTextCompact() for details on how this is formed.

Currently str() and compact() are identical functions, though the
output from str() may change in future versions of Regina.

Returns:
    a compact representation of this group presentation.)doc";

// Docstring regina::python::doc::GroupPresentation_::countGenerators
static const char *countGenerators =
R"doc(Returns the number of generators in this group presentation.

Returns:
    the number of generators.)doc";

// Docstring regina::python::doc::GroupPresentation_::countRelations
static const char *countRelations =
R"doc(Returns the number of relations in this group presentation.

Returns:
    the number of relations.)doc";

// Docstring regina::python::doc::GroupPresentation_::enumerateCovers
static const char *enumerateCovers =
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
*action* (which must be a function or some other callable object).

* The first argument to *action* must be a group presentation. This
  will be the index *k* subgroup corresponding to the representation.
  This argument will be passed as an rvalue; a typical action could
  (for example) take it by const reference and query it, or take it by
  value and modify it, or take it by rvalue reference and move it into
  more permanent storage.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return ``void``.

* It is completely safe for *action* to (if you wish) make changes to
  the original presentation (i.e., the group presentation upon which
  enumerateCovers() is being called). This will not interfere with the
  enumeration or change the results in any way.

This routine produces a constant stream of output (i.e., it calls
*action* as soon as each representation is found).

.. warning::
    The running time is ``(k!)^g``, where *k* is the subgroup index
    described above, and *g* is the number of generators of this group
    presentation. In particular, the running time grows *extremely*
    quickly with *k*.

.. warning::
    This routine does *not* simplify the group presentation before it
    runs. You should make sure that you have simplified the
    presentation, either using Regina or some other tool, before
    running this routine, since (as noted above) the running time is
    exponential in the number of generators.

.. warning::
    Likewise, this routine does not simplify the subgroup
    presentations before passing them to *action*. These presentations
    can be quite large, and (for example) if all you care about is
    their abelianisations then you are better off using the *abelian*
    group simplification / computation instead (which is much faster).

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
    does *not* take an addition argument list (*args*). The second
    form is ``enumerateCovers(index)``, which returns a Python list
    containing all of the corresponding subgroups, each given as a
    GroupPresentation. In both forms, the template parameter *index*
    becomes the first argument to the Python function.

Template parameter ``index``:
    the number *k* in the description above; in other words, the index
    of the resulting subgroups. Currently this must be between 2 and 7
    inclusive; this range is chosen because those are the *k* for
    which Regina's permutation class Perm<k> is highly optimised.

Parameter ``action``:
    a function (or other callable object) to call for each
    representation that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial subgroup presentation argument.

Returns:
    the total number of representations found.)doc";

// Docstring regina::python::doc::GroupPresentation_::gap
static const char *gap =
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

// Docstring regina::python::doc::GroupPresentation_::homologicalAlignment
static const char *homologicalAlignment =
R"doc(Rewrites the presentation so that generators of the group map to
generators of the abelianisation, with any left-over generators
mapping to zero (if possible). Consider this a *homological*-alignment
of the presentation.

If the abelianisation of this group has rank *N* and *M* invariant
factors ``d0 | d2 | ... | d(M-1)``, this routine applies Nielsen moves
to the presentation to ensure that under the markedAbelianisation()
routine, generators 0 through *M*-1 are mapped to generators of the
relevant ``Z_di`` group. Similarly, generators *M* through *M*+*N*-1
are mapped to +/-1 in the appropriate factor. All further generators
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

Returns:
    an isomorphism describing the reduction map from the original
    presentation to the new presentation, or no value if this
    presentation was not changed.)doc";

// Docstring regina::python::doc::GroupPresentation_::identifyAbelian
static const char *identifyAbelian =
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
    intelligentSimplify, possibly in concert with
    proliferateRelators(), in order to discover adequately many
    commutators.

Returns:
    ``True`` if the group is shown to be abelian, or ``False`` if the
    result is inconclusive.)doc";

// Docstring regina::python::doc::GroupPresentation_::identifySimplyIsomorphicTo
static const char *identifySimplyIsomorphicTo =
R"doc(Attempts to prove that this and the given group presentation are
*simply isomorphic*.

A *simple isomorphism* is an isomorphism where each generator *g_(i)*
of this presentation is sent to some generator *g_(j)*^(+/-1) of the
other presentation. Moreover, at present this routine only looks for
maps where both presentations have the same number of generators, and
where distinct generators *g_(i)* of this presentation correspond to
distinct generators *g_(j)* of the other presentation (possibly with
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

// Docstring regina::python::doc::GroupPresentation_::incidence
static const char *incidence =
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

// Docstring regina::python::doc::GroupPresentation_::intelligentNielsen
static const char *intelligentNielsen =
R"doc(Looks for Nielsen moves that will simplify the presentation. Performs
one of the most-effective moves, if it can find any.

If this routine does return a homomorphism (because some move was
performed), then this homomorphsm will in fact be a declared
isomorphism. See the HomGroupPresentation class notes for details on
what this means.

.. note::
    If you all care about is whether the presentation changed, you can
    simply cast the return value to a ``bool``. This will then mirror
    the behaviour of intelligentNielsen() from Regina 6.0 and earlier,
    when the return type was simply ``bool``.

Returns:
    an isomorphism describing the map from the original presentation
    to the new presentation, or no value if this presentation was not
    changed.)doc";

// Docstring regina::python::doc::GroupPresentation_::intelligentSimplify
static const char *intelligentSimplify =
R"doc(Attempts to simplify the group presentation as intelligently as
possible without further input.

The current simplification method uses a combination of small
cancellation theory and Nielsen moves.

If this routine does return a homomorphism (because the presentation
was changed), then this homomorphsm will in fact be a declared
isomorphism. See the HomGroupPresentation class notes for details on
what this means.

.. note::
    If you all care about is whether the presentation changed, you can
    simply cast the return value to a ``bool``. This will then mirror
    the behaviour of intelligentSimplify() from Regina 6.0 and
    earlier, when the return type was simply ``bool``.

Returns:
    an isomorphism describing the reduction map from the original
    presentation to the new presentation, or no value if this
    presentation was not changed.)doc";

// Docstring regina::python::doc::GroupPresentation_::isValid
static const char *isValid =
R"doc(Tests whether all of the relations for the group are indeed words in
the generators. This routine returns ``False`` if at least one relator
uses an out-of-bound generator, and ``True`` otherwise.

This routine is intended only for sanity checking: you should never
have an invalid group presentation in the first place.

Returns:
    ``True`` if and only if all of the relations are words in the
    generators.)doc";

// Docstring regina::python::doc::GroupPresentation_::markedAbelianisation
static const char *markedAbelianisation =
R"doc(Computes the abelianisation of this group. The coordinates in the
chain complex correspond to the generators and relators for this
group.

Returns:
    the abelianisation of this group.)doc";

// Docstring regina::python::doc::GroupPresentation_::nielsenCombine
static const char *nielsenCombine =
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

// Docstring regina::python::doc::GroupPresentation_::nielsenInvert
static const char *nielsenInvert =
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

// Docstring regina::python::doc::GroupPresentation_::nielsenTransposition
static const char *nielsenTransposition =
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

// Docstring regina::python::doc::GroupPresentation_::prettyRewriting
static const char *prettyRewriting =
R"doc(An entirely cosmetic re-writing of the presentation, which is fast and
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

.. note::
    If you all care about is whether the presentation changed, you can
    simply cast the return value to a ``bool``. This will then mirror
    the behaviour of prettyRewriting() from Regina 6.0 and earlier,
    when the return type was simply ``bool``.

Returns:
    an isomorphism describing the map from the original presentation
    to the new presentation, or no value if this presentation was not
    changed.)doc";

// Docstring regina::python::doc::GroupPresentation_::proliferateRelators
static const char *proliferateRelators =
R"doc(A routine to help escape local wells when simplifying presentations,
which may be useful when small cancellation theory can't find the
simplest relators.

Given a presentation <g_i | r_i>, this routine appends consequences of
the relators {r_i} to the presentation that are of the form ab, where
both a and b are cyclic permutations of relators from the collection
{r_i}.

Passing depth=1 means it will only form products of two relators.
Depth=2 means products of three, etc. Depth=4 is typically the last
depth before the exponential growth of the operation grows out of
hand. It also conveniently trivializes all the complicated trivial
group presentations that we've come across so far.

.. warning::
    Do not call this routine with depth n before having called it at
    depth n-1 first. Depth=0 is invalid, and depth=1 should be your
    first call to this routine. This routine gobbles up an exponential
    amount of memory (exponential in your presentation size times n).
    So do be careful when using it.

Parameter ``depth``:
    controls the depth of the proliferation, as described above; this
    must be strictly positive.)doc";

// Docstring regina::python::doc::GroupPresentation_::recogniseGroup
static const char *recogniseGroup =
R"doc(Attempts to recognise the group corresponding to this presentation.
This routine is much more likely to be successful if you have already
called intelligentSimplify().

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

// Docstring regina::python::doc::GroupPresentation_::relation
static const char *relation =
R"doc(Returns the relation at the given index in this group presentation.
The relation will be of the form ``expresson = 1``.

Parameter ``index``:
    the index of the desired relation; this must be between 0 and
    countRelations()-1 inclusive.

Returns:
    the expression that the requested relation sets to 1; for
    instance, if the relation is ``g1^2 g2 = 1`` then this will be the
    expression ``g1^2 g2``.)doc";

// Docstring regina::python::doc::GroupPresentation_::relations
static const char *relations =
R"doc(Returns the list of all relations in this group presentation.

Python:
    This routine returns a python list of copied GroupExpression
    objects. In particular, modifying this list or the relations
    within it will not modify the group presentation from which they
    came.

Python:
    The list itself is not returned by reference (instead this routine
    returns a new Python list). However, the relations within this
    list are still returned by reference.

Returns:
    the list of relations.)doc";

// Docstring regina::python::doc::GroupPresentation_::relatorLength
static const char *relatorLength =
R"doc(The sum of the word lengths of the relators. Word lengths are
computing using GroupExpression::wordLength(). Used as a coarse
measure of the complexity of the presentation.

Returns:
    the sum of word lengths.)doc";

// Docstring regina::python::doc::GroupPresentation_::simplifyWord
static const char *simplifyWord =
R"doc(Uses small cancellation theory to reduce the input word, using the
current presentation of the group. The input word will be modified
directly.

.. warning::
    This routine is only as good as the relator table for the group.
    You might want to consider running intelligentSimplify(), possibly
    in concert with proliferateRelators(), before using this routine
    for any significant tasks.

Parameter ``input``:
    is the word you would like to simplify. This must be a word in the
    generators of this group.

Returns:
    ``True`` if and only if the input word was modified.)doc";

// Docstring regina::python::doc::GroupPresentation_::smallCancellation
static const char *smallCancellation =
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

.. note::
    If you all care about is whether the presentation changed, you can
    simply cast the return value to a ``bool``. This will then mirror
    the behaviour of smallCancellation() from Regina 6.0 and earlier,
    when the return type was simply ``bool``.

Returns:
    an isomorphism describing the reduction map from the original
    presentation to the new presentation, or no value if this
    presentation was not changed.)doc";

// Docstring regina::python::doc::GroupPresentation_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given group presentation.

Parameter ``other``:
    the group presentation whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::GroupPresentation_::tex
static const char *tex =
R"doc(Returns a TeX representation of this group presentation. See
writeTeX() for details on how this is formed.

Returns:
    a TeX representation of this group presentation.)doc";

// Docstring regina::python::doc::GroupPresentation_::writeXMLData
static const char *writeXMLData =
R"doc(Writes a chunk of XML containing this group presentation.

Python:
    The argument *out* should be an open Python file object.

Parameter ``out``:
    the output stream to which the XML should be written.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

