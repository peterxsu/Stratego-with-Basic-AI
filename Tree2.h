#ifndef TREE_H
#define TREE_H

#include "Grid.h"

#define SEARCH_DEPTH 5

class Player;

struct Move
{
	Move() { }
	Move(int a, int b, int c, int d) { x1 = a; y1 = b; x2 = c; y2 = d; }

	int x1, y1, x2, y2;
};

/*
Move * possibleMoves(int team);
int evaluation(int x1, int y1, int x2, int y2, int team);
bool inDanger(int x, int y, int team,int val);
int threat(int x, int y,int team, int flagplace);
*/

class Tree
{
public:

	Tree();
	Tree(Grid *, Player *);
	~Tree();

	// recursively searches the CURRENT GRID STATE for the move with the max/min value (depending on which team is going)
	Move search(int team, int depth, int & val, int alpha, int beta);

	//updates the state according the current state of the game grid
	//we call this before each turn
	void updateState();

	int eval();

	int getFlagX() { return fX; }
	int getFlagY() { return fY; }

	//int unknownPieces[12];

	// ************************************

	Move * possibleMoves(int team);
	int evaluation(int x1, int y1, int x2, int y2, int team);
	bool inDanger(int x, int y, int team,int val);
	int threat(int x, int y,int team, int flagplace);

private:

	//we store a pointer to the actual game grid. this grid is DIFFERENT from the one we use for searching.
	//the one we use for searching is different because it represents the grid as we know it at the time.
	//whenever the 
	Grid * grid;
	// we only store one state the whole time we're searching. We alter the state as we traverse the tree.
	Grid * state;
	Player * player;
	int numEnemyPiecesRemaining;
	int fX, fY;

};

#endif