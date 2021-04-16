#ifndef ____SHARED_MMOVE_H__
#define ____SHARED_MMOVE_H__

#include "shared/edict.hpp"

struct mframe_t
{
    void    (*aifunc)(edict *self, float dist);
    float    dist;
    void    (*thinkfunc)(edict *self);
};

struct mmove_t
{
    int            firstframe;
    int            lastframe;
    mframe_t* frame;
    void        (*endfunc)(edict *self);
};

#endif // ____SHARED_MMOVE_H__