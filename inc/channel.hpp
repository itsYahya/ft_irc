#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "client.hpp"
# include <map>
# include <vector>

class channel
{
	private:
		static std::map<std::string, int>			clients;
		static std::map<std::string, int>::iterator	iter;
		std::string									nameChannel;
		bool										isPasswd;
		std::string									passwd;
		int											fd_op;

	public:
		channel(std::string name, int fd_op);
		channel(std::string name, int fd_op, std::string passwd);
		~channel();
		std::string			getNameChannel() const;
		static	bool		insertClientToChannel(std::string name,int	fd);
		static	bool		searchClient(std::string nick);
		static	bool		deleteClient(std::string nick);

		std::string			getPasswd() const;
		bool				getIsPasswd() const;
		int					getfd_op()	const;
};

#endif