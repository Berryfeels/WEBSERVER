#ifndef _SERVER
# define _SERVER

#include "webserv.class.hpp"

class Server {
	private:	
		int							sockfd;
		std::string					Port;
		std::string					IP;
		std::string					HostName;
		std::string					ServerName;
		std::string					IndexFile;
		long						client_max_body_size;
		std::vector<METHODS>		allowed_methods;
		std::string					upload_path;
		std::vector<LocationConfig>	locations;
		std::map<int, ErrorPages>	Pages;

	Server()
	:	client_max_body_size(0) {}
	public:

};

#endif