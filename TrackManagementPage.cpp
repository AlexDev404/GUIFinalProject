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
#include "Track_Playlist.hpp"

#include "Track_Playlist-odb.hxx"
#include "TrackManagement.hpp"

#include <qDebug>
#include <QMessageBox>
#include <QFileDialog>
#include "CustomRules.hpp"

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
	auto model2 = new QStandardItemModel(this);

	std::string currentTabText = ui->managementTab_fp->tabText(ui->managementTab_fp->currentIndex()).toStdString();

	if (currentTabText == "Genres") {

		// Autofill `editGenreTitle` with the genre title when the genre is clicked
		connect(ui->genresListView_fp, &QListView::clicked, [=]() {
			std::string view_data = ui->genresListView_fp->currentIndex().data().toString().toStdString();
			ui->GenreDatabaseID->setText(QString::fromStdString(view_data.substr(0, view_data.find("\n"))));
			ui->editGenreTitle->setText(QString::fromStdString(view_data.substr(view_data.find("\n") + 1)));
		});

		// Set the data for the genres list view
		ui->genresListView_fp->setModel(model);
		odb::result<Genres> genreList = database_context.query<Genres>();
		for (odb::result<Genres>::iterator genreIt = genreList.begin(); genreIt != genreList.end(); genreIt++) {
			// Get the track
			Genres genre = *(genreIt);
			
			// Next: Create a QStandardItem and append it to the list of genres
			QStandardItem* view = new QStandardItem(QString::fromStdString(std::to_string(genre.Id()) + "\n" + (genre.Title().empty() ? "No title" : genre.Title())));
			view->setEditable(false);
			

			model->appendRow(view);
		}
		/// END SET DATA FOR GENRES

		// do something...

		// END GENRES
	}
	else if (currentTabText == "Albums") {

		// Autofill `editAlbumTitle` with the album title when the album is clicked
		connect(ui->albumsListView_fp, &QListView::clicked, [=]() {
			std::string view_data = ui->albumsListView_fp->currentIndex().data().toString().toStdString();
			ui->AlbumDatabaseID->setText(QString::fromStdString(view_data.substr(0, view_data.find("\n"))));
			std::string view_data_2 = view_data.substr(view_data.find("\n") + 1);
			ui->editAlbumTitle->setText(QString::fromStdString(view_data_2.substr(0, view_data_2.find("\n"))));
			ui->editAlbumReleaseDate->setText(QString::fromStdString(view_data_2.substr(view_data_2.find("\n") + 1)));
			});

		// Set the data for the albums list view
		ui->albumsListView_fp->setModel(model);
		odb::result<Albums> albumsList = database_context.query<Albums>();
		for (odb::result<Albums>::iterator albumIt = albumsList.begin(); albumIt != albumsList.end(); albumIt++) {
			// Get the album
			Albums album = *(albumIt);

			// Next: Create a QStandardItem and append it to the list of album
			QStandardItem* view = new QStandardItem(QString::fromStdString(std::to_string(album.Id()) + "\n" + (album.Title().empty() ? "No title" : album.Title()) + "\n" + album.ReleaseDate()));
			view->setEditable(false);
			model->appendRow(view);
		}
		/// END SET DATA FOR ALBUMS

		// do something...

		// END ALBUMS
	}
	else if (currentTabText == "Tracks") {
		// Populate the `editTrackArtist` with all the available artists
		odb::result<Artists> artistList = database_context.query<Artists>();
		for (odb::result<Artists>::iterator artistIt = artistList.begin(); artistIt != artistList.end(); artistIt++) {
			// Get the artist
			Artists artist = *(artistIt);

			// Next: Create a QStandardItem and append it to the list of artists
			QStandardItem* view = new QStandardItem(QString::fromStdString((artist.Name().empty() ? "No name" : artist.Name())));
			view->setEditable(false);
			ui->editTrackArtist->addItem(view->text());

		}

		// Populate the `editTrackAlbum` with all the available albums
		odb::result<Albums> albumList = database_context.query<Albums>();
		for (odb::result<Albums>::iterator albumIt = albumList.begin(); albumIt != albumList.end(); albumIt++) {
			// Get the album
			Albums album = *(albumIt);

			// Next: Create a QStandardItem and append it to the list of album
			QStandardItem* view = new QStandardItem(QString::fromStdString((album.Title().empty() ? "No title" : album.Title())));
			view->setEditable(false);
			ui->editTrackAlbum->addItem(view->text());
		}

		// Populate the `editTrackGenre` with all the available genres
		odb::result<Genres> genreList = database_context.query<Genres>();
		for (odb::result<Genres>::iterator genreIt = genreList.begin(); genreIt != genreList.end(); genreIt++) {
			// Get the genre
			Genres genre = *(genreIt);

			// Next: Create a QStandardItem and append it to the list of genre
			QStandardItem* view = new QStandardItem(QString::fromStdString((genre.Title().empty() ? "No title" : genre.Title())));
			view->setEditable(false);
			ui->editTrackGenre->addItem(view->text());
		}

		// Autofill `editGenreTitle` with the genre title when the genre is clicked
		connect(ui->tracksListView_fp, &QListView::clicked, [=]() {
			
			odb::sqlite::database database_context = db.getDatabase();
			odb::transaction t(database_context.begin());

			std::string view_data = ui->tracksListView_fp->currentIndex().data().toString().toStdString();
			/*
			ID
			TITLE
			ALBUM
			ARTIST
			GENRE
			*/
			// Split by "\n"
			const QStringList view_data_split = QString::fromStdString(view_data).split("\n");
			Track* track = database_context.query_one<Track>(odb::query<Track>::id == view_data_split[0].toInt());
			if (track == NULL) {
				return;
			}
			Artists* artist = (Artists*)(track->ArtistId());
			Albums* album = (Albums*)(track->AlbumId());
			Genres* genre = (Genres*)(track->GenreId());

			ui->TrackDatabaseID->setText(view_data_split[0]);
			ui->editTrackTitle->setText(view_data_split[1]);
			
			// Set the artist
			if (artist != NULL) {
				ui->editTrackArtist->setCurrentIndex(artist->Id() - 1);
			}

			// Set the album
			if (album != NULL) {
				//const int album_index = std::stoi(view_data_split[2].toStdString());
				string album_name = view_data_split[2].toStdString(); // This is the album name

				// Get the album name's place in the index of `editTrackAlbum`
				int album_index = ui->editTrackAlbum->findText(QString::fromStdString(album_name));
				ui->editTrackAlbum->setCurrentIndex(album_index);
			}

			// Set the genre
			if (genre != NULL) {
				ui->editTrackGenre->setCurrentIndex(genre->Id() - 1);
			}
		});



		// Set the data for the albums list view
		ui->tracksListView_fp->setModel(model);
		odb::result<Track> tracksList = database_context.query<Track>();
		for (odb::result<Track>::iterator trackIt = tracksList.begin(); trackIt != tracksList.end(); trackIt++) {
			// Get the track
			Track track = *(trackIt);
			Albums track_album = *(track.AlbumId());
			Artists track_artist = *(track.ArtistId());
			Genres track_genre = *(track.GenreId());

			// Next: Create a QStandardItem and append it to the list of album
			QStandardItem* view = new QStandardItem(QString::fromStdString(std::to_string(track.Id()) + "\n" + (track.Title().empty() ? "No title" : track.Title()) + "\n" + track_album.Title() + "\n" + track_artist.Name() + "\n" + track_genre.Title()));
			view->setText(view->text());

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
		qDebug() << "PLAYLISTINFO: " << playlistInfo;
		// Return if playlistInfo is empty
		if(playlistInfo.empty()) return;

		// Sidepane #1
		ui->userplaylistview->setModel(model2);
		// Set the stylesheet
		ui->userplaylistview->setStyleSheet("QStandardItem { background-color: #14273f; color: white; border: 1px solid #1c1c1c; }");

		// Get the content from the index
		//string playlistInfo = index.data().toString().toStdString();

		std::string playlist_name_only_ = playlistInfo.substr(0, playlistInfo.find("(")); // Start at the beginning of the string, end at the opening "("
		Utility::trim(playlist_name_only_);

		std::string playlist_name_only = playlist_name_only_ == "Your library" ? "DEFAULT" : playlist_name_only_; // Is this the default playlist? (the user's library)
		// Start at the opening "(" end at the close ")"
		std::string playlist_year_only = playlistInfo.substr(playlistInfo.find("(") + 1, playlistInfo.find(")") - playlistInfo.find("(") - 1);
		Utility::trim(playlist_year_only);
		//playlistInfo = "";

		// set title of the playlist
		ui->playlist_name->setText(QString::fromStdString(playlist_name_only_));

		Playlist* playlist = database_context.query_one<Playlist>(odb::query<Playlist>::name == playlist_name_only && odb::query<Playlist>::year == playlist_year_only);

		// The default playlist
		if (playlist == NULL) {
			return;
		}

		// Receive tracks associated with the playlist
		odb::result<Track_Playlist> tracks = database_context.query<Track_Playlist>(odb::query<Track_Playlist>::playlist_id == playlist->Id());

		// Get all the tracks from the playlist
		for (odb::result<Track_Playlist>::iterator trackIt = tracks.begin(); trackIt != tracks.end(); trackIt++) {
			Track track = *(trackIt->TrackId());
			Albums track_album = *(track.AlbumId());
			Artists track_artist = *(track.ArtistId());

			if (track.Title() != playlist_name_only) {
				// Create a QStandardItem for the track
				QStandardItem* view = new QStandardItem(QString::fromLatin1((track.Title().empty() ? track.FileName() : track.Title() + "\n" + track_album.Title() + "\n" + track_artist.Name())));
				view->setEditable(false);
				model2->appendRow(view);
			}
		}


		// Sidepane #2
		ui->userlibraryview->setModel(model);
		odb::result<Track> trackList = database_context.query<Track>();
		for(odb::result<Track>::iterator trackIt = trackList.begin(); trackIt != trackList.end(); trackIt++){
			// Get the track
			Track track = *(trackIt);
			Albums track_album = *(track.AlbumId());
			Artists track_artist = *(track.ArtistId());

			// Next: Create a QStandardItem and append it to the list of tracks
			QStandardItem* view = new QStandardItem(QString::fromStdString((track.Title().empty() ? "No title" : track.Title()) + "\n" + track_album.Title() + "\n" + track_artist.Name()));
		
			view->setEditable(false);
			view->setText(view->text());
			model->appendRow(view);
		}
		/// END SET DATA FOR PLAYLISTS

		// do something...

		// END PLAYLISTS
	}
	else {
		// God knows.
		return;
	}

	t.commit(); // Don't need the database anymore beyond this point
}

