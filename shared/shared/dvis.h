#ifndef ____SHARED_SHARED_DVIS_H__
#define ____SHARED_SHARED_DVIS_H__

typedef struct
{
    int numclusters;
    int bitofs[8][2];  // bitofs[numclusters][2]
} dvis_t;

#endif // ____SHARED_SHARED_DVIS_H__