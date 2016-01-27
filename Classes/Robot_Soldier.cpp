//
//  Robot_Soldier.cpp
//  SpaceGame
//
//  Created by Anders on 10/01/16.
//
//

#include "Robot_Soldier.h"
#include "cocos2d.h"

Robot_Soldier::Robot_Soldier(){
    player_sprite = cocos2d::Sprite::create();
    player_sprite->setAnchorPoint(Point(0.5,0));
    this->addChild(player_sprite);
    hsp =60;
    right = true;
    hitBoxHeight = 32;
    hitBoxWidth = 32;
    addHitboxToSprite();
    //drawHitbox();
    this->setScale(3);
    this->schedule(CC_SCHEDULE_SELECTOR(Robot_Soldier::updateAI),0.5);
    AI_STATE = AI_PATROLLING;
    HP = 3;
    check_for_gravity = true;
    maxFlySpeed = 300;
    this->visionRange*=2;
    idString = "enemy";
    spriteFrameCache = spriteFrameCache->getInstance();
    animationCache = animationCache->getInstance();
    spriteFrameCache->addSpriteFramesWithFile("Robot_Soldier.plist");
    addAnimation("Robot_Soldier ","robSol_WalkRight",1,4,0.2f);
    addAnimation("Robot_Soldier ","robSol_WalkLeft",1,4,0.2f);
    addAnimation("Robot_Soldier ","robSol_Dead",5,6,0.3f);
    maxWalkSpeed = 30;
    hsp = maxWalkSpeed;
    HP = 1;
    moveWhenOutsideOfScreen = true;
}


void Robot_Soldier::updateGameObject(float delta){
    
    
    if(AI_STATE!=AI_DEAD){
        if(HP<=0){
            AI_STATE = AI_DEAD;
            auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
            audio->setEffectsVolume(0.2f);
            audio->playEffect("explosion.aif");
            if(hsp>=0){
                setAnimation("robSol_Dead");
                player_sprite->setScaleX(1);
            }
            else{
                setAnimation("robSol_Dead");
                player_sprite->setScaleX(-1);
            }
            
            return;
            
        }
        if(hsp>0){
            setAnimation("robSol_WalkRight");
            player_sprite->setScaleX(1);
        }
        else{
            setAnimation("robSol_WalkLeft");
            player_sprite->setScaleX(-1);
        }
    }
    else{
        hsp *= 0.98;
    }
}
int lost_track = 0;
void Robot_Soldier::updateAI(float delta){
        if(AI_STATE == AI_PATROLLING){
            dumbWalk();
            player_sprite->setColor(cocos2d::Color3B::YELLOW);
            if(targetIsVisible(target)||target_state == TARGET_VISIBLE){
                cocos2d::log("ALERTED}\n");
                AI_STATE = AI_ATTACK;
                target_state = TARGET_VISIBLE;
            }
            return;
        } 
        else if(AI_STATE == AI_ATTACK){
            approachTarget();
            player_sprite->setColor(cocos2d::Color3B::RED);
            if(targetIsVisible(target)){
                lost_track = 0;
                target_state = NO_TRACE_OF_TARGET;
                shoot_target(1);
            }
            else{
                lost_track++;
            }
            if(lost_track>75){
                AI_STATE = AI_PATROLLING;
            }
            return;
        }
}

void Robot_Soldier::colide(AdvancedGameobject *other_obj){
    if(other_obj->idString == "player"&&HP>0){
        bump_and_hurt(other_obj);
    }
}

Robot_Soldier::~Robot_Soldier(){
    
}