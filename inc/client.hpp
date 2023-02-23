#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

typedef enum e_type{
	FDFREE = 0,
	FDBUSY = 1
}	t_type;

class client{
	private:
		int			fdClient;
		bool		auth;
		std::string	nickName;
		std::string	realName;
		std::string	loginName;
		std::string loginPass;
		
	public:
		t_type		type;
		client();
		~client();
		
		void 	reset();
		bool	isfree();

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

		void		authenticate();
		bool		authenticated() const;
};

#endif