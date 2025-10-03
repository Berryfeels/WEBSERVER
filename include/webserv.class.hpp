#ifndef _WEBSERV
# define _WEBSERV

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
#include "Server.class.hpp"

class WebServManager {
private:
    std::vector<Server>						Servers_;	//Clause: std::vector<std::unique_ptr<Server>> servers;
    std::vector<std::unique_ptr<TCPSocket>> sockets_;	// contains all sockets including the bind socket
	std::vector<pollfd> 					pollfds_;	// contains all pollfds for the sockets
	std::map<int, std::unique_ptr<TCPSocket>> clients_; //sockets of the client
	void _handle_client_data(std::unique_ptr<TCPSocket> &client_socket); // handle incoming client data
	std::map<int, ErrorPages>				Pages;
	//PLACE HOLDER FOR PARSING PORTS
    std::vector<int> 						ports_ = {8080, 8081, 8082};
	long									client_max_body_size;
	std::vector<METHODS>					allowed_methods;
	//ConfigData() : client_max_body_size(0) {}
	bool 									running;
    
public:
	time_t 									client_timestamp[MAX_CLIENTS];
	
	void initData();
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

void	handle_sigint(int signal);
void	cleanup_resources(pollfd *fds, int tcpSocket);
void	handleClientTimeouts(struct pollfd* fds, time_t* timestamps, time_t current_time);
int		pollout(int fd);
#endif