#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
    auto layer = GameScene::create();
    layer->SetPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if ( !Scene::initWithPhysics() )
    {
        return false;
    }
    this->getPhysicsWorld()->setFixedUpdateRate(1);
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    this->setContentSize(Size(1280, 720));

    setupPhysics();
    setupGraphics();
    setupListeners();
    setupSchedules();
    createEnemyShips();
    createBullets();

    return true;

}

#pragma region Setups
void GameScene::setupPhysics()
{
    auto side = cocos2d::PhysicsBody::createEdgeBox(this->getContentSize());
    side->setCollisionBitmask(0x000003);
    side->setContactTestBitmask(true);
    this->setPhysicsBody(side);
}

void GameScene::setupGraphics()
{
    Director::getInstance()->getOpenGLView()->setFrameSize(1280, 720);
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1280, 720, ResolutionPolicy::EXACT_FIT);
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->setContentSize(Size(1280, 720));

    scoreText = Label::create();
    scoreText->setString("0");
    scoreText->setSystemFontSize(30);
    scoreText->setPosition(Vec2(visibleSize.width - 45, visibleSize.height - 30));
    this->addChild(scoreText);

    spaceShip = Sprite::create("spaceship.png");
    spaceShip->setPosition(Vec2(visibleSize.width / 2, 0));
    spaceShip->setScale(visibleSize.width / 5000, visibleSize.height / 5000);
    this->addChild(spaceShip, 2);
}

void GameScene::setupListeners()
{
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseUp = CC_CALLBACK_1(GameScene::onMouseUp, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMoved, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameScene::setupSchedules()
{
    this->scheduleUpdate();
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updatePosition), enemyMoveTimeDelta);
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::fireBullet), bulletFireRate);
}

void GameScene::createEnemyShips()
{
    for (int i = 0; i < 16; i++)
    {
        auto a = EnemyShip::Create(new EnemyShipA(), this, Vec2(75 + (i) * 60, visibleSize.height - 75));
        this->addChild(a);
        enemyShips.push_back(a);
    }

    for (int i = 0; i < 16; i++)
    {
        auto a = EnemyShip::Create(new EnemyShipB(), this, Vec2(75 + (i) * 60, visibleSize.height - 25));
        this->addChild(a);
        enemyShips.push_back(a);
    }
}

void GameScene::createBullets()
{
    for (size_t i = 0; i < 2; i++)
    {
        bullets.push_back(Bullet::createBullet(spaceShip->getPosition() + Vec2::ANCHOR_TOP_LEFT * -60, 0.05f, this));
        this->addChild(bullets.back());
    }

    lastBulletIndex = 4;
}
#pragma endregion

#pragma region Update

void GameScene::update(float delta)
{
    if (streak) {
        streakCountdown -= delta;
        if (streakCountdown <= 0) {
            spaceShip->setColor(cocos2d::Color3B::WHITE);

            streak = false;
            streakCountdown = 3;
            bulletDamage = 25;
        }
    }

    if (isTouching)
    {
        float newPosX = clampf(spaceShip->getPosition().x + direction * 3, 30, visibleSize.width-30);
        spaceShip->setPositionX(newPosX);

    }
}
void GameScene::updatePosition(float delta) {

    for  (EnemyShip * var : enemyShips)
    {
        var->Update(moveDown ? -Vec2::ANCHOR_TOP_LEFT * 100 : -Vec2::ANCHOR_BOTTOM_RIGHT * 10 * enemyMoveDirection, 15);
    }
    moveDown = false;
}

