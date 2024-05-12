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
    bool portionsFailed = false;

    // Populate the `libraryDisplayListView`
    for (odb::result<Playlist>::iterator it = playlist_ptr.begin(); it != playlist_ptr.end(); it++) {
        if (&it == NULL) continue; // Quietly fail
        Playlist playlist = *(it); // Get the playlist

        /*
          # What's left sirrr are u awake (still lol) dead inside but sadly awake.......................cries** well idk what to say T-T other than .....
          come help meee plss im almost done T-T maybe u can regain some HP LMAO wwith...line 100 where we are displaying the info should i not finish this first........
          yes r u having troule??
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
        string playlist_name = playlist.Name() == "DEFAULT" ? "Your library" : playlist.Name(); // Default playlist is the user's scanned library
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
                    + "Length: " + std::to_string(playlist_duration / 60) // does that still have an error? hb now?good lmao alr...? wait
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
