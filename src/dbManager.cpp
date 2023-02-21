#include "dbManager.hpp"

std::map<std::string, int> dbManager::clients;
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

bool	dbManager::insertClient(std::string name, int fd)
{
	std::map<std::string, int>::iterator iter; 
	iter = clients.find(name);
	if (iter == clients.end())
	{
		clients.insert(std::pair<std::string, int>(name, fd));
		return (true);
	}
	return (false);
}

int&	dbManager::searchClient(std::string nick)
{
	return (clients.find(nick)->second);
}

const std::string&	dbManager::searchClient(int	fd)
{
	std::map<std::string, int>::iterator iter;
	iter = clients.begin();
	while (iter != clients.end())
	{
		if (iter->second == fd)
			return (iter->first);
		iter++;
	}
	std::cout << "hhiiiii\n";
	return (iter->first);
}

bool	dbManager::deleteClient(std::string nick)
{
	std::map<std::string, int>::iterator iter;

	iter = clients.find(nick);
	if (iter != clients.end())
	{
		clients.erase(iter);
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
	if (ch.insertClientToChannel(nick, clients.find(nick)->second))
		return (true);
	else
		return (false);
}

bool	dbManager::deleteChannel(std::string nameChannel)
{
	std::map<std::string, channel>::iterator iter;

	iter = channels.find(nameChannel);
	if (iter != channels.end())
	{
		channels.erase(nameChannel);
		return (true);
	}
	return (false);
}