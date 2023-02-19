#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "client.hpp"
# include <map>
# include <vector>

class channel
{
	private:
		std::string								nameChannel;
		static std::map<std::string, client>	clients;

	public:
		channel(std::string name);
		~channel();
		std::string			getNameChannel() const;
		static	bool		insertClientToChannel(client cl);
		static	bool		searchClient(std::string nick);
		static	bool		deleteClient(std::string nick);
};

#endif