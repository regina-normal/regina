#ifndef CASSON_H
#define CASSON_H

#define LN(ch)   (ch=='u') ? 0 : ((ch=='v') ? 1 : ((ch=='w') ? 2 : 3))

const int vertex_at_faces[4][4] =
        {{9,2,3,1},
         {3,9,0,2},
         {1,3,9,0},
         {2,0,1,9}};

typedef struct CassonFormat CassonFormat;
typedef struct EdgeInfo EdgeInfo;
typedef struct TetEdgeInfo TetEdgeInfo;

struct CassonFormat
{
	int		num_tet;
	EdgeInfo	*head;
};

struct EdgeInfo
{
	int		index,
			singular_index;
	double 		singular_order;

	TetEdgeInfo	*head;
	EdgeInfo	*prev,
			*next;
};

struct TetEdgeInfo
{
	int		tet_index,f1,f2;
	TetEdgeInfo	*prev,
			*next;
};


#endif
