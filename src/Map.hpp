static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;


struct Tile {
	bool canWalk; //can we walk through?
	Tile() : canWalk(false) {}
};

class Map {
public:
	int width,height;
	
	Map(int width, int height);
	~Map();
	bool isWall(int x, int y) const;
	void render() const;
protected:
	Tile *tiles;
		friend class BspListener;
	void dig(int x1, int y1, int x2, int y2);
	void createRoom(bool first, int x1, int y1, int x2, int y2);
};
