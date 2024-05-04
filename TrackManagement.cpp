#include "TrackManagement.hpp"
#include "TrackImage.hpp"

#include "taglib/id3v2tag.h"
#include "taglib/id3v2frame.h"
#include "taglib/mpegfile.h"
#include "taglib/id3v2header.h"
#include "taglib/attachedpictureframe.h"

#include <fstream>
#include <string>

// For the UI
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDirIterator>

#include <QFile>
#include <QFileDialog>

int TrackManagement::AddTrack(std::string& fileLocation, Playlist& defaultPlaylist, odb::sqlite::database& database_context) {

    TagLib::MPEG::File mpegFile(fileLocation.c_str());
    TagLib::ID3v2::Tag* id3v2tag = mpegFile.ID3v2Tag();
    TagLib::ID3v2::FrameList frame;
    TagLib::ID3v2::AttachedPictureFrame* coverArt = NULL;

    if (id3v2tag == NULL) {
        return 1;
    }

    // Query to see if the album already exists
    odb::result<Albums> albums = database_context.query<Albums>(odb::query<Albums>::title == id3v2tag->album().toCString());
    Albums album(id3v2tag->album().toCString(), std::to_string(id3v2tag->year()));
    if (albums.begin() == albums.end()) {
        database_context.persist(album);
    }
    else {
        album = *albums.begin();
    }

    // Query to see if the artist already exists
    odb::result<Artists> artists = database_context.query<Artists>(odb::query<Artists>::name == id3v2tag->artist().toCString());
    Artists artist(id3v2tag->artist().toCString());
    if (artists.begin() == artists.end()) {
        database_context.persist(artist);
    }
    else {
        artist = *artists.begin();
    }
    

    // Query to see if the genre already exists
    odb::result<Genres> genres = database_context.query<Genres>(odb::query<Genres>::title == id3v2tag->genre().toCString());
    Genres genre(id3v2tag->genre().toCString());
    if (genres.begin() == genres.end()) {
        database_context.persist(genre);
    }
    else {
        genre = *genres.begin();
    }


    // Query to see if the genre already exists
    odb::result<Track> tracks = database_context.query<Track>(odb::query<Track>::file_location == fileLocation);
    if (tracks.begin() != tracks.end()) {
        return 1; // Exists
	}

    // Get the duration
    int duration = mpegFile.audioProperties()->lengthInMilliseconds() / 1000;
    TrackImage image(0x0, 0); // Initialize the image object


    // Extract the album art
        // Picture frame                              // 0x03 is the ID for the front cover image (APIC = "Attached PICture Frame")
        frame = id3v2tag->frameListMap()["APIC"]; // "APIC" is the frame ID for attached picture frames in ID3v2 tags
                                                      // There can be multiple "APIC" frames in a tag (e.g. one for the front cover, one for the back cover, etc.)
        if (!frame.isEmpty()) {
            for (TagLib::ID3v2::FrameList::ConstIterator it = frame.begin(); it != frame.end(); ++it) {
				coverArt = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(*it);
                if (coverArt->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover) {
					// Begin writing the image to memory
                    void* source_image = malloc(coverArt->picture().size());
                    if (source_image) {
                        // Copy the image data to the source_image buffer
                        memcpy(source_image, coverArt->picture().data(), coverArt->picture().size());
                        // Set the image object to the album art
                        image = TrackImage((char*)source_image, coverArt->picture().size()); // Cast the void* to a char* to match the constructor
                        //free(source_image); // Free the memory allocated for the image
                    }
                    else {
						qDebug() << "Failed to allocate memory for the image";
                    }
					break;
			}
        }
	}

    // Create a new track object
    Track track(id3v2tag->title().toCString(), &artist, &album, &genre, "", std::to_string(id3v2tag->year()), duration, fileLocation, image);
    if (tracks.begin() == tracks.end()) {
            database_context.persist(track);

        try {
            // Add the track to the default playlist
            Track_Playlist playlist_map(&track, &defaultPlaylist);
            database_context.persist(playlist_map);

        }
        catch (odb::exception& e) {
            qDebug() << e.what();
        }

    }

    // Cleanup (to avoid another memory leak)
    frame.clear();
    frame.~List();
    id3v2tag->~Tag();
    return 0;
}

