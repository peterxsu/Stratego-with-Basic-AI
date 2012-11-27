#ifndef AI_H
#define AI_H

#include "Player.h"

class Grid;
class Actor;

class AI : public Player
{
public:

	AI();
	AI(int, Grid *);
	~AI();

private:

};

#endif
