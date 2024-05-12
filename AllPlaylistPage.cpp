#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.hpp"

#include "Track.hpp"
#include "Track-odb.hxx"

#include "Playlist.hpp"
#include "Playlist-odb.hxx"

#include "Albums.hpp"
#include "Albums-odb.hxx"

#include "Track_Playlist.hpp"
#include "Track_Playlist-odb.hxx"

#include <QMessageBox>

#include "CustomRules.hpp"

void MainWindow::LoadAllPlaylistPage() { // it's called that
    ui->mainStackedWidget->setCurrentWidget(ui->AllPlaylistPage);

    // Create the database context
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    // Create a transations to retrieve all the playlists from the database
    odb::result<Playlist> playlist_ptr = database_context.query<Playlist>(odb::query<Playlist>()); // Query everything

    // Attach the model to the `PlaylistDisplayListView`
    auto model = new QStandardItemModel(this);
    ui->PlaylistDisplayListView->setModel(model);
	// Set the context menu to appear when the user right-clicks an item
	ui->PlaylistDisplayListView->setContextMenuPolicy(Qt::CustomContextMenu);
	// Open a context menu when the user right-clicks an item
	connect(ui->PlaylistDisplayListView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowPlaylistContextMenu(QPoint)));

    bool portionsFailed = false;

    // Populate the `libraryDisplayListView`
    for(odb::result<Playlist>::iterator it = playlist_ptr.begin(); it != playlist_ptr.end(); it++){
        if(&it == NULL) continue; // Quietly fail
        Playlist playlist = *(it); // Get the playlist

        /*
          # What's left
          1. Get the playlist name Aiyesha
          2. Get the playlist year Mickali? **queue illuminati music**
          3. Get the playlist duration

          4. Display this information as a card on the UI
          5. On double-click, redirect to libraryDisplayPage
        */

        // Get the track_mapping of the first track of the playlist
        odb::result<Track_Playlist> track_mapping = database_context.query<Track_Playlist>(odb::query<Track_Playlist>::playlist_id == playlist.Id());
        void* track_playlist_ = &(*(track_mapping.begin())); // The first track
        if (track_playlist_ == NULL) {
            portionsFailed = true;
            continue;
        }

        // Do a cast to get a valid object
        Track_Playlist track_playlist = *(Track_Playlist*)(track_playlist_); // should work now (99% sure)
        // Retrieve the track from the playlist
        const Track* track_ = track_playlist.TrackId(); // Cuz this already returns a track.. ready :)to run? yess :) XDD
        // hate this lmao
        if (track_ == NULL) {
            portionsFailed = true;
            continue;
        }


        Track track = *(track_);
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
        // i'll just observe then k **sad
        // Get the playlist name
        string playlist_name = playlist.Name() == "DEFAULT"? "Your library": playlist.Name(); // Default playlist is the user's scanned library
        // Get the playlist year
        string playlist_year = playlist.Year(); 

        // Get the playlist duration
        double playlist_duration = playlist.Duration();

        // what next.. I assume we put images here right? want me to do that...yes miss 
        // wait but playlist don't have image in db/..we can use the first track of the playlist as the cover or just a placeholder
        // thats what i did for the album
        // Display this information as a card on the UI sorry not sorry ruff
        QStandardItem* playlist_card = new QStandardItem(
            QIcon(pixmap), // The icon
            QString::fromStdString( // The text
                std::string(playlist_name + " (" + playlist_year + ")" + "\n" 
                            + "Length: " + std::to_string(playlist_duration/60) // does that still have an error? hb now?good lmao alr...? wait
                            )
                )
            // k miss we are ready (again T-T) lets hope it works this time...RASS T-T
        );

        // The user should not be able to edit this
        playlist_card->setEditable(false);

        // Set the hidden text
        playlist_card->setText(playlist_card->text());

        // Resize the card to 175 pixels
        playlist_card->setSizeHint(QSize(125, 175));

        // Have the image fit the listView
        ui->PlaylistDisplayListView->setIconSize(QSize(100, 100));
        model->appendRow(playlist_card);
    }

    if (portionsFailed) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Database error");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("<FONT COLOR='BLACK'>Portions of the page could not be loaded.</ FONT>");
        msgBox.exec();
    }

    t.commit(); // Dispose of the database context
} 



