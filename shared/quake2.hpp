
#pragma once

#include "game/game.hpp"
#include "shared/defines.hpp"
#include "shared/entity.hpp"

class quake2 final
{
public:
    static quake2* getInstance();

    cvar* allow_download_players = nullptr;
    cvar* allow_download_models = nullptr;
    cvar* allow_download_sounds = nullptr;
    cvar* allow_download_maps = nullptr;
    cvar* allow_download = nullptr;
    cvar* dedicated = nullptr;
    game_import gi;
    game_export_t ge;
    entity_s* currententity = nullptr;
    image_t* draw_chars = nullptr;

    ~quake2();

protected:
private:
    quake2();

    static quake2* s_instance;
};