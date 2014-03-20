/**
 *  Headers for additional SnapPy code that is not shipped with the
 *  SnapPea kernel.
 */

#ifndef _SnapPy_
#define _SnapPy_

#include "../kernel/SnapPea.h"
#include "../kernel/kernel_namespace.h"

extern int* get_cusp_equation(regina::snappea::Triangulation* manifold,
    int cusp_num, int m, int l, int* numRows);
extern void free_cusp_equation(int *equation);

#include "../kernel/end_namespace.h"

#endif
