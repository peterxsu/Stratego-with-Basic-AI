#ifndef TREE_H
#define TREE_H

#include "Grid.h"

#define SEARCH_DEPTH 3

class Player;

struct Move
{
	Move() { }
	Move(int a, int b, int c, int d) { x1 = a; y1 = b; x2 = c; y2 = d; }

	int x1, y1, x2, y2;
};

class Tree
{
public:

	Tree();
	Tree(Grid *, Player *);
	~Tree();

	// recursively searches the CURRENT GRID STATE for the move with the max/min value (depending on which team is going)
	Move * search(int team, int depth, int & val);

	int eval();

private:

	// we only store one state the whole time we're searching. We alter the state as we traverse the tree.
	Grid * state;
	Player * player;

};

#endif
