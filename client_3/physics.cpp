#include "physics.h"

player gui;
player me;

void initialphysics()
{
	gui = player(400, 400);
	me = player(400, 400);
}

void setphysics()
{
	me.MoveToSpeed();
	gui.setspeed();
	gui.MoveToSpeed();

	me.MoveDobj();
	gui.MoveDobj();
	gui.SetDobj();
}
