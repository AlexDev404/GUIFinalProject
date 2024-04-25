#include "mainwindow.h"

// Include the database class-schemas
#include "Track.hpp"
#include "Artists.hpp"
#include "Albums.hpp"
#include "Playlist.hpp"

// Include the database mappings
#include "Track-odb.hxx"
#include "Artists-odb.hxx"
#include "Albums-odb.hxx"
#include "Playlist-odb.hxx"

void MainWindow::qMain() {
    // Main entry point
    // Setup database
    // --------------------------------- DEBUG ----------------------------------------------------------------------------------------------
    db = *new database();
    db.setDatabase("userdata");
    odb::sqlite::database database_context = db.getDatabase();

    odb::transaction t(database_context.begin());

    // .. Create a new playlist
    Playlist playlist("My Playlist", "2015");

    // Create a new artist
    Artists the_beatles("The Beatles");

    // Create the two albums (name, year)
    Albums one_remastered("1 (Remastered)", "2015");
    Albums let_it_be_remastered("Let It Be (Remastered)", "2015");

    // string title, int* artist_id, string year, string file_location

    // Create a new track
    Track track("Hey, Jude", &the_beatles, &one_remastered, "1968", "C://Users//Downloads//The_Beatles__Hey_Jude.mp3");
    Track track2("Let It Be", &the_beatles, &let_it_be_remastered, "1970", "C://Users//Downloads//The_Beatles__Let_It_Be.mp3");

    // Add the tracks to the playlist
    playlist.AddTrack(&track); // Track_Playlist.map(playlist, track);
    playlist.AddTrack(&track2); // Track_Playlist.map(playlist, track2);

    // Save the artist and track
    unsigned long the_beatles_id, track_1_id, track_2_id;
    the_beatles_id = database_context.persist(the_beatles);
    track_1_id = database_context.persist(track);
    track_2_id = database_context.persist(track2);

    // Execute SQL commands to create the table
    //database_context.execute("CREATE TABLE IF NOT EXISTS Person (id INTEGER PRIMARY KEY, first TEXT, last TEXT, age INTEGER)");
    t.commit();
    // -------------------------------- END_DEBUG --------------------------------------------------------------------------------------------
}