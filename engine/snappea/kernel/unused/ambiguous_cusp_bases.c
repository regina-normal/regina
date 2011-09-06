/*
 *	ambiguous_cusp_bases.c
 *
 *	For certain manifolds, the "canonical" peripheral curves
 *	installed by terse_to_tri() are not well defined. 
 *	(The problem is that the cusps do not have unique shortest geodesics, 
 *	so the geometry of the cusp alone does not provide any way to select
 *	a preferred meridian.)  The function
 *
 *		void resolve_ambiguous_bases(	Triangulation	*aTriangulation,
 *										char			*aDehydratedDescription);
 *
 *	resolves this problem for the census manifolds by choosing a
 *	set of peripheral curves based on the homology of the manifold as
 *	a whole.  For non-census manifolds, the function posts a warning and 
 *	leaves the existing (meridian, longitude) unchanged.
 *
 *	Comments:
 *
 *	(1)	Full documentation appears in the files "Read Me" and
 *		"ambiguous examples.symmetries" in the folder "cusped census 2.1".
 *
 *	(2)	The choices rely on a fixed orientation for the manifold.
 *
 *	(3)	For most of the 2-cusp manifolds, all (shortest) choices for
 *		the meridian of a single cusp are equivalent, but once you've
 *		chosen it the choices for the remaining cusp are *not* equivalent.
 *		This observation lets us treat the 1-cusp and 2-cusp cases using
 *		the same usual_algorithm() code, which rotates the coordinates
 *		on cusp 0 until the homology is right, regardless of whether the
 *		manifold has a second cusp or not.
 *
 *	Added to SnapPea 28 February 2008 by JRW to replace the equivalent function
 *	in the old rehydrate_census.c.
 */

#include "kernel.h"

#define AMBIGUITY_EPSILON	1e-6	//	Probably even a much smaller epsilon would work.

static Complex	gSquareCuspShape		= {0.0, 1.0},
				gHexagonalCuspShape60	= {+0.5, ROOT_3_OVER_2},
				gHexagonalCuspShape120	= {-0.5, ROOT_3_OVER_2};

/*
 *	The documentation at the top of the file change_peripheral_curves.c
 *	explains the interpretation of the following "change matrices".
 */
static CONST MatrixInt22
		rotate6[2] =
						{
							{
								{  0, 1 },
								{ -1, 1 }
							},
							{
								{  1, 0 },
								{  0, 1 }
							}
						},
		rotate6a[2] =	{
							{
								{  1, 0 },
								{  0, 1 }
							},
							{
								{  0, 1 },
								{ -1, 1 }
							}
						},
		rotate4[2] =	{
							{
								{  0, 1 },
								{ -1, 0 }
							},
							{
								{  1, 0 },
								{  0, 1 }
							}
						};

static void		usual_algorithm(Triangulation *aTriangulation, int anM, int anL, CONST MatrixInt22 *aChangeMatrixArray, int aNumCoefficients, int aFirstCoefficient, int aSecondCoefficient, int aThirdCoefficient);
static void		algorithm_s596(Triangulation *aTriangulation);
static Boolean	check_homology(Triangulation *aTriangulation, AbelianGroup *anAbelianGroup);


