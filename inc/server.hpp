#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include "dbManager.hpp"

#define BUFFER_SIZE 1024
#define MAX_FDS 1024

class server{
	private:
		int							sock;
		int							port;
		std::string					password;
		fd_set						s_read;
		struct sockaddr_in			addr;
		socklen_t					len;
		char						buffer[BUFFER_SIZE];
		std::vector<client> 		clients;
		dbManager					*db;
		int							n;

	public:
		server();
		~server();
		void						create();
		void						listen();
		void						init_fds();
		void						accept(int &s);
		void						read(int s);
		int							&getport();
		void						setpassword(std::string pass);
		std::string					&getpassword();
		void						close();

};

#endif