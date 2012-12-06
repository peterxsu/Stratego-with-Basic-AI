#ifndef ACTOR_H
#define ACTOR_H

//#define BOMB 0
//#define 

#include <SFML/Graphics.hpp>

class Actor
{
public:

	Actor();
	Actor(int ty, int te);
	Actor(Actor*);

	int getTeam() { return team; }
	int getType() { return type; }
	int getPlaced() { return isPlaced; }
	void setPlaced(int p) { isPlaced = p; }

	void setMoved(int m) { moved = m; }
	int getMoved() { return moved; }
	void setKnown(int k) { known = k; }
	int getKnown() { return known; }

private:

	// 1 - 9, 1 is strongest
	// 0 is bomb
	int type;
	int team;
	int isPlaced;

	int moved;
	int known;

};

#endif
