//
//  RExamples.h
//  Regina
//
//  Created by Ben Burton on 12/10/13.
//  Copyright (c) 2013 Regina Development Team. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * Details of an example file that can be opened within Regina.
 */
@interface Example : NSObject

@property (strong, nonatomic, readonly) NSString* file;
@property (strong, nonatomic, readonly) NSString* desc;

- (id)initWithFile:(NSString*)f desc:(NSString*)d;
+ (id)exampleWithFile:(NSString*)f desc:(NSString*)d;
+ (NSArray*)all;

@end

