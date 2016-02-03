#pragma once

#include "cocos2d.h"
//#include "Player.h"
#include <algorithm>
#include <string>
#include <vector>
#include "outer_space_frame_work/gameobjects/Advanced_Gameobject.h"
#include "outer_space_frame_work/gameobjects/SoundFx.h"
#include "ui/CocosGUI.h"
//#include "Guided_Misile.h"


using namespace std;
USING_NS_CC;

class Hero : public AdvancedGameobject
{
public:
  // FUNCTIONS
  Hero(float sf);
  void updateGameObject(float interval);
  void setupAnimation(void);
  virtual bool onTouchBegan(const std::vector<Touch*>&, Event*);
  virtual void onTouchEnded(const std::vector<Touch*>&, Event*);
  virtual void onTouchMoved(const std::vector<Touch*>&, Event*);
  virtual void onTouchCancelled(const std::vector<Touch*>&, Event*);
  void updateAnimationInterrupt(float delta);
  //void addAnimation(string fileName,string animation_name,int start,int end);
  void updateAnimation(int lastAnimation, int newAnimation);
  void addStdMovement(float delta);
  void addJetPackMovement(float delta);
  void addDashMovement(float delta);
  void tapCounter(float delta);
  unsigned short leftCounter;
  unsigned short rightCounter;
  bool hurt(int dmg, AdvancedGameobject *other_obj);

  Point getCameraView();
  ~Hero();


  //FIELDS
  int direction;
  bool jetPack = false;
  float boostPower;
  int lastDir = RIGHT;
  Point velocity;
  float throtle;
  //Guided_Misile *misile;
  bool guided_misile = false;
  float cameraOffset;
  float fule_max;
  float fule;
  float fule_consume_rate;
  float touch_timer;
  float double_touch;
  float dash_timer;
  float dash_timer_max;
  float dash_accelaration;
  float cool_down_timer;
  float cool_down_timer_max;
  bool landed;
  bool alternative_controll = true;
  bool first_boost = false;
  string nextEntrance;
  bool attacking = false;
  float throtle_amount;
  float left_dash = 0;
  float right_dash = 0;
  float dash_max = 1;
  float jetpack_swipe = 50;

  SoundFx *sFx1;// Attack boost
  SoundFx *sFx2; // Pulsating
};

