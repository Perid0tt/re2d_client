#include "physics.h"

player gui;
player me;

void initialphysics()
{
	gui.MoveToCord(400, 400);
	me.MoveToCord(400, 400);
}

void setphysics()
{
	me.MoveToSpeed();
	gui.setspeed();
	gui.MoveToSpeed();
}
