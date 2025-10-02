#ifndef LOCATIONCONFIG_H
#define LOCATIONCONFIG_H

#include "ErrorPages.class.hpp"
#include <string>

struct LocationConfig {
	int									LocLen; // length of locations to determine order (greedy search).
	std::vector<METHODS>				allowed_methods;
	LOCTYPE								type; // 	STANDARD, CGI, REDIRECT
	bool								Autoindex;
	std::string							IndexFile;
	long								client_max_body_size;
	std::string 						Location;
	std::string 						Root;
	std::string							UploadPath;
	std::map<int, std::string>			redirection;
	std::map<int, ErrorPages>			Pages;
	std::map<std::string, std::string>	cgi_ext; // only used if cgi line is present. first string cgi type ".py", ".php",... second string executable home "/usr/bin/python3" "/usr/bin/php-cgi"

	LocationConfig()
	:	LocLen(0),
		type(STANDARD),
		Autoindex(false),
		client_max_body_size(0) {}
};



#endif