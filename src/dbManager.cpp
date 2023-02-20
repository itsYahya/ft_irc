#include "dbManager.hpp"

std::map<std::string, client> dbManager::clients;
std::map<std::string, channel> dbManager::channels;
dbManager* dbManager::instance = 0;


dbManager::dbManager(){}
dbManager::~dbManager() {}


dbManager&	dbManager::getInstance()
{
	if (instance == 0)
		instance = new dbManager();
	return (*instance);
}

bool	dbManager::insertClient(client cl)
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

client&	dbManager::searchClient(std::string nick)
{
	return (clients.find(nick)->second);
}

client&	dbManager::searchClient(int fd)
{
	std::map<std::string, client>::iterator it;
	it = clients.begin();
	while (it != clients.end())
	{
		if (it->second.getfdClient()==fd)
			return (it->second);
		it++;
	}
	return (it->second);
}

bool	dbManager::deleteClient(std::string nick)
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

bool	dbManager::insertChannel(channel ch)
{
	std::map<std::string, channel>::iterator iter; 
	iter = channels.find(ch.getNameChannel());
	if (iter != channels.end())
	{
		channels.insert(std::pair<std::string, channel>(ch.getNameChannel(), ch));
		return (true);
	}
	return (false);
}

channel&	dbManager::searchChannel(std::string nameChannel)
{
	return (channels.find(nameChannel)->second);
}

bool		dbManager::joinClientChannel(std::string nameChannel, std::string nick)
{
	channel ch = dbManager::searchChannel(nameChannel);
	if (ch.deleteClient(nick) == true)
		return (true);
	else
		return (false);
}