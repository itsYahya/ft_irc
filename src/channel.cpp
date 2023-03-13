#include "channel.hpp"
#include "helper.hpp"

channel::channel(std::string name) : nameChannel(name), topic("")
{
	isPasswd = false;
}
channel::channel(std::string name, std::string passwd) : nameChannel(name), passwd(passwd), topic("")
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
	if (clients.size() == 0 || !searchClient(name))
	{
		clients.insert(std::pair<std::string, int>(name, fd));
		return (true);
	}
	return (false);
}

bool	channel::searchClient(std::string nick)
{
	cls_iter = clients.find(nick);
	if (cls_iter != clients.end() && cls_iter->first == nick)
		return (true);
	return (false);
}

bool		channel::deleteClient(std::string nick)
{
	std::map<std::string, int> &cl = getClients();
	std::map<std::string, int>::iterator it = cl.find(nick);
	if (it != clients.end())
	{
		
		std::cout << "before check => " << it->first<< "\n";
		clients.erase(it);
		it = clients.begin();
		while (it != clients.end())
		{
			std::cout << "check => " << it->first<< "\n";
			it++;
		}
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

void				channel::setBannedClient(std::string host)
{
	if (!host.empty())
		ban_clients.push_back(host);
}

bool				channel::getBannedClient(std::string host)
{
	for (size_t i = 0; i < ban_clients.size(); i++)
	{
		if (ban_clients[i].compare(host) == 0)
			return (true);
	}
	return (false);
}

std::string		channel::getInfo(std::string nick){
	std::string	info = ":" + server::getShost() + " " + helper::itos(322);
	info += " " + nick + " " + nameChannel + " " + helper::itos(clients.size()) + " :" + topic + "\r\n";
	return (info);
}



std::string		channel::getInfosHeader(std::string nick){
	std::string header = ":" + server::getShost() + " " + helper::itos(321);
	header += " " + nick + " Channel :Users Name\n";
	return (header);
}

std::string		channel::getInfosFooter(std::string nick){
	std::string footer = ":" + server::getShost() + " " + helper::itos(323);
	footer += " " + nick + " End of /LIST\n";
	return (footer);
}

std::map<std::string, int>	&channel::getClients(){
	return (clients);
}

std::string channel::geTopic() const{
	return (topic);
}

void		channel::seTopic(const std::string &topic){
	this->topic = topic;
}
