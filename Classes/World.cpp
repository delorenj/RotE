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

	scrollAction = CCFollow::create(pViewport, worldBoundary);
	runAction(scrollAction);	
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

void World::ccTouchesBegan(CCSet *touches, CCEvent* event) {

    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch) {
			CCMoveTo* move = CCMoveTo::create(1.0f, touchToPoint(touch));
			CCEaseOut* ease = CCEaseOut::create(move, 2);
			pViewport->runAction(ease);
        }
    }
}

void World::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch && touchOffset.x && touchOffset.y) {
        }
    }
}

void World::ccTouchesEnded(CCSet* touches, CCEvent* event) {
	CCLOG("Viewport Position: (%f, %f)", pViewport->getPositionX(), pViewport->getPositionY());
}