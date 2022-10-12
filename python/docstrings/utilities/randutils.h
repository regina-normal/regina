/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::RandomEngine
static const char *RandomEngine =
R"doc(Offers threadsafe access to Regina's global uniform random bit
generator.

Regina has at its heart a global uniform random bit generator (URBG)
that complies with the C++ UniformRandomBitGenerator concept, and can
be used with other data types and algorithms from the standard
``random`` header.

An object of this class offers threadsafe access to this global URBG.

This class behaves in many ways like std::scoped_lock. On construction
a RandomEngine object will lock an internal mutex that grants
exclusive access to the global URBG, and on destruction the
RandomEngine object will unlock this mutex.

The engine() function grants direct access to the URBG. To ensure
thread safety, you should only use this URBG during the scope of the
RandomEngine object (i.e., while the internal mutex is locked). The
easiest way to do this is to never store a reference to engine() for
later use, but instead to pass engine() directly to other random
number generation functions as you use them.

The engine is seeded with a default value. This means that each time
you run the same code in a new process (but on the same machine), you
should receive the same sequence of random bits. However, the
generation algorithm may be specific to your machine, so running the
same code on different machines might well *not* generate the same
random bits.

If you need to re-seed the random engine with a value that is
unpredictable (e.g., using hardware entropy), you can call
reseedWithHardware(). If you wish to re-seed the random engine with
its default value (to behave as though the process had just started),
you can call reseedWithDefault().

RandomEngine objects are not copyable, movable or swappable. In
particular, Regina does not offer any way for a RandomEngine to
transfer its duty (i.e., unlocking the internal mutex upon
destruction) to another object.

.. warning::
    Locks are *not* recursive. If the same thread attempts to create a
    second RandomEngine object before the previous one is destroyed,
    the resulting behaviour is undefined.

Python:
    Python users only have access to the static member functions in
    this class (which still supports basic random number generation as
    well as reseeding).)doc";

namespace RandomEngine_ {

// Docstring regina::python::doc::RandomEngine_::rand
static const char *rand =
R"doc(A convenience function that returns a random integer modulo *range*,
in a thread-safe manner. The result will be between 0 and (*range* -
1) inclusive, and all such integers should be returned with equal
probability.

This is expensive because it locks and unlocks the internal mutex. If
you need to create many random numbers in quick succession, consider
creating a single RandomEngine object and accessing engine() directly
using the standard C++ randomness functions.

Python:
    The integer type *Int* will be treated as ``long``.

Template parameter ``Int``:
    a native integer type (e.g., ``int``, ``long``, ``size_t``, ``long
    long``, etc.); this may be either signed or unsigned.

Parameter ``range``:
    the size of the range of possible results; this must be strictly
    positive.

Returns:
    a random integer between 0 and (*range* - 1) inclusive.)doc";

// Docstring regina::python::doc::RandomEngine_::reseedWithDefault
static const char *reseedWithDefault =
R"doc(Reseeds the global uniform random bit generator using the default
seed. This should make the sequence of random bits completely
predictable between runs of the same program on the same machine.
(Note that even when using the same seed, different operating systems,
architectures and/or compilers may produce different random bits).

Regina seeds the global uniform random bit generator in this way on
startup, and so typically you would not need to call this routine
yourself.

This routine is thread-safe, and it locks the internal mutex while it
runs.)doc";

// Docstring regina::python::doc::RandomEngine_::reseedWithHardware
static const char *reseedWithHardware =
R"doc(Reseeds the global uniform random bit generator using hardware
entropy. This should make the sequence of random bits unpredictable
(and, in particular, different between runs of your program).

Regina does *not* do this by default.

This routine is thread-safe, and it locks the internal mutex while it
runs.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

