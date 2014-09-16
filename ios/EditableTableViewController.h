
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

#import <UIKit/UIKit.h>

/**
 * The view controller for a table that supports actions on cells, such as
 * renaming or deleting cells.
 *
 * This superclass offers a place to register where actions are taking
 * place (the \a actionPath property), and manages the positioning of the
 * table as the keyboard appears and disappears.
 *
 * All other functionality is left for subclasses to implement.
 */
@interface EditableTableViewController : UITableViewController

/**
 * If the user is currently editing a table cell (e.g., renaming
 * or deleting it), then this stores the corresponding index in the table.
 * Otherwise this property is \a nil.
 *
 * Subclasses should update this property as the user initiates, cancels
 * and/or completes actions, and should disable appropriate parts of the user
 * interface whenever it is non-nil.
 */
@property (strong, nonatomic) NSIndexPath *actionPath;

@end
