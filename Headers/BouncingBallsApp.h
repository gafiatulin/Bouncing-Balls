//
//  BouncingBallsApp.h
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 29.09.13.
//
//

#ifndef __BouncingBalls__BouncingBallsApp__
#define __BouncingBalls__BouncingBallsApp__

#include "BallController.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"


class BouncingBallsApp : public AppCocoaView {
public:
	void setup();
    void update();
	void draw();
    BallController mBallController;
private:
    gl::VboMesh	mMesh;
    gl::Texture mTexture;
};

#endif /* defined(__BouncingBalls__BouncingBallsApp__) */