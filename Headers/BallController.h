//
//  BallController.h
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 29.09.13.
//
//

#ifndef __BouncingBalls__BallController__
#define __BouncingBalls__BallController__

#include "Ball.h"

#include <vector>

class BallController
{
public:
    BallController();
    void update();
    void collide();
    void draw(const gl::VboMesh &mesh);
    void addBall();
    void addBallWithGroup(std::string groupUnigueId, float radius, float hue, float sat, float bri);
    void addBallWithGroup(std::string groupUnigueId, float radius, float x1, float y1, float x2, float y2, float r, float g, float b);
    void removeBall();
    void removeBallWithGroup(std::string groupUnigueId);
    std::vector<std::string> getBallsWithGroup(std::string groupUnigueId);
    std::vector<Ball*> mBalls;
};


#endif /* defined(__BouncingBalls__BallController__) */
