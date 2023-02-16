#include "server.hpp"
#include "exceptions.hpp"

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
	if (sock < 0)
		throw myexception("something went wrong !!");
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (bind(sock, (struct sockaddr*)&addr, len) < 0)
		throw myexception("port already in use !!");
	if (::listen(sock, MAX_FDS) < 0)
		throw myexception("something went wrong !!");
}

void	server::close(){
	::close(sock);
}

void	server::init_fds(){
	FD_ZERO(&s_read);
	FD_SET(sock, &s_read);
	for (int i = 0; i < MAX_FDS; i++){
		if (!clients[i].isfree())
			FD_SET(i, &s_read);
	}
}

void	server::listen(){
	while (1){
		int		s;

		init_fds();
		n = select(MAX_FDS, &s_read, NULL, NULL, NULL);
		if (n < 0)
			throw myexception("something went wrong !!");
		for (int i = 0; i < MAX_FDS && n > 0; i++){
			s = i;
			if (FD_ISSET(i, &s_read)){
				if (sock == i)
					accept(s);
				read(s);
				n--;
			}
		}
	}
}

void	server::accept(int &s){
	s = ::accept(sock, (struct sockaddr*)&addr, &len);
	if (s < 0)
		throw myexception("something went wrong !!");
	std::cout << "new client" << std::endl;
	clients[s].type = FDBUSY;
}

void	server::read(int s){
	int	rd = recv(s, buffer, BUFFER_SIZE, 0);
	if (rd <= 0){
		clients[s].reset();
		::close(s);
		std::cout << "a client went a way" << std::endl;
	}
	else{
		buffer[rd] = 0;
		std::cout << "**" << buffer << "**" << std::endl;
	}
}