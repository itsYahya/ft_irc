#include "server.hpp"
#include "exceptions.hpp"

server::server()
{
	sock = 0;
	port = 0;
	FD_ZERO(&s_read);
	len = sizeof(addr);
	(void) buffer;
	clients.resize(MAX_FDS);
	db = &dbManager::getInstance();
	command::init_cmds();
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
		init_fds();
		n = select(MAX_FDS, &s_read, NULL, NULL, NULL);
		if (n < 0)
			throw myexception("something went wrong !!");
		for (int i = 0; i < MAX_FDS && n > 0; i++){
			if (FD_ISSET(i, &s_read)){
				if (sock == i)
					accept();
				else{
					std::cout << clients[i].getloginPass() << std::endl;
					read(i, 0);
				}
				n--;
			}
		}
	}
}

void	server::accept(){
	int s = ::accept(sock, (struct sockaddr*)&addr, &len);
	if (s < 0)
		throw myexception("something went wrong !!");
	std::cout << "new client" << std::endl;
	clients[s].type = FDBUSY;
	clients[s].setfdClient(s);
	read(s, 1);
}

void	replace_nl(char *buffer){
	int	i = 0;
	for (; buffer[i]; i++){
		if (buffer[i] == '\n' || buffer[i] == 13)
			break;
	}
	buffer[i] = 0;
}

void	server::read(int s, int first){
	int	rd = recv(s, buffer, BUFFER_SIZE, 0);
	if (rd <= 0){
		clients[s].reset();
		::close(s);
		std::cout << "a client went a way" << std::endl;
	}
	else{
		replace_nl(buffer);
		command cmd(buffer);
		cmd.switch_cmd(cmd, s, *db);
		if (first)
			clients[s].setfdClient(s);
		if (cmd.gettype() == CMD_PASS || cmd.gettype() == CMD_NICK || cmd.gettype() == CMD_USER)
			auth(clients[s], cmd, first);
	}
}

void	server::auth(client &c, command cmd, int first){
	int	type = cmd.gettype();

	if (type == CMD_PASS){
		if (password.compare(cmd.getbody()) == 0)
			c.setloginPass(cmd.getbody());
		else{
			c.reset();
			::close(c.getfdClient());
		}
	}
	else if (type == CMD_NICK && !first)
		c.setnickName(cmd.getbody());
	else if (type == CMD_USER && !first)
		c.setloginName(cmd.getbody());
}