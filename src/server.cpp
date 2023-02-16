#include "server.hpp"

server::server(){
	sock = 0;
	port = 0;
	FD_ZERO(&s_read);
	len = sizeof(addr);
	(void) buffer;
}

server::~server(){
	
}

int		&server::getport(){
	return (port);
}

void	server::setpassword(std::string pass){
	password = pass;
}

std::string	&server::getpassword(){
	return (password);
}