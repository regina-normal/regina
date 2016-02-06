
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

/**
 * Stores the details for one of Regina's ready-made (and read-only)
 * example data files.
 *
 * These examples can be accessed through class methods such as
 * intro and census.
 */
@interface Example : NSObject

/**
 * The filename, relative to Regina's internal examples directory.
 * Typically this filename will not contain any directory component.
 */
@property (strong, nonatomic, readonly) NSString* file;

/**
 * A human-readable description of this example file.
 */
@property (strong, nonatomic, readonly) NSString* desc;

/**
 * An initialiser that sets the \a file and \a desc properties
 * to the given values.
 */
- (id)initWithFile:(NSString*)f desc:(NSString*)d;

/**
 * A constructor that sets the \a file and \a desc properties
 * to the given values.
 */
+ (id)exampleWithFile:(NSString*)f desc:(NSString*)d;

/**
 * The introductory examples file.
 *
 * This returns a reference to a global Example object
 * (thus multiple calls to this method will return the same pointer).
 */
+ (Example*)intro;

/**
 * The set of all available census data files.
 *
 * This returns a reference to a global array of Example objects
 * (in particular, multiple calls to this method will return a pointer
 * to the same array).
 */
+ (NSArray*)census;

@end

