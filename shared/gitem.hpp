#ifndef ____SHARED_GITEM_H__
#define ____SHARED_GITEM_H__

typedef struct gitem_s
{
    char        *classname;    // spawning name
    bool    (*pickup)(struct edict_s *ent, struct edict_s *other);
    void        (*use)(struct edict_s *ent, struct gitem_s *item);
    void        (*drop)(struct edict_s *ent, struct gitem_s *item);
    void        (*weaponthink)(struct edict_s *ent);
    char        *pickup_sound;
    char        *world_model;
    int            world_model_flags;
    char        *view_model;

    // client side info
    char        *icon;
    char        *pickup_name;    // for printing on pickup
    int            count_width;        // number of digits to display by icon

    int            quantity;        // for ammo how much, for weapons how much is used per shot
    char        *ammo;            // for weapons
    int            flags;            // IT_* flags

    int            weapmodel;        // weapon model index (for weapons)

    void        *info;
    int            tag;

    char        *precaches;        // string of all models, sounds, and images this item will use
} gitem_t;

#endif // ____SHARED_GITEM_H__