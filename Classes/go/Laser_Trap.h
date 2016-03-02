//
//  Robot_Soldier.hpp
//  SpaceGame
//
//  Created by Anders on 10/01/16.
//
//

#pragma once
#include <stdio.h>
#include "cocos2d.h"
#include "outer_space_frame_work/gameobjects/EnemyGameobject.h"

USING_NS_CC;
#define MAX_CAPACITY 15
class Laser_Trap : public EnemyGameObject{
public:
    Laser_Trap(float duration, uint32_t direction, uint32_t range, float delay);
    int range;
    void colide(AdvancedGameobject *other_obj);
    ~Laser_Trap();
    void updateGameObject(float delta);
    void add_updateFunction(float delta);
    void updateAI(float delta);
    Sprite *laser_sprites[MAX_CAPACITY];
    int current;
    bool output;
    float time;
    float time_counter;
};
