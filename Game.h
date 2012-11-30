#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

using namespace std;

#include "Grid.h"
#include "Player.h"
#include "Input.h"
#include "AI.h"

class Game
{
public:

	Game();
	~Game();

	bool init();
	void reset();
	bool run();
	void cleanUp();

	void displayMenu();
	void playGame();
	void setupGame();

	void setPlayer(int p) { curPlayer = p; if (grid) grid->setPlayer(p); }

	//utility function to draw text
	sf::Text getText(double tx, double ty, string str, int size, sf::Color col = sf::Color(0, 0, 0, 255));
	int mouseOver(int x1, int y1, int x2, int y2);
	sf::Vector2f getMousePos();
	int button(int x, int y, sf::Texture texture);
	void highlightTile(int x, int y, sf::Color col);
	void drawBg();
	void setInfo(string str);
	void setInfoHeight(int h);
	void drawInfo();
	void switchPlayers();

private:

	Grid * grid;
	Player * players[2];

	Input * input;

	//which player is currently going?
	int curPlayer;
	//which type of piece is being placed?
	int placePiece;
	//which piece is selected to move?
	Actor * selectedPiece;
	int selectedX, selectedY;
	//is it between players' turns?	
	int waiting;
	//player mode - 0 = h vs h, 1 = h vs ai, 2 = ai vs ai
	int playerMode;
	int spaceDown;

	// Graphics resources
	sf::RenderWindow win;
	sf::View view;
	sf::Transform transform;
	sf::Font font;
	sf::Texture bg;
	sf::Texture title;
	sf::Texture buttons[3];
	sf::RenderTexture infoImg;
	int infoHeight;

	//-----------------------------------------------------

	enum GameState
	{
		MENU,
		SET,
		PLAY,
	};

	enum MenuState
	{
		MAIN,
		OPTIONS,
	};

	GameState state;
	MenuState menuState;

};

#endif
