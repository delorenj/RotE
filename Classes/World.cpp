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
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();    
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
#if DEBUG_MAP_INIT
    CCLOG("(%f,%f)", origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
#endif
    pMap = CCTMXTiledMap::create(TMX_FILE);
	pMap->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));    
    pMap->setAnchorPoint(ccp(0.5f, 0.5f));
    zoomFactor = 1;
    addChild(pMap, 1);
    b2Vec2 grav = b2Vec2(0, 0);
    pWorld = new b2World(grav);
    b2BodyDef mapBodyDef;
    mapBodyDef.position.Set(pMap->getPositionX()/PTM_RATIO, pMap->getPositionY()/PTM_RATIO);
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
    m_bMapMoved = 0;
    
    m_pTestSprite = CCSprite::create("dog.png");
    m_pMapLayer = pMap->layerNamed("ground");
    
    CCArray * pChildrenArray = pMap->getChildren();
    CCSpriteBatchNode* child = NULL;
    CCObject* pObject = NULL;
    
    //Set layers to antialias mode
    CCARRAY_FOREACH(pChildrenArray, pObject)
    {
        child = (CCSpriteBatchNode*)pObject;
        if(!child)
            break;
        child->getTexture()->setAntiAliasTexParameters();
    }

    pMap->addChild(m_pTestSprite);
        
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

CCPoint World::positionForTile(CCPoint tileCoord) {
    CCSprite* tile = m_pMapLayer->tileAt(tileCoord);
	CCPoint pos = tile->getPosition();
    pos = tile->convertToNodeSpace(pos);
#if DEBUG_MAP
	CCLOG("Tile (%f, %f): (%f, %f) : MAP= (%f, %f)", tileCoord.x, tileCoord.y, pos.x, pos.y, pMap->getPositionX(), pMap->getPositionY());
#endif
	return pos;
}

CCPoint World::tileForPosition(CCPoint p) {
	int x = (int)(p.x / pMap->getTileSize().width);
	int y = (int)(((pMap->getMapSize().height * pMap->getTileSize().width) - p.y) / pMap->getTileSize().width);
#if DEBUG_MAP
	CCLOG("getTileSize()   : (%f, %f)", pMap->getTileSize().width, pMap->getTileSize().height);
	CCLOG("getContentSize(): (%f, %f)", pMap->getContentSize().width, pMap->getContentSize().height);
	CCLOG("point           : (%f, %f)", p.x, p.y);
	CCLOG("tile           : (%d, %d)", x, y);
#endif
	return ccp(x, y);
}

CCPoint World::touchToTile(CCTouch* touch) {
    CCPoint pos = touchToPoint(touch);
    pos = pMap->convertToNodeSpace(pos);
    
    float halfMapWidth = pMap->getMapSize().width * 0.5f;
    float mapHeight = pMap->getMapSize().height;
    float pointWidth = pMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR();
    float pointHeight = pMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
    
    CCPoint tilePosDiv = CCPointMake(pos.x/pointWidth, pos.y/pointHeight);
    float inverseTileY = mapHeight - tilePosDiv.y;
    
    float posX = (int)(inverseTileY+tilePosDiv.x - halfMapWidth);
    float posY = (int)(inverseTileY-tilePosDiv.x + halfMapWidth);
    
    posX = MAX(0, posX);
    posX = MIN(pMap->getMapSize().width-1, posX);
    posY = MAX(0, posY);
    posY = MIN(pMap->getMapSize().height-1, posY);
    
    return CCPointMake(posX, posY);
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

void World::updateTestSprite() {
    
};

void World::ccTouchesBegan(CCSet *touches, CCEvent* event) {
//    if(touchMap.count() >= MAX_TOUCHES) return;
    
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch) {
            touchMap[touch->getID()] = touch;
#if DEBUG_MAP_TOUCH
            CCLOG("Touch Started: %d, id=%d", touchMap.size(), touch->getID());
#endif
            if(touchMap.size() == 1) {
                b2Vec2 pos = pBody->GetPosition();
                touchOffset = ccpSub(ccp(pos.x*PTM_RATIO, pos.y*PTM_RATIO),touchToPoint(touch));
            } else if(touchMap.size() == 2) {
                touchMap[touch->getID()] = touch;
                CCPoint p1 = touchToPoint(touchMap[0]);
                CCPoint p2 = touchToPoint(touchMap[1]);
                initialTouchDistance = ccpDistance(p1, p2);
                initialZoomFactor = zoomFactor;
                lastTouchDistance = initialTouchDistance;
            }            
        }
    }
}

