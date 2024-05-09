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

#include <qDebug>
#include <QMessageBox>

// So this is the report page. It is a simple page that displays the report of the
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

    // Call the PlayTrack function when the QStandardItem is double clicked
    connect(ui->reportListView, &QListView::doubleClicked, [=](const QModelIndex& index) {
        PlayTrack(index);
        });

    // --------------------------------- Up to here should be right ---------------------------------------


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
        // Append the top 5 tracks to the `ui->reportListView`
        // Loop through the `playCounts` five times
        // Loop until we're at the fifth track (tracks.begin() + 4)
        for (odb::result<Track_Playcount>::iterator trackNow = playCounts.begin(); trackNow != playCounts.end(); trackNow++) {
            // The track
            Track track = *(trackNow->TrackId());
            // The user
            Windows_Account user = *(trackNow->UserId());
            // Reading and comphrending, I told you 11 I'm honestly half awake okay
            // Well which questions do u have maybe i can answer them for u

            // Me still processing sorry..k
            // I'm confused as to how we getting the most listened to. becuase?? Since we doing in base on users
            // I assume we limit the 5 in the if statement good point

            // Me forgetting about the limit at the top
            // loll its okays i changed it just now
            // .. it's now querying the top 5 tracks to the current user now so we dont have to check that anymore:)
            // u can take a look at it and if u have any questions ask me here 
            // so we just display now...yes
            // can u do it??? Probably yeah 
            // ok then i believe in u :D Do i have to process this now:| yes pls
            // just 2 lines of code (... i think?) xDDDDD it the append to the list view...?yes
            // but we not making it facny with the image...?
            // well we can if u rlly wanna?? or if u want we can try again later afterward:)...??

            if (user.Id() == currentUser.Id()) {
                // do something...torture u silly lmao hmm
				Artists artist = *(track.ArtistId());

				// Create a new QStandardItem
                QStandardItem* view = new QStandardItem(QString::fromStdString(track.Title() + "\n" + artist.Name()));
                view->setText(view->text()); // Add the hidden data to allow the track to play
				view->setEditable(false);

                // Append the item to the model
                model->appendRow(view); // here against my will yes
                // compile it finally lmao
            }
        }
    }
    else if (ui->reportTypeComboBox->currentText() == "Albums") {
        // Display top 5 most played albums
        // You need to implement this part to query and display top 5 albums
    }
    else if (ui->reportTypeComboBox->currentText() == "Artists") {
        // Display top 5 most played artists
        // You need to implement this part to query and display top 5 artists
    }

    t.commit(); // Don't need the database anymore beyond this point
}