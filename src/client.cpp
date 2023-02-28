#include "client.hpp"
#include "helper.hpp"


client::client(){
	this->type = FDFREE;
	this->fdClient = -1;
	this->auth = false;
	this->s_w = false;
}

client::~client(){
}

void	client::reset(){
	this->type = FDFREE;
	this->fdClient = -1;
	this->auth = false;
	this->s_w = false;
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
	return (auth);
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