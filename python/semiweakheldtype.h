/*! \file python/semiwrakheldtype.h
 *  \brief Provides held type and policies for wrapping C++ classes in Python.
 */

#ifndef __SEMIWEAKHELDTYPE_H
#ifndef __DOXYGEN
#define __SEMIEWAKHELDTYPE_H
#endif

#include "utilities/nsemiweakptr.h"

#include <typeinfo>

namespace boost {
namespace python {

template<class T> struct pointee;

struct default_call_policies;

} } // namespace boost::python

namespace regina {
    
namespace python {

/**
 * A subclass of \c NSemiWeakPtr that throws a python exception when trying
 * to dereference an expired smart pointer (i.e., after some C++ class called
 * the destructor on the pointee).
 */ 
template<class T> class SemiWeakHeldType :
    public regina::NSemiWeakPtr<T> {

public:
    SemiWeakHeldType(T*);
    template<class Y> SemiWeakHeldType(const SemiWeakHeldType<Y>&);
    T* get() const;
};

/**
 * An implementation of boost::python's ResultConverter concept that takes a
 * raw pointer of an object derived from \c NSemiWeakBase and casts it to the
 * HeldType before converting it to a PyObject using the Base result converter.
 */

template<class HeldType, typename T, class Base>
struct to_held_type_result_converter : Base {
    // - HeldType is, e.g., a smart pointer to the class we try to wrap
    // - T is a raw pointer to the class we try to wrap
    // - Base is an existing result converter taking the HeldType
    to_held_type_result_converter() {}
    to_held_type_result_converter(T t) : Base(HeldType(t)) {}
};

/**
 * An implementation of boost::python's ResultConverterGenerator that uses the
 * \c to_held_type_result_converter.
 */

template<template<class U> class HeldType = regina::python::SemiWeakHeldType,
         class Base = boost::python::default_call_policies>
struct to_held_type {
    // - U is the class we try to wrap
    // - HeldType is, e.g., a smart pointer to that class
    // - Base is a call policy. This used to get a result converter which is
    //   used by to_held_type_result_converter:
    //   to_held_type_result_converter first converts a row pointer to the
    //   HeldType and then applies the Base's result converter to obtain a
    //   PyObject.
    
    // Result converter generator from Base
    typedef typename Base::result_converter base_generator;
    template<class T> struct apply {
        // - T is a raw pointer to the class we try to wrap.
        
        // The type of the class we try to wrap.
        typedef typename boost::python::pointee<T>::type pointee_type;
        // The type for holding that class
        typedef HeldType<pointee_type> HeldTypeT;
        // The result converter from the Base call policy.
        typedef typename base_generator::template apply<HeldTypeT>::type
            base_converter;
        // And finally, our result converter.
        typedef to_held_type_result_converter<HeldTypeT, T, base_converter>
            type;
    };
};

// A helper to raise a python exception indicating what type of object we tried
// to dereference.
void raiseExpiredException(const std::type_info&);

} } // namespace regina::python

namespace boost {

// Let boost infrastructure know that \c SemiWeakHeldType is derefrencable.
// Note that get() is not virtual, so we need to overwrite the earlier
// get_pointer for the base class \c NSemiWeakPtr.
template<class T>
T* get_pointer(regina::python::SemiWeakHeldType<T> const &ptr) {
    return ptr.get();
}
    
namespace python {

// Let boost::python infrastructure know.
template<class T> struct pointee<regina::python::SemiWeakHeldType<T> > {
    typedef T type;
};

} } // namespace boost::python

namespace regina {
namespace python {

template<class T> SemiWeakHeldType<T>::SemiWeakHeldType(T* t)
    : regina::NSemiWeakPtr<T>(t) { }
        
template<class T> template<class Y>
SemiWeakHeldType<T>::SemiWeakHeldType(const SemiWeakHeldType<Y> &other)
    : regina::NSemiWeakPtr<T>(other) { }

template<class T> T* SemiWeakHeldType<T>::get() const {
    T* t = regina::NSemiWeakPtr<T>::get();
    if (t == 0) {
        raiseExpiredException(typeid(T));
    }
    return t;
}

} } // namespace regina::python

#endif
