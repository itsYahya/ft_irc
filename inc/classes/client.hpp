#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class client{
	private:
		int			fdClient;
		std::string	nickName;
		std::string	realName;
		std::string	loginName;
		std::string loginPass;
	public:
		client();
		~client();

		void	setfdClient(int fdCL);
		void	setnickName(std::string nickNM);
		void	setrealName(std::string  realNM);
		void	setloginName(std::string  loginNM);
		void	setloginPass(std::string  loginPS);

		int			getfdClient() const;
		std::string	getnickName() const;
		std::string	getrealName() const;
		std::string	getloginName() const;
		std::string	getloginPass() const;
};

#endif