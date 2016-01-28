//
//  ScanTiledMap.cpp
//  SpaceGame
//
//  Created by Anders on 11/11/15.
//
//

#pragma once

#include "cocos2d.h"
#include <vector>
#include "Advanced_Gameobject.h"


using namespace std;
USING_NS_CC;
class Passage : public AdvancedGameobject{
public:
    // Constructor & Destructor
    Passage(float x,float y,float w,float h,string sc,string ps);
    ~Passage();

    //Functions & Methods
    void colide(AdvancedGameobject *other_obj);
    void updateGameObject(float delta);
    
    
    // Vars
    string leads_to_scene;
    string leads_to_passage;
    string passage_location;
    bool touching_player;
    int touchCounter;
    bool ready = false;
    
};

