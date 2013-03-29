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
    pMap = new CCTMXTiledMap();
    pMap->initWithTMXFile(TMX_FILE);
    addChild(pMap, 1);

}

World::~World() {
    removeChild(pMap);
    if(pMap) {
        delete pMap;
    }
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
    this->touchOffset = CCPointZero;

    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch) {
            this->touchOffset = ccpSub(pMap->getPosition(),touchToPoint(touch));
        }
    }
}

void World::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch && touchOffset.x && touchOffset.y) {
			pMap->setPosition(ccpAdd(this->touchToPoint(touch), this->touchOffset));
            
        }
    }
}

void World::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    CCLOG("Nope!");
}