#pragma endregion
void GameScene::fireBullet(float delta)
{
    if (bullets.empty())
    {
        auto bullet = Bullet::createBullet(spaceShip->getPosition() + Vec2::ANCHOR_TOP_LEFT * -200, 0.05f, this);
        bullets.push_back(bullet);
        this->addChild(bullet);
    }

    auto lastBullet = bullets.back();
    if (!lastBullet->activated && lastBullet->getParent() == nullptr) {
        this->addChild(lastBullet);
    }

    if (streak) {
        lastBullet->bulletSprite->setColor(Color3B::YELLOW);
    } else if (lastBullet->bulletSprite->getColor() != Color3B::WHITE) {
        lastBullet->bulletSprite->setColor(Color3B::WHITE);
    }

    lastBullet->Move(spaceShip->getPosition());
    bullets.pop_back();
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody *body1 = contact.getShapeA()->getBody();
    PhysicsBody *body2 = contact.getShapeB()->getBody();

    PhysicsBody* bulletBody;
    PhysicsBody* enemyBody;


    bool bulletCollisionConditionWithEnemy = body1->getCollisionBitmask() == 0x000001 &&
                                             body2->getCollisionBitmask() == 0x000002;

    bool bulletCollisionConditionWithEnemy2 = body2->getCollisionBitmask() == 0x000001 &&
                                              body1->getCollisionBitmask() == 0x000002;



    if (bulletCollisionConditionWithEnemy || bulletCollisionConditionWithEnemy2)
    {
        if (bulletCollisionConditionWithEnemy)
        {
            enemyBody = body2;
            bulletBody = body1;
        }
        else {
            enemyBody = body1;
            bulletBody = body2;
        }

        auto a = (Bullet*)bulletBody->getNode();
        bullets.push_back(a);
        a->Deactivate(spaceShip->getPosition() + Vec2::ANCHOR_TOP_LEFT * -200);
        removeChild(a);

        auto b = (EnemyShip*)enemyBody->getNode();

        if(b != nullptr)
            b->TakeDamage(bulletDamage);

        if (b != nullptr && b->health <= 0)
        {

            enemyShips.erase(std::remove(enemyShips.begin(), enemyShips.end(), b));
            this->removeChild(b);

            if (enemyShips.size() == 0)
            {
                auto newScene = GameScene::createScene();
                _director->replaceScene(newScene);
            }


            killCounter++;
            scoreText->setString(std::to_string(killCounter));

            if (!streak) {
                streakCounter++;
            }


            if (!streak && streakCounter % 5 == 0)
            {
                spaceShip->setColor(cocos2d::Color3B::ORANGE);
                streakCounter = 0;
                bulletDamage = 100;
                streak = true;
            }
        }

    }

    bool bulletCollisionConditionWithEdge = body1->getCollisionBitmask() == 0x000003 &&
                                            body2->getCollisionBitmask() == 0x000001;
    bool bulletCollisionConditionWithEdge2 = body1->getCollisionBitmask() == 0x000001 &&
                                             body2->getCollisionBitmask() == 0x000003;

    if (bulletCollisionConditionWithEdge || bulletCollisionConditionWithEdge2)
    {
        if (bulletCollisionConditionWithEdge) {
            bulletBody = body2;
        }
        else {
            bulletBody = body1;

        }
        auto a = (Bullet*)bulletBody->getNode();
        bullets.push_back(a);
        a->Deactivate(spaceShip->getPosition() + Vec2::ANCHOR_TOP_LEFT * -200);
        removeChild(a);
    }

    bool enemyShipCollisionConditionWithEdge = body1->getCollisionBitmask() == 0x000003 &&
                                               body2->getCollisionBitmask() == 0x000002;

    if (enemyShipCollisionConditionWithEdge)
    {
        moveDown = true;
        enemyMoveDirection *= -1;

        if (enemyShips[0]->getPositionY() < 0)
        {
            auto newScene = GameOver::createScene();
            _director->replaceScene(newScene);
        }
    }
    return true;
}

#pragma region Mouse Actions
bool GameScene::onMouseUp(cocos2d::EventMouse* mouse)
{
    isTouching = false;
    return 1;
}

void GameScene::onMouseMoved(cocos2d::EventMouse* mouse)
{
    direction = mouse->getCursorX() > visibleSize.width / 2 ? 1 : -1;
}
bool GameScene::onMouseDown(cocos2d::EventMouse* mouse)
{
    isTouching = true;
    return 1;
}
#pragma endregion
