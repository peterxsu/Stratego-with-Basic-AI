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
	static Grid * state; //The state of the grid
	static int depth;
	int team;    //the team to move per state.we keep cycling through it
	int calcEvaluationFunction(int,int,int,int,grid);//We use this to evaluate the state at the maxdepth
    void updateeval(int); //This is going to update the evaluation function of nodes higher than maxdepth
    void moveStore(int x1, int y1, int x2, int y2);//this function stores moves in movelist
    int moveList[MAX_DEPTH][4];   //the movelist stores moves for every state we traverse through
    int moveMemory[4];  //This is the memory of the optimal move
    void movecopy(); //movecopy stores the optimal moves from movestore to movememory
    int invokeTeam; //invoke team is the team that called the tree

public:
	Tree(Grid*, int); //this initialises the tree object
    int* traverseTree(); //traverse tree returns null for depth of 0 and evaluation pointer for any other depth
    int* returnMoveMemory();

};

Tree::Tree(Grid* g, int t)
{
	state = new Grid(g);
	team = t;
	depth = 0;
    invokeTeam=t;
}

int* Tree::traverseTree()          //This is an int*, as I am recursively returning eval function
{
	int * evaluation;
    evaluation=NULL;
	if (depth < MAX_DEPTH) //we are cycling through possible moves in the nested for loops for depth<max depth to get all possible states for every move
	{   
		Actor* currentActor = NULL;
		for(int x = 0; x < 10; x++)
			for(int y = 0; y < 10; y++)
			{ 
				currentActor = state->getActor(x,y);
				if(currentActor && currentActor->getTeam() == team && currentActor->getType() > 0 && currentActor->getType() < 11) //checking if valid actor and valid type and if it is in the team that has to move
				{
					if(grid->isValidMove(x, y, x-1, y, team)) //checking whether moving an actor to the left square is a valid move
					{
						moveStore(x, y, x-1, y); //storing in the move store
						depth++;// increasing depth as we are producing possible nodes for every possible move
						Grid* temp; //producing a temporary grid, so that after recursion we can come back to the previous states
                        int *temp2;
						temp = new Grid(state); 
						state->move(x, y, x-1, y, team); //we are making the move now
						team = (team+1)%2; //flipping the team as it is the opponents move
						temp2=traverseTree(); //storing the evaluation pointer
						depth--;
						delete state;
						state = temp;
                        team = (team+1)%2;
                        updateeval(temp2,evaluation); //updating the evaluation pointer.min for one player;max for the other
                        delete temp2; //deleting the temporary pointers
					}
					if(currentActor->getType() == 9) //checking if an actor at a particular spot is a scout
					{
						int distance = 2;
						while(grid->isValidMove(x, y, x-distance, y, team))//Producing all the moves which involve a scout moving left
						{
							moveStore(x, y, x-distance, y); //storing eveery possible move that lead to children states of moving a scout to the left
							depth++; //increasing depth as we produce the children states
							Grid* temp;
                            int *temp2; //used for storing the evaluation pointer after recursion
							temp = new Grid(state);
							state->move(x, y, x-distance, y, team);
							team = (team+1)%2; //flipping the team as it is the opponents move
							temp2=traverseTree();//storing the evaluation pointer
							depth--;
							delete state;
							state = temp;
							team = (team+1)%2;
                            updateeval(temp2,evaluation); //updating the evaluation pointer.min for one player;max for the other
                            delete temp2;//deleting the temporary pointers
							distance++;
						}
					}
					if(grid->isValidMove(x, y, x, y-1, team)) //checking a valid move for moving downwards
					{
						moveStore(x, y, x, y-1);//storing eveery possible move that lead to children states of moving a scout to the left
						depth++;//increasing depth as we produce the children states
                        int *temp2;//used for storing the evaluation pointer after recursion
						Grid* temp;
						temp = new Grid(state);
						state->move(x, y, x, y-1, team);
						team = (team+1)%2;
						depth--;
                        temp2=traverseTree();//storing the evaluation pointer
						delete state;
						state = temp;
						team = (team+1)%2;
                        updateeval(temp2,evaluation); //updating the evaluation pointer.min for one player;max for the other
                        delete temp2; //deleting the temporary pointers
                        //The other functions are copies of the same code checking if ever actor can move every coordinate in all the 3 directions.the speciality cases for the scouts are recorded in a while loop for varying distances starting from 2
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
	if (depth)   //we return evaluation pointers for every depth other than start.for start,we return null
		return evaluation;
	else
	{
		delete evaluation;
		return NULL;
	}
}

void Tree::moveStore(int x1, int y1, int x2, int y2) //movestore stores every possible move while playing
{
	moveList[depth][0] = x1;
	moveList[depth][1] = y1;
	moveList[depth][2] = x2;
	moveList[depth][3] = y2;
}

int Tree::calcEvaluationFunction(int xin,int yin,int xfinal,int yfinal,Grid * previous)//calculating the evaluation function
{
          
}

void Tree::updateeval(int * temp, int * evaluation) //updateeval expects the human to choose the minimum of the evaluation function for all children states and the AI to choose the maximum
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

int* Tree::returnMoveMemory() //we return the most optimal move to take
{
	return  moveMemory;
}

//remember
#endif
