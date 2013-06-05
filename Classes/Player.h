//
//  Player.h
//  RotE
//
//  Created by Jarad Delorenzo on 6/4/13.
//
//

#ifndef __RotE__Player__
#define __RotE__Player__

#include <iostream>
#include <cocos2d.h>

using namespace cocos2d;

class Player : public CCNode {
public:
    Player();
    ~Player();
    
	virtual void update(float dt);
    
    int getLevel() { return m_level; }
    void addToScore(int points) { m_score += points; }
    int getScore() { return m_score; }
    CCString getName() { return m_name; }
    void setName(CCString name) { m_name = name; }

protected:
    int m_level;
    int m_score;
    CCString m_name;
    
    void levelUp() { m_level++; }
    
};

#endif /* defined(__RotE__Player__) */
