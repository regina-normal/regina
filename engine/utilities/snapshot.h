
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file utilities/snapshot.h
 *  \brief Provides a mechanism for snapshotting objects at a particular
 *  moment in time.
 */

#ifndef __REGINA_SNAPSHOT_H
#ifndef __DOXYGEN
#define __REGINA_SNAPSHOT_H
#endif

#include <atomic>
#include <exception>

namespace regina {

class SnapshotWriteError;

template <class T> class SnapshotRef;
template <class T> class Snapshottable;

/**
 * An exception thrown when someone tries to modify the read-only deep copy
 * taken by a snapshot.  See the Snapshot class notes for more information.
 *
 * \ingroup utilities
 */
class SnapshotWriteError : public std::exception {
    public:
        /**
         * Creates a new exception.
         */
        SnapshotWriteError() noexcept = default;

        /**
         * Creates a new copy of the given exception.
         */
        SnapshotWriteError(const SnapshotWriteError&) noexcept = default;

        /**
         * Sets this to be a copy of the given exception.
         */
        SnapshotWriteError& operator = (const SnapshotWriteError&) noexcept =
            default;

        /**
         * Returns a human-readable description of the error that occurred.
         */
        const char* what() const noexcept override {
            return "Snapshot detected modification of its internal deep copy";
        }
};

/**
 * Keeps a snapshot of an object of type \a T as it was at a particular
 * moment in time.
 *
 * To describe how this class works, we need some terminology:
 *
 * - the \e image is a single object of type \a T whose snapshot we are taking;
 *
 * - the \e viewers are many object of other types that all require access to
 *   this snapshot.
 *
 * The life cycle of this process is as follows:
 *
 * - An image \a I is created and modified over time; initially it
 *   remains uninvolved in the snapshotting machinery.
 *
 * - At some point in time, a viewer \a V1 wishes to take a snapshot of \a I.
 *   To do this, it creates a <i>snapshot reference</i> <tt>SnapshotRef(I)</tt>.
 *   This is a cheap operation that "enrols" \a I in the snapshotting machinery,
 *   by creating a single Snapshot object \a S.
 *
 * - More viewers may take snapshots of \a I, either by creating a new
 *   <tt>SnapshotRef(I)</tt> or by copying other viewers' snapshot references.
 *   Again, these are all cheap operations.  All references to \a I will
 *   refer to the same snapshot object \a S.
 *
 * - If the image is about to modify itself or be destroyed, it notifies its
 *   snapshot \a S, which in turn takes a deep copy of \a I and stores
 *   it for safekeeping.  This is an expensive operation.  The original
 *   image now loses its link to \a S, and becomes "unenrolled" from the
 *   snapshotting machinery again; the only way to access the original
 *   snapshot at this point is by copying other references.
 *
 * - After the image was modified, making a new <tt>SnapshotRef(I)</tt> will
 *   re-enrol \a I and create a completely new Snapshot object.  The original
 *   Snapshot may of course still exist, maintaining its copy of \a I as it
 *   used to be.
 *
 * - Each snapshot \a S is reference counted: when the last reference to it is
 *   destroyed, then \a S is also destroyed (along with the deep copy of the
 *   original image, if one was ever made).
 *
 * Regarding access to the image:
 *
 * - A Snapshot and SnapshotRef can outlive the original image \a I.
 *
 * - It is important that every snapshot reference only ever accesses the
 *   underlying image via the SnapshotRef dereference operators.  This is
 *   because the image may change to be a different object if the original
 *   is modified or destroyed.
 *
 * - Snapshot references are only ever granted \e read-only access to the image.
 *   Semantically this makes sense (since this is a snapshot); also this
 *   avoids unpleasant lifespan questions if someone tries to modify a
 *   Snapshot's internal deep copy.  If you need write access from a snapshot,
 *   then take your own deep copy and modify that: it is no less efficient
 *   (since the snapshot would have taken a copy otherwise), and this way
 *   the ownership and lifespan semantics are clear.  Any attempt to
 *   cast away constness and modify the image through a snapshot may
 *   result in a regina::SnapshotWriteError exception being thrown.
 *
 * This snapshotting machinery is, in some ways, a slimmed-down variant of
 * copy-on-write machinery, designed to be easily plugged into type \a T.
 * The requirement on \a T are as follows:
 *
 * - \a T must derive from the base class Snapshottable<T>.  The overhead is
 *   one extra pointer to the storage for \a T.
 *
 * - If \a T supports move/copy construction, move/copy assignment and/or
 *   swapping, then their implementations must call the corresponding
 *   operations from the base class Snapshottable<T>.
 *
 * - In particular, \a T must have a copy constructor.  This will be used by
 *   the snapshot whenever it needs to take its own deep copy.
 *
 * - Whenever an object of type \a T changes, it \e must call
 *   Snapshottable<T>::takeSnapshot() from within the modifying member
 *   function, \e before the change takes place (though there are a handful
 *   of exceptions to this requirement, described in the Snapshottable class
 *   notes).  If the object does not have a current snapshot, this is very fast
 *   (a single test for a null pointer).  If the object does have a current
 *   snapshot, then this will be expensive since it will trigger a deep copy.
 *
 * - Likewise, in the destructor for \a T, the first call should be to
 *   Snapshottable<T>::takeSnapshot().
 *
 * - If you are doing a complex series of modifications, it is not essential
 *   to worry about multiple calls to takeSnapshot() coming from nested
 *   modificiation routines, since only the first of these calls could be
 *   expensive.  (This is in contrast, for instance, to the packet listener
 *   machinery, where nested modifications need to be managed more carefully
 *   because firing a packetToBeChanged() event will always iterate through
 *   all registered listeners.)
 *
 * Regarding multithreading:
 *
 * - In general, this class is \e not thread-safe; in particular, the
 *   code that creates new snapshots, takes deep copies before modification,
 *   destroys snapshots, and enrols/unenrols images from the snapshot
 *   machinery is all unsafe for multithreading.
 *
 * - However: the reference counting machinery \e is thread-safe.
 *   This means that, if your image is not being modified and you already have
 *   one snapshot reference \a R, it is safe to create more references,
 *   access the image through your references, and/or destroy references,
 *   all from multiple threads, as long as one reference always remains.
 *
 * - This means that, again assuming your image is not being modified,
 *   you can hold on to a particular snapshot reference \a R and use that
 *   as a "temporary guarantee" of thread-safety.
 *
 * - All of this requires a bit of care.  However, the trade-off is that
 *   frequent operations (including Snapshottable<T>::takeSnapshot() which
 *   must be called on every object of type \a T before every modification,
 *   even if no snapshots exist), can take place without the overhead of
 *   locking and unlocking mutexes.
 *
 * This Snapshot class itself should remain forever behind the scenes:
 * end users cannot access it and should not know about it.
 * Images should always work through their base class Snapshottable<T>,
 * and viewers shoudl always work through SnapshotRef<T>.
 *
 * \ifacespython Not present.
 *
 * \ingroup utilities
 */
template <class T>
class Snapshot {
    static_assert(std::is_base_of_v<Snapshottable<T>, T>,
        "Snapshot<T> requires T to be derived from Snapshottable<T>.");