/************************* GENRES ********************************/

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
	Genres* genre = database_context.query_one<Genres>(odb::query<Genres>::id == genre_id);

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

/************************* ALBUMS ********************************/
// Action: Delete the albums on-click
void MainWindow::deleteAlbumFromTrackManagement() {
	int album_id = ui->AlbumDatabaseID->text().toInt();
	std::string album_title = ui->editAlbumTitle->text().toStdString();
	std::string album_rsd = ui->editAlbumReleaseDate->text().toStdString();

	odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());

	try {
		// Delete the albums
		database_context.erase<Albums>(album_id);
		// Delete the model
		ui->allAlbumsListView->model()->deleteLater();

		// Remove the model
		ui->allAlbumsListView->setModel(nullptr);

		// Delete the model from allTracksPage
		ui->allTracksListView->model()->deleteLater();

		// Remove the model from the allTracksPage
		ui->allTracksListView->setModel(nullptr);
	}
	catch (const odb::exception& e) {
		QMessageBox::warning(this, "Error", "Failed to delete album. Error: " + QString::fromStdString(e.what()));
		return;
	}
	// Commit the transaction
	t.commit();

	// Reload the page
	LoadTrackManagementPage();
}

// Action: Add the albums on-click
void MainWindow::addAlbumFromTrackManagement() {
	int album_id = ui->AlbumDatabaseID->text().toInt();
	std::string album_title = ui->editAlbumTitle->text().toStdString();
	std::string album_rsd = ui->editAlbumReleaseDate->text().toStdString();

	odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());

	if (album_title.empty()) {
		QMessageBox::warning(this, "Error", "Please enter a album title.");
		return;
	}

	// Query for an already existing albums
	Albums* album = database_context.query_one<Albums>(odb::query<Albums>::id == album_id);

	if (album != NULL) {
		//if (album->Title() == album_title) {
		//    QMessageBox msgBox;
		//    msgBox.setText("Album already exists.");
		//    msgBox.setInformativeText("You cannot have two albums of the same name");
		//    msgBox.exec();
		//    return;
		//}

		// Edit the album
		try {
			// Edit the album
			album->SetTitle(album_title);
			album->SetReleaseDate(album_rsd);
			database_context.update(*album);
			// Commit the transaction
			t.commit();

			// Delete the model
			ui->allAlbumsListView->model()->deleteLater();

			// Remove the model
			ui->allAlbumsListView->setModel(nullptr);

			// Delete the model from allTracksPage
			ui->allTracksListView->model()->deleteLater();

			// Remove the model from the allTracksPage
			ui->allTracksListView->setModel(nullptr);

			// Reload the page
			LoadTrackManagementPage();
			return;
		}
		catch (const odb::exception& e) {
			QMessageBox::warning(this, "Error", "Failed to edit album. Error: " + QString::fromStdString(e.what()));
			return;
		}
	}
	else {
		QMessageBox msgBox;
		msgBox.setText("Album does not exist.");
		msgBox.setInformativeText("You cannot add an album that does not exist.");
		msgBox.exec();
	}

	// Add the genre
	//Genres new_genre(genre_title);
	//database_context.persist(new_genre);

	// Commit the transaction
	//t.commit();

	// Reload the page
	//LoadTrackManagementPage();
}

