#include "./webserver.class.hpp"

int main ()
{
	//create a request;
	//Request request;
	//request.head = Header();
	std::string answer;
	WebServManager manager;
	//a request has a header;
	std::signal(SIGINT, manager.handle_sigint);
	
	//in the header there are the info that
	//the server needs to fetch the files needed

	request.head.Method = "HTTP 1/1";
	request.head.Host = "google.com 80";
	request.head.Connection = "close";

	//the server fetch the files and send them back
	//to the local host.

	//initialize servers with parser
	manager.ServerConfig();
	manager.runAllServers();
	answer= request.ConnectToServer(request.head);
	return 0;
}
