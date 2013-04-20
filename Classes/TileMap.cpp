#include "TileMap.h"

TileMap* TileMap::create(const char* filename) {
	TileMap* pTM = new TileMap();
	if(pTM && pTM->initWithTMXFile(filename)) {
		pTM->autorelease();
		return pTM;
	}
	CC_SAFE_DELETE(pTM);
	return NULL;
}

CCPoint TileMap::coordinatesAtPosition(CCPoint point) {
	return ccp((int)(point.x / this->getTileSize().width), (int)(this->getMapSize().height - (point.y / this->getTileSize().height)));
}

unsigned int TileMap::getGIDAtPosition(CCPoint point) {
	CCPoint coord = coordinatesAtPosition(point);
	CCTMXLayer* layer = dynamic_cast<CCTMXLayer*>(getChildByTag(0));
	return layer->tileGIDAt(coord);	
}