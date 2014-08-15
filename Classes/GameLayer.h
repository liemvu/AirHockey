#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameSprite.h"


using namespace cocos2d;

class GameLayer : public cocos2d::Layer
{
public:
    ~GameLayer();
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameLayer);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    GameSprite * _player1;
    GameSprite * _player2;
    GameSprite * _ball;
    Sprite * _logo;
    
    Vector<GameSprite*> * _players;
    LabelTTF * _player1ScoreLabel;
    LabelTTF * _player2ScoreLabel;
    
    Size _screenSize;
    
    int _player1Score;
    int _player2Score;
    
    void playerScore(int player);
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    void update(float dt);
};

#endif // __HELLOWORLD_SCENE_H__
