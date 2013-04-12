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
}

void World::ccTouchesBegan(CCSet *touches, CCEvent* event) {
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch) {
            b2Vec2 pos = pBody->GetPosition();
			touchOffset = ccpSub(ccp(pos.x*PTM_RATIO, pos.y*PTM_RATIO),touchToPoint(touch));
        }
    }
}

void World::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch && touchOffset.x && touchOffset.y) {
            lastPos = pBody->GetPosition();
			CCPoint newpos = ccpAdd(touchToPoint(touch), touchOffset);
//            b2Vec2 linearVelocity = b2Vec2(newpos.x/PTM_RATIO, newpos.y/PTM_RATIO) - pBody->GetPosition();
//            pBody->SetLinearVelocity(linearVelocity);
            pBody->SetTransform(b2Vec2(newpos.x/PTM_RATIO, newpos.y/PTM_RATIO), pBody->GetAngle());

        }
    }
}

void World::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch && touchOffset.x && touchOffset.y) {
            b2Vec2 linearVelocity = b2Vec2(lastPos - pBody->GetPosition());
            linearVelocity *= linearVelocity.Normalize() * -1 * 15;
            pBody->SetLinearVelocity(linearVelocity);
        }
    }
    
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