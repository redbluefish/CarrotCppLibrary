#ifndef CARROT_CPP_LIB_GENERAL_H
#define CARROT_CPP_LIB_GENERAL_H

#include <exception>
#include <string>

class CarrotCppLibException
{
private:
	std::string content;
public:
	const std::string& what()
	{
		return content;
	}
	CarrotCppLibException(const std::string& str)
	{
		content = str;
	}
	CarrotCppLibException(CarrotCppLibException&) = delete;
	CarrotCppLibException(CarrotCppLibException&&) = default;
	CarrotCppLibException& operator=(CarrotCppLibException&) = delete;
	CarrotCppLibException& operator=(CarrotCppLibException&&) = default;
	
};

#endif