/************************* TRACKS ********************************/
// Action: Add the track on-click
void MainWindow::addTrackFromTrackManagement() {
	int track_id = ui->TrackDatabaseID->text().toInt();
	std::string track_title = ui->editTrackTitle->text().toStdString();
	int track_artist = ui->editTrackArtist->currentIndex() + 1;
	string track_album_name = ui->editTrackAlbum->currentText().toStdString();
	int track_genre = ui->editTrackGenre->currentIndex() + 1;
	
	odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());

	if (track_title.empty()) {
		QMessageBox::warning(this, "Error", "Please enter a track title.");
		return;
	}

	// Query for an already existing track
	Track* track = database_context.query_one<Track>(odb::query<Track>::id == track_id);

	if (track != NULL) {
		/*if (genre->Title() == genre_title) {
			QMessageBox msgBox;
			msgBox.setText("Genre already exists.");
			msgBox.setInformativeText("You cannot have two genres of the same name");
			msgBox.exec();
			return;
		}*/

		// Edit the genre
		try {
			// What's the genre?
			Genres* genre = database_context.query_one<Genres>(odb::query<Genres>::id == track_genre);
			// What's the artist?
			Artists* artist = database_context.query_one<Artists>(odb::query<Artists>::id == track_artist);
			// What's the album?
			Albums* album = database_context.query_one<Albums>(odb::query<Albums>::title == track_album_name);

			if (genre == NULL || artist == NULL || album == NULL) {
				QMessageBox msgBox;
				msgBox.setText("Track does not exist.");
				msgBox.setInformativeText("You cannot add or edit a track that does not exist.");
				msgBox.exec();
				return;
			}

			// Edit the genre
			track->SetTitle(track_title);
			track->SetGenreId(genre);
			track->SetArtistId(artist);
			track->SetAlbumId(album);

			database_context.update(track);
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
	else {
		QMessageBox msgBox;
		msgBox.setText("Track does not exist.");
		msgBox.setInformativeText("You cannot edit a track that does not exist.");
		msgBox.exec();
		return;
	}

	//// What's the genre?
	//Genres* genre = database_context.query_one<Genres>(odb::query<Genres>::id == track_genre);
	//// What's the artist?
	//Artists* artist = database_context.query_one<Artists>(odb::query<Artists>::id == track_artist);
	//// What's the album?
	//Albums* album = database_context.query_one<Albums>(odb::query<Albums>::id == track_album);

	//if (genre == NULL || artist == NULL || album == NULL) {
	//    QMessageBox msgBox;
	//    msgBox.setText("Track does not exist.");
	//    msgBox.setInformativeText("You cannot edit a track that does not exist.");
	//    msgBox.exec();
	//    return;
	//}


	//// Add the new track
	//Track new_track(track_title, artist, album, genre, "", track_year, 2022.00, "C:\\ATE\\", TRACKIMAGE);
	//database_context.persist(new_genre);

	// Commit the transaction
	t.commit();

	// Reload the page
	LoadTrackManagementPage();
}
// Action: Delete the track on-click
void MainWindow::deleteTrackFromTrackManagement() {
	int track_id = ui->TrackDatabaseID->text().toInt();
	odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());

	// Query for an already existing track
	Track* track = database_context.query_one<Track>(odb::query<Track>::id == track_id);

	if (track != NULL) {
		int result = TrackManagement::RemoveTrack(*track, database_context);
		if (result == 0) {
			// Delete the model
			ui->allAlbumsListView->model()->deleteLater();

			// Remove the model
			ui->allAlbumsListView->setModel(nullptr);

			// Delete the model from allTracksPage
			ui->allTracksListView->model()->deleteLater();

			// Remove the model from the allTracksPage
			ui->allTracksListView->setModel(nullptr);
			t.commit();
			LoadTrackManagementPage();
		}
		else {
			QMessageBox msgBox;
			msgBox.setText("Track removal failed.");
			msgBox.setInformativeText("The track removal failed with an unknown error.\nSee console for details.");
			msgBox.exec();
			return;
		}
	}
}
// Action: Edit the track path on-click
void MainWindow::addTrackFromFolder() {
	filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\");
	if (filePath.isEmpty()) {
		// Clean up (This actually does something)
		filePath.clear();
		filePath.~QString();
		filePath = nullptr;
		return;
	}
	int track_id = ui->TrackDatabaseID->text().toInt();
	odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());

	// Query for an already existing track
	Track* track = database_context.query_one<Track>(odb::query<Track>::id == track_id);

	if (track != NULL) {
		track->SetFileName(filePath.toStdString());
		database_context.update(*track);
		t.commit();
		LoadTrackManagementPage();
	}
	else {
		QMessageBox msgBox;
		msgBox.setText("Track does not exist.");
		msgBox.setInformativeText("You cannot edit a track that does not exist.");
		msgBox.exec();
		return;
	}
}

