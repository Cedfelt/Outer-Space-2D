#pragma once

#include "cocos2d.h"
//#include "Hero.h"
#include "ui/CocosGUI.h"
#include "SpaceGameLayer.h"
using namespace std;
class Space_Level : public SpaceGameLayer
{
public:
    // MY STUFF
    static cocos2d::Scene* createScene(const string m_n);
    virtual bool init();
    CREATE_FUNC(Space_Level);
    ~Space_Level();
};


