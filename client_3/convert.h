#pragma once
#include <string>
using namespace std;

struct dualnet_int
{
	int gui, me;
};
struct dualnet_string
{
	string gui, me;
};

string ch_tostr(char*, int);

string split(string, string, int);

double okr(double, unsigned int);