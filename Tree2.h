#ifndef TREE_H
#define TREE_H

#include "Grid.h"

#define SEARCH_DEPTH 3

class Move
{
	Move() { }
	Move(int a, int b, int c, int d, int e) { x1 = a; y1 = b; x2 = c; y2 = d; depth = e; }

	int x1, y1, x2, y2, depth;
};

class Tree
{
public:

	Tree();
	Tree(Grid *);

	//recursively searches the CURRENT GRID STATE for the move with the max/min value (depending on which team is going)
	Move * search(int team);

private:

	Grid * state;

};

#endif
