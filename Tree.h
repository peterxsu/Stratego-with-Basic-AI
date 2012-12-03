#ifndef Heuristic_h
#define Heuristic_h
#include <iostream>
#include "Grid.h"
#include "Actor.h"
#define MAX_DEPTH 5
#define MATERIAL_CON 66
enum Terminal
{
	win,loss
};

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
	static int depth;
	int team;
	int calcEvaluationFunction(int,int,int,int,grid);
    int evaluation;
    void updateeval(int);
    void moveStore(int x1, int y1, int x2, int y2);
    int moveList[MAX_DEPTH][4];   //the movelist stores moves for every state we traverse through
    int moveMemory[4];  //This is the memory of the optimal move
    void movecopy();
    int invokeTeam;
    

public:
	Tree(Grid*, int);
    int* traverseTree();
    int[] returnMoveMemory();

};

Tree::Tree(Grid* g, int t)
{
	state = new Grid(g);
	team = t;
	depth = 0;
    invokeTeam=t;
}

int* Tree::traverseTree()          //This is an int, as I am recursively returning eval function
{   int * evaluation;
    evaluation=NULL;
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
                        int *temp2;
						temp = new Grid(state);
						state->move(x, y, x-1, y, team);
						team = (team+1)%2;
						temp2=traverseTree();
						depth--;
						delete state;
						state = temp;
                        team = (team+1)%2;
                        updateeval(temp2,evaluation);
                        delete temp2;
					}
					if(currentActor->getType() == 9)
					{
						int distance = 2;
						while(grid->isValidMove(x, y, x-distance, y, team))
						{
							moveStore(x, y, x-distance, y);
							depth++;
							Grid* temp;
                            int *temp2;
							temp = new Grid(state);
							state->move(x, y, x-distance, y, team);
							team = (team+1)%2;
							temp2=traverseTree();
							depth--;
							delete state;
							state = temp;
							team = (team+1)%2;
                            updateeval(temp2,evaluation);
                            delete temp2;
							distance++;
						}
					}
					if(grid->isValidMove(x, y, x, y-1, team))
					{
						moveStore(x, y, x, y-1);
						depth++;
                        int *temp2;
						Grid* temp;
						temp = new Grid(state);
						state->move(x, y, x, y-1, team);
						team = (team+1)%2;
						depth--;
                        temp2=traverseTree();
						delete state;
						state = temp;
						team = (team+1)%2;
                        updateeval(temp2,evaluation);
                        delete temp2;
					}
					if(currentActor->getType() == 9)
					{
						int distance = 2;
						while(grid->isValidMove(x, y, x, y-distance, team))
						{
							moveStore(x, y, x, y-distance);
							depth++;
                            int *temp2;
							Grid* temp;
							temp = new Grid(state);
							state->move(x, y, x, y- distance, team);
							team = (team+1)%2;
							temp2=traverseTree();
							depth--;
							delete state;
							state = temp;
							team = (team+1)%2;
                            updateeval(temp2,evaluation);
							distance++;
                            delete temp2;
						}
					}
					if(grid->isValidMove(x, y, x+1, y, team))
					{
						moveStore(x, y, x+1, y);
						depth++;
						Grid* temp;
                        int *temp2;
						temp = new Grid(state);
						state->move(x, y, x+1, y, team);
						team = (team+1)%2;
                        temp2=traverseTree();
                        depth--;
						delete state;
						state = temp;
						team = (team+1)%2;
                        updateeval(temp2,evaluation);
                        delete temp2;
					}
					if(currentActor->getType() == 9)
					{
						int distance = 2;
						while(grid->isValidMove(x, y, x+distance, y, team))
						{
							moveStore(x, y, x+distance, y);
							depth++;
							Grid* temp;
                            int * temp2;
							temp = new Grid(state);
							state->move(x, y, x+distance, y, team);
							team = (team+1)%2;
							temp2=traverseTree();
							depth--;
							delete state;
							state = temp;
							team = (team+1)%2;
                            updateeval(temp2,evaluation);
                            delete temp2;
							distance++;
						}
					}
					if(grid->isValidMove(x, y, x, y+1, team))
					{
						moveStore(x, y, x, y+1);
						depth++;
						Grid* temp;
                        int *temp2;
						temp = new Grid(state);
						state->move(x, y, x, y+1, team);
						team = (team+1)%2;
						temp2=traverseTree();
                        depth--;
						delete state;
						state = temp;
						team = (team+1)%2;
                        updateeval(temp2,evaluation);
                        delete temp2;
					}
					if(currentActor->getType() == 9)
					{
						int distance = 2;
						while(grid->isValidMove(x, y, x, y+distance, team))
						{
							moveStore(x, y, x, y+distance);
							depth++;
							Grid* temp;
                            int *temp2;
							temp = new Grid(state);
							state->move(x, y, x, y+distance, team);
							team = (team+1)%2;
							temp2=traverseTree();
							depth--;
							delete state;
							state = temp;
							team = (team+1)%2;
                            updateeval(temp2,evaluation);
                            delete temp2;
							distance++;
						}
					}
				}
			}
	}
	else if(depth==MAX_DEPTH)
	{
		evaluation = new int(calcEvaluationFunction());
	}
	if (depth)
		return evaluation;
	else
	{
		delete evaluation;
		return NULL;
	}
}

void Tree::moveStore(int x1, int y1, int x2, int y2)
{
	moveList[depth][0] = x1;
	moveList[depth][1] = y1;
	moveList[depth][2] = x2;
	moveList[depth][3] = y2;
}

int Tree::calcEvaluationFunction(int xin,int yin,int xfinal,int yfinal,Grid * previous)
{
          
}

void Tree::updateeval(int * temp, int * evaluation)
{
	if(evaluation!=NULL)
	{
		if (team==invokeTeam)                  //AI is going to be max
		{
			if (*temp>=*evaluation)    //chooses the maximum value
			{
				*evaluation=*temp;
				movecopy();
			}
		}
		else          //human is going to be the min
		{
			if(*temp<=*evaluation)
			{
				*evaluation=*temp;      //We expect the human to choose the best possible movecharacterised by min
				movecopy();
			}
		}
	}
	else
	{
		evaluation= new int(*temp);
		movecopy();
	}
}

void Tree::movecopy()
{
	moveMemory[0]=moveList[0][0];
    moveMemory[1]=moveList[0][1];
    moveMemory[2]=moveList[0][2];
    moveMemory[3]=moveList[0][3];
}

int[] Tree::returnMoveMemory()
{
	return  moveMemory;
}

//remember
#endif
