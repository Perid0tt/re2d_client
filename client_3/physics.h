#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <windows.h>

using namespace std;

double okr(double, unsigned int);

struct coord
{
	float x = 0, y = 0;
};

struct dir
{
	float angle, value;
};

class player
{
public:
	coord c;
	dir speed;
	bool wasd[4] = { 0,0,0,0 };
	void getwasd(char C_wasd[])
	{
		if (C_wasd[0] == 'w')wasd[0] = 1;
		else wasd[0] = 0;
		if (C_wasd[1] == 'a')wasd[1] = 1;
		else wasd[1] = 0;
		if (C_wasd[2] == 's')wasd[2] = 1;
		else wasd[2] = 0;
		if (C_wasd[3] == 'd')wasd[3] = 1;
		else wasd[3] = 0;
	}
	void setspeed()
	{
		int x, y, gip;

		x = int(wasd[3]) - int(wasd[1]);
		y = int(wasd[2]) - int(wasd[0]);

		if (y == 1 && x == 0) speed.angle = M_PI_2;
		if (y == 1 && x == 1) speed.angle = M_PI_4;
		if (y == 0 && x == 1) speed.angle = 0;
		if (y == -1 && x == 1) speed.angle = -M_PI_4;
		if (y == -1 && x == 0) speed.angle = -M_PI_2;
		if (y == -1 && x == -1) speed.angle = -M_PI_4 * 3;
		if (y == 0 && x == -1) speed.angle = M_PI;
		if (y == 1 && x == -1) speed.angle = M_PI_4 * 3;

		if (x != 0 || y != 0) speed.value = 0.5;
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
};

void physics();