#include "physics.h"

player gui;
player me;

double okr(double number, unsigned int accuracy)
{
	return double(int(number * pow(10, accuracy))) / pow(10, accuracy);
}

void setphysics()
{
	me.MoveToSpeed();
	gui.setspeed();
	gui.MoveToSpeed();
}
