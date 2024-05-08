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
	odb::result<Track_Playcount> trackList = database_context.query<Track_Playcount>(odb::query<Track_Playcount>::user_id == currentUser.Id());
	
	// Initialize the total number of tracks played
	int totalTracksPlayed = 0; // The total number of tracks played

	// Initialize the total number of albums played
	int totalAlbumsPlayed = 0;
	Albums* album_now = new Albums("NULL_ALBUM", "NULL_DATE");

	// Initialize the total number of artists played
	int totalArtistsPlayed = 0;
	Artists* artist_now = new Artists("NULL_ARTIST");

	// Initialize the total time spent listening to music
	double totalTimeSpentListeningToMusic = 0; // The total time spent listening to music

	// Initialize the average time spent listening to music
	double averageTimeSpentListeningToMusic = 0; // The average time spent listening to music

	// Iterate through the trackList and get the total number of tracks played, albums played, artists played, and the total time spent listening to music
	for (odb::result<Track_Playcount>::iterator i = trackList.begin(); i != trackList.end(); ++i) {
		// Convert the track_id to a Track object
		Track track = *(i->TrackId()); // Get the track object
		Albums album = *(track.AlbumId()); // Get the album object
		Artists artist = *(track.ArtistId()); // Get the artist object

		// If the album is not the same as the previous album, increment the total number of albums played
		if ((album_now == NULL) || (album_now->Id() != album.Id())) {
			*album_now = album;
			totalAlbumsPlayed++;
		}

		// If the artist is not the same as the previous artist, increment the total number of artists played
		if ((artist_now == NULL) || (artist_now->Id() != artist.Id())) {
			*artist_now = artist;
			totalArtistsPlayed++;
		}

		totalTracksPlayed++;
		totalTimeSpentListeningToMusic += track.Duration();
	}

	delete album_now;
	delete artist_now;

	// Now, calculate the average time spent listening to music
	averageTimeSpentListeningToMusic = totalTimeSpentListeningToMusic / totalTracksPlayed;

	// Start to fill out the columns of reportTableDisplay
    // With its appropriate values
	//ui->reportTableDisplay->setColumnCount(3);
	//ui->reportTableDisplay->setRowCount(5);
	//ui->reportTableDisplay->setHorizontalHeaderItem(0, new QTableWidgetItem("Total played (Tracks, Albums, Artists)")); // The first column is the total number of tracks, albums, and artists played
	//ui->reportTableDisplay->setHorizontalHeaderItem(1, new QTableWidgetItem("Average played (Tracks, Albums, Artists)")); // The second column is the average number of tracks, albums, and artists played
	//ui->reportTableDisplay->setHorizontalHeaderItem(2, new QTableWidgetItem("Total Listening Time")); // The third column is the total time spent listening to music

	//// Set the total number of tracks played
	//ui->reportTableDisplay->setItem(0, 0, new QTableWidgetItem(QString::number(totalTracksPlayed))); // Set the total number of tracks played
	//ui->reportTableDisplay->setItem(0, 1, new QTableWidgetItem(QString::number(averageTimeSpentListeningToMusic / 60) + " minutes")); // Set the average time spent listening to music
	//ui->reportTableDisplay->setItem(0, 2, new QTableWidgetItem(QString::number(totalTimeSpentListeningToMusic / 60) + " minutes")); // Set the total time spent listening to music

	//// Set the total number of albums played
	//ui->reportTableDisplay->setItem(1, 0, new QTableWidgetItem(QString::number(totalAlbumsPlayed)));
	//ui->reportTableDisplay->setItem(1, 1, new QTableWidgetItem("N/A"));
	//ui->reportTableDisplay->setItem(1, 2, new QTableWidgetItem("N/A"));

	//// Set the total number of artists played
	//ui->reportTableDisplay->setItem(2, 0, new QTableWidgetItem(QString::number(totalArtistsPlayed)));
	//ui->reportTableDisplay->setItem(2, 1, new QTableWidgetItem("N/A"));
	//ui->reportTableDisplay->setItem(2, 2, new QTableWidgetItem("N/A"));


	//// Set the column width of the reportTableDisplay
	////ui->reportTableDisplay->setColumnWidth(0, 100);
	////ui->reportTableDisplay->setColumnWidth(1, 100);
	////ui->reportTableDisplay->setColumnWidth(2, 100);

	////// Set the row height of the reportTableDisplay
	////ui->reportTableDisplay->setRowHeight(0, 50);
	////ui->reportTableDisplay->setRowHeight(1, 50);
	////ui->reportTableDisplay->setRowHeight(2, 50);
	////ui->reportTableDisplay->setRowHeight(3, 50);
	////ui->reportTableDisplay->setRowHeight(4, 50);

	//// Set the font of the reportTableDisplay
	//ui->reportTableDisplay->setFont(QFont("Arial", 12, QFont::Bold));

	//// Set the font of the reportTableDisplay headers
	//ui->reportTableDisplay->horizontalHeader()->setFont(QFont("Arial", 12, QFont::Bold));

	//// Set the font of the reportTableDisplay items
	//ui->reportTableDisplay->horizontalHeaderItem(0)->setFont(QFont("Arial", 12, QFont::Bold));
	//ui->reportTableDisplay->horizontalHeaderItem(1)->setFont(QFont("Arial", 12, QFont::Bold));
	//ui->reportTableDisplay->horizontalHeaderItem(2)->setFont(QFont("Arial", 12, QFont::Bold));

	t.commit(); // Don't need the database anymore beyond this point
}