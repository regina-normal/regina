
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

#import "EltMoves4.h"
#import "TextHelper.h"
#import "triangulation/dim4.h"

@interface EltMoves4 () {
    NSMutableArray* options42;
    NSMutableArray* options33;
    NSMutableArray* options24;
    NSMutableArray* options15;
    NSMutableArray* options20Triangle;
    NSMutableArray* options20Edge;
    NSMutableArray* optionsOpenBook;
    NSMutableArray* optionsShell;
    NSMutableArray* optionsCollapseEdge;

    NSAttributedString* unavailable;
}

@property (weak, nonatomic) IBOutlet UIButton *button42;
@property (weak, nonatomic) IBOutlet UIButton *button33;
@property (weak, nonatomic) IBOutlet UIButton *button24;
@property (weak, nonatomic) IBOutlet UIButton *button15;
@property (weak, nonatomic) IBOutlet UIButton *button20Triangle;
@property (weak, nonatomic) IBOutlet UIButton *button20Edge;
@property (weak, nonatomic) IBOutlet UIButton *buttonOpenBook;
@property (weak, nonatomic) IBOutlet UIButton *buttonShell;
@property (weak, nonatomic) IBOutlet UIButton *buttonCollapseEdge;

@property (weak, nonatomic) IBOutlet UIStepper *stepper42;
@property (weak, nonatomic) IBOutlet UIStepper *stepper33;
@property (weak, nonatomic) IBOutlet UIStepper *stepper24;
@property (weak, nonatomic) IBOutlet UIStepper *stepper15;
@property (weak, nonatomic) IBOutlet UIStepper *stepper20Triangle;
@property (weak, nonatomic) IBOutlet UIStepper *stepper20Edge;
@property (weak, nonatomic) IBOutlet UIStepper *stepperOpenBook;
@property (weak, nonatomic) IBOutlet UIStepper *stepperShell;
@property (weak, nonatomic) IBOutlet UIStepper *stepperCollapseEdge;

@property (weak, nonatomic) IBOutlet UILabel *detail42;
@property (weak, nonatomic) IBOutlet UILabel *detail33;
@property (weak, nonatomic) IBOutlet UILabel *detail24;
@property (weak, nonatomic) IBOutlet UILabel *detail15;
@property (weak, nonatomic) IBOutlet UILabel *detail20Triangle;
@property (weak, nonatomic) IBOutlet UILabel *detail20Edge;
@property (weak, nonatomic) IBOutlet UILabel *detailOpenBook;
@property (weak, nonatomic) IBOutlet UILabel *detailShell;
@property (weak, nonatomic) IBOutlet UILabel *detailCollapseEdge;

@property (weak, nonatomic) IBOutlet UILabel *fVector;

@end

@implementation EltMoves4

- (void)viewWillAppear:(BOOL)animated
{
    unavailable = [TextHelper dimString:@"Not available"];
    [self reloadMoves];
}

