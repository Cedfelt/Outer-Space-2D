//
//  SpaceGameLayer.cpp
//  SpaceGame
//
//  Created by Anders on 13/10/15.
//
//


using namespace std;
#include "SpaceGameLayer.h"
#include "Physic.h"
//#include  "globals.h"
//#include "text_box.h"
//#include "SpacePig.h"
//#include "Damage_Zone.h"
//#include "brain_sucker.h"
#include "SimpleAudioEngine.h"
#include "Passage.h"
//#include "Zombie.h"
//#include "Flying_Blocker.h"
//#include "Spitting_Projectiles.h"
#include "Space_Level.h"
//#include "DeathRain.h"
//#include "MovingPlattform.h"
#include "Robot_Soldier.h"
//#include "Patrol_Ufo.h"
int counter;
SpaceGameLayer::SpaceGameLayer() {
  cocos2d::Director::getInstance()->getTextureCache()->removeAllTextures();
  ///////////////////////////////
  // SCHEDULE UPDATE FUNCTION
  this->schedule(schedule_selector(SpaceGameLayer::super_update));
  counter = 0;
}

void SpaceGameLayer::super_init(const char*map_name) {

  cocos2d::Layer::init();
  auto director = cocos2d::Director::getInstance();
  this->setScale(1);
  float sf = director->getContentScaleFactor();
  this->setAnchorPoint(cocos2d::Point(0, 0));

  ////////////////////////////////////
  // LEVEL-SETUP - GENERAL
  level = new Level();
  level->loadMap(map_name);
  this->addChild(level);
  level->setPosition(0, 0);
  // Special atribute layers
  auto foreground = level->map->getLayer("foreground");
  background = level->map->getLayer("background");
  background2 = level->map->getLayer("background2");
  background3 = level->map->getLayer("background3");
  if (foreground != nullptr) {
    level->getMap()->removeChild(foreground);
    this->addChild(foreground, 10);
    foreground->setScale(level->getScale());
  }

  ////////////////////////////////////
  // OUTSIDE OF SCREEN RECTANGLE - GENERAL - This rect is used to determine which go's are on/off-screen
  outside_screen = 1.5;
  float screnWidh = director->getWinSize().width*outside_screen;
  float screnHeight = director->getWinSize().height*outside_screen;
  float distanceX = (screnWidh - screnWidh / outside_screen) / 2;
  float distanceY = (screnHeight - screnHeight / outside_screen) / 2;
  screen.setRect(getPositionX() - distanceX, getPositionY() - distanceY, screnWidh, screnHeight);

  if (view_of_screen_rect) {
    screen_draw = cocos2d::DrawNode::create();
    cocos2d::Vec2 rectangle[4];
    rectangle[0] = cocos2d::Vec2(screen.getMinX(), screen.getMinY());
    rectangle[1] = cocos2d::Vec2(screen.getMaxX(), screen.getMinY());
    rectangle[2] = cocos2d::Vec2(screen.getMaxX(), screen.getMaxY());
    rectangle[3] = cocos2d::Vec2(screen.getMinX(), screen.getMaxY());
    screen_draw->drawPolygon(rectangle, 4, cocos2d::Color4F::RED, 1, cocos2d::Color4F::BLUE);
    screen_draw->setAnchorPoint(getAnchorPoint());
    addChild(screen_draw, 3);
  }


  ////////////////////////////////////
  // SPAWN GAME OBJACTS - SPECIFIC
  getObjects(sf);
  setViewPointCenter(hero->getCameraView());

  ////////////////////////////////////
  // MUSIC SETUP - SPECIFIC
  //auto mapGroup =level->getMap()->getProperties();
  //string track_name = mapGroup["music_track"].asString();
  auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
  /*if(track_name=="silence"){
      current_track = "silence";
      audio->stopBackgroundMusic();
  }
  else if(track_name!=current_track){
      current_track = track_name;
      audio->playBackgroundMusic(track_name.c_str(),true);
  }*/

}


