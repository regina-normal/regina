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

class NSemiWeakBase;

/**
 * A helper class for \c NSemiWeakPtr. The dereferencing of a \c NSemiWeakPtr
 * is indirected through this class, i.e., \c NSemiWeakPtr is pointing to this
 * class, which is pointing to the pointee, an object deriving from
 * \c NSemiWeakBase.
 * The life time of an object of this class is tied to the existence of
 * \c NSemiWeakPtr's pointing to it (through reference counting), not to that of
 * the corresponding pointee.
 */

class NSemiWeakRemnant :
#ifdef USE_BOOST_INTRUSIVE_REF_COUNTER
    public boost::intrusive_ref_counter<NSemiWeakRemnant> {
#else
    public regina::temporary::NIntrusiveRefCounter<NSemiWeakRemnant> {
#endif

public:
    /**
     * Given an \p object deriving from \c NSemiWeakBase, find or create the
     * remnant corresponding the object.
     */
    static NSemiWeakRemnant* getOrCreate(NSemiWeakBase* object);
    ~NSemiWeakRemnant();
    /**
     * Dereference, i.e., find the pointee corresponding to the remnant.
     */
    NSemiWeakBase* get() const;
    /**
     * Expire the remnant so that it can no longer be dereferenced, i.e.,
     * get will return 0. Called by the pointee's destructor.
     */
    void expire();

private:
    NSemiWeakRemnant(NSemiWeakBase* object);
    NSemiWeakBase* object_;
    /**< The pointee. */
};

inline NSemiWeakBase* NSemiWeakRemnant::get() const {
    return object_;
}

inline void NSemiWeakRemnant::expire() {
    object_ = 0;
}
    
} // namespace regina

#endif
