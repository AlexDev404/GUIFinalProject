#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.hpp"

#include "Track.hpp"
#include "Track_Playcount.hpp"
#include "Albums.hpp"
#include "Artists.hpp"
#include "WindowsAccount.hpp"

#include "Track-odb.hxx"
#include "Track_Playcount-odb.hxx"
#include "Albums-odb.hxx"
#include "Artists-odb.hxx"
#include "WindowsAccount-odb.hxx"

#include "CustomRules.hpp"

#include <qDebug>
#include <QMessageBox>

#include <vector>
#include <algorithm>


// So this is the report page. It is  simple page that displays the report of the
// The recently played tracks, albums, and artists. It also displays the total number of
// tracks, albums, and artists played. It also displays the total time spent listening to music
// Which is just all the song durations added up. It also displays the average time spent listening
// To music, which is the total time spent listening to music divided by the total number of tracks played.

void MainWindow::LoadReportPage() {
    ui->mainStackedWidget->setCurrentWidget(ui->reportPage);

    // Query the database for the total tracks played by the current WindowsAccount
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    auto model = new QStandardItemModel(this);
    ui->reportListView->setModel(model);

    ui->reportsPageChooser->setVisible(false);

    // ---------------------------------------------------------- Track Report ----------------------------------------------------------
    // Populate the reportlistView with the calculated statistics
    if (ui->reportTypeComboBox->currentText() == "Tracks") {
        // Disable the reportPageChooser
        ui->reportsPageChooser->setVisible(false);
        ui->mainStackedWidget->setCurrentWidget(ui->reportPage);
        string user_id = std::to_string(currentUser.Id());
        odb::result<Track_Playcount> playCounts;

        try {
            // Query the track_Playcount table
            playCounts = database_context.query<Track_Playcount>(
                ("WHERE user_id = " + user_id + " ORDER BY " + odb::query<Track_Playcount>::count + " DESC LIMIT 5")
            ); // The top 5 tracks

        }
        catch (odb::exception& error) {
            qDebug() << error.what();
            // Handle the error
            // Display the error message
            QMessageBox::critical(this, "Error", QString::fromStdString(error.what()));
        }

        int rank = 1;

        // Append the top 5 tracks to the listView
        // Loop through the `playCounts` five times
        for (odb::result<Track_Playcount>::iterator trackNow = playCounts.begin(); trackNow != playCounts.end(); trackNow++) {

            // The track
            Track track = *(trackNow->TrackId());
            Albums album = *(track.AlbumId());
            Artists artist = *(track.ArtistId());

            // Number of times played
            int track_play_count = trackNow->Count();

            // The user
            Windows_Account user = *(trackNow->UserId());

            if (user.Id() == currentUser.Id()) {

                // Create a QStandardItem for the track
                QStandardItem* view = new QStandardItem(QString::fromLatin1((track.Title().empty() ? track.FileName() : track.Title() + "\nAlbum: " + album.Title() + "\nArtist: " +
                    artist.Name() + "\nYou played this track " + std::to_string(track_play_count) + " time(s).")));

                // Enable editing
                view->setEditable(false);

                // Append the album and artist below the title
                view->setText("#" + QString::number(rank) + " " + view->text());

                // Append the item to the model
                model->appendRow(view);

                rank++;

            }
        }
    }
    // ---------------------------------------------------------- Albums Report ----------------------------------------------------------
    else if (ui->reportTypeComboBox->currentText() == "Albums") {
        // Disable the reportPageChooser
        ui->reportsPageChooser->setVisible(false);

        string user_id = std::to_string(currentUser.Id());
        odb::result<Track_Playcount> playCounts;

        try {
            // Query the track_Playcount table
            playCounts = database_context.query<Track_Playcount>(
                ("WHERE user_id = " + user_id + " ORDER BY " + odb::query<Track_Playcount>::count)
            ); // Find all the tracks listened to by the user and sort by their count

        }
        catch (odb::exception& error) {
            qDebug() << error.what();
            // Handle the error
            // Display the error message
            QMessageBox::critical(this, "Error", QString::fromStdString(error.what()));
        }

        // We are storing the album_id and the count (how many times the album was played)
        std::vector<std::pair<int, int>> album_count; // album_id, album_count

        // Append the top 5 tracks to the listView
        // Loop through the `playCounts` five times
        for (odb::result<Track_Playcount>::iterator trackNow = playCounts.begin(); trackNow != playCounts.end(); trackNow++) {

            // The track
            Track track = *(trackNow->TrackId()); // we gotta loop through all the tracks and then group them 
            // but we have track each one individually
            Windows_Account user = *(trackNow->UserId());
            if (user.Id() == currentUser.Id()) {
                Albums track_album = *(track.AlbumId()); // rtn lemme see whats up with micks..im back

                bool found = false;
                for (std::pair<int, int> album : album_count) {
                    if (album.second == track_album.Id()) {
                        album.first += trackNow->Count();
                        found = true;
                        break;
                    } // we're done sorting it so now we just have to trim the vector to 5 elements 
                    // since those are the 5 most listened albums
                }
                if (!found) {
                    album_count.push_back(std::make_pair(track_album.Id(), trackNow->Count()));
                }
            }
        }

        sort(album_count.begin(), album_count.end(), Utility::compareAlbum);

        album_count.resize(5); // Resize the vector to 5 elements

		int rank = 1;
        bool albumFailed = false;
        // u gotta get the album from the database tho
        for (auto x : album_count) {
            // u gotta retrieve the data from the database using the vector and assign it to the variables
            int album_id = x.first; // 
            // 
            Albums* albums_ = database_context.query_one<Albums>(odb::query<Albums>::id == album_id);

            if (albums_ == NULL) {
                albumFailed = true;
                continue; // Weren't able to load this album since it returned a nullptr
                // assume we are in some sort of loop
            }

            // Prepare the data
            Albums albums = *(albums_);
            // Do a shitty ring-around to get the artist name
            odb::result<Track> track = database_context.query<Track>(odb::query<Track>::artist_id == albums.Id());
            // Let's do some void checks
            // Cast to void
            void* track_is_void = &(*(track.begin()));
            if (track_is_void == NULL) {
                continue; // Quietly fail
            }

            Artists artist = *(Artists*)(track_is_void); // Should be a valid artist now
            string album_artist = artist.Name();
            // Create a QStandardItem for the album
            QStandardItem* album = new QStandardItem(QString::fromStdString(std::string(std::string("#" + std::to_string(rank) + " ") + albums.Title() + "\n" + album_artist + "\nYou played this album " + std::to_string(x.second) + " time(s)")));
            // Set the hidden data
            album->setText(album->text());

            // Append the item to the model
            model->appendRow(album);
            rank++;
            /// Loop ends here
        }
        if (albumFailed) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Load error");
            msgBox.setIcon(QMessageBox::Critical); // are the errors gone now T-T
            msgBox.setText("The database request failed and we weren't able to load some of the albums.");
            msgBox.exec();
        }
    }
    // ---------------------------------------------------------- Artists Report ----------------------------------------------------------
    else if (ui->reportTypeComboBox->currentText() == "Artists") {
        // Display top 5 most played artists

        // Disable the reportPageChooser
        ui->reportsPageChooser->setVisible(false);

        string user_id = std::to_string(currentUser.Id());
        odb::result<Track_Playcount> playCounts;

        try {
            // Query the track_Playcount table
            playCounts = database_context.query<Track_Playcount>(
                ("WHERE user_id = " + user_id)
            ); // Find all the tracks listened to by the user

        }
        catch (odb::exception& error) {
            qDebug() << error.what();
            // Handle the error
            // Display the error message
            QMessageBox::critical(this, "Error", QString::fromStdString(error.what()));
        }

        // Display for rank listing
        int rank = 1;

        std::map<int, int> artist_count; // artist_id, artist_count

        // Accumulate play counts for each artist
        for (odb::result<Track_Playcount>::iterator trackNow = playCounts.begin(); trackNow != playCounts.end(); trackNow++) {
            Track track = *(trackNow->TrackId());
            Windows_Account user = *(trackNow->UserId());
            if (user.Id() == currentUser.Id()) {
                Artists track_artist = *(track.ArtistId());

                if (artist_count.find(track_artist.Id()) != artist_count.end()) {
                    artist_count[track_artist.Id()] += trackNow->Count();
                }
                else {
                    artist_count[track_artist.Id()] = trackNow->Count();
                }
            }
        }

        // Sort the artists based on their play count
        std::vector<std::pair<int, int>> sorted_artist_count(artist_count.begin(), artist_count.end());

        std::sort(sorted_artist_count.begin(), sorted_artist_count.end(), Utility::compareAlbum);

        // Resize the vector to 5 elements
        sorted_artist_count.resize(5);

        // Display the top artists
        for (const auto& artist_pair : sorted_artist_count) {
            int artist_id = artist_pair.first;
            int play_count = artist_pair.second;

            // Get the artist from the DB
            Artists* artist = database_context.query_one<Artists>(odb::query<Artists>::id == artist_id);

            if (artist != nullptr) {
                QStandardItem* artist_item = new QStandardItem(QString::fromStdString(std::string(artist->Name() + "\nYou played tracks by this artist "
                    + std::to_string(play_count) + " time(s)")));

                artist_item->setText("#" + QString::number(rank) + " " + artist_item->text());

                // Enable editing
                artist_item->setEditable(false);

                model->appendRow(artist_item);

                rank++;
            }
        }

    }
    else if (ui->reportTypeComboBox->currentText() == "Choose a report") {
        // Enable the reportPageChooser
        ui->reportsPageChooser->setVisible(true);
    }

    t.commit(); // Don't need the database anymore beyond this point

}
	