int TrackManagement::RemoveTrack(Track& track, odb::sqlite::database& database_context) {
	odb::result<Track> tracks = database_context.query<Track>(odb::query<Track>::file_location == track.FileName());
    if (tracks.begin() != tracks.end()) {
        try{
			database_context.erase(*tracks.begin());
		}
        catch (odb::exception& e) {
			qDebug() << e.what();
			return 1;
		}
		return 0;
	}
	return 1;
}

// MainWindow external functions

/// <summary>
/// Abstraction of the AddTrack function
/// Adds a track to the database and updates the UI
/// </summary>
void MainWindow::UIAddTrack() {
    // Open a file dialog to select a folder
    folderPath = QFileDialog::getExistingDirectory(this, tr("Open Folder"), "C:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (folderPath.isEmpty()) {
        // Clean up (This actually does something)
        folderPath.clear();
        folderPath.~QString();
        folderPath = nullptr;
        return;
    }

    // Let's try to get the default playlist
    db = *new database();
    db.setDatabase("userdata");
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    // Query for the default playlist
    odb::result<Playlist> playlists = database_context.query<Playlist>(odb::query<Playlist>::name == "DEFAULT");

    // The default playlist
    // What it is: A playlist that contains all the tracks in the folder
    // Basically, this is the user's library. 
    // It is the default playlist that is created when the user opens the application for the first time
    // (but of course, the user doesn't know this)
    if (playlists.begin() == playlists.end()) {
        database_context.persist(defaultPlaylist);
    }
    else {
        defaultPlaylist = *playlists.begin();
    }

    // Walk through the folder and get all MP3 files in it
    QDirIterator it(folderPath, QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        // Get the next file
        it.next();

        // Parse the ID3 tags
        std::string filename = it.filePath().toStdString();
        //TagLib::FileRef f(filename.c_str());
        //if (!f.tag()) {
            //qDebug() << "No ID3 tag found";
          //  continue; // Skip the file if there is no ID3 tag
        //}

        // Get the file location
        std::string fileLocation = it.filePath().toStdString();

        // Add the track to the default playlist
        int exists = TrackManagement::AddTrack(fileLocation, defaultPlaylist, database_context);
        if (exists == 1) {
            continue;
        }
    }

    // Update the default playlist and commit the transaction
    database_context.update(defaultPlaylist);
    t.commit();

    // Destroy all our objects
    // Clean up (to be safe)
    folderPath.clear();
    folderPath.~QString();
    folderPath = nullptr;
    // Last time, the memory was spiking up to nearly ~800MB
    // This time, it's ~243MB

    // Update the UI
    StateHasChanged(ui->allTracksListView, QSize(125, 175), QSize(100, 100));
    StateHasChanged(ui->libraryListView, QSize(125, 30), QSize(16, 16));
}


