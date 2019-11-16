#pragma once
struct coord
{
	int x = 0, y = 0;
};

struct player
{
	coord c;
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
	void move()
	{
		if (wasd[0]) c.y-=3;
		if (wasd[1]) c.x-=3;
		if (wasd[2]) c.y+=3;
		if (wasd[3]) c.x+=3;
	}
	void moveto(int x, int y)
	{
		c.x = x;
		c.y = y;
	}
};

void setphysics();