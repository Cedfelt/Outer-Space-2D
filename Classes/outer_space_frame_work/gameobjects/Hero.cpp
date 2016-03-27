#include "Hero.h"
#include "globals.h"

//#define GUI
using namespace std;
using namespace cocos2d;
Hero::Hero(float sf)
{
  this->resolution_scale = sf;
  hitBoxWidth = 15 * sf;
  hitBoxHeight = 31 * sf;
  setScale(resolution_scale);
  maxFlySpeed *= resolution_scale;
  MAX_JUMP_SPEED *= resolution_scale;
  maxWalkSpeed = 220 / 2 * sf;
  maxFlySpeed = 270 / 2 * sf;
  groundAcceleration = 8 / 2 * sf;;
  groundDeaccleration = 0.65 / 2 * sf;;
  dash_accelaration = 650 / 2 * sf;

  setAnchorPoint(Point(0.5f, 0));
  cameraOffset = 0;
  setupAnimation();
  player_sprite->setAnchorPoint(Point(0.5,0));
  boostPower = 5;
  idString = "player";
  vsp = 0;
  leftCounter = 0;
  rightCounter = 0;
  // SET UP 
  // INIT POS
  /*velocity = Point(0, 0);*/
  //position = Point(256,256);
  // SET UP TOUCH
  auto touchListener = EventListenerTouchAllAtOnce::create();
  touchListener->onTouchesBegan = CC_CALLBACK_2(Hero::onTouchBegan, this);
  touchListener->onTouchesEnded = CC_CALLBACK_2(Hero::onTouchEnded, this);
  touchListener->onTouchesMoved = CC_CALLBACK_2(Hero::onTouchMoved, this);
  touchListener->onTouchesCancelled = CC_CALLBACK_2(Hero::onTouchCancelled, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
  touch_timer = -1;

  //setAnchorPoint(Point(0.5f,0.5f));
  ////////////////////////////
  // ADD UPDATE
  this->schedule(CC_SCHEDULE_SELECTOR(Hero::updateAnimationInterrupt));

  ////////////////////////////
  // OBJECT SETTINGS
 
  airAcceleration = 1.4 / 2;;
  airDeacceleration = 0.998;;
  grounded = false;
  
  cool_down_timer = 0;
  cool_down_timer_max = 0.6f;
  dash_timer_max = 0.5f;
  dash_max = 0.9f;
  // fule
  fule_max = 100;
  fule = fule_max;
  fule_consume_rate = 50;
  
  // Double Touch
  double_touch = 0.5f;
  // DASH
  dash_timer = 0;
  
  drawHitbox();

  // landed
  landed = false;
  toucing_exit = false;




  imune = false;
  //player_sprite->setPosition(0.5f, 0);
  
  player_sprite->getTexture()->setAliasTexParameters();

  
  drawHitbox();
  addHitboxToSprite();
  sFx1 = new SoundFx("jet_pack_boost.aif");
  addChild(sFx1);
  sFx2 = new SoundFx("jet_pack_hum.aif");
  sFx2->loop = TRUE;
  addChild(sFx2);
}

void Hero::updateGameObject(float delta) {
  if (HP <= 0) {
    return;
  }
  if (on_platform) {
    jump = false;
    grounded = true;
  }
  if (HP >= 0 && !guided_misile) {
    if (left_dash <= 0 && right_dash <= 0) {
      check_for_gravity = true;
      addStdMovement(delta);
      addJetPackMovement(delta);
    }
    else {
      check_for_gravity = false;
      addDashMovement(delta);
    }
  }
  

  // Uppdate ViewPoint
  if (vsp<-maxFlySpeed&&cameraOffset>-55) {
    cameraOffset -= (0.8);
  }
  else if (vsp > maxFlySpeed / 8 && cameraOffset < 75) {
    cameraOffset += (0.5);
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

void Hero::setupAnimation() {


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
  Hero::addAnimation("PTSHR", "IdleR", 1, 4, 0.2f);
  Hero::addAnimation("PTSHR", "IdleL", 1, 4, 0.2f);
  Hero::addAnimation("PTSHR", "WalkR", 5, 8, 0.15f);
  Hero::addAnimation("PTSHR", "WalkL", 5, 8, 0.15f);
  Hero::addAnimation("PTSHR", "flyR", 9, 12, 0.2f);
  Hero::addAnimation("PTSHR", "flyL", 9, 12, 0.2f);
  Hero::addAnimation("PTSHR", "fallR", 13, 16, 0.2f);
  Hero::addAnimation("PTSHR", "fallL", 13, 16, 0.2f);
  Hero::addAnimation("PTSHR", "dashR", 17, 20, 0.15f);
  Hero::addAnimation("PTSHR", "dashL", 17, 20, 0.15f);
  Hero::addAnimation("PTSHR", "dieR", 21, 24, 0.15f);
  Hero::addAnimation("PTSHR", "dieL", 21, 24, 0.15f);

  E_LastAnimation = RIGHT;
  E_NextAnimation = RIGHT;
  player_sprite = cocos2d::Sprite::create();
  player_sprite->runAction(RepeatForever::create(Animate::create(animationCache->getAnimation("WalkR"))));
  direction = RIGHT;

  this->addChild(player_sprite, 6);
}



void Hero::updateAnimation(int lastAnimation, int newAnimation) {}

void Hero::updateAnimationInterrupt(float delta) {}


// TOUCH-CONTROL
int tapChecks = 0;
const int taps = 15;
bool Hero::onTouchBegan(const std::vector<Touch*>& touch, Event* event)
{

  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
  for (int i = 0;i < touch.size();i++) {
    if (touch.at(i)->getLocation().x < winSize.width / 2 && !left) {
      left = true;

      if (rightCounter < 2)// Dont want double tap on both
        leftCounter++;
    }
    else if (!right) {
      right = true;
      if (leftCounter < 2)
        rightCounter++;
    }
  }
  tapChecks = 0;
  unschedule(schedule_selector(Hero::tapCounter));
  this->schedule(schedule_selector(Hero::tapCounter), 1.0 / 60.0, taps, 0);
  return true;
}

void Hero::tapCounter(float delta) {
  tapChecks++;
  if (rightCounter > 1) {
    cocos2d::log("DOUBLE RIGHT\n");
    unschedule(schedule_selector(Hero::tapCounter));
    tapChecks = taps;
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
  else if (leftCounter > 1) {
    cocos2d::log("DOUBLE LEFT\n");
    unschedule(schedule_selector(Hero::tapCounter));
    tapChecks = taps;
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
  if (tapChecks == taps) {
    leftCounter = 0;
    rightCounter = 0;
    tapChecks = 0;
  }
}

void Hero::onTouchEnded(const std::vector<Touch*>& touches, Event*)
{
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  for (int i = 0;i < touches.size();i++) {
    if ((touches.at(i)->getStartLocation().x) < winSize.width / 2) {
      // LEFT
      left = false;
#ifdef GUI
      button_left->setBright(true);
#endif
      first_boost = true;
    }
    else {
      // RIGHT
      right = false;
#ifdef GUI
      button_right->setBright(true);
#endif
      first_boost = true;
    }
  }
  if (!right&&!left && (left_dash <= 0) && (right_dash <= 0)) {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopAllEffects();
  }
}

void Hero::onTouchMoved(const std::vector<Touch*>& touch, Event* event)
{
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  if (alternative_controll) {
    for (int i = 0;i < touch.size();i++) {
      float current_touch = touch.at(0)->getLocation().y;
      float swipe_distance = current_touch - touch.at(0)->getStartLocation().y;
      if (swipe_distance > jetpack_swipe) {
        throtle_amount = swipe_distance / jetpack_swipe;
        throtle_amount *= throtle_amount;// adding a different throtle function x2
        if ((touch.at(i)->getStartLocation().x) < winSize.width / 2) {
          // LEFT
          if (fule > 0 && first_boost) {
            sFx1->pan = -0.5f;
            sFx1->play(0.2f);
            sFx2->play(0.1f);
            first_boost = false;
          }
          jetPack = true;
#ifdef GUI
          button_left->setBright(false);
#endif
          jump = true;
        }
        else {
          // RIGHT
          if (fule > 0 && first_boost) {
            auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
            sFx1->pan = 0.5f;
            sFx1->play(0.2f);
            sFx2->play(0.1f);
            first_boost = false;
          }
          jump = true;
          jetPack = true;
#ifdef GUI
          button_right->setBright(false);
#endif
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




// ADD MOVEMENT
void Hero::addStdMovement(float delta) {
  /////////////////////////
  // Add Movement
  ////////////////////////
  if (grounded) {
    if (right&&!left) {
      if (hsp < maxWalkSpeed) {
        /*hsp += groundAcceleration;*/
        hsp = maxWalkSpeed;
      }
      setAnimation("WalkR");
      player_sprite->setScaleX(1);
      direction = RIGHT;

    }
    else if (left&&!right) {
      if (hsp > -maxWalkSpeed) {
        /*hsp -= groundAcceleration;*/
        hsp = -maxWalkSpeed;
      }
      setAnimation("WalkL");
      player_sprite->setScaleX(-1);
      direction = LEFT;
    }
    else {
      if (direction == RIGHT) {
        setAnimation("IdleR");
        player_sprite->setScaleX(1);
      }
      else {
        setAnimation("IdleL");
        player_sprite->setScaleX(-1);
      }
      hsp *= groundDeaccleration;
    }
  }

}
void Hero::addJetPackMovement(float delta) {

  // JET_PACK
  if (!grounded) {
    landed = false;// FLYING
  }



  if (grounded&&!landed) {
    landed = true;
    jetPack = false;
#ifdef GUI
    button_left->setBright(true);
#endif

  }


  // Air control without jetpack
  if (!grounded&&!jetPack)
    if (right&&!left) {
      if (hsp < maxFlySpeed) {
        hsp += 1 * airAcceleration * 0.4f;
        direction = RIGHT;
      }
    }
    else if (left&&!right) {
      if (hsp > -maxFlySpeed) {
        hsp -= 1 * airAcceleration * 0.7f;
        direction = LEFT;
      }
    }
    else {
      hsp *= (airDeacceleration);
    }


    if ((left || right) && jetPack&&fule > 0) {
      fule -= delta*fule_consume_rate;
      if (dash_timer > 0) {
        fule -= delta*fule_consume_rate * 3;
      }
      if ((vsp / 2) < (MAX_JUMP_SPEED)) {
        if (vsp == 0) {
          vsp = 90;
        }
        vsp += ((boostPower*jetPack))*0.25*throtle_amount;
      }

      // Add Setering when in mid-air
      if (right&&!left) {
        if (hsp < maxFlySpeed) {
          hsp += 1 * airAcceleration;// NOT THIS
          direction = RIGHT;
        }
      }
      else if (left&&!right) {
        if (hsp > -maxFlySpeed) {
          hsp -= 1 * airAcceleration;// NOT THIS
          direction = LEFT;
        }
      }
    }
    if (grounded&&fule < fule_max) {
      fule += delta*fule_consume_rate * 2;
    }


    if (!left&&!right) {
      throtle = 0;
      jetPack = false;
    }

    if (std::abs(hsp) > 1.2*maxFlySpeed) {
      hsp *= airDeacceleration*0.7;//BOOST DEACELERATION
    }

    // Animation setup
    if ((vsp > 0 || jetPack) && !grounded) {
      if (direction == RIGHT) {
        setAnimation("flyR");
        player_sprite->setScaleX(1);
      }
      else {
        setAnimation("flyL");
        player_sprite->setScaleX(-1);
      }
    }
    else if (vsp < 0 && !grounded) {
      if (direction == RIGHT) {
        setAnimation("fallR");
        player_sprite->setScaleX(1);
      }
      else {
        setAnimation("fallL");
        player_sprite->setScaleX(-1);
      }
    }

}
void Hero::addDashMovement(float delta) {
  vsp = 0;
  attacking = false;
  if (left_dash > 0) {
    direction = LEFT;
    left_dash -= delta;
    if (left_dash > 0.5*dash_max) {
      hsp = 0;
      vsp = 0;
      setAnimation("dashL");
      player_sprite->setScaleX(-1);
    }
    else {
      if (fule >= 0) {
        attacking = true;
        fule -= delta*fule_consume_rate * 0.5f;
        setAnimation("dieL");
        player_sprite->setScaleX(-1);
        hsp = -dash_accelaration;
      }
      else
        left_dash = -1;
    }
  }
  else if (right_dash > 0) {
    direction = RIGHT;
    right_dash -= delta;
    if (right_dash > 0.5*dash_max) {
      hsp = 0;
      vsp = 0;
      setAnimation("dashR");
      player_sprite->setScaleX(1);
    }
    else {
      if (fule >= 0) {
        attacking = true;
        fule -= delta*fule_consume_rate * 0.5f;
        setAnimation("dieR");
        player_sprite->setScaleX(1);
        hsp = dash_accelaration;
      }
      else
        right_dash = -1;
    }
  }
  // Stop sound when dashings over
  if (right_dash < 0 && left_dash < 0) {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopAllEffects();
    attacking = false;
  }
}

Point Hero::getCameraView() {

  return Point(getPositionX(), getPositionY());//+cameraOffset
 //return(Point(misile->getPositionX(),misile->getPositionY()+hitBoxHeight));
}

bool Hero::hurt(int dmg, AdvancedGameobject *other_obj) {
  if ((!imune&&!attacking)||(dmg==HUGE_AMOUNT_OF_DMG)) {
    this->imune = true;
    this->HP -= dmg;
    this->schedule(CC_SCHEDULE_SELECTOR(AdvancedGameobject::resetImune), 1);
    if (idString == "player") {
      auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
      audio->setEffectsVolume(0.2f);
      audio->playEffect("Cry.aif");
      if (HP > 0) {
        AdvancedGameobject::flash(5, 0.1);
      }
    }
  }
  else {
    other_obj->hurt(1, this);
    //other_obj->hsp = hsp*1.4;
    //other_obj->vsp = abs(hsp);
  }
  return 0;
}