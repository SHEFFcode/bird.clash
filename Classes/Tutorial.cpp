#include "Tutorial.h"
#include "GamePlay.h"

USING_NS_CC;

Tutorial::~Tutorial(){
//    menu->release();
//    menu2->release();
//    menuSprite->release();
//    power1Text->release();
//    power2Text->release();
//    power3Text->release();
//    bird->release();
//    finger->release();
//    target->release();
//    arrow_left->release();
//    arrow_right->release();
//    playText->release();
//    instructions->release();
//    cherryScore->release();
//    redX3->release();
//    cherry->release();
//    cherryAnimation->release();
}

Scene* Tutorial::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Tutorial::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Tutorial::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create("background.png");
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    background->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background);
    
    // Done Button
    auto done = MenuItemImage::create("menu_purchase/done.png", "menu_purchase/done.png", CC_CALLBACK_1(Tutorial::GoToGamePlay, this));
    done->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.1));
    done->setScaleX(1.5);
    done->setScaleY(1.75);
    
    auto doneText = Label::createWithTTF( "Done", "Arial_Regular.ttf", visibleSize.height * 0.085);
    doneText->setColor(Color3B::WHITE);
    doneText->setPosition(Point(done->getPositionX(), done->getPositionY()));
    
    // Menu
    auto menuDone = Menu::create(done, NULL);
    menuDone->setPosition(Point::ZERO);
    this->addChild(menuDone,1);
    this->addChild(doneText,1);
    
    // Move and change PlayText to be the score
    playText = Label::createWithTTF("0", "Arial_Regular.ttf", visibleSize.height * 0.1);
    playText->setColor(Color3B::RED);
    playText->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.8));
    
    this->addChild(playText,1);
    
    // Add X's
    auto redX = Sprite::create("BlackX.png");
    auto xWidth = redX->getContentSize().width;
    auto adjust = (redX->getContentSize().height * 1.5) * 0.75;
    redX->setPosition(Point(visibleSize.width - adjust, visibleSize.height - adjust));
    redX->retain();
    redX->setScale(1.5);
    this->addChild(redX);
    
    auto redX2 = Sprite::create("BlackX.png");
    redX2->setPosition(Point( redX->getPositionX() - ((xWidth * 1.5) * 1.25), redX->getPositionY()));
    redX2->retain();
    redX2->setScale(1.5);
    this->addChild(redX2);
    
    redX3 = Sprite::create("BlackX.png");
    redX3->setPosition(Point( redX2->getPositionX() - ((xWidth * 1.5) * 1.25), redX2->getPositionY()));
    redX3->retain();
    redX3->setScale(1.5);
    this->addChild(redX3);
    
    auto cherrySprite = Sprite::create("cherry.png");
    cherrySprite->setPosition(Vec2((cherrySprite->getContentSize().width * 0.65), visibleSize.height - (cherrySprite->getContentSize().width * 0.65)));
    cherrySprite->setScale(0.85);
    this->addChild(cherrySprite);

    cherryScore = Label::createWithTTF("0", "Arial_Regular.ttf", visibleSize.height * 0.085);
    cherryScore->setColor(Color3B::RED);
    cherryScore->setPosition(Vec2(cherrySprite->getPositionX() + (cherrySprite->getContentSize().width * 0.75) + (cherryScore->getContentSize().width / 2), cherrySprite->getPositionY() - (cherryScore->getContentSize().height * 0.1)));
    
    this->addChild(cherryScore,1);
    
    menuSprite = Sprite::create("powerups/minus2.png");
    auto plusMinus = MenuItemSprite::create(menuSprite, menuSprite, CC_CALLBACK_1(Tutorial::None, this));
    plusMinus->setPosition(Vec2(plusMinus->getContentSize().width / 2, (plusMinus->getContentSize().height * 1.25) / 2));
    plusMinus->setScale(1.25);
    
    menu = Menu::create(plusMinus, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,1);
    
    auto bluebird = MenuItemImage::create("powerups/blue.png", "powerups/blue_selected.png", CC_CALLBACK_1(Tutorial::BlueBirds, this));
    bluebird->setScale(1.25);
    auto width = bluebird->getContentSize().width * 1.25;
    auto height = (bluebird->getContentSize().height * 1.25) / 2;
    bluebird->setPosition(Vec2(plusMinus->getPositionX() + width, height));;
    
    auto light = MenuItemImage::create("powerups/lightning.png", "powerups/lightning_selected.png", CC_CALLBACK_1(Tutorial::Lightning, this));
    light->setScale(1.25);
    light->setPosition(Vec2(bluebird->getPositionX() + width, height));
    
    auto grow = MenuItemImage::create("powerups/grow.png", "powerups/grow_selected.png", CC_CALLBACK_1(Tutorial::Grow, this));
    grow->setScale(1.25);
    grow->setPosition(Vec2(light->getPositionX() + width, height));
    
    
    auto wSet = (bluebird->getContentSize().width * 1.25) * 0.4;
    
    power1Text = Label::createWithTTF("1", "Arial_Regular.ttf", wSet);
    power1Text->setColor(Color3B::BLACK);
    auto textHeight = width + (((bluebird->getContentSize().height * 1.25) - width - power1Text->getContentSize().height / 2) / 2);
    power1Text->setPosition(Vec2(bluebird->getPositionX() + (power1Text->getContentSize().width * 0.15), textHeight));
    
    power2Text = Label::createWithTTF( "1", "Arial_Regular.ttf", wSet);
    power2Text->setColor(Color3B::BLACK);
    power2Text->setPosition(Point(power1Text->getPositionX() + width, textHeight));
    
    power3Text = Label::createWithTTF( "1", "Arial_Regular.ttf", wSet);
    power3Text->setColor(Color3B::BLACK);
    power3Text->setPosition(Point(power2Text->getPositionX() + width, textHeight));
    
    this->addChild(power1Text,2);
    this->addChild(power2Text,2);
    this->addChild(power3Text,2);
    
    menu2 = Menu::create(bluebird, light, grow, NULL);
    menu2->setPosition(Point::ZERO);
    this->addChild(menu2,1);
    
    menuShow = false;
    menuSprite->setTexture("powerups/plus2.png");
    menu2->setOpacity(0);
    menu2->setEnabled(false);
    power1Text->setOpacity(0);
    power2Text->setOpacity(0);
    power3Text->setOpacity(0);
    
    instructions = Label::createWithTTF("Tap birds to score points.", "Arial_Regular.ttf", visibleSize.height * 0.05);
    instructions->setColor(Color3B::BLACK);
    instructions->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    
    this->addChild(instructions,1);
    
    arrow_right = Sprite::create("arrow_right.png");
    arrow_right->setPosition(Vec2(instructions->getPositionX() - (instructions->getContentSize().width / 2) - (arrow_right->getContentSize().width * 0.75) , instructions->getPositionY()));
    this->addChild(arrow_right);
    
    arrow_left = Sprite::create("arrow_left.png");
    arrow_left->setPosition(Vec2(instructions->getPositionX() + (instructions->getContentSize().width / 2) + (arrow_left->getContentSize().width * 0.75) , instructions->getPositionY()));
    this->addChild(arrow_left);
    
    auto hide = FadeTo::create(1,0);
    auto hide2 = FadeTo::create(1,0);
    auto hide3 = FadeTo::create(1,0);
    auto scaleOut = ScaleTo::create(1.25,1.25);
    auto scaleOut2 = ScaleTo::create(1.25,1.25);
    auto scaleIn = ScaleTo::create(1.25,1);
    auto scaleIn2 = ScaleTo::create(1.25,1);
    
    Sequence *seq = Sequence::create(DelayTime::create(5), hide, NULL);
    Sequence *seq2 = Sequence::create(scaleOut, scaleIn, scaleOut, scaleIn, hide2, NULL);
    Sequence *seq3 = Sequence::create(scaleOut2, scaleIn2, scaleOut2, scaleIn2, hide3, NULL);
    instructions->runAction(seq);
    arrow_right->runAction(seq2);
    arrow_left->runAction(seq3);
    
    bird = Sprite::create("bird_right.png");
    bird->setPosition(Vec2(-bird->getContentSize().width / 2, visibleSize.height * 0.35));
    bird->setRotation(-35);
    bird->setOpacity(0);
    this->addChild(bird);
    
    auto show = FadeTo::create(0,255);
    auto move = MoveTo::create(2, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    
    Sequence *bird1_seq = Sequence::create(DelayTime::create(6), show, move, DelayTime::create(1.25), CallFunc::create(std::bind(&Tutorial::ChangeBird, this)), NULL);
    bird->runAction(bird1_seq);
    
    finger = Sprite::create("finger.png");
    finger->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.25));
    finger->setOpacity(0);
    finger->setRotation(-45);
    this->addChild(finger,3);
    
    target = Sprite::create("target.png");
    target->setPosition(Vec2(visibleSize.width / 2 + (finger->getContentSize().width * 0.125), visibleSize.height / 2 - (finger->getContentSize().height * 0.275)));
    target->setOpacity(0);
    this->addChild(target,2);
    
    auto show2 = FadeTo::create(0,255);
    auto move2 = MoveTo::create(1, Vec2(visibleSize.width / 2 + (finger->getContentSize().width / 2), visibleSize.height / 2 - (finger->getContentSize().height / 2)));
    
    Sequence *finger1_seq = Sequence::create(DelayTime::create(6), show2, DelayTime::create(1), move2, NULL);
    finger->runAction(finger1_seq);
    
    auto show3 = FadeTo::create(0,255);
    auto scaleOut3 = ScaleTo::create(1,1.5);
    Sequence *target1_seq = Sequence::create(DelayTime::create(8), show3, scaleOut3, NULL);
    target->runAction(target1_seq);
    
    return true;
}

