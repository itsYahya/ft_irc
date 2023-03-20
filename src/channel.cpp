#include "channel.hpp"
#include "helper.hpp"

channel::channel(std::string name) : nameChannel(name), topic("")
{
	isPasswd = false;
	isModerate = false;
	isTopicProtected = false;
	noexternal = false;
	limit = -1;
	inviteonly = false;
	secrectchannel = false;
}
channel::channel(std::string name, std::string passwd) : nameChannel(name), passwd(passwd), topic("")
{
	limit = -1;
	inviteonly = false;
	secrectchannel = false;
	isPasswd = true;
	isModerate = false;
	isTopicProtected = false;
	noexternal = false;
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
		clients.erase(it);
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
	if (!host.empty() && getBannedClient(host) == -1)
		ban_clients.push_back(host);
}

int					channel::getBannedClient(std::string host)
{
	int	index = 0;
	for (size_t i = 0; i < ban_clients.size(); i++)
	{
		if (ban_clients[i] == host)
			return (index);
		index++;
	}
	return (-1);
}

std::string		channel::getInfo(std::string nick){
	std::string	info = ":" + server::getShost() + " " + helper::itos(322);
	info += " " + nick + " " + nameChannel + " " + helper::itos(clients.size()) + " :" + topic + "\r\n";
	return (info);
}

void	channel::notifi(const std::string &msg){
	int					fd;
	clients_iter_type	iter = clients.begin();

	for (; iter != clients.end(); iter++){
		fd = iter->second;
		::send(fd, msg.c_str(), msg.length(), 0);
	}
}

void channel::notifi(const std::string &msg, std::map<int, int> &fds)
{
	int					fd;
	clients_iter_type	iter = clients.begin();

	for (; iter != clients.end(); iter++){
		fd = iter->second;
		if (fds.find(fd) == fds.end()){
			::send(fd, msg.c_str(), msg.length(), 0);
			fds[fd] = fd;
		}
	}
}

size_t channel::empty(){
	return (clients.empty());
}

std::string channel::modesInfo(std::string nick){
	std::string info = ":" + server::getShost() + " 324 " + nick + " " + nameChannel + " +";
	if (secrectchannel) info += "s";
	if (noexternal) info += "n";
	if (isModerate) info += "m";
	if (isTopicProtected) info += "t";
	if (inviteonly) info += "i";
	if (limit > 0) info += "l";
	if (isPasswd) info += "k";
	info += "\r\n";
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

void channel::moderate(const std::string &msg){
	notifi(msg);
	isModerate = true;
}

bool channel::moderated(){
	return (isModerate);
}

void channel::protecTopic(const std::string &msg){
	notifi(msg);
	isTopicProtected = true;
}

bool channel::topicProtected(){
	return isTopicProtected;
}

bool	channel::wantsMore(){
	return (clients.size() < static_cast<size_t>(limit) || limit < 0);
}

void	channel::setLimit(size_t l, const std::string &msg){
	notifi(msg);
	limit = l;
}

void	channel::setKey(const std::string &key, const std::string &msg){
	notifi(msg);
	isPasswd = true;
	passwd = key;
}

void		channel::deleteBannedClient(std::string host)
{
	int index = getBannedClient(host);
	if (!ban_clients.empty() && index >= 0)
		ban_clients.erase(ban_clients.begin() + index);
}

bool		channel::noExteranl(){
	return (noexternal);
}

bool channel::inviteOnly(){
	return (inviteonly);
}

void	channel::setInviteOnly(){
	inviteonly = true;
}

bool	channel::secretChannel(){
	return (secrectchannel);
}

void	channel::setSectretChannel(){
	secrectchannel = true;
}

void		channel::setNoExternal(){
	noexternal = true;
}
