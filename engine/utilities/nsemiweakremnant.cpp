#include "utilities/nsemiweakremnant.h"
#include "utilities/nsemiweakbase.h"

namespace regina {

/* static */
NSemiWeakRemnant*
NSemiWeakRemnant::getOrCreate(NSemiWeakBase* object) {
    // Each pointee \p object has at most one remnant. If it already exists,
    // ust it.
    if (object and object->remnant_) {
        return object->remnant_;
    }
    // Otherwise, create a new one associated with the pointee.
    return new NSemiWeakRemnant(object);
}

NSemiWeakRemnant::NSemiWeakRemnant(NSemiWeakBase* object) : object_(object) {
    // Associate this remnant with the pointee.
    // This is so that the pointee can expire this remnant upon its destruction.
    if (object) {
        object->remnant_ = this;
    }
}

NSemiWeakRemnant::~NSemiWeakRemnant() {
    // Disassociate with pointee.
    if (object_) {
        // Pointee's back-pointer to remnant will no longer be valid, reset
        // to zero so that pointee's destructor won't call it.
        object_->remnant_ = 0;
        // If no other C++ object claims ownership, delete the pointee.
        if (not object_->hasOwningParent()) {
            delete object_;
        }
    }
}
    
} // namespace regina
