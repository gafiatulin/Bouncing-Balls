//
//  BallController.cpp
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 29.09.13.
//
//
#include "BallController.h"

#include <fstream>

BallController::BallController()
{
}

void BallController::update()
{
	for( std::vector<Ball*>::iterator p = mBalls.begin(); p != mBalls.end(); ++p )
		(*p)->update();
    collide();
}

void BallController::collide()
{
    for(std::vector<Ball*>::iterator p1 = mBalls.begin(); p1 < mBalls.end()-1; ++p1 )
        for(std::vector<Ball*>::iterator p2 = p1+1; p2 < mBalls.end(); ++p2 )
            if((*p1)->isNear(*p2))
                if((*p1)->isCollidingWith(*p2))
                    (*p1)->collideWith(*p2);
}

void BallController::draw(const gl::VboMesh &mesh)
{
	for( std::vector<Ball*>::iterator p = mBalls.begin(); p != mBalls.end(); ++p )
		(*p)->draw(mesh);
}

void BallController::addBall()
{
	mBalls.push_back( new Ball() );
}
void BallController::addBallWithGroup(std::string groupUnigueId, float radius, float hue, float sat, float bri)
{
    mBalls.push_back(new Ball(groupUnigueId, radius, hue, sat, bri));
}

void BallController::addBallWithGroup(std::string groupUnigueId, float radius, float x1, float y1, float x2, float y2, float r, float g, float b)
{
    mBalls.push_back(new Ball(groupUnigueId, radius, x1, y1, x2, y2, r, g, b));
}

void BallController::removeBall()
{
    if(!mBalls.empty())
    {
        delete mBalls.back();
		mBalls.pop_back();
    }
}
void BallController::removeBallWithGroup(std::string groupUnigueId)
{
    if(!mBalls.empty())
    {
        std::vector<Ball*>temp;
        temp.resize(mBalls.size());
        std::reverse_copy(mBalls.begin(), mBalls.end(), temp.begin());
        temp.erase(std::find_if(temp.begin(), temp.end(), [groupUnigueId](Ball* b)->bool{return b->mGroupUnigueID== groupUnigueId;}));
        mBalls.clear();
        mBalls.resize(temp.size());
        std::reverse_copy(temp.begin(), temp.end(), mBalls.begin());
    }
}

std::vector<std::string>BallController::getBallsWithGroup(std::string groupUnigueId)
{
    std::vector<std::string> temp;
    std::for_each(mBalls.begin(), mBalls.end(), [&temp, groupUnigueId](Ball* b)
    {
        if(b->mGroupUnigueID == groupUnigueId)
        {
            temp.push_back(b->description());
        }
    });
    return temp;
}