void SpaceGameLayer::super_update(float delta) {
  for (int i = 0; i < SpaceGameLayer::gameObjects.size();i++) {
    // Remove Objects
    if (gameObjects.at(i)->removeObj) {
      AdvancedGameobject *removeObj = gameObjects.at(i);
      this->removeChild(gameObjects.at(i));
      gameObjects.eraseObject(removeObj);
      removeObj->release();
      continue;
    }

    // Handeling Gameobjects depening if they are on/off-screen
    if (goOnScreen(gameObjects.at(i))) {
      gameObjects.at(i)->setVisible(true);
      // GameObject is on screen and should be moved
      moveGameObject(gameObjects.at(i), level, delta);
    }
    else {
      gameObjects.at(i)->setVisible(false);
      // If gameObject should be moved even though is not on screeen, Then Move it!
      if (gameObjects.at(i)->moveWhenOutsideOfScreen) {
        moveGameObject(gameObjects.at(i), level, delta);
      }
    }
  }

  // Seperate Player Movement & Collision with other GO's
  moveGameObject(hero, level, delta);
  gameObjectCollision(&gameObjects, hero, delta);
  setViewPointCenter(Point(hero->getCameraView()));
  update_layers();
  updateOffScreenRect();
  checkExits();
  for (int i = 0;i < addToGameObjects.size();i++) {
    AdvancedGameobject *go = addToGameObjects.at(0);
    /* go->setPosition(go->start_x,go->start_y);
     addChild(go);
     gameObjects.pushBack(go);
     addToGameObjects.eraseObject(go);*/
  }
  for (int i = 0;i < addToGameObjects.size();i++) {
    addToGameObjects.popBack();
  }

}

void SpaceGameLayer::update_layers() {
  if (background != NULL)
    //background->setPosition(0.9*level->map->convertToNodeSpace(Point(0,0.)));
    int a = 1;
  if (background2 != NULL)
    //background2->setPosition(0.6*level->map->convertToNodeSpace(Point(0,0.)));
    int a = 1;
  if (background3 != NULL)
    //background3->setPosition(0.8*level->map->convertToNodeSpace(Point(0,0.)));
    int a = 1;
}

bool SpaceGameLayer::goOnScreen(AdvancedGameobject *obj) {
  //return screen.intersectsRect(obj->getShape());
  return true;
}

void SpaceGameLayer::updateOffScreenRect() {
  // Set size of the rectangle deactivating sprites of-screen
  float screnWidh = cocos2d::Director::getInstance()->getWinSize().width*outside_screen;
  float screnHeight = cocos2d::Director::getInstance()->getWinSize().height*outside_screen;
  float distanceX = (screnWidh - screnWidh / outside_screen) / 2;
  float distanceY = (screnHeight - screnHeight / outside_screen) / 2;
  float sx = convertToNodeSpace(cocos2d::Point(0, 0.)).x;
  float sy = convertToNodeSpace(cocos2d::Point(0, 0.)).y;
  // Limit Screen to Go Outside of Map
  if (sx < 0) {
    sx = 0;
  }
  if (sy < 0) {
    sy = 0;
  }
  screen.setRect(sx - distanceX, sy - distanceY, screnWidh, screnHeight);
  if (view_of_screen_rect) {
    screen_draw->setPosition(sx, sy);
  }
}