// Show the context menu for the user list
void MainWindow::ShowPlaylistContextMenu(QPoint pos) {
    // wanna do this one here Lol
    QPoint item = ui->PlaylistDisplayListView->mapToGlobal(pos);
    QMenu* playlistContextMenu = new QMenu(ui->PlaylistDisplayListView);
    QAction* deletePlaylist = ui->PlaylistDisplayListView->addAction("Delete this playlist");

    playlistContextMenu->addAction(deletePlaylist);

    QAction* rightClickItem = playlistContextMenu->exec(item);

    // Delete the user when the user clicks the delete button
    if (rightClickItem == deletePlaylist) {  
        /*
        # Playlist data format
        PLAYLIST_NAME (PLAYLIST_YEAR)
        Duration: PLAYLIST_DURATION/60 minutes

        # As a one-liner
        PLAYLIST_NAME (PLAYLIST_YEAR)\nDuration: PLAYLIST_DURATION/60 minutes
        */

        // Get the currently selected playlist on the playlist page and extract information from it
        QModelIndex index = ui->PlaylistDisplayListView->currentIndex(); // This is the index of the playlist
        QStandardItemModel* model = (QStandardItemModel*)ui->PlaylistDisplayListView->model(); // Convert it into an item model
        QStandardItem* item = model->itemFromIndex(index); // Get the currently selected item
        std::string playlist_name = item->text().toStdString(); // Extract the hidden text from the item (see the format above)
        
        // Extract the information we need
        // Format of playlist as text: See above.
        std::string playlist_name_only_ = playlist_name.substr(0, playlist_name.find("(")); // Start at the beginning of the string, end at the opening "("
        Utility::trim(playlist_name_only_); 
        std::string playlist_name_only = playlist_name_only_ == "Your library"? "DEFAULT": playlist_name_only_; // Is this the default playlist? (the user's library)
        // Start at the opening "(" end at the close ")"
        std::string playlist_year_only = playlist_name.substr(playlist_name.find("(") + 1, playlist_name.find(")") - playlist_name.find("(") - 1);
        Utility::trim(playlist_year_only); 

        // Start off the transaction to remove the playlist from the database
        odb::sqlite::database database_context = db.getDatabase();
        odb::transaction t(database_context.begin());

        // Query for the data needed to be deleted
        Playlist* playlist_to_delete = database_context.query_one<Playlist>(odb::query<Playlist>::name == playlist_name_only 
                                                                            && odb::query<Playlist>::year == playlist_year_only);

        // This here is the logic for the confirmation dialog asking the user if 
        // They meant to initiate the delete process
        try {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Action confirmation");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("<FONT COLOR='BLACK'>Are you sure?</ FONT>");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

			// If we selected no, then return
            if (msgBox.exec() == QMessageBox::No) {
                return;
            }

            // If the album lookup failed (the album is null)
            // Display an error and then stop the process
			if (playlist_to_delete == NULL) {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Error while deleting playlist");
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText("<FONT COLOR='BLACK'>Playlist not found</ FONT>");
                msgBox.exec();
                return;
			}

            if(playlist_to_delete->Name() == "DEFAULT") {
                throw "Cannot delete your own library.";
            }

            // Delete the album (cascade)
            database_context.erase(playlist_to_delete);
        }
        // If we have an error, just display it as a messagebox
        catch (odb::exception& e) {
            qDebug() << e.what();
            std::string what_isthe_error = e.what();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error while deleting playlist");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(QString::fromStdString(std::string("<FONT COLOR='RED'>" + what_isthe_error + "< / FONT>")));
            msgBox.exec();
        }
        catch (std::string& e) {
            qDebug() << e;
            std::string what_isthe_error = e;
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error while deleting playlist");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(QString::fromStdString(what_isthe_error));
            msgBox.exec();
        }
        catch (...) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Unknown error while deleting playlist");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(QString::fromStdString(std::string("Unknown exception while deleting the playlist.")));
            msgBox.exec();
        }
        t.commit(); // Save the changes which part u wanna do :D 
        // Close the context menu
        playlistContextMenu->close();
        // Delete the model
		ui->allAlbumsListView->model()->deleteLater();

        // Remove the model
		ui->allAlbumsListView->setModel(nullptr);

		// Delete the model from allTracksPage
		ui->allTracksListView->model()->deleteLater();

        // Remove the model from the allTracksPage
		ui->allTracksListView->setModel(nullptr);

		// Delete the model from PlaylistDisplayListView
		ui->PlaylistDisplayListView->model()->deleteLater(); // added these though

        // Remove the model from the allTracksPage
		ui->PlaylistDisplayListView->setModel(nullptr);

        LoadAllPlaylistPage(); // Reload the page
    }
}
