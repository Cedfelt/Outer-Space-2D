//
//  Hero_Gui.cpp
//  SpaceGame
//
//  Created by Anders on 10/01/16.
//
//

#include "Hero_Gui.h"
#include "cocos2d.h"
#define GUI

Hero_Gui::Hero_Gui() {
  hsp = 0;
  hitBoxHeight = 32;
  hitBoxWidth = 32;
  addHitboxToSprite();
  this->setScale(1);
  HP = 3;
  check_for_gravity = false;

  //////////////////////////////
  // JETPACK BUTTONS
  button_left = cocos2d::ui::Button::create("jetpack_logo0.png", "pink_sqr.png", "jetpack_logo1.png");
  button_left->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    switch (type)
    {
    case ui::Widget::TouchEventType::BEGAN:
      break;
    case ui::Widget::TouchEventType::ENDED:
      cocos2d::log("BUTTON PRESSED");
      if (true) {
        button_left->setBright(true);
      }
      else {
        button_left->setBright(false);
      }
      break;
    default:
      break;
    }
  });
#ifdef GUI
  this->addChild(button_left, 6);
#endif

  button_right = cocos2d::ui::Button::create("jetpack_logo0.png", "pink_sqr.png", "jetpack_logo1.png");
  button_right->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    switch (type)
    {
    case ui::Widget::TouchEventType::BEGAN:
      break;
    case ui::Widget::TouchEventType::ENDED:
      cocos2d::log("BUTTON PRESSED");
      if (true) {
        button_right->setBright(true);
      }
      else {
        button_right->setBright(false);
      }
      break;
    default:
      break;
    }
  });

#ifdef GUI
  this->addChild(button_right, 6);
#endif

  hp_label = Label::createWithTTF("FUEL: 100 %", "fonts/Marker Felt.ttf", 24);
  hp_label->setTextColor(colors[int((0))]);
  // position the label on the center of the screen
#ifdef GUI
  this->addChild(hp_label, 6);
#endif

}


void Hero_Gui::updateGameObject(float delta) {
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
#ifdef GUI
  button_left->setPosition(this->convertToNodeSpace(Point(0.05*winSize.width, 0.8*winSize.height)));
  button_right->setPosition(this->convertToNodeSpace(Point(0.95*winSize.width, 0.8*winSize.height)));
  string fule_string = std::to_string(int(hero->fule));
  fule_string = "FULE: " + fule_string + " %";
  hp_label->setTextColor(colors[int((hero->fule / 33.3))]);
  hp_label->setString(fule_string);
  hp_label->setPosition(this->convertToNodeSpace(Point(0.05*winSize.width, 0.7*winSize.height)));
#endif
}

void Hero_Gui::colide(AdvancedGameobject *other_obj) {

}

Hero_Gui::~Hero_Gui() {

}