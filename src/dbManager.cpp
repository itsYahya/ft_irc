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
	iterator_clinet iter; 
	iter = clients.find(name);
	if (iter == clients.end())
	{
		clients.insert(std::pair<std::string, int>(name, fd));
		return (true);
	}
	return (false);
}

int	dbManager::searchClient(std::string nick)
{
	iterator_clinet iter = clients.find(nick);
	if (iter == clients.end())
		return (-1);
	return (iter->second);
}

bool	dbManager::deleteClient(std::string nick)
{
	iterator_clinet iter;

	iter = clients.find(nick);
	if (iter != clients.end())
	{
		clients.erase(iter);
		return (true);
	}
	return (false);
}

bool	dbManager::insertChannel(channel& ch)
{
	iterator_channel iter; 
	iter = channels.find(ch.getNameChannel());
	if (iter == channels.end())
	{
		channels.insert(std::pair<std::string, channel>(ch.getNameChannel(), ch));
		return (true);
	}
	return (false);
}

dbManager::iterator_channel	dbManager::searchChannel(std::string nameChannel)
{
	return (channels.find(nameChannel));
}

bool	dbManager::srchChannel(std::string nameChannel)
{
	iterator_channel iter = channels.find(nameChannel);
	if (iter != channels.end())
		return (true);
	return (false);
}
bool		dbManager::joinClientChannel(std::string nameChannel, std::string nick, int fd)
{
	channel ch = dbManager::searchChannel(nameChannel)->second;
	if (ch.insertClientToChannel(nick, fd))
	{
		std::cout << nameChannel << " => " << nick << " \n";
		return (true);
	}
	else
		return (false);
}

bool	dbManager::deleteChannel(std::string nameChannel)
{
	iterator_channel iter;

	iter = channels.find(nameChannel);
	if (iter != channels.end())
	{
		channels.erase(nameChannel);
		return (true);
	}
	return (false);
}

bool	dbManager::updateNickClient(std::string curr, std::string new_){
	iterator_clinet	iter = clients.find(curr);
	int				fd = -1;

	if (iter == clients.end())
		return (false);
	fd = iter->second;
	clients.erase(iter->first);
	clients.insert(std::pair<std::string, int>(new_, fd));
	return (true);
}

bool	dbManager::deleteClientChannel(std::string nameChannel, std::string nick)
{
	iterator_channel iter = channels.find(nameChannel);
	if(iter != channels.end())
	{
		if (iter->second.deleteClient(nick))
			return (true);
	}
	return (false);
}

dbManager::channels_type	&dbManager::getChannels(){
	return (channels);
}

dbManager::clients_type		&dbManager::getClients(){
	return (clients);
}

bool	dbManager::isEndChannelIter(iterator_channel iter){
	return (iter == channels.end());
}

bool	dbManager::isEndClientIter(iterator_clinet iter){
	return (iter == clients.end());
}