/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SigCensus
static const char *SigCensus =
R"doc(A class responsible for building censuses of splitting surface
signatures.

The main entry point for building a census is the static routine
formCensus(). End users cannot create their own objects of this class.

See formCensus() for further information.)doc";

namespace SigCensus_ {

// Docstring regina::python::doc::SigCensus_::formCensus
static const char *formCensus =
R"doc(Forms a census of all splitting surface signatures of the given order.
The order of a signature is the number of quads in the corresponding
splitting surface.

Each signature will be produced precisely once up to equivalence.
Signatures are considered equivalent if they are related by some
combination of:

* relabelling symbols;

* rotating an individual cycle;

* inverting an individual cycle (i.e., reversing the cycle and
  changing the case of each symbol in the cycle);

* reversing all cycles without changing the case of any symbols.

Each signature produced will have its cycles ordered by decreasing
length. Each cycle will have at least half of its symbols lower-case.

For each signature that is generated, this routine will call *action*
(which must be a function or some other callable type).

* The first argument to *action* must be a const reference to a
  Signature. This will be the signature that was found. If *action*
  wishes to keep the signature, it should take a deep copy (not a
  reference), since the signature may be changed and reused after
  *action* returns.

* The second argument to *action* must be a const reference to a
  SigCensus::IsoList. This will be the list of all automorphisms of
  the signature that was found. Again, if *action* wishes to keep
  these automorphisms, it should take a deep copy of this list.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* The return value of *action* will be ignored; typically it would
  return ``void``.

.. warning::
    Currently upper-case symbols in signatures are not supported by
    this routine; only signatures whose symbols are all lower-case
    will be produced.

Python:
    This function is available, and *action* may be a pure Python
    function. However, *action* cannot take any additional arguments
    beyond the signature and its automorphisms (and therefore the
    additional *args* list is omitted here).

Parameter ``order``:
    the order of signatures to generate.

Parameter ``action``:
    a function (or other callable type) to call for each signature
    that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial signature and automorphism arguments.

Returns:
    the total number of non-equivalent signatures that were found.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

