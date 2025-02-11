#include "libtcod.hpp"
#include "Map.hpp"
#include "Actor.hpp"
#include "Engine.hpp"

class BspListener : public ITCODBspCallback {
private:
	Map &map; //map to dig
	int roomNum; //room number
	int lastx, lasty; //center of last room

public:
	BspListener(Map &map) : map(map), roomNum(0) {}
	bool visitNode(TCODBsp *node, void *userData) {
		if(node->isLeaf()) {
			int x,y,w,h;
			//dig room
			TCODRandom *rng=TCODRandom::getInstance();
			w=rng->getInt(ROOM_MIN_SIZE, node->w-2);
			h=rng->getInt(ROOM_MIN_SIZE, node->h-2);
			x=rng->getInt(node->x+1, node->x+node->w-w-1);
			y=rng->getInt(node->y+1, node->y+node->h-h-1);
			map.createRoom(roomNum == 0, x, y ,x+w-1, y+h-1);
			if(roomNum != 0) {
				//dig from last room
				map.dig(lastx,lasty,x+w/2,lasty);
				map.dig(x+w/2,lasty,x+w/2,y+h/2);
			}
			lastx=x+w/2;
			lasty=y+h/2;
			roomNum++;
		}
		return true;
	}
};

Map::Map(int width, int height) : width(width),height(height) {
	tiles=new Tile[width*height];
	TCODBsp bsp(0,0,width,height);
	bsp.splitRecursive(NULL,8,ROOM_MAX_SIZE,ROOM_MAX_SIZE,1.5f,1.5f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener,NULL);
}

Map::~Map() {
	delete [] tiles;
}

bool Map::isWall(int x, int y) const {
	return !tiles[x+y*width].canWalk;
}

void Map::dig(int x1, int y1, int x2, int y2) {
	if(x2 < x1) {
		int tmp=x2;
		x2=x1;
		x1=tmp;
	}
	if(y2 < y1) {
		int tmp=y2;
		y2=y1;
		y1=tmp;
	}
	for(int tilex=x1; tilex <= x2; tilex++) {
		for(int tiley=y1; tiley <= y2; tiley++) {
			tiles[tilex+tiley*width].canWalk=true;
		}
	}
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2) {
	dig(x1,y1,x2,y2);
	if(first) {
		engine.player->x=(x1+x2)/2;
		engine.player->y=(y1+y2)/2;
	} else {
		TCODRandom *rng=TCODRandom::getInstance();
		if(rng->getInt(0,3)==0) {
			engine.actors.push(new Actor((x1+x2)/2,(y1+y2)/2,'@',TCODColor::yellow));
		}
	}
}

void Map::render() const {
	static const TCODColor darkWall(0,0,100);
	static const TCODColor darkGround(50,50,150);

	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			TCODConsole::root->setCharBackground(x,y,isWall(x,y) ? darkWall : darkGround);
		}
	}
}


