//
//  Robot_Soldier.cpp
//  SpaceGame
//
//  Created by Anders on 10/01/16.
//
//

#include "Robot_Soldier.h"
#include "cocos2d.h"

Robot_Soldier::Robot_Soldier(const float scale) {
  this->resolution_scale = scale;
  hitBoxWidth = 15 * scale;
  hitBoxHeight = 31 * scale;
  setScale(resolution_scale);
  maxWalkSpeed = 100 / 2 * scale;
  maxFlySpeed = 270 / 2 * scale;
  groundAcceleration = 8 / 2 * scale;
  groundDeaccleration = 0.65 / 2 * scale;
  player_sprite = cocos2d::Sprite::create();
  player_sprite->setAnchorPoint(Point(0.5, 0));
  this->addChild(player_sprite);
  hsp = 60;
  right = true;
  hitBoxHeight = 32;
  hitBoxWidth = 32;
  addHitboxToSprite();
  //drawHitbox();
  this->schedule(CC_SCHEDULE_SELECTOR(Robot_Soldier::updateAI), 0.5);
  AI_STATE = AI_PATROLLING;
  HP = 3;
  check_for_gravity = true;
  this->visionRange *= 2;
  idString = "enemy";
  spriteFrameCache = spriteFrameCache->getInstance();
  animationCache = animationCache->getInstance();
  spriteFrameCache->addSpriteFramesWithFile("Robot_Soldier.plist");
  addAnimation("Robot_Soldier ", "robSol_WalkRight", 1, 4, 0.2f);
  addAnimation("Robot_Soldier ", "robSol_WalkLeft", 1, 4, 0.2f);
  addAnimation("Robot_Soldier ", "robSol_Dead", 5, 6, 0.3f);
  hsp = maxWalkSpeed;
  HP = 1;
  moveWhenOutsideOfScreen = true;
}


void Robot_Soldier::updateGameObject(float delta) {


  if (AI_STATE != AI_DEAD) {
    if (HP <= 0) {
      AI_STATE = AI_DEAD;
      auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
      audio->setEffectsVolume(0.2f);
      audio->playEffect("explosion.aif");
      if (hsp >= 0) {
        setAnimation("robSol_Dead");
        player_sprite->setScaleX(1);
      }
      else {
        setAnimation("robSol_Dead");
        player_sprite->setScaleX(-1);
      }

      return;

    }
    if (hsp > 0) {
      setAnimation("robSol_WalkRight");
      player_sprite->setScaleX(1);
    }
    else {
      setAnimation("robSol_WalkLeft");
      player_sprite->setScaleX(-1);
    }
  }
  else {
    hsp *= 0.98;
  }
}
int lost_track = 0;
void Robot_Soldier::updateAI(float delta) {
  if (AI_STATE == AI_PATROLLING) {
    dumbWalk();
    if (targetIsVisible(target)) {
      cocos2d::log("ALERTED}\n");
      AI_STATE = AI_ATTACK;
    }
    return;
  }
  else if (AI_STATE == AI_ATTACK) {
    approachTarget();
    if (targetIsVisible(target)) {
      lost_track = 0;
      shoot_target(1);
    }
    else {
      lost_track++;
    }
    if (lost_track > 75) {
      AI_STATE = AI_PATROLLING;
    }
    return;
  }
}

void Robot_Soldier::colide(AdvancedGameobject *other_obj) {
  if (other_obj->idString == "player"&&HP > 0) {
    bump_and_hurt(other_obj);
  }
}

Robot_Soldier::~Robot_Soldier() {

}