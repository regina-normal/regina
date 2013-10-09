//
//  RDetailViewController.h
//  Regina
//
//  Created by Ben Burton on 9/10/13.
//  Copyright (c) 2013 Regina Development Team. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface RDetailViewController : UIViewController <UISplitViewControllerDelegate>

@property (strong, nonatomic) id detailItem;

@property (weak, nonatomic) IBOutlet UILabel *detailDescriptionLabel;
@end
