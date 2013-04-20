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

#define CLAMP(x,y,z) MIN(MAX(x,y),z)
#define PTM_RATIO 32.0
#define MAX_TOUCHES 2

using namespace cocos2d;

class World : public CCLayer {
public:
    World();
    ~World();

    CCSize getWorldSize();
    CCPoint touchToPoint(CCTouch* touch);
    CCPoint positionForTile(CCPoint p);
    CCPoint tileForPosition(CCPoint p);
    void ccTouchesBegan(CCSet* touches, CCEvent* event);
    void ccTouchesMoved(CCSet* touches, CCEvent* event);
    void ccTouchesEnded(CCSet* touches, CCEvent* event);
	virtual void update(float dt);    
    
protected:
    b2World* pWorld;
    b2Body* pBody;
    b2Body* pMapFrictionBody;
    b2Vec2 lastPos;
    CCTMXLayer* m_pMapLayer;
    float lastTime;
    CCTouch* touchMap[MAX_TOUCHES];
    b2FrictionJoint* pFrictionJoint;
    b2MassData mass;
    CCTMXTiledMap* pMap;
    CCPoint touchOffset;
	CCRect worldBoundary;
	float elapsedTime;
    double zoomFactor;
    double initialZoomFactor;
    int numTouches();
    float initialTouchDistance;
    float lastTouchDistance;
	bool m_bMapMoved;
    CCSprite* m_pTestSprite;
    
    void updateTestSprite();
    
};
#endif /* defined(__RotE__World__) */
