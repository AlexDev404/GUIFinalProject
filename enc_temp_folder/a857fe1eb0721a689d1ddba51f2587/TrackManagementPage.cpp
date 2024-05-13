#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.hpp"

#include "Genres.hpp"
#include "Genres-odb.hxx"

#include "Track.hpp"
#include "Track-odb.hxx"

#include "Track_Playcount.hpp"
#include "Track_Playcount-odb.hxx"

#include "Albums.hpp"
#include "Albums-odb.hxx"

#include "Artists.hpp"
#include "Artists-odb.hxx"

#include "WindowsAccount.hpp"
#include "WindowsAccount-odb.hxx"

#include <qDebug>
#include <QMessageBox>

// So this is the report page. It is a simple page that displays the report of the
// The recently played tracks, albums, and artists. It also displays the total number of
// tracks, albums, and artists played. It also displays the total time spent listening to music
// Which is just all the song durations added up. It also displays the average time spent listening
// To music, which is the total time spent listening to music divided by the total number of tracks played.

void MainWindow::LoadTrackManagementPage() {
    ui->mainStackedWidget->setCurrentWidget(ui->formsPage);

    // Query the database for the total tracks played by the current WindowsAccount
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    auto model = new QStandardItemModel(this);

    std::string currentTabText = ui->managementTab_fp->tabText(ui->managementTab_fp->currentIndex()).toStdString();

    if (currentTabText == "Genres") {

		// Autofill `editGenreTitle` with the genre title when the genre is clicked
        connect(ui->genresListView_fp, &QListView::clicked, [=]() {
            std::string view_data = ui->genresListView_fp->currentIndex().data().toString().toStdString();
            ui->GenreDatabaseID->setText(QString::fromStdString(view_data.substr(view_data.find("\n") + 1)));
			ui->editGenreTitle->setText(QString::fromStdString(view_data.substr(0, view_data.find("\n"))));
        });




		// Set the data for the genres list view
        ui->genresListView_fp->setModel(model);
        odb::result<Genres> genreList = database_context.query<Genres>();
        for (odb::result<Genres>::iterator genreIt = genreList.begin(); genreIt != genreList.end(); genreIt++) {
            // Get the track
            Genres genre = *(genreIt);
            
            // Next: Create a QStandardItem and append it to the list of genres
            QStandardItem* view = new QStandardItem(QString::fromLatin1((genre.Title().empty() ? "No title" : genre.Title()) + "\n" + std::to_string(genre.Id())));
            view->setEditable(false);
            

            model->appendRow(view);
        }
		/// END SET DATA FOR GENRES

        // do something...

        // END GENRES
    }
    else if (currentTabText == "Albums") {
        // Set the data for the albums list view
        ui->albumsListView_fp->setModel(model);
        odb::result<Albums> albumsList = database_context.query<Albums>();
        for (odb::result<Albums>::iterator albumIt = albumsList.begin(); albumIt != albumsList.end(); albumIt++) {
            // Get the album
            Albums album = *(albumIt);

            // Next: Create a QStandardItem and append it to the list of album
            QStandardItem* view = new QStandardItem(QString::fromLatin1((album.Title().empty() ? "No title" : album.Title())));
            view->setEditable(false);
            model->appendRow(view);
        }
        /// END SET DATA FOR ALBUMS

		// do something...

		// END ALBUMS
    }
    else if (currentTabText == "Tracks") {
        // Set the data for the albums list view
        ui->tracksListView_fp->setModel(model);
        odb::result<Track> tracksList = database_context.query<Track>();
        for (odb::result<Track>::iterator trackIt = tracksList.begin(); trackIt != tracksList.end(); trackIt++) {
            // Get the track
            Track track = *(trackIt);
			Albums track_album = *(track.AlbumId());
			Artists track_artist = *(track.ArtistId());

            // Next: Create a QStandardItem and append it to the list of album
            QStandardItem* view = new QStandardItem(QString::fromLatin1((track.Title().empty() ? "No title" : track.Title()) + "\n" + track_album.Title() + "\n" + track_artist.Name()));

            // Resize to 327px (width)
			view->setSizeHint(QSize(327, 80));

            view->setEditable(false);
            model->appendRow(view);
        }
        /// END SET DATA FOR ALBUMS

        // do something...

        // END ALBUMS
    }
    else if (currentTabText == "Playlists") {
		// Set the data for the playlists list view
        odb::result<Track> trackList = database_context.query<Track>();
        for(odb::result<Track>::iterator trackIt = trackList.begin(); trackIt != trackList.end(); trackIt++){
            // Get the track
            Track track = *(trackIt);
            Albums track_album = *(track.AlbumId());
            Artists track_artist = *(track.ArtistId());

            // Next: Create a QStandardItem and append it to the list of tracks
            QStandardItem* view = new QStandardItem(QString::fromLatin1((track.Title().empty() ? "No title" : track.Title()) + "\n" + track_album.Title() + "\n" + track_artist.Name()));
        
            view->setEditable(false);

            model->appendRow(view);
        }
		/// END SET DATA FOR PLAYLISTS

        // do something...

		// END PLAYLISTS
    }
    else {
        // God knows
        return;
    }

    t.commit(); // Don't need the database anymore beyond this point
}


// Action: Delete the genre on-click
void MainWindow::deleteGenreFromTrackManagement() {
	int genre_id = ui->GenreDatabaseID->text().toInt();
    std::string genre_title = ui->editGenreTitle->text().toStdString();

    odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());

    try {
        // Delete the genre
        database_context.erase<Genres>(genre_id);
    }
	catch (const odb::exception& e) {
		QMessageBox::warning(this, "Error", "Failed to delete genre. Error: " + QString::fromStdString(e.what()));
		return;
	}
	// Commit the transaction
	t.commit();

	// Reload the page
	LoadTrackManagementPage();
}

// Action: Add the genre on-click
void MainWindow::addGenreFromTrackManagement() {
    int genre_id = ui->GenreDatabaseID->text().toInt();
    std::string genre_title = ui->editGenreTitle->text().toStdString();

    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

	if (genre_title.empty()) {
		QMessageBox::warning(this, "Error", "Please enter a genre title.");
		return;
	}

	// Query for an already existing genre
	Genres* genre = database_context.query_one<Genres>(odb::query<Genres>::id == genre_id && odb::query<Genres>::title == genre_title);

    if (genre != NULL) {
        if (genre->Title() == genre_title) {
            QMessageBox msgBox;
            msgBox.setText("Genre already exists.");
            msgBox.setInformativeText("You cannot have two genres of the same name");
            msgBox.exec();
            return;
        }

        // Edit the genre
        try {
            // Edit the genre
            genre->SetTitle(genre_title);
            database_context.update(*genre);
            // Commit the transaction
            t.commit();
            // Reload the page
            LoadTrackManagementPage();
            return;
        }
        catch (const odb::exception& e) {
            QMessageBox::warning(this, "Error", "Failed to edit genre. Error: " + QString::fromStdString(e.what()));
            return;
        }
    }

    // Add the genre
    Genres new_genre(genre_title);
    database_context.persist(new_genre);
    
    // Commit the transaction
    t.commit();

    // Reload the page
    LoadTrackManagementPage();
}