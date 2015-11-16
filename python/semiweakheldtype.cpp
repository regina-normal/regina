#include <Python.h>

#include "semiweakheldtype.h"


#include <boost/version.hpp>
#if BOOST_VERSION >= 105600
#define USE_BOOST_DEMANGLE
#endif
#ifdef USE_BOOST_DEMANGLE
#include <boost/core/demangle.hpp>
#endif



namespace regina {
namespace python {

void raiseExpiredException(const std::type_info& info)
{
    #ifdef USE_BOOST_DEMANGLE
    const std::string typeName = boost::core::demangle(info.name());
    #else
    const std::string typeName = info.name();
    #endif

    const std::string msg =
        "Python reference to object of type " + typeName + " expired.";
    
    PyErr_SetString(PyExc_RuntimeError, msg.c_str());
}

} } // regina::python
