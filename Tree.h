#ifndef Heuristic_h
#define Heuristic_h
#include <iostream>
#include <list>
#include "Grid.h"
#include "Actor.h"
#define MAX_DEPTH 5

class Tree
{   
private:
	int evaluationFunction;
      /*The above function is going to be an evaluation function
       which returns a number depicting how favourable a particular position
       is.This going to implement a minimax algorithm with alpha-beta pruning(minimize unecessary
        searches).This is going to be implemented via depth-first search(recursive). 
      
                     */
	static Grid * state;
	tree ** nextdepth;// this holds a an arra
	static int moveList[MAX_DEPTH][4];    //change the macro upon need
	static int depth;
	int team;
	int calcEvaluationFunction();
	void moveStore(int, int, int, int);

public:
	Tree(Grid*, int);
    void updateState();

};

Tree::Tree(Grid* g, int t)
{
	state = new Grid(g);
	team = t;
	depth = 0;
}

void Tree::traverseTree()
{
	if (depth < MAX_DEPTH)
	{
		Actor* currentActor = NULL;
		for(int x = 0; x < 10; x++)
			for(int y = 0; y < 10; y++)
			{
				currentActor = state->getActor(x,y);
				if(currentActor && currentActor->getTeam() == team && currentActor->getType() > 0 && currentActor->getType() < 11)
				{
					if(grid->isValidMove(x, y, x-1, y, team))
					{
						moveStore(x, y, x-1, y);
						depth++;
						Grid* temp;
						temp = new Grid(state);
						state->move(x, y, x-1, y, team);
						team = (team+1)%2;
						traverseTree();
						depth--;
						delete state;
						state = temp;
						team = (team+1)%2;
					}
					if(currentActor->getType() == 9)
					{
						int distance = 2;
						while(grid->isValidMove(x, y, x-distance, y, team))
						{
							moveStore(x, y, x-distance, y);
							depth++;
							Grid* temp;
							temp = new Grid(state);
							state->move(x, y, x-distance, y, team);
							team = (team+1)%2;
							traverseTree();
							depth--;
							delete state;
							state = temp;
							team = (team+1)%2;
							distance++;
						}
					}
					if(grid->isValidMove(x, y, x, y-1, team))
					{
						moveStore(x, y, x, y-1);
						depth++;
						Grid* temp;
						temp = new Grid(state);
						state->move(x, y, x, y-1, team);
						team = (team+1)%2;
						traverseTree();
						depth--;
						delete state;
						state = temp;
						team = (team+1)%2;
					}
					if(currentActor->getType() == 9)
					{
						int distance = 2;
						while(grid->isValidMove(x, y, x, y-distance, team))
						{
							moveStore(x, y, x, y-distance);
							depth++;
							Grid* temp;
							temp = new Grid(state);
							state->move(x, y, x, y-distance, team);
							team = (team+1)%2;
							traverseTree();
							depth--;
							delete state;
							state = temp;
							team = (team+1)%2;
							distance++;
						}
					}
					if(grid->isValidMove(x, y, x+1, y, team))
					{
						moveStore(x, y, x+1, y);
						depth++;
						Grid* temp;
						temp = new Grid(state);
						state->move(x, y, x+1, y, team);
						team = (team+1)%2;
						traverseTree();
						depth--;
						delete state;
						state = temp;
						team = (team+1)%2;
					}
					if(currentActor->getType() == 9)
					{
						int distance = 2;
						while(grid->isValidMove(x, y, x+distance, y, team))
						{
							moveStore(x, y, x+distance, y);
							depth++;
							Grid* temp;
							temp = new Grid(state);
							state->move(x, y, x+distance, y, team);
							team = (team+1)%2;
							traverseTree();
							depth--;
							delete state;
							state = temp;
							team = (team+1)%2;
							distance++;
						}
					}
					if(grid->isValidMove(x, y, x, y+1, team))
					{
						moveStore(x, y, x, y+1);
						depth++;
						Grid* temp;
						temp = new Grid(state);
						state->move(x, y, x, y+1, team);
						team = (team+1)%2;
						traverseTree();
						depth--;
						delete state;
						state = temp;
						team = (team+1)%2;
					}
					if(currentActor->getType() == 9)
					{
						int distance = 2;
						while(grid->isValidMove(x, y, x, y+distance, team))
						{
							moveStore(x, y, x, y+distance);
							depth++;
							Grid* temp;
							temp = new Grid(state);
							state->move(x, y, x, y+distance, team);
							team = (team+1)%2;
							traverseTree();
							depth--;
							delete state;
							state = temp;
							team = (team+1)%2;
							distance++;
						}
					}
				}
			}
	}
}

void Tree::moveStore(int x1, int y1, int x2, int y2)
{
	moveList[depth][0] = x1;
	moveList[depth][1] = y1;
	moveList[depth][2] = x2;
	moveList[depth][3] = y2;
}


#endif
