//
//  Ball.cpp
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 29.09.13.
//
//

#include "Ball.h"

Ball::Ball()
{
    mRadius = Rand::randFloat(10, 20);
    float x = Rand::randFloat(getWindowWidth()-2*mRadius) + mRadius;
    float y = Rand::randFloat(getWindowHeight()-2*mRadius)+ mRadius;
    mPos = Vec2f(x, y);
    mPrevPosition = mPos;
    mVel = Rand::randVec2f()* Rand::randFloat(5, 25);
    mMass = M_PI*mRadius*mRadius;
    mColor = Colorf(CM_HSV, Rand::randFloat(0.0f, 1.0f), randFloat(0.75f, 1.0f), randFloat(0.75f, 1.0f));
    inCollisionLR = inCollisionTB = false;
    mHasBeenDrawn = false;
}

Ball::Ball(std::string groupUnigueID, float radius, float hue, float sat, float bri)
{
    mGroupUnigueID = groupUnigueID;
    mRadius = radius;
    float x = Rand::randFloat(getWindowWidth()-2*mRadius) + mRadius;
    float y = Rand::randFloat(getWindowHeight()-2*mRadius)+ mRadius;
    mPos = Vec2f(x, y);
    mPrevPosition = mPos;
    mVel = Rand::randVec2f()* Rand::randFloat(5, 25);
    mMass = M_PI*mRadius*mRadius;
    mColor = Colorf(CM_HSV, hue, sat, bri);
    inCollisionLR = inCollisionTB = false;
    mHasBeenDrawn = false;
}

Ball::Ball(std::string groupUnigueID, float radius, float x1, float y1, float x2, float y2, float r, float g, float b)
{
    mGroupUnigueID = groupUnigueID;
    mRadius = radius;
    mPos = Vec2f(x1, y1);
    mPrevPosition = mPos;
    mVel = Vec2f(x2, y2);
    mMass = M_PI*mRadius*mRadius;
    mColor = Colorf(CM_RGB, r, g, b);
    inCollisionLR = inCollisionTB = false;
    mHasBeenDrawn = false;
}

void Ball::update()
{
    if(mHasBeenDrawn) mPrevPosition = mPos;
    mPos += mVel;
    collideWithWalls();
    mHasBeenDrawn = false;
}

void Ball::collideWithWalls()
{
    if ((mPos.x <= mRadius) || (mPos.x >= (getWindowWidth()-mRadius)))
    {
        if(!inCollisionLR)
        {
            mVel.x*=-1;
            inCollisionLR = true;
        }
        else
        {
            mPos.x = (mPos.x <= mRadius)? mRadius:getWindowWidth()-mRadius;
            mVel.x += (mPos.x <= mRadius)?1:-1;
        }
    }
    else
        inCollisionLR = false;
    if ((mPos.y <= mRadius) || (mPos.y >= (getWindowHeight()-mRadius)))
    {
        if(!inCollisionTB)
        {
            mVel.y*=-1;
            inCollisionTB = true;
        }
        else
        {
            mPos.y = (mPos.y <= mRadius)? mRadius:getWindowHeight()-mRadius;
            mVel.y += (mPos.y <= mRadius)?1:-1;
        }
    }
    else
        inCollisionTB = false;

}

void Ball::draw(const gl::VboMesh &mesh)
{
    gl::pushModelView();
    gl::color( mColor );
    gl::translate( mPos);
    gl::scale( Vec2f(mRadius/10.0f, mRadius/10.0f) );
    gl::draw( mesh );
	gl::popModelView();
    mHasBeenDrawn = true;
}

bool Ball::isCollidingWith(Ball *other){return (mPos - other->mPos).length() <= mRadius + other->mRadius;}

bool Ball::isNear(Ball* other){return (mPos.x + mRadius + other->mRadius > other->mPos.x) && (mPos.x < other->mPos.x + mRadius + other->mRadius) && (mPos.y + mRadius + other->mRadius > other->mPos.y) && (mPos.y < other->mPos.y + mRadius + other->mRadius);}

void Ball::collideWith(Ball *other)
{
    float minimal = mRadius + other->mRadius;
    mPos -= mVel;
    other->mPos -= other->mVel;
    Vec2f line = other->mPos - mPos;
    Vec2f un = line.normalized();
    Vec2f ut = Vec2f(-un.y, un.x);
    float distance = line.dot(un);
    float v1n = mVel.dot(un);
    float v2n = other->mVel.dot(un);
    float v1t = ut.dot(mVel);
    float v2t = ut.dot(other->mVel);
    if(v1n == v2n) return;
    float t = (minimal - distance) / (v2n - v1n);
    mPos += t * mVel;
    other->mPos += t * other->mVel;
    float newV1n = ((v1n*(mMass-other->mMass))+(2*other->mMass*v2n))/(mMass + other->mMass);
    float newV2n = ((v2n*(other->mMass-mMass))+(2*mMass*v1n))/(mMass + other->mMass);
    mVel = (newV1n*un) + (v1t*ut);
    other->mVel = (newV2n*un) + (v2t*ut);
    mPos += (1.0f - t) * mVel;
    other->mPos += (1.0f -t) * other->mVel;
    collideWithWalls();
    other->collideWithWalls();
}

std::string Ball::description()
{
    std::ostringstream ss;
    ss<<mRadius<<" "<<mPos.x<<" "<<mPos.y<<" "<<mVel.x<<" "<<mVel.y<<" "<<mColor.r<<" "<<mColor.g<<" "<<mColor.b;
    return ss.str();
}