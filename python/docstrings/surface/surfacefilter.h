/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SurfaceFilter
static const char *SurfaceFilter =
R"doc(A packet that accepts or rejects normal surfaces. Different subclasses
of SurfaceFilter represent different filtering methods.

When deriving classes from SurfaceFilter:

* Add a new filter constant to the SurfaceFilterType enum;

* Create a new subclass *C* of SurfaceFilter, which begins with the
  REGINA_SURFACE_FILTER macro;

* Perform all tasks required for this new innate packet type *C*, as
  outlined in the Packet class documentation;

* Override the virtual function writeTextShort(), as well as all pure
  virtual functions from both the Packet and SurfaceFilter base
  classes (except for those already provided by REGINA_PACKET and
  REGINA_SURFACE_FILTER).

Like all packet types, Regina's filter types do not support C++ move
semantics, since this would interfere with the structure of the packet
tree. They do support copy construction, copy assignment and swaps,
but only in the derived filter classes (e.g., you cannot assign from
the polymorphic base class SurfaceFilter). Moreover, these operations
only copy/swap the filter content, not the packet infrastructure
(e.g., they do not touch packet labels, or the packet tree, or event
listeners).)doc";

// Docstring regina::python::doc::SurfaceFilterCombination
static const char *SurfaceFilterCombination =
R"doc(A normal surface filter that simply combines other filters. This
filter will combine, using boolean *and* or *or*, all of the filters
that are immediate children of this packet. This packet may have
children that are not normal surface filters; such children will
simply be ignored.

If there are no immediate child filters, a normal surface will be
accepted if this is an *and* filter and rejected if this is an *or*
filter.

Like all packet types, this class does not support C++ move semantics
since this would interfere with the structure of the packet tree. It
does support copy construction, copy assignment and swaps; however,
these operations only copy/swap the mathematical content, not the
packet infrastructure (e.g., they do not touch packet labels, or the
packet tree, or event listeners).)doc";

// Docstring regina::python::doc::SurfaceFilterProperties
static const char *SurfaceFilterProperties =
R"doc(A normal surface filter that filters by basic properties of the normal
surface.

If a property of the surface (such as Euler characteristic or
orientability) cannot be determined, the surface will pass any test
based on that particular property. For instance, say a surface is
required to be both orientable and compact, and say that orientability
cannot be determined. Then the surface will be accepted solely on the
basis of whether or not it is compact.

Like all packet types, this class does not support C++ move semantics
since this would interfere with the structure of the packet tree. It
does support copy construction, copy assignment and swaps; however,
these operations only copy/swap the mathematical content, not the
packet infrastructure (e.g., they do not touch packet labels, or the
packet tree, or event listeners).)doc";

// Docstring regina::python::doc::swap
static const char *swap =
R"doc(Swaps the contents of the given combination filters.

This global routine simply calls SurfaceFilterCombination::swap(); it
is provided so that SurfaceFilterCombination meets the C++ Swappable
requirements.

Parameter ``a``:
    the first filter whose contents should be swapped.

Parameter ``b``:
    the second filter whose contents should be swapped.)doc";

// Docstring regina::python::doc::swap_2
static const char *swap_2 =
R"doc(Swaps the contents of the given property-based filters.

This global routine simply calls SurfaceFilterProperties::swap(); it
is provided so that SurfaceFilterProperties meets the C++ Swappable
requirements.

Parameter ``a``:
    the first filter whose contents should be swapped.

Parameter ``b``:
    the second filter whose contents should be swapped.)doc";

namespace SurfaceFilterCombination_ {

// Docstring regina::python::doc::SurfaceFilterCombination_::__eq
static const char *__eq =
R"doc(Determines if this and the given filter use the same boolean
operation.

Parameter ``other``:
    the filter to compare with this.

Returns:
    ``True`` if and only if this and the given filter use the same
    boolean operation.)doc";

// Docstring regina::python::doc::SurfaceFilterCombination_::__init
static const char *__init =
R"doc(Creates a new surface filter that accepts all normal surfaces. This
will be an *and* filter.)doc";

