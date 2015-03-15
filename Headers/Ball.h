//
//  Ball.h
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 29.09.13.
//
//

#ifndef BouncingBalls_Ball_h
#define BouncingBalls_Ball_h

#import <sstream>
#include "cinder/app/AppCocoaView.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

class Ball
{
public:
    Ball();
    ~Ball();
    Ball(std::string groupUnigueID, float radius, float hue, float sat, float bri);
    Ball(std::string groupUnigueID, float radius, float x1, float y1, float x2, float y2, float r, float g, float b);
    void update();
    void draw(const gl::VboMesh &mesh);
    void collideWithWalls();
    bool isCollidingWith(Ball *other);
    bool isNear(Ball *other);
    void collideWith(Ball *other);
    std::string description();
    
    std::string mGroupUnigueID;
    float mRadius;
    float mMass;
    Colorf mColor;
    Vec2f mPos;
    Vec2f mPrevPosition;
    Vec2f mVel;
    bool inCollisionLR;
    bool inCollisionTB;
    bool mHasBeenDrawn;
};

#endif
