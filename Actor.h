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

	int getTeam() { return team; }
	int getType() { return type; }
	int getPlaced() { return isPlaced; }
	void setPlaced(int p) { isPlaced = p; }

private:

	// 1 - 9, 1 is strongest
	// 0 is bomb
	int type;
	int team;
	int isPlaced;

};

#endif
