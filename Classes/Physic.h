//
//  Physic.hpp
//  SpaceGame
//
//  Created by Anders on 06/10/15.
//
//

#ifndef Physic_h
#define Physic_h

#include <stdio.h>
#include "Advanced_Gameobject.h"
#include "Hero.h"
#include "Level.h"

USING_NS_CC;

using namespace std;
using namespace cocos2d;
void moveGameObject(AdvancedGameobject *obj,Level *level,float delta);
void gameObjectCollision(Vector<AdvancedGameobject*> *gameObjects,Hero *hero,float delta);
int sign(float num);

#endif /* Physic_hpp */
