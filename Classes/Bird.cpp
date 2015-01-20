#include "Bird.h"
#include "Definitions.h"
#include "Math.h"
#include "external/Box2d/Box2d.h"
#include "GamePlay.h"

USING_NS_CC;

Bird::~Bird(){
//    bird->release();
}

Bird::Bird( cocos2d::Layer *layer, b2World *world )
{
    visibleSize = Director::getInstance()->getVisibleSize();
    int visibleWidth = visibleSize.width;
    int visibleHeight = visibleSize.height;
    float positionX, positionY;
    long whichBird;
    
    auto region = rand() % 4 + 1;
    
    // Adjust random region based on previous regions
    UserDefault *def = UserDefault::getInstance();
    auto velocity = def->getFloatForKey("velocity", 0);
    auto power3_activated = def->getIntegerForKey("power3_activated",0);
    auto bird1_region = def->getIntegerForKey("bird1_region", 0);
    auto bird2_region = def->getIntegerForKey("bird2_region", 0);
    auto blue1 = def->getIntegerForKey("blue1");
    auto blue2 = def->getIntegerForKey("blue2");
    auto blue3 = def->getIntegerForKey("blue3");
    
    if(bird1_region == 0) {
        def->setIntegerForKey("bird1_region", region);
        whichBird = 1;
    } else {
        if(bird2_region == 0){
            while(region == bird1_region){
                 region = rand() % 4 + 1;
            }
            def->setIntegerForKey("bird2_region", region);
            whichBird = 2;
        } else {
            while(region == bird1_region || region == bird2_region){
                region = rand() % 4 + 1;
            }
            whichBird = 3;
        }
    }
    
    if((whichBird == 1 && !blue1) || (whichBird == 2 && !blue2) || (whichBird == 3 && !blue3)) {
        bird = Sprite::create("bird_left.png");
    } else {
        bird = Sprite::create("blue_left.png");
    }
    
    def->flush();
    
    auto rotation = rand() % 46 + 1;
    
    int halfBird = bird->getContentSize().width / 2;
    auto birdY = rand() % (visibleHeight - halfBird) + halfBird;
    auto birdX = rand() % (visibleWidth - halfBird) + halfBird;
    bool top = false;
    
    if(region == 1 || region == 3)
    {
        if(birdY > visibleHeight / 2){
            top = true;
            rotation = rand() % 26 + 1;
        }
    } else {
        rotation = rotation + 45;
        if(birdX > visibleWidth / 2)
            top = true;
    }

    auto angle = rotation * M_PI / 180;
    auto xPos = cos(angle) * (visibleWidth * 1.5);
    auto yPos = sin(angle) * (visibleWidth * 1.5);
    
    // Set the bird's position based on region
    if(region == 1)  // Left
    {
        bird->setPosition(Point(-halfBird,birdY));
        if((whichBird == 1 && !blue1) || (whichBird == 2 && !blue2) || (whichBird == 3 && !blue3)) {
            bird->setTexture("bird_right.png");
        } else {
            bird->setTexture("blue_right.png");
        }
        if(top)  // Above middle of screen
        {
            bird->setRotation(rotation);
            bird->setTag(1);
            positionX = bird->getPositionX() + xPos;
            positionY = -(bird->getPositionY() + yPos);
        }
        else   // Below middle of screen
        {
            bird->setRotation(-rotation);
            bird->setTag(2);
            positionX = bird->getPositionX() + xPos;
            positionY = bird->getPositionY() + yPos;
        }
    }
    if(region == 2) // Top
    {
        bird->setPosition(Point(birdX, visibleHeight + halfBird));
        if(top)  // Past middle of screen on x axis
        {
            bird->setRotation(-rotation);
            bird->setTag(3);
            positionX = -(bird->getPositionX() + xPos);
            positionY = -(bird->getPositionY() + yPos);
        }
        else   // Before middle of screen on x axis
        {
            bird->setTag(4);
            if((whichBird == 1 && !blue1) || (whichBird == 2 && !blue2) || (whichBird == 3 && !blue3)) {
                bird->setTexture("bird_right.png");
            } else {
                bird->setTexture("blue_right.png");
            }
            bird->setRotation(rotation);
            positionX = bird->getPositionX() + xPos;
            positionY = -(bird->getPositionY() + yPos);
        }
    }
    if(region == 3)  // Right
    {
        bird->setPosition(Point(visibleWidth + halfBird, birdY));
        if(top)  // Above middle of screen
        {
            bird->setRotation(-rotation);
            bird->setTag(5);
            positionX = -(bird->getPositionX() + xPos);
            positionY = -(bird->getPositionY() + yPos);
        }
        else   // Below middle of screen
        {
            bird->setRotation(rotation);
            bird->setTag(6);
            positionX = -(bird->getPositionX() + xPos);
            positionY = bird->getPositionY() + yPos;
        }
    }
    if(region == 4)  // Bottom
    {
        bird->setPosition(Point(birdX, -halfBird));
        if(top)  // Past middle of screen on x axis
        {
            bird->setRotation(rotation);
            bird->setTag(7);
            positionX = -(bird->getPositionX() + xPos);
            positionY = bird->getPositionY() + yPos;
        }
        else   // Before middle of screen on x axis
        {
            if((whichBird == 1 && !blue1) || (whichBird == 2 && !blue2) || (whichBird == 3 && !blue3)) {
                bird->setTexture("bird_right.png");
            } else {
                bird->setTexture("blue_right.png");
            }
            bird->setRotation(-rotation);
            bird->setTag(8);
            positionX = bird->getPositionX() + xPos;
            positionY = bird->getPositionY() + yPos;
        }
    }
    // Create ball body
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.userData = bird;
    ballBodyDef.position.Set(bird->getPositionX() / PTM_RATIO,bird->getPositionY() / PTM_RATIO);
    b2Body *ballBody = world->CreateBody(&ballBodyDef);
    
    // Create circle shape
    b2CircleShape circle;
    circle.m_radius = (bird->getContentSize().height / 2) / PTM_RATIO;
    
    // Create shape definition and add body
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.f;
    ballShapeDef.restitution = 0.f;
    ballShapeDef.isSensor = true;
    ballShapeDef.filter.categoryBits = BIRD_COLLISION_BITMASK;
    ballShapeDef.filter.maskBits = BIRD_COLLISION_BITMASK;
    ballShapeDef.userData = (void*) whichBird;
    ballBody->SetLinearVelocity( b2Vec2(positionX * velocity / PTM_RATIO, positionY * velocity / PTM_RATIO));
    ballBody->CreateFixture(&ballShapeDef);
    
    if(!power3_activated){
        auto tmpScale = bird->getScale();
        //tmpScale = tmpScale / 1.5;
        bird->setScale(tmpScale);
    } else {
        bird->setScale(1.5);
    }
    
    layer->addChild(bird, 100);
    
    //setup touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = CC_CALLBACK_2( Bird::onTouchBegan, this );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, bird);
}

bool Bird::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event )
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode))
    {
        target->setTag(0);
        return true;
    }
    return false;
}