// Docstring regina::python::doc::SurfaceFilterCombination_::__init_2
static const char *__init_2 =
R"doc(Creates a new copy of the given filter.

Like all packet types, this only copies the filter content, not the
packet infrastructure (e.g., it will not copy the packet label, it
will not clone the given packet's children, and it will not insert the
new packet into any packet tree).)doc";

// Docstring regina::python::doc::SurfaceFilterCombination_::__ne
static const char *__ne =
R"doc(Determines if this and the given filter do not use the same boolean
operation.

Parameter ``other``:
    the filter to compare with this.

Returns:
    ``True`` if and only if this and the given filter use different
    boolean operations.)doc";

// Docstring regina::python::doc::SurfaceFilterCombination_::setUsesAnd
static const char *setUsesAnd =
R"doc(Sets whether this is an *and* or an *or* combination.

Parameter ``value``:
    ``True`` if this is to be an *and* combination, or ``False`` if
    this is to be an *or* combination.)doc";

// Docstring regina::python::doc::SurfaceFilterCombination_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given filter.

Like all packet types, this only swaps the filter content, not the
packet infrastructure (e.g., it will not swap packet labels, or change
either packet's location in any packet tree).

Parameter ``other``:
    the filter whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::SurfaceFilterCombination_::usesAnd
static const char *usesAnd =
R"doc(Determines whether this is an *and* or an *or* combination.

Returns:
    ``True`` if this is an *and* combination, or ``False`` if this is
    an *or* combination.)doc";

}

namespace SurfaceFilterProperties_ {

// Docstring regina::python::doc::SurfaceFilterProperties_::__eq
static const char *__eq =
R"doc(Determines if this and the given filter are configured to filter on
the same set of constraints.

Even if both filters are designed to reject *every* surface by having
an empty BoolSet for one of the boolean conditions, they will still
not compare as equal unless they use an empty BoolSet for the *same*
boolean condition(s). In other words, this test compares the precise
configurations of the filters, not their deduced behaviour.

Parameter ``other``:
    the filter to compare with this.

Returns:
    ``True`` if and only if this and the given filters are identical.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::__init
static const char *__init = R"doc(Creates a new surface filter that accepts all normal surfaces.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::__init_2
static const char *__init_2 =
R"doc(Creates a new copy of the given filter.

Like all packet types, this only copies the filter content, not the
packet infrastructure (e.g., it will not copy the packet label, it
will not clone the given packet's children, and it will not insert the
new packet into any packet tree).)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::__ne
static const char *__ne =
R"doc(Determines if this and the given filter are configured to filter on
the different sets of constraints.

Even if both filters are designed to reject *every* surface by having
an empty BoolSet for one of the boolean conditions, they will still
compare as different unless they use an empty BoolSet for the *same*
boolean condition(s). In other words, this test compares the precise
configurations of the filters, not their deduced behaviour.

Parameter ``other``:
    the filter to compare with this.

Returns:
    ``True`` if and only if this and the given filters are not
    identical.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::addEulerChar
static const char *addEulerChar =
R"doc(Adds the given Euler characteristic to the set of allowable Euler
characteristics. See eulerChars() for further details.

Parameter ``ec``:
    the new allowable Euler characteristic.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::compactness
static const char *compactness =
R"doc(Returns the set of allowable compactness properties. Note that this is
a subset of ``{ true, false }``. Any surface whose compactness
property is not in this set will not be accepted by this filter.

Returns:
    the set of allowable compactness properties.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::countEulerChars
static const char *countEulerChars =
R"doc(Returns the number of allowable Euler characteristics. See
eulerChars() for further details.

Returns:
    the number of allowable Euler characteristics.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::eulerChar
static const char *eulerChar =
R"doc(Returns the allowable Euler characteristic at the given index in the
set. See eulerChars() for further details.

Parameter ``index``:
    the index in the set of allowable Euler characteristics; this must
    be between 0 and countEulerChars()-1 inclusive.

