#include "Tree2.h"

#include <cstdlib>
#include <vector>

using namespace std;

Tree::Tree()
{
	state = NULL;
}

Tree::Tree(Grid * g)
{
	state = new Grid(g);
}

Move * Tree::search(int team, int depth, int & val)
{
	vector<int> values;
	vector<Move*> moves;		
	
	if (depth == SEARCH_DEPTH)
	{
		val = eval();
		return NULL;
	}

	for (int x1 = 0; x1 < 10; x1++)
	{
		for (int y1 = 0; y1 < 10; y1++)
		{	
			if (state->getActor(x1, y1))
			{	
				if (state->getActor(x1, y1)->getTeam() == team)
				{
					if (state->getActor(x1, y1)->getType() == 9)
					{
						for (int x2 = 0; x2 < 10; x2++)
							if (state->isValidMove(x1, y1, x2, y1, team))
								moves.push_back(new Move(x1, y1, x2, y1, depth));
						for (int y2 = 0; y2 < 10; y2++)
							if (state->isValidMove(x1, y1, x1, y2, team))
								moves.push_back(new Move(x1, y1, x1, y2, depth));
					}
					else
					{
						if (state->isValidMove(x1, y1, x1 - 1, y1, team))
							moves.push_back(new Move(x1, y1, x1 - 1, y1, depth));
						if (state->isValidMove(x1, y1, x1 + 1, y1, team))
							moves.push_back(new Move(x1, y1, x1 + 1, y1, depth));
						if (state->isValidMove(x1, y1, x1, y1 - 1, team))
							moves.push_back(new Move(x1, y1, x1, y1 - 1, depth));
						if (state->isValidMove(x1, y1, x1, y1 + 1, team))
							moves.push_back(new Move(x1, y1, x1, y1 + 1, depth));
					}
				}
			}
		}
	}
	int bestVal = 0;
	int bestIndex = 0;
	if (moves.size() > 0)
	{
		state->move(moves[0]->x1, moves[0]->y1, moves[0]->x2, moves[0]->y2, team);
		search(1 - team, depth + 1, bestVal);
		state->undoMove();
		for (int i = 0; i < moves.size(); i++)
		{
			state->move(moves[i]->x1, moves[i]->y1, moves[i]->x2, moves[i]->y2, team);
			int v;
			search(1 - team, depth + 1, v);
			
			if (team == 0)
			{
				if (v > bestVal)
				{
					bestIndex = i;
					bestVal = v;
				}
			}
			else
			{
				if (v < bestVal)
				{
					bestIndex = i;
					bestVal = v;
				}
			}
			state->undoMove();
		}
								
		val = bestVal;
		if (depth > 1)
		{
			for (int i = 0; i < moves.size(); i++) delete moves[i];
			return NULL;
		}
		else
		{
			for (int i = 0; i < moves.size(); i++) { if (bestIndex != i) delete moves[i]; }
			return moves[bestIndex];
		}
	}
	else
	{
		val = 0;
		return NULL;
	}
}

int Tree::eval()
{
	return rand();
}
