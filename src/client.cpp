#include "client.hpp"
#include "helper.hpp"


client::client(){
	reset();
}

client::~client(){
}

void	client::reset(){
	quitChannels();
	this->type = FDFREE;
	this->fdClient = -1;
	this->auth = false;
	this->flauth = false;
	this->s_w = false;
	this->realName = "";
	this->nickName = "";
	this->list = "";
	this->windex = 0;
	this->pong = true;
	this->cmd = "";
	this->list_mode.clear();
}

bool	client::isfree(){
	return (this->type == FDFREE);
}

void	client::setfdClient(int fdCL){
	this->fdClient = fdCL;
}
void	client::setnickName(std::string nickNM){
	this->nickName = nickNM;
}
void	client::setrealName(std::string  realNM){
	this->realName = realNM;
}
void	client::setloginName(std::string  loginNM){
	this->loginName = loginNM;
}
void	client::setloginPass(std::string  loginPS){
	this->loginPass = loginPS;
}

int				client::getfdClient() const{
	return (this->fdClient);
}
std::string		client::getnickName() const{
	return (this->nickName);
}
std::string		client::getrealName() const{
	return (this->realName);
}
std::string		client::getloginName() const{
	return (this->loginName);
}
std::string		client::getloginPass() const{
	return (this->loginPass);
}

void	client::authenticate(){
	this->auth = true;
}

bool	client::authenticated() const{
	std::string nick = "nick" + helper::itos(fdClient);
	return (auth && nickName.compare(nick) && !realName.empty());
}

void	client::register_(int fd){
	type = FDBUSY;
	fdClient = fd;
	nickName = std::string("nick") + helper::itos(fd);
}

bool	client::writeState() const{
	return (s_w);
}

void	client::setWriteState(){
	s_w = true;
}

void	client::unsetWriteState(){
	s_w = false;
}

std::string	&client::getList(){
	return (list);
}

int			&client::getWindex(){
	return (windex);
}

std::string	&client::getHost() {
	return (host);
}

std::string	client::getClinetFullname(){
	std::string name;

	name = ":" + nickName + "!~" + loginName + "@" + host + " ";
	return (name);
}

void	client::pinged(time_t time){
	ping = time;
}

time_t	client::getPing(){
	return (std::difftime(std::time(NULL), ping));
}

bool	&client::getPong(){
	return (pong);
}

void	client::setmode(std::string channel, t_mode mode){
	if (!channel.empty() && list_mode.find(channel)->first != channel)
		list_mode.insert(std::pair<std::string, t_mode>(channel, mode));
	if (!channel.empty() && list_mode.find(channel)->first == channel)
		list_mode.find(channel)->second = mode;
}

t_mode	client::getmode(std::string channel)
{
	iter_mode = list_mode.find(channel);
	if (iter_mode != list_mode.end())
		return (iter_mode->second);
	return (NONE);
}

std::string	&client::getCmd(){
	return (cmd);
}

bool	client::checkChannel(const std::string &ch)
{
	std::map<std::string, t_mode> &cl = getClientMode();
	 std::map<std::string, t_mode>::iterator it = cl.find(ch);
	if (it->first == ch){
		std::cout << "true client::checkChannel\n";
		return (true);
	}
	std::cout << "false client::checkChannel\n";
	return (false);
}

bool	client::quitChannel(const std::string &channel){
	return (dbManager::deleteClientChannel(channel, nickName));
}

void	client::quitChannels(){
	iter_mode = list_mode.begin();
	for (; iter_mode != list_mode.end(); iter_mode++)
		quitChannel(iter_mode->first);
}

time_t	client::getSessionTime(){
	return (std::difftime(std::time(NULL), session));
}

void	client::setSessionTime(){
	session = std::time(NULL);
}

void	client::erasemode(std::string channel)
{
	std::map<std::string, t_mode> &cl = getClientMode();
	std::map<std::string, t_mode>::iterator it = cl.find(channel);
	if (it != cl.end())
	{
		cl.erase(it);
		// return (true);
		std::cout << "true erasemode\n";
	}
	// return (false);	
	std::cout << "false erasemode\n";
}

bool	&client::getFlauth(){
	return (flauth);
}

void	client::setInvite(std::string channel)
{
	if(getInvite(channel) == -1)
		list_invite.push_back(channel);
}

int	client::getInvite(std::string channel)
{
	int index = 0;
	for (size_t x = 0; x < list_invite.size(); x++)
	{
		if (list_invite[x] == channel)
			return (index);
		index++;
	}
	return (-1);
}

void	client::eraseInvite(std::string channel)
{
	size_t index = getInvite(channel);
	// std::cout << "index => " << index << " name => " << list_invite[index] << "\n";
	if (!list_invite.empty() && index >= 0)
		list_invite.erase(list_invite.begin() + index);
}

std::map<std::string, t_mode>	&client::getClientMode()
{
	return (list_mode);
}