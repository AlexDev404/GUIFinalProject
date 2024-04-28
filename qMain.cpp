#include "mainwindow.h"

// Include the database class-schemas
#include "Track.hpp"
#include "Track_Playlist.hpp"
#include "Artists.hpp"
#include "Albums.hpp"
#include "Playlist.hpp"

// Include the database mappings
#include "Track-odb.hxx"
#include "Track_Playlist-odb.hxx"
#include "Artists-odb.hxx"
#include "Albums-odb.hxx"
#include "Playlist-odb.hxx"

// Include command-line output
#include <QDebug>

void createTables(odb::sqlite::database &database_context) {
    try {
	// Create the tables
    // PLEASE NOTE: The tables are created in a specific order to avoid errors
    // Please only edit if needed + if you know what you are doing, because the order is important
    // Otherwise, please ask for help or assistance


    // Left-hand side (Artists, Albums, Genres) -- Autonoumous tables/classes

    // Create the Artists table
    database_context.execute("CREATE TABLE IF NOT EXISTS Artists (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, name TEXT)");
    // Create the Albums table
    database_context.execute("CREATE TABLE IF NOT EXISTS Albums (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, title TEXT, release_date TEXT)");
    // Create the Genres table
    database_context.execute("CREATE TABLE IF NOT EXISTS Genres (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, title TEXT)");

    // Right-hand side (Track, Playlist, Track_Playlist, Track_Playcount, Windows_Account) -- Autonomous tables/classes

    // Create the Playlist table
    database_context.execute("CREATE TABLE IF NOT EXISTS Playlist (id INTEGER PRIMARY KEY, name TEXT, year TEXT, duration REAL)");
    // Create the Roles table
    database_context.execute("CREATE TABLE IF NOT EXISTS Roles (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, name TEXT)");


    // Center-Down (Track_Playcount, Track_Playlist) -- Mix of Autonomous + Dependent tables/classes
    
    // Create the Windows_Account table (independent)
    database_context.execute("CREATE TABLE IF NOT EXISTS Windows_Account (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, username TEXT, role_id TEXT)");

    // Dependent tables

    // Create the Track table
    database_context.execute(std::string("CREATE TABLE IF NOT EXISTS Track (id INTEGER PRIMARY KEY, title TEXT, artist_id INTEGER, album_id INTEGER, year TEXT, genre_id INTEGER,") +
        std::string("lyrics TEXT, duration REAL, file_location TEXT, ") + 
        std::string("FOREIGN KEY(artist_id) REFERENCES Artists(id), FOREIGN KEY(album_id) REFERENCES Albums(id), FOREIGN KEY(genre_id) REFERENCES Genres(id))"));

    // Create the Track_Playlist table
    database_context.execute(std::string("CREATE TABLE IF NOT EXISTS Track_Playlist (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, track_id INTEGER, playlist_id INTEGER, ") +
                             std::string("FOREIGN KEY(track_id) REFERENCES Track(id), FOREIGN KEY(playlist_id) REFERENCES Playlist(id))"));

    // Create the Track_Playcount table
    database_context.execute(std::string("CREATE TABLE IF NOT EXISTS Track_Playcount (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, user_id INTEGER, ") +
                             std::string("track_id INTEGER, count INTEGER, FOREIGN KEY(track_id) REFERENCES Track(id), FOREIGN KEY(user_id) REFERENCES Windows_Account(id))"));
    }
    catch (const odb::exception& e) {
        qDebug() << e.what();
    }
    catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

void MainWindow::qMain() {
    // Main entry point
    // Setup database
    // --------------------------------- DEBUG ----------------------------------------------------------------------------------------------
    db = *new database();
    db.setDatabase("userdata");
    odb::sqlite::database database_context = db.getDatabase();

    odb::transaction t(database_context.begin());

    // Create the tables
    createTables(database_context);

    // .. Create a new playlist
    Playlist playlist("My Playlist", "2015");

    // Create a new artist
    Artists the_beatles("The Beatles");

    // Create the two albums (name, year)
    Albums one_remastered("1 (Remastered)", "2015");
    Albums let_it_be_remastered("Let It Be (Remastered)", "2015");
    Genres rock("Rock");

    // string title, int* artist_id, string year, string file_location

    // Create a new track
    Track track("Hey, Jude", &the_beatles, &one_remastered, "1968", 3.50, "C://Users//Downloads//The_Beatles__Hey_Jude.mp3");
    Track track2("Let It Be", &the_beatles, &let_it_be_remastered, "1970", 4.00, "C://Users//Downloads//The_Beatles__Let_It_Be.mp3");
    
    // Rock and roll song using the full track constructor
    Track rock_and_roll("Rock and Roll Music", &the_beatles, &one_remastered, &rock, "x", "1964", 1.25, "C://Users//Downloads//The_Beatles__Rock_and_Roll_Music.mp3");

    // Add the tracks to the playlist
    Track_Playlist playlist_map_0(&track, &playlist); // Track_Playlist.map(playlist, track);
    Track_Playlist playlist_map_1(&track2, &playlist); // Track_Playlist.map(playlist, track2);

    // Save the artist and track
    unsigned long album_0_id, album_1_id, genre_0_id, the_beatles_id, track_0_id, track_1_id, track_2_id, playlist_map_0_id, playlist_map_1_id, playlist_0_id;
    try {
        the_beatles_id = database_context.persist(the_beatles);
        album_0_id = database_context.persist(one_remastered);
        album_1_id = database_context.persist(let_it_be_remastered);
        genre_0_id = database_context.persist(rock);
        track_0_id = database_context.persist(rock_and_roll);
        track_1_id = database_context.persist(track);
        track_2_id = database_context.persist(track2);

        // Playlist and its mappings
        playlist_0_id = database_context.persist(playlist);
        playlist_map_0_id = database_context.persist(playlist_map_0);
        playlist_map_1_id = database_context.persist(playlist_map_1);
        
    } catch(const odb::exception& e) {
        qDebug() << e.what();
	}
    // Execute SQL commands to create the table
    //database_context.execute("CREATE TABLE IF NOT EXISTS Person (id INTEGER PRIMARY KEY, first TEXT, last TEXT, age INTEGER)");
    t.commit();
    // -------------------------------- END_DEBUG --------------------------------------------------------------------------------------------
}