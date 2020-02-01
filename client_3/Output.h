#pragma once

#include <sstream>

//...standard class body

class LStream : public std::stringbuf {
public:
	int sync();
};