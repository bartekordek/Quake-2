#ifndef ____SHARED_VRECT_H__
#define ____SHARED_VRECT_H__

typedef struct vrect_s
{
    int                             x,y,width,height;
    struct vrect_s  *pnext;
} vrect_t;

#endif // ____SHARED_VRECT_H__