#include "engine.h"

struct musicslot {
    char* name;
    int incidence;
};

struct playlist {
    // char* name;
    int max_incidence;
    vector<musicslot> slots;
};

extern bool playlistexists(const char *id);
extern musicslot pickfromplaylist(const char *id);