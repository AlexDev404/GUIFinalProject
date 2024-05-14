// Logic for the all-tracks page
#include "mainwindow.h"
#include "ui_mainwindow.h"

// For populating the default playlist
#include "database.hpp"
#include "Track_Playlist.hpp"
#include "Track_Playlist-odb.hxx"

// Schemas
#include "Playlist.hpp"

// Mappings
#include "Playlist-odb.hxx"

#include <QMessageBox>

void MainWindow::on_addButton_atp_clicked()
{
	this->UIAddTrack();
}


void MainWindow::LoadAllTracksPage(QListView* listView, QSize size, QSize icon_size) {
    // Update the UI
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    if (listView->model() != nullptr) {
        return;
    }

    auto model = new QStandardItemModel(this);
    listView->setModel(model);

    if (current_role->Name() == "Administrator") {
        // Set the context menu to appear when the user right-clicks an item
        listView->setContextMenuPolicy(Qt::CustomContextMenu);
        // Open a context menu when the user right-clicks an item
        connect(listView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowTracksContextMenu(QPoint)));
    }

        // Call the PlayTrack function when the QStandardItem is double clicked
        connect(listView, &QListView::doubleClicked, [=](const QModelIndex& index) {
            PlayTrack(index);
            });
    

    // Query for the default playlist
    odb::result<Playlist> playlists = database_context.query<Playlist>(odb::query<Playlist>::name == "DEFAULT");

    // The default playlist
    // What it is: A playlist that contains all the tracks in the folder
    // Basically, this is the user's library. 
    // It is the default playlist that is created when the user opens the application for the first time
    // (but of course, the user doesn't know this)
    // Playlist defaultPlaylist("DEFAULT", "2022"); // Globally defined in mainwindow.h
    if (playlists.begin() == playlists.end()) {
        database_context.persist(defaultPlaylist);
    }
    else {
        defaultPlaylist = *playlists.begin();
    }

    // Get all the tracks in the default playlist
    odb::result<Track_Playlist> track_map = database_context.query<Track_Playlist>(odb::query<Track_Playlist>::playlist_id == defaultPlaylist.Id());

    // Add the tracks to the list
    for (odb::result<Track_Playlist>::iterator it = track_map.begin(); it != track_map.end(); it++) {
        // Get the track
        Track track = *(it->TrackId());
        Albums track_album = *(track.AlbumId());
        Artists track_artist = *(track.ArtistId());

        TrackImage track_image = track.Image();
        // Check to see if it's 16 bytes long. If it is, it's an empty image (probably a bug)
        if (!track_image.Data() || track_image.Size() == 16) {
            image.load(":/otherfiles/assets/images/album.png"); // Replace with default image
        }
        else {
            image.loadFromData(QByteArray::fromRawData(track_image.Data(), track_image.Size() == 16 ? 0 : track_image.Size()), "JPG"); // Pretty much all of the images are JPGs
        }

        // Information about the track
        image = image.scaled(60, 60); // Downscale the image to 60x60
        image = image.convertToFormat(QImage::Format_Indexed8); // Convert the image to an indexed 8-bit image
        pixmap = QPixmap::fromImage(image).scaled(icon_size); // Standardize the icon size across all the tracks
        image = *(new QImage);
        // Create a QStandardItem for the track
        trackView = new QStandardItem(QIcon(pixmap), QString::fromLatin1((track.Title().empty() ? track.FileName() : track.Title()) + "\n"));
        QString albumRow = QString::fromStdString(track_album.Title());
        QString artistRow = QString::fromStdString(track_artist.Name());

        // We can add more information to the trackView if we want
        // For example, we can add the artist, album, etc.
        trackView->setEditable(false);

        // Append the album and artist below the title
        trackView->setText(trackView->text() + albumRow + "\n" + artistRow);

        // Resize the trackView to 175 pixels
        trackView->setSizeHint(size);

        // Have the image fit the trackView
        listView->setIconSize(icon_size);
        model->appendRow(trackView);
    }

    // Commit the transaction
    t.commit();
}

// Show the context menu for the user list
void MainWindow::ShowTracksContextMenu(QPoint pos) {
    QPoint item = ui->allTracksListView->mapToGlobal(pos);
    QMenu* trackContextMenu = new QMenu(ui->allTracksListView);
    QAction* deleteTrack = trackContextMenu->addAction("Delete this track");

    trackContextMenu->addAction(deleteTrack);

    QAction* rightClickItem = trackContextMenu->exec(item);

    // Delete the user when the user clicks the delete button
    if (rightClickItem == deleteTrack) {
        QModelIndex index = ui->allTracksListView->currentIndex();
        QStandardItemModel* model = (QStandardItemModel*)ui->allTracksListView->model();
        QStandardItem* item = model->itemFromIndex(index);
        QStringList track_name = item->text().split("\n");
        std::string track_name_only = track_name[0].toStdString();
        std::string track_album_only = track_name[1].toStdString();

        // Delete the user from the database
        odb::sqlite::database database_context = db.getDatabase();
        odb::transaction t(database_context.begin());

        // Query for the album
        Track* track_to_delete = new Track("NO_NAME");

        if (!track_album_only.empty()) {
            // Query for the track
            Albums* track_album = database_context.query_one<Albums>(odb::query<Albums>::title == track_album_only);
            if (track_album != NULL) {
                track_to_delete = database_context.query_one<Track>(odb::query<Track>::title == track_name_only && odb::query<Track>::album_id == track_album->Id());
            }
            else {
                track_to_delete = database_context.query_one<Track>(odb::query<Track>::title == track_name_only);
            }
        }
        else {
            // Query for the track
            track_to_delete = database_context.query_one<Track>(odb::query<Track>::file_location == track_name_only);
        }

        try {

            QMessageBox msgBox;
            msgBox.setWindowTitle("Action confirmation");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("<FONT COLOR='BLACK'>Delete this track?</ FONT>");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            // If we selected no, then return
            if (msgBox.exec() == QMessageBox::No) {
                return;
            }

            if (track_to_delete == NULL) {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Error while deleting track");
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText("<FONT COLOR='BLACK'>Track not found</ FONT>");
                msgBox.exec();
                return;
            }

            // Delete cascade
            database_context.erase(track_to_delete);
        }
        catch (odb::exception& e) {
            qDebug() << e.what();
            std::string what_isthe_error = e.what();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error while deleting track");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(QString::fromStdString(std::string("<FONT COLOR='RED'>" + what_isthe_error + "< / FONT>")));
            msgBox.exec();
        }

        t.commit(); // Save the changes
        // Close the context menu
        trackContextMenu->close();
        // Delete the model
        ui->allTracksListView->model()->deleteLater();

        // Remove the model
        ui->allTracksListView->setModel(nullptr);

        // Delete the model from allTracksPage
        ui->libraryListView->model()->deleteLater();

        // Remove the model from the allTracksPage
        ui->libraryListView->setModel(nullptr);

        // Reload the views
        LoadAllTracksPage(ui->allTracksListView, QSize(125, 175), QSize(100, 100));
        LoadAllTracksPage(ui->libraryListView, QSize(125, 30), QSize(16, 16));
    }
}