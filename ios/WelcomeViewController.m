//
//  WelcomeViewController.m
//  Regina
//
//  Created by Ben Burton on 13/10/13.
//  Copyright (c) 2013 Regina Development Team. All rights reserved.
//

#import "WelcomeViewController.h"

@implementation WelcomeViewController

- (IBAction)done:(id)sender {
    [self dismissViewControllerAnimated:YES completion:NULL];
}

- (void)viewDidLoad {    
    [super viewDidLoad];
    
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"welcome" withExtension:@"html"];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [_html loadRequest:request];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType {
    // Open external links in a real web browser (e.g., Safari)
    if (navigationType == UIWebViewNavigationTypeLinkClicked) {
        [[UIApplication sharedApplication] openURL:[request URL]];
        return FALSE;
    }
    return TRUE;
}

@end
