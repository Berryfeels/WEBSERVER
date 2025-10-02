
#ifndef ERRORPAGES_H
#define ERRORPAGES_H

#include <string>

struct ErrorPages {
	int				Code;
	std::string		path;

	ErrorPages() : Code(0) {}
};

#endif