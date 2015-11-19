#include <type_traits>
#include <boost/python/def_visitor.hpp>

namespace regina {
namespace python {

/**
 * TODO: Document.
 */
enum EqualityType {
    /**
     * Indicates that, when comparing two python objects using == or !=, the
     * underlying C++ operators == and != will be used.
     */
    BY_VALUE = 1,
    /**
     * Indicates that, when comparing two python objects using == or !=, the
     * code will test whether or not these wrap the same underlying C++
     * object.  In other words, they will test whether the wrapped C++ pointers
     * are the same.
     */
    BY_REFERENCE = 2
};

/**
 * Adds appropriate == and != operators to the python bindings for
 * Regina's C++ classes.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>c.def(regina::python::add_eq_operators())</t>, where \a c is the
 * boost::python::class_ object that wraps \a T.  The effect will be as follows:
 *
 * - If \a T provides both == and != operators (either as member
 *   functions or as global functions), then the python operators == and !=
 *   will compare by value.
 *
 * - If \a T provides neither == nor != operators, then the python
 *   operators == and != will compare by reference.
 *
 * - If \a T provides one of == or != but not the other, then this will
 *   generate a compile error.
 *
 * Furthermore, this will add an attribute \a equalityType to the python
 * wrapper class, which will be the corresponding constant from the
 * EqualityType enum (either \a BY_VALUE or \a BY_REFERENCE).
 */
struct add_eq_operators;

namespace add_eq_operators_detail {
    template <class T> void operator == (const T& a, const T& b);
    template <class T> void operator != (const T& a, const T& b);
    template <class T> const T& makeRef();

    template <class T,
        bool hasOperators = ! std::is_same<void,
            decltype(makeRef<T>() == makeRef<T>())>::value>
    struct EqualityOperators;

    template <class T>
    struct EqualityOperators<T, true> {
        static constexpr EqualityType equalityType() {
            return BY_VALUE;
        }

        // If we are instantiating this template then we already know
        // that type T offers an == operator.
        // Ensure that we have an != operator also.
        static_assert(! std::is_same<void,
            decltype(makeRef<T>() != makeRef<T>())>::value,
            "Wrapped C++ type implements == but not !=.");

        static bool are_equal(const T& a, const T& b) {
            return (a == b);
        }

        static bool are_not_equal(const T& a, const T& b) {
            return (a != b);
        }
    };

    template <class T>
    struct EqualityOperators<T, false> {
        static constexpr EqualityType equalityType() {
            return BY_REFERENCE;
        }

        // If we are instantiating this template then we know that type T
        // does not offer an == operator.
        // Ensure that there is no != operator either.
        static_assert(std::is_same<void,
            decltype(makeRef<T>() != makeRef<T>())>::value,
            "Wrapped C++ type implements != but not ==.");

        static bool are_equal(const T& a, const T& b) {
            return (&a == &b);
        }

        static bool are_not_equal(const T& a, const T& b) {
            return (&a != &b);
        }
    };
} // namespace add_eq_operators_detail

struct add_eq_operators : boost::python::def_visitor<add_eq_operators> {
    friend class boost::python::def_visitor_access;

    template <typename Class>
    void visit(Class& c) const {
        typedef typename Class::wrapped_type Type;

        c.def("__eq__",
            &add_eq_operators_detail::EqualityOperators<Type>::are_equal);
        c.def("__ne__",
            &add_eq_operators_detail::EqualityOperators<Type>::are_not_equal);
        c.attr("equalityType") =
            add_eq_operators_detail::EqualityOperators<Type>::equalityType();
    }
};

} } // namespace regina::python
