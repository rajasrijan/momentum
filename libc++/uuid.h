#ifndef UUID_H
#define UUID_H
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <string>

union uuid_t
{
    uint8_t ui8[16];
    uint16_t ui16[8];
    uint32_t ui32[4];
    uint64_t ui64[2];
};

inline bool operator==(const uuid_t &rhs, const uuid_t &lhs)
{
    return (rhs.ui64[0] == lhs.ui64[0]) && (rhs.ui64[1] == lhs.ui64[1]);
}
namespace std
{

inline uuid_t to_uuid(const char *str)
{
    uuid_t result = {};
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
                return {};
            }
            result.ui32[2] = (result.ui32[2] << 4) | hex;
        }
    }
    else if (strlen(str) == 36)
    {
        /* standard uuid format */
        int idx = 0;
        for (size_t i = 0; str[i] != 0; i++)
        {
            if (str[i] == '-')
                continue;
            int hex = char_to_hex(tolower(str[i]));
            if (hex == -1)
            {
                return {};
            }
            result.ui8[(idx / 2)] = (result.ui8[(idx / 2)] << 4) | hex;
            idx++;
        }
    }
    return result;
}
inline string to_string(const uuid_t &uuid)
{
    char str[64] = {};
    //  for stupid fat32
    if ((uuid.ui64[0] == 0) && (uuid.ui32[3] == 0))
        snprintf(str, sizeof(str), "%hX-%hX", uuid.ui16[5], uuid.ui16[4]);
    else
        snprintf(str, sizeof(str), "%X-%hX-%hX-%hX-%hX%X", uuid.ui32[3], uuid.ui16[5], uuid.ui16[4], uuid.ui16[3], uuid.ui16[2], uuid.ui32[0]);

    return string(str);
}
} // namespace std

#endif //UUID_H