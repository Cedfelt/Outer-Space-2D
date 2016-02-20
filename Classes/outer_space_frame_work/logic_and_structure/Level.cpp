#include "Level.h"
#include "globals.h"
#include <iostream>
using namespace std;
USING_NS_CC;


void Level::loadMap(const char* mapname) {

	map = TMXTiledMap::create(mapname);
    map->setAnchorPoint(Point(0,0));
    // Normal_Walls
    TMXLayer *tempLayer = &walls;
    tempLayer = map->getLayer("walls");
    int mapheight = (int)map->getMapSize().height-1;
    int mapWidth = (int)map->getMapSize().width-1;
    for(int x = 0;x<mapWidth;x++){
        for(int y = 0;y<mapheight;y++){
            int gid = tempLayer->getTileGIDAt(Point(x, mapheight - (y)));
            if(gid>0){
                blocked[x][y] = 2;
            }
            else{
                blocked[x][y] = -1;
            }
        }
    }
    // Thin Walls
    tempLayer = map->getLayer("walls_thin");
    if(tempLayer!=NULL){
        for(int x = 0;x<mapWidth;x++){
            for(int y = 0;y<mapheight;y++){
                int gid = tempLayer->getTileGIDAt(Point(x, mapheight - (y)));
                if(gid>0){
                    blocked[x][y] = 1;
                }
            }
        }
    }
    this->setScale(3);
    map->setScale(1);
    background = map->getLayer("Background");
    this->addChild(map);
    
    //background = map->getLayer("Background");
}

TMXTiledMap * Level::getMap() {
    map->setAnchorPoint(Point(0,0));
    return map;
}

Point Level::tileCoordinateToPosition(Point point) {

	float x = floor(point.x * map->getTileSize().width * SCALE_FACTOR);
	float y = floor(point.y * map->getTileSize().height * SCALE_FACTOR);

	return Point(x, y);

}

Point Level::positionToTileCoordinate(Point point) {

	float x = floor(point.x / (map->getTileSize().width * SCALE_FACTOR));
	float y = floor(point.y / (map->getTileSize().height * SCALE_FACTOR));

	return Point((int)x, (int)y);
}

vector<Rect> Level::getCollisionTilesY(Point point, int direction) {

	vector<Rect> list;

	TMXLayer *walls = map->getLayer("walls");

	int mapheight = (int)map->getMapSize().height - 1;

	for (int b = -1; b < 2; b++) {

		Sprite *tile = walls->getTileAt(Point((int)point.x + b, mapheight - ((int)point.y + direction)));

		if (tile != NULL) {

			Rect tileRect = Rect();

			Point tmp = walls->positionAt(Point((int)point.x + b, mapheight - ((int)point.y + direction)));
			tileRect.setRect(tmp.x * SCALE_FACTOR, tmp.y * SCALE_FACTOR, map->getTileSize().width * SCALE_FACTOR, map->getTileSize().height * SCALE_FACTOR);

			list.push_back(tileRect);
		}

	}

	return list;

}

vector<Rect> Level::getCollisionTilesX(Point point, int direction) {

	vector<Rect> list;

	TMXLayer *walls = map->getLayer("walls");

	int mapheight = (int)map->getMapSize().height - 1;

	for (int b = -1; b < 2; b++) {

		Sprite *tile = walls->getTileAt(Point((int)point.x + direction, mapheight - ((int)point.y + b)));

		if (tile != NULL) {

			Rect tileRect = Rect();

			Point tmp = walls->positionAt(Point((int)point.x + direction, mapheight - ((int)point.y + b)));
			tileRect.setRect(tmp.x * SCALE_FACTOR, tmp.y * SCALE_FACTOR, map->getTileSize().width * SCALE_FACTOR, map->getTileSize().height * SCALE_FACTOR);

			list.push_back(tileRect);
		}

	}

	return list;
}

int Level::getBlocked(float x,float y, int w,int h){
    //TMXLayer *walls = map->getLayer("walls");
    /*float sf = this->getScale();
    Rect player_rect;
    player_rect.setRect(x, y, w, h);
     
    int range = 16;
    float x_rect = (x/(TS*sf)-range/2.0);
    float y_rect = (y/(TS*sf)-range/2.0);
    int xRange = x_rect +range;
    int yRange = y_rect +range;
    Rect tile_rect;
    
    
    int mapWidth = (int)map->getMapSize().width;
    int mapHeight = (int)map->getMapSize().height;
    
    if((x_rect+xRange)>mapWidth*TS*sf){
        xRange=mapWidth-x_rect;
    }
    if((y_rect+yRange)>mapHeight*TS*sf){
        xRange=mapHeight-y_rect;
    }
    if(x_rect<0){
        x_rect=0;
    }
    if(y_rect<0){
        y_rect=0;
    }
    for(int xx = x_rect; xx<xRange;xx++){
        for(int yy = y_rect; yy<yRange;yy++){
            
            if(blocked[xx][yy]==0){
                continue;
            }
            else if (blocked[xx][yy]==2){
                tile_rect.setRect(xx*TS*sf,yy*TS*sf,TS*sf,TS*sf);
            }
            else{
                tile_rect.setRect(xx*TS*sf,yy*TS*sf,sf*TS,1);
            }
            int mapheight = (int)map->getMapSize().height - 1;
            bool cond1 = tile_rect.intersectsRect(player_rect);
            int val = blocked[xx][yy];
            bool cond2 = (val>0);
            //bool cond3 = walls->getTileAt(Point(xx/TS, mapheight - (yy/TS)))!=NULL;
            if(xx<0||xx>=mapWidth||yy<0||yy>=mapheight){
                assert("ERROR");
            }
            else if(cond1&&cond2){
                return 1;
            }
        }
    }
    return 0;*/
    // Colission 2
    float sf = this->getScale();
    Rect player_rect;
    Rect tile_rect;
    player_rect.setRect(x, y, w, h);
    int x_min = (x/(TS*sf));
    int x_max = ((x+w)/(TS*sf));
    int y_min = (y/(TS*sf))-1;
    int y_max = ((y+h)/(TS*sf))-1;
    int xRange = x_min + x_max-x_min+1;
    int yRange = y_min + y_max-y_min+2;
    for(int xx = x_min; xx<xRange;xx++){
        for(int yy = y_min; yy<yRange;yy++){
            
            if(blocked[xx][yy]==-1){
                continue;
            }
            else if (blocked[xx][yy]==2){
                tile_rect.setRect(xx*TS*sf,yy*TS*sf,TS*sf,TS*sf);
            }
            else if(blocked[xx][yy]==1){
                tile_rect.setRect(xx*TS*sf,yy*TS*sf,sf*TS,1);
            }
    
            bool cond1 = tile_rect.intersectsRect(player_rect);
            if(cond1){
                return 1;
            }
        }
    }
    return 0;
}

bool Level::isTiledBlocked(unsigned int x,unsigned int y){
    float sf = this->getScale();
    x = x/(TS*sf);
    y = y/(TS*sf);
    
    if(x>=TILE_COUNT_X){
        return true;
    }
    if(y>=TILE_COUNT_Y){
        return true;
    }
    return (blocked[x][y]==2);
    
}


Level::Level(void)
{
}

Level::~Level(void)
{
    cocos2d::log("Dealocate LEVEL\n");
    
}