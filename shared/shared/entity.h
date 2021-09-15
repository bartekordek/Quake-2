#ifndef ____SHARED_SHARED_ENTITY_H__
#define ____SHARED_SHARED_ENTITY_H__


typedef struct entity_s
{
    struct model_s* model;  // opaque type outside refresh
    float angles[3];

    /*
    ** most recent data
    */
    float origin[3];  // also used as RF_BEAM's "from"
    int frame;        // also used as RF_BEAM's diameter

    /*
    ** previous data for lerping
    */
    float oldorigin[3];  // also used as RF_BEAM's "to"
    int oldframe;

    /*
    ** misc
    */
    float backlerp;  // 0.0 = current, 1.0 = old
    int skinnum;     // also used as RF_BEAM's palette index

    int lightstyle;  // for flashing entities
    float alpha;     // ignore if RF_TRANSLUCENT isn't set

    struct image_s* skin;  // NULL for inline skin
    int flags;

} entity_t;

#endif // ____SHARED_SHARED_ENTITY_H__