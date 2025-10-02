#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <memory>

class TCPSocket
{
	private:
		sockaddr_in address_;
		int fd_;

	public:
		TCPSocket();
		TCPSocket(int existing_fd);
		TCPSocket(const TCPSocket &source);
		~TCPSocket();

		void bindToAddress (const sockaddr_in &address);
		void listenSocket();
		std::string readIncomingRequest();
		bool writeData(const std::string &data);
	};