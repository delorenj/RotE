//
//  IsoPrototype.cpp
//  RotE
//
//  Created by Jarad Delorenzo on 3/28/13.
//
//

#include "IsoPrototype.h"
#include "AppMacros.h"
#include "World.h"

USING_NS_CC;

#define WORLD 1

CCScene* IsoPrototype::scene()
{
    CCScene *scene = CCScene::create();
    IsoPrototype *layer = IsoPrototype::create();
    scene->addChild(layer);
    return scene;
}


bool IsoPrototype::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
//    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
//    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLayerColor* color = new CCLayerColor();
    color->initWithColor(ccc4(64,64,64,255));
    addChild(color, -1);
    
    World* pWorld = new World();
    addChild(pWorld, 0, WORLD);
    
    return true;
}


void IsoPrototype::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
