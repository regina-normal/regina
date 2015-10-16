#ifndef __NINTRUSIVEREFCOUNTER_H
#ifndef __DOXYGEN
#define __NINTRUSIVEREFCOUNTER_H
#endif

namespace regina {
namespace temporary {

/**
 * A crude replacement for boost::intrusive_ref_counter so that the code
 * compiles against versions of boost prior 1.55. It is only a stop-gap
 * measure.
 */

template<typename T> class NIntrusiveRefCounter {
public:
    NIntrusiveRefCounter() : refCounter(0) { }
    mutable int refCounter;
};

} } // namespace regina::temporary

namespace boost {

template<typename T>
inline void intrusive_ptr_add_ref(const ::regina::temporary::NIntrusiveRefCounter<T>* p) {
    p->refCounter++;
}

template<typename T>
inline void intrusive_ptr_release(const ::regina::temporary::NIntrusiveRefCounter<T>* p) {
    p->refCounter--;
    if (p->refCounter == 0) {
        delete static_cast<const T*>(p);
    }
}

} // namespace boost

#endif
