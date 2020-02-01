#include "Output.h"

int LStream::sync() {
	str("");
	return std::stringbuf::sync();
}