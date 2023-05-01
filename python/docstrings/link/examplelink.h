/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::ExampleLink
static const char *ExampleLink =
R"doc(This class offers routines for constructing ready-made examples of
knots and links. These examples may be useful for testing new code, or
for simply getting a feel for how Regina works.

The sample links offered here may prove especially useful in Regina's
scripting interface, where working with pre-existing files is more
complicated than in the GUI.)doc";

namespace ExampleLink_ {

// Docstring regina::python::doc::ExampleLink_::borromean
static const char *borromean =
R"doc(Returns a six-crossing diagram of the Borromean rings.

Returns:
    the Borromean rings.)doc";

// Docstring regina::python::doc::ExampleLink_::conway
static const char *conway =
R"doc(Returns the 11-crossing Conway knot.

This is the reflection of *K11n34* in the Knot Atlas, and is a mutant
of the Kinoshita-Terasaka knot.

Returns:
    the Conway knot.)doc";

// Docstring regina::python::doc::ExampleLink_::figureEight
static const char *figureEight =
R"doc(Returns a four-crossing diagram of the figure eight knot.

Returns:
    the figure eight knot.)doc";

// Docstring regina::python::doc::ExampleLink_::gordian
static const char *gordian =
R"doc(Returns Haken's Gordian unknot, a 141-crossing diagram of the unknot
that is difficult to untangle.

Returns:
    the Gordian unknot.)doc";

// Docstring regina::python::doc::ExampleLink_::gst
static const char *gst =
R"doc(Returns a 48-crossing potential counterexample to the slice-ribbon
conjecture, as described by Gompf, Scharlemann and Thompson.

Specifically, this knot is Figure 2 from their paper "Fibered knots
and potential counterexamples to the property 2R and slice-ribbon
conjectures", arXiv:1103.1601.

Returns:
    the Gompf-Scharlemann-Thompson knot.)doc";

// Docstring regina::python::doc::ExampleLink_::hopf
static const char *hopf =
R"doc(Returns a two-crossing diagram of the Hopf link. This is the variant
in which both crossings are positive.

Returns:
    the Hopf link.)doc";

// Docstring regina::python::doc::ExampleLink_::kinoshitaTerasaka
static const char *kinoshitaTerasaka =
R"doc(Returns the 11-crossing Kinoshita-Terasaka knot.

This is the reflection of *K11n42* in the Knot Atlas, and is a mutant
of the Conway knot. It has trivial Alexander polynomial.

Returns:
    the kinoshita-Terasaka knot.)doc";

// Docstring regina::python::doc::ExampleLink_::monster
static const char *monster =
R"doc(Returns the monster unknot, a 10-crossing diagram of the unknot that
is difficult to untangle.

Returns:
    the monster unknot.)doc";

// Docstring regina::python::doc::ExampleLink_::torus
static const char *torus =
R"doc(Returns the (*p*,*q*) torus link.

The parameters *p* and *q* must be non-negative, but they do not need
to be coprime.

All of the crossings in the resulting link will be positive.

Parameter ``p``:
    the first parameter of the torus link; this must be strictly non-
    negative.

Parameter ``q``:
    the second parameter of the torus link; this must also be strictly
    non-negative.

Returns:
    the (*p*, *q*) torus link.)doc";

// Docstring regina::python::doc::ExampleLink_::trefoil
static const char *trefoil =
R"doc(Returns a three-crossing diagram of the right-hand trefoil. This
returns the same knot as trefoilRight().

Returns:
    the right-hand trefoil.)doc";

// Docstring regina::python::doc::ExampleLink_::trefoilLeft
static const char *trefoilLeft =
R"doc(Returns a three-crossing diagram of the left-hand trefoil.

Returns:
    the left-hand trefoil.)doc";

// Docstring regina::python::doc::ExampleLink_::trefoilRight
static const char *trefoilRight =
R"doc(Returns a three-crossing diagram of the right-hand trefoil. This
returns the same knot as trefoil().

Returns:
    the right-hand trefoil.)doc";

// Docstring regina::python::doc::ExampleLink_::unknot
static const char *unknot =
R"doc(Returns a zero-crossing diagram of the unknot.

Returns:
    the unknot.)doc";

// Docstring regina::python::doc::ExampleLink_::whitehead
static const char *whitehead =
R"doc(Returns a five-crossing diagram of the Whitehead link.

Returns:
    the Whitehead link.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

