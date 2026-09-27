#include "musicplaylist.h"

Slotmanager<playlist> music_playlists;
typedef Slotmanager<playlist>::Handle PlaylistHandle;

bool playlistexists(const char *id)
{
    return music_playlists.hasslot(id);
}

playlist &getplaylist(const char *id, int &index)
{
    PlaylistHandle handle = music_playlists[id];
    index = handle.getindex();
    return handle.get();
}

musicslot &addmusicslot(vector<musicslot> &slots)
{
    return slots.add();
}

int addmusic(const char *id, const char *name, int incidence)
{
    int playlist_index = -1;
    playlist &playlist = getplaylist(id, playlist_index);
    if(verbose) conoutf(colourwhite, "Got playlist %s with index %i", id, playlist_index);

    musicslot &slot = addmusicslot(playlist.slots);
    slot.name = newstring(name);
    slot.incidence = incidence;
    if(verbose) conoutf(colourgrey, "Added %s to %s with incidence of %i", slot.name, id, slot.incidence);

    if(playlist.max_incidence < incidence)
    {
        playlist.max_incidence = incidence;
        if(verbose) conoutf(colouryellow, "Playlist %s new max_incidence: %i", id, playlist.max_incidence);
    }

    // conoutf(colorwhite, "%s", slot.name);
    
    return playlist_index;
}

bool clearplaylist(const char *id)
{
    if(!playlistexists(id))
    {
        return false;
    }
    
    music_playlists.remove(id);
    return true;
}

musicslot pickfromplaylist(const char *id)
{
    if(!playlistexists(id))
    {
        conoutf(colourred, "Playlist %s does not exist!", id);
        return { 0 };
    }

    int index = -1;
    playlist &playlist = getplaylist(id, index);

    index = rnd(playlist.slots.length());
    musicslot track = playlist.slots[index];

    if(verbose) conoutf(colourgrey, "CONSIDER %s from %s", track.name, id);

    int roll = rnd(playlist.max_incidence);
    if(roll > track.incidence){
        if(verbose) conoutf(colourred, "%s: %i > %i", track.name, roll, track.incidence);
        return pickfromplaylist(id);
    }

    if(verbose) conoutf(colourgreen, "SELECTED %s from %s", track.name, id);
    
    return track;
}

int playfromplaylist(const char *id)
{
    musicslot slot = pickfromplaylist(id);
    
    playmusic(slot.name, true);
    return 0;
}

ICOMMAND(0, loadmusic, "ssi", (char *id, char *name, int *incidence),
{
    if(*incidence < 0 || *incidence > 1024)
    {
        conoutf(colourred, "Incidence must be between 0 and 1024 (got %i)", *incidence);
        intret(-1);
        return;
    }
    intret(addmusic(id, name, *incidence));
});

ICOMMAND(0, clearplaylist, "s", (char *id),
    intret(clearplaylist(id));
);

ICOMMAND(0, musicplaylist, "s", (char *id),
    playfromplaylist(id);
);