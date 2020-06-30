#include <ctype.h>
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

unsigned long long std::to_uuid(const char *str)
{
    unsigned long long result = 0;
    auto char_to_hex = [](char c) -> int { return (c >= '0' && c <= '9') ? (int)(c - '0') : (c >= 'a' && c <= 'f') ? (int)(c - 'a' + 10) : -1; };

    if (strlen(str) == 9)
    {
        /* fat32 uuid format */
        for (size_t i = 0; str[i] != 0; i++)
        {
            if (str[i] == '-')
                continue;
            int hex = char_to_hex(tolower(str[i]));
            if (hex == -1)
            {
                return 0;
            }
            result = (result << 4) | hex;
        }
        result <<= 48;
    }
    else if (strlen(str) == 36)
    {
        /* standard uuid format */
        asm("cli;hlt;");
    }
    return result;
}