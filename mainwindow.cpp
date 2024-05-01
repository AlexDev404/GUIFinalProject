#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QFile>
#include <QColorSpace>

// For populating the default playlist
#include "database.hpp"
#include "Track_Playlist.hpp"
#include "Track_Playlist-odb.hxx"

// Schemas
#include "Playlist.hpp"

// Mappings
#include "Playlist-odb.hxx"

// ID3 Tagging
#include "taglib/tag.h"
#include "taglib/fileref.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LoadResources();
    qMain();
}

void MainWindow::LoadResources(){
    // ------------------------------------ Load resources, fonts, etc. ------------------------------------
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-Black.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-BlackItalic.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-ExtraBold.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-ExtraBoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-ExtraLight.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-ExtraLightItalic.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-Italic.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-Light.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-Medium.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-SemiBold.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-SemiBoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-Thin.ttf");
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/WorkSans-ThinItalic.ttf");
    // Variable fonts (needed?)
    QFontDatabase::addApplicationFont(":/resources/assets/fonts/variable/WorkSans-VariableFont_wght.ttf");



    // ------------------------------------ Load the external CSS file ------------------------------------
    QFile styleFile(":/otherfiles/assets/css/finalproject.css");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet);
        styleFile.close();
    }
    else {
        qDebug() << "Failed to open CSS file:" << styleFile.errorString();
    }

    // ------------------------------------ Load an image from file ------------------------------------
    searchIcon = *(new QIcon(":/otherfiles/assets/images/searchImg.png"));
    settingIcon = *(new QIcon(":/otherfiles/assets/images/settings.png"));
    userIcon = *(new QIcon(":/otherfiles/assets/images/user.png"));
    headphonesIcon = *(new QIcon(":/otherfiles/assets/images/headphones.png"));
    backIcon = *(new QIcon(":/otherfiles/assets/images/backward.png"));
    playIcon = *(new QIcon(":/otherfiles/assets/images/play.png"));
    pauseIcon = *(new QIcon(":/otherfiles/assets/images/pause.png"));
    stopIcon = *(new QIcon(":/otherfiles/assets/images/stop.png"));
    forwardIcon = *(new QIcon(":/otherfiles/assets/images/forward.png"));
    playlistImage = *(new QPixmap(":/otherfiles/assets/images/playlist.jpg"));

    trackIcon = *(new QIcon(":/otherfiles/assets/images/tracks.png"));
    albumIcon = *(new QIcon(":/otherfiles/assets/images/album.png"));
    playlistIcon = *(new QIcon(":/otherfiles/assets/images/playlist.png"));
    languageIcon = *(new QIcon(":/otherfiles/assets/images/lang.png"));
    usermanIcon = *(new QIcon(":/otherfiles/assets/images/user_big.png"));
    trackmanIcon = *(new QIcon(":/otherfiles/assets/images/trackman.png"));
    reportsIcon = *(new QIcon(":/otherfiles/assets/images/reports.png"));

    englishIcon = *(new QIcon(":/otherfiles/assets/images/englishflag.png"));
    spanishIcon = *(new QIcon(":/otherfiles/assets/images/spanishflag.png"));

    // -------------------------------------------- Apply icons --------------------------------------------

    // Mainwindow
    setWindowTitle("Music in Action");
    setWindowIcon(QIcon(headphonesIcon));

    // Setting initial windows
    ui->mainStackedWidget->setCurrentIndex(4);
    ui->managementTab_fp->setCurrentIndex(0);
    ui->trackManagementSub->setCurrentIndex(0);

    // Header
    ui->user_loggedin->setIcon(QIcon(userIcon));
    ui->settings->setIcon(QIcon(settingIcon));

    // Search Buttons
    ui->search_submit->setIcon(QIcon(searchIcon));
    ui->SearchEditTrackButton_fp->setIcon(QIcon(searchIcon));

    // Play Area Icons
    ui->back_pa->setIcon(QIcon(backIcon));
    ui->back_pa->setIcon(QIcon(backIcon));
    ui->play_pause_pa->setIcon(QIcon(stopIcon));
    ui->forward_pa->setIcon(QIcon(forwardIcon));

    int h = ui->track_image_pa->height();    // Get the height & width of the QLabel
    int w = ui->track_image_pa->width();
    ui->track_image_pa->setPixmap(playlistImage.scaled(w, h));

    // Settings window buttons
    ui->viewSongsButton->setIcon(QIcon(trackIcon));
    ui->viewSongsButton->setIconSize(QSize(35, 35));

    ui->viewAlbumsButton->setIcon(QIcon(albumIcon));
    ui->viewAlbumsButton->setIconSize(QSize(35, 35));

    ui->viewPlaylistButton->setIcon(QIcon(playlistIcon));
    ui->viewPlaylistButton->setIconSize(QSize(35, 35));

    ui->languageButton->setIcon(QIcon(languageIcon));
    ui->languageButton->setIconSize(QSize(35, 35));

    ui->userManagementButton->setIcon(QIcon(usermanIcon));
    ui->userManagementButton->setIconSize(QSize(35, 35));

    ui->trackManagementButton->setIcon(QIcon(trackmanIcon));
    ui->trackManagementButton->setIconSize(QSize(35, 35));

    ui->reportButton->setIcon(QIcon(reportsIcon));
    ui->reportButton->setIconSize(QSize(35, 35));

    // Connect the combo box to track settings window
    connect(ui->trackManagementcomboBox, SIGNAL(currentIndexChanged(int)), ui->trackManagementSub, SLOT(setCurrentIndex(int)));

    // Translation buttons
    ui->englishButton->setIcon(QIcon(englishIcon));
    ui->englishButton->setIconSize(QSize(125, 125));

    ui->spanishButton->setIcon(QIcon(spanishIcon));
    ui->spanishButton->setIconSize(QSize(125, 125));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_settings_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(2);
}


