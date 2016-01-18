
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

#import "SnapPeaViewController.h"
#import "TextHelper.h"
#import "TriangulationViewController.h"
#import "TriComposition.h"
#import "manifold/nmanifold.h"
#import "subcomplex/naugtrisolidtorus.h"
#import "subcomplex/nblockedsfs.h"
#import "subcomplex/nblockedsfsloop.h"
#import "subcomplex/nblockedsfspair.h"
#import "subcomplex/nblockedsfstriple.h"
#import "subcomplex/nl31pillow.h"
#import "subcomplex/nlayeredchain.h"
#import "subcomplex/nlayeredchainpair.h"
#import "subcomplex/nlayeredlensspace.h"
#import "subcomplex/nlayeredloop.h"
#import "subcomplex/nlayeredsolidtorus.h"
#import "subcomplex/nlayeredsurfacebundle.h"
#import "subcomplex/npillowtwosphere.h"
#import "subcomplex/npluggedtorusbundle.h"
#import "subcomplex/nplugtrisolidtorus.h"
#import "subcomplex/nsatblock.h"
#import "subcomplex/nsatregion.h"
#import "subcomplex/nsnappedball.h"
#import "subcomplex/nsnappedtwosphere.h"
#import "subcomplex/nspiralsolidtorus.h"
#import "subcomplex/nstandardtri.h"
#import "subcomplex/ntxicore.h"
#import "triangulation/ntriangulation.h"

#define INDENT1 "\t• "
#define INDENT2 "\t\t- "
#define INDENT3 "\t\t\t- "
#define INDENT4 "\t\t\t\t- "

@interface TriComposition () <UITextFieldDelegate> {
    UILabel* copyFrom;
}
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *volume;
@property (weak, nonatomic) IBOutlet UILabel *solnType;
@property (weak, nonatomic) IBOutlet UIButton *lockIcon;

@property (weak, nonatomic) IBOutlet UILabel *isosig;
@property (weak, nonatomic) IBOutlet UILabel *standard;
@property (weak, nonatomic) IBOutlet UITextView *components;

@property (assign, nonatomic) regina::NTriangulation* packet;
@end

@implementation TriComposition

- (void)viewDidLoad
{
    UILongPressGestureRecognizer *r = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.view addGestureRecognizer:r];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = static_cast<regina::NTriangulation*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);

    [self reloadPacket];
}

- (IBAction)longPress:(id)sender {
    UILongPressGestureRecognizer *press = static_cast<UILongPressGestureRecognizer*>(sender);
    if (press.state == UIGestureRecognizerStateBegan) {
        copyFrom = nil;
        CGPoint location = [press locationInView:self.view];
        if (CGRectContainsPoint(self.isosig.frame, location))
            copyFrom = self.isosig;
        else if (CGRectContainsPoint(self.standard.frame, location))
            copyFrom = self.standard;
        if (! copyFrom)
            return;
        
        [self becomeFirstResponder];
        
        UIMenuController *copyMenu = [UIMenuController sharedMenuController];
        CGRect textBounds = [copyFrom textRectForBounds:copyFrom.bounds limitedToNumberOfLines:copyFrom.numberOfLines];
        [copyMenu setTargetRect:textBounds inView:copyFrom];
        copyMenu.arrowDirection = UIMenuControllerArrowDefault;
        [copyMenu setMenuVisible:YES animated:YES];
    }
}

