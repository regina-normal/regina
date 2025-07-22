
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file python/helpers/exception.h
 *  \brief Assists with wrapping Regina's exception classes.
 *
 *  This header is _not_ included automatically by python/helpers.h.
 *  If you need it, you will need to include it yourself.
 */

#include <pybind11/pybind11.h>
#if REGINA_PYBIND11_VERSION == 3
#include <pybind11/subinterpreter.h>
#include <mutex>
#endif
#include "utilities/exception.h"
#include "../helpers/docstrings.h"

namespace regina::python {

#if REGINA_PYBIND11_VERSION == 3
/**
 * For a single C++ exception type, holds a corresponding Python exception
 * type for each subinterpreter.
 *
 * All insertion and lookup operations are thread-safe.
 */
class ExceptionCache {
    private:
        std::map<int64_t, PyObject*> cache_;
            /**< A map from subinterpreter IDs to Python exception types. */
        std::mutex mutex_;
            /**< Protects cache_. */

    public:
        ExceptionCache() = default;

        void insert(PyObject* exc) {
            int64_t id = pybind11::subinterpreter::current().id();

            std::scoped_lock lock(mutex_);
            auto result = cache_.emplace(id, exc);
            if (! result.second) {
                // An exception is already registered for this subinterpreter.
                // This should only happen if regina's module was loaded,
                // unloaded, and is now being loaded again.  We definitely
                // want the new value -- the old value is a stale pointer
                // which we can safely discard.
                result.first->second = exc;
            }
        }

        PyObject* lookup() {
            // This function is non-const because we need to lock mutex_.
            int64_t id = pybind11::subinterpreter::current().id();

            std::scoped_lock lock(mutex_);
            auto it = cache_.find(id);
            return (it == cache_.end() ? nullptr : it->second);
        }
};
#endif

/**
 * Adds Python bindings for one of Regina's C++ exception types.
 *
 * This routine will create a corresponding Python exception type (using
 * PyExc_RuntimeError as its base), and will register a translator between
 * the two.
 *
 * Conceptually, this does a similar job to `pybind11::register_exception()`;
 * however, it is also safe to use with subinterpreters (which, as of
 * pybind11 3.0.0rc3, `pybind11::register_exception()` is not).
 */
template <typename ReginaExceptionType>
void registerReginaException(pybind11::module_& m, const char* className,
        const char* docstring) {
#if REGINA_PYBIND11_VERSION == 3
    // In pybind11 v3 (at least in 3.0.0rc3), the implementation of
    // register_exception() uses a global singleton Python exception object,
    // which breaks things when we load the module multiple times in different
    // subinterpreters.  We will need to implement things manually ourselves,
    // with a different Python exception object for each subinterpreter.
    //
    // TODO: This code does come with a small memory leak, in that once a
    // subinterpreter is destroyed, the corresponding entry in cache is not
    // removed (although the Python exception type should be destroyed).
    // For now, we live with this memory leak: it is only relevant when using
    // subinterpreters in the GUI, and these are created manually by the user
    // when opening a new python console -- this means there should never be
    // so many that this becomes a serious problem.

    static ExceptionCache cache;

    // The following exception type should live for only as long as the module
    // is loaded, since it is the reference in m.__dict__ that keeps it alive
    // on the python side.  In our own cache we just keep a borrowed reference.
    pybind11::exception<ReginaExceptionType> exc(m, className, docstring,
        PyExc_RuntimeError);
    cache.insert(exc.ptr());

    pybind11::register_exception_translator([](std::exception_ptr p) {
        if (!p) {
            return;
        }
        try {
            std::rethrow_exception(p);
        } catch (const ReginaExceptionType &e) {
            if (PyObject* exc = cache.lookup())
                PyErr_SetString(exc, e.what());
            else {
                // This should never happen.  But.. just in case, translate
                // this to a standard python RuntimeError instead.
                PyErr_SetString(PyExc_RuntimeError, e.what());
            }
        }
    });
#elif REGINA_PYBIND11_VERSION == 2
    // pybind11 v2 does not support subinterpreters properly; however,
    // register_exception() does appear to work perfectly fine with the
    // subinterpreter hacks that we are using.
    pybind11::register_exception<ReginaExceptionType>(m, className,
        PyExc_RuntimeError).doc() = docstring;
#else
    #error "Unsupported pybind11 version"
#endif
}

} // namespace regina::python

