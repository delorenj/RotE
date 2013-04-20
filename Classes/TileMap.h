#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include <cocos2d.h>

using namespace cocos2d;

class TileMap : public cocos2d::CCTMXTiledMap {
public:
	static TileMap* create(const char* filename);

	CCPoint coordinatesAtPosition(CCPoint point);
	unsigned int getGIDAtPosition(CCPoint point);
};

#endif
