//
//  BallsGroup.h
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 17/11/13.
//
//

#import <Foundation/Foundation.h>

@interface BallsGroup : NSObject

@property (nonatomic, strong) NSString *uniqueID;
@property (nonatomic, strong) NSColor *groupColor;
@property (nonatomic) BOOL colorMayVary;
@property (nonatomic) float groupMinRadius;
@property (nonatomic) float groupMaxRadius;
@property (nonatomic) int ballsCounter;
-(id)initWithString:(NSString *) description;
-(NSString *) description;
@end
