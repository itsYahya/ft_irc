#include "channel.hpp"

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