void SpaceGameLayer::getObjects(float sf) {

  const string objectName = "objects";
  auto objectGroup = level->getMap()->getObjectGroup(objectName);
  ValueVector obj = objectGroup->getObjects();
  for (int i = 0;i < obj.size();i++) {
    ValueMap vm = obj.at(i).asValueMap();
    int x = vm["x"].asInt();
    int y = vm["y"].asInt();
    float w = vm["width"].asInt();
    float h = vm["height"].asInt();
    x *= level->getScale();
    y *= level->getScale();;
    w *= level->getScale();;
    h *= level->getScale();;
    string name = vm["name"].asString();
    string type = vm["type"].asString();

    if (name == "player_start") {
      hero = new Hero(sf);
      load_player_stats();
      hero->setPosition(x, y);
      this->addChild(hero, 6);
      this->addChild(hero->player_sprite, 5);
      hero->addToGameObjects = &addToGameObjects;

    }

    if (name == "passage") {
      string sc = vm["to_scene"].asString();
      string ps = vm["to_passage"].asString();
      Passage *pass = new Passage(x, y, w, h, sc, ps);
      exits.pushBack(pass);
      pass->target = hero;
      this->addChild(pass, 6);
      this->setScale(sf);
    }

    if (name == "robot_soldier") {
      Robot_Soldier *fb = new Robot_Soldier();
      fb->setPosition(x, y);
      gameObjects.pushBack(fb);
      fb->target = hero;
      this->addChild(fb, 6);
    }

    /*if(name == "premade_textbox"){

        //PremadeTextBox *tb = new PremadeTextBox(x,y,w,h);
         tb->setPosition(x,y);
         gameObjects.pushBack(tb);
         this->addChild(tb,6);
    }
    if(name == "space_pig"){

        SpacePig *sp = new SpacePig();
        sp->setPosition(x,y);
        sp->setZone(w*(type=="zone"), h*(type=="zone"));
        gameObjects.pushBack(sp);
        this->addChild(sp,6);
        //sp->setScale(sf);

    }

    if(name == "flying_blocker"){
        Flying_Blocker *fb = new Flying_Blocker();
        fb->setPosition(x,y);
        gameObjects.pushBack(fb);
        fb->target = hero;
        this->addChild(fb,6);
    }

    if(name == "flying_blocker"){
        Flying_Blocker *fb = new Flying_Blocker();
        fb->setPosition(x,y);
        gameObjects.pushBack(fb);
        fb->target = hero;
        this->addChild(fb,6);
    }

   

    if(name == "patrol_ufo"){
        Patrol_Ufo *fb = new Patrol_Ufo();
        fb->setPosition(x,y);
        gameObjects.pushBack(fb);
        fb->target = hero;
        this->addChild(fb,6);
    }

    if(name == "spitting_projectiles"){

        Spitting_Projectiles *sp = new Spitting_Projectiles;
        sp->setPosition(x+0.5*w,y+0.5*h);
        sp->setZone(w*(type=="zone"), h*(type=="zone"));
        gameObjects.pushBack(sp);
        this->addChild(sp,6);
        sp->setScale(sf);
        sp->target = hero;
        sp->addToGameObjects = &addToGameObjects;
        sp->currentGameLayer = this;

    }
    if(name == "damage"){

        Damage_Zone *dmg = new Damage_Zone(x,y,w,h);
        dmg->setPosition(x,y);
        gameObjects.pushBack(dmg);
        this->addChild(dmg,6);
        this->setScale(sf);
    }
    if(name == "brain_sucker"){
        BrainSucker *bs = new BrainSucker();
        bs->setPosition(x,y);
        gameObjects.pushBack(bs);
        bs->target = hero;
        this->addChild(bs,6);
        this->setScale(sf);
    }




    if(name == "death_rain"){
        DeathRain *dr = new DeathRain(w*(type=="zone"),h*(type=="zone"));
        dr->setPosition(x,y);
        dr->setZone(w*(type=="zone"), h*(type=="zone"));
        gameObjects.pushBack(dr);
        this->addChild(dr,6);
        dr->setScale(sf);
        dr->target = hero;
        dr->addToGameObjects = &addToGameObjects;
        dr->currentGameLayer = this;
    }
    if(name == "moving_plattform"){
        MovingPlattform *mp = new MovingPlattform(x, y);
        this->addChild(mp,6);
        mp->setScale(sf);
        mp->target = hero;
        mp->addToGameObjects = &addToGameObjects;
        gameObjects.pushBack(mp);
        mp->currentGameLayer = this;
    }
*/
  }
  if (hero->nextEntrance != "") {
    for (int i = 0;i < exits.size();i++) {
      if (hero->nextEntrance == exits.at(i)->idString) {
        hero->toucing_exit = true;
        hero->setPositionX(exits.at(i)->getPositionX());
        hero->setPositionY(exits.at(i)->getPositionY());
      }
    }
  }
  for (int i = 0;i < gameObjects.size();i++) {
    gameObjects.at(i)->target = hero;
    gameObjects.at(i)->player_screen = &screen;
    gameObjects.at(i)->map_data = this->level;
    gameObjects.at(i)->addToGameObjects = &addToGameObjects;
  }
  for (int i = 0;i < exits.size();i++) {
    exits.at(i)->target = hero;
  }
}

