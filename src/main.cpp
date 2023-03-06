#include "helper.hpp"

server	*ptr;

void	sighandler(int sig){
	(void)sig;
	ptr->clear();
	exit(1);
}

int main(int ac, char **av){
	server server;
	
	try{
		if (ac == 2 || ac == 3){
			if (helper::parse_arguments(ac, av, server)){
				std::cerr << "error !" << std::endl;
				return (1);
			}
			ptr = &server;
			signal(SIGINT, sighandler);
			signal(SIGPIPE, SIG_IGN);
			server.create();
			server.listen();
		}else
			std::cerr << "wrong number of arguments" << std::endl;
	} catch (const std::exception &excp){
		std::cerr << excp.what() << std::endl;
	}
	return (1);
}