void Tutorial::GoToGamePlay( cocos2d::Ref *sender )
{
    auto scene = GamePlay::createScene(0);
    Director::getInstance()->replaceScene(scene);
}

void Tutorial::HideShowMenu( cocos2d::Ref *sender )
{
    if(!menuShow){
        menuShow = 1;
        menuSprite->setTexture("powerups/minus2.png");
        menu2->setOpacity(255);
        menu2->setEnabled(true);
        power1Text->setOpacity(255);
        power2Text->setOpacity(255);
        power3Text->setOpacity(255);
    } else {
        menuShow = 0;
        menuSprite->setTexture("powerups/plus2.png");
        menu2->setOpacity(0);
        menu2->setEnabled(false);
        power1Text->setOpacity(0);
        power2Text->setOpacity(0);
        power3Text->setOpacity(0);
    }
}

void Tutorial::BlueBirds( cocos2d::Ref *sender )
{

}

void Tutorial::Lightning( cocos2d::Ref *sender )
{

}

void Tutorial::Grow( cocos2d::Ref *sender )
{

}

void Tutorial::ChangeBird(){
    bird->setTexture("bird_front.png");
    bird->setRotation(0);
    finger->setOpacity(0);
    target->setOpacity(0);
    playText->setString("1");
    arrow_left->setPosition(Vec2(playText->getPositionX() + (arrow_left->getContentSize().width), playText->getPositionY()));
    arrow_right->setPosition(Vec2(playText->getPositionX() - (arrow_right->getContentSize().width), playText->getPositionY()));
    
    auto show = FadeTo::create(0,255);
    auto show2 = FadeTo::create(0,255);
    auto show3 = FadeTo::create(0,255);
    auto scaleOut = ScaleTo::create(1.25,1.25);
    auto scaleOut2 = ScaleTo::create(1.25,1.25);
    auto scaleIn = ScaleTo::create(1.25,1);
    auto scaleIn2 = ScaleTo::create(1.25,1);
    auto hide3 = FadeTo::create(0,0);
    instructions->setString("Blue Birds are 2 points!");
    
    Sequence *seq = Sequence::create(DelayTime::create(0.5), show, scaleOut, scaleIn, scaleOut, scaleIn, scaleOut, scaleIn, NULL);
    Sequence *seq2 = Sequence::create(DelayTime::create(0.5), show2, scaleOut2, scaleIn2, scaleOut2, scaleIn2, scaleOut2, scaleIn2, scaleOut2, scaleIn2, NULL);
    Sequence *seq3 = Sequence::create(DelayTime::create(4.5), hide3, NULL);
    Sequence *seq4 = Sequence::create(DelayTime::create(4.5), show3, CallFunc::create(std::bind(&Tutorial::ArrowReset, this)), NULL);
    Sequence *seq5 = Sequence::create(DelayTime::create(7.5), CallFunc::create(std::bind(&Tutorial::CallBird, this)), NULL);
    arrow_left->runAction(seq);
    arrow_right->runAction(seq2);
    bird->runAction(seq3);
    instructions->runAction(seq4);
    this->runAction(seq5);
}

