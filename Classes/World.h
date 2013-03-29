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

#define TMX_FILE "isometric_grass_and_water.tmx"

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
    
protected:
    CCTMXTiledMap* pMap;
    CCPoint touchOffset;
    
    
};
#endif /* defined(__RotE__World__) */
