#include "server.hpp"
#include "exceptions.hpp"
#include <stdlib.h>
#include "helper.hpp"
#include  <netdb.h>

std::string 		server::shost;
std::vector<client>	server::clients;

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
	memset(&time, 0, sizeof time);
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
	getShost() = "localhost";
	if (bind(sock, (struct sockaddr*)&addr, len) < 0)
		throw myexception("port already in use !!");
	if (::listen(sock, MAX_FDS) < 0)
		throw myexception("something went wrong !!");
}

void	server::close(int sock, client &c, const std::string &msg){
	std::string info = c.getnickName();
	if (!msg.empty())
		c.informChannels(msg, false);
	dbManager::deleteClient(info);
	c.reset();
	::close(sock);
	std::cout << info <<" went away !!" << std::endl;
}

bool	server::checkPing(client &c, int fd){
	if (c.isfree()) return (false);
	if (c.getPing() >= PINGTIME && c.getPong()){
		std::string msg = "PING :" + getShost() + "\n";
		::send(fd, msg.c_str(), msg.length(), 0);
		c.getPong() = false;
		c.pinged(std::time(NULL));
	}
	else if (c.getPing() >= PINGTIME){
		closingLink("Ping timeout", c);
		return (false);
	}
	return (true);
}

void	server::init_fds(){
	time.tv_sec = 40;
	FD_ZERO(&s_read);
	FD_ZERO(&s_write);
	if (sock > 0) FD_SET(sock, &s_read);
	for (int i = 0; i < MAX_FDS; i++){
		if (checkPing(clients[i], i)){
			FD_SET(i, &s_read);
			if (clients[i].writeState()) FD_SET(i, &s_write);
			time.tv_sec = std::min(time.tv_sec, PINGTIME - clients[i].getPing());
		}
	}
}

void	server::listen(){
	while (1){
		init_fds();
		n = select(MAX_FDS, &s_read, &s_write, NULL, &time);
		if (n < 0)
			throw myexception("something went wrong !!");
		for (int i = 0; i < MAX_FDS && n > 0; i++){
			if (FD_ISSET(i, &s_read)){
				if (sock == i) accept();
				else read(i);
				n--;
			}
			if (FD_ISSET(i, &s_write)) write(i, clients[i]);
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
	clients[s].pinged(std::time(NULL));
	clients[s].setSessionTime();
}

static bool	checkhNl(const char *buffer){
	size_t	len;
	
	len = strlen(buffer);
	if (len == 0) return (false);
	char	ch = buffer[len - 1];
	return (ch == '\n');
}

void	server::read(int s){
	bool		nl;
	int			rd;
	client		&c = clients[s];
	std::string	&textCmd = c.getCmd();
	
	rd = recv(s, buffer, BUFFER_SIZE, 0);
	buffer[rd] = 0;
	if (rd <= 0)
		close(s, c, c.getClinetFullname() + "QUIT :Client closed connection\r\n");
	else{
		nl = checkhNl(buffer);
		textCmd += buffer;
		if (nl){
			execut(clients[s], textCmd, s);
			textCmd = "";
		}
	}
}

void	server::checkout_nick(client &c, std::string nick){
	int fd = -1;
	helper::vector	res = helper::split(nick, ' ');
	
	if (res.size() == 0){
		std::string msg = ":" + getShost() + " 431 :No nickname given\n";
		::send(c.getfdClient(), msg.c_str(), msg.length(), 0);
		return ;
	}
	nick = res[0];
	fd = db->searchClient(nick);
	if (fd == -1){
		if (c.authenticated()){
			c.informChannels(c.getClinetFullname() + "NICK :" + nick + "\r\n", true);
			c.updateNick(nick);
		}
		db->updateNickClient(c.getnickName(), nick);
		c.setnickName(nick);
	}
	else{
		std::string msg = ":" + getShost() + " 433 * " + nick + " :Nickname is already in use.\n";
		::send(c.getfdClient(),  msg.c_str(), msg.length(), 0);
	}
}

void	server::checkout_user(client &c, std::string body){
	std::vector<std::string> arr = helper::split(body, ' ');
	if (arr.size() != 4){
		std::string nick = c.getnickName();
		int			fd = c.getfdClient();
		std::string msg = ":" + getShost() + " 461 " + nick + " USER :Not enough parameters\n";
		::send(fd, msg.c_str(), msg.length(), 0);
	} else {
		c.setloginName(arr[0]);
		c.setrealName(arr[3]);
	}
}

void	server::auth(client &c, command cmd){
	int			type = cmd.gettype();
	bool		&auth = c.getFlauth();

	if (type == CMD_PASS){
		if (password.compare(cmd.getbody()) == 0)
			c.authenticate();
		else
			closingLink("Wrong password", c);
	}
	else if (type == CMD_NICK)
		checkout_nick(c, cmd.getbody());
	else if (type == CMD_USER)
		checkout_user(c, cmd.getbody());
	if (!auth && c.authenticated()){
		auth = welcome(c);
	}
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

void	server::clear(){
	server::iterator	iter = clients.begin();
	client				cl;

	::close(sock);
	sock = -1;
	for (; iter != clients.end(); iter++){
		cl = *iter;
		if (!(cl).isfree()) close(cl.getfdClient(), cl, "");
	}
}

void	server::closingLink(std::string reson, client &c){
	int fd = c.getfdClient();
	std::string msg = "ERROR :Closing Link: " + c.getHost() + " (QUIT :" + reson + ")\r\n";
	::send(fd, msg.c_str(), msg.length(), 0);
	close(fd, c, c.getClinetFullname() + "QUIT :" + reson + "\r\n");
}

std::string	&server::getShost(){
	return (shost);
}

void	server::execut(client &c, std::string &textCmd, int fd){
	std::vector<std::string>			res;
	std::vector<std::string>::iterator	iter;

	res = helper::splitCmds(textCmd, '\r');
	iter = res.begin();
	for (; iter != res.end(); iter++){
		command cmd((*iter).c_str());
		if (cmd.gettype() == CMD_PASS || cmd.gettype() == CMD_NICK || cmd.gettype() == CMD_USER)
			auth(c, cmd);
		else if (c.authenticated())
			cmd.switch_cmd(fd, db, c, clients);
		else{
			std::string msg = ":" + getShost() + " 451 * " + cmd.getname() + " :You must finish connecting first.\r\n";
			::send(fd, msg.c_str(), msg.length(), 0);
		}
	}
}

bool	server::welcome(client &c){
	int			fd = c.getfdClient();
	int			&index = c.getWindex();
	std::string &list = c.getList();

	list = ":" + getShost() + " 001 " + c.getnickName() + " :Welcome to our IRC server that was create by itsYahya && ababouel\r\n:";
	list += getShost() + " 002 " + c.getnickName() + " :Your host is " + getShost() + "\r\n:";
	list += getShost() + " 003 " + c.getnickName() + " :This server was created 01/03/2023\r\n:";
	list += getShost() + " 004 " + c.getnickName() + " localhost 1.0 - -\r\n:";
	list += getShost() + " 372 " + c.getnickName() + " welcome to localhost\r\n:";
	list += getShost() + " 376 " + c.getnickName() + " :Feel free to use our bot, provided only for you. (BOT)\r\n";
	c.setWriteState();
	index = 0;
	server::write(fd, c);
	return (true);
}
