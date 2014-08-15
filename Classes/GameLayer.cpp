#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"

USING_NS_CC;
using namespace CocosDenshion;
GameLayer::~GameLayer() {
    delete _players;
}

Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    _player1Score = 0;
    _player2Score = 0;
    _screenSize = Director::getInstance()->getWinSize();
    
    Sprite * court = Sprite::create("court.png");
    court->setPosition(Point(_screenSize.width*0.5, _screenSize.height*0.5));
    this->addChild(court);
    
    _player1 = GameSprite::gameSpriteWithFile("mallet.png");
    _player1->setPosition(Point(_screenSize.width*0.5, _player1->radius()*2));
    this->addChild(_player1);
    
    _player2 = GameSprite::gameSpriteWithFile("mallet.png");
    _player2->setPosition(Point(_screenSize.width*0.5, _screenSize.height - _player1->radius()*2));
    this->addChild(_player2);
    
    _ball = GameSprite::gameSpriteWithFile("puck.png");
    _ball->setPosition(Point(_screenSize.width*0.5, _screenSize.height*0.5 - 2*_ball->radius()));
    this->addChild(_ball);

    _players = new Vector<GameSprite*>();
    _players->pushBack(_player1);
    _players->pushBack(_player2);

    _player1ScoreLabel = LabelTTF::create("0", "Arial", 60);
    _player1ScoreLabel->setPosition(Point(_screenSize.width-60, _screenSize.height*0.5-80));
    _player1ScoreLabel->setRotation(90);
    this->addChild(_player1ScoreLabel);
    
    _player2ScoreLabel = LabelTTF::create("0", "Arial", 60);
    _player2ScoreLabel->setPosition(Point(_screenSize.width-60, _screenSize.height*0.5+80));
    _player2ScoreLabel->setRotation(90);
    this->addChild(_player2ScoreLabel);
    
    _logo = Sprite::create("logo.png");
    _logo->setPosition(_screenSize.width*0.5, _screenSize.height*0.5);
    this->addChild(_logo);
    
    //this->schedule(schedule_selector(GameLayer::update));
    
    return true;
}

