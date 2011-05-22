/**
 * This is the file casson.h.  It was initially written by Damien Heard
 * as part of the program Orb (http://www.ms.unimelb.edu.au/~snap/orb.html).
 *
 * This header was introduced into Regina for the Orb / Casson import
 * and export routines, which were contributed by Ryan Budney.  It is
 * for internal use by these routines only; any other code should call
 * the public routines from orb.h.
 *
 * Many thanks to Damien Heard for giving permission for his code to
 * be distributed under the terms of the GNU General Public License.
 */

#ifndef CASSON_H
#define CASSON_H

#include "regina-core.h"

#define LN(ch)   (ch=='u') ? 0 : ((ch=='v') ? 1 : ((ch=='w') ? 2 : 3))

const int vertex_at_faces[4][4] =
        {{9,2,3,1},
         {3,9,0,2},
         {1,3,9,0},
         {2,0,1,9}};

typedef struct CassonFormat CassonFormat;
typedef struct EdgeInfo EdgeInfo;
typedef struct TetEdgeInfo TetEdgeInfo;

struct REGINA_LOCAL CassonFormat
{
        int             num_tet;
        EdgeInfo        *head;
};

struct REGINA_LOCAL EdgeInfo
{
        int             index,
                        singular_index;
        double          singular_order;

        TetEdgeInfo     *head;
        EdgeInfo        *prev,
                        *next;
};

struct REGINA_LOCAL TetEdgeInfo
{
        int             tet_index,f1,f2;
        TetEdgeInfo     *prev,
                        *next;
};


#endif
