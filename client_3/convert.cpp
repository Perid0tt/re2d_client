#include "convert.h"

string ch_tostr(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

string split(string s, string delimiter, int number)
{
	size_t pos = 0;
	string token;
	int i = 1;
	bool sovpad = false;
	while ((pos = s.find(delimiter)) != string::npos)
	{
		token = s.substr(0, pos);
		if (i == number)
		{
			sovpad = true;
			break;
		}
		s.erase(0, pos + delimiter.length());
		i++;
	}
	if (sovpad)return token;
	else return s;
}

double okr(double number, unsigned int accuracy)
{
	return double(int(number * pow(10, accuracy))) / pow(10, accuracy);
}