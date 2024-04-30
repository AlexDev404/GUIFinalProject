// Logic for the all-albums page
#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::LoadAllAlbumsPage() {

	// Load all albums
	//ui->allAlbumsListView->setModel(playlist->GetAllAlbums());
	ui->allAlbumsListView->setViewMode(QListView::IconMode);
	ui->allAlbumsListView->setResizeMode(QListView::Adjust);
	ui->allAlbumsListView->setWrapping(true);
	ui->allAlbumsListView->setSpacing(10);
	ui->allAlbumsListView->setIconSize(QSize(200, 200));
	ui->allAlbumsListView->setGridSize(QSize(200, 200));
	ui->allAlbumsListView->setUniformItemSizes(true);
	//ui->allAlbumsListView->setStyleSheet("");

	// Set the current index to the all albums page

	ui->mainStackedWidget->setCurrentIndex(3);
}

