#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.hpp"

#include "Track.hpp"
#include "Track_Playcount.hpp"
#include "Albums.hpp"
#include "Artists.hpp"
#include "WindowsAccount.hpp"
#include "Track_Playlist.hpp"

#include "Track-odb.hxx"
#include "Track_Playcount-odb.hxx"
#include "Albums-odb.hxx"
#include "Artists-odb.hxx"
#include "WindowsAccount-odb.hxx"
#include "Track_Playlist-odb.hxx"

#include <qDebug>
#include <QMessageBox>
#include "CustomRules.hpp"

// So this is the report page. It is a simple page that displays the report of the
// The recently played tracks, albums, and artists. It also displays the total number of
// tracks, albums, and artists played. It also displays the total time spent listening to music
// Which is just all the song durations added up. It also displays the average time spent listening
// To music, which is the total time spent listening to music divided by the total number of tracks played.

void MainWindow::LoadTrackManagementPage(std::string playlistInfo) {
    ui->mainStackedWidget->setCurrentWidget(ui->formsPage);

    // Query the database for the total tracks played by the current WindowsAccount
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    auto model = new QStandardItemModel(this);
    auto model2 = new QStandardItemModel(this);

    std::string currentTabText = ui->managementTab_fp->tabText(ui->managementTab_fp->currentIndex()).toStdString();

    if (currentTabText == "Genres") {

    }
    else if (currentTabText == "Albums") {

    }
    else if (currentTabText == "Tracks") {

    }
    else if (currentTabText == "Playlists") {
        // Return if playlistInfo is empty
        if(playlistInfo.empty()) return;

        // Sidepane #1
        ui->userplaylistview->setModel(model2);
        // Set the stylesheet
        ui->userplaylistview->setStyleSheet("QStandardItem { background-color: #14273f; color: white; border: 1px solid #1c1c1c; }");

        // Get the content from the index
        //string playlistInfo = index.data().toString().toStdString();

        std::string playlist_name_only_ = playlistInfo.substr(0, playlistInfo.find("(")); // Start at the beginning of the string, end at the opening "("
        Utility::trim(playlist_name_only_);

        std::string playlist_name_only = playlist_name_only_ == "Your library" ? "DEFAULT" : playlist_name_only_; // Is this the default playlist? (the user's library)
        // Start at the opening "(" end at the close ")"
        std::string playlist_year_only = playlistInfo.substr(playlistInfo.find("(") + 1, playlistInfo.find(")") - playlistInfo.find("(") - 1);
        Utility::trim(playlist_year_only);

        // set title of the playlist
        ui->playlist_name->setText(QString::fromStdString(playlist_name_only_));

        Playlist* track_playlist = database_context.query_one<Playlist>(odb::query<Playlist>::name == playlist_name_only && odb::query<Playlist>::year == playlist_year_only);

        // The default playlist
        if (track_playlist == NULL) {
            return;
        }

        // Receive tracks associated with the playlist
        odb::result<Track_Playlist> tracks = database_context.query<Track_Playlist>(odb::query<Track_Playlist>::playlist_id == track_playlist->Id());

        // Get all the tracks from the playlist
        for (odb::result<Track_Playlist>::iterator trackIt = tracks.begin(); trackIt != tracks.end(); trackIt++) {
            Track track = *(trackIt->TrackId());
            Albums track_album = *(track.AlbumId());
            Artists track_artist = *(track.ArtistId());

            if (track.Title() != playlist_name_only) {
                // Create a QStandardItem for the track
                QStandardItem* view = new QStandardItem(QString::fromLatin1((track.Title().empty() ? track.FileName() : track.Title() + "\n" + track_album.Title() + "\n" + track_artist.Name())));
                view->setEditable(false);
                model2->appendRow(view);
            }
        }


        // Sidepane #2
        ui->userlibraryview->setModel(model);
        odb::result<Track> trackList = database_context.query<Track>();
        for(odb::result<Track>::iterator trackIt = trackList.begin(); trackIt != trackList.end(); trackIt++){
            // Get the track
            Track track = *(trackIt);
            Albums track_album = *(track.AlbumId());
            Artists track_artist = *(track.ArtistId());

            // Next: Create a QStandardItem and append it to the list of tracks
            QStandardItem* view = new QStandardItem(QString::fromStdString((track.Title().empty() ? "No title" : track.Title()) + "\n" + track_album.Title() + "\n" + track_artist.Name()));
        
            view->setEditable(false);
            view->setText(view->text());
            model->appendRow(view);
        }
    }
    else {
        return;
    }

    t.commit(); // Don't need the database anymore beyond this point
}

// --------------------------- Playlist Management ----------------
void MainWindow::deleteTrackfromPlaylist(){

    // get the index of the trackt to delete
    QModelIndex index = ui->userplaylistview->currentIndex(); // index of the track

    QStandardItemModel* model = (QStandardItemModel*)ui->userplaylistview->model();
    
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());
    
    // Get the content from the index
    QStringList trackInfo = index.data().toString().split("\n");

    // Get the info from the split
    // title
    // album
    // artist
    string track_title = trackInfo[0].toStdString();
    string album_title = trackInfo[1].toStdString();
    string artist_name = trackInfo[2].toStdString();

    // Query track_title
    Albums* album = database_context.query_one<Albums>(odb::query<Albums>::title == album_title);

    // Query artist_name
    Artists* artist = database_context.query_one<Artists>(odb::query<Artists>::name == artist_name);

    // Is this a track in the playlist
    Track* track = database_context.query_one<Track>(odb::query<Track>::title == track_title
                                                  && odb::query<Track>::album_id == album->Id()
                                                  && odb::query<Track>::artist_id == artist->Id());

    try {
        // Get the track_mapping
        Track_Playlist* track_mapping = database_context.query_one<Track_Playlist>(odb::query<Track_Playlist>::track_id == track->Id());
        if(track_mapping == NULL) return;
        // Delete the albums
        database_context.erase<Track_Playlist>(track_mapping->Id());
        // Delete the model
        ui->allAlbumsListView->model()->deleteLater();

        // Remove the model
        ui->allAlbumsListView->setModel(nullptr);

        // Delete the model from allTracksPage
        ui->allTracksListView->model()->deleteLater();

        // Remove the model from the allTracksPage
        ui->allTracksListView->setModel(nullptr);

        // Delete the model from userplaylistview
        ui->userplaylistview->model()->deleteLater();

        // Remove the model from the userplaylistview
        ui->userplaylistview->setModel(nullptr);

        // Delete the model from userlibraryview
        ui->userlibraryview->model()->deleteLater();

        // Remove the model from the userlibraryview
        ui->userlibraryview->setModel(nullptr);
        
    }
    catch (const odb::exception& e) {
        QMessageBox::warning(this, "Error", "Failed to delete playlist. Error: " + QString::fromStdString(e.what()));
        return;
    }
    // Commit the transaction
    t.commit();

    // Reload the page
    LoadTrackManagementPage();

    // ui->managementTab_fp->setCurrentWidget(ui->playlist);

}
