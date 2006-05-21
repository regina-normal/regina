/*
 *	unix_file_io.h
 *
 *	These two functions allow unix-style programs
 *	to read and save Triangulations.
 */

#ifndef _unix_file_io_
#define _unix_file_io_

#include "SnapPea.h"

#ifdef __cplusplus
extern "C" {
#endif

extern Triangulation	*get_triangulation(char *file_name);
extern void				save_triangulation(Triangulation *manifold, char *file_name);

#ifdef __cplusplus
}
#endif

#endif
