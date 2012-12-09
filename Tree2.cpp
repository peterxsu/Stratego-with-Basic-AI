#include "Tree2.h"
#include "Player.h"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <list>
#include <cmath>

#define FLAG_VALUE 1000000

using namespace std;

Tree::Tree()
{
	state = NULL;
	grid = NULL;
	player = NULL;
	flagX = 0;
	flagY = 0;
}

Tree::Tree(Grid * g, Player * p)
{
	grid = g;
	player = p;
	state = NULL;

	updateState();
}

Tree::~Tree()
{
	if (state) delete state;
}

Move Tree::search(int team, int depth, int & val, int alpha, int beta)
{

	// these vectors store all possible moves and their corresponding values
	vector<int> values;
	vector<Move> moves;		
	
	// if we've reached our max depth we return the value of the state and NULL for the move
	if (depth == SEARCH_DEPTH + 1)
	{
		val = eval();
		return Move(0, 0, 0, 0);
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
								moves.push_back(Move(x1, y1, x2, y1));
						for (int y2 = 0; y2 < 10; y2++)
							if (state->isValidMove(x1, y1, x1, y2, team))
								moves.push_back(Move(x1, y1, x1, y2));
					}
					else
					{
						if (state->isValidMove(x1, y1, x1 - 1, y1, team))
							moves.push_back(Move(x1, y1, x1 - 1, y1));
						if (state->isValidMove(x1, y1, x1 + 1, y1, team))
							moves.push_back(Move(x1, y1, x1 + 1, y1));
						if (state->isValidMove(x1, y1, x1, y1 - 1, team))
							moves.push_back(Move(x1, y1, x1, y1 - 1));
						if (state->isValidMove(x1, y1, x1, y1 + 1, team))
							moves.push_back(Move(x1, y1, x1, y1 + 1));
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
		/*
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
			if (team == player->getTeam())
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
		*/
		int aindex = 0, bindex = 0;
		for (int i = 0; i < moves.size(); i++)
		{
			// alter the state by making the move
			state->move(moves[i].x1, moves[i].y1, moves[i].x2, moves[i].y2, team);
			int v;
			// check the value of this state as determined by the max/min of the states that can occur after it
			search(1 - team, depth + 1, v, alpha, beta);
			// if it's the computer we want to maximize value
			state->undoMove();

			if (team == player->getTeam())
			{
				if (v > alpha)
				{
					alpha = v;
					aindex = i;
				}
				if (alpha >= beta)
				{
					val = alpha;
					return moves[i];
				}
			}
			// otherwise minimize it
			else
			{
				if (v < beta)
				{
					beta = v;
					bindex = i;
				}
				if (alpha >= beta)
				{
					val = beta;
					return moves[i];
				}
			}
		}
		if (team == player->getTeam())
		{
			val = alpha;
			return moves[aindex];
		}
		else
		{
			val = beta;
			return moves[bindex];
		}
		/*					
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
		*/
	}
	else
	{
		val = 0;
		return Move(0, 0, 0, 0);
	}
}

// evaluation should be based on a few things:
//  - how many pieces are left and what are their values?
//  - how far are we from the flag? or did we capture the flag?
//  - did we disarm a bomb? the removal of bombs should be given more
//    weight than that of other pieces because it is often necessary to remove them before
//    the flag can be taken.

int Tree::eval()
{
	int pieceTotal = 0;
	int distTotal = 0;
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			if (state->getActor(x, y))
			{
				int ty = state->getActor(x, y)->getType();
				int te = state->getActor(x, y)->getTeam();
				if (te != 1 && te != 0) continue;
				int mult = (te == player->getTeam()) * 2 - 1;
				switch (ty)
				{
 				case 0:  //bombs
					pieceTotal += mult * 20;
					break;
				case 11:
					pieceTotal += mult * FLAG_VALUE;
					break;
				default:
					pieceTotal += mult * (20 - ty);
				}
				if (te == player->getTeam())
					distTotal += abs(flagX - x) + abs(flagY - y);
			}
		}
	}
	return 10 * pieceTotal + distTotal;
}

// in updateState(), we use the actual game grid and properties specifying information such as which pieces have moved
// to generate a game state that represents the grid as the AI sees it.

/*

rules to follow when generating state:

start with list of all actors
remove actors that are known, add to state
for all unknown actors that haven't moved, represent them first with bombs and flags
if those run out, represent them with remaining actors

*/