    private:
        const T* value_;
            /**< The object as it was when this snapshot was created.
                 We maintain this as a two-way link: value_->snapshot_
                 must also point to this object. */
        bool owner_;
            /**< This is false if value_ is the original image, or true if
                 value_ is our own deep copy. */
        std::atomic<int> refCount_;
            /**< The number of SnapshotRefs that point to this snapshot. */

        /**
         * Creates a new snapshot pointing to the given image, which we
         * assume has not yet been modified.  It is assumed that this
         * snapshot was triggered by the creation of a new SnapshotRef,
         * and so the initial reference count will be set to 1.
         */
        Snapshot(const T* src) : value_(src), owner_(false), refCount_(1) {
        }
        /**
         * Destroys this snapshot.  If we took our own deep copy then this
         * is destroyed also; otherwise the image is completely unenrolled
         * from the snapshotting machinery.
         */
        ~Snapshot() {
            // Even if we are about to destroy value_, we still need to
            // unenrol it first so that its destructor does not attempt to
            // trigger a deep copy.
            value_->snapshot_ = nullptr;
            if (owner_)
                delete value_;
        }
        /**
         * Triggers the creation of a deep copy of the image.
         * This is called from Snapshottable<T> whenever \a value is
         * about to be modified or destroyed.
         *
         * If we already \e have a deep copy, then this is an error:
         * it means the call came from within the deep copy, which should be
         * read-only and which we ourselves are responsible for destroying.
         */
        void freeze() {
            if (owner_) {
                throw SnapshotWriteError();
            } else {
                value_->snapshot_ = nullptr;
                value_ = new T(*value_);
                value_->snapshot_ = this;
                owner_ = true;
            }
        }