void World::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    for(auto it = touches->begin(); it != touches->end(); it++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touch && touchOffset.x && touchOffset.y) {
			m_bMapMoved = true;
            if(!touchMap[0]) return; //if released fingers in reverse order, return
            if(touch == touchMap[0]) {   //if finger on screen is not the movey finger, return
                lastPos = pBody->GetPosition();
                CCPoint newpos = ccpAdd(touchToPoint(touch), touchOffset);
                pBody->SetTransform(b2Vec2(newpos.x/PTM_RATIO, newpos.y/PTM_RATIO), pBody->GetAngle());
            }
            
            if(touchMap.size() == 2) {
                CCPoint p1 = touchToPoint(touchMap[0]);
                CCPoint p2 = touchToPoint(touchMap[1]);
                float currentTouchDistance = ccpDistance(p1, p2);
                zoomFactor = -(initialTouchDistance - currentTouchDistance)*0.015f + initialZoomFactor;
                if(zoomFactor < 0.5f) {
                    zoomFactor = 0.5f;
                }
                if(zoomFactor > 3.0f) {
                    zoomFactor = 3.0f;
                }
            }

        }
    }
}

void World::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    int i=0;
    for(auto it = touches->begin(); it != touches->end(); it++, i++) {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        if(touchMap.size() >= 2 && touch->getID() == 0) {
            touchMap.clear();
            return;
        }
        touchMap.erase(touch->getID());
#if DEBUG_MAP_TOUCH
        CCLOG("Touches: %d, id=%d", touchMap.size(), touch->getID());
#endif      		
        if(touch && touchOffset.x && touchOffset.y && m_bMapMoved) {
            b2Vec2 linearVelocity = b2Vec2(lastPos - pBody->GetPosition());
            linearVelocity *= linearVelocity.Normalize() * -1 * 15;
            pBody->SetLinearVelocity(linearVelocity);
        }
        
        if(touch && !m_bMapMoved) {
#pragma mark select_tile
            CCPoint touchPosInTileSpace = touchToTile(touch);
            CCSprite* tile = m_pMapLayer->tileAt(touchPosInTileSpace);
            m_pTestSprite->setPosition(tile->getPosition());
#if DEBUG_MAP

            CCPoint tilePosWorld = pMap->convertToWorldSpace(tile->getPosition());
            
            CCLOG("------- PMAP DEBUG INFO -------");
            CCLOG("pMap position: (%f, %f)", pMap->getPositionX(), pMap->getPositionY());
            CCLOG("pMap scale: %f", pMap->getScaleX());
            CCLOG("num tiles: (%f, %f)", pMap->getMapSize().width, pMap->getMapSize().height);
            CCLOG("tile size: (%f, %f)", pMap->getTileSize().width, pMap->getTileSize().height);
            CCLOG("pMap size: (%f, %f)", pMap->getTileSize().width * pMap->getMapSize().width, pMap->getTileSize().height * pMap->getMapSize().height);
            CCLOG("tile(0,0) local pos: (%f, %f)", tile->getPositionX(), tile->getPositionY());
            CCLOG("tile(0,0): (%f, %f)", tilePosWorld.x, tilePosWorld.y);
            CCLOG("touch pos: (%f, %f)", touchPosInTileSpace.x, touchPosInTileSpace.y);
#endif
        }
    }
	m_bMapMoved = false;
}