- (void)reloadPacket
{
    if ([self.parentViewController isKindOfClass:[SnapPeaViewController class]])
        [static_cast<SnapPeaViewController*>(self.parentViewController) updateHeader:self.header volume:self.volume solnType:self.solnType];
    else
        [static_cast<TriangulationViewController*>(self.parentViewController) updateHeader:self.header lockIcon:self.lockIcon];

    self.isosig.text = @(self.packet->isoSig().c_str());
    
    regina::NStandardTriangulation* stdTri = regina::NStandardTriangulation::isStandardTriangulation(self.packet);
    if (stdTri)
        self.standard.text = @(stdTri->name().c_str());
    else
        self.standard.attributedText = [TextHelper dimString:@"Not recognised"];
    delete stdTri;
    
    NSMutableString* details = [[NSMutableString alloc] init];
    
    // Look for complete closed triangulations.
    [self findAugTriSolidTori:details];
    [self findL31Pillows:details];
    [self findLayeredChainPairs:details];
    [self findLayeredLensSpaces:details];
    [self findLayeredLoops:details];
    [self findPlugTriSolidTori:details];
    [self findBlockedTriangulations:details];

    // Look for interesting surfaces.
    [self findPillowSpheres:details];
    [self findSnappedSpheres:details];
    
    // Look for bounded subcomplexes.
    [self findLayeredSolidTori:details];
    [self findSnappedBalls:details];
    [self findSpiralSolidTori:details];
    
    self.components.font = self.standard.font;
    self.components.text = details;
}

#pragma mark - UIResponder

- (BOOL)canBecomeFirstResponder
{
    return (copyFrom != nil);
}

- (BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
    if (action == @selector(copy:) && copyFrom)
        return YES;
    else
        return [super canPerformAction:action withSender:sender];
}

- (void)copy:(id)sender
{
    if (copyFrom)
        [[UIPasteboard generalPasteboard] setString:copyFrom.text];
}

#pragma mark - Subcomplexes

- (void)findAugTriSolidTori:(NSMutableString*)details
{
    unsigned long nComps = self.packet->countComponents();
    
    regina::NAugTriSolidTorus* aug;
    for (unsigned long i = 0; i < nComps; i++) {
        aug = regina::NAugTriSolidTorus::isAugTriSolidTorus(self.packet->component(i));
        if (aug) {
            [details appendFormat:@"Augmented triangular solid torus %s\n", aug->name().c_str()];
            [details appendFormat:@INDENT1 "Component %ld\n", i];

            const regina::NTriSolidTorus& core = aug->core();
            [details appendFormat:@INDENT1 "Core: tets %ld, %ld, %ld\n",
             core.getTetrahedron(0)->index(),
             core.getTetrahedron(1)->index(),
             core.getTetrahedron(2)->index()];

            if (aug->hasLayeredChain()) {
                if (aug->chainType() == regina::NAugTriSolidTorus::CHAIN_MAJOR)
                    [details appendString:@INDENT1 "Attached: layered chain (major) + layered solid torus]n"];
                else if (aug->chainType() == regina::NAugTriSolidTorus::CHAIN_AXIS)
                    [details appendString:@INDENT1 "Attached: layered chain (axis) + layered solid torus\n"];
                else
                    [details appendString:@INDENT1 "Attached: layered chain (unknown) + layered solid torus\n"];
            } else
                [details appendString:@INDENT1 "Attached: 3 layered solid tori\n"];
     
            [details appendString:@"\n"];
            delete aug;
        }
    }
}

- (void)findL31Pillows:(NSMutableString*)details
{
    unsigned long nComps = self.packet->countComponents();
    
    regina::NL31Pillow* pillow;
    for (unsigned long i = 0; i < nComps; i++) {
        pillow = regina::NL31Pillow::isL31Pillow(self.packet->component(i));
        if (pillow) {
            [details appendFormat:@"L(3,1) pillow %s\n", pillow->name().c_str()];
            [details appendFormat:@INDENT1 "Component %ld\n", i];
            [details appendFormat:@INDENT1 "Pillow interior vertex: %ld\n",
             pillow->tetrahedron(0)->getVertex(pillow->interiorVertex(0))->index()];
            
            [details appendString:@"\n"];
            delete pillow;
        }
    }
}

- (void)findLayeredChainPairs:(NSMutableString*)details
{
    unsigned long nComps = self.packet->countComponents();
    
    regina::NLayeredChainPair* pair;
    for (unsigned long i = 0; i < nComps; i++) {
        pair = regina::NLayeredChainPair::isLayeredChainPair(self.packet->component(i));
        if (pair) {
            [details appendFormat:@"Layered chain pair %s\n", pair->name().c_str()];
            [details appendFormat:@INDENT1 "Component %ld\n", i];
            [details appendFormat:@INDENT1 "Chain lengths: %ld, %ld\n",
             pair->chain(0)->index(),
             pair->chain(1)->index()];
            
            [details appendString:@"\n"];
            delete pair;
        }
    }
}

