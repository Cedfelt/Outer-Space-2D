//
//  EnemyGameObject.cpp
//  SpaceGame
//
//  Created by Anders on 05/11/15.
//
//

#include "EnemyGameObject.h"
#include  "Projectile.h"




// ENEMY GO
EnemyGameObject::EnemyGameObject(){
    shooting = false;
    target_state = NO_TRACE_OF_TARGET;
    
}


void EnemyGameObject::dumbWalk(){
    if(hsp == 0||!inZone()){
        if(right){
            right = false;
            left = true;
            hsp = -maxWalkSpeed;
        }
        else{
            right = true;
            left = false;
            hsp = maxWalkSpeed;
        }
    }
}



void EnemyGameObject::approachTarget(){
    if(hsp ==0&&vsp==0){
        vsp = maxFlySpeed;
    }
    float ang = angleToTarget(target);
    
    if(ang*57>90){
        right = false;
        left = true;
        hsp = -maxWalkSpeed*1.5;
    }
    else{
        right = true;
        left = false;
        hsp = maxWalkSpeed*1.5;
    }
    
}

// ENEMY GO
void EnemyGameObject::shot_player_in_zone(){
    if(targetInZone(target)&&!shooting){
        shooting = true;
        this->schedule(schedule_selector(EnemyGameObject::shoot_target),0,0,0);
    }
}

void EnemyGameObject::shoot_target(float delta){
    
    float ang = angleToTarget(target);
    float xSpeed = cos(ang);
    float ySpeed = sin(ang);
    Projectile *proj = new Projectile(getPositionX()+hitBoxWidth/2,getPositionY()+hitBoxHeight/2);
    addToGameObjects->pushBack(proj);
    proj->hsp = xSpeed*proj->maxFlySpeed+hitBoxWidth/2;
    proj->vsp = ySpeed*proj->maxFlySpeed+hitBoxHeight/2;
    proj->start_x = getPositionX()+hitBoxWidth/2;
    proj->start_y = getPositionY()+hitBoxHeight/2;
    this->schedule(schedule_selector(EnemyGameObject::reload),0,0,reload_time);
    
}

void EnemyGameObject::reload(float delta){
    shooting = false;
    this->unschedule(schedule_selector(EnemyGameObject::shoot_target));
    this->unschedule(schedule_selector(EnemyGameObject::reload));
}

void EnemyGameObject::bump_and_hurt(AdvancedGameobject *other_obj){
    float angle = angleToTarget(other_obj);
    float xForce = cos(angle);
    float yForce = sin(angle);
    if(yForce ==0){
        yForce = 0.1;
    }
    other_obj->hsp = 100*xForce;
    other_obj->vsp = 100*yForce;
    other_obj->hurt(1,this);
}

bool EnemyGameObject::inZone(){
    if(zone.size.width==0){
        return true;
    }
    Rect er;
    er.setRect(getPositionX()-hitboxModX, getPositionY()-hitboxModY, hitBoxWidth, hitBoxHeight);
    return er.intersectsRect(zone);
}

bool EnemyGameObject::targetInZone(AdvancedGameobject *tar){
    if(zone.size.width==0){
        return true;
    }
    Rect target;
    float xx = (tar->getPosition()).x;
    float yy = (tar->getPosition()).y;
    target.setRect(xx, yy, tar->hitBoxWidth, tar->hitBoxHeight);
    return target.intersectsRect(zone);
}
void EnemyGameObject::updateAI(float delta){

}

EnemyGameObject::~EnemyGameObject(){
    cocos2d::log("Dealocate EnemyGo\n");
}
