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

// Include command-line output
#include <qDebug>

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
        std::string("lyrics TEXT, duration REAL, file_location TEXT, FOREIGN KEY(artist_id) REFERENCES Artists(id), FOREIGN KEY(album_id) REFERENCES ") +
        std::string("Artists(id), FOREIGN KEY(genre_id) REFERENCES Genres(id))"));

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
    catch (const odb::database_exception& e) {
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

    // string title, int* artist_id, string year, string file_location

    // Create a new track
    Track track("Hey, Jude", &the_beatles, &one_remastered, "1968", "C://Users//Downloads//The_Beatles__Hey_Jude.mp3");
    Track track2("Let It Be", &the_beatles, &let_it_be_remastered, "1970", "C://Users//Downloads//The_Beatles__Let_It_Be.mp3");

    // Add the tracks to the playlist
    playlist.AddTrack(&track); // Track_Playlist.map(playlist, track);
    playlist.AddTrack(&track2); // Track_Playlist.map(playlist, track2);

    // Save the artist and track
    unsigned long the_beatles_id, track_1_id, track_2_id;
    try {
        the_beatles_id = database_context.persist(the_beatles);
        track_1_id = database_context.persist(track);
        track_2_id = database_context.persist(track2);
    } catch(const odb::exception& e) {
        qDebug() << e.what();
	}
    // Execute SQL commands to create the table
    //database_context.execute("CREATE TABLE IF NOT EXISTS Person (id INTEGER PRIMARY KEY, first TEXT, last TEXT, age INTEGER)");
    t.commit();
    // -------------------------------- END_DEBUG --------------------------------------------------------------------------------------------
}