- (void)findLayeredLensSpaces:(NSMutableString*)details
{
    unsigned long nComps = self.packet->countComponents();
    
    regina::NLayeredLensSpace* lens;
    for (unsigned long i = 0; i < nComps; i++) {
        lens = regina::NLayeredLensSpace::isLayeredLensSpace(self.packet->component(i));
        if (lens) {
            [details appendFormat:@"Layered lens space %s\n", lens->name().c_str()];
            [details appendFormat:@INDENT1 "Component %ld\n", i];
            
            const regina::NLayeredSolidTorus& torus(lens->torus());
            [details appendFormat:@INDENT1 "Layered %ld-%ld-%ld solid torus %s\n",
             torus.getMeridinalCuts(0),
             torus.getMeridinalCuts(1),
             torus.getMeridinalCuts(2),
             (lens->isSnapped() ? "snapped shut" : "twisted shut")];

            [details appendString:@"\n"];
            delete lens;
        }
    }
}

- (void)findLayeredLoops:(NSMutableString*)details
{
    unsigned long nComps = self.packet->countComponents();
    
    regina::NLayeredLoop* loop;
    for (unsigned long i = 0; i < nComps; i++) {
        loop = regina::NLayeredLoop::isLayeredLoop(self.packet->component(i));
        if (loop) {
            [details appendFormat:@"Layered loop %s\n", loop->name().c_str()];
            [details appendFormat:@INDENT1 "Component %ld\n", i];
            
            if (loop->isTwisted()) {
                [details appendFormat:@INDENT1 "Length %ld, twisted\n", loop->getLength()];
                [details appendFormat:@INDENT1 "Hinge: edge %ld\n", loop->getHinge(0)->index()];
            } else {
                [details appendFormat:@INDENT1 "Length %ld, not twisted\n", loop->getLength()];
                [details appendFormat:@INDENT1 "Hinges: edge %ld, %ld\n",
                 loop->getHinge(0)->index(),
                 loop->getHinge(1)->index()];
            }
            
            [details appendString:@"\n"];
            delete loop;
        }
    }
}

- (void)findPlugTriSolidTori:(NSMutableString*)details
{
    unsigned long nComps = self.packet->countComponents();
    
    regina::NPlugTriSolidTorus* plug;
    const regina::NLayeredChain* chain;
    for (unsigned long i = 0; i < nComps; i++) {
        plug = regina::NPlugTriSolidTorus::isPlugTriSolidTorus(self.packet->component(i));
        if (plug) {
            [details appendFormat:@"Plugged triangular solid torus %s\n", plug->name().c_str()];
            [details appendFormat:@INDENT1 "Component %ld\n", i];

            const regina::NTriSolidTorus& core(plug->core());
            [details appendFormat:@INDENT1 "Core: tets %ld, %ld, %ld\n",
             core.getTetrahedron(0)->index(),
             core.getTetrahedron(1)->index(),
             core.getTetrahedron(2)->index()];

            [details appendString:@INDENT1 "Chain lengths: "];
            for (int j = 0; j < 3; j++) {
                chain = plug->chain(j);
                if (chain) {
                    [details appendFormat:@"%ld", chain->index()];
                    if (plug->chainType(j) == regina::NPlugTriSolidTorus::CHAIN_MAJOR)
                        [details appendString:@" (major)"];
                    else
                        [details appendString:@" (minor)"];
                } else
                    [details appendString:@"0"];
                if (j < 2)
                    [details appendString:@", "];
            }
            [details appendString:@"\n"];

            if (plug->equatorType() == regina::NPlugTriSolidTorus::EQUATOR_MAJOR)
                [details appendString:@INDENT1 "Equator type: major\n"];
            else
                [details appendString:@INDENT1 "Equator type: minor\n"];

            [details appendString:@"\n"];
            delete plug;
        }
    }
}