    friend class Snapshottable<T>;
    friend class SnapshotRef<T>;
};

/**
 * A base class for images of type \a T that can be snapshotted at a particular
 * moment in time.
 *
 * See the Snapshot documentation for a full explanation of how Regina's
 * snapshotting machinery works.
 *
 * As a quick recap, the requirements for the class \a T are:
 *
 * - \a T must derive from Snapshottable<T>, and must have a copy constructor.
 *
 * - If \a T supports any move, copy and/or swap operations, then these must
 *   call the corresponding operations from Snapshottable<T>.
 *
 * - Every modifying member function must call Snapshottable<T>::takeSnapshot()
 *   before the modification takes place.  There are a handful of exceptions
 *   to this requirement, as described below.
 *
 * - The destructor must likewise call Snapshottable<T>::takeSnapshot() before
 *   any data is destroyed.
 *
 * There are some situations where an object of type \a T is modified but
 * does \e not need to call takeSnapshot().  These include:
 *
 * - move, copy and swap operations, since these are required to call the
 *   base class implementations from Snapshottable<T>, which take care of
 *   any snapshotting that is required;
 *
 * - modifications of objects that are freshly constructed, and cannot
 *   possibly have snapshots that refer to them yet.
 *
 * \ifacespython Not present.
 *
 * \ingroup utilities
 */
template <class T>
class Snapshottable {
    private:
        mutable Snapshot<T>* snapshot_;
            /**< The unique snapshot of this object in its present state,
                 or \c null if this object is not currently enrolled in
                 the snapshotting machinery. */

    protected:
        /**
         * Default constructor.  The new object will be unenrolled from
         * the snapshotting machinery.
         */
        Snapshottable() : snapshot_(nullptr) {
        }

        /**
         * Copy constructor.  The new object will be unenrolled from the
         * snapshotting machinery, even if the source object does have a
         * current snapshot.
         */
        Snapshottable(const Snapshottable&) : snapshot_(nullptr) {
        }

        /**
         * Move constructor.
         *
         * This should \e only be called when the entire type \a T contents
         * of \a src are being moved into this new type \a T object.  If \a src
         * has a current snapshot, then this object will move in as the new
         * image for that \e same snapshot.  This avoids a deep copy of \a src,
         * even though \a src is changing (and presumably will be destroyed).
         *
         * In particular, if the move constructor for \a T calls this
         * base class constructor (as it should), then there is no need
         * for it to call takeSnapshot() from \a src.
         *
         * @param src the snapshot image being moved.
         */
        Snapshottable(Snapshottable&& src) noexcept : snapshot_(src.snapshot_) {
            if (snapshot_) {
                snapshot_->value_ = static_cast<T*>(this);
                src.snapshot_ = nullptr;
            }
        }

        /**
         * Copy assignment.
         *
         * The previous value will have its snapshot taken if necessary,
         * since it will be overwritten.  Therefore, if the copy assignment
         * operator for \a T calls this base class operator (as it should),
         * then it does not need to call takeSnapshot() itself.
         *
         * The new value will left unenrolled from the snapshotting machinery.
         *
         * This is safe to use with self-assignment, but this will still
         * behave as though the object were being modified (i.e., it may
         * still take a snapshot).
         *
         * @return a reference to this object.
         */
        // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
        Snapshottable& operator = (const Snapshottable&) {
            if (snapshot_) {
                snapshot_->freeze();
                snapshot_ = nullptr;
            }
            return *this;
        }

