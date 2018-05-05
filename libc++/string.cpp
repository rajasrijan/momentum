#include <string>

std::string operator+(const char *first, std::string second)
{
	size_t new_length = strlen(first) + second.size();
	char *new_string = new char[new_length + 1];
	strcpy(new_string, first);
	strcat(new_string, second.c_str());
	std::string ret = new_string;
	delete[] new_string;
	return ret;
}