//
//  ScanTiledMap.cpp
//  SpaceGame
//
//  Created by Anders on 11/11/15.
//
//



#include "Passage.h"

// Constructor & Destructor
Passage::Passage(float x,float y,float w,float h,string sc,string ps){
    setPosition(x, y);
    hitBoxWidth = w;
    hitBoxHeight = h;
    leads_to_scene = sc;
    leads_to_passage = ps;
    addHitboxToSprite();
    //drawHitbox();
    touching_player =false;
    touchCounter = 0;
    this->idString = ps;
}
Passage::~Passage(){

}

//Functions & Methods
void Passage::colide(AdvancedGameobject *other_obj){
    
}

void Passage::updateGameObject(float delta){
    
    Rect player_r;
    if(target!=NULL){
        Vec2 vh = target->getPosition();
        player_r.setRect(target->getPositionX()-target->hitboxModX, vh.y-target->hitboxModY, target->hitBoxWidth, target->hitBoxHeight);
        Rect passage_r;
        Vec2 pass = getPosition();
        passage_r.setRect(getPositionX()-hitboxModX, getPositionY()-hitboxModY, hitBoxWidth, hitBoxHeight);
        
        if(!passage_r.intersectsRect(player_r)){
            ready = true;
        }
        if(passage_r.intersectsRect(player_r)&&ready){
            target->toucing_exit = true;
            target->go_to_nextScene = true;
            target->go_to_scene = leads_to_scene;
            target->go_to_passage = leads_to_passage;
        }
    
    }
    
}

