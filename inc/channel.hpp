#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "client.hpp"
# include <map>
# include <vector>

class channel
{
	private:
		static std::map<std::string, int>			clients;
		std::map<std::string, int>::iterator		iter;
		std::string									nameChannel;
		bool										isPasswd;
		std::string									passwd;
		int											fd_op;

	public:
		channel(std::string name, int fd_op);
		channel(std::string name, int fd_op, std::string passwd);
		~channel();
		std::string			getNameChannel() const;
		bool				insertClientToChannel(std::string name,int	fd);
		bool				searchClient(std::string nick);
		bool				deleteClient(std::string nick);

		std::string			getPasswd() const;
		bool				getIsPasswd() const;
		int					getfd_op()	const;
		int					getNumberOfClients() const;

		std::string			getInfo(std::string nick);
		static std::string	getInfosHeader(std::string nick);
		static std::string	getInfosFooter(std::string nick);

		std::map<std::string, int>	&getClients();
};

#endif