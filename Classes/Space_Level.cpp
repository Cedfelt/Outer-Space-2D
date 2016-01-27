

//#include "Hero.h"
//#include "Level.h"
//#include "globals.h"
#include <stdlib.h>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
//#include "Physic.h"
//#include "Zombie.h"
#include "Space_Level.h"
using namespace std;



cocos2d::Scene* Space_Level ::createScene(string m_n)
{
  cocos2d::Director::getInstance()->setProjection(cocos2d::Director::Projection::_2D);
    auto scene = cocos2d::Scene::create();
    auto layer = Space_Level::create();
    scene->setAnchorPoint(cocos2d::Point(0.0,0.0));
    scene->addChild(layer);
    scene->setScale(1);// Adjust depending on screen-size
    return scene;
    
}

bool Space_Level::init()
{
    super_init(cocos2d::UserDefault::getInstance()->getStringForKey("current_level").c_str());
    return true;
}

Space_Level::~Space_Level()
{
    cocos2d::log("Dealocate Space_Level\n");
    this->cleanup();
    this->removeAllChildrenWithCleanup(true);
    
}



