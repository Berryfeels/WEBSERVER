/*
1 create class request with
 - header
 	- Host
	- Connection
 - connection method
	- con dentro le connessioni su socket ecc. 
2 create an answer class
 - Header
 - method to print out*/
#ifndef _WEBSERVER
# define _WEBSERVER

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <csignal>
#include <vector>
#include <memory>
#include <map>
#include <stdexcept>

using std::string;

#define MAX_CLIENTS 100
#define TIMEOUT 50000

#include "TCPSocket.class.hpp"
#include "LocationConfig.class.hpp"
#include "ErrorPages.class.hpp"
#include "Defines.class.hpp"

// class Header{
// 	private:
	
// 	public:
// 	string Method;
// 	string Host;
// 	string Connection;
// 	string getMethod(){
// 		return Method;
// 	}
// 	string getHost(){
// 		return Host;
// 	}
// 	string getConnection(){
// 		return Connection;
// 	}

// };

//  class Request {
// 	private:
	
// 	public:
// 	Header head;
// 	string ConnectToServer(Header head);
//  };

//  class Answer {
// 	public:
// 	std::string answer;
//  };

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

class WebServManager {
private:
    std::vector<Server>	Servers;//Clause: std::vector<std::unique_ptr<Server>> servers;
    	std::vector<std::unique_ptr<TCPSocket>> _sockets; // contains all sockets including the bind socket
	std::vector<pollfd> _pollfds;					   // contains all pollfds for the sockets

	void _handle_client_data(std::unique_ptr<TCPSocket> &client_socket); // handle incoming client data
	std::map<int, ErrorPages>	Pages;
	std::vector<struct pollfd>	master_fds;
    long						client_max_body_size;
	std::vector<METHODS>		allowed_methods;
	//ConfigData() : client_max_body_size(0) {}
	bool running;
    
public:
    time_t client_timestamp[MAX_CLIENTS];
    void addServer(const Server& config);
    void runWebserv();
    void shutdown();

	/*WebServer(const std::string& host, int port);
    ~WebServer();
    bool initialize();
    void run();
    void shutdown();
    int getSocketFd() const { return sockfd; }
    int getPort() const { return port; }*/
};

#endif