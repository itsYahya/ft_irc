#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

class server{
	private:

	public:
		int				port;
		std::string		password;
		
		server();
		~server();
};

#endif