- (void)reloadMoves
{
    self.fVector.text = [NSString stringWithFormat:@"f-vector: (%ld, %ld, %ld, %ld, %ld)",
                         self.packet->countFaces<0>(),
                         self.packet->countFaces<1>(),
                         self.packet->countFaces<2>(),
                         self.packet->countFaces<3>(),
                         self.packet->size()];

    size_t i;

    options42 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countEdges(); ++i)
        if (self.packet->fourTwoMove(self.packet->edge(i), true, false))
            [options42 addObject:@(i)];
    if (options42.count > 0) {
        self.button42.enabled = self.stepper42.enabled = YES;
        self.stepper42.maximumValue = options42.count - 1;
        if (self.stepper42.value >= options42.count)
            self.stepper42.value = options42.count - 1;
        else
            [self changed42:nil];
    } else {
        self.button42.enabled = self.stepper42.enabled = NO;
        self.detail42.attributedText = unavailable;
    }

    options33 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countTriangles(); ++i)
        if (self.packet->threeThreeMove(self.packet->triangle(i), true, false))
            [options33 addObject:@(i)];
    if (options33.count > 0) {
        self.button33.enabled = self.stepper33.enabled = YES;
        self.stepper33.maximumValue = options33.count - 1;
        if (self.stepper33.value >= options33.count)
            self.stepper33.value = options33.count - 1;
        else
            [self changed33:nil];
    } else {
        self.button33.enabled = self.stepper33.enabled = NO;
        self.detail33.attributedText = unavailable;
    }

    options24 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countTetrahedra(); ++i)
        if (self.packet->twoFourMove(self.packet->tetrahedron(i), true, false))
            [options24 addObject:@(i)];
    if (options24.count > 0) {
        self.button24.enabled = self.stepper24.enabled = YES;
        self.stepper24.maximumValue = options24.count - 1;
        if (self.stepper24.value >= options24.count)
            self.stepper24.value = options24.count - 1;
        else
            [self changed24:nil];
    } else {
        self.button24.enabled = self.stepper24.enabled = NO;
        self.detail24.attributedText = unavailable;
    }

    options15 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->size(); ++i)
        if (self.packet->oneFiveMove(self.packet->pentachoron(i), true, false))
            [options15 addObject:@(i)];
    if (options15.count > 0) {
        self.button15.enabled = self.stepper15.enabled = YES;
        self.stepper15.maximumValue = options15.count - 1;
        if (self.stepper15.value >= options15.count)
            self.stepper15.value = options15.count - 1;
        else
            [self changed15:nil];
    } else {
        self.button15.enabled = self.stepper15.enabled = NO;
        self.detail15.attributedText = unavailable;
    }

    options20Triangle = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countTriangles(); ++i)
        if (self.packet->twoZeroMove(self.packet->triangle(i), true, false))
            [options20Triangle addObject:@(i)];
    if (options20Triangle.count > 0) {
        self.button20Triangle.enabled = self.stepper20Triangle.enabled = YES;
        self.stepper20Triangle.maximumValue = options20Triangle.count - 1;
        if (self.stepper20Triangle.value >= options20Triangle.count)
            self.stepper20Triangle.value = options20Triangle.count - 1;
        else
            [self changed20Triangle:nil];
    } else {
        self.button20Triangle.enabled = self.stepper20Triangle.enabled = NO;
        self.detail20Triangle.attributedText = unavailable;
    }

    options20Edge = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countEdges(); ++i)
        if (self.packet->twoZeroMove(self.packet->edge(i), true, false))
            [options20Edge addObject:@(i)];
    if (options20Edge.count > 0) {
        self.button20Edge.enabled = self.stepper20Edge.enabled = YES;
        self.stepper20Edge.maximumValue = options20Edge.count - 1;
        if (self.stepper20Edge.value >= options20Edge.count)
            self.stepper20Edge.value = options20Edge.count - 1;
        else
            [self changed20Edge:nil];
    } else {
        self.button20Edge.enabled = self.stepper20Edge.enabled = NO;
        self.detail20Edge.attributedText = unavailable;
    }

    optionsOpenBook = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countTetrahedra(); ++i)
        if (self.packet->openBook(self.packet->tetrahedron(i), true, false))
            [optionsOpenBook addObject:@(i)];
    if (optionsOpenBook.count > 0) {
        self.buttonOpenBook.enabled = self.stepperOpenBook.enabled = YES;
        self.stepperOpenBook.maximumValue = optionsOpenBook.count - 1;
        if (self.stepperOpenBook.value >= optionsOpenBook.count)
            self.stepperOpenBook.value = optionsOpenBook.count - 1;
        else
            [self changedOpenBook:nil];
    } else {
        self.buttonOpenBook.enabled = self.stepperOpenBook.enabled = NO;
        self.detailOpenBook.attributedText = unavailable;
    }

    optionsShell = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->size(); ++i)
        if (self.packet->shellBoundary(self.packet->pentachoron(i), true, false))
            [optionsShell addObject:@(i)];
    if (optionsShell.count > 0) {
        self.buttonShell.enabled = self.stepperShell.enabled = YES;
        self.stepperShell.maximumValue = optionsShell.count - 1;
        if (self.stepperShell.value >= optionsShell.count)
            self.stepperShell.value = optionsShell.count - 1;
        else
            [self changedShell:nil];
    } else {
        self.buttonShell.enabled = self.stepperShell.enabled = NO;
        self.detailShell.attributedText = unavailable;
    }

    optionsCollapseEdge = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countEdges(); ++i)
        if (self.packet->collapseEdge(self.packet->edge(i), true, false))
            [optionsCollapseEdge addObject:@(i)];
    if (optionsCollapseEdge.count > 0) {
        self.buttonCollapseEdge.enabled = self.stepperCollapseEdge.enabled = YES;
        self.stepperCollapseEdge.maximumValue = optionsCollapseEdge.count - 1;
        if (self.stepperCollapseEdge.value >= optionsCollapseEdge.count)
            self.stepperCollapseEdge.value = optionsCollapseEdge.count - 1;
        else
            [self changedCollapseEdge:nil];
    } else {
        self.buttonCollapseEdge.enabled = self.stepperCollapseEdge.enabled = NO;
        self.detailCollapseEdge.attributedText = unavailable;
    }
}

