
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "colors.inc"
#include "textures.inc"
#include "stones.inc"

#declare Sheet1 = color rgbt <0, 5, 0, 0.95>;
#declare Sheet2 = color rgbt <5, 0, 0, 0.95>;
#declare Sheet3 = color rgbt <0, 0, 5, 0.95>;
#declare All = color rgb <3,0,6>;

#declare loc = <4, 0.6, 0.2>;

camera {
	location loc
	sky <0,0,1>
	look_at <0, 0, 0.2>
}

plane {
	-loc, 4.1
	pigment { White }
}

light_source {
	<4, 1, 0.4>
	color White
}	
light_source {
	<3, 4, 0.4>
	color White
}
light_source {
	<4, -3, 0.4>
	color White
}
light_source {
	<-4, 3, 0.4>
	color White
}
light_source {
	<0, -4, 0.4>
	color White
}
light_source {
	<-4, 4, 0.4>
	color White
}

#declare root2 = 1.4142136;
#declare root3 = 1.7320508;
#declare a = <2, 0, -root2 / 2>;
#declare b = <-1, root3, -root2 / 2>;
#declare c = <-1, -root3, -root2 / 2>;
#declare d = <0, 0, 3 * root2 / 2>;
#declare tetthickness = 0.075;

#declare tetskeleton = union {
	cylinder { a, b, tetthickness }
	cylinder { b, c, tetthickness }
	cylinder { c, d, tetthickness }
	cylinder { d, a, tetthickness }
	cylinder { a, c, tetthickness }
	cylinder { b, d, tetthickness }
	sphere { a, tetthickness }
	sphere { b, tetthickness }
	sphere { c, tetthickness }
	sphere { d, tetthickness }
}

#declare tridisc = prism {
	linear_sweep linear_spline
	-tetthickness/2, tetthickness/2,
	4,
	<0.5, 0>, <-0.25, root3/4>, <-0.25, -root3/4>, <0.5, 0>
}

#declare trid = object {
	tridisc
	rotate <-90, 0, 0>
	translate <0, 0, root2>
}

#declare tria = object {
	tridisc
	rotate <-90, 0, 0>
	translate <0, 0, root2>
	rotate <0, 0, 180>
	rotate <0, 109.47122, 0>
}

#declare trib = object {
	tridisc
	rotate <-90, 0, 0>
	translate <0, 0, root2>
	rotate <0, 0, 180>
	rotate <0, 109.47122, 0>
	rotate <0, 0, 120>
}

#declare tric = object {
	tridisc
	rotate <-90, 0, 0>
	translate <0, 0, root2>
	rotate <0, 0, 180>
	rotate <0, 109.47122, 0>
	rotate <0, 0, 240>
}

#declare quaddisc = prism {
	linear_sweep linear_spline
	-tetthickness/2, tetthickness/2,
	5,
	<root3/2, root3/2>, <root3/2, -root3/2>, <-root3/2, -root3/2>,
	<-root3/2, root3/2>, <root3/2, root3/2>
}

#declare quadabcd = object {
	quaddisc
	rotate <35.26439, 0, 0>
	rotate <0, 0, 30>
	pigment { Blue }
}

#declare quadacbd = object {
	quaddisc
	rotate <35.26439, 0, 0>
	rotate <0, 0, 30>
	rotate <0, 0, 120>
	pigment { Blue }
}

#declare quadadbc = object {
	quaddisc
	rotate <35.26439, 0, 0>
	rotate <0, 0, 30>
	rotate <0, 0, 240>
	pigment { Blue }
}

#declare everything = union {
	object {
		tetskeleton
		texture { T_Stone20 } // Brown = 20
	}
	
	object {
		tria
		texture { T_Stone21 } // Red = 21
	}
	
	object {
		trib
		texture { T_Stone18 } // Green = 18
	}
	
	object {
		tric
		texture { T_Stone23 } // Orange = 23
	}
	
	object {
		trid
		texture { T_Stone13 } // Blue = 13
	}
	
	object {
		quadacbd
		texture { T_Stone12 } // Pink = 12
	}
}

object {
	everything
	scale <1,1,0.9>
}

