#include "server.hpp"
#include "exceptions.hpp"
#include <stdlib.h>
#include "helper.hpp"
#include  <netdb.h>

server::server()
{
	sock = 0;
	port = 0;
	FD_ZERO(&s_read);
	FD_ZERO(&s_write);
	len = sizeof(addr);
	(void) buffer;
	clients.resize(MAX_FDS);
	db = &dbManager::getInstance();
	command::init_cmds();
}

server::~server(){
	delete db;
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

void	server::close(int sock){
	client	&c = clients[sock];

	db->deleteClient(c.getnickName());
	c.reset();
	::close(sock);
	std::cout << "client went away !!" << std::endl;
}

void	server::init_fds(){
	FD_ZERO(&s_read);
	FD_ZERO(&s_write);
	FD_SET(sock, &s_read);
	for (int i = 0; i < MAX_FDS; i++){
		if (!clients[i].isfree()){
			FD_SET(i, &s_read);
			if (clients[i].writeState())
				FD_SET(i, &s_write);
		}
	}
}

void	server::listen(){
	while (1){
		init_fds();
		n = select(MAX_FDS, &s_read, &s_write, NULL, NULL);
		if (n < 0)
			throw myexception("something went wrong !!");
		for (int i = 0; i < MAX_FDS && n > 0; i++){
			if (FD_ISSET(i, &s_read)){
				if (sock == i)
					accept();
				else
					read(i);
				n--;
			}
			if (FD_ISSET(i, &s_write))
				write(i, clients[i]);
		}
	}
}

void	server::accept(){
	int s = ::accept(sock, (struct sockaddr*)&addr, &len);
	if (s < 0)
		throw myexception("something went wrong !!");
	std::cout << "new client" << std::endl;
	clients[s].register_(s);
	clients[s].getHost() = getClientHost(&addr.sin_addr, addr.sin_len);
	db->insertClient(clients[s].getnickName(), s);
	read(s);
}

void	replace_nl(char *buffer){
	int	i = 0;
	for (; buffer[i]; i++){
		if (buffer[i] == '\n' || buffer[i] == 13)
			break;
	}
	buffer[i] = 0;
}

void	server::read(int s){
	int	rd = recv(s, buffer, BUFFER_SIZE, 0);
	if (rd <= 0)
		close(s);
	else{
		replace_nl(buffer);
		command cmd(buffer);
		if (cmd.gettype() == CMD_PASS || cmd.gettype() == CMD_NICK || cmd.gettype() == CMD_USER)
			auth(clients[s], cmd);
		else if (clients[s].authenticated())
			cmd.switch_cmd(s, db, clients[s]);
	}
}

void	server::chekout_nick(client &c, std::string nick){
	int fd = db->searchClient(nick);

	if (fd == -1){
		db->updateNickClient(c.getnickName(), nick);
		c.setnickName(nick);
	}
	else{
		std::string msg = ":127.0.0.1 433 * " + nick + " :Nickname is already in use.\n";
		::send(c.getfdClient(),  msg.c_str(), msg.length(), 0);
	}
}

void	server::auth(client &c, command cmd){
	int	type = cmd.gettype();

	if (type == CMD_PASS){
		if (password.compare(cmd.getbody()) == 0)
			c.authenticate();
		else{
			send(c.getfdClient(), "wrong pass", 10, 0);
			close(c.getfdClient());
		}
	}
	else if (type == CMD_NICK && c.authenticated())
		chekout_nick(c, cmd.getbody());
	else if (type == CMD_USER && c.authenticated())
		c.setloginName(cmd.getbody());
}

void	server::write(int fd, client &c){
	int			&windex = c.getWindex();
	size_t		chank = 0;
	std::string	&list = c.getList();
	
	chank = std::min(strlen(list.c_str() + windex), (size_t)1);
	windex += send(fd, list.c_str() + windex, chank, 0);
	if (strlen(list.c_str() + windex) == 0){
		c.unsetWriteState();
		windex = 0;
		list = "";
	}
}

std::string	server::getClientHost(const void *addr, socklen_t len){
	struct hostent *h = gethostbyaddr(addr, len, AF_INET);
	return (std::string(h->h_name));
}

client	&server::getClientByFd(int fd){
	return (clients[fd]);
}