
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#import "NewPacketPageViewController.h"

/**
 * The main controller for creating a new triangulation.
 *
 * This holds a NewPacketPageViewController; the corresponding pages
 * are described by separate view controller classes below.
 */
@interface NewTri3Controller : NewPacketController
@end

/**
 * The controller for creating a new empty triangulation.
 *
 * This represents an individual page within the larger NewTri3Controller.
 */
@interface NewTri3EmptyPage : UIViewController <PacketCreator>
@end

/**
 * The controller for creating a ready-made example triangulation.
 *
 * This represents an individual page within the larger NewTri3Controller.
 */
@interface NewTri3ExamplePage : UIViewController <PacketCreator>
@end

/**
 * The controller for creating triangulations from parameterised families.
 *
 * This represents an individual page within the larger NewTri3Controller.
 */
@interface NewTri3ConstructionPage : UIViewController <PacketCreator>
@end

/**
 * The controller for creating a new triangulation from an isomorphism signature.
 *
 * This represents an individual page within the larger NewTri3Controller.
 */
@interface NewTri3IsosigPage : UIViewController <PacketCreator>
@end
