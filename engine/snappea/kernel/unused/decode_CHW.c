/*
 *	decode_CHW.c
 *
 *	Decodes ideal triangulations encoded in the format described in
 *
 *		Callahan, Hildebrand & Weeks,
 *		“A census of cusped hyperbolic 3-manifolds”,
 *		Mathematics of Computation 68 (1999) 321-332.
 *
 *	The cusped census manifolds are all stored in this format.
 *	For example, the Gieseking manifold is “baaaade” while
 *	the last 7-tetrahedron manifold is “hfdafbcdefgggaqhhqadl”.
 */

#include "kernel.h"


Triangulation *CHW_to_tri(
	char	*anEncoding,
	Boolean	aChernSimonsIsPresent,
	double	aChernSimonsValue)
{
	char				*theNextChar			= NULL;
	unsigned int		theNumTetrahedra		= 0,
						theNewTetBits			= 0,
						i;
	TerseTriangulation	*theTerseTriangulation	= NULL;
	Triangulation		*theTriangulation		= NULL;
	
	theNextChar = anEncoding;
	theNumTetrahedra = *theNextChar++ - 'a';
	theTerseTriangulation = alloc_terse(theNumTetrahedra);
	
	//	Fill in the num_tetrahedra.
	theTerseTriangulation->num_tetrahedra = theNumTetrahedra;
	
	//	Fill in the glues_to_old_tet array.
	//	Note #1:  The ordering in CHW is slightly unnatural.
	//	Note #2:  The boolean flag in CHW is the opposite of ours.
	theNewTetBits += (*theNextChar++ - 'a') << 4;
	theNewTetBits += (*theNextChar++ - 'a') << 0;
	if (theNumTetrahedra > 4)
	{
		theNewTetBits += (*theNextChar++ - 'a') << 12;
		theNewTetBits += (*theNextChar++ - 'a') <<  8;
	}
	for (i = 0; i < 2*theNumTetrahedra; i++)
		theTerseTriangulation->glues_to_old_tet[i] = (theNewTetBits & (0x00000001 << i)) ? FALSE : TRUE;
	
	//	Fill in the which_old_tet array.
	for (i = 0; i < theNumTetrahedra + 1; i++)
		theTerseTriangulation->which_old_tet[i] = *theNextChar++ - 'a';
	
	//	Fill in the which_gluing array.
	for (i = 0; i < theNumTetrahedra + 1; i++)
		theTerseTriangulation->which_gluing[i] = permutation_by_index[*theNextChar++ - 'a'];
	
	//	Fill in the Chern-Simons invariant.
	theTerseTriangulation->CS_is_present	= aChernSimonsIsPresent;
	theTerseTriangulation->CS_value			= aChernSimonsIsPresent ? aChernSimonsValue : 0.0;
	
	//	theTerseTriangulation is complete.  Convert it to a real Triangulation.
	theTriangulation = terse_to_tri(theTerseTriangulation);

	free_terse_triangulation(theTerseTriangulation);

	return theTriangulation;
}
