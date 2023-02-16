#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_FDS 1024

class server{
	private:
		int					sock;
		int					port;
		std::string			password;
		fd_set				s_read;
		struct sockaddr_in	addr;
		socklen_t			len;
		char				buffer[BUFFER_SIZE];

	public:
		server();
		~server();
		void		create();
		int			&getport();
		void		setpassword(std::string pass);
		std::string	&getpassword();

};

#endif