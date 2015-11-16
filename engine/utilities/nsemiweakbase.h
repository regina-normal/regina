#ifndef __NSEMIWEAKBASE_H
#ifndef __DOXYGEN
#define __NSEMIWEAKBASE_H
#endif

#include "utilities/nsemiweakremnant.h"

namespace regina {

/**
 * A base class for objects to be referenceable by a \c NSemiWeakPtr (referred
 * to as pointee's of \c NSemiWeakPtr). Every derived class needs to implement
 * hasOwner to indicate whether any non-NSemiWeakPtr claims ownership of
 * it. Details of ownership semantics are explained in \c NSemiWeakPtr.
 *
 * The overhead introduced by subclassing from \c NSemiWeakBase without using
 * the features of the accompanying smart pointer \c NSemiWeakPtr are minimal:
 * one extra pointer that needs to be zero'd upon construction.
 *
 * Most classes subcleass from \c NSemiWeakBase for python wrapping.
 */
template <class T>
class NSemiWeakBase {
public:
    ~NSemiWeakBase();

protected:
    /**
     * Default constructor.
     */
    NSemiWeakBase() : remnant_(0) { }

private:
    // Prevent derived classes from accidentally calling copy constructor.
    // A derived classes copy constructor by default calls the above default
    // constructor, which it should because it sets the remnant_ to zero on
    // the copied object.
    NSemiWeakBase(const NSemiWeakBase &);

    // Similarly, for operator=
    NSemiWeakBase & operator=(const NSemiWeakBase &);
    
    friend class NSemiWeakRemnant<T>;
    NSemiWeakRemnant<T> *remnant_;
    /**< Points to the corresponding persistent object. */
};

template <class T>
NSemiWeakBase<T>::~NSemiWeakBase() {
    // If existing, expire the remnant. Thus, all NSemiWeakPtr's pointing to
    // this object know that they cannot be dereferenced anylonger.
    if (remnant_) {
        remnant_->expire();
    }
}

} // namespace regina

#endif
