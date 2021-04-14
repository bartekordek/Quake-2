#ifndef ____SHARED_GLPOLY_H__
#define ____SHARED_GLPOLY_H__

#include "shared/defines.hpp"

typedef struct glpoly_s
{
    struct    glpoly_s    *next;
    struct    glpoly_s    *chain;
    int        numverts;
    int        flags;            // for SURF_UNDERWATER (not needed anymore?)
    float    verts[4][VERTEXSIZE];    // variable sized (xyz s1t1 s2t2)
} glpoly_t;
#endif // ____SHARED_GLPOLY_H__