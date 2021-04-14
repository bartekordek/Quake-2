#ifndef ____SHARED_MMOVE_H__
#define ____SHARED_MMOVE_H__

typedef struct
{
    void    (*aifunc)(struct edict_s *self, float dist);
    float    dist;
    void    (*thinkfunc)(struct edict_s *self);
} mframe_t;

typedef struct
{
    int            firstframe;
    int            lastframe;
    mframe_t    *frame;
    void        (*endfunc)(struct edict_s *self);
} mmove_t;

#endif // ____SHARED_MMOVE_H__