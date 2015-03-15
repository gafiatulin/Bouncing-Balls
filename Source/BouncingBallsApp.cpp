//
//  BouncingBallsApp.cpp
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 29.09.13.
//
//

#include "BouncingBallsApp.h"

void BouncingBallsApp::setup()
{
    Rand::randSeed(clock());
    setFrameRate(30.0f);
    size_t slices = 50;
    std::vector<Vec3f> positions;
    std::vector<Vec2f> texcoords;
    std::vector<uint32_t> indices;
    indices.push_back( positions.size());
    texcoords.push_back( Vec2f(0.5f, 0.5f) );
    positions.push_back( Vec3f::zero() );
    for(size_t i=0;i<=slices;++i)
    {
        float angle = i / (float) slices * 2.0f * (float) M_PI;
        Vec2f v(sinf(angle), cosf(angle));
        indices.push_back( positions.size() );
        texcoords.push_back( Vec2f(0.5f, 0.5f) + 0.5f * v );
        positions.push_back( 10.0f * Vec3f(v, 0.0f) );
    }
    gl::VboMesh::Layout layout;
    layout.setStaticPositions();
    layout.setStaticTexCoords2d();
    layout.setStaticIndices();
    mMesh = gl::VboMesh( (size_t) (slices + 2), (size_t) (slices + 2), layout, GL_TRIANGLE_FAN );
    mMesh.bufferPositions( &positions.front(), positions.size());
    mMesh.bufferTexCoords2d(0, texcoords);
    mMesh.bufferIndices( indices );
    
    mTexture = gl::Texture(loadImage(loadResource("ball.png")));
    gl::enableVerticalSync();
}

void BouncingBallsApp::update()
{
    mBallController.update();
}

void BouncingBallsApp::draw()
{
	gl::clear(Color(1,1,1), true);
	gl::enable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR , GL_ONE_MINUS_SRC_ALPHA);
    if(mTexture) mTexture.enableAndBind();
    mBallController.draw(mMesh);
    if(mTexture) mTexture.unbind();
    gl::disable(GL_BLEND);
    gl::color(0.5f, 0.5f, 0.5f);
    gl::drawLine(Vec2f::zero(), Vec2f(0.0f, getWindowHeight()));
    gl::drawLine(Vec2f(0.0f, getWindowHeight()), Vec2f(getWindowWidth(), getWindowHeight()));
    gl::drawLine(Vec2f(getWindowWidth(), getWindowHeight()), Vec2f(getWindowWidth(), 0.0f));
    gl::drawLine(Vec2f(getWindowWidth(), 0.0f), Vec2f::zero());
}
