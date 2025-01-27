//
//  SpaceGameLayer.hpp
//  SpaceGame
//
//  Created by Anders on 13/10/15.
//
//
#pragma once

#include "outer_space_frame_work/gameobjects/Advanced_Gameobject.h"
#include "outer_space_frame_work/gameobjects/Hero.h"
#include "outer_space_frame_work/logic_and_structure/Level.h"


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
  virtual void super_init(const char*map_name, uint32_t res_scale);
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
  cocos2d::Vector<cocos2d::TMXLayer*> background_layers;
  int level_counter = 0;
  float outside_screen;
  cocos2d::DrawNode *screen_draw;
  bool view_of_screen_rect = false;

  Hero *hero;
  Level *level;

};

