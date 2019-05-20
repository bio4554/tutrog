class Actor {
public:
	int x,y; //x and y position
	int ch; //char code
	TCODColor col; //color

	Actor(int x, int y, int ch, const TCODColor &col);
	void render() const;
};
