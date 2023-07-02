#pragma once
#include "cocos2d.h"
#define string std::string

using namespace cocos2d;
using namespace std;

class EnemyShip : public Sprite {
public:
    friend class GameScene;

    static EnemyShip* Create(EnemyShip*, cocos2d::Scene* scene, cocos2d::Vec2 position);

    EnemyShip(char* _spriteName, int _health, float _scale)
    {
        spriteName = _spriteName;
        health = _health;
        scale = _scale;
    }
    void Update(cocos2d::Vec2 moveDir, float moveCount);
    void Init(cocos2d::Scene *scene, cocos2d::Vec2 position);
    void TakeDamage(int dmg);
private:
    Sprite *shipSprite;
    Vec2 startPos;

    char *spriteName;
    int health;
    float scale;
};

class EnemyShipA : public EnemyShip {
public:
    EnemyShipA() :EnemyShip("enemyA.png", 50, .05f) {}
};

class EnemyShipB : public EnemyShip{
public:
    EnemyShipB() :EnemyShip("enemyB.png", 100, .1f) {}
};
