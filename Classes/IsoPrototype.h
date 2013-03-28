//
//  IsoPrototype.h
//  RotE
//
//  Created by Jarad Delorenzo on 3/28/13.
//
//

#ifndef __RotE__IsoPrototype__
#define __RotE__IsoPrototype__

#include "cocos2d.h"

class IsoPrototype : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(IsoPrototype);
};

#endif /* defined(__RotE__IsoPrototype__) */
