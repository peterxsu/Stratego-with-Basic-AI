#include "Game.h"

#include <iostream>
#include <sstream>
using namespace std;

Game::Game()
{
	grid = NULL;
	players[0] = NULL;
	players[1] = NULL;

	input = NULL;

	curPlayer = 0;
	placePiece = 0;
	selectedPiece = NULL;
	selectedX = 0;
	selectedY = 0;
	infoHeight = 0;

	waiting = 0;

	playerMode = 0;

	spaceDown = 0;
}

Game::~Game()
{
}

bool Game::init()
{	 
	state = MENU;
	menuState = MAIN;

	win.create(sf::VideoMode(800, 600), "Stratego");//, sf::Style::Fullscreen);
	view = sf::View(sf::FloatRect(0, 0, 800, 600));
	transform = sf::Transform::Identity;


	if (!font.loadFromFile("Sansation_Regular.ttf")) return false;
	if (!bg.loadFromFile("bg.png")) return false;
	if (!title.loadFromFile("title.png")) return false;

	input = new Input;

	buttons[0].loadFromFile("hvh.png");
	buttons[1].loadFromFile("hva.png");
	buttons[2].loadFromFile("ava.png");

	Grid::loadResources();

	return true;
}

void Game::reset()
{
	if (grid) delete grid;
	if (players[0]) delete players[0];
	if (players[1]) delete players[1];

	grid = new Grid();
	switch (playerMode)
	{
	case 0:
		players[0] = new Player(1, 0, grid);
		players[1] = new Player(1, 1, grid);
		break;
	case 1:
		players[0] = new Player(1, 0, grid);
		players[1] = new Player(0, 1, grid);
		break;
	case 2:
		players[0] = new Player(0, 0, grid);
		players[1] = new Player(0, 1, grid);
		break;
	}	

	curPlayer = 0;
	selectedPiece = NULL;
	placePiece = 0;
	selectedX = 0;
	selectedY = 0;
	infoHeight = 240;

	setPlayer(0);
	setInfo("Player 1, place your\npieces.\nPress space when\nyou're done.");

	waiting = 0;
}

bool Game::run()
{

	//check for events
	sf::Event e;
	while (win.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			return false;
		}
		if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::Escape)
			{
				return false;
			}
		}
		if (e.type == sf::Event::Resized)
		{
			float width = win.getSize().x;
			float height = win.getSize().y;
			view = sf::View(sf::FloatRect(0, 0, width, height));
			win.setSize(sf::Vector2u(width, height));
			win.setView(view);
			transform = sf::Transform::Identity;
			float scale = min(view.getSize().x / 800.0f, view.getSize().y / 600.0f);
			transform.scale(scale, scale);
		}
	}

	input->getInput();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) == 0) spaceDown = 0;

	//------------------------------------------------------

	win.clear(sf::Color(80, 80, 80, 255));

	switch (state)
	{
	case MENU:
		win.clear(sf::Color(120, 150, 230, 255));
		displayMenu();
		break;

	case SET:
		setupGame();
		break;

	case PLAY:
		playGame();
		break;
	}

	win.display();

	return true;
}

void Game::cleanUp()
{
	if (grid) delete grid;
	if (players[0]) delete players[0];
	if (players[1]) delete players[1];

	if (input) delete input;

	if (win.isOpen()) win.close();
}

void Game::displayMenu()
{
	win.draw(sf::Sprite(title), transform);
	for (int i = 0; i < 3; i++)
	{
		if (button(400, 400 + 60 * i, buttons[i]))
		{
			playerMode = i;
			reset();
			state = SET;
		}
	}
}

