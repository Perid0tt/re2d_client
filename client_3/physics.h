#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include "convert.h"


using namespace std;


struct coord
{
	float x = 0, y = 0;
};

struct dir
{
	float angle = 0, value = 0;
};

enum DobjType
{
	null,
	ball,
	bullet
};


class DinamicObj
{
public:
	coord c;
	dir speed;
	int type = ball;
	bool taken = true;

	void setmove(coord cord, dir spd)
	{
		if (taken)
		{
			speed = spd;
			c.x = cord.x + 10 * cos(speed.angle);
			c.y = cord.y + 10 * sin(speed.angle);
		}
	}
	void move()
	{
		c.x += speed.value * cos(speed.angle);
		c.y += speed.value * sin(speed.angle);
	}
};


class player
{
public:
	player()
	{
		InitDobj();
	}
	player(int x, int y)
	{
		InitDobj();
		MoveToCord(x, y);
	}

	coord c;
	dir speed;
	int a = 5;
	bool key[6] = { 0,0,0,0,0,0 };
	void getkeys(char C_wasd[])
	{
		if (C_wasd[0] == 'w')key[0] = 1;
		else key[0] = 0;
		if (C_wasd[1] == 'a')key[1] = 1;
		else key[1] = 0;
		if (C_wasd[2] == 's')key[2] = 1;
		else key[2] = 0;
		if (C_wasd[3] == 'd')key[3] = 1;
		else key[3] = 0;
		if (C_wasd[4] == '<')key[4] = 1;
		else key[4] = 0;
		if (C_wasd[5] == '>')key[5] = 1;
		else key[5] = 0;
	}
	void setspeed()
	{
		int x, y, gip;

		x = int(key[3]) - int(key[1]);
		y = int(key[2]) - int(key[0]);

		if (y == 1 && x == 0) speed.angle = M_PI_2;
		if (y == 1 && x == 1) speed.angle = M_PI_4;
		if (y == 0 && x == 1) speed.angle = 0;
		if (y == -1 && x == 1) speed.angle = -M_PI_4;
		if (y == -1 && x == 0) speed.angle = -M_PI_2;
		if (y == -1 && x == -1) speed.angle = -M_PI_4 * 3;
		if (y == 0 && x == -1) speed.angle = M_PI;
		if (y == 1 && x == -1) speed.angle = M_PI_4 * 3;

		if (x != 0 || y != 0) speed.value = 3;
		else speed.value = 0;
	}
	void MoveToSpeed()
	{
		c.x += speed.value * cos(speed.angle);
		c.y += speed.value * sin(speed.angle);
	}
	void MoveToCord(int x, int y)
	{
		c.x = x;
		c.y = y;
	}

	vector<DinamicObj> dobj;
	int dobj_num = 1;
	void InitDobj()
	{
		vector<DinamicObj>().swap(dobj);
		dobj.resize(dobj_num);
	}
	void SetDobj()
	{
		dobj.resize(dobj_num);
		for (int i = 0; i < dobj_num; i++)
		{
			if (key[4]) dobj[i].taken = false;
			if (key[5]) dobj[i].taken = true;
			dobj[i].setmove(c, speed);
		}
	}
	void MoveDobj()
	{
		dobj.resize(dobj_num);
		for (int i = 0; i < dobj_num; i++)
		{
			dobj[i].move();
		}
	}
};

void setphysics();
void initialphysics();