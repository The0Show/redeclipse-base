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