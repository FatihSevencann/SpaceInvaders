#pragma once
#include "cocos2d.h"
#include "EnemyShip.h"
#include "Bullet.h"
#include "GameOverScene.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(GameScene);
private:

    std::vector<EnemyShip*> enemyShips;
    std::vector<Bullet*> bullets;

    int lastBulletIndex;
    bool isTouching;
    int direction;
    int enemyMoveDirection = 1;
    int killCounter, streakCounter;
    bool streak;
    float streakCountdown = 3;

    bool moveDown;
    float enemyMoveTimeDelta = 1 / 2.0f;
    float bulletFireRate = 1 / 3.0f;
    int bulletDamage = 40;

    Label* scoreText;
    Size visibleSize;
    Sprite* spaceShip;

    bool onMouseUp(cocos2d::EventMouse* mouse);
    void onMouseMoved(cocos2d::EventMouse* mouse);
    bool onMouseDown(cocos2d::EventMouse* mouse);
    bool onContactBegin(cocos2d::PhysicsContact& contact);


    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };
    cocos2d::PhysicsWorld* sceneWorld;

    void update(float delta) override;
    void updatePosition(float delta);
    void fireBullet(float delta);



    void handleBulletCollision(cocos2d::PhysicsBody* bulletBody, cocos2d::PhysicsBody* enemyBody);
    void handleEdgeCollision(cocos2d::PhysicsBody* bulletBody, cocos2d::PhysicsBody* edgeBody);
    void handleEnemyShipCollision(cocos2d::PhysicsBody* enemyBody, cocos2d::PhysicsBody* edgeBody);

    void setupPhysics();
    void setupGraphics();
    void setupListeners();
    void setupSchedules();
    void createEnemyShips();
    void createBullets();


};
