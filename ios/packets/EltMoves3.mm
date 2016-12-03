
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

#import "EltMoves3.h"
#import "TextHelper.h"
#import "triangulation/dim3.h"

@interface EdgeArg : NSObject
@property (assign, nonatomic) unsigned long edge;
@property (assign, nonatomic) int arg;
- (id)init:(unsigned long)edge arg:(int)arg;
@end

@implementation EdgeArg
- (id)init:(unsigned long)edge arg:(int)arg
{
    self = [super init];
    if (self) {
        _edge = edge;
        _arg = arg;
    }
    return self;
}
@end

@interface EltMoves3 () {
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

@implementation EltMoves3

- (void)viewWillAppear:(BOOL)animated
{
    unavailable = [TextHelper dimString:@"Not available"];
    [self reloadMoves];
}

- (void)reloadMoves
{
    self.fVector.text = [NSString stringWithFormat:@"f-vector: (%ld, %ld, %ld, %ld)",
                         self.packet->countFaces<0>(),
                         self.packet->countFaces<1>(),
                         self.packet->countFaces<2>(),
                         self.packet->size()];

    unsigned long i;
    int arg;

    options32 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countEdges(); ++i)
        if (self.packet->threeTwoMove(self.packet->edge(i), true, false))
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
    for (i = 0; i < self.packet->countTriangles(); ++i)
        if (self.packet->twoThreeMove(self.packet->triangle(i), true, false))
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

    options14 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->size(); ++i)
        if (self.packet->oneFourMove(self.packet->tetrahedron(i), true, false))
            [options14 addObject:@(i)];
    if (options14.count > 0) {
        self.button14.enabled = self.stepper14.enabled = YES;
        self.stepper14.maximumValue = options14.count - 1;
        if (self.stepper14.value >= options14.count)
            self.stepper14.value = options14.count - 1;
        else
            [self changed14:nil];
    } else {
        self.button14.enabled = self.stepper14.enabled = NO;
        self.detail14.attributedText = unavailable;
    }

    options44 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countEdges(); ++i)
        for (arg = 0; arg < 2; ++arg)
            if (self.packet->fourFourMove(self.packet->edge(i), arg, true, false))
                [options44 addObject:[[EdgeArg alloc] init:i arg:arg]];
    if (options44.count > 0) {
        self.button44.enabled = self.stepper44.enabled = YES;
        self.stepper44.maximumValue = options44.count - 1;
        if (self.stepper44.value >= options44.count)
            self.stepper44.value = options44.count - 1;
        else
            [self changed44:nil];
    } else {
        self.button44.enabled = self.stepper44.enabled = NO;
        self.detail44.attributedText = unavailable;
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

    options20Vtx = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countVertices(); ++i)
        if (self.packet->twoZeroMove(self.packet->vertex(i), true, false))
            [options20Vtx addObject:@(i)];
    if (options20Vtx.count > 0) {
        self.button20Vtx.enabled = self.stepper20Vtx.enabled = YES;
        self.stepper20Vtx.maximumValue = options20Vtx.count - 1;
        if (self.stepper20Vtx.value >= options20Vtx.count)
            self.stepper20Vtx.value = options20Vtx.count - 1;
        else
            [self changed20Vtx:nil];
    } else {
        self.button20Vtx.enabled = self.stepper20Vtx.enabled = NO;
        self.detail20Vtx.attributedText = unavailable;
    }

    options21 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countEdges(); ++i)
        for (arg = 0; arg < 2; ++arg)
            if (self.packet->twoOneMove(self.packet->edge(i), arg, true, false))
                [options21 addObject:[[EdgeArg alloc] init:i arg:arg]];
    if (options21.count > 0) {
        self.button21.enabled = self.stepper21.enabled = YES;
        self.stepper21.maximumValue = options21.count - 1;
        if (self.stepper21.value >= options21.count)
            self.stepper21.value = options21.count - 1;
        else
            [self changed21:nil];
    } else {
        self.button21.enabled = self.stepper21.enabled = NO;
        self.detail21.attributedText = unavailable;
    }

    optionsOpenBook = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countTriangles(); ++i)
        if (self.packet->openBook(self.packet->triangle(i), true, false))
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

    optionsCloseBook = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->countEdges(); ++i)
        if (self.packet->closeBook(self.packet->edge(i), true, false))
            [optionsCloseBook addObject:@(i)];
    if (optionsCloseBook.count > 0) {
        self.buttonCloseBook.enabled = self.stepperCloseBook.enabled = YES;
        self.stepperCloseBook.maximumValue = optionsCloseBook.count - 1;
        if (self.stepperCloseBook.value >= optionsCloseBook.count)
            self.stepperCloseBook.value = optionsCloseBook.count - 1;
        else
            [self changedCloseBook:nil];
    } else {
        self.buttonCloseBook.enabled = self.stepperCloseBook.enabled = NO;
        self.detailCloseBook.attributedText = unavailable;
    }

    optionsShell = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->size(); ++i)
        if (self.packet->shellBoundary(self.packet->tetrahedron(i), true, false))
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

