//
//  WelcomeViewController.m
//  Regina
//
//  Created by Ben Burton on 13/10/13.
//  Copyright (c) 2013 Regina Development Team. All rights reserved.
//

#import "WelcomeViewController.h"

@implementation WelcomeViewController

- (void)viewDidLoad {
    NSLog(@"Welcome!");
    
    [super viewDidLoad];
    
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"welcome" withExtension:@"html"];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [_html loadRequest:request];
}

@end
