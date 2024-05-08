// Logic for the all-albums page
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Albums.hpp"
#include "Albums-odb.hxx"

#include "Track.hpp"
#include "Track-odb.hxx"


void MainWindow::LoadAllAlbumsPage() {

    // TODO: Clear the tracks view


    // Load all albums
    // Set the current index to the all albums page
    ui->mainStackedWidget->setCurrentWidget(ui->allAlbumsPage);

    // Query the database for the albums
    // Update the UI

    if (ui->allAlbumsListView->model() != nullptr) {
        return;
    }

    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    auto model = new QStandardItemModel(this);
    ui->allAlbumsListView->setModel(model);

    // Call the PlayTrack function when the QStandardItem is double clicked
    connect(ui->allAlbumsListView, &QListView::doubleClicked, [=](const QModelIndex& index) {
        LoadPlayListDisplayPage(index);
    });

    // Query for the default playlist
    odb::result<Albums> albums = database_context.query<Albums>();

    // The default playlist
    // What it is: A playlist that contains all the tracks in the folder
    // Basically, this is the user's library. 
    // It is the default playlist that is created when the user opens the application for the first time
    // (but of course, the user doesn't know this)
    // Playlist defaultPlaylist("DEFAULT", "2022"); // Globally defined in mainwindow.h
    if (albums.begin() == albums.end()) {
        return;
    }
    // Add the tracks to the list
    for (odb::result<Albums>::iterator it = albums.begin(); it != albums.end(); it++) {
        Albums album = *(it);

        // Get the first track of the album
        odb::result<Track> tracks = database_context.query<Track>(odb::query<Track>::album_id == album.Id());
        Track track = *(tracks.begin()); // Should return a pointer to the first track in the sequence

        TrackImage track_image = track.Image();

        if (!track_image.Data() || track_image.Size() == 16) {
            image.load(":/otherfiles/assets/images/album.png"); // Replace with default image
        }
        else {
            image.loadFromData(QByteArray::fromRawData(track_image.Data(), track_image.Size() == 16 ? 0 : track_image.Size()), "JPG"); // Pretty much all of the images are JPGs
        }

        // Information about the track
        image = image.scaled(60, 60); // Downscale the image to 60x60
        image = image.convertToFormat(QImage::Format_Indexed8); // Convert the image to an indexed 8-bit image
        pixmap = QPixmap::fromImage(image).scaled(100, 100); // Standardize the icon size across all the tracks
        image = *(new QImage);
        // Create a QStandardItem for the track
        trackView = new QStandardItem(QIcon(pixmap), QString::fromLatin1((album.Title().empty() ? "No title" : album.Title()) + "\n"));
        QString albumYear = QString::fromStdString(album.ReleaseDate() == "0" ? "" : album.ReleaseDate());

        // We can add more information to the trackView if we want
        // For example, we can add the artist, album, etc.
        trackView->setEditable(false);

        // Append the album and artist below the title
        trackView->setText(trackView->text() + albumYear);

        // Resize the trackView to 175 pixels
        trackView->setSizeHint(QSize(125, 175));

        // Have the image fit the trackView
        ui->allAlbumsListView->setIconSize(QSize(100, 100));
        model->appendRow(trackView);
    }

    // Commit the transaction
    t.commit();
}

