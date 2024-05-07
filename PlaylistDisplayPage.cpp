// Logic for the playlist-view page
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Albums.hpp"
#include "Albums-odb.hxx"

#include "Track_Playlist.hpp"
#include "Track_Playlist-odb.hxx"

void MainWindow::LoadPlayListDisplayPage(const QModelIndex& index) {
    QSize icon_size = QSize(50, 50);
    QSize card_size = QSize(14, 70);

    db = *new database();
    db.setDatabase("userdata");
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

	auto model = new QStandardItemModel(this);
	ui->details_listView_ld->setModel(model);

	// Query for the default playlist
	odb::result<Albums> albums = database_context.query<Albums>();

	// The default playlist
	if (albums.begin() == albums.end()) {
		return;
	}

    // Add the albums to the list
    for (odb::result<Albums>::iterator albumIt = albums.begin(); albumIt != albums.end(); albumIt++) {
        Albums album = *(albumIt);

        // Query for the tracks associated with the current album
        odb::result<Track> tracks = database_context.query<Track>(odb::query<Track>::album_id == album.Id());

        // Add the tracks to the list
        for (odb::result<Track>::iterator trackIt = tracks.begin(); trackIt != tracks.end(); trackIt++) {
            Track track = *(trackIt);
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
            QString artistRow = QString::fromStdString(track_artist.Name());

            // Append the album and artist below the title
            trackView->setText(trackView->text());

            // Resize the trackView to the specified size
            trackView->setSizeHint(card_size);

            // Have the image fit the trackView
            ui->details_listView_ld->setIconSize(icon_size);
            model->appendRow(trackView);
        }
    }

    // Commit the transaction
    t.commit();
    ui->mainStackedWidget->setCurrentIndex(0);
}