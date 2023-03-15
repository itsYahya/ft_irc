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
	std::string	info =  ":" + server::getShost() + " " + helper::itos(474);
	info += " " + nick + " " + nameChannel + " :Cannot join channel (+b)\n";
	send(fd, info.c_str(), info.size(), 0);
}

void	dbManager::getInfoInvalid(int fd,std::string nick){
	std::string	info =  ":" + server::getShost() + " " + helper::itos(476);
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
	send(cl.getfdClient(), info.c_str(), info.size(), 0);
	info.clear();
	info = ":" + cl.getHost() + " 366 " + cl.getnickName() + " " + ch.getNameChannel() + " :End of /NAMES list.\n";
	send(cl.getfdClient(), info.c_str(), info.size(), 0);
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

void	dbManager::getInfoPartChannel(client &cl, std::vector<std::string> info)
{
	std::string msg = "";
	if (info.size() > 1)
	{
		msg += cl.getClinetFullname() + " PART " + info[0] + " ";
		for (size_t i = 1; i < info.size(); i++)
			msg += " " + info[i];
		msg += "\n";
	}
	else
		msg += cl.getClinetFullname() + " PART " + info[0] + "\n";
	sendMsgCls(msg, info[0]);
}

void	dbManager::getInfoKickChannel(client &cl, std::vector<std::string> info)
{
	std::string msg = cl.getClinetFullname() + " KICK " + info[0] + " ";
	for (size_t i = 1; i < info.size(); i++)
		msg += " " + info[i];
	msg += "\n";
	sendMsgCls(msg, info[0]);
}

void	dbManager::getInfoPartError(client &cl, std::string namechannel, int num)
{
	std::string info = "";
	if (num == 500)
		info +=  ":localhost  " + cl.getnickName() + " " + namechannel + " : the client has already joined the channel\n";
	else if (num == 475)
		info +=  ":localhost 475 " + cl.getnickName() + " " + namechannel + " :Cannot join channel (+k) - bad key\n";
	else if (num == 403)
		info +=  ":localhost 403 " + cl.getnickName() + " " + namechannel + " :No such channel\n";
	else if (num == 442)
		info +=  ":localhost 442 " + cl.getnickName() + " " + namechannel + " :You're not on that channel\n";
	send(cl.getfdClient(), info.c_str(), info.size(), 0);
}

bool	dbManager::getInfoKickError(client &cl, dbManager &db,std::vector<std::string> body)
{
	std::string info = "";
	bool result = true;
	if (body[0].c_str()[0] == '#' && body[0].length() <= 1 && !srchChannel(body[0]))
		info +=  ":localhost 403 " + cl.getnickName() + " " + body[0] + " :No such channel\n";
	else if (body.size() < 2)
		info +=  ":localhost 401 " + cl.getnickName() + " " + body[1] + " :no such nick/channel\n";	
	else if (isEndChannelIter(searchChannel(body[0])) && db.searchChannel(body[0])->second.searchClient(body[1]))
		info +=  ":localhost 441 " + cl.getnickName() + " " + body[0] + " " + body[1] + " :They aren't on that channel\n";
	else if (cl.getmode(body[0]) != OP_CLIENT)
		info += ":localhost 482 " + cl.getnickName() + " " + body[0] + " :You're not channel operator\n";
	if (!info.empty())
	{
		send(cl.getfdClient(), info.c_str(), info.size(), 0); 
		result = false;
	}
	return (result);
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

bool		dbManager::nextClientmode(client &cl,channel &ch,std::vector<client> & cls)
{
	std::vector<std::string> str = helper::split(ch.getNameChannel(), ' ');
	ch.cls_iter = ch.clients.begin();
	while (ch.cls_iter != ch.clients.end())
	{
		if (ch.cls_iter->second != cl.getfdClient())
		{
			cls[ch.cls_iter->second].setmode(ch.getNameChannel(), OP_CLIENT);
			return (true);
		}
		ch.cls_iter++;
	}
	return (false);
}