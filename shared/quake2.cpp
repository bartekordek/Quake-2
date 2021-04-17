#include "shared/quake2.hpp"

quake2* quake2::s_instance = nullptr;

quake2* quake2::getInstance()
{
    if ( s_instance == nullptr )
    {
        s_instance = new quake2();
    }
    return s_instance;
}

quake2::quake2() {}

quake2::~quake2() {}