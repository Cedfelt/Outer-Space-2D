//
//  SpaceGameLayer.hpp
//  SpaceGame
//
//  Created by Anders on 13/10/15.
//
//
#pragma once

#include "Advanced_Gameobject.h"
#include "Hero.h"
#include "Level.h"


using namespace std;
//using namespace cocos2d;

static string current_track;
class SpaceGameLayer : public cocos2d::Layer
{

public:
    virtual void getObjects(float sf);
    virtual void setViewPointCenter(cocos2d::Point position);
    virtual void checkExits();
    virtual void super_update(float delta);
    virtual void super_init(const char*map_name);
    virtual void save_player_stats();
    virtual void load_player_stats();
    virtual bool goOnScreen(AdvancedGameobject *obj);
    virtual void updateOffScreenRect();
    virtual void update_layers();
    cocos2d::Rect screen;
    SpaceGameLayer();
    ~SpaceGameLayer();
    cocos2d::Vector<AdvancedGameobject*> gameObjects;
    cocos2d::Vector<AdvancedGameobject*> addToGameObjects;
    cocos2d::Vector<AdvancedGameobject*> exits;
     cocos2d::TMXLayer *background;
     cocos2d::TMXLayer *background2;
     cocos2d::TMXLayer *background3;
    int level_counter = 0;
    float outside_screen;
    cocos2d::DrawNode *screen_draw;
    bool view_of_screen_rect = false;
    
    Hero *hero;
    Level *level;
    
};

