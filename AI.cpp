#include "AI.h"

#include <iostream>

using namespace std;

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

void AI::placePieces()
{
	cout << "placed\n";
}


