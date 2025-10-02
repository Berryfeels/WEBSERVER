
#ifndef DEFINES_H
#define DEFINES_H


#define STD_SCOPE	"127.0.0.1"
#define STD_PORT	80
// if STD_PORT is used already in the config file the webserver does STD_PORT +1.??
// [ ] TODO Decide wether to implement like that or throw an error.

# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define BOLD    "\033[1m"

enum METHODS {
	GET,
	POST,
	DELETE
};

enum LOCTYPE {
	STANDARD,
	CGI,
	REDIRECT
};

#endif