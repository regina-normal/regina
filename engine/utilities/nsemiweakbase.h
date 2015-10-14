#ifndef __NSEMIWEAKBASE_H
#ifndef __DOXYGEN
#define __NSEMIWEAKBASE_H
#endif

namespace regina {

class NSemiWeakRemnant;

/**
 * A base class for objects to be referenceable by a \c NSemiWeakPtr (referred
 * to as pointee's of \c NSemiWeakPtr). Every derived class needs to implement
 * hasOwningParent to indicate whether any non-NSemiWeakPtr claims ownership of
 * it. Details of ownership semantics are explained in \c NSemiWeakPtr.
 *
 * The overhead introduced by subclassing from \c NSemiWeakBase without using
 * the features of the accompanying smart pointer \c NSemiWeakPtr are minimal:
 * one extra pointer that needs to be zero'd upon construction.
 *
 * Most classes subcleass from \c NSemiWeakBase for python wrapping.
 */
class NSemiWeakBase {
public:
    virtual ~NSemiWeakBase();

protected:
    /**
     * Default constructor.
     */
    NSemiWeakBase() : remnant_(0) { }

    /**
     * Every derived class must implement this method. It must return true
     * if some other C++ object has a non-NSemiWeakPtr pointer (e.g., raw
     * pointer) pointing to this object.
     */
    virtual bool hasOwningParent() const = 0;

private:
    // Prevent derived classes from accidentally calling copy constructor.
    // A derived classes copy constructor by default calls the above default
    // constructor, which it should because it sets the remnant_ to zero on
    // the copied object.
    NSemiWeakBase(const NSemiWeakBase &);
    
    friend class NSemiWeakRemnant;
    NSemiWeakRemnant *remnant_;
    /**< Points to the corresponding persistent object. */
};

} // namespace regina

#endif