void MainWindow::on_search_submit_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(8);
}


void MainWindow::on_reportButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(7);
}


void MainWindow::on_your_playlists_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(6);
}


void MainWindow::on_trackManagementButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
}


void MainWindow::on_userManagementButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(5);
}


void MainWindow::on_all_tracks_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(4);
}


void MainWindow::on_all_albums_clicked()
{
    this->LoadAllAlbumsPage();
}


void MainWindow::on_viewSongsButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(4);
}


void MainWindow::on_viewAlbumsButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(3);
}


void MainWindow::on_viewPlaylistButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(6);
}


void MainWindow::on_languageButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(9);
}

void MainWindow::on_actionOpen_Folder_triggered()
{
    this->UIAddTrack();
}

void MainWindow::StateHasChanged(QListView* listView, QSize size, QSize icon_size) {
    // Update the UI
    db = *new database();
    db.setDatabase("userdata");
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    auto model = new QStandardItemModel(this);
    listView->setModel(model);
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
        trackView = new QStandardItem(QIcon(pixmap), QString::fromLatin1((track.Title().empty()? track.FileName(): track.Title()) + "\n"));
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

void MainWindow::on_play_pause_pa_clicked()
{
    // Check if we have any media loaded
    // If we don't, we can't play anything
    if (player->mediaStatus() == QMediaPlayer::NoMedia) {
		return;
	}
    // Check if we are paused already
    if (player->playbackState() == QMediaPlayer::PausedState) {
		player->play();
        // Change the icon to a pause icon
        ui->play_pause_pa->setIcon(pauseIcon);
	}
    else {
		player->pause();
        // Change the icon to a play icon
		ui->play_pause_pa->setIcon(playIcon);
	}
}


void MainWindow::on_back_pa_clicked()
{
    // Check if we have any media loaded
    // If we don't, we can't play anything
    if (player->mediaStatus() == QMediaPlayer::NoMedia) {
        return;
    }
    // Are we at the start of the playlist?
    if (currentTrack.Id() == 1) {
		return;
	}
    // Play the next track in the DEFAULT playlist
    // Get the current track's position in the track_playlist table
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());
    Track_Playlist track_map = *(database_context.query_one<Track_Playlist>(odb::query<Track_Playlist>::playlist_id == defaultPlaylist.Id() &&
        odb::query<Track_Playlist>::track_id == (currentTrack.Id() - 1))); // Same thing but -1

    // Map this track_map to a track
    currentTrack = *(track_map.TrackId());

    // Get the URL of the track
    track_url = new QUrl(QString::fromStdString(currentTrack.FileName()));

    qDebug() << "Now Playing: " << QString::fromStdString(currentTrack.Title());

    TrackImage track_image = currentTrack.Image();
    Albums track_album = *(currentTrack.AlbumId());
    Artists track_artist = *(currentTrack.ArtistId());

    // Set the QLabel, "track_image_pa" to the album art of the track
    ui->track_image_pa->setPixmap(QPixmap::fromImage(QImage::fromData(QByteArray::fromRawData(track_image.Data(), track_image.Size()), "JPG")));

    // Set the QLabel, "track_name_pa" to the title of the track
    ui->track_name_pa->setText(QString::fromStdString(currentTrack.Title()));

    // Set the QLabel, "mia_pa_album" to the album of the track
    ui->mia_pa_album->setText(QString::fromStdString(track_album.Title()));

    // Set the QLabel, "mia_pa" to the artist of the track
    ui->mia_pa->setText(QString::fromStdString(track_artist.Name()));


    player->stop();
    player->setSource(*track_url);

    // Check if there was an error setting the media content
    if (player->error() != QMediaPlayer::NoError) {
        qDebug() << "----Error setting media content----";
        qDebug() << "Error:" << player->errorString();
        qDebug() << "File path:" << track_url;
        return;
    }

    // Set the audio device and play the media
    player->setAudioOutput(device);
    player->play();

    t.commit();

}