- (void)describeSatRegion:(const regina::NSatRegion&)region details:(NSMutableString*)details
{
    regina::NSatBlockSpec spec;
    regina::NSatAnnulus ann;
    unsigned nAnnuli;
    long b;
    int a;
    bool ref, back;
    NSString *thisAnnulus, *adjAnnulus;
    for (b = region.numberOfBlocks() - 1; b >= 0; b--) {
        spec = region.block(b);
        [details appendFormat:@INDENT2 "Block %ld: %s\n", b, spec.block->abbr().c_str()];
        
        nAnnuli = spec.block->nAnnuli();
        
        [details appendString:@INDENT3 "Adjacencies:\n"];
        
        for (a = nAnnuli - 1; a >= 0; a--) {
            thisAnnulus = [NSString stringWithFormat:@"Annulus %ld/%d", b, a];
            if (! spec.block->hasAdjacentBlock(a))
                [details appendFormat:@INDENT4 "%@ → boundary\n", thisAnnulus];
            else {
                adjAnnulus = [NSString stringWithFormat:@"Annulus %ld/%d",
                              region.blockIndex(spec.block->adjacentBlock(a)),
                              spec.block->adjacentAnnulus(a)];
                ref = spec.block->adjacentReflected(a);
                back = spec.block->adjacentBackwards(a);
                
                if (ref && back)
                    [details appendFormat:@INDENT4 "%@ → %@ (reflected, backwards)\n", thisAnnulus, adjAnnulus];
                else if (ref)
                    [details appendFormat:@INDENT4 "%@ → %@ (reflected)\n", thisAnnulus, adjAnnulus];
                else if (back)
                    [details appendFormat:@INDENT4 "%@ → %@ (backwards)\n", thisAnnulus, adjAnnulus];
                else
                    [details appendFormat:@INDENT4 "%@ → %@\n", thisAnnulus, adjAnnulus];
            }
        }
        
        if (nAnnuli == 1) {
            [details appendString:@INDENT3 "1 annulus\n"];
        } else {
            [details appendFormat:@INDENT3 "%d annuli\n", nAnnuli];
        }
        for (a = nAnnuli - 1; a >= 0; a--) {
            thisAnnulus = [NSString stringWithFormat:@"Annulus %ld/%d", b, a];
            ann = spec.block->annulus(a);
            
            [details appendFormat:@INDENT4 "%@ : Tet %ld (%d%d%d), Tet %ld (%d%d%d)\n",
             thisAnnulus,
             ann.tet[0]->index(),
             ann.roles[0][0], ann.roles[0][1], ann.roles[0][2],
             ann.tet[1]->index(),
             ann.roles[1][0], ann.roles[1][1], ann.roles[1][2]];
        }
        
        if (spec.refVert && spec.refHoriz)
            [details appendString:@INDENT3 "Reflected vertically and horizontally\n"];
        else if (spec.refVert)
            [details appendString:@INDENT3 "Reflected vertically\n"];
        else if (spec.refHoriz)
            [details appendString:@INDENT3 "Reflected horizontally\n"];
        else
            [details appendString:@INDENT3 "No reflections\n"];
        
        [details appendFormat:@INDENT3 "%s\n", spec.block->str().c_str()];
    }
}

