#include "physics.h"

player gui;
player me;

bool pressed = false;
extern dualnet_int killdobjnum;
extern dualnet_bool dobjkilled;

void initialphysics()
{
	gui = player(400, 400);
	me = player(400, 400);
}

void CheckForeginDobjzz() //необходимое локальное вычисление (т к пока что только 2 игрока)
{
	if (dobjkilled.me)killdobjnum.me = -1;
	if (gui.key[5] && !pressed)
	{
		pressed = true;
		for (int i = 0; i < me.dobj_num; i++)
		{
			if (InDistance(25, gui.c, me.dobj[i].c) && killdobjnum.me == -1)
			{
				gui.dobj_num++;
				gui.dobj.resize(gui.dobj_num);
				gui.dobj[gui.dobj_num - 1] = me.dobj[i];
				killdobjnum.me = i;
			}
		}
	}
	if (!gui.key[5]) pressed = false;
}

void setphysics()
{
	me.MoveToSpeed();
	gui.setspeed();
	gui.MoveToSpeed();

	me.MoveDobj();
	gui.MoveDobj();
	gui.SetDobj();
	CheckForeginDobjzz();
}

bool InDistance(float dist, coord point1, coord point2)
{
	if (sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2)) <= dist)return true;
	else return false;
}
