//
//  World.cpp
//  RotE
//
//  Created by Jarad Delorenzo on 3/29/13.
//
//

#include "World.h"

World::World() {
    this->setTouchEnabled(true);
    pMap = CCTMXTiledMap::create(TMX_FILE);
    pMap->setAnchorPoint(ccp(0.5f, 0.5f));
    zoomFactor = 1;
    addChild(pMap, 1);
    b2Vec2 grav = b2Vec2(0, 0);
    pWorld = new b2World(grav);
    b2BodyDef mapBodyDef;
    mapBodyDef.position.Set(0.0, 0.0);
    mapBodyDef.type = b2_dynamicBody;
    mapBodyDef.userData = pMap;
    pBody = pWorld->CreateBody(&mapBodyDef);
    b2FixtureDef pMapFix;
    b2CircleShape mapHandle;
    mapHandle.m_radius = 1.0f;
    pMapFix.shape = &mapHandle;
    pMapFix.density = 1.0f;
    pBody->CreateFixture(&pMapFix);
    mapBodyDef.type = b2_staticBody;
    pMapFrictionBody = pWorld->CreateBody(&mapBodyDef);
    
    b2FrictionJointDef fjDef;
    fjDef.collideConnected = true;
    fjDef.localAnchorA.SetZero();
    fjDef.localAnchorB.SetZero();
    fjDef.maxForce = 40;
    fjDef.bodyA = pBody;
    fjDef.bodyB = pMapFrictionBody;
    
    pWorld->CreateJoint(&fjDef);
    
	worldBoundary = CCRectMake(	0,
								0,
								pMap->getTileSize().width*getWorldSize().width, 
								pMap->getTileSize().height*getWorldSize().height);

	elapsedTime = 0;
    
    // Zero out all touch indicies
    for(int i=0; i<MAX_TOUCHES; i++) {
        touchMap[i] = NULL;
    }
    
	scheduleUpdate();
}

World::~World() {
    removeChild(pMap);
}

CCPoint World::touchToPoint(CCTouch* touch)
{
	// convert the touch object to a position in our cocos2d space
	return CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
}

CCSize World::getWorldSize() {
    return pMap->getMapSize();
}

void World::update(float dt) {
	elapsedTime += dt;
    
    pWorld->Step(dt, 10, 10);
    for(b2Body *b = pWorld->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData() != NULL) {
            CCSprite *obj = (CCSprite *)b->GetUserData();
            obj->setPosition(ccp(b->GetPosition().x * PTM_RATIO,
                                 b->GetPosition().y * PTM_RATIO));
        }
    }
    pMap->setScale(zoomFactor);
}

void World::ccTouchesBegan(CCSet *touches, CCEvent* event) {
    if(numTouches() >= MAX_TOUCHES) return;
    
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch) {
            if(numTouches() == 0) {
                touchMap[0] = touch;
                b2Vec2 pos = pBody->GetPosition();
                touchOffset = ccpSub(ccp(pos.x*PTM_RATIO, pos.y*PTM_RATIO),touchToPoint(touch));
            } else if(numTouches() == 1) {
                touchMap[1] = touch;
                CCPoint p1 = touchToPoint(touchMap[0]);
                CCPoint p2 = touchToPoint(touchMap[1]);
                initialTouchDistance = ccpDistance(p1, p2);
                initialZoomFactor = zoomFactor;
                lastTouchDistance = initialTouchDistance;
            }
        }
    }
    CCLOG("%d", numTouches());
}

void World::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch && touchOffset.x && touchOffset.y) {

            if(!touchMap[0]) return; //if released fingers in reverse order, return
            if(touch == touchMap[0]) {   //if finger on screen is not the movey finger, return
                lastPos = pBody->GetPosition();
                CCPoint newpos = ccpAdd(touchToPoint(touch), touchOffset);
                pBody->SetTransform(b2Vec2(newpos.x/PTM_RATIO, newpos.y/PTM_RATIO), pBody->GetAngle());
            }
            
            if(numTouches() == 2) {
                CCPoint p1 = touchToPoint(touchMap[0]);
                CCPoint p2 = touchToPoint(touchMap[1]);
                float currentTouchDistance = ccpDistance(p1, p2);
                zoomFactor = -(initialTouchDistance - currentTouchDistance)*0.015f + initialZoomFactor;
                if(zoomFactor < 0.5f) {
                    zoomFactor = 0.5f;
                }
                CCLOG("Zoom Factor: %f", zoomFactor);
            }

        }
    }
}

void World::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    int i=0;
    for(auto it = touches->begin(); it != touches->end(); it++, i++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch && touchOffset.x && touchOffset.y) {
            if(touch->getID() >= MAX_TOUCHES) return;
            touchMap[touch->getID()] = NULL;
            b2Vec2 linearVelocity = b2Vec2(lastPos - pBody->GetPosition());
            linearVelocity *= linearVelocity.Normalize() * -1 * 15;
            pBody->SetLinearVelocity(linearVelocity);
        }
    }
    CCLOG("%d", numTouches());
    
//	float denom = (elapsedTime - prevTouchTime);
//	denom = denom==0 ? 0.0001 : denom;
//	float mag = ccpDistance(oldPos, newPos) / denom;
//	CCLOG("Mag = %f", mag);
//    
//    CCPoint direction = ccpNormalize(ccp(oldPos.x - newPos.x, oldPos.y - newPos.y));
//    CCPoint driftPos = ccpMult(direction, -40);
//	CCMoveTo* move = CCMoveTo::create(1.0f, driftPos);
//	CCEaseOut* ease = CCEaseOut::create(move, 2);
//	pMap->runAction(ease);
//
//    CCLOG("direction = %f,%f", direction.x, direction.y);
//    direction = ccpMult(direction, clampf(mag, 0, 30));
//    CCLOG("direction*clamp = %f,%f", direction.x, direction.y);

}

int World::numTouches() {
    int count = 0;
    for(int i=0; i<MAX_TOUCHES; i++) {
        if(touchMap[i]) count++;
    }
    return count;
}
