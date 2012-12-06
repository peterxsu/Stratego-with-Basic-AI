#include "Tree2.h"
#include "Player.h"

#include <cstdlib>
#include <vector>

using namespace std;

Tree::Tree()
{
	state = NULL;
	grid = NULL;
}

Tree::Tree(Grid * g, Player * p)
{
	// create a copy of the grid to represent the state.
	// it needs to be a copy because we will be modifying it.
	grid = g;
	player = p;
	state = NULL;

	updateState();
}

Tree::~Tree()
{
	if (state) delete state;
}

Move * Tree::search(int team, int depth, int & val)
{

	// these vectors store all possible moves and their corresponding values
	vector<int> values;
	vector<Move*> moves;		
	
	// if we've reached our max depth we return the value of the state and NULL for the move
	if (depth == SEARCH_DEPTH)
	{
		val = eval();
		return NULL;
	}

	// iterate through entire board to consider every possible move
	for (int x1 = 0; x1 < 10; x1++)
	{
		for (int y1 = 0; y1 < 10; y1++)
		{	
			// check if there's an actor at the spot
			if (state->getActor(x1, y1))
			{	
				// and make sure it's on the right team
				if (state->getActor(x1, y1)->getTeam() == team)
				{
					// now we check every move this piece could make,
					// and add each move to our moves vector
					// if it's a scout we must consider its movement
					if (state->getActor(x1, y1)->getType() == 9)
					{
						for (int x2 = 0; x2 < 10; x2++)
							if (state->isValidMove(x1, y1, x2, y1, team))
								moves.push_back(new Move(x1, y1, x2, y1));
						for (int y2 = 0; y2 < 10; y2++)
							if (state->isValidMove(x1, y1, x1, y2, team))
								moves.push_back(new Move(x1, y1, x1, y2));
					}
					else
					{
						if (state->isValidMove(x1, y1, x1 - 1, y1, team))
							moves.push_back(new Move(x1, y1, x1 - 1, y1));
						if (state->isValidMove(x1, y1, x1 + 1, y1, team))
							moves.push_back(new Move(x1, y1, x1 + 1, y1));
						if (state->isValidMove(x1, y1, x1, y1 - 1, team))
							moves.push_back(new Move(x1, y1, x1, y1 - 1));
						if (state->isValidMove(x1, y1, x1, y1 + 1, team))
							moves.push_back(new Move(x1, y1, x1, y1 + 1));
					}
				}
			}
		}
	}
	// now we check which of the possible moves has the highest (or lowest, depending on which team is making the move)
	// value
	int bestVal = 0;
	int bestIndex = 0;
	// make sure there is at least one valid move
	if (moves.size() > 0)
	{
		// first we check the first move and compare the rest to it
		state->move(moves[0]->x1, moves[0]->y1, moves[0]->x2, moves[0]->y2, team);
		search(1 - team, depth + 1, bestVal);
		state->undoMove();
		for (int i = 1; i < moves.size(); i++)
		{
			// alter the state by making the move
			state->move(moves[i]->x1, moves[i]->y1, moves[i]->x2, moves[i]->y2, team);
			int v;
			// check the value of this state as determined by the max/min of the states that can occur after it
			search(1 - team, depth + 1, v);
			// if it's the computer we want to maximize value
			if (team == 0)
			{
				if (v > bestVal)
				{
					bestIndex = i;
					bestVal = v;
				}
			}
			// otherwise minimize it
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
		// clean up, but if we're at a depth of 1, we're returning the move to the user
		// so don't delete that one.
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

// not complete at the moment

int Tree::eval()
{
	int total = 0;
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			if (state->getActor(x, y))
			{
				if (state->getActor(x, y)->getTeam() == player->getTeam())
				{
					total -= 11 - state->getActor(x, y)->getType();
				}
				else
				{
					total += 11 - state->getActor(x, y)->getType();
				}
			}
		}
	}
	return total;
}

void Tree::updateState()
{
	if (state) delete state;
	state = new Grid(grid);
}
