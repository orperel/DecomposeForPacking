#pragma once

enum KEYBOARD_KEY
{
	UNSUPPORTED,
	KEY_LEFT,
	KEY_RIGHT
};

class IInputListener
{
public:
	virtual void onKeyPressed(KEYBOARD_KEY key) = 0;
};