void Tutorial::ArrowReset(){
    arrow_right->setPosition(Vec2(instructions->getPositionX() - (instructions->getContentSize().width / 2) - (arrow_right->getContentSize().width * 0.75) , instructions->getPositionY()));
    arrow_right->setOpacity(255);
    
    arrow_left->setPosition(Vec2(instructions->getPositionX() + (instructions->getContentSize().width / 2) + (arrow_left->getContentSize().width * 0.75) , instructions->getPositionY()));
    arrow_left->setOpacity(255);
}

void Tutorial::CallBird(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    playText->setString("0");
    bird->setPosition(Vec2(-bird->getContentSize().width / 2, visibleSize.height * 0.35));
    bird->setOpacity(255);
    bird->setRotation(-35);
    bird->setTexture("blue_right.png");
    arrow_right->setOpacity(0);
    arrow_left->setOpacity(0);
    instructions->setOpacity(0);
    finger->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.25));
    target->setPosition(Vec2(visibleSize.width / 2 + (finger->getContentSize().width * 0.125), visibleSize.height / 2 - (finger->getContentSize().height * 0.275)));
    
    auto show2 = FadeTo::create(0,255);
    auto move2 = MoveTo::create(1, Vec2(visibleSize.width / 2 + (finger->getContentSize().width / 2), visibleSize.height / 2 - (finger->getContentSize().height / 2)));
    
    Sequence *finger1_seq = Sequence::create(show2, DelayTime::create(1), move2, NULL);
    finger->runAction(finger1_seq);
    
    auto show3 = FadeTo::create(0,255);
    target->setScale(1);
    auto scaleOut3 = ScaleTo::create(1,1.5);
    Sequence *target1_seq = Sequence::create(DelayTime::create(2), show3, scaleOut3, NULL);
    target->runAction(target1_seq);
    
    auto move = MoveTo::create(2, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    
    Sequence *bird1_seq = Sequence::create(move, DelayTime::create(1.25), CallFunc::create(std::bind(&Tutorial::ChangeBird2, this)), NULL);
    bird->runAction(bird1_seq);
}

void Tutorial::ChangeBird2(){
    bird->setTexture("blue_front.png");
    bird->setRotation(0);
    finger->setOpacity(0);
    target->setOpacity(0);
    playText->setString("2");
    arrow_left->setPosition(Vec2(playText->getPositionX() + (arrow_left->getContentSize().width), playText->getPositionY()));
    arrow_right->setPosition(Vec2(playText->getPositionX() - (arrow_right->getContentSize().width), playText->getPositionY()));
    
    auto show = FadeTo::create(0,255);
    auto show2 = FadeTo::create(0,255);
    auto scaleOut = ScaleTo::create(1.25,1.25);
    auto scaleOut2 = ScaleTo::create(1.25,1.25);
    auto scaleIn = ScaleTo::create(1.25,1);
    auto scaleIn2 = ScaleTo::create(1.25,1);
    auto hide3 = FadeTo::create(0,0);
    
    Sequence *seq = Sequence::create(DelayTime::create(0.5), show, scaleOut, scaleIn, scaleOut, scaleIn, scaleOut, scaleIn, NULL);
    Sequence *seq2 = Sequence::create(DelayTime::create(0.5), show2, scaleOut2, scaleIn2, scaleOut2, scaleIn2, scaleOut2, scaleIn2, scaleOut2, scaleIn2, NULL);
    Sequence *seq3 = Sequence::create(DelayTime::create(4.5), hide3, NULL);
    Sequence *seq4 = Sequence::create(DelayTime::create(4.5), CallFunc::create(std::bind(&Tutorial::Miss, this)), NULL);
    arrow_left->runAction(seq);
    arrow_right->runAction(seq2);
    bird->runAction(seq3);
    this->runAction(seq4);
}

void Tutorial::Miss()
{
    playText->setString("0");
    bird->setTexture("bird_right.png");
    bird->setRotation(0);
    bird->setRotation(-35);

    instructions->setString("Miss 3 birds and Game Over!");
    instructions->setOpacity(255);
    this->ArrowReset();
    
    
    auto scaleOut = ScaleTo::create(1.25,1.25);
    auto scaleOut2 = ScaleTo::create(1.25,1.25);
    auto scaleIn = ScaleTo::create(1.25,1);
    auto scaleIn2 = ScaleTo::create(1.25,1);
    
    Sequence *seq = Sequence::create(scaleOut, scaleIn, scaleOut, scaleIn, NULL);
    Sequence *seq2 = Sequence::create(scaleOut2, scaleIn2, scaleOut2, scaleIn2, NULL);
    Sequence *seq3 = Sequence::create(DelayTime::create(5), CallFunc::create(std::bind(&Tutorial::MissBird, this)), NULL);
    
    arrow_left->runAction(seq);
    arrow_right->runAction(seq2);
    
    this->runAction(seq3);
}

void Tutorial::MissBird(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    bird->setOpacity(255);
    instructions->setOpacity(0);
    arrow_left->setOpacity(0);
    arrow_right->setOpacity(0);
    bird->setPosition(Vec2(-bird->getContentSize().width / 2, visibleSize.height * 0.35));
    auto move = MoveTo::create(4, Vec2(visibleSize.width, visibleSize.height * 0.75));
    
    Sequence *bird1_seq = Sequence::create(move, CallFunc::create(std::bind(&Tutorial::Missed, this)), NULL);
    bird->runAction(bird1_seq);
}

void Tutorial::Missed(){
    bird->setOpacity(0);
    redX3->setTexture("RedX.png");
    redX3->setScale(2.5);
    redX3->setOpacity(255);
    arrow_right->setPosition(Vec2(redX3->getPositionX() - arrow_right->getContentSize().width, redX3->getPositionY() - arrow_right->getContentSize().height));
    arrow_right->setRotation(-33);
    arrow_right->setOpacity(255);
    auto scale = ScaleTo::create(0.5, 1.5);
    auto scaleOut = ScaleTo::create(1.25,1.25);
    auto scaleIn = ScaleTo::create(1.25,1);
    Sequence *seq = Sequence::create(scaleOut, scaleIn, scaleOut, scaleIn, NULL);
    arrow_right->runAction(seq);
    redX3->runAction(scale);
    
    Sequence *seq2 = Sequence::create(DelayTime::create(5), CallFunc::create(std::bind(&Tutorial::Cherries, this)), NULL);
    this->runAction(seq2);
}

