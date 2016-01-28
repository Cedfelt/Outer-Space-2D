//
//  Advanced_Gameobject.cpp
//  SpaceGame
//
//  Created by Anders on 06/10/15.
//
//


#include "Advanced_Gameobject.h"
AdvancedGameobject::AdvancedGameobject() {
  HP = 3;
  this->schedule(CC_SCHEDULE_SELECTOR(AdvancedGameobject::updateGameObject));
  //player_sprite->setAnchorPoint(Point(0,0));
  this->setAnchorPoint(Point(0, 0));
  removeObj = false;
  animationCache = AnimationCache::getInstance();
  
}

void AdvancedGameobject::setPos(float x, float y){
  HP = 3;
  this->schedule(CC_SCHEDULE_SELECTOR(AdvancedGameobject::updateGameObject));
  //player_sprite->setAnchorPoint(Point(0,0));
  this->setAnchorPoint(Point(0, 0));
  removeObj = false;
  animationCache = AnimationCache::getInstance();
}

void AdvancedGameobject::addHitboxToSprite(){
    hitboxModX = hitBoxWidth*getAnchorPoint().x;
    hitboxModY = hitBoxHeight*getAnchorPoint().y;
    
}

void AdvancedGameobject::colide(AdvancedGameobject *other_obj){
}


void AdvancedGameobject::addAnimation(std::string fileName,std::string animation_name,int start,int end,float animSpeed){
    cocos2d::Vector<cocos2d::SpriteFrame*> animFrames(end-start);
    std::string fileFormat = "%i.png";
    std::string fileName_update = fileName+fileFormat;
    
    for(int i = start;i<=end;i++){
        auto name = cocos2d::String::createWithFormat(fileName_update.c_str(), i);
        auto frame = spriteFrameCache->getSpriteFrameByName(name->getCString());
        frame->getTexture()->setAliasTexParameters();
        animFrames.pushBack(frame);
    }
    animationCache->addAnimation(cocos2d::Animation::createWithSpriteFrames(animFrames,animSpeed), animation_name);
}

void AdvancedGameobject::flash(int times, float interval){
    this->schedule(CC_SCHEDULE_SELECTOR(AdvancedGameobject::flash_sprite),interval,times,0);
}

void AdvancedGameobject::flash_sprite(float delta){
    cocos2d::Color3B c = player_sprite->getColor();
    if(c !=c.RED){
        player_sprite->setColor(c.RED);
    }
    else{
        player_sprite->setColor(c.WHITE);
    }
}

float AdvancedGameobject::angleToTarget(AdvancedGameobject *other_obj){
    float b = (other_obj->getPositionX()+other_obj->hitBoxWidth*other_obj->getAnchorPoint().x)-(getPositionX()+hitboxModX);
    float a = (other_obj->getPositionY()+other_obj->hitBoxWidth*other_obj->getAnchorPoint().y) -(getPositionY()+hitboxModY);
    float angle = atan((double)((a/b)));
    if(b<0){
        angle+=3.14159265358979323846;// PI
    }
    return angle;  
}

bool AdvancedGameobject::hurt(int dmg,AdvancedGameobject *other_obj){
    if(!imune){
        this->imune = true;
        this->HP-=dmg;
        this->schedule(CC_SCHEDULE_SELECTOR(AdvancedGameobject::resetImune),1);
        if(idString=="player"){
            auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
            audio->setEffectsVolume(0.2f);
            audio->playEffect("Cry.aif");
            if(HP>0){
                AdvancedGameobject::flash(5,0.1);
            }
        }
    }
    return true;
}



void AdvancedGameobject::resetImune(float delta){
    imune = false;
}

void AdvancedGameobject::setZone(float w,float h){
    zone.setRect(getPositionX(), getPositionY(), w, h);
}

void AdvancedGameobject::drawHitbox(){
    auto ap = this->getAnchorPoint();
   
    cocos2d::DrawNode *rectNode = cocos2d::DrawNode::create();
    cocos2d::Vec2 rectangle[4];
    rectangle[0] = cocos2d::Vec2(0-ap.x*hitBoxWidth,0-ap.y*hitBoxHeight);
    rectangle[1] = cocos2d::Vec2(hitBoxWidth-ap.x*hitBoxWidth,0-ap.y*hitBoxHeight);
    rectangle[2] = cocos2d::Vec2(hitBoxWidth-ap.x*hitBoxWidth,hitBoxHeight-ap.y*hitBoxHeight);
    rectangle[3] = cocos2d::Vec2(0-ap.x*hitBoxWidth,hitBoxHeight-ap.y*hitBoxHeight);
    rectNode->drawPolygon(rectangle, 4, cocos2d::Color4F::RED, 1, cocos2d::Color4F::BLUE);
    rectNode->setAnchorPoint(getAnchorPoint());
    addChild(rectNode,-1);
}

void AdvancedGameobject::updateGameObject(float delta){
    // Virtual Member to other class, dont delete
}

cocos2d::Rect AdvancedGameobject::getShape(){
    return cocos2d::Rect(getPositionX()-hitboxModX, getPositionY()-hitboxModY, hitBoxWidth*getScale(), hitBoxHeight*getScale());
}

bool AdvancedGameobject::targetIsVisible(AdvancedGameobject *tar){
  cocos2d::Point start_p = getPosition();
  cocos2d::Point goal_p = tar->getPosition();
    float ang = angleToTarget(tar);
    float distance = std::abs(start_p.getDistance(goal_p));
    if(distance>visionRange){
        return false;
    }
    
    if(right){
        if(ang*57.296>35)
            return false;
    }
    else if(left){
        if(ang*57.296<145)
        return false;
    }
    float next_x;
    float next_y;
    float amount_of_x = std::cos(ang);
    float amount_of_y = std::sin(ang);
    /*for(int i=1;i<distance;i = i+map_data->getScale()*map_data->map->getTileSize().width){
        start_p.set(start_p.x+map_data->getScale()*map_data->map->getTileSize().width*amount_of_x, start_p.y+i*amount_of_y);
        if(map_data->isTiledBlocked((int)start_p.x, start_p.y+(1.5*map_data->getScale()*map_data->map->getTileSize().width))){
            return false;
        }
    }*/
    return true;
    
}

void AdvancedGameobject::setAnimation(std::string anim){
    if(prevAnimation!=anim){
        player_sprite->stopAllActions();
        player_sprite->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animationCache->getAnimation(anim))));
    }
    prevAnimation = anim;
}

AdvancedGameobject::~AdvancedGameobject(){
     cocos2d::log("Dealocate AdvGo\n");
    this->cleanup();
    spriteFrameCache->removeUnusedSpriteFrames();
    spriteFrameCache->destroyInstance();
}











