#include "MyContactListener.h"
#include "Definitions.h"

USING_NS_CC;

void MyContactListener::BeginContact(b2Contact* contact)
{
    UserDefault *def = UserDefault::getInstance();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    int visibleWidth = visibleSize.width;
    int visibleHeight = visibleSize. height;
    int halfVisibleWidth = visibleSize.width / 2;
    int halfVisibleHeight = visibleSize.height / 2;
    float positionX, positionY;
    long whichBird;
    
    b2Body* bird;
    b2Body* border;
    b2Fixture* a = contact->GetFixtureA();
    b2Fixture* b = contact->GetFixtureB();
    
    //make sure only one of the fixtures was a sensor
    bool sensorA = a->IsSensor();
    bool sensorB = b->IsSensor();
    if ( ! (sensorA && sensorB) )
    {
        if(a->IsSensor()){
            bird = a->GetBody();
            border = b->GetBody();
            whichBird = (long)a->GetUserData();
        } else {
            border = a->GetBody();
            bird = b->GetBody();
            whichBird = (long)b->GetUserData();
        }
        Sprite* birdSprite = (Sprite*)bird->GetUserData();
        float birdX = birdSprite->getPositionX();
        float birdY = birdSprite->getPositionY();
        auto birdTag = birdSprite->getTag();
        long borderCheck = (long)border->GetUserData();
        
        auto blue = def->getIntegerForKey("blue", 0);
        auto velocity = def->getFloatForKey("velocity", 0);
        
        auto rotation = rand() % 46 + 1;
        
        bool top = false;
        
        if(borderCheck == 1 || borderCheck == 3)  // bottom and right
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
        
        // Add missed bird and stop redirection if 3 collisions. GameOver if 3 misses in total
        if(((birdTag == 1 || birdTag == 4) && (borderCheck == 1 || borderCheck == 3)) || ((birdTag == 2 || birdTag == 8) && (borderCheck == 2 || borderCheck == 3)) || ((birdTag == 3 || birdTag == 5) && (borderCheck == 1 || borderCheck == 4)) || ((birdTag == 6 || birdTag == 7) && (borderCheck == 2 || borderCheck == 4)))
        {
            if(whichBird == 1)
            {
                int bird1_collisions = def->getIntegerForKey("bird1_collisions", 0);
                bird1_collisions++;
                def->setIntegerForKey("bird1_collisions", bird1_collisions);
                if(bird1_collisions >= 3) {
                    this->Missed();
                    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(birdSprite);
                    return;
                }
            }
            if(whichBird == 2)
            {
                int bird2_collisions = def->getIntegerForKey("bird2_collisions", 0);
                bird2_collisions++;
                def->setIntegerForKey("bird2_collisions", bird2_collisions);
                if(bird2_collisions >= 3) {
                    this->Missed();
                    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(birdSprite);
                    return;
                }
            }
            if(whichBird == 3)
            {
                int bird3_collisions = def->getIntegerForKey("bird3_collisions", 0);
                bird3_collisions++;
                def->setIntegerForKey("bird3_collisions", bird3_collisions);
                if(bird3_collisions >= 3) {
                    this->Missed();
                    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(birdSprite);
                    return;
                }
            }
        }
        
        // Redirect Bird
        if((birdTag == 1 || birdTag == 4) && (borderCheck == 1 || borderCheck == 3))  // LeftTop || TopLeft -> bottom and right
        {
            if(borderCheck == 1)  // Bottom
            {
                if( birdX > halfVisibleWidth )
                {
                    if(whichBird != blue) {
                        birdSprite->setTexture("bird_left.png");
                    } else {
                        birdSprite->setTexture("blue_left.png");
                    }
                    birdSprite->setTag(7);
                    birdSprite->setRotation(rotation);
                    positionX = -(birdX + xPos);
                    positionY = birdY + yPos;
                } else {
                    if(whichBird != blue) {
                        birdSprite->setTexture("bird_right.png");
                    } else {
                        birdSprite->setTexture("blue_right.png");
                    }
                    birdSprite->setTag(8);
                    birdSprite->setRotation(-rotation);
                    positionX = birdX + xPos;
                    positionY = birdY + yPos;
                }
            } else {  // Right
                if(whichBird != blue) {
                    birdSprite->setTexture("bird_left.png");
                } else {
                    birdSprite->setTexture("blue_left.png");
                }
                if( birdY > halfVisibleHeight )
                {
                    birdSprite->setTag(5);
                    birdSprite->setRotation(-rotation);
                    positionX = -(birdX + xPos);
                    positionY = -(birdY + yPos);
                } else {
                    birdSprite->setTag(6);
                    birdSprite->setRotation(rotation);
                    positionX = -(birdX + xPos);
                    positionY = birdY + yPos;
                }
            }
        }
        if((birdTag == 2 || birdTag == 8) && (borderCheck == 2 || borderCheck == 3))  // LeftBottom || BottomLeft -> top and right
        {
            if(borderCheck == 2)  // Top
            {
                if( birdX > halfVisibleWidth )
                {
                    if(whichBird != blue) {
                        birdSprite->setTexture("bird_left.png");
                    } else {
                        birdSprite->setTexture("blue_left.png");
                    }
                    birdSprite->setTag(3);
                    birdSprite->setRotation(-rotation);
                    positionX = -(birdX + xPos);
                    positionY = -(birdY + yPos);
                } else {
                    if(whichBird != blue) {
                        birdSprite->setTexture("bird_right.png");
                    } else {
                        birdSprite->setTexture("blue_right.png");
                    }
                    birdSprite->setTag(4);
                    birdSprite->setRotation(rotation);
                    positionX = birdX + xPos;
                    positionY = -(birdY + yPos);
                }
            } else {  // Right
                if(whichBird != blue) {
                    birdSprite->setTexture("bird_left.png");
                } else {
                    birdSprite->setTexture("blue_left.png");
                }
                if( birdY > halfVisibleHeight )
                {
                    birdSprite->setTag(5);
                    birdSprite->setRotation(-rotation);
                    positionX = -(birdX + xPos);
                    positionY = -(birdY + yPos);
                } else {
                    birdSprite->setTag(6);
                    birdSprite->setRotation(rotation);
                    positionX = -(birdX + xPos);
                    positionY = birdY + yPos;
                }
            }
        }
        if((birdTag == 3 || birdTag == 5) && (borderCheck == 1 || borderCheck == 4))  // TopRight || RightTop -> bottom and left
        {
            if(borderCheck == 1)  // Bottom
            {
                if( birdX > halfVisibleWidth )
                {
                    if(whichBird != blue) {
                        birdSprite->setTexture("bird_left.png");
                    } else {
                        birdSprite->setTexture("blue_left.png");
                    }
                    birdSprite->setTag(7);
                    birdSprite->setRotation(rotation);
                    positionX = -(birdX + xPos);
                    positionY = birdY + yPos;
                } else {
                    if(whichBird != blue) {
                        birdSprite->setTexture("bird_right.png");
                    } else {
                        birdSprite->setTexture("blue_right.png");
                    }
                    birdSprite->setTag(8);
                    birdSprite->setRotation(-rotation);
                    positionX = birdX + xPos;
                    positionY = birdY + yPos;
                }
            } else {  // Left
                if(whichBird != blue) {
                    birdSprite->setTexture("bird_right.png");
                } else {
                    birdSprite->setTexture("blue_right.png");
                }
                if( birdY > halfVisibleHeight )
                {
                    birdSprite->setTag(1);
                    birdSprite->setRotation(rotation);
                    positionX = birdX + xPos;
                    positionY = -(birdY + yPos);
                } else {
                    birdSprite->setTag(2);
                    birdSprite->setRotation(-rotation);
                    positionX = birdX + xPos;
                    positionY = birdY + yPos;
                }
            }
        }
        if((birdTag == 6 || birdTag == 7) && (borderCheck == 2 || borderCheck == 4))  // RightBottom || BottomRight -> top and left
        {
            if(borderCheck == 2)  // Top
            {
                if( birdX > halfVisibleWidth )
                {
                    if(whichBird != blue) {
                        birdSprite->setTexture("bird_left.png");
                    } else {
                        birdSprite->setTexture("blue_left.png");
                    }
                    birdSprite->setTag(3);
                    birdSprite->setRotation(-rotation);
                    positionX = -(birdX + xPos);
                    positionY = -(birdY + yPos);
                } else {
                    if(whichBird != blue) {
                        birdSprite->setTexture("bird_right.png");
                    } else {
                        birdSprite->setTexture("blue_right.png");
                    }
                    birdSprite->setTag(4);
                    birdSprite->setRotation(rotation);
                    positionX = birdX + xPos;
                    positionY = -(birdY + yPos);
                }
            } else {  // Left
                if(whichBird != blue) {
                    birdSprite->setTexture("bird_right.png");
                } else {
                    birdSprite->setTexture("blue_right.png");
                }
                if( birdY > halfVisibleHeight )
                {
                    birdSprite->setTag(1);
                    birdSprite->setRotation(rotation);
                    positionX = birdX + xPos;
                    positionY = -(birdY + yPos);
                } else {
                    birdSprite->setTag(2);
                    birdSprite->setRotation(-rotation);
                    positionX = birdX + xPos;
                    positionY = birdY + yPos;
                }
            }
        }
        
        if(((birdTag == 1 || birdTag == 4) && (borderCheck == 1 || borderCheck == 3)) || ((birdTag == 2 || birdTag == 8) && (borderCheck == 2 || borderCheck == 3)) || ((birdTag == 3 || birdTag == 5) && (borderCheck == 1 || borderCheck == 4)) || ((birdTag == 6 || birdTag == 7) && (borderCheck == 2 || borderCheck == 4)))
        {
            bird->SetLinearVelocity( b2Vec2(positionX * velocity / PTM_RATIO, positionY * velocity / PTM_RATIO));
        }
        
        def->flush();
        
        contact->SetEnabled(false);
    }
}

void MyContactListener::EndContact(b2Contact* contact)
{
     contact->SetEnabled(true);
}

void MyContactListener::PreSolve(b2Contact* contact,
                                 const b2Manifold* oldManifold)
{
}

void MyContactListener::PostSolve(b2Contact* contact,
                                  const b2ContactImpulse* impulse)
{
}

void MyContactListener::Missed()
{
    UserDefault *def = UserDefault::getInstance();
    auto missed_birds = def->getIntegerForKey("missed_birds", 0);
    auto bird_count = def->getIntegerForKey("bird_count", 0);
    bird_count++;
    missed_birds++;
    def->setIntegerForKey("missed_birds", missed_birds);
    def->setIntegerForKey("bird_count", bird_count);
    def->flush();
}















