//
//  Projectile.hpp
//  SpaceGame
//
//  Created by Anders on 29/10/15.
//
//


#pragma once
#include <stdio.h>
#include "outer_space_frame_work/gameobjects/EnemyGameobject.h"

class Projectile : public EnemyGameObject{
public:
    Projectile(float x,float y);
    void colide(AdvancedGameobject *other_obj);
    ~Projectile();
    void updateGameObject(float delta);
    
};