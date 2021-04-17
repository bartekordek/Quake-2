
#pragma once

// destination class for quake2::getInstance()->gi.multicast()
enum class multicast_t : short
{
    MULTICAST_ALL,
    MULTICAST_PHS,
    MULTICAST_PVS,
    MULTICAST_ALL_R,
    MULTICAST_PHS_R,
    MULTICAST_PVS_R
};