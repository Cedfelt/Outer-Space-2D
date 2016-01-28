#pragma once

#include "cocos2d.h"
#include <vector>

using namespace std;
USING_NS_CC;

#define TILE_COUNT_X 400
#define TILE_COUNT_Y 400

class Level : public Node
{
public:
    
	TMXTiledMap *map;
    enum mapProps {
        TS = 8
    };
    short blocked [TILE_COUNT_X][TILE_COUNT_Y];
	void loadMap(const char* name);
	TMXTiledMap * getMap();
    TMXLayer walls;

	Point tileCoordinateToPosition(Point point);
	Point positionToTileCoordinate(Point point);
	vector<Rect> getCollisionTilesY(Point point, int direction);
	vector<Rect> getCollisionTilesX(Point point, int direction);
    int getBlocked(float x,float y, int w,int h);
    bool isTiledBlocked(unsigned int x,unsigned int y);
    TMXLayer *background;
    
	Level(void);
	virtual ~Level(void);
};