void SpaceGameLayer::setViewPointCenter(cocos2d::Point position) {
  // NO PROBS HERE
  Size winSize = Director::getInstance()->getWinSize() / 1;//Scene Scale Factor

  int x = MAX(position.x, winSize.width / 2);
  int y = MAX(position.y, winSize.height / 2);
  x = MIN(x, (level->getScale()*level->getMap()->getMapSize().width * this->level->getMap()->getTileSize().width) - winSize.width / 2);
  y = MIN(y, (level->getScale()*level->getMap()->getMapSize().height * level->getMap()->getTileSize().height) - winSize.height/2);
  Point actualPosition = Point(x, y);

  Point centerOfView = Point(winSize.width / (2), winSize.height / (2));
  centerOfView.subtract(actualPosition);// ccpSub(centerOfView, actualPosition);
  this->setPosition(centerOfView);
}



SpaceGameLayer::~SpaceGameLayer() {
  /*this->cleanup();
  cocos2d::log("Dealocate SpaceGameLayer\n");
  //unscheduleAllSelectors();
  stopAllActions();
  //removeFromParentAndCleanup(true);

  for(int i = 0;i<gameObjects.size();i++){
      gameObjects.at(i)->release();
  }
  for(int i = 0;i<exits.size();i++){
      exits.at(i)->release();
  }
  //hero->release();
  //auto aud = CocosDenshion::SimpleAudioEngine::getInstance();
  //aud->end();
  //level->release();*/

}

void SpaceGameLayer::save_player_stats() {
  // NO PROBS HERE
  UserDefault::getInstance()->setFloatForKey("velocity_y", (hero->vsp));
  UserDefault::getInstance()->setFloatForKey("velocity_x", (hero->hsp));
  UserDefault::getInstance()->setFloatForKey("fule", (hero->fule));
  UserDefault::getInstance()->setStringForKey("entrance", hero->nextEntrance);
  UserDefault::getInstance()->flush();
}
void SpaceGameLayer::load_player_stats() {
  // NO PROBS HERE
  hero->vsp = UserDefault::getInstance()->getFloatForKey("velocity_y");
  hero->hsp = UserDefault::getInstance()->getFloatForKey("velocity_x");
  hero->fule = 100;//UserDefault::getInstance()->getFloatForKey("fule");
  hero->nextEntrance = UserDefault::getInstance()->getStringForKey("entrance");
  UserDefault::getInstance()->flush();

}

void SpaceGameLayer::checkExits() {
  if (hero->go_to_nextScene&&level_counter >= 5) {
    hero->go_to_nextScene = false;
    cocos2d::log("go_to_next_Scene");
    // SELECT SCENE
    hero->nextEntrance = hero->go_to_passage;
    UserDefault::getInstance()->setStringForKey("current_level", hero->go_to_scene);
    UserDefault::getInstance()->flush();
    save_player_stats();
    auto scene = Space_Level::createScene("Whoop");
    Director::getInstance()->replaceScene(scene);
  }
  else {
    hero->go_to_nextScene = false;
    level_counter = 5;
  }

}
