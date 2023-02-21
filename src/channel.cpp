#include "channel.hpp"

std::map<std::string, int> channel::clients;

channel::channel(std::string name) : nameChannel(name){}
channel::~channel(){}

std::string channel::getNameChannel() const
{
	return (nameChannel);
}

bool	channel::insertClientToChannel(std::string name,int fd)
{
	std::map<std::string, int>::iterator iter;
	iter = clients.find(name);
	if (iter != clients.end())
	{
		clients.insert(std::pair<std::string, int>(name, fd));
		return (true);
	}
	return (false);
}

bool	channel::searchClient(std::string nick)
{
	std::map<std::string, int>::iterator iter;
	iter = clients.find(nick);
	if (iter != clients.end())
		return (true);
	return (false);
}

bool		channel::deleteClient(std::string nick)
{
	std::map<std::string, int>::iterator iter;

	iter = clients.find(nick);
	if (iter != clients.end())
	{
		clients.erase(nick);
		return (true);
	}
	return (false);
}
