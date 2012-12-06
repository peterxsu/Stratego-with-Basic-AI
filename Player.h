#ifndef PLAYER_H
#define PLAYER_H

class Grid;
class Actor;

#include <string>

using namespace std;

struct MoveList
{
	int x1, y1, x2, y2;
	MoveList* next;
};

class Tree;

class Player
{
public:

	Player(int, int, Grid*);
	~Player();

	void makeMove();
	void placePieces();

	void choosePieceMove(int &, int &);
	void chooseDestination(int &, int &);
	Actor* choosePiece();
	void choosePlacement(int &, int &);
	Actor* findPiece(int);

	int getTeam();
	int getHuman() { return isHuman; }
	//loads piece placement from file
	int loadPlacement(string file);

	void removeAll();

	//automatically places pieces for debugging purposes
	void autoPlacePieces();

	int getLeft(int type);
	int getTotalLeft();
	Actor * getNext(int type);

	Actor * getActor(int i) { return actors[i]; }

	static string typeToString(int ty);
	
	MoveList* getPossibleMoves();

protected:

	int team;
	Grid * grid;
	Actor ** actors;
	int isHuman;

	//used for AI

	Tree * tree;

private:


};

#endif
