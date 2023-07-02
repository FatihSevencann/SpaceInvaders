#pragma once
#include "cocos2d.h"
#include "EnemyShip.h"
#include "Bullet.h"
#include "GameOverScene.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    std::vector<EnemyShip*> enemyShips;

    std::vector<Bullet*> bullets;

    int lastBulletIndex;

    bool isTouching;
    int direction;
    int enemyMoveDirection=1;
    int killCounter, streakCounter;
    bool streak;
    float streakCountdown = 3;

    bool moveDown;

    float enemyMoveTimeDelta = 1 / 3.0f;
    float bulletFireRate = 1 / 5.0f;

    int bulletDamage = 25;

    cocos2d::Label* scoreText;

    cocos2d::Size visibleSize;

    cocos2d::Sprite* spaceShip;

    bool onMouseUp(cocos2d::EventMouse* mouse);
    void onMouseMoved(cocos2d::EventMouse* mouse);
    bool onMouseDown(cocos2d::EventMouse* mouse);

    bool onContactBegin(cocos2d::PhysicsContact& contact);

    virtual bool init();
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };
    cocos2d::PhysicsWorld* sceneWorld;

    void update(float) override;
    void updatePosition(float);
    void fireBullet(float);

    CREATE_FUNC(GameScene);
};
