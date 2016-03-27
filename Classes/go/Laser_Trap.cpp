//
//  Laser_Trap.cpp
//  SpaceGame
//
//  Created by Anders on 10/01/16.
//
//

#include "Laser_Trap.h"
#include "cocos2d.h"

enum LASER_DIR {
  LASER_LEFT,
  LASER_RIGHT,
  LASER_UP,
  LASER_DOWN
};

Laser_Trap::Laser_Trap(float resolution_scale, float output_time, float input_time, uint32_t direction, uint32_t range, float delay) {

  resolution_scale *= 2;
  CCASSERT(range < MAX_CAPACITY, "LASER BUGG TO SMALL");
  this->resolution_scale = resolution_scale;
  player_sprite = cocos2d::Sprite::create();
  setAnchorPoint(Point(0.5, 0));
  this->addChild(player_sprite);
  player_sprite->setAnchorPoint(Point(0.5, 0));
  dmg_power = HUGE_AMOUNT_OF_DMG;
   hitBoxHeight = 8* range * resolution_scale;
  hitBoxWidth = 8 *resolution_scale;
  
 
  this->setScale(resolution_scale);
  this->schedule(CC_SCHEDULE_SELECTOR(Laser_Trap::updateAI), 0.5);
  HP = 3;
  check_for_gravity = false;
  idString = "object";
  spriteFrameCache = spriteFrameCache->getInstance();
  animationCache = animationCache->getInstance();
  spriteFrameCache->addSpriteFramesWithFile("laser_trap.plist");
  addAnimation("laser_trap", "laser_trap_base", 5, 5, 2.f);
  addAnimation("laser_trap", "laser_mid", 3, 4, 6.0 / 60.0);
  addAnimation("laser_trap", "laser_top_down", 1, 2, 6.0 / 60.0);
  setAnimation("laser_trap_base");
  maxWalkSpeed = 0;
  hsp = 0;
  HP = 1;
  o_time = output_time;
  i_time = input_time;
  ot_counter = o_time;
  setAnchorPoint(Point(0.5, 0));
  moveWhenOutsideOfScreen = true;
  laser_delay = delay;
  bool b = true;
  this->range = range;
  for (int i = 1;i <= range;i++) {
    laser_sprites[i - 1] = Sprite::create("laser_trap.png");
    laser_sprites[i - 1]->setAnchorPoint(Point(0.5, 0));
    addChild(laser_sprites[i - 1]);
    laser_sprites[i - 1]->setPosition(0, i * 8);
    laser_sprites[i - 1]->runAction(RepeatForever::create(Animate::create(animationCache->getAnimation("laser_mid"))));
    laser_sprites[i - 1]->setOpacity(0);
    addChild(laser_sprites[i - 1]);
  }
  laser_sprites[range - 1]->stopAllActions();
  laser_sprites[range - 1]->runAction(RepeatForever::create(Animate::create(animationCache->getAnimation("laser_top_down"))));
  current = 0;
  if (direction == LASER_UP) {
    addHitboxToSprite();
    /*drawHitbox();*/
  }
  else if (direction == LASER_DOWN) {
    this->setScaleY(-1 * this->getScaleY());
  }
  else if (direction == LASER_LEFT) {
    
    this->setRotation(-90);
  }
  else if (direction == LASER_RIGHT) {
    this->setRotation(90);
  }
  output = true;
  ot_counter = o_time;
  ot_counter = 0;
  this->unschedule(CC_SCHEDULE_SELECTOR(AdvancedGameobject::updateGameObject));
  add_updateFunction(delay);
  
}

void Laser_Trap::add_updateFunction(float delta) {
  this->schedule(CC_SCHEDULE_SELECTOR(Laser_Trap::updateGameObject));
}

void Laser_Trap::updateGameObject(float delta) {
  Laser_Trap *lt = this;
  if (ot_counter > 0) {
    ot_counter -= delta;
    return;
  }

  if (output) {
    laser_sprites[current]->setOpacity(0xFF);
    current++;
    if (current == range) {
      output = false;
      current = range - 1;
      ot_counter = o_time;
    }
  }
  else {
    laser_sprites[current]->setOpacity(0x00);
    current--;
    if (current < 0) {
      output = true;
      current = 0;
      ot_counter = i_time;
    }
  }
}

void Laser_Trap::updateAI(float delta) {

}

void Laser_Trap::colide(AdvancedGameobject *other_obj) {
  if (!output) {
    if (other_obj->idString == "player"&&HP>0 && !other_obj->imune) {
      bump_and_hurt(other_obj);
    }
  }
}

bool Laser_Trap::hurt(int dmg, AdvancedGameobject *other_obj) {
  return false;
}

Laser_Trap::~Laser_Trap() {

}