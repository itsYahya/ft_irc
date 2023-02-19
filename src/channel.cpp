#include "channel.hpp"

std::map<std::string,client> channel::clients;

channel::channel(std::string name) : nameChannel(name){}
channel::~channel(){}

std::string channel::getNameChannel() const
{
	return (nameChannel);
}

bool	channel::insertClientToChannel(client cl)
{
	std::map<std::string, client>::iterator iter;
	iter = clients.find(cl.getnickName());
	if (iter != clients.end())
	{
		clients.insert(std::pair<std::string, client>(cl.getnickName(), cl));
		return (true);
	}
	return (false);
}

bool	channel::searchClient(std::string nick)
{
	std::map<std::string, client>::iterator iter;
	iter = clients.find(nick);
	if (iter != clients.end())
		return (true);
	return (false);
}

bool		channel::deleteClient(std::string nick)
{
	std::map<std::string, client>::iterator iter;

	iter = clients.find(nick);
	if (iter != clients.end())
	{
		clients.erase(nick);
		return (true);
	}
	return (false);
}
