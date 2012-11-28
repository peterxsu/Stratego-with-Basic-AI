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
}

Game::~Game()
{
}

bool Game::init()
{	 
	state = MENU;

	win.create(sf::VideoMode(1366, 768), "Stratego", sf::Style::Fullscreen);
	view = sf::View(sf::FloatRect(0, 0, 800, 600));
	transform = sf::Transform::Identity;

	input = new Input;

	if (!font.loadFromFile("dum1.ttf")) return false;
	if (!bg.loadFromFile("bg.png")) return false;
	if (!title.loadFromFile("title.png")) return false;

	playImg.loadFromFile("play.png");
	optionsImg.loadFromFile("options.png");

	Grid::loadResources();

	return true;
}

void Game::reset()
{
	if (grid) delete grid;
	if (players[0]) delete players[0];
	if (players[1]) delete players[1];

	grid = new Grid();
	players[0] = new Player(0, grid);
	players[1] = new Player(1, grid);

	curPlayer = 0;
	selectedPiece = NULL;
	placePiece = 0;
	selectedX = 0;
	selectedY = 0;

	setPlayer(0);
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

	//------------------------------------------------------

	win.clear(sf::Color(80, 80, 80, 255));

	switch (state)
	{
	case MENU:
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
	button(400, 500, optionsImg);
	if (button(400, 400, playImg))
	{
		reset();
		state = SET;
	}
}

void Game::playGame()
{
	drawBg();
	win.draw(*grid);


	if (grid->getOver())
	{
		//game is over, do something
	}
	else
	{
		sf::Vector2i tpos;
		tpos.x = sf::Mouse::getPosition().x / 60.0f;
		tpos.y = sf::Mouse::getPosition().y / 60.0f;
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
					if (grid->isValidMove(selectedX, selectedY, x, selectedY, players[curPlayer]))
					{
						highlightTile(x, selectedY, sf::Color(255, 255, 0, 100));
					}
				}
				for (int y = 0; y < 10; y++)
				{
					if (grid->isValidMove(selectedX, selectedY, selectedX, y, players[curPlayer]))
					{
						highlightTile(selectedX, y, sf::Color(255, 255, 0, 100));
					}
				}
			}
			else
			{
				if (grid->isValidMove(selectedX, selectedY, selectedX - 1, selectedY, players[curPlayer]))
					highlightTile(selectedX - 1, selectedY, sf::Color(255, 255, 0, 100));
				if (grid->isValidMove(selectedX, selectedY, selectedX + 1, selectedY, players[curPlayer]))
					highlightTile(selectedX + 1, selectedY, sf::Color(255, 255, 0, 100));
				if (grid->isValidMove(selectedX, selectedY, selectedX, selectedY - 1, players[curPlayer]))
					highlightTile(selectedX, selectedY - 1, sf::Color(255, 255, 0, 100));
				if (grid->isValidMove(selectedX, selectedY, selectedX, selectedY + 1, players[curPlayer]))
					highlightTile(selectedX, selectedY + 1, sf::Color(255, 255, 0, 100));
			}
			if (input->getMouseState(0) == Input::PRESSED)
			{
				if (grid->isValidMove(selectedX, selectedY, tpos.x, tpos.y, players[curPlayer]))
				{
					grid->move(selectedX, selectedY, tpos.x, tpos.y, players[curPlayer]);
					selectedPiece = NULL;
					setPlayer(1 - curPlayer);
				}
			}
		}
	}
}

void Game::setupGame()
{
	drawBg();

	win.draw(*grid, transform);

	stringstream stream;
	stream << "Player " << (curPlayer + 1) << ", place";

	win.draw(getText(615, 5, stream.str(), 35, sf::Color(255, 255, 255, 255)), transform);
	win.draw(getText(615, 25, "your pieces.", 35, sf::Color(255, 255, 255, 255)), transform);

	sf::Sprite tileSprite;
	sf::Sprite tileBgSprite;
	tileBgSprite.setTexture(Grid::actorTiles[curPlayer]);

	for (int i = 0; i <= 11; i++)
	{
		tileSprite.setTexture(Grid::actorChars[i]);
		tileSprite.setPosition(610 + 70 * (i >= 6), 150 + i % 6 * 60);
		tileBgSprite.setPosition(610 + 70 * (i >= 6), 150 + i % 6 * 60);
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
		if (curPlayer == 0)
			setPlayer(1);
		else
		{
			setPlayer(0);
			state = PLAY;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		players[curPlayer]->autoPlacePieces();
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
