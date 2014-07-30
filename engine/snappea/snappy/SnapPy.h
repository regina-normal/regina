/**
 *  Headers for additional SnapPy code that is not shipped with the
 *  SnapPea kernel.
 */

#ifndef _SnapPy_
#define _SnapPy_

#include "../kernel/SnapPea.h"
#include "../kernel/kernel_namespace.h"

/* Prototypes for functions defined in gluing_equations.c */

extern int** get_gluing_equations(Triangulation* manifold, 
				  int* num_rows, 
				  int* num_cols);

extern void free_gluing_equations(int** equations, 
				  int num_rows);

extern int* get_cusp_equation(Triangulation* manifold,
			      int cusp_num,
			      int m,
			      int l,
			      int* num_rows);

extern void free_cusp_equation(int* equation);

#include "../kernel/end_namespace.h"

#endif
