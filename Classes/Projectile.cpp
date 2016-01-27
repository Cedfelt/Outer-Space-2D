//
//  Projectile.cpp
//  SpaceGame
//
//  Created by Anders on 29/10/15.
//
//

#include "Projectile.h"
#include "cocos2d.h"

Projectile::Projectile(float x,float y){
    player_sprite = cocos2d::Sprite::create("space_pig1.png");
    player_sprite->setAnchorPoint(Point(0,0));
    //this->addChild(player_sprite);
    setPosition(x,y);
    right = true;
    hitBoxHeight = 16;
    hitBoxWidth = 16;
    maxWalkSpeed = 30;
    maxFlySpeed = 450;
    addHitboxToSprite();
    drawHitbox();
    this->setScale(1);
    this->check_for_gravity = false;
    this->vsp;
    setPos(start_x, start_y);
    moveWhenOutsideOfScreen = true;
    
    
}


void Projectile::updateGameObject(float delta){
    if(vsp==0||hsp==0){
        this->removeObj = true;
    }
}

void Projectile::colide(AdvancedGameobject *other_obj){
    if(other_obj->idString == "player"){
        bump_and_hurt(other_obj);
        this->removeObj = true;
    }
}

Projectile::~Projectile(){
    
}