/*********************** PLAYLIST *******************************/
// Action: Add the track to the playlist
void MainWindow::addTrackfromPlaylistFromTrackManagement() {

	// get the index of the trackt to delete
	QModelIndex index = ui->userlibraryview->currentIndex(); // index of the track

	QStandardItemModel* model = (QStandardItemModel*)ui->userlibraryview->model();

	odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());

	// Get the content from the index
	QStringList trackInfo = index.data().toString().split("\n");

	// Get the info from the split
	// title
	// album
	// artist
	string track_title = trackInfo[0].toStdString();
	string album_title = trackInfo[1].toStdString();
	string artist_name = trackInfo[2].toStdString();
	//qDebug() << track_title << album_title << artist_name;

	try {
		// Get the track_mapping
		Track* track = database_context.query_one<Track>(odb::query<Track>::title == track_title
			&& odb::query<Track>::album_id == database_context.query_one<Albums>(odb::query<Albums>::title == album_title)->Id()
			&& odb::query<Track>::artist_id == database_context.query_one<Artists>(odb::query<Artists>::name == artist_name)->Id());
		if (track == NULL) return;
		
	   // Get the playlist
	   // The playlist name is stored in `playlist_name` so we can just query this
		string playlist_name = ui->playlist_name->text().toStdString();
		// Ensure to trim the name so that we don't have any issues
		Utility::trim(playlist_name);
		// Is the name "Your library"? If so we convert this back to "DEFAULT"
		if (playlist_name == "Your library") {
			playlist_name = "DEFAULT";
		}

		// Initiate the query
		Playlist* playlist = database_context.query_one<Playlist>(odb::query<Playlist>::name == playlist_name);

		// Is this a track in the playlist
		Track_Playlist* track_mapping = database_context.query_one<Track_Playlist>(odb::query<Track_Playlist>::playlist_id == playlist->Id() 
																				&& odb::query<Track_Playlist>::track_id == track->Id());
		if (track_mapping != NULL) {
			QMessageBox msgbox;
			msgbox.setText("Track already exists in the playlist.");
			msgbox.setInformativeText("You cannot add the same track to the playlist.");
			msgbox.exec();
			return;
		}

		// Add the track to the playlist
		Track_Playlist new_track_playlist(track, playlist);

		// Persist the track_playlist
		database_context.persist(new_track_playlist);

		// Delete the model
		ui->allAlbumsListView->model()->deleteLater();

		// Remove the model
		ui->allAlbumsListView->setModel(nullptr);

		// Delete the model from allTracksPage
		ui->allTracksListView->model()->deleteLater();

		// Remove the model from the allTracksPage
		ui->allTracksListView->setModel(nullptr);

		// Delete the model from userplaylistview
		ui->userplaylistview->model()->deleteLater();

		// Remove the model from the userplaylistview
		ui->userplaylistview->setModel(nullptr);

		// Delete the model from userlibraryview
		ui->userlibraryview->model()->deleteLater();

		// Remove the model from the userlibraryview
		ui->userlibraryview->setModel(nullptr);

	}
	catch (const odb::exception& e) {
		QMessageBox::warning(this, "Error", "Failed to delete playlist. Error: " + QString::fromStdString(e.what()));
		return;
	}
	// Commit the transaction
	t.commit();

	// Reload the page
	LoadTrackManagementPage();

}
// Action: Delete the track from the playlist
void MainWindow::deleteTrackfromPlaylistFromTrackManagement() {

	// get the index of the trackt to delete
	QModelIndex index = ui->userplaylistview->currentIndex(); // index of the track

	QStandardItemModel* model = (QStandardItemModel*)ui->userplaylistview->model();
	
	odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());
	
	// Get the content from the index
	QStringList trackInfo = index.data().toString().split("\n");

	// Get the info from the split
	// title
	// album
	// artist
	string track_title = trackInfo[0].toStdString();
	string album_title = trackInfo[1].toStdString();
	string artist_name = trackInfo[2].toStdString();

	// Query track_title
	Albums* album = database_context.query_one<Albums>(odb::query<Albums>::title == album_title);

	// Query artist_name
	Artists* artist = database_context.query_one<Artists>(odb::query<Artists>::name == artist_name);

	// Is this a track in the playlist
	Track* track = database_context.query_one<Track>(odb::query<Track>::title == track_title
												  && odb::query<Track>::album_id == album->Id()
												  && odb::query<Track>::artist_id == artist->Id());

	try {
		// Get the playlist
		// The playlist name is stored in `playlist_name` so we can just query this
		string playlist_name = ui->playlist_name->text().toStdString();
		// Ensure to trim the name so that we don't have any issues
		Utility::trim(playlist_name);
		// Is the name "Your library"? If so we convert this back to "DEFAULT"
		if (playlist_name == "Your library") {
			playlist_name = "DEFAULT";
		}

		// Initiate the query
		Playlist* playlist = database_context.query_one<Playlist>(odb::query<Playlist>::name == playlist_name);
		if (playlist == NULL) return;
		// Get the track_mapping
		Track_Playlist* track_mapping = database_context.query_one<Track_Playlist>(odb::query<Track_Playlist>::id == playlist->Id()
																				&& odb::query<Track_Playlist>::track_id == track->Id());
		if (track_mapping == NULL) return;
		// Delete the albums
		database_context.erase<Track_Playlist>(track_mapping->Id());
		// Delete the model
		ui->allAlbumsListView->model()->deleteLater();

		// Remove the model
		ui->allAlbumsListView->setModel(nullptr);

		// Delete the model from allTracksPage
		ui->allTracksListView->model()->deleteLater();

		// Remove the model from the allTracksPage
		ui->allTracksListView->setModel(nullptr);

		// Delete the model from userplaylistview
		ui->userplaylistview->model()->deleteLater();

		// Remove the model from the userplaylistview
		ui->userplaylistview->setModel(nullptr);

		// Delete the model from userlibraryview
		ui->userlibraryview->model()->deleteLater();

		// Remove the model from the userlibraryview
		ui->userlibraryview->setModel(nullptr);

	}
	catch (const odb::exception& e) {
		QMessageBox::warning(this, "Error", "Failed to delete playlist. Error: " + QString::fromStdString(e.what()));
		return;
	}
	// Commit the transaction
	t.commit();

	// Reload the page
	LoadTrackManagementPage();

	// ui->managementTab_fp->setCurrentWidget(ui->playlist);

}
// Action: Create a new playlist
void MainWindow::createPlaylistFromTrackManagement() {
	// Get the playlist name
	// We can get the playlist name from `playlist_name`
	string playlist_name = ui->playlist_name->text().toStdString();
	// Ensure to trim the name so that we don't have any issues
	Utility::trim(playlist_name);

	// Check to see if this playlist already exists
	odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());

	Playlist* track_playlist = database_context.query_one<Playlist>(odb::query<Playlist>::name == playlist_name);

	if (track_playlist != NULL) {
		QMessageBox msgBox;
		msgBox.setText("Playlist already exists.");
		msgBox.setInformativeText("You cannot have two playlists of the same name");
		msgBox.exec();
		return;
	}

	// Add the playlist
	Playlist new_playlist(playlist_name, std::to_string(current_year));
	database_context.persist(new_playlist);

	// Commit the transaction
	t.commit();

	// Remove the models and load the all playlist page
	// Delete the model
	ui->allAlbumsListView->model()->deleteLater();

	// Remove the model
	ui->allAlbumsListView->setModel(nullptr);

	// Delete the model from allTracksPage
	ui->allTracksListView->model()->deleteLater();

	// Remove the model from the allTracksPage
	ui->allTracksListView->setModel(nullptr);

	// Delete the model from userplaylistview
	ui->userplaylistview->model()->deleteLater();

	// Remove the model from the userplaylistview
	ui->userplaylistview->setModel(nullptr);

	// Delete the model from userlibraryview
	ui->userlibraryview->model()->deleteLater();

	// Remove the model from the userlibraryview
	ui->userlibraryview->setModel(nullptr);

	LoadAllPlaylistPage();
}