        /**
         * Move assignment.
         *
         * The previous value will have its snapshot taken if necessary,
         * since it will be overwritten.  Therefore, if the move assignment
         * operator for \a T calls this base class operator (as it should),
         * then it does not need to call takeSnapshot() itself.
         *
         * This should \e only be called when the entire type \a T contents
         * of \a src are being moved into this type \a T object.  If \a src
         * has a current snapshot, then this object will move in as the new
         * image for that \e same snapshot.  This avoids a deep copy of \a src,
         * even though \a src is changing (and presumably will be destroyed).
         *
         * In particular, if the move assignment operator for \a T calls this
         * base class operator (again, as it should), then there is no need
         * for it to call takeSnapshot() from \a src.
         *
         * This operator is not marked \c noexcept, since it might create a
         * deep copy of the old type \a T value of this object.
         *
         * @param src the snapshot image being moved.
         * @return a reference to this object.
         */
        Snapshottable& operator = (Snapshottable&& src) {
            if (snapshot_)
                snapshot_->freeze();
            snapshot_ = src.snapshot_;
            if (snapshot_) {
                snapshot_->value_ = static_cast<T*>(this);
                src.snapshot_ = nullptr;
            }
            return *this;
        }

        /**
         * Swap operation.
         *
         * This should \e only be called when the entire type \a T contents
         * of this object and \a other are being swapped.  If one object has
         * a current snapshot, then the other object will move in as the new
         * image for that \e same snapshot.  This avoids a deep copies of
         * this object and/or \a other, even though both objects are changing.
         *
         * In particular, if the swap function for \a T calls this base class
         * function (as it should), then there is no need to call
         * takeSnapshot() from either this object or \a src.
         *
         * @param other the snapshot image being swapped with this.
         */
        void swap(Snapshottable& other) noexcept {
            std::swap(snapshot_, other.snapshot_);
            if (snapshot_)
                snapshot_->value_ = static_cast<T*>(this);
            if (other.snapshot_)
                other.snapshot_->value_ = static_cast<T*>(&other);
        }

        /**
         * Must be called before modification and/or destruction of the
         * type \a T contents.  See the Snapshot class notes for a full
         * explanation of how this requirement works.
         *
         * There are a few exceptions where takeSnapshot() does not need
         * to be called: these are where type \a T move, copy and/or swap
         * operations call the base class operations (as they should).
         * See the Snapshottable move, copy and swap functions for details.
         *
         * If this object has a current snapshot, then this function will
         * trigger a deep copy with the snapshot.
         *
         * After this function returns, this object is guaranteed to be
         * completely unenrolled from the snapshotting machinery.
         */
        void takeSnapshot() {
            if (snapshot_)
                snapshot_->freeze();
        }

        /**
         * Determines if this object is a read-only deep copy that was created
         * by a snapshot.
         *
         * Recall that, if an image \a I of type \a T has a snapshot pointing
         * to it, and if that image \a I is about to be modified or destroyed,
         * then the snapshot will make an internal deep copy of \a I and refer
         * to that instead.
         *
         * The purpose of this routine is to identify whether the current
         * object is such a deep copy.  This may be important information,
         * since a snapshot's deep copy is read-only: it must not be modified
         * or destroyed by the outside world.  (Of course the only way to
         * access this deep copy is via const reference from the SnapshotRef
         * dereference operators, but there are settings in which this
         * constness is "forgotten", such as Regina's Python bindings.)
         *
         * \ifacespython Although this Snapshottable base class is not
         * visible to Python, the isReadOnlySnapshot() function is still
         * visible as a member function of the class \a T.
         *
         * @return \c true if and only if this object is a deep copy that was
         * taken by a Snapshot object of some original type \a T image.
         */
        bool isReadOnlySnapshot() const {
            return snapshot_ && snapshot_->owner_;
        }

    private:
        /**
         * Returns the current snapshot for this object, creating one if
         * necessary, and increments its reference count.
         *
         * This should only be called when creating a new SnapshotRef.
         *
         * @return the snapshot for this object.  This is guaranteed to
         * be non-null, and to have a positive reference count.
         */
        Snapshot<T>* addSnapshotRef() const {
            if (snapshot_)
                ++snapshot_->refCount_;
            else
                snapshot_ = new Snapshot<T>(static_cast<const T*>(this));
            return snapshot_;
        }

