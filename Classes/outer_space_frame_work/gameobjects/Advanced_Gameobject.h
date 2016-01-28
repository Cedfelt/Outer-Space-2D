//
//  Advanced_Gameobject.hpp
//  SpaceGame
//
//  Created by Anders on 06/10/15.
//
//

#pragma once
//#include "Level.h"
#include "cocos2d.h"
#include <algorithm>
#include <string>
#include <vector>
#include "SimpleAudioEngine.h"
#include "outer_space_frame_work/logic_and_structure/level.h"
USING_NS_CC;

class AdvancedGameobject : public cocos2d::Layer
{

public:
    // FUNCTIONS
    AdvancedGameobject();
    ~AdvancedGameobject();
    virtual void colide(AdvancedGameobject *other_obj);
    virtual void addAnimation(std::string fileName,std::string animation_name,int start,int end,float animSpeed);
    virtual void updateGameObject(float delta);
    virtual float angleToTarget(AdvancedGameobject *other_obj);
    virtual bool hurt(int dmg,AdvancedGameobject *other_obj);
    virtual void resetImune(float delta);
    virtual void flash(int times, float interval);
    virtual void flash_sprite(float delta);
    virtual void drawHitbox();
    virtual void addHitboxToSprite();
    virtual void setZone(float w,float h);
    virtual void setPos(float x,float y);
    virtual cocos2d::Rect getShape();
    virtual bool targetIsVisible(AdvancedGameobject *target);
    float visionRange = 256;// Tiles
    virtual void setAnimation(std::string anim);

    
    // VARS
    float hitboxModX;
    float hitboxModY;
    bool toucing_exit;
    bool go_to_nextScene;
    bool jump;
    bool moveWhenOutsideOfScreen = false;
    cocos2d::Rect *player_screen;
    std::string go_to_scene;
    std::string go_to_passage;
    float start_x;
    float start_y;
    Level *map_data;
    typedef enum tAI_STATE {
        AI_PATROLLING,
        AI_APPROACH,
        AI_ALERTED,
        AI_ATTACK,
        AI_DEAD,
        AI_NO_MORE_UPDATES,
    }STATE;
    STATE AI_STATE;
    
    // Deallocate this
    cocos2d::Sprite *player_sprite;
    std::string idString;
    //Point position;
    cocos2d::Rect zone;
    
    
    cocos2d::SpriteFrameCache *spriteFrameCache;
    cocos2d::AnimationCache *animationCache;
    // OBJECT SETTINGS
    float maxWalkSpeed = 75;
    float maxFlySpeed = 150;
    float groundAcceleration = 8;
    float groundDeaccleration = 0.65;
    cocos2d::Point velocity;
    float airAcceleration = 8;
    float airDeacceleration = 0.994;
    float animationSpeed = 0.2;
    int spriteWidth = 64;
    int spriteHeight = 64;
    int E_LastAnimation;
    int E_NextAnimation;
    std::string prevAnimation;
    int HP;
    bool imune;
    bool on_platform = false;
    AdvancedGameobject *target;
    bool left = false;
    bool right = false;
    float hitBoxWidth = 0;
    float hitBoxHeight = 0;
    float vsp = 0;
    float hsp = 0;
    unsigned int grounded;
    bool check_for_gravity = true;
    bool removeObj;
    cocos2d::Vector<AdvancedGameobject*> *addToGameObjects;
    cocos2d::Layer *currentGameLayer;
    
    
    
    
    enum Animation_Enums{
        IDLE_LEFT,
        IDLE_RIGHT,
        LEFT,
        RIGHT,
        BOOST_LEFT,
        BOOST_RIGHT,
        JUMP_LEFT,
        JUMP_RIGHT,
        FALL_LEFT,
        FALL_RIGHT,
        UP,
        DOWN,
        dieR,
        dieL
    };

    std::string animatioNames[14] = {
      "IdleL",
      "IdleR",
      "WalkL",
      "WalkR",
      "dashL",
      "dashR",
      "flyL",
      "flyR",
      "fallL",
      "fallR",
      "UP",
      "DOWN",
      "dieR",
      "dieL"
    };
};

