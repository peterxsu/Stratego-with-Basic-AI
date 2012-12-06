#ifndef INPUT_H
#define INPUT_H

class Input
{
public:

	Input();
	~Input();

	//called once per frame
	void getInput();

	int getMouseState(int i) { return mouseStates[i]; }

	enum MouseState
	{
		PRESSED,
		DOWN,
		RELEASED,
		UP,
	};

private:

	void updateState(int i, int state);

	MouseState mouseStates[3];

};

#endif
