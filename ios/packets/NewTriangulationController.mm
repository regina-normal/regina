
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

#import "NewTriangulationController.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "triangulation/nexampletriangulation.h"
#import "triangulation/ntriangulation.h"

@interface NewTriangulationController ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *types;
@property (weak, nonatomic) IBOutlet UIView *container;
@property (weak, nonatomic) NewPacketPageViewController *pages;
@end

@implementation NewTriangulationController

- (void)viewDidLoad
{
    self.pages = static_cast<NewPacketPageViewController*>(self.childViewControllers.lastObject);
    [self.pages fillWithPages:@[@"newTriEmpty", @"newTriExample", @"newTriIsosig"]
                 pageSelector:self.types
                   defaultKey:@"NewTriangulationPage"];
}

- (IBAction)create:(id)sender
{
    regina::NPacket* ans = [self.pages create];
    if (ans) {
        self.spec.parent->insertChildLast(ans);
        [self.spec created:ans];
        [self dismissViewControllerAnimated:YES completion:nil];
    }
}

- (IBAction)cancel:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end

#pragma mark - Empty page

@implementation NewTriangulationEmptyPage

- (regina::NPacket*)create
{
    regina::NPacket* ans = new regina::NTriangulation();
    ans->setLabel("3-D Triangulation");
    return ans;
}

@end

#pragma mark - Example triangulation

typedef regina::NTriangulation* (*TriangulationCreator)();

/**
 * Represents a single option in the examples picker.
 */
@interface ExampleTriangulation : NSObject

@property (strong, nonatomic) NSString* name;
@property (assign, nonatomic) TriangulationCreator creator;

+ (id)exampleWithName:(NSString*)name creator:(TriangulationCreator)creator;
- (regina::NTriangulation*)create;

@end

@implementation ExampleTriangulation

+ (id)exampleWithName:(NSString *)name creator:(TriangulationCreator)creator
{
    ExampleTriangulation* e = [[ExampleTriangulation alloc] init];
    if (e) {
        e.name = name;
        e.creator = creator;
    }
    return e;
}

- (regina::NTriangulation *)create
{
    regina::NTriangulation* ans = (*self.creator)();
    ans->setLabel(self.name.UTF8String);
    return ans;
}

@end

#pragma mark - Example page

@interface NewTriangulationExamplePage () <UIPickerViewDataSource, UIPickerViewDelegate> {
    NSArray* options;
}
@property (weak, nonatomic) IBOutlet UIPickerView *example;
@end

#define KEY_LAST_EXAMPLE @"NewTriangulationExample"

@implementation NewTriangulationExamplePage

- (void)viewDidLoad
{
    options = @[[ExampleTriangulation exampleWithName:@"3-sphere (1 tetrahedron)" creator:&regina::NExampleTriangulation::threeSphere],
                [ExampleTriangulation exampleWithName:@"3-sphere (dual to Bing's house)" creator:&regina::NExampleTriangulation::bingsHouse],
                [ExampleTriangulation exampleWithName:@"3-sphere (600-cell)" creator:&regina::NExampleTriangulation::sphere600],
                [ExampleTriangulation exampleWithName:@"Connected sum ℝP³ # ℝP³" creator:&regina::NExampleTriangulation::rp3rp3],
                [ExampleTriangulation exampleWithName:@"Figure eight knot complement" creator:&regina::NExampleTriangulation::figureEightKnotComplement],
                [ExampleTriangulation exampleWithName:@"Gieseking manifold" creator:&regina::NExampleTriangulation::gieseking],
                [ExampleTriangulation exampleWithName:@"Lens space L(8,3)" creator:&regina::NExampleTriangulation::lens8_3],
                [ExampleTriangulation exampleWithName:@"Poincaré homology sphere" creator:&regina::NExampleTriangulation::poincareHomologySphere],
                [ExampleTriangulation exampleWithName:@"Product ℝP² × S¹" creator:&regina::NExampleTriangulation::rp2xs1],
                [ExampleTriangulation exampleWithName:@"Product S² × S¹" creator:&regina::NExampleTriangulation::s2xs1],
                [ExampleTriangulation exampleWithName:@"Solid Klein bottle" creator:&regina::NExampleTriangulation::solidKleinBottle],
                [ExampleTriangulation exampleWithName:@"Trefoil knot complement" creator:&regina::NExampleTriangulation::trefoilKnotComplement],
                [ExampleTriangulation exampleWithName:@"Weeks manifold" creator:&regina::NExampleTriangulation::weeks],
                [ExampleTriangulation exampleWithName:@"Weber-Seifert dodecahedral space" creator:&regina::NExampleTriangulation::weberSeifert],
                [ExampleTriangulation exampleWithName:@"Whitehead link complement" creator:&regina::NExampleTriangulation::whiteheadLinkComplement]];
    
    self.example.dataSource = self;
    self.example.delegate = self;
    
    [self.example selectRow:[[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_EXAMPLE] inComponent:0 animated:NO];
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return options.count;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return [options[row] name];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    [[NSUserDefaults standardUserDefaults] setInteger:[self.example selectedRowInComponent:0] forKey:KEY_LAST_EXAMPLE];
}

- (regina::NPacket *)create
{
    return [options[[self.example selectedRowInComponent:0]] create];
}

@end

#pragma mark - Isosig page

@interface NewTriangulationIsosigPage ()
@property (weak, nonatomic) IBOutlet UITextField *isosig;
@end

@implementation NewTriangulationIsosigPage

- (IBAction)editingEnded:(id)sender {
    NewTriangulationController* c = static_cast<NewTriangulationController*>(self.parentViewController.parentViewController);
    [c create:sender];
}

- (regina::NPacket *)create
{
    std::string sig = [self.isosig.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]].UTF8String;
    if (sig.empty()) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Empty Isomorphism Signature"
                              message:@"Please type an isomorphism signature into the box provided."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }
    
    regina::NTriangulation* t = regina::NTriangulation::fromIsoSig(sig);
    if (! t) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Invalid Isomorphism Signature"
                              message:nil
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }
    
    t->setLabel(sig);
    return t;
}

@end
