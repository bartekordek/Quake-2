#ifndef ____SHARED_SHARED_OBJECTS_H__
#define ____SHARED_SHARED_OBJECTS_H__


#include "game/game.h"

extern volatile game_export_t ge;
extern game_import_t gi;

void initGame();

#endif // ____SHARED_SHARED_OBJECTS_H__