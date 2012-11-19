#include <iostream>

#include "Actor.h"
#include "Grid.h"
#include "Player.h"
#include "Game.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main()
{
	
	Game game;

	if (!game.init()) return 0;
	while (game.run());
	game.cleanUp();

	return 0;
}
