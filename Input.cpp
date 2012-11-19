#include "Input.h"
#include <SFML/Window.hpp>

Input::Input()
{
	for (int i = 0; i < 3; i++) mouseStates[i] = UP;
}

Input::~Input()
{
}

void Input::getInput()
{
	updateState(0, sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
	updateState(1, sf::Mouse::isButtonPressed(sf::Mouse::Button::Right));
	updateState(2, sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle));
}

void Input::updateState(int i, int state)
{
	if (state == 0)
	{
		if (mouseStates[i] == DOWN || mouseStates[i] == PRESSED)
			mouseStates[i] = RELEASED;
		else
			mouseStates[i] = UP;
	}
	else
	{
		if (mouseStates[i] == DOWN || mouseStates[i] == PRESSED)
			mouseStates[i] = DOWN;
		else
			mouseStates[i] = PRESSED;
	}
}