#include "TrackManagement.hpp"

void TrackManagement::addTrack(TagLib::FileRef f, Playlist defaultPlaylist, odb::sqlite::database &database_context, std::string fileLocation) {

    // Query to see if the album already exists
    odb::result<Albums> albums = database_context.query<Albums>(odb::query<Albums>::title == f.tag()->album().toCString());
    Albums album(f.tag()->album().toCString(), std::to_string(f.tag()->year()));
    if (albums.begin() == albums.end()) {
        database_context.persist(album);
    }
    else {
        album = *albums.begin();
    }

    // Query to see if the artist already exists
    odb::result<Artists> artists = database_context.query<Artists>(odb::query<Artists>::name == f.tag()->artist().toCString());
    Artists artist(f.tag()->artist().toCString());
    if (artists.begin() == artists.end()) {
        database_context.persist(artist);
    }
    else {
        artist = *artists.begin();
    }

    // Query to see if the genre already exists
    odb::result<Genres> genres = database_context.query<Genres>(odb::query<Genres>::title == f.tag()->genre().toCString());
    Genres genre(f.tag()->genre().toCString());
    if (genres.begin() == genres.end()) {
        database_context.persist(genre);
    }
    else {
        genre = *genres.begin();
    }

    // Get the duration
    int duration = f.audioProperties()->lengthInMilliseconds() / 1000;

    // Query to see if the genre already exists
    odb::result<Track> tracks = database_context.query<Track>(odb::query<Track>::file_location == fileLocation);
    // Create a new track object
    Track track(f.tag()->title().toCString(), &artist, &album, &genre, "", std::to_string(f.tag()->year()), duration, fileLocation);
    if (tracks.begin() == tracks.end()) {
        database_context.persist(track);



        // Add the track to the default playlist
        Track_Playlist playlist_map(&track, &defaultPlaylist);
        try {
            database_context.persist(playlist_map);
        }
        catch (odb::exception& e) {
            qDebug() << e.what();
        }

    }
}