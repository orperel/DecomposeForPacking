#pragma once

enum KEYBOARD_KEY
{
	UNSUPPORTED,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_R
};

class IInputListener
{
public:
	virtual void onKeyPressed(KEYBOARD_KEY key) = 0;
};

