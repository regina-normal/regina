/**
 *  Headers for additional SnapPy code that is not shipped with the
 *  SnapPea kernel.
 */

#ifndef _SnapPy_
#define _SnapPy_

#include "../kernel/SnapPea.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int* get_cusp_equation(Triangulation* manifold, int cusp_num,
    int m, int l, int* numRows);
extern void free_cusp_equation(int *equation);

#ifdef __cplusplus
}
#endif

#endif
