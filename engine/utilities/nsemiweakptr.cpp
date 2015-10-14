#include "utilities/nsemiweakptr.h"
#include "utilities/nsemiweakremnant.h"

namespace regina {

NSemiWeakPtrBase::NSemiWeakPtrBase(const NSemiWeakPtrBase& other)
    : remnant_(other.remnant_) { }

NSemiWeakPtrBase::NSemiWeakPtrBase(NSemiWeakBase* object) {
    if (object) {
        remnant_.reset(NSemiWeakRemnant::getOrCreate(object));
    }
}
    
NSemiWeakPtrBase::~NSemiWeakPtrBase() { }

NSemiWeakBase* NSemiWeakPtrBase::getBase_() const {
    if (not remnant_) {
        return 0;
    }
    return remnant_->get();
}

} // namespace regina
