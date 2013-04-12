//
//  World.h
//  RotE
//
//  Created by Jarad Delorenzo on 3/29/13.
//
//

#ifndef __RotE__World__
#define __RotE__World__

#include <cocos2d.h>
#include <Box2D/Box2D.h>

#ifdef __APPLE__
#define TMX_FILE "isometric_grass_and_water.tmx"
#else
#define TMX_FILE "../Resources/isometric_grass_and_water.tmx"
#endif

#define PTM_RATIO 32.0

using namespace cocos2d;

class World : public CCLayer {
public:
    World();
    ~World();

    CCSize getWorldSize();
    CCPoint touchToPoint(CCTouch* touch);
    void ccTouchesBegan(CCSet* touches, CCEvent* event);
    void ccTouchesMoved(CCSet* touches, CCEvent* event);
    void ccTouchesEnded(CCSet* touches, CCEvent* event);
	virtual void update(float dt);
    
protected:
    b2World* pWorld;
    b2Body* pBody;
    b2Body* pMapFrictionBody;
    b2Vec2 lastPos;
    float lastTime;
    b2FrictionJoint* pFrictionJoint;
    b2MassData mass;
    CCTMXTiledMap* pMap;
    CCPoint touchOffset;
	CCRect worldBoundary;
	float elapsedTime;
};
#endif /* defined(__RotE__World__) */
