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

	pViewport = CCNode::create();
	pViewport->setPosition(500,200);
	pMap->addChild(pViewport, 1);

	worldBoundary = CCRectMake(	0,
								0,
								pMap->getTileSize().width*getWorldSize().width, 
								pMap->getTileSize().height*getWorldSize().height);

	elapsedTime = 0;
	scheduleUpdate();
	//scrollAction = CCFollow::create(pViewport, worldBoundary);
	//runAction(scrollAction);	
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
}

void World::ccTouchesBegan(CCSet *touches, CCEvent* event) {
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch) {
			prevTouchTime = elapsedTime;
			oldPos = pMap->getPosition();
			newPos = oldPos;
			touchOffset = ccpSub(oldPos,touchToPoint(touch));			
        }
    }
}

void World::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch && touchOffset.x && touchOffset.y) {
			oldPos = pMap->getPosition();
			newPos = ccpAdd(this->touchToPoint(touch), this->touchOffset);
			pMap->setPosition(newPos);			
        }
    }
}

void World::ccTouchesEnded(CCSet* touches, CCEvent* event) {
	float denom = (elapsedTime - prevTouchTime);
	denom = denom==0 ? 0.0001 : denom;
	float mag = ccpDistance(oldPos, newPos) / denom;
	CCLOG("Mag = %f", mag);
	CCMoveTo* move = CCMoveTo::create(1.0f, ccpMult(newPos, clampf(mag,0,30)));
	CCEaseOut* ease = CCEaseOut::create(move, 2);
	pMap->runAction(ease);


}