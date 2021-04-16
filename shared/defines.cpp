#include "shared/defines.hpp"

char	*suf[6] = {"rt", "bk", "lf", "ft", "up", "dn"};

char* toChar( const std::string& someString )
{
    return (char*) someString.c_str();
}