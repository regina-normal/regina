#include "utilities/nsemiweakbase.h"
#include "utilities/nsemiweakremnant.h"

namespace regina {

/* virtual */
NSemiWeakBase::~NSemiWeakBase() {
    // If existing, expire the remnant. Thus, all NSemiWeakPtr's pointing to
    // this object know that they cannot be dereferenced anylonger.
    if (remnant_) {
        remnant_->expire();
    }
}
    

} // namespace regina
