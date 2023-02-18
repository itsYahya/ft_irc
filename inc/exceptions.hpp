#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <iostream>

class myexception : public std::exception{
	private:
		std::string	message;
	public:
		myexception(std::string msg) { message = msg; }
		const char	*what() const throw() { return (message.c_str()); }
		virtual ~myexception() _NOEXCEPT{}
};

#endif