#include "channel.hpp"
#include "helper.hpp"

std::map<std::string, int> channel::clients;
std::map<std::string, int>::iterator	channel::iter;
channel::channel(std::string name, int fd_op) : nameChannel(name), fd_op(fd_op)
{
	isPasswd = false;
}
channel::channel(std::string name, int fd_op, std::string passwd) : nameChannel(name), passwd(passwd), fd_op(fd_op)
{
	isPasswd = true;
}
channel::~channel(){}

std::string channel::getNameChannel() const
{
	return (nameChannel);
}

bool	channel::insertClientToChannel(std::string name,int fd)
{
	iter = clients.find(name);
	if (iter == clients.end())
	{
		clients.insert(std::pair<std::string, int>(name, fd));
		return (true);
	}
	return (false);
}

bool	channel::searchClient(std::string nick)
{
	iter = clients.find(nick);
	if (iter != clients.end())
		return (true);
	return (false);
}

bool		channel::deleteClient(std::string nick)
{
	iter = clients.find(nick);
	if (iter != clients.end())
	{
		clients.erase(nick);
		return (true);
	}
	return (false);
}

std::string			channel::getPasswd() const
{
	return (passwd);
}

bool				channel::getIsPasswd() const
{
	return (isPasswd);
}

int					channel::getfd_op()	const
{
	return (fd_op);
}

std::string		channel::getInfo(std::string nick){
	std::string	info = ":127.0.0.1 " + helper::itos(322);
	info += " " + nick + " " + nameChannel + " :channel status\n";
	return (info);
}

std::string		channel::getInfosHeader(std::string nick){
	std::string header = ":127.0.0.1 " + helper::itos(321);
	header += " " + nick + " Channel :Users Name\n";
	return (header);
}

std::string		channel::getInfosFooter(std::string nick){
	std::string footer = ":127.0.0.1 " + helper::itos(323);
	footer += " " + nick + " End of /LIST\n";
	return (footer);
}