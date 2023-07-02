#pragma once
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class Bullet:public Sprite
{
    friend class GameScene;
public:
    static Bullet* createBullet(cocos2d::Vec2 startPosition, float scale, cocos2d::Scene* scene);
    void Init(Vec2, float, cocos2d::Scene*);

private:
    Sprite* bulletSprite;
    Vec2 startPos;
    int id;
    float scale;
    bool activated;
    Action *action;
    void Move(Vec2);
    void Deactivate(Vec2);
};