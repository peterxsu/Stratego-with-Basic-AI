//
//  Grid.h
//  Stratego
//
//  Created by Alex Lancaster on 11/9/12.
//  Copyright (c) 2012 Alex Lancaster. All rights reserved.
//

#ifndef GRID_H
#define GRID_H

#define SIZE 10

#include "Actor.h"
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

class Player;

struct PastMove
{
	PastMove() { }
	PastMove(int a, int b, int c, int d, Actor * e, Actor * f, Actor * g, Actor * h) { x1 = a; y1 = b; x2 = c; y2 = d; a1 = e; a2 = f; a3 = g; a4 = h;}

	int x1, y1, x2, y2;
	Actor * a1;
	int pasta1known;
	int pasta1moved;
	Actor * a2;
	int pasta2known;
	int pasta2moved;
	Actor * a3;
	Actor * a4;
	int team;
};

class Grid : public sf::Drawable
{
public:
    
	Grid(int);
	Grid(Grid*);
	~Grid();
    
	// adds an actor to the grid at the specified
	// returns 1 if the addition was successful, i.e. the spot was available
	// 0 otherwise
	int add(Actor *, int, int);
    
	// removes the actor at the specified location
	// returns 1 if there was an actor at that location
	// 0 otherwise
	int remove(int, int);
    
	// moves the actor at the specified location
	// to the other specified location
	// returns 1 if the move was successful
	// 0 otherwise
    // passes in x1 y1 x2 y2
	int move(int, int, int, int, int, int clear = 1);

	void undoMove();
	void redoMove();

	
	// returns true if the move is valid
	// false otherwise
	// should check to see if the move is within the range of the piece that is being moved, i.e. the piece isn't being moved diagonally
	// should check to see if the piece is being moved to a space that isn't currently occupied
    // passes in x1 y1 x2 y2
	bool isValidMove(int, int, int, int, int);

	// returns the actor occupying a location
	Actor * getActor(int, int);

	//prints the grid to the screen
	void print();
    
	int getOver() { return isOver; }
	void setOver(int o) {isOver = o;}
	int getWinner() { return winner; }
	void setRevealed(int r) { revealed = r; }
	int getRevealed() { return revealed; }

	//sets the current player to assist drawing.
	//a current player of -1 makes no one's pieces visible, and a current player of 2 makes
	//both player's pieces visible
	void setPlayer(int p) { curPlayer = p; }

	int getAttack() { return attack; }
	int getOffense() { return off; }
	int getDefense() { return def; }

	virtual void draw(sf::RenderTarget &, sf::RenderStates) const;

	//converts world coordinate to grid coordinate. world coordinates are pixels.
	//grid coordinates are [0, columns] x [0, rows] and can be non-integral (i.e. 4.5, 3.5 would be
	//in the middle of the cell at (4,3))
	void toGrid(double worldX, double worldY, double & gridX, double & gridY);
	//converts grid coordinate to world coordinate
	void toWorld(double gridX, double gridY, double & worldX, double & worldY);
	//snaps a world coordinate to grid
	void snapToGrid(double worldX, double worldY, int & cellX, int & cellY);

	static void loadResources();
	
	static sf::Texture actorChars[12];
	static sf::Texture actorTiles[2];

	vector<PastMove> history;
	vector<PastMove> future;
    
private:
    
	Actor *** grid;
	int isOver;
	int winner;
	int revealed;
	int attack;
	int off, def;

	int curPlayer;

	//if owned is set to 1, that means the actors in this grid are owned by player objects,
	//so the grid does not need to delete them when it's deleted.
	//otherwise the actors are not owned by any other object, so the grid must take care of them when it's deleted.
	int owned;
	

};

#endif
