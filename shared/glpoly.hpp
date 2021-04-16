#ifndef ____SHARED_GLPOLY_H__
#define ____SHARED_GLPOLY_H__

#include "shared/defines.hpp"

struct glpoly_s
{
    glpoly_s* next = nullptr;
    glpoly_s* chain = nullptr;
    int        numverts;
    int        flags;            // for SURF_UNDERWATER (not needed anymore?)
    float    verts[4][VERTEXSIZE];    // variable sized (xyz s1t1 s2t2)
};
#endif // ____SHARED_GLPOLY_H__