- (regina::Vertex<4>*)vertexFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid vertex stepper value: %d", use);
        return 0;
    }
    return self.packet->vertex([options[use] longValue]);
}

- (regina::Edge<4>*)edgeFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid edge stepper value: %d", use);
        return 0;
    }
    return self.packet->edge([options[use] longValue]);
}

- (regina::Triangle<4>*)triangleFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid triangle stepper value: %d", use);
        return 0;
    }
    return self.packet->triangle([options[use] longValue]);
}

- (regina::Tetrahedron<4>*)tetrahedronFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid tetrahedron stepper value: %d", use);
        return 0;
    }
    return self.packet->tetrahedron([options[use] longValue]);
}

- (regina::Pentachoron<4>*)pentachoronFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid pentachoron stepper value: %d", use);
        return 0;
    }
    return self.packet->pentachoron([options[use] longValue]);
}

- (NSAttributedString*)vertexDesc:(regina::Vertex<4>*)vertex
{
    if (! vertex)
        return [TextHelper badString:@"Invalid vertex"];

    NSMutableString* text = [[NSMutableString alloc] init];

    const regina::VertexEmbedding<4>& e0 = vertex->embedding(0);
    [text appendFormat:@"Vertex %ld — %ld (%d)",
     vertex->index(),
     e0.pentachoron()->index(),
     e0.vertex()];

    if (vertex->degree() > 1) {
        const regina::VertexEmbedding<4>& e1 = vertex->embedding(1);
        [text appendFormat:@", %ld (%d)",
         e1.pentachoron()->index(),
         e1.vertex()];
        if (vertex->degree() > 2)
            [text appendString:@", …"];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (NSAttributedString*)edgeDesc:(regina::Edge<4>*)edge
{
    if (! edge)
        return [TextHelper badString:@"Invalid edge"];

    NSMutableString* text = [[NSMutableString alloc] init];

    const regina::EdgeEmbedding<4>& e0 = edge->embedding(0);
    [text appendFormat:@"Edge %ld — %ld (%s)",
     edge->index(),
     e0.pentachoron()->index(),
     e0.vertices().trunc2().c_str()];

    if (edge->degree() > 1) {
        const regina::EdgeEmbedding<4>& e1 = edge->embedding(1);
        [text appendFormat:@", %ld (%s)",
         e1.pentachoron()->index(),
         e1.vertices().trunc2().c_str()];
        if (edge->degree() > 2)
            [text appendString:@", …"];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (NSAttributedString*)triangleDesc:(regina::Triangle<4>*)triangle
{
    if (! triangle)
        return [TextHelper badString:@"Invalid triangle"];

    NSMutableString* text = [[NSMutableString alloc] init];

    const regina::TriangleEmbedding<4>& e0 = triangle->embedding(0);
    [text appendFormat:@"Triangle %ld — %ld (%s)",
     triangle->index(),
     e0.pentachoron()->index(),
     e0.vertices().trunc3().c_str()];

    if (triangle->degree() > 1) {
        const regina::TriangleEmbedding<4>& e1 = triangle->embedding(1);
        [text appendFormat:@", %ld (%s)",
         e1.pentachoron()->index(),
         e1.vertices().trunc3().c_str()];
        if (triangle->degree() > 2)
            [text appendString:@", …"];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (NSAttributedString*)tetrahedronDesc:(regina::Tetrahedron<4>*)tetrahedron
{
    if (! tetrahedron)
        return [TextHelper badString:@"Invalid tetrahedron"];

    NSMutableString* text = [[NSMutableString alloc] init];

    const regina::TetrahedronEmbedding<4>& e0 = tetrahedron->embedding(0);
    [text appendFormat:@"Tet %ld — %ld (%s)",
     tetrahedron->index(),
     e0.pentachoron()->index(),
     e0.vertices().trunc4().c_str()];

    if (tetrahedron->degree() > 1) {
        const regina::TetrahedronEmbedding<4>& e1 = tetrahedron->embedding(1);
        [text appendFormat:@", %ld (%s)",
         e1.pentachoron()->index(),
         e1.vertices().trunc4().c_str()];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (NSAttributedString*)pentachoronDesc:(regina::Pentachoron<4>*)pentachoron
{
    if (! pentachoron)
        return [TextHelper badString:@"Invalid pentachoron"];

    NSString* text = [NSString stringWithFormat:@"Pentachoron %ld", pentachoron->index()];
    return [[NSAttributedString alloc] initWithString:text];
}

- (IBAction)do42:(id)sender
{
    regina::Edge<4>* use = [self edgeFor:self.stepper42 options:options42];
    if (! use)
        return;

    self.packet->fourTwoMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do33:(id)sender
{
    regina::Triangle<4>* use = [self triangleFor:self.stepper33 options:options33];
    if (! use)
        return;

    self.packet->threeThreeMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do24:(id)sender
{
    regina::Tetrahedron<4>* use = [self tetrahedronFor:self.stepper24 options:options24];
    if (! use)
        return;

    self.packet->twoFourMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do15:(id)sender
{
    regina::Pentachoron<4>* use = [self pentachoronFor:self.stepper15 options:options15];
    if (! use)
        return;

    self.packet->oneFiveMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do20Triangle:(id)sender
{
    regina::Triangle<4>* use = [self triangleFor:self.stepper20Triangle options:options20Triangle];
    if (! use)
        return;

    self.packet->twoZeroMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do20Edge:(id)sender
{
    regina::Edge<4>* use = [self edgeFor:self.stepper20Edge options:options20Edge];
    if (! use)
        return;

    self.packet->twoZeroMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)doOpenBook:(id)sender
{
    regina::Tetrahedron<4>* use = [self tetrahedronFor:self.stepperOpenBook options:optionsOpenBook];
    if (! use)
        return;

    self.packet->openBook(use, true, true);
    [self reloadMoves];
}

- (IBAction)doShell:(id)sender
{
    regina::Pentachoron<4>* use = [self pentachoronFor:self.stepperShell options:optionsShell];
    if (! use)
        return;

    self.packet->shellBoundary(use, true, true);
    [self reloadMoves];
}

- (IBAction)doCollapseEdge:(id)sender
{
    regina::Edge<4>* use = [self edgeFor:self.stepperCollapseEdge options:optionsCollapseEdge];
    if (! use)
        return;

    self.packet->collapseEdge(use, true, true);
    [self reloadMoves];
}

- (IBAction)changed42:(id)sender
{
    self.detail42.attributedText = [self edgeDesc:[self edgeFor:self.stepper42 options:options42]];
}

- (IBAction)changed33:(id)sender
{
    self.detail33.attributedText = [self triangleDesc:[self triangleFor:self.stepper33 options:options33]];
}

- (IBAction)changed24:(id)sender
{
    self.detail24.attributedText = [self tetrahedronDesc:[self tetrahedronFor:self.stepper24 options:options24]];
}

- (IBAction)changed15:(id)sender
{
    self.detail15.attributedText = [self pentachoronDesc:[self pentachoronFor:self.stepper15 options:options15]];
}

- (IBAction)changed20Triangle:(id)sender
{
    self.detail20Triangle.attributedText = [self triangleDesc:[self triangleFor:self.stepper20Triangle options:options20Triangle]];
}

- (IBAction)changed20Edge:(id)sender
{
    self.detail20Edge.attributedText = [self edgeDesc:[self edgeFor:self.stepper20Edge options:options20Edge]];
}

- (IBAction)changedOpenBook:(id)sender
{
    self.detailOpenBook.attributedText = [self tetrahedronDesc:[self tetrahedronFor:self.stepperOpenBook options:optionsOpenBook]];
}

- (IBAction)changedShell:(id)sender
{
    self.detailShell.attributedText = [self pentachoronDesc:[self pentachoronFor:self.stepperShell options:optionsShell]];
}

- (IBAction)changedCollapseEdge:(id)sender
{
    self.detailCollapseEdge.attributedText = [self edgeDesc:[self edgeFor:self.stepperCollapseEdge options:optionsCollapseEdge]];
}

- (IBAction)close:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end
