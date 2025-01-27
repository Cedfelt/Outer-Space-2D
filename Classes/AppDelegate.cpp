#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "outer_space_frame_work/Space_Level.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
  //set OpenGL context attributions,now can only set six attributions:
  //red,green,blue,alpha,depth,stencil
  GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

  GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
  // initialize director
  const uint32_t scale = 2;
  uint32_t res_x = scale*1136 / 2;
  uint32_t res_y = scale * 640 / 2;

  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();
  if (!glview) {
    glview = GLViewImpl::createWithRect("SpaceGame", Rect(0, 0, res_x , res_y ));
    director->setOpenGLView(glview);
  }

  director->getOpenGLView()->setDesignResolutionSize(res_x, res_y, ResolutionPolicy::SHOW_ALL);

  // turn on display FPS
  director->setDisplayStats(true);

  // set FPS. the default value is 1.0/60 if you don't call this
  director->setAnimationInterval(1.0 / 60);

  FileUtils::getInstance()->addSearchPath("res");

  // create a scene. it's an autorelease object
  // create a scene. it's an autorelease object
  UserDefault::getInstance()->setStringForKey("current_level", "level5.tmx");
  UserDefault::getInstance()->setIntegerForKey("resolution_factor", scale);
  UserDefault::getInstance()->flush();
  auto scene = Space_Level::createScene("small_scale_test.tmx");

  // run
  director->runWithScene(scene);

  return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
  Director::getInstance()->stopAnimation();

  // if you use SimpleAudioEngine, it must be pause
  // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
  Director::getInstance()->startAnimation();

  // if you use SimpleAudioEngine, it must resume here
  // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
