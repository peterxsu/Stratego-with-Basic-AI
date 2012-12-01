#include "Actor.h"

Actor::Actor()
{
	isPlaced = 0;
	type = 0;
	team = 0;
}

Actor::Actor(int ty, int te)
{
	type = ty;
	team = te;
	isPlaced = 0;
}

Actor::Actor(Actor* a)
{
	isPlaced = a->isPlaced;
	type = a->type;
	team = a->team;
}
