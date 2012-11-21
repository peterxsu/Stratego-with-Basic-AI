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

class Player
{
public:

	Player(int, Grid*);
	~Player();
	void makeMove();
	void choosePieceMove(int &, int &);
	void chooseDestination(int &, int &);
	void placePieces();
	Actor* choosePiece();
	void choosePlacement(int &, int &);
	Actor* findPiece(int);
	int getTeam();

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

private:


};

#endif