/// <summary>
/// Plays a track from the list view
/// </summary>
/// <param name="index"></param>
void MainWindow::PlayTrack(const QModelIndex& index) {

    //qDebug() << "Playing track: " << index.data().toString();

    // Split the index data by the newline into a QStringList 
    QStringList trackInfo = index.data().toString().split("\n");
    QString trackFileName = trackInfo[0]; // Typically the file path
    // FileName, TrackTitle, AlbumName, ArtistName 


    // Play the track
    // Display the track information

    if (trackInfo.size() < 3 || (trackInfo[1].toStdString() == ""))
    {
        qDebug() << "Invalid track information";

        QImage placeholder;
        placeholder.load(":/otherfiles/assets/images/album.png");

        // Set the QLabel, "track_image_pa" to the album art of the track
        ui->track_image_pa->setPixmap(QPixmap::fromImage(placeholder));

        // Set the QLabel, "track_name_pa" to the title of the track
        ui->track_name_pa->setText("Untitled Track");

        // Set the QLabel, "mia_pa_album" to the album of the track
        ui->mia_pa_album->setText("No album available");

        // Set the QLabel, "mia_pa" to the artist of the track
        ui->mia_pa->setText("No artist or missing information");
        // return;
    }
    else {

        // Retrieve the track from the database using the track title, album name and artist name
        odb::sqlite::database database_context = db.getDatabase();
        odb::transaction t(database_context.begin());

        // Query for the album
        Albums* track_album = database_context.query_one<Albums>(odb::query<Albums>::title == trackInfo[1].toStdString());
        // Query for the artist
        Artists* track_artist = database_context.query_one<Artists>(odb::query<Artists>::name == trackInfo[2].toStdString());

        // Query for the track
        currentTrack = *(database_context.query_one<Track>(
            odb::query<Track>::title == trackInfo[0].toStdString()
            && odb::query<Track>::album_id == track_album->Id()
            && odb::query<Track>::artist_id == track_artist->Id()));


        qDebug() << "Now Playing: " << QString::fromStdString(currentTrack.Title());

        TrackImage track_image = currentTrack.Image();

		// Set the play area data
        SetPlayAreaData(track_image, currentTrack.Title(), track_album->Title(), track_artist->Name());


        trackFileName = QString::fromStdString(currentTrack.FileName());

        t.commit();
    }

    track_url = new QUrl(trackFileName);

    player->stop();
    player->setSource(*track_url);

    // Check if there was an error setting the media content
    if (player->error() != QMediaPlayer::NoError) {
        qDebug() << "----Error setting media content----";
        qDebug() << "Error:" << player->errorString();
        qDebug() << "File path:" << track_url;
        return;
    }

    // Set the audio device and play the media
    player->setAudioOutput(device);
    player->play();
}

void MainWindow::SetPlayAreaData(TrackImage& track_image, std::string track_title, std::string album_name, std::string artist_name) {
    // Set the QLabel, "track_image_pa" to the album art of the track
    ui->track_image_pa->setPixmap(QPixmap::fromImage(QImage::fromData(QByteArray::fromRawData(track_image.Data(), (track_image.Data() == NULL || track_image.Size() == 16 || track_image.Size() < 0) ? 0 : track_image.Size()), "JPG")));

    // Set the QLabel, "track_name_pa" to the title of the track
    ui->track_name_pa->setText(QString::fromStdString(track_title));

    // Set the QLabel, "mia_pa_album" to the album of the track
    ui->mia_pa_album->setText(QString::fromStdString(album_name));

    // Set the QLabel, "mia_pa" to the artist of the track
    ui->mia_pa->setText(QString::fromStdString(artist_name));

    // Run a quick query against the databaseand add the track to the recently played table (Track_Playcount)
    odb::sqlite::database database_context = db.getDatabase();

    // OK, so knowing that we have the current track stored in `currentTrack`, we can now add it to the Track_Playcount table
    odb::result<Track_Playcount> track_playcount = database_context.query<Track_Playcount>(odb::query<Track_Playcount>::track_id == currentTrack.Id());
	// If the track is not in the Track_Playcount table, add it
    if (track_playcount.begin() == track_playcount.end()) {
		// Add the track to the Track_Playcount table with a playcount of 1
        Track_Playcount track_playcount_entry(&currentUser, &currentTrack, 1);
        database_context.persist(track_playcount_entry);
    }
	else { // If the track is in the Track_Playcount table, update the playcount
        Track_Playcount track_playcount_entry = *track_playcount.begin();
        track_playcount_entry.SetCount(track_playcount_entry.Count() + 1);
        database_context.update(track_playcount_entry);
    }

	// With this, we are now tracking the playcount of the track
}
