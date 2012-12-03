#include <iostream>
#include <fstream>

#include "Player.h"
#include "Grid.h"
#include "Actor.h"

using namespace std;


Player::Player(int h, int t, Grid* g)
{
	isHuman = h;
	team = t;
	grid = g;
	actors = new Actor*[40];
	actors[0] = new Actor(1, team);
	actors[1] = new Actor(2, team);
	actors[2] = new Actor(3, team);
	actors[3] = new Actor(3, team);
	for (int x = 4; x < 7; x++)
		actors[x] = new Actor(4, team);
	for (int x = 7; x < 11; x++)
		actors[x] = new Actor(5, team);
	for (int x = 11; x < 15; x++)
		actors[x] = new Actor(6, team);
	for (int x = 15; x < 19; x++)
		actors[x] = new Actor(7, team);
	for (int x = 19; x < 24; x++)
		actors[x] = new Actor(8, team);
	for (int x = 24; x < 32; x++)
		actors[x] = new Actor(9, team);
	for (int x = 32; x < 38; x++)
		actors[x] = new Actor(0, team);
	actors[38] = new Actor(10, team);
	actors[39] = new Actor(11, team);
}

Player::~Player()
{
	for (int x = 0; x < 40; x++)
		delete actors[x];
	delete []actors;
}

void Player::makeMove()
{
	int x1, x2, y1, y2;
	choosePieceMove(x1, y1);
	do
	{
		chooseDestination(x2, y2);
	}while(!grid->isValidMove(x1, y1, x2, y2, team));
	grid->move(x1, y1, x2, y2, team);
}

void Player::choosePieceMove(int &x, int &y)
{
	char c;
	do
	{
	cout << "Please enter the coordinates of the piece you want to move" << endl;
	cin >> c >> y;
	c = toupper(c);
	x = c - 'A';
	}while(x < 0 || x > 9 || y < 0 || y > 9 /*checks if selected square is within the board boundaries*/|| grid->getActor(x, y)->getTeam() != team /*checks if the piece is on the correct team*/ || grid->getActor(x, y)->getType() < 1 || grid->getActor(x, y)->getType() > 10 /*checks if the piece is movable*/);
}

void Player::chooseDestination(int &x, int &y)
{
	char c;
	cout << "Please enter the coordinates of the square you want to move to" << endl;
	cin >> c >> y;
	c = toupper(c);
	x = c - 'A';
}

void Player::placePieces()
{
	loadPlacement("template1.dat");
}

Actor* Player::choosePiece()
{
	int type = -2;
	do
	{
		grid->print();
		cout << "Which piece do you want to place on the board?" << endl;
		cin >> type;
	}while(type < -1 || type > 11 || !findPiece(type));
	return findPiece(type);
}

void Player::choosePlacement(int &x, int &y)
{
	char c;
	int minX, maxX;
	if (team == 1)
	{
		minX = 0;
		maxX = 3;
	}
	else
	{
		minX = 6;
		maxX = 9;
	}
	do
	{
	cout << "Please enter the coordinates of the square where you want to place this piece" << endl;
	cin >> c >> y;
	c = toupper(c);
	x = c - 'A';
	}while(x < minX || x > maxX || y < 0 || y > 9 || grid->getActor(x, y));
}

Actor* Player::findPiece(int t)
{
	for (int x = 0; x < 40; x++)
	{
		if (actors[x]->getType() == t && !actors[x]->getPlaced())
			return actors[x];
	}
	return NULL;
}

int Player::getTeam()
{
	return team;
}

int Player::getLeft(int type)
{
	int left = 0;
	for (int i = 0; i < 40; i++)
	{
		if (actors[i]->getType() == type && actors[i]->getPlaced() == 0)
		{
			left++;
		}
	}
	return left;
}

Actor * Player::getNext(int type)
{
	if (getLeft(type))
	{
		for (int i = 0; i < 40; i++)
		{
			if (actors[i]->getType() == type)
			{
				if (!actors[i]->getPlaced())
				{
					return actors[i];
				}
			}
		}
	}
	return NULL;
}

string Player::typeToString(int ty)
{
	switch (ty)
	{
	case 0: return "Bomb";
	case 1: return "Marshall";
	case 2: return "General";
	case 3: return "Colonel";
	case 4: return "Major";
	case 5: return "Captain";
	case 6: return "Lieutenant";
	case 7: return "Sergeant";
	case 8: return "Miner";
	case 9: return "Scout";
	case 10: return "Spy";
	case 11: return "Flag";
	}
}

int Player::getTotalLeft()
{
	int n = 0;
	for (int i = 0; i < 40; i++)
	{
			if (!actors[i]->getPlaced())
				n++;
	}
	return n;
}

