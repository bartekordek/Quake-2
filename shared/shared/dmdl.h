#ifndef ____SHARED_SHARED_DMDL_H__
#define ____SHARED_SHARED_DMDL_H__


// the glcmd format:
// a positive integer starts a tristrip command, followed by that many
// vertex structures.
// a negative integer starts a trifan command, followed by -x vertexes
// a zero indicates the end of the command list.
// a vertex consists of a floating point s, a floating point t,
// and an integer vertex index.

typedef struct
{
    int ident;
    int version;

    int skinwidth;
    int skinheight;
    int framesize;  // byte size of each frame

    int num_skins;
    int num_xyz;
    int num_st;  // greater than num_xyz for seams
    int num_tris;
    int num_glcmds;  // dwords in strip/fan command list
    int num_frames;

    int ofs_skins;   // each skin is a MAX_SKINNAME string
    int ofs_st;      // byte offset from start for stverts
    int ofs_tris;    // offset for dtriangles
    int ofs_frames;  // offset for first frame
    int ofs_glcmds;
    int ofs_end;  // end of file

} dmdl_t;


#endif // ____SHARED_SHARED_DMDL_H__