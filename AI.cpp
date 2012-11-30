#include "AI.h"

AI::AI() : Player(0, NULL)
{
}

AI::AI(int t, Grid * g) : Player(t, g)
{
	isHuman = 0;
}

AI::~AI()
{
}



