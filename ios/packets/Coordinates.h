
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#import <Foundation/Foundation.h>
#import "hypersurface/hypercoords.h"
#import "maths/integer.h"
#import "surfaces/normalcoords.h"
#import "triangulation/forward.h"

namespace regina {
    class NormalHypersurface;
    class NormalSurface;
}

@interface Coordinates : NSObject

/**
 * Return a human-readable name for the given coordinate system.
 */
+ (NSString*)name:(regina::NormalCoords)coordSystem capitalise:(BOOL)capitalise;

/**
 * Return a human-readable adjective for the given coordinate system.
 * Unlike the human-readable name, this is a very short (often one-word)
 * adjective that is designed to be slotted into a longer sentence.
 */
+ (NSString*)adjective:(regina::NormalCoords)coordSystem capitalise:(BOOL)capitalise;

/**
 * Does the given coordinate system generate almost normal
 * surfaces when used with NormalSurfaces::enumerate()?
 *
 * Only coordinate systems that are used for enumerating surfaces
 * (not just viewing surfaces) are relevant here.
 */
+ (BOOL)generatesAlmostNormal:(regina::NormalCoords)coordSystem;

/**
 * Return the number of coordinate columns in the given coordinate
 * system.
 */
+ (unsigned long)numColumns:(regina::NormalCoords)coordSystem tri:(regina::Triangulation<3>*)tri;

/**
 * Return a column header for the given coordinate of the given
 * coordinate system.
 *
 * The associated triangulation may be passed so that more precise
 * information can be returned, though this routine will behave
 * well without it.
 */
+ (NSString*)columnName:(regina::NormalCoords)coordSystem whichCoord:(unsigned long)whichCoord tri:(regina::Triangulation<3>*)tri;

/**
 * Returns the longest coordinate column header that we might expect to see.
 *
 * Here the associated triangulation is mandatory.
 */
+ (NSString*)longestColumnName:(regina::NormalCoords)coordSystem tri:(regina::Triangulation<3>*)tri;

/**
 * Return a particular coordinate of a normal surface in the given
 * coordinate system.
 */
+ (regina::LargeInteger)getCoordinate:(regina::NormalCoords)coordSystem surface:(const regina::NormalSurface&)surface whichCoord:(unsigned long)whichCoord;

@end

@interface HyperCoordinates : NSObject

/**
 * Return a human-readable name for the given coordinate system.
 */
+ (NSString*)name:(regina::HyperCoords)coordSystem capitalise:(BOOL)capitalise;

/**
 * Return a human-readable adjective for the given coordinate system.
 * Unlike the human-readable name, this is a very short (often one-word)
 * adjective that is designed to be slotted into a longer sentence.
 */
+ (NSString*)adjective:(regina::HyperCoords)coordSystem capitalise:(BOOL)capitalise;

/**
 * Return the number of coordinate columns in the given coordinate
 * system.
 */
+ (size_t)numColumns:(regina::HyperCoords)coordSystem tri:(regina::Triangulation<4>*)tri;

/**
 * Return a column header for the given coordinate of the given
 * coordinate system.
 *
 * The associated triangulation may be passed so that more precise
 * information can be returned, though this routine will behave
 * well without it.
 */
+ (NSString*)columnName:(regina::HyperCoords)coordSystem whichCoord:(size_t)whichCoord tri:(regina::Triangulation<4>*)tri;

/**
 * Returns the longest coordinate column header that we might expect to see.
 *
 * Here the associated triangulation is mandatory.
 */
+ (NSString*)longestColumnName:(regina::HyperCoords)coordSystem tri:(regina::Triangulation<4>*)tri;

/**
 * Return a particular coordinate of a normal surface in the given
 * coordinate system.
 */
+ (regina::LargeInteger)getCoordinate:(regina::HyperCoords)coordSystem surface:(const regina::NormalHypersurface&)surface whichCoord:(size_t)whichCoord;

@end
