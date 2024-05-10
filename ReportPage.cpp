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
    ui->mainStackedWidget->setCurrentIndex(7);

    // Query the database for the total tracks played by the current WindowsAccount
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    auto model = new QStandardItemModel(this);
    ui->reportListView->setModel(model);
    // Query the database for the total tracks played by the current WindowsAccount
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    auto model = new QStandardItemModel(this);
    ui->reportlistView->setModel(model);

    // Call the PlayTrack function when the QStandardItem is double clicked
    connect(ui->reportListView, &QListView::doubleClicked, [=](const QModelIndex& index) {
        PlayTrack(index);
        });
    // Call the PlayTrack function when the QStandardItem is double clicked
    connect(ui->reportlistView, &QListView::doubleClicked, [=](const QModelIndex& index) {
        PlayTrack(index);
        });

    // Populate the reportlistView with the calculated statistics
    if (ui->reportTypeComboBox->currentText() == "Tracks") {
		string user_id = std::to_string(currentUser.Id());
        odb::result<Track_Playcount> playCounts;

        try {
           // Query the track_Playcount table
           playCounts = database_context.query<Track_Playcount>(
               ("WHERE user_id = " + user_id + " ORDER BY " + odb::query<Track_Playcount>::count + " DESC LIMIT 5")
        ); // The top 5 tracks

        } catch (odb::exception& error) {
            qDebug() << error.what();
            // Handle the error
            // Display the error message
			QMessageBox::critical(this, "Error", QString::fromStdString(error.what()));
        }

        // Top 5 count
        int rank = 1;

        // Append the top 5 tracks to the listView
        // Loop through the `playCounts` five times
        for(odb::result<Track_Playcount>::iterator trackNow = playCounts.begin(); trackNow != playCounts.end(); trackNow++) {
            
            // The track
            Track track = *(trackNow->TrackId());
            // The user
            Windows_Account user = *(trackNow->UserId());

            if (user.Id() == currentUser.Id()) {
                Albums track_album = *(track.AlbumId());
                Artists track_artist = *(track.ArtistId());

                TrackImage track_image = track.Image();
                // Check to see if it's 16 bytes long. 
                if (!track_image.Data() || track_image.Size() == 16) {
                    image.load(":/otherfiles/assets/images/album.png"); // Replace with default image
                }
                else {
                    image.loadFromData(QByteArray::fromRawData(track_image.Data(), track_image.Size() == 16 ? 0 : track_image.Size()), "JPG"); // Pretty much all of the images are JPGs
                }

                // Information about the track
                image = image.scaled(60, 60); // Downscale the image to 60x60
                image = image.convertToFormat(QImage::Format_Indexed8); // Convert the image to an indexed 8-bit image
                pixmap = QPixmap::fromImage(image).scaled(QSize(100,100)); // Standardize the icon size across all the tracks
                image = *(new QImage);

                // Create a QStandardItem for the track
                QStandardItem* view = new QStandardItem(QIcon(pixmap), QString::fromLatin1((track.Title().empty() ? track.FileName() : track.Title())));

                // We can add more information to the trackView if we want
                // For example, we can add the artist, album, etc.
                view->setEditable(false);

                // Have the image fit the trackView
                ui->reportlistView->setIconSize(QSize(100, 100));

                // Append the album and artist below the title
                view->setText("#" + QString::number(rank) + " " + view->text());

                // Append the item to the model
                model->appendRow(view); 

                rank++;

            }
        }
    }
    else if (ui->reportTypeComboBox->currentText() == "Albums") {
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

        // Top 5 count
        int rank = 1;

        // Append the top 5 tracks to the listView
        // Loop through the `playCounts` five times
        for (odb::result<Track_Playcount>::iterator trackNow = playCounts.begin(); trackNow != playCounts.end(); trackNow++) {

            // The track
            Track track = *(trackNow->TrackId());
            // The user
            Windows_Account user = *(trackNow->UserId());

            if (user.Id() == currentUser.Id()) {
                Albums track_album = *(track.AlbumId());
                Artists track_artist = *(track.ArtistId());

                TrackImage track_image = track.Image();
                // Check to see if it's 16 bytes long. 
                if (!track_image.Data() || track_image.Size() == 16) {
                    image.load(":/otherfiles/assets/images/album.png"); // Replace with default image
                }
                else {
                    image.loadFromData(QByteArray::fromRawData(track_image.Data(), track_image.Size() == 16 ? 0 : track_image.Size()), "JPG"); // Pretty much all of the images are JPGs
                }

                // Information about the track
                image = image.scaled(60, 60); // Downscale the image to 60x60
                image = image.convertToFormat(QImage::Format_Indexed8); // Convert the image to an indexed 8-bit image
                pixmap = QPixmap::fromImage(image).scaled(QSize(100, 100)); // Standardize the icon size across all the tracks
                image = *(new QImage);

                // Create a QStandardItem for the track
                QStandardItem* view = new QStandardItem(QIcon(pixmap), QString::fromLatin1((track.Title().empty() ? track.FileName() : track.Title())));

                // We can add more information to the trackView if we want
                // For example, we can add the artist, album, etc.
                view->setEditable(false);

                // Have the image fit the trackView
                ui->reportlistView->setIconSize(QSize(100, 100));

                // Append the album and artist below the title
                view->setText("#" + QString::number(rank) + " " + view->text());

                // Append the item to the model
                model->appendRow(view);

                rank++;

            }
        }
    }
    else if (ui->reportTypeComboBox->currentText() == "Artists") {
        // Display top 5 most played artists
        // You need to implement this part to query and display top 5 artists
    }
    else if (ui->reportTypeComboBox->currentText() == "Choose a report") {
        // Display top 5 most played artists
        // You need to implement this part to query and display top 5 artists
    }

    t.commit(); // Don't need the database anymore beyond this point
}