#include "webserver.class.hpp"
using std::string;


bool signal_flag = true;

// Cleanup function
void cleanup_resources(pollfd *fds, int tcpSocket)
{
    std::cout << "\nCleaning up resources..." << std::endl;
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (fds[i].fd != -1)
			close(fds[i].fd);
	}
	close(tcpSocket);
}

// Signal handler function
void handle_sigint(int signal) {
    std::cout << "\nReceived signal " << signal << " (Ctrl+C)" << std::endl;
    signal_flag = false;
}


void handleClientTimeouts(struct pollfd* fds, time_t* timestamps, time_t current_time)
{
	printf("The current function is: %s\n", __func__);
    for (int i = 1; i < MAX_CLIENTS; i++) {
        if (timestamps[i] != 0) { // Active connection
            int idle_time = current_time - timestamps[i];
            
            if (idle_time > 10) { // 10 second timeout
                std::cout << "Connection " << fds[i].fd << " timed out after " 
                         << idle_time << " seconds" << std::endl;
                
                // Send timeout response if possible
                string timeout_response = "HTTP/1.0 408 Request Timeout\r\n\r\n";
                write(fds[i].fd, timeout_response.c_str(), timeout_response.length());
                
                // Close connection
                close(fds[i].fd);
                fds[i].fd = -1;
                fds[i].events = 0;
                fds[i].revents = 0;
                timestamps[i] = 0;
            }
        }
    }
}

int pollout(int fd)
{
	int success;
	string response = "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\nFuck you bitch!\n";
	success = write(fd, response.c_str(), response.length());
	return success;
}

string  Server::ConnectToServer()
{
	//create a socket;
	char	request[2000];//buffer to fetch the client request text
	int		tcpSocket;//the fd for the server socket
	int		ready;//return of poll() indicating if a fd is ready to performe I/O
	int		client_fd;
	int		i;
	//Struct per poll(), che eitera la serie di fd relativi alle richieste di connessione
	struct pollfd fds[MAX_CLIENTS];
	time_t client_timestamp[MAX_CLIENTS];
	time_t current_time;
	struct sockaddr_in addr;
	struct sockaddr_in addr_client;
	std::signal(SIGINT, handle_sigint);

	//create and connect the socket to a server(local in this case);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8080);
	
	tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (tcpSocket < 0) {
        perror("socket");
		cleanup_resources(fds, tcpSocket);
        return "";
    }
	/*bind the socket to an address and port
	The address can be a specific IP (like 127.0.0.1 for local access only) 
	or an empty string ('') to make the server accessible on all available network interfaces.*/
    if (bind(tcpSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
		cleanup_resources(fds, tcpSocket);
        return "";
    }
	
	//listen to request
	if (listen(tcpSocket, MAX_CLIENTS) == -1)// MAX_CLIENTS is the arbitrary amount accepted to que. 
											//  At MAX_CLIENTS + 1 requests, listen will send an error to the client
	{
		std::cout<< errno << std::endl;
		return NULL;
	}

	// Inizializza di fds(pollfd), tutti gli elementi a -1 (fd non valido)
	for (i = 0; i < MAX_CLIENTS; i++) 
	{
		fds[i].fd = -1;
		fds[i].events = 0;
		fds[i].revents = 0;
		client_timestamp[i] = 0;
    }
	fds[0].fd = tcpSocket;
	fds[0].events = POLLIN;

	while(signal_flag)
	{
		//TIMEOUT CHECK
		current_time = time(NULL);//set current time
		//NOTIFIES INCOMING CONNECTIONS
		int ready = poll(fds, MAX_CLIENTS, TIMEOUT);
		if (ready == 0){
			std::cout<< "Poll Timeout."<<std::endl;
			continue;
		}
		else if (ready < 0)
		{
			if(errno == EINTR)
				std::cerr<<"Interrupted by signal."<< std::endl;
			else
				std::cerr<<"Error:poll():ready < 0"<<std::endl;
			cleanup_resources(fds, tcpSocket);
			return "poll error";
		}
		else if (ready > 0)
		{
			//TIMEOUT CHECK
			handleClientTimeouts(fds, client_timestamp, current_time);
			//SIGNAL CHECK FOR CLEAN
			if(signal_flag == false)
			{
				cleanup_resources(fds, tcpSocket);
				std::exit(0); // Or use exit(EXIT_SUCCESS);
			}
			//NEW CLIENT ADDITION
			if(fds[0].revents & POLLIN)
			{
				//initialize values for new client(request) and create new socket
				socklen_t addr_len = sizeof(addr_client);
				client_fd = accept(tcpSocket, (struct sockaddr*)&addr_client, &addr_len);
				if(client_fd == -1)
				{
					std::cout<< errno << std::endl;
					return "";
				}
				for (i = 1; i < MAX_CLIENTS; i++) 
				{
					if (fds[i].fd == -1) 
					{
						fds[i].fd = client_fd;
						fds[i].events = POLLIN;
						break;
					}
				}
				if(i > MAX_CLIENTS)//when all 'slots' of poll struct are occupied by fds still in use
				{
					std::cerr<<"Too many clients."<<std::endl;
					close(client_fd);
				}
			}
			//NEW CLIENT HANDLING
			//save the client request in the array fds		
			for(i = 1; i < MAX_CLIENTS; i++)
			{
				if(fds[i].fd == -1)//if fd is empty, jumps over to next fd
					continue;
							
				//PRINT CLIENTS REQUEST
				if (fds[i].revents & POLLIN) 
				{
					int n_bytes = read(fds[i].fd, request, sizeof(request));
					if(n_bytes < 0){
						close(fds[i].fd);
						fds[i].fd = -1;
						fds[i].revents = 0;
					}
					else{
						request[n_bytes] = '\0';
						write(1, (char*)request, strlen((char*)request));
						//sets events to POLLOUT to send answer (next else if)
						fds[i].events = POLLOUT;
						client_timestamp[i] = current_time;
					}
				}
				else if(fds[i].revents & POLLOUT)
				{
					//ANSWER TO CLIENT
					if(pollout(fds[i].fd) == -1)
					{
						close(fds[i].fd);
						fds[i].fd = -1;
					}
					else
					{	
						close(fds[i].fd);
						fds[i].fd = -1;
						fds[i].revents = 0;
						fds[i].events = POLLIN;
						client_timestamp[i] = 0;
					}
				}
			}
			}
	}

	return "server terminated";
}