void MainWindow::on_forward_pa_clicked()
{
    // Check if we have any media loaded
    // If we don't, we can't play anything
    if (player->mediaStatus() == QMediaPlayer::NoMedia) {
        return;
    }
    // Play the next track in the DEFAULT playlist
    // Get the current track's position in the track_playlist table
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());
    Track_Playlist* track_map_ = database_context.query_one<Track_Playlist>(odb::query<Track_Playlist>::playlist_id == defaultPlaylist.Id() &&
        odb::query<Track_Playlist>::track_id == (currentTrack.Id() + 1)); // Same thing but +1

    // Are we at the end of the playlist?
    if (track_map_ == nullptr) {
        		return;
    }

    Track_Playlist track_map = *track_map_;

    // Map this track_map to a track
    currentTrack = *(track_map.TrackId());

    // Get the URL of the track
    track_url = new QUrl(QString::fromStdString(currentTrack.FileName()));

    qDebug() << "Now Playing: " << QString::fromStdString(currentTrack.Title());

    TrackImage track_image = currentTrack.Image();
    Albums track_album = *(currentTrack.AlbumId());
    Artists track_artist = *(currentTrack.ArtistId());

    // Set the QLabel, "track_image_pa" to the album art of the track
    ui->track_image_pa->setPixmap(QPixmap::fromImage(QImage::fromData(QByteArray::fromRawData(track_image.Data(), track_image.Size()), "JPG")));

    // Set the QLabel, "track_name_pa" to the title of the track
    ui->track_name_pa->setText(QString::fromStdString(currentTrack.Title()));

    // Set the QLabel, "mia_pa_album" to the album of the track
    ui->mia_pa_album->setText(QString::fromStdString(track_album.Title()));

    // Set the QLabel, "mia_pa" to the artist of the track
    ui->mia_pa->setText(QString::fromStdString(track_artist.Name()));


    player->stop();
    player->setSource(*track_url);

    // Check if there was an error setting the media content
    if (player->error() != QMediaPlayer::NoError) {
        qDebug() << "----Error setting media content----";
        qDebug() << "Error:" << player->errorString();
        qDebug() << "File path:" << track_url;
        return;
    }

    // Set the audio device and play the media
    player->setAudioOutput(device);
    player->play();

    t.commit();
}