- (void)findBlockedTriangulations:(NSMutableString*)details
{
    regina::NBlockedSFS* sfs = regina::NBlockedSFS::isBlockedSFS(self.packet);
    if (sfs) {
        [details appendString:@"Blocked Seifert Fibred Space\n"];
        [details appendString:@INDENT1 "Saturated region:\n"];
        [self describeSatRegion:sfs->region() details:details];
        [details appendString:@"\n"];
        delete sfs;
    }
    
    regina::NBlockedSFSLoop* loop = regina::NBlockedSFSLoop::isBlockedSFSLoop(self.packet);
    if (loop) {
        [details appendString:@"Blocked SFS Loop\n"];
        
        [details appendString:@INDENT1 "Internal region:\n"];
        [self describeSatRegion:loop->region() details:details];
        
        [details appendFormat:@INDENT1 "Matching relation: %@\n",
         [TriComposition matrixString:loop->matchingReln()]];
        
        [details appendString:@"\n"];
        delete loop;
    }
    
    regina::NBlockedSFSPair* pair = regina::NBlockedSFSPair::isBlockedSFSPair(self.packet);
    if (pair) {
        [details appendString:@"Blocked SFS Pair\n"];
        
        [details appendString:@INDENT1 "Second region:\n"];
        [self describeSatRegion:pair->region(1) details:details];
        
        [details appendString:@INDENT1 "First region:\n"];
        [self describeSatRegion:pair->region(0) details:details];
        
        [details appendFormat:@INDENT1 "Matching relation (first → second): %@\n",
         [TriComposition matrixString:pair->matchingReln()]];
        
        [details appendString:@"\n"];
        delete pair;
    }
    
    regina::NBlockedSFSTriple* triple = regina::NBlockedSFSTriple::isBlockedSFSTriple(self.packet);
    if (triple) {
        [details appendString:@"Blocked SFS Triple\n"];
        
        [details appendString:@INDENT1 "Second end region:\n"];
        [self describeSatRegion:triple->end(1) details:details];
        
        [details appendString:@INDENT1 "First end region:\n"];
        [self describeSatRegion:triple->end(0) details:details];
        
        [details appendString:@INDENT1 "Central region:\n"];
        [self describeSatRegion:triple->centre() details:details];
        
        [details appendFormat:@INDENT1 "Matching relation (centre → second end): %@\n",
         [TriComposition matrixString:triple->matchingReln(1)]];
        
        [details appendFormat:@INDENT1 "Matching relation (centre → first end): %@\n",
         [TriComposition matrixString:triple->matchingReln(0)]];
        
        [details appendString:@"\n"];
        delete triple;
    }
    
    regina::NLayeredTorusBundle* bundle = regina::NLayeredTorusBundle::isLayeredTorusBundle(self.packet);
    if (bundle) {
        [details appendString:@"Layered Torus Bundle\n"];
        
        [details appendFormat:@INDENT1 "Layering relation (lower a/b → upper a/b): %@\n",
         [TriComposition matrixString:bundle->layeringReln()]];
        
        [details appendFormat:@INDENT1 "Core relation (upper a/b → lower a/b): %@\n",
         [TriComposition matrixString:bundle->core().parallelReln()]];
        
        [details appendFormat:@INDENT1 "Core T × I triangulation: %s\n",
         bundle->core().name().c_str()];
        
        [details appendString:@"\n"];
        delete bundle;
    }
    
    regina::NPluggedTorusBundle* pBundle = regina::NPluggedTorusBundle::isPluggedTorusBundle(self.packet);
    if (pBundle) {
        [details appendString:@"Plugged Torus Bundle\n"];
        
        [details appendString:@INDENT1 "Saturated region:\n"];
        [self describeSatRegion:pBundle->region() details:details];
        
        [details appendFormat:@INDENT1 "Matching relation (joining region boundaries): %@\n",
         [TriComposition matrixString:pBundle->matchingReln()]];
        
        [details appendFormat:@INDENT1 "Thin I-bundle (T × I): %s\n",
         pBundle->bundle().name().c_str()];
        
        [details appendString:@"\n"];
        delete pBundle;
    }
}

- (void)findLayeredSolidTori:(NSMutableString*)details
{
    unsigned long nTets = self.packet->size();
    
    regina::NLayeredSolidTorus* torus;
    unsigned long topIndex;
    for (unsigned long i = 0; i < nTets; i++) {
        torus = regina::NLayeredSolidTorus::formsLayeredSolidTorusBase(self.packet->getTetrahedron(i));
        if (torus) {
            [details appendFormat:@"Layered solid torus %s\n", torus->name().c_str()];
            [details appendFormat:@INDENT1 "Base: tet %ld\n", torus->getBase()->index()];
            topIndex = torus->getTopLevel()->index();
            [details appendFormat:@INDENT1 "Top level: tet %ld\n", topIndex];
            [details appendFormat:@INDENT1 "Weight %ld edge: %@\n",
             torus->getMeridinalCuts(0),
             [TriComposition edgeStringForTorus:topIndex
                                          first:torus->getTopEdge(0, 0)
                                         second:torus->getTopEdge(0, 1)]];
            [details appendFormat:@INDENT1 "Weight %ld edge: %@\n",
             torus->getMeridinalCuts(1),
             [TriComposition edgeStringForTorus:topIndex
                                          first:torus->getTopEdge(1, 0)
                                         second:torus->getTopEdge(1, 1)]];
            [details appendFormat:@INDENT1 "Weight %ld edge: %@\n",
             torus->getMeridinalCuts(2),
             [TriComposition edgeStringForTorus:topIndex
                                          first:torus->getTopEdge(2, 0)
                                         second:torus->getTopEdge(2, 1)]];
            
            [details appendString:@"\n"];
            delete torus;
        }
    }
}

