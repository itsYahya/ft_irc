#include "server.hpp"

server::server(){
	sock = 0;
	port = 0;
	FD_ZERO(&s_read);
	len = sizeof(addr);
	(void) buffer;
	clients.resize(MAX_FDS);
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

void	server::create(){
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
		std::cerr << "something went wrong !!" << std::endl;
		exit(1);
	}
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (bind(sock, (struct sockaddr*)&addr, len) < 0){
		std::cerr << "port in use !!" << std::endl;
		exit(1);
	}
	if (listen(sock, MAX_FDS) < 0){
		std::cerr << "something went wrong !!" << std::endl;
		exit(1);
	}
	std::cout << "server was created successfully!!" << std::endl;
}

void	server::close(){
	::close(sock);
}