void Tree::updateState()
{
	if (state) delete state;

	state = new Grid(0);

	vector<Actor*> immobile;
	vector<Actor*> mobile;

	int notMoved = 0;

	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			Actor * a = grid->getActor(x, y);
			if (a)
			{
				if (a->getTeam() == 1 - player->getTeam() && !a->getKnown())
				{
					if (a->getType() == 0 || a->getType() == 11)
						immobile.push_back(a);
					else
						mobile.push_back(a);
					if (a->getMoved() == 0) notMoved++;
				}
			}
		}
	}

	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			if (grid->getActor(x, y) && !state->getActor(x, y))
			{
				if (grid->getActor(x, y)->getTeam() == player->getTeam() || grid->getActor(x, y)->getKnown())
				{
					state->add(new Actor(grid->getActor(x, y)), x, y);
				}
				else
				{
					if (!grid->getActor(x, y)->getMoved())
					{
						int i = rand() % notMoved;
						notMoved--;
						if (i >= immobile.size())
						{	
							i = rand() % mobile.size();
							state->add(new Actor(mobile[i]), x, y);
							Actor * temp = mobile[i];
							mobile[i] = mobile[mobile.size() - 1];
							mobile[mobile.size() - 1] = temp;
							mobile.pop_back();
						}
						else
						{
							state->add(new Actor(immobile[i]), x, y);
							Actor * temp = immobile[i];
							immobile[i] = immobile[immobile.size() - 1];
							immobile[immobile.size() - 1] = temp;
							immobile.pop_back();
						}
					}
					else
					{
						int i = rand() % mobile.size();
						state->add(new Actor(mobile[i]), x, y);
						Actor * temp = mobile[i];
						mobile[i] = mobile[mobile.size() - 1];
						mobile[mobile.size() - 1] = temp;
						mobile.pop_back();
					}
				}
				if (state->getActor(x, y)->getType() == 11 && state->getActor(x, y)->getTeam() == 1 - player->getTeam())
				{
					flagX = x;
	 				flagY = y;
				}
			}
		}
	}

}

// ***********************************************************************************************************************************

Move * Tree::possibleMoves(int team)
{
    
	// these vectors store all possible moves and their corresponding values
	vector<int> values;
	vector<Move*> moves;
    
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
                            {
								moves.push_back(new Move(x1, y1, x2, y1));
                                values.push_back(evaluation(x1, y1, x2, y1, team));
                            }
						for (int y2 = 0; y2 < 10; y2++)
							if (state->isValidMove(x1, y1, x1, y2, team))
                            {
								moves.push_back(new Move(x1, y1, x1, y2));
                                values.push_back(evaluation(x1, y1, x1, y2, team));
                            }
					}
					else
					{
						if (state->isValidMove(x1, y1, x1 - 1, y1, team))
                        {
							moves.push_back(new Move(x1, y1, x1 - 1, y1));
                            values.push_back(evaluation(x1, y1, x1-1, y1, team));
                        }
						if (state->isValidMove(x1, y1, x1 + 1, y1, team))
                        {
							moves.push_back(new Move(x1, y1, x1 + 1, y1));
                            values.push_back(evaluation(x1, y1, x1 +1, y1, team));
                        }
						if (state->isValidMove(x1, y1, x1, y1 - 1, team))
                        {
							moves.push_back(new Move(x1, y1, x1, y1 - 1));
                            values.push_back(evaluation(x1, y1, x1, y1 - 1, team));
                        }
						if (state->isValidMove(x1, y1, x1, y1 + 1, team))
                        {
							moves.push_back(new Move(x1, y1, x1, y1 + 1));
                            values.push_back(evaluation(x1, y1, x1, y1 +1, team));
                        }
					}
				}
			}
		}
	}
    int movesSize = values.size();
    int max = -10000;
    int indexOfMax;
    //traverses entire values array to find which move has the highest value (best move)
    //moves index corresponds with values index
    for(int j=0; j<movesSize; j++)
    {
        if(values[j]>max)
        {
            max=values[j];
            indexOfMax=j;
        }
    }
    return moves[indexOfMax];
}