    friend class Snapshot<T>;
    friend class SnapshotRef<T>;
};

/**
 * A reference to an image of type \a T that has been snapshotted at a
 * particular moment in time.
 *
 * See the Snapshot documentation for a full explanation of how Regina's
 * snapshotting machinery works.
 *
 * As a quick recap:
 *
 * - There can be many SnapshotRef references all pointing to the same image.
 *   The underlying snapshot is guaranteed to exist and maintain the same value
 *   for as long as there is some SnapshotRef that refers to it, even if the
 *   original type \a T object has since been modified or destroyed.
 *
 * - The user of a SnapshotRef \a R must only access the snapshotted image
 *   through the dereference operators <tt>*R</tt> and <tt>R-&gt;</tt>.
 *   This access must remain read-only, and any attempt to circumvent it
 *   could lead to a regina::SnapshotWriteError exception being thrown.
 *
 * - The user of a SnapshotRef should not keep a pointer or reference to the
 *   underlying type \a T object, since the snapshot may switch to a different
 *   internal type \a T object if the original is ever modified or destroyed.
 *
 * These references are lightweight objects that can be freely copied by
 * value.  They do also offer move and swap operations that in some cases
 * are more efficient (though all copy, move and swap operations are fast).
 *
 * \ifacespython Not present.
 *
 * \ingroup utilities
 */
template <class T>
class SnapshotRef {
    private:
        Snapshot<T>* snapshot_; // non-null
            /**< The snapshot that we are referencing.  This is held by
                 pointer to support assignment; it must not be \c null. */

