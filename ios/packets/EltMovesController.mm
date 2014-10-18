
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

#import "EltMovesController.h"

@interface EltMovesController ()

@property (weak, nonatomic) IBOutlet UIButton *button32;
@property (weak, nonatomic) IBOutlet UIButton *button23;
@property (weak, nonatomic) IBOutlet UIButton *button14;
@property (weak, nonatomic) IBOutlet UIButton *button44;
@property (weak, nonatomic) IBOutlet UIButton *button20Edge;
@property (weak, nonatomic) IBOutlet UIButton *button20Vtx;
@property (weak, nonatomic) IBOutlet UIButton *button21;
@property (weak, nonatomic) IBOutlet UIButton *buttonOpenBook;
@property (weak, nonatomic) IBOutlet UIButton *buttonCloseBook;
@property (weak, nonatomic) IBOutlet UIButton *buttonShell;
@property (weak, nonatomic) IBOutlet UIButton *buttonCollapseEdge;

@property (weak, nonatomic) IBOutlet UIStepper *stepper32;
@property (weak, nonatomic) IBOutlet UIStepper *stepper23;
@property (weak, nonatomic) IBOutlet UIStepper *stepper14;
@property (weak, nonatomic) IBOutlet UIStepper *stepper44;
@property (weak, nonatomic) IBOutlet UIStepper *stepper20Edge;
@property (weak, nonatomic) IBOutlet UIStepper *stepper20Vtx;
@property (weak, nonatomic) IBOutlet UIStepper *stepper21;
@property (weak, nonatomic) IBOutlet UIStepper *stepperOpenBook;
@property (weak, nonatomic) IBOutlet UIStepper *stepperCloseBook;
@property (weak, nonatomic) IBOutlet UIStepper *stepperShell;
@property (weak, nonatomic) IBOutlet UIStepper *stepperCollapseEdge;

@property (weak, nonatomic) IBOutlet UILabel *detail32;
@property (weak, nonatomic) IBOutlet UILabel *detail23;
@property (weak, nonatomic) IBOutlet UILabel *detail14;
@property (weak, nonatomic) IBOutlet UILabel *detail44;
@property (weak, nonatomic) IBOutlet UILabel *detail20Edge;
@property (weak, nonatomic) IBOutlet UILabel *detail20Vtx;
@property (weak, nonatomic) IBOutlet UILabel *detail21;
@property (weak, nonatomic) IBOutlet UILabel *detailOpenBook;
@property (weak, nonatomic) IBOutlet UILabel *detailCloseBook;
@property (weak, nonatomic) IBOutlet UILabel *detailShell;
@property (weak, nonatomic) IBOutlet UILabel *detailCollapseEdge;

@property (weak, nonatomic) IBOutlet UILabel *fVector;

@end

@implementation EltMovesController

- (void)viewWillAppear:(BOOL)animated
{
    [self reloadMoves];
}

- (void)reloadMoves
{
    // TODO
}

- (IBAction)do32:(id)sender
{
}

- (IBAction)do23:(id)sender
{
}

- (IBAction)do14:(id)sender
{
}

- (IBAction)do44:(id)sender
{
}

- (IBAction)do20Edge:(id)sender
{
}

- (IBAction)do20Vtx:(id)sender
{
}

- (IBAction)do21:(id)sender
{
}

- (IBAction)doOpenBook:(id)sender
{
}

- (IBAction)doCloseBook:(id)sender
{
}

- (IBAction)doShell:(id)sender
{
}

- (IBAction)doCollapseEdge:(id)sender
{
}

- (IBAction)changed32:(id)sender
{
}

- (IBAction)changed23:(id)sender
{
}

- (IBAction)changed14:(id)sender
{
}

- (IBAction)changed44:(id)sender
{
}

- (IBAction)changed20Edge:(id)sender
{
}

- (IBAction)changed20Vtx:(id)sender
{
}

- (IBAction)changed21:(id)sender
{
}

- (IBAction)changedOpenBook:(id)sender
{
}

- (IBAction)changedCloseBook:(id)sender
{
}

- (IBAction)changedShell:(id)sender
{
}

- (IBAction)changedCollapseEdge:(id)sender
{
}

- (IBAction)close:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end
