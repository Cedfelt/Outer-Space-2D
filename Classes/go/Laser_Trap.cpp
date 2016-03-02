//
//  Laser_Trap.cpp
//  SpaceGame
//
//  Created by Anders on 10/01/16.
//
//

#include "Laser_Trap.h"
#include "cocos2d.h"

Laser_Trap::Laser_Trap(float duration,uint32_t direction,uint32_t range,float delay) {
  player_sprite = cocos2d::Sprite::create();
  player_sprite->setAnchorPoint(Point(0.0, 0));
  this->addChild(player_sprite);
  hsp = 0;
  hitBoxHeight = 32;
  hitBoxWidth = 32;
  addHitboxToSprite();
  this->setScale(3);
  this->schedule(CC_SCHEDULE_SELECTOR(Laser_Trap::updateAI), 0.5);
  HP = 3;
  check_for_gravity = false;
  idString = "object";
  spriteFrameCache = spriteFrameCache->getInstance();
  animationCache = animationCache->getInstance();
  spriteFrameCache->addSpriteFramesWithFile("laser_trap.plist");
  addAnimation("laser_trap", "laser_trap_base", 5, 5, 2.f);
  addAnimation("laser_trap", "laser_mid", 3, 4, 6.0/60.0);
  addAnimation("laser_trap", "laser_top", 1, 2, 6.0 / 60.0);
  setAnimation("laser_trap_base");
  maxWalkSpeed = 0;
  hsp = 0;
  HP = 1;
  time = duration;
  time_counter = time;
  setAnchorPoint(Point(0.5, 0));
  moveWhenOutsideOfScreen = true;
  bool b = true;
  this->range = range;
  for (int i = 1;i <= range;i++) {
    laser_sprites[i-1] = Sprite::create("laser_trap.png");
    laser_sprites[i-1]->setAnchorPoint(Point(0.0, 0));
    addChild(laser_sprites[i-1]);
    laser_sprites[i-1]->setPosition(0, i*8);
    laser_sprites[i-1]->runAction(RepeatForever::create(Animate::create(animationCache->getAnimation("laser_mid"))));
    laser_sprites[i - 1]->setOpacity(0);
  }
  laser_sprites[range-1]->stopAllActions();
  laser_sprites[range - 1]->runAction(RepeatForever::create(Animate::create(animationCache->getAnimation("laser_top"))));
  current = 0;
  output = true;

    this->unschedule(CC_SCHEDULE_SELECTOR(AdvancedGameobject::updateGameObject));
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(Laser_Trap::add_updateFunction),delay);
  

}

void Laser_Trap::add_updateFunction(float delta) {
  this->schedule(CC_SCHEDULE_SELECTOR(Laser_Trap::updateGameObject));
}

void Laser_Trap::updateGameObject(float delta) {
  Laser_Trap *lt = this;
  if(time_counter>0){
    time_counter -=delta;
    return;
  }
  
  if (output) {
    laser_sprites[current]->setOpacity(0xFF);
    current++;
    if (current == range) {
      output = false;
      current = range - 1;
      time_counter = time;
    }
  }
  else {
    laser_sprites[current]->setOpacity(0x00);
    current--;
    if (current< 0) {
      output = true;
      current = 0;
      time_counter = time;
    }
  }
}

void Laser_Trap::updateAI(float delta) {
  
}

void Laser_Trap::colide(AdvancedGameobject *other_obj) {

}

Laser_Trap::~Laser_Trap() {

}