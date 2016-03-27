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

class Robot_Soldier : public EnemyGameObject{
public:
    Robot_Soldier(const float scale);
    void colide(AdvancedGameobject *other_obj);
    ~Robot_Soldier();
    void updateGameObject(float delta);
    void updateAI(float delta);
    
    
};