    public:
        /**
         * Creates a new snapshot reference to the given object in its
         * current state.
         *
         * If you already have a snapshot reference \a R to the same object
         * in the same state, it is (slightly) cheaper to copy \a R instead
         * of going through the source object \a src.
         *
         * @param src the underlying type \a T object whose current
         * snapshot we wish to maintain a reference to.
         */
        SnapshotRef(const T& src) : snapshot_(src.addSnapshotRef()) {
        }
        /**
         * Creates a new copy of the given snapshot reference.
         *
         * The new reference will refer to the same object as \a src, as it
         * appeared at the same point in time.
         *
         * @param src the snapshot reference to copy.
         */
        SnapshotRef(const SnapshotRef& src) : snapshot_(src.snapshot_) {
            ++snapshot_->refCount_;
        }
        /**
         * Moves the given snapshot reference into this new reference.
         *
         * The new reference will refer to the same object as \a src, as it
         * appeared at the same point in time.
         *
         * This move constructor is identical to the copy constructor,
         * and there is no particular reason to call it.  It is included
         * here for consistency because the move \e assignment operator is
         * different from (and more efficient) than copy assignment.
         *
         * @param src the snapshot reference to move.
         */
        SnapshotRef(SnapshotRef&& src) noexcept : snapshot_(src.snapshot_) {
            // We don't do anything special here like preserving the reference
            // count, because this would require setting src_.snapshot_ to
            // null.  This is a problem because the destructor (which is
            // probably called more often) assumes that snapshot_ is non-null.
            //
            // So: we increment the reference count here, and src will
            // decrement it again when it is destroyed.
            ++snapshot_->refCount_;
        }
        /**
         * Destroys this snapshot reference.  If this is the last surviving
         * reference to the underlying snapshot, then the snapshot itself
         * (along with its internal deep copy of the original object, if
         * it made one) will be destroyed.
         */
        ~SnapshotRef() {
            if (--snapshot_->refCount_ == 0)
                delete snapshot_;
        }
        /**
         * Sets this to be a snapshot of the given object in its current state.
         *
         * If you already have a snapshot reference \a R to the same object
         * in the same state, it is (slightly) cheaper to assign from \a R
         * instead of assigning from the source object \a src.
         *
         * If the old value of this reference was the last surviving reference
         * to its underlying snapshot, then the old snapshot itself (along
         * with its internal deep copy of the original object, if it made one)
         * will be destroyed.
         *
         * Self-assignment (<tt>r = *r</tt>) is harmless, and will never cause
         * the underlying snapshot to be destroyed.
         *
         * @param src the underlying type \a T object whose current
         * snapshot we wish to make this a reference to.
         * @return a reference to this object.
         */
        SnapshotRef& operator = (const T& src) {
            if (snapshot_ != src.snapshot_) {
                if (--snapshot_->refCount_ == 0)
                    delete snapshot_;
                snapshot_ = src.addSnapshotRef();
            }
            return *this;
        }
        /**
         * Sets this to be a copy of the given snapshot reference.
         *
         * This reference will be made to refer to the same object as \a src,
         * as it appeared at the same point in time.
         *
         * If the old value of this reference was the last surviving reference
         * to its underlying snapshot, then the old snapshot itself (along
         * with its internal deep copy of the original object, if it made one)
         * will be destroyed.
         *
         * Self-assignment (<tt>r = r</tt>) is harmless, and will never cause
         * the underlying snapshot to be destroyed.
         *
         * @param src the snapshot reference to copy.
         * @return a reference to this object.
         */
        // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
        SnapshotRef& operator = (const SnapshotRef& src) {
            if (snapshot_ != src.snapshot_) {
                if (--snapshot_->refCount_ == 0)
                    delete snapshot_;
                snapshot_ = src.snapshot_;
                ++snapshot_->refCount_;
            }
            return *this;
        }
        /**
         * Moves the given snapshot reference into this new reference.
         *
         * The new reference will refer to the same object as \a src, as it
         * appeared at the same point in time.
         *
         * This is more efficient than copy assignment, since it avoids
         * atomic changes to reference counts.
         *
         * @param src the snapshot reference to move.
         * @return a reference to this object.
         */
        SnapshotRef& operator = (SnapshotRef&& src) noexcept {
            // Let src manage the destruction of the old snapshot,
            // if this is necessary.
            std::swap(snapshot_, src.snapshot_);
            return *this;
        }
        /**
         * Swaps this and the given reference so that they refer to each
         * others' snapshots.
         *
         * Self-swapping (<tt>r.swap(r)</tt>) is harmless, and will never
         * cause the underlying snapshot to be destroyed.
         *
         * This is more efficient than a sequence of copies, since it avoids
         * atomic changes to reference counts.
         *
         * @param other the reference to swap with this.
         */
        void swap(SnapshotRef& other) noexcept {
            std::swap(snapshot_, other.snapshot_);
        }
        /**
         * Tests whether this and the given snapshot reference refer to
         * the same snapshot of the same type \a T object.
         *
         * This will be true if and only if both references were obtained from
         * the same type \a T object (possibly via copies/moves/swaps from
         * other references), during the same time period in which the
         * underlying type \a T object was not modified.
         *
         * @param rhs the snapshot reference to compare with this.
         * @return \c true if and only if this and \a rhs refer to the
         * same snapshot of the same underlying type \a T object.
         */
        bool operator == (const SnapshotRef& rhs) const {
            return snapshot_ == rhs.snapshot_;
        }
        /**
         * Tests whether this and the given snapshot reference do not refer
         * to the same snapshot of the same type \a T object.
         *
         * This will be true if either the two references were obtained from
         * different type \a T objects (possibly via copies/moves/swaps from
         * other references), or if they were obtained from the same type \a T
         * object but at times that were separated by a modification of this
         * type \a T object.
         *
         * @param rhs the snapshot reference to compare with this.
         * @return \c true if and only if this and \a rhs do not refer to the
         * same snapshot of the same underlying type \a T object.
         */
        bool operator != (const SnapshotRef& rhs) const {
            return snapshot_ != rhs.snapshot_;
        }
        /**
         * Gives read-only access to the image that was snapshotted.
         *
         * Note that this dereference operator may refer to different objects
         * in memory at different times (according to how the snapshots are
         * being managed and copied behind the scenes).  Therefore you should
         * always access the snapshot through these dereference operators,
         * and never take your own reference or pointer to the underlying
         * type \a T object.
         *
         * @return a reference to the snapshot of the type \a T object.
         */
        const T& operator * () const {
            return *snapshot_->value_;
        }
        /**
         * Gives read-only access to the image that was snapshotted.
         *
         * Note that this dereference operator may refer to different objects
         * in memory at different times (according to how the snapshots are
         * being managed and copied behind the scenes).  Therefore you should
         * always access the snapshot through these dereference operators,
         * and never take your own reference or pointer to the underlying
         * type \a T object.
         *
         * @return a reference to the snapshot of the type \a T object.
         */
        const T* operator -> () const {
            return snapshot_->value_;
        }
};

/**
 * Swaps the given references so that they refer to each others' snapshots.
 *
 * This global routine simply calls SnapshotRef<T>::swap(); it is provided
 * so that SnapshotRef<T> meets the C++ Swappable requirements.
 *
 * See the Snapshot documentation for a full explanation of how Regina's
 * snapshotting machinery works.
 *
 * @param a the first snapshot reference to swap.
 * @param b the second snapshot reference to swap.
 *
 * \ingroup utilities
 */
template <class T>
void swap(SnapshotRef<T>& a, SnapshotRef<T>& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif
