#include "Hero.h"
#include "globals.h"




Hero::Hero(float sf)
{
    cameraOffset = 0;
    using namespace std;
    using namespace cocos2d;
    leftCounter = 0;
    rightCounter = 0;
    setupAnimation();
    boostPower = 5;
    idString = "player";
    // SET UP SPRITE
    //player_sprite = cocos2d::Sprite::create("bellafigur.png");
	//player_sprite = cocos2d::Sprite::create("pink_sqr.png");
    player_sprite->setPositionX(256);
	  player_sprite->setPositionY(256);
    player_sprite->setScale(sf);
    player_sprite->setAnchorPoint(Point(0.5f,0));
    
    // INIT POS
    velocity = Point(0, 0);
    //position = Point(256,256);
    hitBoxWidth = 30;
    hitBoxHeight = 63;
    vsp = 0;
    
    // SET UP TOUCH
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(Hero::onTouchBegan, this);
    touchListener->onTouchesEnded = CC_CALLBACK_2(Hero::onTouchEnded, this);
    touchListener->onTouchesMoved = CC_CALLBACK_2(Hero::onTouchMoved, this);
    touchListener->onTouchesCancelled = CC_CALLBACK_2(Hero::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    touch_timer = -1;
    
    // add a label shows "Hello World"
    // create and initialize a label
    
   
    hp_label = Label::createWithTTF("FUEL: 100 %", "fonts/Marker Felt.ttf", 24);
    hp_label->setTextColor(colors[int((0))]);
    // position the label on the center of the screen
    this->addChild(hp_label,6);

    ////////////////////////////
    // ADD UPDATE
    this->schedule(CC_SCHEDULE_SELECTOR(Hero::updateAnimationInterrupt));
    
    ////////////////////////////
    // OBJECT SETTINGS
    maxWalkSpeed = 220;
    maxFlySpeed = 270;
    groundAcceleration = 3;
    groundDeaccleration = 0.65;
    airAcceleration = 1.4 ;
    airDeacceleration = 0.998;
    grounded = false;
    dash_accelaration = 650;
    cool_down_timer = 0;
    cool_down_timer_max = 0.6f;
     dash_timer_max = 0.5f;
    dash_max =0.9f;
    
    // fule
    fule_max = 100;
    fule = fule_max;
    fule_consume_rate = 10;
    
    // Double Touch
    double_touch = 0.5f;
    // DASH
    dash_timer = 0;
   
    
    // landed
    landed = false;
    toucing_exit = false;
    
    
    //////////////////////////////
    // JETPACK BUTTONS
    button_left = cocos2d::ui::Button::create("jetpack_logo0.png", "pink_sqr.png", "jetpack_logo1.png");
    button_left->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                cocos2d::log("BUTTON PRESSED");
                if(jetPack){
                    jetPack = false;
                    button_left->setBright(true);
                }
                else{
                    jetPack = true;
                    button_left->setBright(false);
                }
                break;
            default:
                break;
        }
    });
    this->addChild(button_left,6);
    
    button_right = cocos2d::ui::Button::create("jetpack_logo0.png", "pink_sqr.png", "jetpack_logo1.png");
    button_right->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                cocos2d::log("BUTTON PRESSED");
                if(jetPack){
                    jetPack = false;
                    button_right->setBright(true);
                }
                else{
                    jetPack = true;
                    button_right->setBright(false);
                }
                break;
            default:
                break;
        }
    });
    
    imune = false;
    this->addChild(button_right,6);
    player_sprite->setPosition(0.5f,0);
    player_sprite->setScale(3);
    player_sprite->getTexture()->setAliasTexParameters();
    
    //setAnchorPoint(Point(0.5f,0));
    //drawHitbox();
    addHitboxToSprite();
}

void Hero::updateGameObject(float delta) {
    if(on_platform){
        jump = false;
        grounded = true;
    }
    if(HP>=0&&!guided_misile){
        updateTimers(delta);
        if(left_dash<=0&&right_dash<=0){
            check_for_gravity = true;
            addStdMovement(delta);
            addJetPackMovement(delta);
        }
        else{
            check_for_gravity = false;
            addDashMovement(delta);
        }
    }
    else if(HP==0){
        hsp = 0;
        vsp = 0;
    }
    // GUI
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    button_left->setPosition(this->convertToNodeSpace(Point(0.05*winSize.width,0.8*winSize.height)));
    button_right->setPosition(this->convertToNodeSpace(Point(0.95*winSize.width,0.8*winSize.height)));
    // Uppdate ViewPoint
    if(vsp<-maxFlySpeed&&cameraOffset>-55){
        cameraOffset-=(0.8);
    }else if(vsp>maxFlySpeed/8&&cameraOffset<75){
        cameraOffset+=(0.5);
    }
    
    /*if(guided_misile){
        if(misile->idString=="remove"){
            misile->removeObj = true;
            guided_misile = false;
        }
        if(left&&!right){
            misile->steerLeft();
        }
        if(right&&!left){
            misile->steerRight();
        }
    }*/
}

void Hero::setupAnimation(){
    
    
    spriteFrameCache = spriteFrameCache->getInstance();
    animationCache = animationCache->getInstance();
    //spriteFrameCache->addSpriteFramesWithFile("space_explorer2.plist");
    //Hero::addAnimation("SpaceExplorer64 ","IdleR",1,1);
    //Hero::addAnimation("SpaceExplorer64 ","IdleL",2,2);
    //Hero::addAnimation("SpaceExplorer64 ","WalkR",3,6);
    //Hero::addAnimation("SpaceExplorer64 ","WalkL",7,10);
    //Hero::addAnimation("SpaceExplorer64 ","flyR",11,11);
    //Hero::addAnimation("SpaceExplorer64 ","flyL",12,12);
    //Hero::addAnimation("SpaceExplorer64 ","fallR",13,13);
    //Hero::addAnimation("SpaceExplorer64 ","fallL",14,14);
    //Hero::addAnimation("SpaceExplorer64 ","dashR",15,15,0.2f);
    //Hero::addAnimation("SpaceExplorer64 ","dashL",16,16,0.2f);
    //Hero::addAnimation("SpaceExplorer64 ","dieR",17,23,0.2f);
    //Hero::addAnimation("SpaceExplorer64 ","dieL",24,30,0.2f);
    
    spriteFrameCache->addSpriteFramesWithFile("PTSH_new_colors.plist");
    Hero::addAnimation("PTSHR","IdleR",1,4,0.2f);
    Hero::addAnimation("PTSHR","IdleL",1,4,0.2f);
    Hero::addAnimation("PTSHR","WalkR",5,8,0.15f);
    Hero::addAnimation("PTSHR","WalkL",5,8,0.15f);
    Hero::addAnimation("PTSHR","flyR",9,12,0.2f);
    Hero::addAnimation("PTSHR","flyL",9,12,0.2f);
    Hero::addAnimation("PTSHR","fallR",13,16,0.2f);
    Hero::addAnimation("PTSHR","fallL",13,16,0.2f);
    Hero::addAnimation("PTSHR","dashR",17,20,0.15f);
    Hero::addAnimation("PTSHR","dashL",17,20,0.15f);
    Hero::addAnimation("PTSHR","dieR",21,24,0.15f);
    Hero::addAnimation("PTSHR","dieL",21,24,0.15f);
    
    E_LastAnimation = RIGHT;
    E_NextAnimation = RIGHT;
    player_sprite = cocos2d::Sprite::create();
    player_sprite->runAction(RepeatForever::create(Animate::create(animationCache->getAnimation("WalkR"))));
    direction = RIGHT;
    
    //this->addChild(player_sprite,5);
}



void Hero::updateAnimation(int lastAnimation,int newAnimation){
    

}

void Hero::updateAnimationInterrupt(float delta){
    player_sprite->cocos2d::Node::setPosition((int)getPositionX(), (int)getPositionY());
    bool forever = true;
    // LEFT
    if(HP>=0){
    if(left){
        if(!right)
            direction = Hero::LEFT;
    }
    // RIGHT
    if(right){
        if(!left)
            direction = Hero::RIGHT;
    }
    if(direction==LEFT){
        lastDir = LEFT;
        
        if(!jump){
            if(!left||(right&&left)){
                // IDLE L
                E_NextAnimation = IDLE_LEFT;
            }
            else{
                //WALK L
                E_NextAnimation = LEFT;
                
            }
        }
        else{
            if(dash_timer>0&&fule>0){
                E_NextAnimation = BOOST_LEFT;
            }
            else if(fule>0&&jetPack&&(jump&&(left||right))){
                //FLY L
                //if(!(left&&right)){
                    E_NextAnimation = JUMP_LEFT;
                //}
            }
            else{
                // FALL L
                E_NextAnimation = FALL_LEFT;
            }
            // BOOST L
            if(left_dash>0){
                E_NextAnimation = BOOST_LEFT;
                if(right_dash<0.5*dash_max){
                    E_NextAnimation = dieL;
                }
            }
            
        
        }
    }
    else if(direction==RIGHT){
        if(!jump){
            if(!right||(right&&left)){
                // IDLE R
                E_NextAnimation = IDLE_RIGHT;
            }
            else{
                //WALK R
                E_NextAnimation = RIGHT;
                
            }
        }
        else{
            
            if(dash_timer>0&&fule>0){
                E_NextAnimation = BOOST_RIGHT;
            }
            else if(fule>0&&jetPack&&(jump&&(left||right))){
                //FLY R
                //if(!(left&&right)){
                    E_NextAnimation = JUMP_RIGHT;
                //}
            }
            else{
                // FALL R
                E_NextAnimation = FALL_RIGHT;
            }
        }
        // BOOST R
        if(right_dash>0){
            E_NextAnimation = BOOST_RIGHT;
            if(right_dash<0.5*dash_max){
                E_NextAnimation = dieR;
            }
        }
        
    }
        if(HP==0){
            HP = -1;
            forever = false;
            if(left){
                E_NextAnimation = dieL;
            }
            else{
                E_NextAnimation = dieR;
            }
        }
        if(E_LastAnimation!=E_NextAnimation){
            E_LastAnimation=E_NextAnimation;
            player_sprite->stopAllActions();
            if(direction==LEFT){
                player_sprite->setFlippedX(true);
            }
            else{
                player_sprite->setFlippedX(false);
            }
            if(forever){
                player_sprite->runAction(RepeatForever::create(Animate::create(animationCache->getAnimation(animatioNames[E_NextAnimation]))));
                
            }
            else{
                player_sprite->runAction((Animate::create(animationCache->getAnimation(animatioNames[E_NextAnimation]))));
            }
        }
    }
    
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // LABEL
    string fule_string = std::to_string(int(fule));
    fule_string = "FULE: " + fule_string +" %";
    hp_label->setTextColor(colors[int((fule/33.3))]);
    hp_label->setString(fule_string);
    hp_label->setPosition(this->convertToNodeSpace(Point(0.05*winSize.width,0.7*winSize.height)));

    
}


// TOUCH-CONTROL
int tapChecks = 0;
const int taps =15;
bool Hero::onTouchBegan(const std::vector<Touch*>& touch, Event* event)
{
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    for(int i = 0;i<touch.size();i++){
        if(touch.at(i)->getLocation().x<winSize.width/2&&!left){
            left = true;
            
            if(rightCounter<2)// Dont want double tap on both
                leftCounter++;
        }
        else if(!right){
            right = true;
            if(leftCounter<2)
                rightCounter++;
        }
    }
    tapChecks=0;
    unschedule(schedule_selector(Hero::tapCounter));
    this->schedule(schedule_selector(Hero::tapCounter),1.0/60.0, taps,0);
    return true;
}

void Hero::tapCounter(float delta){
    tapChecks++;
    if(rightCounter>1){
        cocos2d::log("DOUBLE RIGHT\n");
        unschedule(schedule_selector(Hero::tapCounter));
        tapChecks=taps;
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->setEffectsVolume(0.4f);
        audio->playEffect("jet_pack_boost.aif");
        audio->setEffectsVolume(0.25f);
        audio->playEffect("jet_pack_hum.aif", true);

        /*misile = new Guided_Misile(getPositionX()+hitBoxWidth/2,getPositionY()+hitBoxHeight);
        addToGameObjects->pushBack(misile);
        misile->hsp = maxFlySpeed*2;
        misile->vsp = 0.00001;
        misile->start_x = getPositionX();
        misile->start_y = getPositionY()+hitBoxHeight;
        guided_misile = true;
        misile->setRotation(0);*/
        right_dash = dash_max;
    }
    else if(leftCounter>1){
        cocos2d::log("DOUBLE LEFT\n");
        unschedule(schedule_selector(Hero::tapCounter));
        tapChecks=taps;
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->setEffectsVolume(0.4f);
        audio->playEffect("jet_pack_boost.aif");
        audio->setEffectsVolume(0.25f);
        audio->playEffect("jet_pack_hum.aif", true);
        /*misile = new Guided_Misile(getPositionX(),getPositionY()+hitBoxHeight);
        addToGameObjects->pushBack(misile);
        misile->hsp = -maxFlySpeed*2;
        misile->vsp = 0.00001;
        misile->start_x = getPositionX();
        misile->start_y = getPositionY()+hitBoxHeight;
        guided_misile = true;
        misile->setRotation(180);*/
        left_dash = dash_max;
    }
    if(tapChecks==taps){
        leftCounter=0;
        rightCounter=0;
        tapChecks =0;
    }
}

void Hero::onTouchEnded(const std::vector<Touch*>& touches, Event*)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    for(int i = 0;i<touches.size();i++){
        if((touches.at(i)->getStartLocation().x) < winSize.width/2){
            // LEFT
            left = false;
            button_left->setBright(true);
            first_boost = true;
        }
        else{
        // RIGHT
            right = false;
            button_right->setBright(true);
            first_boost = true;
        }
    }
    if(!right&&!left&&(left_dash<=0)&&(right_dash<=0)){
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->stopAllEffects();
    }
}

void Hero::onTouchMoved(const std::vector<Touch*>& touch, Event* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if(alternative_controll){
        for(int i = 0;i<touch.size();i++){
            float current_touch = touch.at(0)->getLocation().y;
            float swipe_distance = current_touch - touch.at(0)->getStartLocation().y;
            if(swipe_distance>jetpack_swipe){
                throtle_amount = swipe_distance/jetpack_swipe;
                if((touch.at(i)->getStartLocation().x) < winSize.width/2){
                // LEFT
                    if(fule>0&&first_boost){
                        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
                        audio->setEffectsVolume(0.2f);
                        audio->playEffect("jet_pack_boost.aif");
                        first_boost = false;
                        audio->setEffectsVolume(0.1f);
                        audio->playEffect("jet_pack_hum.aif", true);
                    }
                    jetPack = true;
                    button_left->setBright(false);
                    jump = true;
                }
                else{
                // RIGHT
                    if(fule>0&&first_boost){
                        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
                        audio->setEffectsVolume(0.2f);
                        audio->playEffect("jet_pack_boost.aif");
                        audio->setEffectsVolume(0.1f);
                        audio->playEffect("jet_pack_hum.aif", true);
                        first_boost = false;
                    }
                    jump = true;
                    jetPack = true;
                    button_right->setBright(false);
                }
               
            }
        }
    }
    
}

void Hero::onTouchCancelled(const std::vector<Touch*>&, Event*)
{
    cocos2d::log("touch cancelled");
}

Hero::~Hero()
{
    cocos2d::log("Dealocate Hero\n");
}


void Hero:: updateTimers(float delta){
    ///////////////
    // TIMERS
    //////////////
    if(touch_timer>=0){
        touch_timer+=delta;
    }
    if(touch_timer>double_touch){
        touch_timer = -1;
    }
    if(dash_timer>0){
        dash_timer-=delta;
    }
    if(cool_down_timer>0){
        cool_down_timer-=delta;
    }
}

// ADD MOVEMENT
void Hero:: addStdMovement(float delta){
    /////////////////////////
    // Add Movement
    ////////////////////////
    if(grounded){
        if(right&&!left){
            if(hsp<maxWalkSpeed){
                hsp += groundAcceleration;
            }
            
        }
        else if(left&&!right){
            if(hsp>-maxWalkSpeed){
                hsp -= groundAcceleration;
            }
        }
        else{
            hsp*=groundDeaccleration;
        }
    }
    
    if(std::abs(hsp)>1.2*maxFlySpeed){
        hsp*=airDeacceleration*0.7;//BOOST DEACELERATION
    }
    
}
void Hero::addJetPackMovement(float delta){
    
    // JET_PACK
    if(!grounded){
        landed = false;// FLYING
    }
    
    
    if(grounded&&!landed){
        landed = true;
        jetPack = false;
        button_left->setBright(true);

    }
    /*// LEFT
     if(!right){
     throtle = -1;
     direction = Hero::LEFT;
     }
     // RIGHT
     if(!left){
     throtle =1;
     direction = Hero::RIGHT;
     }*/
    
    if(!grounded)
        if(right&&!left){
            if(hsp<maxFlySpeed){
                hsp += 1*airAcceleration;
            }
        }
        else if(left&&!right){
            if(hsp>-maxFlySpeed){
                hsp -= 1*airAcceleration;
            }
        }
        else{
            hsp*=(airDeacceleration-(left&right)*0.0005);
        }
    

    if((left||right)&&jetPack&&fule>0){
        fule-=delta*fule_consume_rate;
        if(dash_timer>0){
            fule-=delta*fule_consume_rate*3;
        }
        if((vsp/2)<(MAX_JUMP_SPEED)){
            if(vsp==0){
                vsp = 90;
            }
            vsp+=((boostPower*jetPack))*0.5*throtle_amount;
        }
        
    }
    if(grounded&&fule<fule_max){
        fule+=delta*fule_consume_rate*2;
    }
    
    
    if(!left&&!right){
        throtle = 0;
        jetPack = false;
    }
    
}
void Hero::addDashMovement(float delta){
    vsp = 0;
    attacking = true;
    if(left_dash>0){
        left_dash-=delta;
        hsp=-dash_accelaration;
        if(left_dash>0.5*dash_max){
            // Building upp
            hsp = 0;
            vsp = 0;
            attacking = false;
        }
    }
    else if (right_dash>0){
        right_dash-=delta;
        hsp = dash_accelaration;
        if(right_dash>0.5*dash_max){
            hsp = 0;
            vsp = 0;
        }
    }
    // Stop sound when dashings over
    if(right_dash<0&&left_dash<0){
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->stopAllEffects();
        attacking = false;
    }
}

Point Hero::getCameraView(){
    
     return Point(getPositionX(),getPositionY());//+cameraOffset
    //return(Point(misile->getPositionX(),misile->getPositionY()+hitBoxHeight));
}

bool Hero::hurt(int dmg,AdvancedGameobject *other_obj){
    if(!imune&&!attacking){
        this->imune = true;
        this->HP-=dmg;
        this->schedule(CC_SCHEDULE_SELECTOR(AdvancedGameobject::resetImune),1);
        if(idString=="player"){
            auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
            audio->setEffectsVolume(0.2f);
            audio->playEffect("Cry.aif");
            if(HP>0){
                AdvancedGameobject::flash(5,0.1);
            }
        }
    }
    else{
        other_obj->hurt(1, this);
        other_obj->hsp = hsp*1.4;
        other_obj->vsp = abs(hsp);
    }
    return 0;
}