int Tree::evaluation(int x1, int y1, int x2, int y2, int team)
{
    //determines where our flag is so that we can account for threat if piece is on our side
    int ourflagY;
	for (int i = 0; i < 10; i++)
    {
		for (int j = 0; j < 10; j++)
		{
			if (state->getActor(i, j) && state->getActor(i,j)->getType()==11 && state->getActor(i,j)->getTeam()==team)
			{
                ourflagY=j;
            }
        }
    }
    int flagX;
    int flagY;
    //search grid for where their flag is
    for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (state->getActor(i, j) && state->getActor(i,j)->getType()==11 && 
			state->getActor(i,j)->getTeam()!=team)
			{
                flagX=i;
                flagY=j;
            }
        }
    }
    //need to make sure math is included so we can use absolute value
    int distToFlag= abs(x1-flagX) + abs(y1-flagY);
    int newdistToFlag = abs(x2-flagX) + abs(y2-flagY);
    //number of their actors remaining to decide how important certain moves are
    int actorsRemaining;
    for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (state->getActor(i, j) && state->getActor(i,j)->getTeam()!=team)
			{
                actorsRemaining++;
            }
        }
    }
    int worth=0;
    int val = state->getActor(x1,y1)->getType();
    bool startdanger = inDanger(x1, y1, team, val);
    bool movedanger = inDanger(x2, y2, team, val);
    if(movedanger)
        worth-=10-val;
    if(startdanger==true && movedanger==false)
        worth+=10-val;
    //checks to make sure we are not moving to a blank space
    if(state->getActor(x2,y2))
    {
        //checks moving into bomb
        if(state->getActor(x2,y2)->getType()==0)
        {
            if(val==8)
            {
                //add five because it is good to take out a bomb, even if it puts the minor in danger (5-2 = 3 so move is positive)
                worth+=5;
            }
            else
                worth-=10-val;
        }
        //checks moving 1 into spy, one of the worst possible moves
        if(state->getActor(x2,y2)->getType()==10 && val==1)
            worth-=11;
        //adds attacking value
        if(val< state->getActor(x2,y2)->getType())
            worth+=10-state->getActor(x2,y2)->getType()+(threat(x2,y2,team, ourflagY)/2);
        //subtracts dying value and makes sure that we aren't moving into a bomb to avoid double counting
        if(val> state->getActor(x2,y2)->getType() && state->getActor(x2,y2)->getType()!=0)
            worth-=10-val;
        //takes flag if can
        if(state->getActor(x2,y2)->getType()==11)
            worth+=1000;
    }
    //moving closer to the flag is somewhat beneficial
    if(newdistToFlag<distToFlag)
        worth+=1;
    //moving a miner to the flag is better than most other pieces because it can take out bombs
    if(newdistToFlag<distToFlag && val==8 && actorsRemaining<25)
        worth+=2;
    //early game scenarios its better to advance good pieces
    if(newdistToFlag<distToFlag && val<=4)
        worth+=1;
    
    return worth;
}
bool Tree::inDanger(int x, int y, int team,int val)
{
    //check up down left and right pieces, makes sure actor exists
    if(x>0 && state->getActor(x-1,y) && 
	state->getActor(x-1,y)->getTeam()!=team && state->getActor(x-1,y)->getType()<=val && state->getActor(x-1,y)->getType()>0)
        return true;
    if(x<9 && state->getActor(x+1,y) && 
	state->getActor(x+1,y)->getTeam()!=team && state->getActor(x+1,y)->getType()<=val && state->getActor(x+1,y)->getType()>0)
        return true;
    if(y>0 && state->getActor(x,y-1) && 
	state->getActor(x,y-1)->getTeam()!=team && state->getActor(x,y-1)->getType()<=val && state->getActor(x,y-1)->getType()>0)
        return true;
    if(y<9 && state->getActor(x,y+1) && 
	state->getActor(x,y+1)->getTeam()!=team && state->getActor(x,y+1)->getType()<=val && state->getActor(x,y+1)->getType()>0)
        return true;
    return false;
    
}
//checks what pieces the oponent can kill aka what pieces we would be saving by killing them
int Tree::threat(int x, int y,int team, int flagplace)
{
	int val;
    //check up down left and right pieces to see what it can kill
    val = state->getActor(x,y)->getType();
    int t =0;
    if(x>0 && state->getActor(x-1,y) && state->getActor(x-1,y)->getTeam()==team && state->getActor(x-1,y)->getType()>=val && state->getActor(x-1,y)->getType()>0)
        t+=10-state->getActor(x-1,y)->getType();
    if(x<9 && state->getActor(x+1,y) && state->getActor(x+1,y)->getTeam()==team && state->getActor(x+1,y)->getType()>=val && state->getActor(x+1,y)->getType()>0)
        t+=10-state->getActor(x+1,y)->getType();
    if(y>0 && state->getActor(x,y-1) && state->getActor(x,y-1)->getTeam()==team && state->getActor(x,y-1)->getType()>=val && state->getActor(x,y-1)->getType()>0)
        t+=10-state->getActor(x,y-1)->getType();
    if(y<9 && state->getActor(x,y+1) && state->getActor(x,y+1)->getTeam()==team && state->getActor(x,y+1)->getType()>=val && state->getActor(x,y+1)->getType()>0)
        t+=10-state->getActor(x,y+1)->getType();
    //adds to the threat if they are on our side
    if(flagplace<=3 && y<=4)
    {
        t+=2;
    }
    if(flagplace>=6 && y>=5)
    {
        t+=2;
    }
    if(flagplace==y)
    {
        t+=2;
    }
    return t;
}
