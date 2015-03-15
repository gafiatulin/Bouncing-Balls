//
//  BallsGroup.m
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 17/11/13.
//
//

#import "BallsGroup.h"

@implementation BallsGroup

- (id)init
{
    self = [super init];
    if (self)
    {
        _groupColor = [NSColor greenColor];
        _uniqueID = @"Default";
        _ballsCounter = 0;
        _colorMayVary = YES;
        _groupMaxRadius = 100;
        _groupMinRadius = 5;
    }
    return self;
}

-(id)initWithString:(NSString *) description
{
    self = [super init];
    if(self){
        NSArray * p =[description componentsSeparatedByString:@", "];
        _uniqueID = p[0];
        _groupColor = [NSColor colorWithRed:[p[1] floatValue] green:[p[2] floatValue] blue:[p[3] floatValue] alpha:1];
        _colorMayVary = [p[4] boolValue];
        _groupMinRadius = [p[5] doubleValue];
        _groupMaxRadius = [p[6] doubleValue];
        _ballsCounter =[p[7] integerValue];
    }
    return self;
}

-(NSString *) description
{
    return [[NSString alloc] initWithFormat:@"%@, %f, %f, %f, %hhd, %f, %f, %d",_uniqueID, [_groupColor redComponent],[_groupColor greenComponent], [_groupColor blueComponent], _colorMayVary, _groupMinRadius, _groupMaxRadius, _ballsCounter];
}
@end
