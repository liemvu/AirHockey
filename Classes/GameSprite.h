//
//  GameSprite.h
//  AirHockey
//
//  Created by William Vo on 4/14/14.
//
//

#ifndef __AirHockey__GameSprite__
#define __AirHockey__GameSprite__

#include "cocos2d.h"

using namespace cocos2d;

class GameSprite : public Sprite {
public:
    CC_SYNTHESIZE(Point, _nextPosition, NextPosition);
    CC_SYNTHESIZE(Point, _vector, Vector);
    CC_SYNTHESIZE(Touch *, _touch, Touch);
    
    GameSprite();
    ~GameSprite();
    
    static GameSprite* gameSpriteWithFile(const char * pszFileName);
    
    virtual void setPosition(const Point& pos);
    inline float radius() {
        return getTexture()->getContentSize().width*0.5f;
    }
};

#endif /* defined(__AirHockey__GameSprite__) */
