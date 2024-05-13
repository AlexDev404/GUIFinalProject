#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.hpp"

#include "Track.hpp"
#include "Track-odb.hxx"

void MainWindow::LoadSearchResultPage() {
	std::string search = ui->search_widget->text().toStdString();

	ui->label_srp_1->setText(QString::fromStdString("Search results for: " + search));

	odb::sqlite::database database_context = db.getDatabase();
	odb::transaction t(database_context.begin());

	// Make the connection to PlayTrack
	connect(ui->searchResultListView, &QListView::doubleClicked, this, &MainWindow::PlayTrack);

	// Make a query to the database
	odb::result<Track> result = database_context.query<Track>(odb::query<Track>::title.like("%" + search + "%"));

	// Clear the list view
	auto model = new QStandardItemModel(this);
	ui->searchResultListView->setModel(model);

	// Add the results to the list view
	for (odb::result<Track>::iterator it = result.begin(); it != result.end(); it++) {
		Track track = *(it);
		Albums* trackAlbum = (Albums*)(track.AlbumId());
		Artists* trackArtist = (Artists*)(track.ArtistId());

		QStandardItem* item = new QStandardItem(QString::fromStdString(track.Title() + "\n" + trackAlbum->Title() + "\n" + trackArtist->Name()));
		// Set the hidden text
		item->setText(item->text());

		// Remove the ability to edit the item
		item->setEditable(false);

		// Add the item to the model
		model->appendRow(item);
	}

	// Set the current index to the search result page
	ui->mainStackedWidget->setCurrentWidget(ui->searchResultPage);

	t.commit();
}