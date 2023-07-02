#include "EnemyShip.h"
#include "cocos2d.h"

EnemyShip* EnemyShip::Create(EnemyShip* enemyShip, cocos2d::Scene* scene, cocos2d::Vec2 position) {

    auto ship = enemyShip;
    if (ship && ship->init()) {
        ship->startPos = position;

        ship->autorelease();
        ship->shipSprite = Sprite::create(enemyShip->spriteName);

        ship->addChild(ship->shipSprite);
        ship->setPosition(position);
        ship->setScale(enemyShip->scale, enemyShip->scale);
        ship->Init(scene, position);
        return ship;
    }
    CC_SAFE_RELEASE(ship);
    return nullptr;
}

void EnemyShip::Init(cocos2d::Scene* scene, cocos2d::Vec2 position) {

    auto enemyBody = cocos2d::PhysicsBody::createBox(shipSprite->getContentSize());

    enemyBody->setCollisionBitmask(0x000002);
    enemyBody->setContactTestBitmask(true);
    enemyBody->setDynamic(false);
    enemyBody->setRotationEnable(false);
    enemyBody->setGravityEnable(false);

    this->setPhysicsBody(enemyBody);

}

void EnemyShip::TakeDamage(int dmg) {
    health -= dmg;
    health = cocos2d::clampf(health, 0, 100);

}

void EnemyShip::Update(cocos2d::Vec2 moveDir ,float moveCount)
{
    this->setPosition(this->getPosition() + moveDir);
}