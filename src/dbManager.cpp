#include "dbManager.hpp"
#include "helper.hpp"

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
	channel &ch = dbManager::searchChannel(nameChannel)->second;
	if (ch.insertClientToChannel(nick, fd))
		return (true);
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

void	dbManager::getInfoBan(int fd,std::string nick, std::string nameChannel){
	std::string	info = ":127.0.0.1 " + helper::itos(474);
	info += " " + nick + " " + nameChannel + " :Cannot join channel (+b)\n";
	send(fd, info.c_str(), info.size(), 0);
}

void	dbManager::getInfoInvalid(int fd,std::string nick){
	std::string	info = ":127.0.0.1 " + helper::itos(476);
	info += " " + nick + " 0  :Invalid channel name\n";
	send(fd, info.c_str(), info.size(), 0);
}

void	dbManager::getInfoNewJoin(client &cl ,std::string namechannel)
{
	std::string info = cl.getClinetFullname() + " JOIN " + namechannel + "\n";
	sendMsgCls(info,namechannel);
}


void	dbManager::getInfoListClInChannel(client &cl, std::string nameChannel, std::vector<client> &cls)
{
	channel& ch = searchChannel(nameChannel)->second;
	std::string info = processInfoCls(ch, cl, cls);
	sendMsgCls(info,ch.getNameChannel());
	info.clear();
	info = ":" + cl.getHost() + " 366 " + cl.getnickName() + " " + ch.getNameChannel() + " :End of /NAMES list.\n";
	sendMsgCls(info, ch.getNameChannel());
}

void	dbManager::sendMsgCls(std::string info, std::string nameChannel)
{
	channel& ch = searchChannel(nameChannel)->second;
	ch.cls_iter = ch.clients.begin();
	while (ch.cls_iter != ch.clients.end())
	{
		send(ch.cls_iter->second, info.c_str(), info.size(), 0);
		ch.cls_iter++;
	}
}

std::string		dbManager::processInfoCls(channel &ch, client &cl, std::vector<client> &cls)
{
	std::string info = ":" + cl.getHost() + " 353 " + cl.getnickName() + " = " + ch.getNameChannel() + " :";
	ch.cls_iter = ch.clients.begin();
	while (ch.cls_iter != ch.clients.end())
	{	
		if (cls[ch.cls_iter->second].getmode(ch.getNameChannel()) == OP_CLIENT)
			info += "@" + ch.cls_iter->first + " ";
		else
			info += ch.cls_iter->first + " ";
		ch.cls_iter++;
	}
	info += "\n";
	return (info);
}