void Game::playGame()
{
	drawBg();
	if (players[curPlayer]->getHuman() == 0)
	{
		grid->setRevealed(0);
		win.draw(*grid, transform);
		grid->setRevealed(1);
	}
	else
		win.draw(*grid, transform);
	drawInfo();


	if (grid->getOver())
	{
		//game is over, do something
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && spaceDown == 0)
		{
			spaceDown = 1;
			state = MENU;
		}
	}
	else
	{
		if (waiting == 1)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && spaceDown == 0)
			{
				spaceDown = 1;
				waiting = 0;
				grid->setRevealed(1);
				if (curPlayer == 0)
					setInfo("Player 1, click on\n a piece to move it.");
				else
					setInfo("Player 2, click on\n a piece to move it.");
			}
		}
		else
		{
			if (!players[curPlayer]->getHuman())
			{
				players[curPlayer]->makeMove();
				selectedPiece = NULL;
				//if the game is over then we make both player's pieces visible
				//otherwise change to other player
				if (!grid->getOver())
				{
					/*
					if (curPlayer == 0)
						//setInfo("Player 2, click on\n a piece to move it.")
							
					else
						setInfo("Player 1, click on\n a piece to move it."); */
					switchPlayers();
				}
				else
				{
					if (grid->getWinner() == 0)
						setInfo("Player 1, you win!\nPress space to return\nto the main menu.");
					else
						setInfo("Player 2, you win!\nPress space to reutnr\nto the main menu.");
					setPlayer(2);
				}
				return;
			}
			if (input->getMouseState(1) == Input::PRESSED)
			{
				grid->undoMove();
			}
			sf::Vector2i tpos;
			tpos.x = getMousePos().x / 60.0f;
			tpos.y = getMousePos().y / 60.0f;
			if (input->getMouseState(0) == Input::PRESSED)
			{
				if (grid->getActor(tpos.x, tpos.y))
				{
					if (grid->getActor(tpos.x, tpos.y)->getTeam() == curPlayer)
					{
						selectedPiece = grid->getActor(tpos.x, tpos.y);
						selectedX = tpos.x;
						selectedY = tpos.y;
					}
				}
			}

			if (selectedPiece)
			{
				highlightTile(selectedX, selectedY, sf::Color(0, 0, 255, 80));
				if (selectedPiece->getType() == 9)
				{
					for (int x = 0; x < 10; x++)
					{
						if (grid->isValidMove(selectedX, selectedY, x, selectedY, players[curPlayer]->getTeam()))
						{
							highlightTile(x, selectedY, sf::Color(255, 255, 0, 100));
						}
					}
					for (int y = 0; y < 10; y++)
					{
						if (grid->isValidMove(selectedX, selectedY, selectedX, y, players[curPlayer]->getTeam()))
						{
							highlightTile(selectedX, y, sf::Color(255, 255, 0, 100));
						}
					}
				}
				else
				{
					if (grid->isValidMove(selectedX, selectedY, selectedX - 1, selectedY, players[curPlayer]->getTeam()))
						highlightTile(selectedX - 1, selectedY, sf::Color(255, 255, 0, 100));
					if (grid->isValidMove(selectedX, selectedY, selectedX + 1, selectedY, players[curPlayer]->getTeam()))
						highlightTile(selectedX + 1, selectedY, sf::Color(255, 255, 0, 100));
					if (grid->isValidMove(selectedX, selectedY, selectedX, selectedY - 1, players[curPlayer]->getTeam()))
						highlightTile(selectedX, selectedY - 1, sf::Color(255, 255, 0, 100));
					if (grid->isValidMove(selectedX, selectedY, selectedX, selectedY + 1, players[curPlayer]->getTeam()))
						highlightTile(selectedX, selectedY + 1, sf::Color(255, 255, 0, 100));
				}
				if (input->getMouseState(0) == Input::PRESSED)
				{
					if (grid->isValidMove(selectedX, selectedY, tpos.x, tpos.y, players[curPlayer]->getTeam()))
					{
						grid->move(selectedX, selectedY, tpos.x, tpos.y, players[curPlayer]->getTeam());
						selectedPiece = NULL;
						//if the game is over then we make both player's pieces visible
						//otherwise change to other player
						if (!grid->getOver())
						{
							/*
							if (curPlayer == 0)
								//setInfo("Player 2, click on\n a piece to move it.");
							
							else
								setInfo("Player 1, click on\n a piece to move it."); */
							switchPlayers();
						}
						else
						{
							if (grid->getWinner() == 0)
								setInfo("Player 1, you win!\nPress space to return\nto the main menu.");
							else
								setInfo("Player 2, you win!\nPress space to reutnr\nto the main menu.");
							setPlayer(2);
						}
					}
				}
			}
		}
	}
}

