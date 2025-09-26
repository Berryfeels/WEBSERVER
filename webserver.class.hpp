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
#include <sys/socket.h>
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

using std::string;

#define MAX_CLIENTS 100
#define TIMEOUT 50000

class Header{
	private:
	
	public:
	string Method;
	string Host;
	string Connection;
	string getMethod(){
		return Method;
	}
	string getHost(){
		return Host;
	}
	string getConnection(){
		return Connection;
	}

};

 class Request {
	private:
	
	public:
	Header head;
	string ConnectToServer(Header head);
 };

 class Answer {
	public:
	std::string answer;
 };

 #endif