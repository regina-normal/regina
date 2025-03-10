
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file python/helpers/gil.h
 *  \brief Assists with managing the Python global interpreter lock (GIL).
 */

#include <map>
#include <mutex>
#include <thread>

namespace regina::python {

/**
 * An object that acquires the GIL on construction, and releases it
 * on destruction.
 *
 * This works similar to pybind11::gil_scoped_acquire.  The advantage of
 * this class is that it works correctly with subinterpreters (which
 * pybind11's GIL-related classes do not).
 *
 * An object of this type cannot be copied or moved.
 */
class GILScopedAcquire {
    public:
        /**
         * Acquires the GIL in order to work with the given Python
         * thread state.
         *
         * \pre The GIL is not held when this constructor is called.
         * \pre The argument \a state is a Python thread state that has
         * already been created, and this constructor is called from the
         * corresponding C++ thread.
         */
        GILScopedAcquire(PyThreadState* state) {
            PyEval_RestoreThread(state);
        }

        /**
         * Releases the GIL.
         *
         * \pre The GIL is held when this destructor is called.
         */
        ~GILScopedAcquire() {
            PyEval_SaveThread();
        }

        GILScopedAcquire(const GILScopedAcquire&) = delete;
        GILScopedAcquire& operator = (const GILScopedAcquire&) = delete;
};

/**
 * An object that releases the GIL on construction, and re-acquires it
 * on destruction.
 *
 * This works similar to pybind11::gil_scoped_release.  The advantage of
 * this class is that it works correctly with subinterpreters (which
 * pybind11's GIL-related classes do not).  The disadvantage is that it
 * insists that the same C++ thread be used for construction and destruction
 * (which for a stack variable is typically true anyway).
 *
 * An object of this type cannot be copied or moved.
 */
class GILScopedRelease {
    private:
        PyThreadState *state;
            /**< The Python thread state that was current when this
                 object was created. */

    public:
        /**
         * Releases the GIL.
         *
         * \pre The GIL is held when this constructor is called.
         */
        GILScopedRelease() {
            state = PyEval_SaveThread();
        }

        /**
         * Re-acquires the GIL.
         *
         * \pre The GIL is not held when this destructor is called.
         * \pre This destructor is being called from the same C++ thread
         * as the corresponding constructor.
         */
        ~GILScopedRelease() {
            PyEval_RestoreThread(state);
        }

        GILScopedRelease(const GILScopedRelease&) = delete;
        GILScopedRelease& operator = (const GILScopedRelease&) = delete;
};

/**
 * An object designed for use with C++ callback functions, which releases the
 * GIL during its lifespan but allows the GIL to be temporarily re-acquired
 * when callbacks are executed (possibly from within different threads).
 *
 * An object of this class is designed to work with a C++ function \a f
 * (which may be multithreaded), where \a f takes a C++ callback function
 * \a c as one of its arguments.  The order in which things should happen is:
 *
 * - We assume that the GIL is already held.  We refer to the current
 *   C++ thread as the \a main thread.
 *
 * - A GILCallbackManager \a m is created.  This has the side-effect of
 *   releasing the GIL.
 *
 * - The C++ function \a f is called, with the callback \a c as one of its
 *   arguments.  The function \a f is allowed to be multithreaded.
 *
 * - Within the execution of \a f, the function \a c is called.  This could
 *   happen many times.  Each time \a c is called, it might happen within the
 *   main thread, or within some other thread that was created by \a f (and
 *   will later be destroyed by \a f also).
 *
 * - The callback \a c should do the following:
 *
 *   - Create a temporary GILCallbackManager::ScopedAcquire object.  This
 *     has the side-effect of re-acquiring the GIL, as well as creating a new
 *     Python thread state if the current C++ thread is not the main thread
 *     and has not yet been seen during the lifespan of \a m.
 *
 *   - Do its actual (e.g., mathematical) work.
 *
 *   - Destroy the temporary GILCallbackManager::ScopedAcquire object.
 *     This has the side-effect of releasing the GIL.
 *
 * - Once \a f has finished, the manager \a m is destroyed within the main
 *   thread.  This has the side-effect of re-acquiring the GIL, as well as
 *   destroying all of the new Python thread states that were created during
 *   callbacks from within other threads.
 *
 * It must be emphasised that, although \a c may be called from a thread
 * \a t that is not the main thread, this thread \a t _must_ be created and
 * destroyed by the main function \a f.  That is, the thread \a t must not
 * have interacted with Python before, and it must not interact with Python
 * again afterwards.  Of the course this thread \a t may be reused multiple
 * times for callbacks to \a c while the function \a f is running.
 *
 * An object of this type cannot be copied or moved.
 *
 * \tparam multithreaded indicates whether this class supports callbacks that
 * are executed from C++ threads other than the main thread.  By default this
 * is supported (as described above).  However, if you know that the entire
 * function \a f will be single-threaded then you can set this to \c false;
 * the result will be slightly faster code (i.e., less overhead), but with the
 * caveat that each GILCallbackManager::ScopedAcquire will throw an exception
 * if it is created outside the main thread (as opposed to the usual behaviour
 * of creating and/or managing a new Python thread state).
 */
template <bool multithreaded = true>
class GILCallbackManager {
    private:
        std::thread::id mainThread_;
            /**< The main C++ thread. */
        PyThreadState* mainState_;
            /**< The Python thread state corresponding to the main C++ thread,
                 whose lifespan extends beyond this manager object. */
        std::map<std::thread::id, PyThreadState*> subStates_;
            /**< The Python thread states corresponding to non-main threads,
                 which this manager has created and must later destroy. */
        std::mutex mutex_;
            /**< Protects \a subStates_. */

