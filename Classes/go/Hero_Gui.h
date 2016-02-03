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
#include "outer_space_frame_work/gameobjects/Advanced_Gameobject.h"
#include "outer_space_frame_work/gameobjects/Hero.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
class Hero_Gui : public AdvancedGameobject{
public:
    Hero_Gui();
    void colide(AdvancedGameobject *other_obj);
    ~Hero_Gui();
    void updateGameObject(float delta);
    // GUI-component
    cocos2d::ui::Button *button_left;
    cocos2d::ui::Button *button_right;
    Label *hp_label;
    cocos2d::Color4B colors[4] = { cocos2d::Color4B::RED,cocos2d::Color4B::YELLOW,cocos2d::Color4B::GREEN,cocos2d::Color4B::GREEN };
    Hero *hero;
};




