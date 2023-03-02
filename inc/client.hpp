#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

typedef enum e_type{
	FDFREE = 0,
	FDBUSY = 1
}	t_type;

typedef enum e_mode{
	SM_CLIENT = 0,
	OP_CLIENT = 1,
}	t_mode;

class client{
	private:
		int			fdClient;
		bool		auth;
		std::string	nickName;
		std::string	realName;
		std::string	loginName;
		std::string loginPass;
		bool		s_w;
		std::string	list;
		int			windex;
		std::string	host;
		
		time_t		ping;
		bool		pong;
		
	public:
		t_mode		mode;
		t_type		type;
		client();
		~client();
		
		void 		reset();
		bool		isfree();

		void		setfdClient(int fdCL);
		void		setnickName(std::string nickNM);
		void		setrealName(std::string  realNM);
		void		setloginName(std::string  loginNM);
		void		setloginPass(std::string  loginPS);

		int			getfdClient() const;
		std::string	getnickName() const;
		std::string	getrealName() const;
		std::string	getloginName() const;
		std::string	getloginPass() const;

		void		authenticate();
		bool		authenticated() const;

		void		register_(int fd);

		bool		writeState() const;
		void		setWriteState();
		void		unsetWriteState();
		std::string	&getList();
		int			&getWindex();

		std::string	&getHost();

		std::string	getClinetFullname();

		time_t		getPing();
		void		pinged(time_t);

		bool		&getPong();
};

#endif