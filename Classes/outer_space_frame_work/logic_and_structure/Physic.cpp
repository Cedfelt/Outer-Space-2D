//
//  Physic.cpp
//  SpaceGame
//
//  Created by Anders on 06/10/15.
//
//

#include "Physic.h"
#include "globals.h"

using namespace std;
void moveGameObject(AdvancedGameobject *obj,Level *level,float delta){
    
    /////////////////////////
    // ADD GRAVITY
    ////////////////////////
    if(obj->vsp>MAX_FALL_SPEED&&obj->check_for_gravity){
        obj->vsp+=GRAVITY*delta;
    }
    
    /////////////////////////
    // HORIZONTAL COLLISION & MOVEMENT
    ////////////////////////
    float distance = obj->hsp*delta;
        Rect player_r;
        Vec2 vh = obj->getPosition();
        player_r.setRect(vh.x-obj->hitboxModX, vh.y-obj->hitboxModY, obj->hitBoxWidth, obj->hitBoxHeight);

    if(level->getBlocked(player_r.getMinX()+(distance),player_r.getMinY(),obj->hitBoxWidth,obj->hitBoxHeight)){
        float signn = -1*sign(obj->hsp);
        obj->hsp =0;
        int counter = 0;
        while(!level->getBlocked(player_r.getMinX()+(distance),player_r.getMinY(),obj->hitBoxWidth,obj->hitBoxHeight)){
            distance+=signn;
            float move = obj->getPositionX()+distance;
            obj->setPosition(move, obj->getPositionY());
            player_r.setRect(obj->getPositionX()-obj->hitboxModX, vh.y-obj->hitboxModY, obj->hitBoxWidth, obj->hitBoxHeight);
            
        }
            distance = 0;
    }
    obj->setPosition(obj->getPositionX()+distance, obj->getPositionY());
    /////////////////////////
    // VERTICAL COLLISION & MOVEMENT
    ////////////////////////
    distance = obj->vsp*delta;
    if(level->getBlocked(player_r.getMinX(),player_r.getMinY()+distance,obj->hitBoxWidth,obj->hitBoxHeight)){
        obj->jump = false;
        float signn = -sign(obj->vsp);
        if(obj->vsp<=0){
            obj->grounded=1;
        }
        
        while(level->getBlocked(player_r.getMinX(),player_r.getMinY()+distance,obj->hitBoxWidth,obj->hitBoxHeight)){
            obj->vsp = 0;
            distance += signn;
            float move = vh.y+distance;
            //obj->setPosition(obj->getPositionX(), move);
            player_r.setRect(vh.x-obj->hitboxModX,move, obj->hitBoxWidth, obj->hitBoxHeight);
        }
        distance = 0;
        
    }
    else{
        obj->setPosition(obj->getPositionX(), obj->getPositionY()+distance);
        obj->grounded = 0;
       
    }
    
    
    
}

// TO-DO- add detection for all objects
void gameObjectCollision(Vector<AdvancedGameobject*> *gameObjects,Hero *hero,float delta){
    for(int i = 0;i<gameObjects->size();i++){
        if(gameObjects->at(i)->isVisible()){
            Rect hr;
            hr.setRect(hero->getPositionX()-hero->hitboxModX, hero->getPositionY()-hero->hitboxModY , hero->hitBoxWidth, hero->hitBoxHeight);

            Rect er;
            AdvancedGameobject *obj = gameObjects->at(i);
            er.setRect(obj->getPositionX()-obj->hitboxModX, obj->getPositionY()-obj->hitboxModY, obj->hitBoxWidth, obj->hitBoxHeight);
            if(hr.intersectsRect(er)){
                hero->colide(gameObjects->at(i));
                gameObjects->at(i)->colide(hero);
            }
        }
    }
}

int sign(float num){
    if (num<0){
        return -1;
    }
    return 1;
}