    public:
        /**
         * Constructs a new callback manager and releases the GIL.
         *
         * \pre The GIL is held when this constructor is called.
         */
        GILCallbackManager() :
                mainThread_(std::this_thread::get_id()),
                mainState_(PyEval_SaveThread()) {
        }

        /**
         * Destroys this callback manager and re-acquires the GIL.
         *
         * \pre The GIL is not held when this constructor is called.
         * \pre This destructor is being called from the same C++ thread
         * as the corresponding constructor.
         */
        ~GILCallbackManager() {
            PyEval_RestoreThread(mainState_);

            // Clean up all the states from threads that we will no longer use.
            // This must be done whilst holding the GIL.
            for (auto& s : subStates_) {
                PyThreadState_Clear(s.second);
                PyThreadState_Delete(s.second);
            }
        }

        GILCallbackManager(const GILCallbackManager&) = delete;
        GILCallbackManager& operator = (const GILCallbackManager&) = delete;

        /**
         * An object that acquires the GIL on construction, and releases it
         * on destruction.
         *
         * This behaves similar to regina::python::GILScopedAcquire, except
         * that it does not require a pre-existing Python thread state to be
         * passed on construction.  Instead this class manages the creation
         * and destruction of Python thread states through an associated
         * GILCallbackManager, as described in the GILCallbackManager
         * class notes.
         *
         * An object of this type cannot be copied or moved.
         */
        class ScopedAcquire {
            public:
                /**
                 * Acquires the GIL.
                 *
                 * \pre The GIL is not held when this constructor is called.
                 * \pre This constructor is being called from a C++ thread that
                 * is either (i) the main thread associated with \a manager,
                 * or else (ii) a new C++ thread that was created during
                 * the lifespan of \a manager.  See the GILCallbackManager
                 * class notes for further details.
                 */
                ScopedAcquire(GILCallbackManager& manager_) {
                    auto id = std::this_thread::get_id();
                    if (id == manager_.mainThread_) {
                        PyEval_RestoreThread(manager_.mainState_);
                    } else {
                        PyThreadState* state;
                        {
                            std::scoped_lock lock(manager_.mutex_);
                            auto pos = manager_.subStates_.find(id);
                            if (pos == manager_.subStates_.end()) {
                                state = PyThreadState_New(
                                    manager_.mainState_->interp);
                                manager_.subStates_.emplace(id, state);
                            } else {
                                state = pos->second;
                            }
                        }
                        PyEval_RestoreThread(state);
                    }
                }

                /**
                 * Releases the GIL.
                 *
                 * \pre The GIL is held when this destructor is called.
                 */
                ~ScopedAcquire() {
                    PyEval_SaveThread();
                }

                ScopedAcquire(const ScopedAcquire&) = delete;
                ScopedAcquire& operator = (const ScopedAcquire&) = delete;
            };
};

template <>
class GILCallbackManager<false> {
    private:
        std::thread::id mainThread_;
        PyThreadState* mainState_;

    public:
        GILCallbackManager() :
                mainThread_(std::this_thread::get_id()),
                mainState_(PyEval_SaveThread()) {
        }

        ~GILCallbackManager() {
            PyEval_RestoreThread(mainState_);
        }

        GILCallbackManager(const GILCallbackManager&) = delete;
        GILCallbackManager& operator = (const GILCallbackManager&) = delete;

        class ScopedAcquire {
            public:
                ScopedAcquire(GILCallbackManager& manager_) {
                    if (std::this_thread::get_id() == manager_.mainThread_)
                        PyEval_RestoreThread(manager_.mainState_);
                    else
                        throw std::runtime_error("A non-multithreaded "
                            "GILCallbackManager observed a ScopedAcquire "
                            "object being created from a different thread.");
                }

                ~ScopedAcquire() {
                    PyEval_SaveThread();
                }

                ScopedAcquire(const ScopedAcquire&) = delete;
                ScopedAcquire& operator = (const ScopedAcquire&) = delete;
            };
};

} // namespace regina::python
