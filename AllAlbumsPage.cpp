// Logic for the all-albums page
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Albums.hpp"
#include "Albums-odb.hxx"

#include "Track.hpp"
#include "Track-odb.hxx"

#include <QMessageBox>


void MainWindow::LoadAllAlbumsPage() {

    // TODO: Clear the tracks view
	bool portionsFailed = false; // If any portion of the code fails, set this to true

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
	// Set the context menu to appear when the user right-clicks an item
	ui->allAlbumsListView->setContextMenuPolicy(Qt::CustomContextMenu);
	// Open a context menu when the user right-clicks an item
	connect(ui->allAlbumsListView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowAlbumContextMenu(QPoint)));

    // Call the PlayTrack function when the QStandardItem is double clicked
    connect(ui->allAlbumsListView, &QListView::doubleClicked, [=](const QModelIndex& index) {
        LoadPlayListDisplayPageAlbums(index);
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
        void* track_ = &(*(tracks.begin())); 

        if (track_ == NULL) {
			portionsFailed = true;
            continue;
        }


		// Get the image of the track
        Track track = *(Track*)(track_);
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

    if (portionsFailed) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Database error");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("<FONT COLOR='BLACK'>Portions of the page could not be loaded.</ FONT>");
        msgBox.exec();
    }


    // Commit the transaction
    t.commit();
}

// Show the context menu for the user list
void MainWindow::ShowAlbumContextMenu(QPoint pos) {
    QPoint item = ui->allAlbumsListView->mapToGlobal(pos);
    QMenu* albumContextMenu = new QMenu(ui->allAlbumsListView);
    QAction* deleteAlbum = albumContextMenu->addAction("Delete this album");

    albumContextMenu->addAction(deleteAlbum);

    QAction* rightClickItem = albumContextMenu->exec(item);

    // Delete the user when the user clicks the delete button
    if (rightClickItem == deleteAlbum) {
        QModelIndex index = ui->allAlbumsListView->currentIndex();
        QStandardItemModel* model = (QStandardItemModel*)ui->allAlbumsListView->model();
        QStandardItem* item = model->itemFromIndex(index);
        std::string album_name = item->text().toStdString();
        std::string album_name_only = album_name.substr(0, album_name.find("\n"));
        std::string album_year_only = album_name.substr(album_name.find("\n") + 1);

        // Delete the user from the database
        odb::sqlite::database database_context = db.getDatabase();
        odb::transaction t(database_context.begin());

        // Query for the user
        Albums* album_to_delete = database_context.query_one<Albums>(odb::query<Albums>::title == album_name_only && odb::query<Albums>::release_date == album_year_only);

        try {

            QMessageBox msgBox;
            msgBox.setWindowTitle("Action confirmation");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("<FONT COLOR='BLACK'>This will also delete any associated tracks.<BR/>Are you sure?</ FONT>");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

			// If we selected no, then return
            if (msgBox.exec() == QMessageBox::No) {
                return;
            }

			if (album_to_delete == NULL) {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Error while deleting album");
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText("<FONT COLOR='BLACK'>Album not found</ FONT>");
                msgBox.exec();
                return;
			}

            // Delete cascade
            database_context.erase(album_to_delete);
        }
        catch (odb::exception& e) {
            qDebug() << e.what();
            std::string what_isthe_error = e.what();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error while deleting album");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(QString::fromStdString(std::string("<FONT COLOR='RED'>" + what_isthe_error + "< / FONT>")));
            msgBox.exec();
        }

        t.commit(); // Save the changes
        // Close the context menu
        albumContextMenu->close();
        // Delete the model
		ui->allAlbumsListView->model()->deleteLater();

        // Remove the model
		ui->allAlbumsListView->setModel(nullptr);

		// Delete the model from allTracksPage
		ui->allTracksListView->model()->deleteLater();

        // Remove the model from the allTracksPage
		ui->allTracksListView->setModel(nullptr);

        LoadAllAlbumsPage(); // Reload the page
    }
}