void Tutorial::Cherries(){
    arrow_right->setOpacity(0);
    arrow_right->setRotation(0);
    redX3->setTexture("BlackX.png");
    instructions->setString("Collect Cherries to buy Powerups!");
    instructions->setOpacity(255);
    this->ArrowReset();
    
    auto scaleOut = ScaleTo::create(1.25,1.25);
    auto scaleOut2 = ScaleTo::create(1.25,1.25);
    auto scaleIn = ScaleTo::create(1.25,1);
    auto scaleIn2 = ScaleTo::create(1.25,1);
    
    Sequence *seq = Sequence::create(scaleOut, scaleIn, scaleOut, scaleIn, NULL);
    Sequence *seq2 = Sequence::create(scaleOut2, scaleIn2, scaleOut2, scaleIn2, NULL);
    
    arrow_left->runAction(seq);
    arrow_right->runAction(seq2);
    
    Sequence *seq3 = Sequence::create(DelayTime::create(5), CallFunc::create(std::bind(&Tutorial::GetCherry, this)), NULL);
    this->runAction(seq3);
}

void Tutorial::GetCherry(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    instructions->setOpacity(0);
    arrow_right->setOpacity(0);
    arrow_left->setOpacity(0);
    cherry = Sprite::create("cherry.png");
    cherry->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(cherry, 1);
    finger->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.25));
    target->setPosition(Vec2(visibleSize.width / 2 + (finger->getContentSize().width * 0.125), visibleSize.height / 2 - (finger->getContentSize().height * 0.275)));
    
    auto show2 = FadeTo::create(0,255);
    auto move2 = MoveTo::create(1, Vec2(visibleSize.width / 2 + (finger->getContentSize().width / 2), visibleSize.height / 2 - (finger->getContentSize().height / 2)));
    
    Sequence *finger1_seq = Sequence::create(show2, DelayTime::create(1), move2, NULL);
    finger->runAction(finger1_seq);
    
    auto show3 = FadeTo::create(0,255);
    target->setScale(1);
    auto scaleOut3 = ScaleTo::create(1,1.5);
    Sequence *target1_seq = Sequence::create(DelayTime::create(2), show3, scaleOut3, NULL);
    target->runAction(target1_seq);
    
    Sequence *seq = Sequence::create(DelayTime::create(3), CallFunc::create(std::bind(&Tutorial::CollectCherry, this)), NULL);
    this->runAction(seq);
}
                             
void Tutorial::CollectCherry(){
    finger->setOpacity(0);
    target->setOpacity(0);
    
    cherryAnimation = Sprite::create("cherry_animation.png");
    cherryAnimation->setPosition(Vec2(cherry->getPositionX(), cherry->getPositionY()));
    cherryAnimation->setScale(0.25);
    this->addChild(cherryAnimation,1);
    this->removeChild(cherry);
    
    auto animation = ScaleTo::create(1, 0.5);
    Sequence *seq = Sequence::create(animation, CallFunc::create(std::bind(&Tutorial::CherryCollected, this)), NULL);
    cherryAnimation->runAction(seq);
}

void Tutorial::CherryCollected(){
    this->removeChild(cherryAnimation);
    cherryScore->setString("1");
    arrow_left->setPosition(Vec2(cherryScore->getPositionX() + arrow_left->getContentSize().width, cherryScore->getPositionY() - arrow_left->getContentSize().height));
    arrow_left->setRotation(33);
    arrow_left->setOpacity(255);
    
    auto scaleOut = ScaleTo::create(1.25,1.25);
    auto scaleIn = ScaleTo::create(1.25,1);
    Sequence *seq = Sequence::create(scaleOut, scaleIn, scaleOut, scaleIn, NULL);
    arrow_left->runAction(seq);
    
    Sequence *seq2 = Sequence::create(DelayTime::create(5), CallFunc::create(std::bind(&Tutorial::Powerups, this)), NULL);
    this->runAction(seq2);
}