void resolve_ambiguous_bases(
	Triangulation	*aTriangulation,
	char			*aDehydratedDescription)
{
	Cusp	*cusp;
	
	for (cusp = aTriangulation->cusp_list_begin.next;
		 cusp != &aTriangulation->cusp_list_end;
		 cusp = cusp->next)
	{
		if (cusp->topology == torus_cusp
		 && (
				complex_modulus(complex_minus(cusp->cusp_shape[initial], gSquareCuspShape      )) < AMBIGUITY_EPSILON
			 || complex_modulus(complex_minus(cusp->cusp_shape[initial], gHexagonalCuspShape60 )) < AMBIGUITY_EPSILON
			 || complex_modulus(complex_minus(cusp->cusp_shape[initial], gHexagonalCuspShape120)) < AMBIGUITY_EPSILON
			))
		{
			if (strcmp(aDehydratedDescription, "cabbbbapt") == 0)				//	m003
				usual_algorithm(aTriangulation, 1, 0, rotate6, 1, 10, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "eahbcdddljjrv") == 0)			//	m125
				usual_algorithm(aTriangulation, 1, 0, rotate4, 1, 3, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "eahbdccdhffff") == 0)			//	m130
				usual_algorithm(aTriangulation, 1, 1, rotate4, 2, 2, 16, -1);
			else
			if (strcmp(aDehydratedDescription, "ebdbcdddaahhx") == 0)			//	m135
				usual_algorithm(aTriangulation, 1, 1, rotate4, 3, 2, 2, 4);
			else
			if (strcmp(aDehydratedDescription, "fcnaabbddeeabonan") == 0)		//	m139
				usual_algorithm(aTriangulation, 1, 0, rotate4, 1, 24, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "ebdbbdddemlqp") == 0)			//	m202
				usual_algorithm(aTriangulation, 1, 0, rotate6, 1, 3, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "finaabdcceepoooqk") == 0)		//	m208
				usual_algorithm(aTriangulation, 1, 0, rotate6, 1, 20, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "gbpaadfcfeeffmghwgm") == 0)		//	s594
				usual_algorithm(aTriangulation, 1, 0, rotate6, 3, 2, 2, 0);
			else
			if (strcmp(aDehydratedDescription, "gddabbcedfffaaxxaax") == 0)		//	s596
				algorithm_s596(aTriangulation);
			else
			if (strcmp(aDehydratedDescription, "gapabbcdeeffbgrrrag") == 0)		//	s859
				usual_algorithm(aTriangulation, 1, 0, rotate4, 1, 6, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "gcoacacedefffkqqchb") == 0)		//	s913
				usual_algorithm(aTriangulation, 1, 0, rotate4, 1, 5, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "gbpaabcfdffefohfxhf") == 0)		//	s955
				usual_algorithm(aTriangulation, 1, 0, rotate6, 2, 4, 20, -1);
			else
			if (strcmp(aDehydratedDescription, "gbpaaddefeffoffhoxh") == 0)		//	s957
				usual_algorithm(aTriangulation, 1, 0, rotate6, 2, 4, 4, -1);
			else
			if (strcmp(aDehydratedDescription, "gdhaabfefefelpllpll") == 0)		//	s959
				usual_algorithm(aTriangulation, 1, 0, rotate6, 1, 9, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "gfdabbcdefffaqhhqax") == 0)		//	s960
				usual_algorithm(aTriangulation, 1, 0, rotate6, 3, 2, 10, 0);
			else
			if (strcmp(aDehydratedDescription, "hbpaccdfefgggkfxcmlaw") == 0)	//	v1859
				usual_algorithm(aTriangulation, 1, 0, rotate4, 3, 2, 2, 2);
			else
			if (strcmp(aDehydratedDescription, "hbpabeffdfgggvqxchhah") == 0)	//	v3318
				usual_algorithm(aTriangulation, 1, 0, rotate4, 2, 2, 2, -1);
			else
			if (strcmp(aDehydratedDescription, "hdhaecdfdfgggqkpbleml") == 0)	//	v3319
				usual_algorithm(aTriangulation, 1, 0, rotate4, 1, 3, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "hbpaebcffdgggfghantuk") == 0)	//	v3461
				usual_algorithm(aTriangulation, 1, 0, rotate6, 1, 5, -1, -1);
			else
			if (strcmp(aDehydratedDescription, "hbpabbcfggfegfkadihgo") == 0)	//	v3551
				usual_algorithm(aTriangulation, 1, 0, rotate6, 1, 14, -1, -1);
			else
			{
				//	That covers all the cusped census manifolds,
				//	so unless people pass in their own dehydrated descriptions,
				//	they should never see this message.
				uAcknowledge("This manifold has a square or hexagonal cusp.  The (meridian, longitude) pair is not uniquely defined.  SnapPea has made an arbitrary choice, which could be CPU-dependent, so be cautious when comparing Dehn filling coefficients across different computers.");
			}
		}
	}
}


