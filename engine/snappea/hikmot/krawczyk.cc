#if 0
#include <Python.h>
#endif
#include <iostream>
#include "kv/kraw-approx.hpp"
#include "kv/rdouble.hpp"
#include "kv/complex.hpp"

namespace ub = boost::numeric::ublas;

typedef regina::kv::interval<double> itvd;

namespace regina {

class Krawczyk {
	public:

	ub::matrix<int> a;
	ub::matrix<int> b;
	ub::vector<int> c;
	int n;

	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		int i, j;
		ub::vector<T> y(n);
		T left, right;

		for (i=0; i<n; i++) {
			left = 1.;
			right = c(i);
			for (j=0; j<n; j++) {
				if (a(i,j) > 0) {
					left *= pow(x(j), a(i,j));
				}
				if (a(i,j) < 0) {
					right *= pow(x(j), -a(i,j));
				}
				if (b(i,j) > 0) {
					left *= pow(1. - x(j), b(i,j));
				}
				if (b(i,j) < 0) {
					right *= pow(1. - x(j), -b(i,j));
				}
			}
			y(i) = left - right;
		}
		return y;
	}
};


// convert n-n complex function to 2n-2n real function

template <class F> class ComplexReal {
	public:

	F f;

	ComplexReal(F f_v) : f(f_v) {}

	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		int n = x.size();
		int m = n / 2;
		int i;
		ub::vector<T> y(n);
		ub::vector< regina::kv::complex<T> > xc(m);
		ub::vector< regina::kv::complex<T> > yc(m);

		for (i=0; i<m; i++) {
			xc(i) = regina::kv::complex<T>(x(i*2), x(i*2+1));
		}

		yc = f(xc);

		for (i=0; i<m; i++) {
			y(i*2) = yc(i).real();
			y(i*2+1) = yc(i).imag();
		}

		return y;
	}
};

} // namespace regina


#if 0
// python interface

PyObject* krawczyk(PyObject* self, PyObject* args)
{

	PyObject* list;
	PyObject* value;
	PyObject* result;
	int i, j, n;
	int count;

	ub::vector<double> x;
	ub::vector<itvd> ix, X;
	ub::matrix<double> R;
	bool r;


	value = PyTuple_GetItem(args, 0);
	n = PyInt_AsLong(value);

	list = PyTuple_GetItem(args, 1);

	Krawczyk f;
	f.n = n;
	f.a.resize(n,n);
	f.b.resize(n,n);
	f.c.resize(n);
    

	count = 0;

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			value = PyList_GetItem(list, count++);
			f.a(i,j) = PyInt_AsLong(value);
		}
	}

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			value = PyList_GetItem(list, count++);
			f.b(i,j) = PyInt_AsLong(value);
		}
	}

	for (i=0; i<n; i++) {
		value = PyList_GetItem(list, count++);
		f.c(i) = PyInt_AsLong(value);
	}

	ComplexReal<Krawczyk> g(f);

	x.resize(n * 2);
	ix.resize(n * 2);
	X.resize(n * 2);
	R.resize(n * 2, n * 2);

	for (i=0; i < n * 2; i++) {
		value = PyList_GetItem(list, count++);
		x(i) = PyFloat_AsDouble(value);
	}
	r = kv::krawczyk_approx(g, x, ix, X, R, 5, 0);
	//x: polised approx. sol.
	//X: initial interval
	//R: approximation of inverse of Jacobian
	//ix: result of verification
	
	//std::cout << "x: " << x << "\n";
	//std::cout << "X: " << X << "\n";
	//std::cout << "R: " << R << "\n";
	//std::cout << "K_F(X): " << ix << "\n";

    
	result = PyList_New( 10 * n + 4 * n * n + 1);

	count = 0;
	for (i=0; i < n * 2; i++) {
		value = PyFloat_FromDouble(x(i));
		PyList_SetItem(result, count++, value);
	}

	for (i=0; i < n * 2; i++) {
		value = PyFloat_FromDouble(X(i).lower());
		PyList_SetItem(result, count++, value);
		value = PyFloat_FromDouble(X(i).upper());
		PyList_SetItem(result, count++, value);
	}

	for (i=0; i < n * 2; i++) {
		for (j=0; j < n * 2; j++){
			value = PyFloat_FromDouble(R(i,j));
			PyList_SetItem(result, count++, value);
		}
	}

	for (i=0; i < n * 2; i++) {
		value = PyFloat_FromDouble(ix(i).lower());
		PyList_SetItem(result, count++, value);
		value = PyFloat_FromDouble(ix(i).upper());
		PyList_SetItem(result, count++, value);
	}

	if (r == true) value = PyInt_FromLong(1);
	else value = PyInt_FromLong(0);
	PyList_SetItem(result, count++, value);

	return result;
}


// python interface
PyObject* atan2(PyObject* self, PyObject* args)
{
	PyObject* list;
	PyObject* value;
	PyObject* result;

	int i, j, n;
	int count;

	double a_d, a_u, b_d, b_u;

	itvd a,b,r;

	value = PyTuple_GetItem(args, 0);
	a_d = PyFloat_AsDouble(value);
	value = PyTuple_GetItem(args, 1);
	a_u = PyFloat_AsDouble(value);
	value = PyTuple_GetItem(args, 2);
	b_d = PyFloat_AsDouble(value);
	value = PyTuple_GetItem(args, 3);
	b_u = PyFloat_AsDouble(value);

	a = itvd(a_d,a_u);
	b = itvd(b_d,b_u);

	r = atan2(a,b);
    
	result = PyList_New(2);

	value = PyFloat_FromDouble(r.lower());
	PyList_SetItem(result, 0, value);
	value = PyFloat_FromDouble(r.upper());
	PyList_SetItem(result, 1, value);

	return result;
}

static PyMethodDef krawczykmethods[] = {
	{"krawczyk", krawczyk, METH_VARARGS},
	{"atan2", atan2, METH_VARARGS},
	{NULL}
};

extern "C" void initkrawczyk() {
	Py_InitModule("krawczyk", krawczykmethods);
}
#endif