void Tutorial::Powerups(){
    arrow_left->setRotation(0);
    instructions->setString("Powerups help you win!");
    instructions->setOpacity(255);
    this->ArrowReset();
    
    auto scaleOut = ScaleTo::create(1.25,1.25);
    auto scaleOut2 = ScaleTo::create(1.25,1.25);
    auto scaleIn = ScaleTo::create(1.25,1);
    auto scaleIn2 = ScaleTo::create(1.25,1);
    
    Sequence *seq = Sequence::create(scaleOut, scaleIn, scaleOut, scaleIn, NULL);
    Sequence *seq2 = Sequence::create(scaleOut2, scaleIn2, scaleOut2, scaleIn2, NULL);
    
    arrow_left->runAction(seq);
    arrow_right->runAction(seq2);
    
    Sequence *seq3 = Sequence::create(DelayTime::create(5), CallFunc::create(std::bind(&Tutorial::PowerupsShow, this)), NULL);
    this->runAction(seq3);
}

void Tutorial::PowerupsShow(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    instructions->setOpacity(0);
    arrow_right->setOpacity(0);
    arrow_left->setOpacity(0);
    finger->setOpacity(255);
    finger->setRotation(-135);
    finger->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto move = MoveTo::create(2, Vec2(menuSprite->getPositionX() + (finger->getContentSize().width / 2) + (finger->getContentSize().width * 0.125), menuSprite->getPositionY() + (finger->getContentSize().height / 2) + (finger->getContentSize().height * 0.275)));
    target->setPosition(Vec2(menuSprite->getPositionX() + (menuSprite->getContentSize().width / 2), menuSprite->getPositionY() + menuSprite->getContentSize().height / 2));
    
    Sequence *finger1_seq = Sequence::create(move, NULL);
    finger->runAction(finger1_seq);
    
    auto show = FadeTo::create(0,255);
    target->setScale(1);
    auto scaleOut = ScaleTo::create(1,1.5);
    Sequence *target1_seq = Sequence::create(DelayTime::create(2), show, scaleOut, NULL);
    target->runAction(target1_seq);
 
    Sequence *seq2 = Sequence::create(DelayTime::create(3), CallFunc::create(std::bind(&Tutorial::Finish, this)), NULL);
    this->runAction(seq2);
}

void Tutorial::Finish(){
    // Opens the powerups menu
    this->HideShowMenu(this);
    target->setOpacity(0);
    finger->setOpacity(0);
    
    // show the left arrow pointing
    arrow_left->setOpacity(255);
    arrow_left->setRotation(-65);
    arrow_left->setPosition(Vec2(power2Text->getPositionX(), power2Text->getPositionY() + (power2Text->getContentSize().width) + arrow_left->getContentSize().height));

    auto scaleOut = ScaleTo::create(1.25,1.25);
    auto scaleIn = ScaleTo::create(1.25,1);
    auto hide = FadeTo::create(0,0);

    Sequence *seq = Sequence::create(scaleOut, scaleIn, scaleOut, scaleIn, hide, NULL);
    arrow_left->runAction(seq);
    
    // Show Text Tutorial Complete
    instructions->setString("Tutorial Complete!");
    instructions->setScale(1.5);
    
    auto show = FadeTo::create(0,255);
    
    Sequence *seq2 = Sequence::create(DelayTime::create(5), show, DelayTime::create(2), CallFunc::create(std::bind(&Tutorial::FinishRedirect, this)), NULL);
    instructions->runAction(seq2);
}

void Tutorial::FinishRedirect(){
    auto scene = GamePlay::createScene(0);
    Director::getInstance()->replaceScene(scene);
}

void Tutorial::None(cocos2d::Ref *sender){
    
}