- (void)findSpiralSolidTori:(NSMutableString*)details
{
    unsigned long nTets = self.packet->size();
    
    regina::NSpiralSolidTorus* spiral;
    regina::NTetrahedron* tet;
    int whichPerm;
    unsigned long i, j;
    for (i = 0; i < nTets; i++) {
        tet = self.packet->getTetrahedron(i);
        for (whichPerm = 0; whichPerm < 24 ; ++whichPerm) {
            if (regina::NPerm4::S4[whichPerm][0] > regina::NPerm4::S4[whichPerm][3])
                continue;
            
            spiral = regina::NSpiralSolidTorus::formsSpiralSolidTorus(tet, regina::NPerm4::S4[whichPerm]);
            if (! spiral)
                continue;
            if (! spiral->isCanonical(self.packet)) {
                delete spiral;
                continue;
            }
            
            // We've got one!
            [details appendFormat:@"Spiralled solid torus %s\n", spiral->name().c_str()];
            
            unsigned long spiralTets = spiral->size();
            
            unsigned long* tetIndex = new unsigned long[spiralTets];
            for (j = 0; j < spiralTets; j++)
                tetIndex[j] = self.packet->tetrahedronIndex(spiral->tetrahedron(j));
            
            [details appendString:(spiralTets == 1 ? @INDENT1 "Tet: " : @INDENT1 "Tets: ")];
            for (j = 0; j < spiralTets; j++) {
                if (j > 0)
                    [details appendString:@", "];
                [details appendFormat:@"%ld", tetIndex[j]];
            }
            [details appendString:@"\n"];

            [details appendString:@INDENT1 "Major edges:\n"];
            for (j = 0; j < spiralTets; j++) {
                [details appendFormat:@INDENT2 "%@ = %@ = %@",
                 [TriComposition edgeStringForTet:tetIndex[(j + spiralTets - 1) % spiralTets]
                                            roles:spiral->vertexRoles((j + spiralTets - 1) % spiralTets)
                                            start:2
                                              end:3],
                 [TriComposition edgeStringForTet:tetIndex[j]
                                            roles:spiral->vertexRoles(j)
                                            start:1
                                              end:2],
                 [TriComposition edgeStringForTet:tetIndex[(j + 1) % spiralTets]
                                            roles:spiral->vertexRoles((j + 1) % spiralTets)
                                            start:0
                                              end:1]];
                [details appendString:@"\n"];
            }
            
            [details appendString:@INDENT1 "Minor edges:\n"];
            for (j = 0; j < spiralTets; j++) {
                [details appendFormat:@INDENT2 "%@ = %@",
                 [TriComposition edgeStringForTet:tetIndex[j]
                                            roles:spiral->vertexRoles(j)
                                            start:1
                                              end:3],
                 [TriComposition edgeStringForTet:tetIndex[(j + 1) % spiralTets]
                                            roles:spiral->vertexRoles((j + 1) % spiralTets)
                                            start:0
                                              end:2]];
                [details appendString:@"\n"];
            }
            
            [details appendString:@INDENT1 "Axis edges:\n"];
            for (j = 0; j < spiralTets; j++) {
                [details appendFormat:@INDENT2 "%@",
                 [TriComposition edgeStringForTet:tetIndex[j]
                                            roles:spiral->vertexRoles(j)
                                            start:0
                                              end:3]];
                [details appendString:@"\n"];
            }
            
            [details appendString:@"\n"];
            delete spiral;
        }
    }
}

