#ifndef __NSEMIWEAKPTR_H
#ifndef __DOXYGEN
#define __NSEMIWEAKPTR_H
#endif

#include <boost/intrusive_ptr.hpp>

namespace regina {

class NSemiWeakBase;
class NSemiWeakRemnant;

// Base class for \c NSemiWeakPtr. Users should always use \c NSemiWeakPtr.
class NSemiWeakPtrBase {
public:
    ~NSemiWeakPtrBase();

private:
    NSemiWeakPtrBase();

protected:
    // Create a \c NSemiWeakPtr with given pointee \p object.
    NSemiWeakPtrBase(NSemiWeakBase* object);
    NSemiWeakPtrBase(const NSemiWeakPtrBase &);
    // Get the pointee.
    NSemiWeakBase* getBase_() const;
private:
    boost::intrusive_ptr<NSemiWeakRemnant> remnant_;
    /**< The remnant that points to the pointee. */
};

/**
 * A reference counted smart pointer that allows its pointee to indicate that it
 * cannot be destroyed because some other C++ class not using this smart pointer
 * owns it:
 *
 * - A pointee must subclass from \c NSemiWeakBase and implement hasOwner
 *   to indicate whether it is not safe to destroy it because some other
 *   object is having a non-NSemiWeakPtr pointing to the pointee.
 * - If a pointee's destructor is called, all \c NSemiWeakPtr pointing to it
 *   will be expired and become non-dereferencable (get() returning 0).
 * - \c NSemiWeakPtr will destroy a pointee if the last \c NSemiWeakPtr pointing
 *   to the pointee goes out of scope and if the pointee's hasOwner
 *   returns false.
 *
 * Under the hood, \c NSemiWeakPtr is using \c NSemiWeakRemnant to achieve this.
 */

template<class T>
class NSemiWeakPtr : protected NSemiWeakPtrBase
{
public:
    // For the boost infrastructure.
    typedef T element_type;

    /**
     * Constructs a \c NSemiWeakPtr pointing at the given pointee \p object.
     */
    NSemiWeakPtr(T* object);
    /**
     * \c NSemiWeakPtr's can be cast to \c NSemiWeakPtr's for a base class.
     */
    template<class Y> NSemiWeakPtr(const NSemiWeakPtr<Y> &);
    /**
     * Return a raw pointer to the pointee, zero if pointee was destroyed.
     */
    T* get() const;
};

} // namespace regina

namespace boost {
// Dereferencable concept for \c NSemiWeakPtr's.
template<class T> T* get_pointer(regina::NSemiWeakPtr<T> const& ptr) {
    return ptr.get();
}
} // namespace boost

namespace regina {

template<class T> NSemiWeakPtr<T>::NSemiWeakPtr(T* object)
    : NSemiWeakPtrBase(object) { }

// This template can only be instantiated if T (whose constructor is called)
// is a base class of Y (returned by other.get()).
template<class T> template<class Y>
NSemiWeakPtr<T>::NSemiWeakPtr(const NSemiWeakPtr<Y> &other)
    : NSemiWeakPtr(other.get()) { }

// By virtue of how \c NSemiWeakPtr's are constructed, getBase_() always holds
// a pointer to T or a dervied class of T.
template<class T> T* NSemiWeakPtr<T>::get() const {
    return static_cast<T*>(getBase_());
}

} // namespace regina

#endif