Returns:
    the requested allowable Euler characteristic.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::eulerChars
static const char *eulerChars =
R"doc(Returns the set of allowable Euler characteristics. Any surface whose
Euler characteristic is not in this set will not be accepted by this
filter. The set will be given in ascending order with no element
repeated.

If this set is empty, all Euler characteristics will be accepted.

Returns:
    the set of allowable Euler characteristics.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::orientability
static const char *orientability =
R"doc(Returns the set of allowable orientabilities. Note that this is a
subset of ``{ true, false }``. Any surface whose orientability is not
in this set will not be accepted by this filter.

Returns:
    the set of allowable orientabilities.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::realBoundary
static const char *realBoundary =
R"doc(Returns the set of allowable has-real-boundary properties. Note that
this is a subset of ``{ true, false }``. Any surface whose has-real-
boundary property is not in this set will not be accepted by this
filter.

Returns:
    the set of allowable has-real-boundary properties.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::removeAllEulerChars
static const char *removeAllEulerChars =
R"doc(Empties the set of allowable Euler characteristics. See eulerChars()
for further details.

Note that this will mean that this filter will allow *any* Euler
characteristic to pass.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::removeEulerChar
static const char *removeEulerChar =
R"doc(Removes the given Euler characteristic from the set of allowable Euler
characteristics. See eulerChars() for further details.

Note that if the allowable set is completely emptied, this filter will
allow *any* Euler characteristic to pass.

Precondition:
    The given Euler characteristic is currently in the allowable set.

Parameter ``ec``:
    the allowable Euler characteristic to remove.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::setCompactness
static const char *setCompactness =
R"doc(Sets the set of allowable compactness properties. See compactness()
for further details.

Parameter ``value``:
    the new set of allowable compactness properties.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::setEulerChars
static const char *setEulerChars =
R"doc(Sets the allowable Euler characteristics to be all integers in the
given iterator range. See eulerChars() for further details.

Python:
    Instead of a pair of iterators, the set of allowable Euler
    characteristic should be passed as a Python list.

Template parameter ``Iterator``:
    an iterator type that, when dereferenced, can be assigned to a
    LargeInteger.

Parameter ``beginEuler``:
    the beginning of an iterator range that gives the new set of
    allowable Euler characteristics.

Parameter ``endEuler``:
    the end of an iterator range (i.e., an iterator past the end of
    the list) that gives the new set of allowable Euler
    characteristics.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::setOrientability
static const char *setOrientability =
R"doc(Sets the set of allowable orientabilities. See orientability() for
further details.

Parameter ``value``:
    the new set of allowable orientabilities.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::setRealBoundary
static const char *setRealBoundary =
R"doc(Sets the set of allowable has-real-boundary properties. See
realBoundary() for further details.

Parameter ``value``:
    the new set of allowable has-real-boundary properties.)doc";

// Docstring regina::python::doc::SurfaceFilterProperties_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given filter.

Like all packet types, this only swaps the filter content, not the
packet infrastructure (e.g., it will not swap packet labels, or change
either packet's location in any packet tree).

Parameter ``other``:
    the filter whose contents should be swapped with this.)doc";

}

namespace SurfaceFilter_ {

// Docstring regina::python::doc::SurfaceFilter_::accept
static const char *accept =
R"doc(Decides whether or not the given normal surface is accepted by this
filter.

The default implementation simply returns ``True``.

Parameter ``surface``:
    the normal surface under investigation.

Returns:
    ``True`` if and only if the given surface is accepted by this
    filter.)doc";

// Docstring regina::python::doc::SurfaceFilter_::filterType
static const char *filterType =
R"doc(Returns the unique integer ID corresponding to the filtering method
that is this particular subclass of SurfaceFilter.

Returns:
    the unique integer filtering method ID.)doc";

// Docstring regina::python::doc::SurfaceFilter_::filterTypeName
static const char *filterTypeName =
R"doc(Returns a string description of the filtering method that is this
particular subclass of SurfaceFilter.

Returns:
    a string description of this filtering method.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

