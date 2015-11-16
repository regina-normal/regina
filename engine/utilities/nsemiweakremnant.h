#ifndef __NSEMIWEAKREMNANT_H
#ifndef __DOXYGEN
#define __NSEMIWEAKREMNANT_H
#endif

#include <boost/version.hpp>
#if BOOST_VERSION >= 105500
#define USE_BOOST_INTRUSIVE_REF_COUNTER
#endif

#ifdef USE_BOOST_INTRUSIVE_REF_COUNTER
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#else
#include "utilities/nintrusiverefcounter.h"
#endif

namespace regina {

template <class> class NSemiWeakBase;

/**
 * A helper class for \c NSemiWeakPtr. The dereferencing of a \c NSemiWeakPtr
 * is indirected through this class, i.e., \c NSemiWeakPtr is pointing to this
 * class, which is pointing to the pointee, an object deriving from
 * \c NSemiWeakBase.
 * The life time of an object of this class is tied to the existence of
 * \c NSemiWeakPtr's pointing to it (through reference counting), not to that of
 * the corresponding pointee.
 */

template <class T>
class NSemiWeakRemnant :
#ifdef USE_BOOST_INTRUSIVE_REF_COUNTER
    public boost::intrusive_ref_counter<NSemiWeakRemnant<T>> {
#else
    public regina::temporary::NIntrusiveRefCounter<NSemiWeakRemnant<T>> {
#endif

public:
    /**
     * Given an \p object deriving from \c NSemiWeakBase, find or create the
     * remnant corresponding the object.
     */
    static NSemiWeakRemnant<T>* getOrCreate(T* object);
    ~NSemiWeakRemnant();
    /**
     * Dereference, i.e., find the pointee corresponding to the remnant.
     */
    T* get() const;
    /**
     * Expire the remnant so that it can no longer be dereferenced, i.e.,
     * get will return 0. Called by the pointee's destructor.
     */
    void expire();

private:
    NSemiWeakRemnant(T* object);
    T* object_;
    /**< The pointee. */
};

template <class T>
inline T* NSemiWeakRemnant<T>::get() const {
    return object_;
}

template <class T>
inline void NSemiWeakRemnant<T>::expire() {
    object_ = 0;
}

template <class T>
NSemiWeakRemnant<T>*
NSemiWeakRemnant<T>::getOrCreate(T* object) {
    // Each pointee \p object has at most one remnant. If it already exists,
    // ust it.
    if (object and object->remnant_) {
        return object->remnant_;
    }
    // Otherwise, create a new one associated with the pointee.
    return new NSemiWeakRemnant<T>(object);
}

template <class T>
NSemiWeakRemnant<T>::NSemiWeakRemnant(T* object) : object_(object) {
    // Associate this remnant with the pointee.
    // This is so that the pointee can expire this remnant upon its destruction.
    if (object) {
        object->remnant_ = this;
    }
}

template <class T>
NSemiWeakRemnant<T>::~NSemiWeakRemnant() {
    // Disassociate with pointee.
    if (object_) {
        // Pointee's back-pointer to remnant will no longer be valid, reset
        // to zero so that pointee's destructor won't call it.
        object_->remnant_ = 0;
        // If no other C++ object claims ownership, delete the pointee.
        if (not object_->hasOwner()) {
            delete object_;
        }
    }
}

} // namespace regina

#endif