- (regina::Vertex<3>*)vertexFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid vertex stepper value: %d", use);
        return 0;
    }
    return self.packet->vertex([options[use] longValue]);
}

- (regina::Edge<3>*)edgeFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid edge stepper value: %d", use);
        return 0;
    }
    return self.packet->edge([options[use] longValue]);
}

- (EdgeArg*)edgeArgFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid edge-with-arg stepper value: %d", use);
        return 0;
    }
    return options[use];
}

- (regina::Triangle<3>*)triangleFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid triangle stepper value: %d", use);
        return 0;
    }
    return self.packet->triangle([options[use] longValue]);
}

- (regina::Tetrahedron<3>*)tetrahedronFor:(UIStepper*)stepper options:(NSArray*)options
{
    NSInteger use = stepper.value;
    if (use < 0 || use >= options.count) {
        NSLog(@"Invalid tetrahedron stepper value: %d", use);
        return 0;
    }
    return self.packet->tetrahedron([options[use] longValue]);
}

- (NSAttributedString*)vertexDesc:(regina::Vertex<3>*)vertex
{
    if (! vertex)
        return [TextHelper badString:@"Invalid vertex"];

    NSMutableString* text = [[NSMutableString alloc] init];

    const regina::VertexEmbedding<3>& e0 = vertex->embedding(0);
    [text appendFormat:@"Vertex %ld — %ld (%d)",
     vertex->index(),
     e0.tetrahedron()->index(),
     e0.vertex()];

    if (vertex->degree() > 1) {
        const regina::VertexEmbedding<3>& e1 = vertex->embedding(1);
        [text appendFormat:@", %ld (%d)",
         e1.tetrahedron()->index(),
         e1.vertex()];
        if (vertex->degree() > 2)
            [text appendString:@", …"];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (NSAttributedString*)edgeDesc:(regina::Edge<3>*)edge
{
    if (! edge)
        return [TextHelper badString:@"Invalid edge"];

    NSMutableString* text = [[NSMutableString alloc] init];

    const regina::EdgeEmbedding<3>& e0 = edge->embedding(0);
    [text appendFormat:@"Edge %ld — %ld (%s)",
     edge->index(),
     e0.tetrahedron()->index(),
     e0.vertices().trunc2().c_str()];

    if (edge->degree() > 1) {
        const regina::EdgeEmbedding<3>& e1 = edge->embedding(1);
        [text appendFormat:@", %ld (%s)",
         e1.tetrahedron()->index(),
         e1.vertices().trunc2().c_str()];
        if (edge->degree() > 2)
            [text appendString:@", …"];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (NSAttributedString*)edgeDesc:(EdgeArg*)edgeArg type:(NSString*)type
{
    if (! edgeArg)
        return [TextHelper badString:@"Invalid edge-with-arg"];

    NSMutableString* text = [[NSMutableString alloc] init];

    regina::Edge<3>* edge = self.packet->edge(edgeArg.edge);
    const regina::EdgeEmbedding<3>& e0 = edge->embedding(0);
    [text appendFormat:@"Edge %ld [%@ %d] — %ld (%s)",
     edge->index(),
     type,
     edgeArg.arg,
     e0.tetrahedron()->index(),
     e0.vertices().trunc2().c_str()];

    if (edge->degree() > 1) {
        const regina::EdgeEmbedding<3>& e1 = edge->embedding(1);
        [text appendFormat:@", %ld (%s)",
         e1.tetrahedron()->index(),
         e1.vertices().trunc2().c_str()];
        if (edge->degree() > 2)
            [text appendString:@", …"];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (NSAttributedString*)triangleDesc:(regina::Triangle<3>*)triangle
{
    if (! triangle)
        return [TextHelper badString:@"Invalid triangle"];

    NSMutableString* text = [[NSMutableString alloc] init];

    const regina::TriangleEmbedding<3>& e0 = triangle->embedding(0);
    [text appendFormat:@"Triangle %ld — %ld (%s)",
     triangle->index(),
     e0.tetrahedron()->index(),
     e0.vertices().trunc3().c_str()];

    if (triangle->degree() > 1) {
        const regina::TriangleEmbedding<3>& e1 = triangle->embedding(1);
        [text appendFormat:@", %ld (%s)",
         e1.tetrahedron()->index(),
         e1.vertices().trunc3().c_str()];
    }

    return [[NSAttributedString alloc] initWithString:text];
}

- (NSAttributedString*)tetrahedronDesc:(regina::Tetrahedron<3>*)tetrahedron
{
    if (! tetrahedron)
        return [TextHelper badString:@"Invalid tetrahedron"];

    NSString* text = [NSString stringWithFormat:@"Tetrahedron %ld", tetrahedron->index()];
    return [[NSAttributedString alloc] initWithString:text];
}

- (IBAction)do32:(id)sender
{
    regina::Edge<3>* use = [self edgeFor:self.stepper32 options:options32];
    if (! use)
        return;

    self.packet->threeTwoMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do23:(id)sender
{
    regina::Triangle<3>* use = [self triangleFor:self.stepper23 options:options23];
    if (! use)
        return;

    self.packet->twoThreeMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do14:(id)sender
{
    regina::Tetrahedron<3>* use = [self tetrahedronFor:self.stepper14 options:options14];
    if (! use)
        return;

    self.packet->oneFourMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do44:(id)sender
{
    EdgeArg* use = [self edgeArgFor:self.stepper44 options:options44];
    if (! use)
        return;

    self.packet->fourFourMove(self.packet->edge(use.edge), use.arg, true, true);
    [self reloadMoves];
}

- (IBAction)do20Edge:(id)sender
{
    regina::Edge<3>* use = [self edgeFor:self.stepper20Edge options:options20Edge];
    if (! use)
        return;

    self.packet->twoZeroMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do20Vtx:(id)sender
{
    regina::Vertex<3>* use = [self vertexFor:self.stepper20Vtx options:options20Vtx];
    if (! use)
        return;

    self.packet->twoZeroMove(use, true, true);
    [self reloadMoves];
}

- (IBAction)do21:(id)sender
{
    EdgeArg* use = [self edgeArgFor:self.stepper21 options:options21];
    if (! use)
        return;

    self.packet->twoOneMove(self.packet->edge(use.edge), use.arg, true, true);
    [self reloadMoves];
}

- (IBAction)doOpenBook:(id)sender
{
    regina::Triangle<3>* use = [self triangleFor:self.stepperOpenBook options:optionsOpenBook];
    if (! use)
        return;

    self.packet->openBook(use, true, true);
    [self reloadMoves];
}

- (IBAction)doCloseBook:(id)sender
{
    regina::Edge<3>* use = [self edgeFor:self.stepperCloseBook options:optionsCloseBook];
    if (! use)
        return;

    self.packet->closeBook(use, true, true);
    [self reloadMoves];
}

- (IBAction)doShell:(id)sender
{
    regina::Tetrahedron<3>* use = [self tetrahedronFor:self.stepperShell options:optionsShell];
    if (! use)
        return;

    self.packet->shellBoundary(use, true, true);
    [self reloadMoves];
}

- (IBAction)doCollapseEdge:(id)sender
{
    regina::Edge<3>* use = [self edgeFor:self.stepperCollapseEdge options:optionsCollapseEdge];
    if (! use)
        return;

    self.packet->collapseEdge(use, true, true);
    [self reloadMoves];
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
    self.detail14.attributedText = [self tetrahedronDesc:[self tetrahedronFor:self.stepper14 options:options14]];
}

- (IBAction)changed44:(id)sender
{
    self.detail44.attributedText = [self edgeDesc:[self edgeArgFor:self.stepper44 options:options44] type:@"axis"];
}

- (IBAction)changed20Edge:(id)sender
{
    self.detail20Edge.attributedText = [self edgeDesc:[self edgeFor:self.stepper20Edge options:options20Edge]];
}

- (IBAction)changed20Vtx:(id)sender
{
    self.detail20Vtx.attributedText = [self vertexDesc:[self vertexFor:self.stepper20Vtx options:options20Vtx]];
}

- (IBAction)changed21:(id)sender
{
    self.detail21.attributedText = [self edgeDesc:[self edgeArgFor:self.stepper21 options:options21] type:@"end"];
}

- (IBAction)changedOpenBook:(id)sender
{
    self.detailOpenBook.attributedText = [self triangleDesc:[self triangleFor:self.stepperOpenBook options:optionsOpenBook]];
}

- (IBAction)changedCloseBook:(id)sender
{
    self.detailCloseBook.attributedText = [self edgeDesc:[self edgeFor:self.stepperCloseBook options:optionsCloseBook]];
}

- (IBAction)changedShell:(id)sender
{
    self.detailShell.attributedText = [self tetrahedronDesc:[self tetrahedronFor:self.stepperShell options:optionsShell]];
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