static void usual_algorithm(
	Triangulation		*aTriangulation,
	int					anM,
	int					anL,
	CONST MatrixInt22	*aChangeMatrixArray,
	int					aNumCoefficients,
	int					aFirstCoefficient,
	int					aSecondCoefficient,
	int					aThirdCoefficient)
{
	int				i,
					theRotationCount;
	long			theCoefficientArray[3];
	AbelianGroup	theAbelianGroup;

	/*
	 *	Set up theAbelianGroup.
	 */
	theCoefficientArray[0] = aFirstCoefficient;
	theCoefficientArray[1] = aSecondCoefficient;
	theCoefficientArray[2] = aThirdCoefficient;
	theAbelianGroup.num_torsion_coefficients	= aNumCoefficients;
	theAbelianGroup.torsion_coefficients		= theCoefficientArray;

	/*
	 *	Set up an (m,l) Dehn filling on each cusp, relative to the initial
	 *	(arbitrary) coordinate system.  Don't actually compute the
	 *	hyperbolic structure -- the computation would be slow (compared
	 *	to what we're doing here) and we don't need the hyperbolic
	 *	structure to check the homology anyhow.
	 */
	for (i = 0; i < get_num_cusps(aTriangulation); i++)
		set_cusp_info(aTriangulation, i, FALSE, anM, anL);

	/*
	 *	We'll keep track of how many times we've been through the following
	 *	while() loop.  If something goes wrong we should display an error
	 *	message instead of looping forever.
	 */
	theRotationCount = 0;

	/*
	 *	If the homology isn't what we want, rotate the coordinate system
	 *	a sixth or quarter turn, according to aChangeMatrixArray.
	 *	After at most two such rotations we should find the meridian
	 *	we're looking for.  See the file "ambiguous examples.symmetries"
	 *	for an explanation of how the desired meridians were chosen.
	 */
	while (check_homology(aTriangulation, &theAbelianGroup) == FALSE)
	{
		/*
		 *	The call to change_peripheral_curves() will adjust the Dehn
		 *	filling coefficients to compensate for the changed coordinate
		 *	system, thereby preserving the original Dehn filling.
		 *	But we want to move on to a new Dehn filling, which is (m,l)
		 *	in the *new* coordinate system.
		 */
		change_peripheral_curves(aTriangulation, aChangeMatrixArray);
		set_cusp_info(aTriangulation, 0, FALSE, anM, anL);

		/*
		 *	We shouldn't have to rotate more than twice to find
		 *	the desired meridian.
		 */
		if (++theRotationCount > 2)
			uFatalError("usual_algorithm", "ambiguous_bases");
	}

	/*
	 *	We've found the correct peripheral curves.  Restore the
	 *	Dehn filling coefficients to their original, unfilled state.
	 */
	for (i = 0; i < get_num_cusps(aTriangulation); i++)
		set_cusp_info(aTriangulation, i, TRUE, 0.0, 0.0);
}


static void algorithm_s596(
	Triangulation	*aTriangulation)
{
	/*
	 *	Please see the file "ambiguous examples.symmetries"
	 *	for an explanation of why s596 needs special treatment.
	 */

	int				theRotationCount;
	long			theCoefficientArray[2];
	AbelianGroup	theAbelianGroup;

	theAbelianGroup.num_torsion_coefficients	= 2;
	theAbelianGroup.torsion_coefficients		= theCoefficientArray;
	theAbelianGroup.torsion_coefficients[0]		= 2;
	theAbelianGroup.torsion_coefficients[1]		= 2;

	set_cusp_info(aTriangulation, 0, FALSE, 1.0, 0.0);
	set_cusp_info(aTriangulation, 1, FALSE, 1.0, 0.0);

	theRotationCount = 0;

	while (check_homology(aTriangulation, &theAbelianGroup) == FALSE)
	{
		/*
		 *	Cycle through all possible combinations
		 *	of meridians for cusps 0 and 1.
		 */
		if (theRotationCount % 3 == 0)
		{
			change_peripheral_curves(aTriangulation, rotate6);
			set_cusp_info(aTriangulation, 0, FALSE, 1.0, 0.0);
		}
		else
		{
			change_peripheral_curves(aTriangulation, rotate6a);
			set_cusp_info(aTriangulation, 1, FALSE, 1.0, 0.0);
		}

		if (++theRotationCount > 8)
			uFatalError("algorithm_s596", "ambiguous_bases");
	}

	set_cusp_info(aTriangulation, 0, TRUE, 0.0, 0.0);
	set_cusp_info(aTriangulation, 1, TRUE, 0.0, 0.0);
}


static Boolean check_homology(
	Triangulation	*aTriangulation,
	AbelianGroup	*anAbelianGroup)
{
	AbelianGroup	*theHomology;
	Boolean			theGroupsAreIsomorphic;
	int				i;

	theHomology = homology(aTriangulation);
	
	if (theHomology == NULL)
		uFatalError("check_homology", "rehydrate_census");
	
	compress_abelian_group(theHomology);

	if (theHomology->num_torsion_coefficients
			!= anAbelianGroup->num_torsion_coefficients)
		theGroupsAreIsomorphic = FALSE;
	else
	{
		theGroupsAreIsomorphic = TRUE;
		for (i = 0; i < theHomology->num_torsion_coefficients; i++)
			if (theHomology->torsion_coefficients[i]
					!= anAbelianGroup->torsion_coefficients[i])
				theGroupsAreIsomorphic = FALSE;
	}

	free_abelian_group(theHomology);

	return theGroupsAreIsomorphic;
}