MoveList* Player::getPossibleMoves()
{
	MoveList* firstMove = NULL;
	MoveList* currentMove = NULL;
	MoveList* newMove = NULL;
	Actor* currentActor = NULL;
	for(int x = 0; x < 10; x++)
		for(int y = 0; y < 10; y++)
		{
			currentActor = grid->getActor(x, y);
			if(currentActor && currentActor->getTeam() == team && currentActor->getType() > 0 && currentActor->getType() < 11)
			{
				if(grid->isValidMove(x, y, x-1, y, team))
				{
					newMove = new MoveList;
					newMove->x1 = x;
					newMove->y1 = y;
					newMove->x2 = x-1;
					newMove->y2 = y;
					newMove->next = NULL;
					if(!firstMove)
						firstMove = newMove;
					else
						currentMove->next = newMove;
					currentMove = newMove;
				}
				if(currentActor->getType() == 9)
				{
					int distance = 2;
					while(grid->isValidMove(x, y, x-distance, y, team))
					{
						newMove = new MoveList;
						newMove->x1 = x;
						newMove->y1 = y;
						newMove->x2 = x-distance;
						newMove->y2 = y;
						newMove->next = NULL;
						if(!firstMove)
							firstMove = newMove;
						else
							currentMove->next = newMove;
						currentMove = newMove;
						distance++;
					}
				}
				if(grid->isValidMove(x, y, x, y-1, team))
				{
					newMove = new MoveList;
					newMove->x1 = x;
					newMove->y1 = y;
					newMove->x2 = x;
					newMove->y2 = y-1;
					newMove->next = NULL;
					if(!firstMove)
						firstMove = newMove;
					else
						currentMove->next = newMove;
					currentMove = newMove;
				}
				if(currentActor->getType() == 9)
				{
					int distance = 2;
					while(grid->isValidMove(x, y, x, y-distance, team))
					{
						newMove = new MoveList;
						newMove->x1 = x;
						newMove->y1 = y;
						newMove->x2 = x;
						newMove->y2 = y-distance;
						newMove->next = NULL;
						if(!firstMove)
							firstMove = newMove;
						else
							currentMove->next = newMove;
						currentMove = newMove;
						distance++;
					}
				}
				if(grid->isValidMove(x, y, x+1, y, team))
				{
					newMove = new MoveList;
					newMove->x1 = x;
					newMove->y1 = y;
					newMove->x2 = x+1;
					newMove->y2 = y;
					newMove->next = NULL;
					if(!firstMove)
						firstMove = newMove;
					else
						currentMove->next = newMove;
					currentMove = newMove;
				}
				if(currentActor->getType() == 9)
				{
					int distance = 2;
					while(grid->isValidMove(x, y, x+distance, y, team))
					{
						newMove = new MoveList;
						newMove->x1 = x;
						newMove->y1 = y;
						newMove->x2 = x+distance;
						newMove->y2 = y;
						newMove->next = NULL;
						if(!firstMove)
							firstMove = newMove;
						else
							currentMove->next = newMove;
						currentMove = newMove;
						distance++;
					}
				}
				if(grid->isValidMove(x, y, x, y+1, team))
				{
					newMove = new MoveList;
					newMove->x1 = x;
					newMove->y1 = y;
					newMove->x2 = x;
					newMove->y2 = y+1;
					newMove->next = NULL;
					if(!firstMove)
						firstMove = newMove;
					else
						currentMove->next = newMove;
					currentMove = newMove;
				}
				if(currentActor->getType() == 9)
				{
					int distance = 2;
					while(grid->isValidMove(x, y, x, y+distance, team))
					{
						newMove = new MoveList;
						newMove->x1 = x;
						newMove->y1 = y;
						newMove->x2 = x;
						newMove->y2 = y+distance;
						newMove->next = NULL;
						if(!firstMove)
							firstMove = newMove;
						else
							currentMove->next = newMove;
						currentMove = newMove;
						distance++;
					}
				}
			}
		}
	return firstMove;
}

void Player::autoPlacePieces()
{
	int i = 0;
	for (int x = 0; x < 10; x++)
	{
		for (int y = team * 6; y < team * 6 + 4; y++)
		{
			if (grid->getActor(x, y))
				grid->remove(x, y);
			grid->add(actors[i], x, y);
			i++;
		}
	}
}

int Player::loadPlacement(string file)
{
	ifstream in;
	in.open(file.c_str());
	if (!in.is_open()) return 0;

	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			int a;
			in >> a;
			if (getLeft(a) > 0)
			{
				int ax;
				if (team == 1) ax = x;
				else ax = 9 - x;
				int ay;
				if (team == 1) ay = 6 + y;
				else ay = 3 - y;
				grid->add(getNext(a), ax, ay);
			}
			else
				return 0;
		}
	}

	in.close();

	return 1;
}

void Player::removeAll()
{
	for (int x = 0; x < 10; x++)
	{
		for (int y = team * 6; y < team * 6 + 4; y++)
		{
			grid->remove(x, y);
		}
	}
}