- (void)findSnappedBalls:(NSMutableString*)details
{
    unsigned long nTets = self.packet->size();
    
    regina::NSnappedBall* ball;
    for (unsigned long i = 0; i < nTets; i++) {
        ball = regina::NSnappedBall::formsSnappedBall(self.packet->getTetrahedron(i));
        if (ball) {
            [details appendString:@"Snapped 3-ball\n"];
            [details appendFormat:@INDENT1 "Tetrahedron %ld\n", i];
            [details appendFormat:@INDENT1 "Equator: edge %d%d\n",
             ball->internalFace(0),
             ball->internalFace(1)];
            
            [details appendString:@"\n"];
            delete ball;
        }
    }
}

- (void)findPillowSpheres:(NSMutableString*)details
{
    unsigned long nTriangles = self.packet->countTriangles();
    
    unsigned long i, j;
    regina::NTriangle* f1;
    regina::NTriangle* f2;
    regina::NPillowTwoSphere* pillow;
    for (i = 0; i < nTriangles; i++) {
        f1 = self.packet->getTriangle(i);
        for (j = i + 1; j < nTriangles; j++) {
            f2 = self.packet->getTriangle(j);
            pillow = regina::NPillowTwoSphere::formsPillowTwoSphere(f1, f2);
            if (pillow) {
                [details appendString:@"Pillow 2-sphere\n"];
                [details appendFormat:@INDENT1 "Triangles: %ld, %ld\n", i, j];
                [details appendFormat:@INDENT1 "Equator: edges %ld, %ld, %ld\n",
                 f1->getEdge(0)->index(),
                 f1->getEdge(1)->index(),
                 f1->getEdge(2)->index()];
                
                [details appendString:@"\n"];
                delete pillow;
            }
        }
    }
}

- (void)findSnappedSpheres:(NSMutableString*)details
{
    unsigned long nTets = self.packet->size();
    
    unsigned long i, j;
    regina::NTetrahedron* t1;
    regina::NTetrahedron* t2;
    regina::NSnappedTwoSphere* sphere;
    for (i = 0; i < nTets; i++) {
        t1 = self.packet->getTetrahedron(i);
        for (j = i + 1; j < nTets; j++) {
            t2 = self.packet->getTetrahedron(j);
            sphere = regina::NSnappedTwoSphere::formsSnappedTwoSphere(t1, t2);
            if (sphere) {
                [details appendString:@"Snapped 2-sphere\n"];
                [details appendFormat:@INDENT1 "Tetrahedra: %ld, %ld\n", i, j];
                
                const regina::NSnappedBall* ball = sphere->snappedBall(0);
                [details appendFormat:@INDENT1 "Equator: edge %ld\n",
                 ball->tetrahedron()->getEdge(ball->equatorEdge())->index()];
                
                [details appendString:@"\n"];
                delete sphere;
            }
        }
    }
}
                 
+ (NSString*)edgeStringForTet:(unsigned long)tetIndex roles:(const regina::NPerm4&)roles start:(int)startPreimage end:(int)endPreimage
{
    return [NSString stringWithFormat:@"%ld (%d%d)", tetIndex, roles[startPreimage], roles[endPreimage]];
}

+ (NSString*)edgeStringForTorus:(unsigned long)tetIndex first:(int)first second:(int)second
{
    if (first < 0)
        return @"None";
    else if (second < 0)
        return [NSString stringWithFormat:@"%ld (%d%d)",
                tetIndex,
                regina::NEdge::edgeVertex[first][0],
                regina::NEdge::edgeVertex[first][1]];
    else
        return [NSString stringWithFormat:@"%ld (%d%d) = %ld (%d%d)",
                tetIndex,
                regina::NEdge::edgeVertex[first][0],
                regina::NEdge::edgeVertex[first][1],
                tetIndex,
                regina::NEdge::edgeVertex[second][0],
                regina::NEdge::edgeVertex[second][1]];
}

+ (NSString*)matrixString:(const regina::NMatrix2&)matrix
{
    return [NSString stringWithFormat:@"[ %ld %ld | %ld %ld ]",
            matrix[0][0], matrix[0][1], matrix[1][0], matrix[1][1]];
}

@end
