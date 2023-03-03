#include "client.hpp"
#include "helper.hpp"


client::client(){
	reset();
}

client::~client(){
}

void	client::reset(){
	this->type = FDFREE;
	this->fdClient = -1;
	this->auth = false;
	this->s_w = false;
	this->realName = "";
	this->nickName = "";
	this->list = "";
	this->windex = 0;
	pong = true;
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
	return (auth && !nickName.empty() && !realName.empty());
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
	if (!channel.empty())
		list_mode.insert(std::pair<std::string, t_mode>(channel, mode));
}

t_mode	client::getmode(std::string channel)
{
	iter_mode = list_mode.find(channel);
	if (iter_mode != list_mode.end())
		return (iter_mode->second);
	return (NONE);
}