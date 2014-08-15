//
//  GameSprite.cpp
//  AirHockey
//
//  Created by William Vo on 4/14/14.
//
//

#include "GameSprite.h"

GameSprite::GameSprite() {
    _vector = Point(0,0);
}

GameSprite::~GameSprite() {
}

GameSprite* GameSprite::gameSpriteWithFile(const char * pszFileName) {
    GameSprite * sprite = new GameSprite();
    if(sprite && sprite->initWithFile(pszFileName)) {
        sprite->autorelease();
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void GameSprite::setPosition(const Point& pos) {
    CCSprite::setPosition(pos);
    if(!_nextPosition.equals(pos)) {
        _nextPosition = pos;
    }
}