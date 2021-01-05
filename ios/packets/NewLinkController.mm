
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#import "NewLinkController.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "link/examplelink.h"
#import "link/link.h"

@interface NewLinkController ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *types;
@property (weak, nonatomic) IBOutlet UIView *container;
@property (weak, nonatomic) NewPacketPageViewController *pages;
@end

@implementation NewLinkController

- (void)viewDidLoad
{
    self.pages = static_cast<NewPacketPageViewController*>(self.childViewControllers.lastObject);
    [self.pages fillWithPages:@[@"newLinkExample", @"newLinkTextCode"]
                 pageSelector:self.types
                   defaultKey:@"NewLinkPage"];
}

- (IBAction)create:(id)sender
{
    regina::Packet* ans = [self.pages create];
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

#pragma mark - Example knot/link

typedef regina::Link* (*LinkCreator)();

/**
 * Represents a single option in the examples picker.
 */
@interface ExampleLink : NSObject

@property (strong, nonatomic) NSString* name;
@property (assign, nonatomic) LinkCreator creator;

+ (id)exampleWithName:(NSString*)name creator:(LinkCreator)creator;
- (regina::Link*)create;

@end

@implementation ExampleLink

+ (id)exampleWithName:(NSString *)name creator:(LinkCreator)creator
{
    ExampleLink* e = [[ExampleLink alloc] init];
    if (e) {
        e.name = name;
        e.creator = creator;
    }
    return e;
}

- (regina::Link *)create
{
    regina::Link* ans = (*self.creator)();
    ans->setLabel(self.name.UTF8String);
    return ans;
}

@end

#pragma mark - Example page

@interface NewLinkExamplePage () <UIPickerViewDataSource, UIPickerViewDelegate> {
    NSArray* options;
}
@property (weak, nonatomic) IBOutlet UIPickerView *example;
@end

#define KEY_LAST_EXAMPLE @"NewLinkExample"

@implementation NewLinkExamplePage

- (void)viewDidLoad
{
    options = @[[ExampleLink exampleWithName:@"Borromean rings" creator:&regina::ExampleLink::borromean],
                [ExampleLink exampleWithName:@"Conway knot" creator:&regina::ExampleLink::conway],
                [ExampleLink exampleWithName:@"Figure eight knot" creator:&regina::ExampleLink::figureEight],
                [ExampleLink exampleWithName:@"Gompf-Scharlemann-Thompson" creator:&regina::ExampleLink::gst],
                [ExampleLink exampleWithName:@"Hopf link" creator:&regina::ExampleLink::hopf],
                [ExampleLink exampleWithName:@"Kinoshita-Terasaka knot" creator:&regina::ExampleLink::kinoshitaTerasaka],
                [ExampleLink exampleWithName:@"Trefoil (left)" creator:&regina::ExampleLink::trefoilLeft],
                [ExampleLink exampleWithName:@"Trefoil (right)" creator:&regina::ExampleLink::trefoilRight],
                [ExampleLink exampleWithName:@"Unknot (no crossings)" creator:&regina::ExampleLink::unknot],
                [ExampleLink exampleWithName:@"Unknot (10-crossing monster)" creator:&regina::ExampleLink::monster],
                [ExampleLink exampleWithName:@"Unknot (141-crossing Gordian)" creator:&regina::ExampleLink::gordian],
                [ExampleLink exampleWithName:@"Whitehead link" creator:&regina::ExampleLink::whitehead]];
   
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

- (regina::Packet *)create
{
    return [options[[self.example selectedRowInComponent:0]] create];
}

@end

#pragma mark - Text code page

@interface NewLinkTextCodePage ()
@property (weak, nonatomic) IBOutlet UITextField *code;
@end

@implementation NewLinkTextCodePage

- (IBAction)editingEnded:(id)sender {
    NewLinkController* c = static_cast<NewLinkController*>(self.parentViewController.parentViewController);
    [c create:sender];
}

- (regina::Packet *)create
{
    std::string code = [self.code.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]].UTF8String;
    if (code.empty()) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Empty Text Code"
                              message:@"Please type a text code into the box provided."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }

    regina::Link* l = new regina::Link(code);
    if (l->isEmpty()) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Invalid Text Code"
                              message:nil
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        delete l;
        return 0;
    }

    l->setLabel(code);
    return l;
}

@end
