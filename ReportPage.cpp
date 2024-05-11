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

    // ---------------------------------------------------------- Track Report ----------------------------------------------------------
    // Populate the reportlistView with the calculated statistics
    if (ui->reportTypeComboBox->currentText() == "Tracks") {
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
                QStandardItem* view = new QStandardItem(QString::fromLatin1((track.Title().empty() ? track.FileName() : track.Title() + "\n" + album.Title() + "\n" + artist.Name() +
                                                                             "\nYou played this track " + std::to_string(track_play_count) + " time(s).")));

                // For example, we can add the artist, album, etc.
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
            ); // Find all the tracks listened to by the user and sort by their `count`

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

        sort(album_count.begin(), album_count.end(), compareAlbum);

        album_count.resize(5); // Resize the vector to 5 elements

        bool albumFailed = false;
        // u gotta get the album from the database tho
        for (auto x : album_count) {
            // u gotta retrieve the data from the database using the vector and assign it to the variables
            int album_id = x.first; // 
            // 
            Albums* albums_ = database_context.query_one<Albums>(odb::query<Albums>::id == album_id);

            if (albums_ == NULL) { // `album_` is the pointer to the album u will generate when getting the data
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
            // nah bro come back and explain cries* T-T i thought u said u didnt wanna have help this timee T-T sob** that's before I thought i could do it
            // well i just made some changes to ur part tell me if its easier now hello :) 
            // bruh if i query i'm querying for the ID...u can query by anything u want but in this case, yes its the ID i  retrieve the id from teh vector then 
            // I query ehat? Wait no promises amen lmao wow ill show u but the n itll disappear *whooshh* so hurry come see
            // yes and i can show u this once but then u gotta remember again >_<  lmao well we shal see LOL
            if (track_is_void == NULL) {
                continue; // Quietly fail
            }

            Artists artist = *(Artists*)(track_is_void); // Should be a valid artist now
            string album_artist = artist.Name();
            // Create a QStandardItem for the album
            QStandardItem* album = new QStandardItem(QString::fromStdString(std::string(albums.Title() + "\n" + album_artist + "\nYou played this album " + std::to_string(x.second) + " time(s)")));
            // Set the hidden data
            album->setText(album->text());

            // Append the item to the model
            model->appendRow(album);
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
    else if (ui->reportTypeComboBox->currentText() == "Artists") {
        // Display top 5 most played artists
        // You need to implement this part to query and display top 5 artists
        // Disable the reportPageChooser
        ui->reportsPageChooser->setVisible(false);
    }
    else if (ui->reportTypeComboBox->currentText() == "Choose a report") {
        // Enable the reportPageChooser
        ui->reportsPageChooser->setVisible(true);
    }

    t.commit(); // Don't need the database anymore beyond this point
}