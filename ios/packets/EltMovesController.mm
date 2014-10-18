
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
#import "TextHelper.h"
#import "triangulation/nedge.h"
#import "triangulation/ntetrahedron.h"
#import "triangulation/ntriangle.h"
#import "triangulation/ntriangulation.h"
#import "triangulation/nvertex.h"

@interface EltMovesController () {
    NSMutableArray* options32;
    NSMutableArray* options23;
    NSMutableArray* options14;
    NSMutableArray* options44;
    NSMutableArray* options20Edge;
    NSMutableArray* options20Vtx;
    NSMutableArray* options21;
    NSMutableArray* optionsOpenBook;
    NSMutableArray* optionsCloseBook;
    NSMutableArray* optionsShell;
    NSMutableArray* optionsCollapseEdge;

    NSAttributedString* unavailable;
}

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
    unavailable = [TextHelper dimString:@"Not available"];
    [self reloadMoves];
}

- (void)reloadMoves
{
    // TODO: Update f-vector.

    unsigned long i;

    options32 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->getNumberOfEdges(); ++i)
        if (self.packet->threeTwoMove(self.packet->getEdge(i), true, false))
            [options32 addObject:@(i)];
    if (options32.count > 0) {
        self.button32.enabled = self.stepper32.enabled = YES;
        self.stepper32.maximumValue = options32.count - 1;
        if (self.stepper32.value >= options32.count)
            self.stepper32.value = options32.count - 1;
        else
            [self changed32:nil];
    } else {
        self.button32.enabled = self.stepper32.enabled = NO;
        self.detail32.attributedText = unavailable;
    }

    options23 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->getNumberOfTriangles(); ++i)
        if (self.packet->twoThreeMove(self.packet->getTriangle(i), true, false))
            [options23 addObject:@(i)];
    if (options23.count > 0) {
        self.button23.enabled = self.stepper23.enabled = YES;
        self.stepper23.maximumValue = options23.count - 1;
        if (self.stepper23.value >= options23.count)
            self.stepper23.value = options23.count - 1;
        else
            [self changed23:nil];
    } else {
        self.button23.enabled = self.stepper23.enabled = NO;
        self.detail23.attributedText = unavailable;
    }
    
    // TODO
}

- (regina::NEdge*)edgeFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid edge stepper value: %d", use);
        return 0;
    }
    return self.packet->getEdge([options[use] longValue]);
}

- (regina::NTriangle*)triangleFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid triangle stepper value: %d", use);
        return 0;
    }
    return self.packet->getTriangle([options[use] longValue]);
}

- (NSAttributedString*)edgeDesc:(regina::NEdge*)edge
{
    if (! edge)
        return [TextHelper badString:@"Invalid edge"];

    NSMutableString* text = [[NSMutableString alloc] init];

    const regina::NEdgeEmbedding& e0 = edge->getEmbedding(0);
    [text appendFormat:@"Edge %ld — %ld (%s)",
     edge->index(),
     e0.getTetrahedron()->index(),
     e0.getVertices().trunc2().c_str()];

    if (edge->getNumberOfEmbeddings() > 1) {
        const regina::NEdgeEmbedding& e1 = edge->getEmbedding(1);
        [text appendFormat:@", %ld (%s)",
         e1.getTetrahedron()->index(),
         e1.getVertices().trunc2().c_str()];
        if (edge->getNumberOfEmbeddings() > 2)
            [text appendString:@", …"];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (NSAttributedString*)triangleDesc:(regina::NTriangle*)triangle
{
    if (! triangle)
        return [TextHelper badString:@"Invalid triangle"];

    NSMutableString* text = [[NSMutableString alloc] init];

    const regina::NTriangleEmbedding& e0 = triangle->getEmbedding(0);
    [text appendFormat:@"Triangle %ld — %ld (%s)",
     triangle->index(),
     e0.getTetrahedron()->index(),
     e0.getVertices().trunc3().c_str()];

    if (triangle->getNumberOfEmbeddings() > 1) {
        const regina::NTriangleEmbedding& e1 = triangle->getEmbedding(1);
        [text appendFormat:@", %ld (%s)",
         e1.getTetrahedron()->index(),
         e1.getVertices().trunc3().c_str()];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (IBAction)do32:(id)sender
{
    regina::NEdge* use = [self edgeFor:self.stepper32 options:options32];
    if (! use)
        return;

    self.packet->threeTwoMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do23:(id)sender
{
    regina::NTriangle* use = [self triangleFor:self.stepper23 options:options23];
    if (! use)
        return;

    self.packet->twoThreeMove(use, true, true);
    [self reloadMoves];
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
    self.detail32.attributedText = [self edgeDesc:[self edgeFor:self.stepper32 options:options32]];
}

- (IBAction)changed23:(id)sender
{
    self.detail23.attributedText = [self triangleDesc:[self triangleFor:self.stepper23 options:options23]];
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