void Game::setupGame()
{
	if (!players[curPlayer]->getHuman())
	{
		if (curPlayer == 0)
		{
			players[curPlayer]->placePieces();
			setPlayer(1);
			setInfo("Player 2, place your\npieces.\nPress space when\nyou're done.");
		}
		else
		{
			players[curPlayer]->placePieces();
			switchPlayers();
			state = PLAY;
			return;
		}
		
	}
	drawBg();
	win.draw(*grid, transform);
	drawInfo();

	sf::Sprite tileSprite;
	sf::Sprite tileBgSprite;
	tileBgSprite.setTexture(Grid::actorTiles[curPlayer]);

	for (int i = 0; i <= 11; i++)
	{
		tileSprite.setTexture(Grid::actorChars[i]);
		tileSprite.setPosition(610 + 54 * (i >= 6), 264 + i % 6 * 54);
		tileBgSprite.setPosition(610 + 54 * (i >= 6), 264 + i % 6 * 54);
		sf::FloatRect rect(tileSprite.getGlobalBounds());
		if (mouseOver(rect.left, rect.top, rect.left + rect.width, rect.top + rect.height) || placePiece == i)
		{
			tileBgSprite.setColor(sf::Color(255, 255, 255, 100));
			tileSprite.setColor(sf::Color(255, 255, 255, 100));
			if (input->getMouseState(0) == Input::PRESSED)
			{
				placePiece = i;
			}
		}
		else
		{
			tileBgSprite.setColor(sf::Color(255, 255, 255, 255));
			tileSprite.setColor(sf::Color(255, 255, 255, 255));
		}
			
		if (players[curPlayer]->getLeft(i))
		{
			win.draw(tileBgSprite, transform);
			win.draw(tileSprite, transform);
		}
	}


	sf::Vector2i tpos;
	tpos.x = getMousePos().x / 60.0f;
	tpos.y = getMousePos().y / 60.0f;
	if (tpos.x < 10 && tpos.x >= 0)
	{
		if (tpos.y >= curPlayer * 6 && tpos.y <= curPlayer * 6 + 3)
		{
			highlightTile(tpos.x, tpos.y, sf::Color(0, 0, 255, 80));
			if (input->getMouseState(0) == Input::PRESSED)
			{
				if (players[curPlayer]->getLeft(placePiece))
					grid->add(players[curPlayer]->getNext(placePiece), tpos.x, tpos.y);
			}
			else if (input->getMouseState(1) == Input::PRESSED)
			{
				grid->remove(tpos.x, tpos.y);
			}
		}
		else
			highlightTile(tpos.x, tpos.y, sf::Color(255, 0, 0, 120));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && players[curPlayer]->getTotalLeft() == 0)
	{
		spaceDown = 1;
		if (curPlayer == 0)
		{
			setPlayer(1);
			setInfo("Player 2, place your\npieces.\nPress space when\nyou're done.");
		}
		else
		{
			switchPlayers();
			state = PLAY;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		//players[curPlayer]->autoPlacePieces();
		players[curPlayer]->removeAll();
		if (!players[curPlayer]->loadPlacement("template1.dat")) cout << "error\n";
	}

}

sf::Text Game::getText(double tx, double ty, string str, int size, sf::Color col)
{
	sf::Text text;

	text.setFont(font);

	text.setCharacterSize(size);
	text.setColor(col);
	text.setPosition(tx, ty);
	text.setString(str);

	return text;
}

int Game::mouseOver(int x1, int y1, int x2, int y2)
{
	int mx = getMousePos().x;
	int my = getMousePos().y;

	//int mx = sf::Mouse::getPosition(win).x;
	//int my = sf::Mouse::getPosition(win).y;

	//sf::Vector2f p1 = transform.transformPoint(sf::Vector2<float>(x1, y1));
	//sf::Vector2f p2 = transform.transformPoint(sf::Vector2<float>(x2, y2));

	//if (mx >= p1.x && mx <= p2.x && my >= p1.y && my <= p2.y)
	if (mx > x1 && mx < x2 && my > y1 && my < y2)
		return 1;
	else
		return 0;
}

sf::Vector2f Game::getMousePos()
{
	return transform.getInverse().transformPoint(sf::Vector2<float>(sf::Mouse::getPosition(win)));
}

int Game::button(int x, int y, sf::Texture texture)
{
	sf::Sprite buttonSpr(texture);
	buttonSpr.setPosition(x, y);
	float w = texture.getSize().x;
	float h = texture.getSize().y;
	buttonSpr.setOrigin(w / 2, h / 2);
	int mo = 0;
	if (mouseOver(x - w / 2, y - h / 2, x + w / 2, y + h / 2))
	{
		buttonSpr.setScale(0.8, 0.8);
		mo = 1;
	}

	win.draw(buttonSpr, transform);

	return mo && input->getMouseState(0) == Input::PRESSED;
}

void Game::highlightTile(int x, int y, sf::Color col)
{
	sf::RectangleShape rect(sf::Vector2f(60.0f, 60.0f));
	rect.setPosition(x * 60, y * 60);
	rect.setFillColor(col);
	win.draw(rect, transform);
}

void Game::drawBg()
{
	sf::Sprite spr(bg);
	spr.setPosition(0, 0);
	win.draw(spr, transform);
}

void Game::setInfo(string str)
{
	infoImg.create(170, infoHeight);
	infoImg.clear(sf::Color(255, 255, 255, 255));

	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setPosition(10, 10);
	text.setColor(sf::Color(0, 0, 0, 255));
	text.setCharacterSize(14);
	
	infoImg.draw(text);

	sf::RectangleShape rect(sf::Vector2f(168, infoHeight - 2));
	rect.setOutlineThickness(2);
	rect.setOutlineColor(sf::Color(0, 0, 0, 255));
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	rect.setPosition(1, 1);

	infoImg.draw(rect);

	infoImg.display();
}

void Game::setInfoHeight(int h)
{
	infoHeight = h;
}

void Game::drawInfo()
{
	sf::Sprite infoSpr(infoImg.getTexture());
	infoSpr.setPosition(615, 15);
	win.draw(infoSpr, transform);
}

void Game::switchPlayers()
{
	setPlayer(1 - curPlayer);
	stringstream stream;
	if (grid->getAttack())
	{
		stream << Player::typeToString(grid->getOffense()) << " attacked\n" << Player::typeToString(grid->getDefense()) << "!\n";
		if (grid->getOffense() < grid->getDefense())
			stream << Player::typeToString(grid->getOffense()) << " won!\n";
		else
			if (grid->getOffense() == grid->getDefense())
				stream << "It was a tie!\n";
			else
				stream << Player::typeToString(grid->getDefense()) << " won!\n";
	}
	stream << "Player " << (curPlayer + 1) << ", press the\nspacebar to begin your\nturn.";
	setInfo(stream.str());
	waiting = 1;
	grid->setRevealed(0);
}