void GameLayer::onEnter() {
    Layer::onEnter();
    
    //register touch event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    
    listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameLayer::onExit() {
    Layer::onExit();
}

void GameLayer::update(float dt) {
    Point ballNextPosition = _ball->getNextPosition();
    Point ballVector = _ball->getVector();
    ballVector = ballVector * 0.98;
    ballNextPosition += ballVector;
    
    GameSprite * player;
    Point playerNextPosition;
    Point playerVector;
    
    float squared_radii = pow(_player1->radius() + _ball->radius(), 2);
    for (int p = 0; p < 2; p++) {
        player = (GameSprite *) _players->at(p);
        playerNextPosition = player->getNextPosition();
        playerVector = player->getVector();
        float diffx = ballNextPosition.x - player->getPositionX();
        float diffy = ballNextPosition.y - player->getPositionY();
        float distance1 = pow(diffx, 2) + pow(diffy, 2);
        float distance2 = pow(_ball->getPositionX() - playerNextPosition.x, 2) +
            pow(_ball->getPositionY() - playerNextPosition.y, 2);
        
        if(distance1 <= squared_radii || distance2 <= squared_radii) {
            float mag_ball = pow(ballVector.x, 2) + pow(ballVector.y, 2);
            float mag_player = pow(playerVector.x, 2) + pow(playerVector.y, 2);
            float force = sqrt(mag_ball + mag_player);
            float angle = atan2(diffy, diffx);
            ballVector.x = force * cos(angle);
            ballVector.y = force * sin(angle);
            ballNextPosition.x = playerNextPosition.x + (player->radius() + _ball->radius() + force) * cos(angle);
            ballNextPosition.y = playerNextPosition.y + (player->radius() + _ball->radius() + force) * sin(angle);
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
        
        
        if (ballNextPosition.x < _ball->radius()) {
            ballNextPosition.x = _ball->radius();
            ballVector.x *= -0.8f;
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
        if (ballNextPosition.x > _screenSize.width - _ball->radius()) {
            ballNextPosition.x = _screenSize.width - _ball->radius();
            ballVector.x *= -0.8f;
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
        
        if (ballNextPosition.y > _screenSize.height - _ball->radius()) {
            if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f ||
                _ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
                ballNextPosition.y = _screenSize.height - _ball->radius();
                ballVector.y *= -0.8f;
                SimpleAudioEngine::getInstance()->playEffect("hit.wav");
            }
        }
        if (ballNextPosition.y < _ball->radius() ) {
            if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f ||
                _ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
                ballNextPosition.y = _ball->radius();
                ballVector.y *= -0.8f;
                SimpleAudioEngine::getInstance()->playEffect("hit.wav");
            }
        }
        
        _ball->setVector(ballVector);
        _ball->setNextPosition(ballNextPosition);
        //check for goals!
        if (ballNextPosition.y  < -_ball->radius() * 2) {
            this->playerScore(2);
        }
        if (ballNextPosition.y > _screenSize.height + _ball->radius() * 2)
        {
            this->playerScore(1);
        }
        
        _player1->setPosition(_player1->getNextPosition());
        _player2->setPosition(_player2->getNextPosition());
        _ball->setPosition(_ball->getNextPosition());
    }
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* event) {
    if(_logo != NULL) {
        return; //logo is showing
    }
    
    std::vector<Touch*> ts = touches;
    Touch* touch;
    Point tap;
    GameSprite* player;
    
    for(std::vector<Touch*>::iterator it = ts.begin(); it != ts.end(); ++it) {
        touch = *it;
        if(!touch) continue;
        
        tap = touch->getLocation();
        for(int p=0; p<2; p++) {
            player = (GameSprite*) _players->at(p);
            if(player->getBoundingBox().containsPoint(tap)) {
                player->setTouch(touch);
            }
        }

    }
}

void GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event) {
    if(_logo != NULL) {
        return; //logo is showing
    }
    
    std::vector<Touch*> ts = touches;
    Touch* touch;
    Point tap;
    GameSprite* player;
    
    for(std::vector<Touch*>::iterator it = ts.begin(); it != ts.end(); ++it) {
        touch = *it;
        if(!touch) continue;
        
        tap = touch->getLocation();
        for(int p=0; p<2; p++) {
            player = (GameSprite *) _players->at(p);
            
            if(player->getTouch() == NULL || player->getTouch() != touch) continue;
            Point nextPosition = tap;
            
            //keep player inside screen
            if(nextPosition.x < player->radius()) {
                nextPosition.x = player->radius();
            }
            if(nextPosition.x > _screenSize.width - player->radius()) {
                nextPosition.x = _screenSize.width - player->radius();
            }
            if(nextPosition.y < player->radius()) {
                nextPosition.y = player->radius();
            }
            if(nextPosition.y > _screenSize.height - player->radius()) {
                nextPosition.y = _screenSize.height - player->radius();
            }
            
            //keep player inside the court
            if(player->getPositionY() < _screenSize.height*0.5) {
                if(nextPosition.y > _screenSize.height*0.5 - player->radius()) {
                    nextPosition.y = _screenSize.height*0.5 - player->radius();
                }
            }
            else {
                if(nextPosition.y < _screenSize.height * 0.5 + player->radius()) {
                    nextPosition.y = _screenSize.height*0.5 + player->radius();
                }
            }
            
            player->setNextPosition(nextPosition);
            player->setVector(Point(tap.x - player->getPositionX(), tap.y - player->getPositionY()));
            
        }
        
    }

    
    
    
    
}

void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event* event) {
    if(_logo != NULL) {
        _logo->removeFromParent();
        _logo = NULL;
        this->schedule(schedule_selector(GameLayer::update));
        return;
    }
    
    std::vector<Touch*> ts = touches;
    Touch* touch;
    Point tap;
    GameSprite* player;
    
    for(std::vector<Touch*>::iterator it = ts.begin(); it != ts.end(); ++it) {
        touch = *it;
        if(!touch) continue;
        
        tap = touch->getLocation();
        for (int p = 0; p < 2; p++) {
            player = (GameSprite *) _players->at(p);
            if (player->getTouch() == NULL || player->getTouch() != touch) continue;
            player->setTouch(NULL);
            player->setVector(Point(0,0));
        }
    }
}


void GameLayer::playerScore(int player) {
    SimpleAudioEngine::getInstance()->playEffect("score.wav");
    _ball->setVector(Point(0,0));
    
    char score_buffer[10];
    if (player == 1) {
        _player1Score++;
        sprintf(score_buffer,"%i", _player1Score);
        _player1ScoreLabel->setString(score_buffer);
        _ball->setNextPosition(Point(_screenSize.width * 0.5,
                                   _screenSize.height * 0.5 + 2 * _ball->radius()));
    } else {
        _player2Score++;
        sprintf(score_buffer,"%i", _player2Score);
        _player2ScoreLabel->setString(score_buffer);
        _ball->setNextPosition(Point(_screenSize.width * 0.5,
                                   _screenSize.height * 0.5 - 2 * _ball->radius()));
    }
    
    _player1->setPosition(Point(_screenSize.width * 0.5, _player1->radius() * 2));
    _player2->setPosition(Point(_screenSize.width * 0.5, _screenSize.height - _player1->radius() * 2));
    _player1->setTouch(NULL);
    _player2->setTouch(NULL);
}