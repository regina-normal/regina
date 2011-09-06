/*
 *	unix_file_io.h
 *
 *	These two functions allow unix-style programs
 *	to read and save Triangulations.
 */

#include "SnapPea.h"

#ifdef __cplusplus
extern "C" {
#endif

extern Triangulation	*get_triangulation(const char *file_name);
extern void				save_triangulation(Triangulation *manifold, const char *file_name);

#ifdef __cplusplus
}
#endif

