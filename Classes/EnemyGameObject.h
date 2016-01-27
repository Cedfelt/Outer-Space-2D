//
//  EnemyGameObject.hpp
//  SpaceGame
//
//  Created by Anders on 05/11/15.
//
//

#pragma once
#include "cocos2d.h"
#include "Advanced_Gameobject.h"

#include <stdio.h>
static int target_state;
class EnemyGameObject : public AdvancedGameobject{
public:
    // ENEMY gameObjects
    EnemyGameObject();
    virtual void dumbWalk();
    virtual void approachTarget();
    virtual void bump_and_hurt(AdvancedGameobject *other_obj);
    virtual void shot_player_in_zone();
    virtual bool inZone();
    virtual bool targetInZone(AdvancedGameobject *target);
    virtual void shoot_target(float delta);
    virtual void reload(float delta);
    bool shooting;
    virtual void updateAI(float delta);
    float reload_time =1;
    
    enum target_states{
        NO_TRACE_OF_TARGET,
        TARGET_VISIBLE,
    
    };
    
    ~EnemyGameObject();

};

