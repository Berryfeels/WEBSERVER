#include "./include/TCPSocket.class.hpp"

// constructor for new client (accept_connection)
TCPSocket::TCPSocket(int existing_fd) : fd_(existing_fd)
{
	//_bind_socket = false;
	memset(&address_, 0, sizeof(address_));

	// set nonblocking flag
	int flags = fcntl(fd_, F_GETFL, 0);
	fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
}

// constructor for bind socket
TCPSocket::TCPSocket()
{
	std::cout<<"Creating bind socket"<<std::endl;
	//_bind_socket = true;

	// open socket
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ < 0)
	{
		throw std::runtime_error("TcpSocket: failed");
	}

	// Set the SO_REUSEADDR option to make the server restart faster
	int optval = 1;
	setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	// set nonblocking flag
	fcntl(fd_, F_SETFL, O_NONBLOCK);
}

void TCPSocket::bindToAddress(const sockaddr_in &address)
{
	address_ = address;
	if (bind(fd_, (sockaddr *)&address_, sizeof(address_)) < 0)
	{
		close(fd_);
		throw std::runtime_error("TcpSocket: failed to bind");
	}
}

void TCPSocket::listenSocket()
{
	if (listen(fd_, 256) < 0)
	{
		close(fd_);
		throw std::runtime_error("TcpSocket: failed to listen");
	}

	std::ostringstream ss;
	ss << "Listening on address: "
	   << inet_ntoa(address_.sin_addr)
	   << " port: " << ntohs(address_.sin_port);
	//DEBUG(ss.str());
}

int TCPSocket::getSocketFd()
{
	return fd_;
}