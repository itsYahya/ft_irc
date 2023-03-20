#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <map>
typedef enum e_type{
	FDFREE = 0,
	FDBUSY = 1
}	t_type;

typedef enum e_mode{
	V_CLIENT  = 4,
	M_CLIENT  = 3,
	SM_CLIENT = 2,
	OP_CLIENT = 1,
	NONE = 0
}	t_mode;

class client{
	private:
		std::map<std::string,t_mode>			list_mode;
		std::map<std::string,t_mode>::iterator	iter_mode;
		std::vector<std::string>				list_invite;

		int												fdClient;
		bool											auth;
		bool											flauth;
		std::string										nickName;
		std::string										realName;
		std::string										loginName;
		std::string 									loginPass;

		bool											s_w;
		std::string										list;
		int												windex;

		std::string										host;
		time_t											ping;
		bool											pong;

		std::string										cmd;

		time_t											session;

	public:
		typedef std::map<std::string,t_mode>			mode_type;
		typedef	mode_type::iterator						mode_iter_type;
		t_type		type;
		client();
		~client();
		
		void											changeMode(const std::string &channel, t_mode mode);
		void 											reset();
		bool											isfree();
		
		void											setfdClient(int fdCL);
		void											setnickName(std::string nickNM);
		void											setrealName(std::string  realNM);
		void											setloginName(std::string  loginNM);
		void											setloginPass(std::string  loginPS);

		int												getfdClient() const;
		std::string										getnickName() const;
		std::string										getrealName() const;
		std::string										getloginName() const;
		std::string										getloginPass() const;

		void											authenticate();
		bool											authenticated() const;

		void											register_(int fd);

		bool											writeState() const;
		void											setWriteState();
		void											unsetWriteState();
		std::string										&getList();
		int												&getWindex();
		void											setmode(std::string channel, t_mode mode);
		t_mode											getmode(std::string channel);
		void											erasemode(std::string channel);
		std::string										&getHost();

		void											setInvite(std::string channel);
		int												getInvite(std::string channel);
		void											eraseInvite(std::string channel);
		std::map<std::string, t_mode> 					&getClientMode();
		std::string										getClinetFullname();

		time_t											getPing();
		void											pinged(time_t);
		bool											&getPong();

		std::string										&getCmd();

		bool											checkChannel(const std::string &ch);
		bool											quitChannel(const std::string &ch);
		void											quitChannels();

		time_t											getSessionTime();
		void											setSessionTime();

		bool											&getFlauth();

		mode_type										&getmodelist();
		
		void											informChannels(const std::string &msg, bool s);
		void											updateNick(const std::string &nick);
};

#endif