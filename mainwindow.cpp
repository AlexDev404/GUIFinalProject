#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QFile>
#include <QColorSpace>
#include <QTranslator>


// For populating the default playlist
#include "database.hpp"
#include "Track_Playlist.hpp"
#include "Track_Playlist-odb.hxx"

// ID3 Tagging
#include "taglib/tag.h"
#include "taglib/fileref.h"

#include <QMessageBox>


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
    ui->mainStackedWidget->setCurrentWidget(ui->allTracksPage);
    ui->managementTab_fp->setCurrentWidget(ui->albums);

    // Header
    ui->user_loggedin->setIcon(QIcon(userIcon));
    ui->settings->setIcon(QIcon(settingIcon));

    // Search Buttons
    ui->search_submit->setIcon(QIcon(searchIcon));

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

    // Translation buttons
    ui->englishButton->setIcon(QIcon(englishIcon));
    ui->englishButton->setIconSize(QSize(125, 125));

    ui->spanishButton->setIcon(QIcon(spanishIcon));
    ui->spanishButton->setIconSize(QSize(125, 125));

    // Connect the English and Spanish buttons to language switching slots
    connect(ui->englishButton, &QPushButton::clicked, this, &MainWindow::switchToEnglish);
    connect(ui->spanishButton, &QPushButton::clicked, this, &MainWindow::switchToSpanish);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_settings_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->settingsPage);
}


void MainWindow::on_search_submit_clicked()
{
    this->LoadSearchResultPage();
}


void MainWindow::on_search_widget_returnPressed()
{
    this->LoadSearchResultPage();
}



void MainWindow::on_reportButton_clicked()
{
    this->LoadReportPage();
}


void MainWindow::on_your_playlists_clicked()
{
    this->LoadAllPlaylistPage();
}


void MainWindow::on_trackManagementButton_clicked()
{
    this->LoadTrackManagementPage();
}


void MainWindow::on_userManagementButton_clicked()
{
    this->LoadUserManagementPage();
}


void MainWindow::on_all_tracks_clicked()
{
    LoadAllTracksPage(ui->allTracksListView, QSize(125, 175), QSize(100, 100));
    ui->mainStackedWidget->setCurrentWidget(ui->allTracksPage);
}


void MainWindow::on_all_albums_clicked()
{
    this->LoadAllAlbumsPage();
}


void MainWindow::on_viewSongsButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->allTracksPage);
}


void MainWindow::on_viewAlbumsButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->allAlbumsPage);
}


void MainWindow::on_viewPlaylistButton_clicked()
{
    this->LoadAllPlaylistPage();
}


void MainWindow::on_languageButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->languagePage);
}

void MainWindow::switchToEnglish() {
	// Change application language to English
	QTranslator translator;
	if (translator.load("_en_US.qm")) {
		qDebug() << "English translation loaded successfully";
		qApp->installTranslator(&translator);
		ui->retranslateUi(this);
	}
	else {
		qDebug() << "Failed to load English translation file";
	}
}

void MainWindow::switchToSpanish() {
	// Change application language to Spanish
	QTranslator translator;
	if (translator.load("_es_MX.qm")) {
		qDebug() << "Spanish translation loaded successfully";
		qApp->installTranslator(&translator);
		ui->retranslateUi(this);
	}
	else {
		qDebug() << "Failed to load Spanish translation file";
	}
}

void MainWindow::on_actionOpen_Folder_triggered()
{
    this->UIAddTrack();
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
    Track_Playlist* track_map_ = database_context.query_one<Track_Playlist>(odb::query<Track_Playlist>::playlist_id == defaultPlaylist.Id() &&
        odb::query<Track_Playlist>::track_id == (currentTrack.Id() - 1)); // Same thing but -1

    if (track_map_ == NULL) {
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

    // Set the play area data
    SetPlayAreaData(track_image, currentTrack.Title(), track_album.Title(), track_artist.Name(), database_context);


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
        odb::query<Track_Playlist>::track_id == (currentTrack.Id() + 1)); 

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

    // Set the play area data
    SetPlayAreaData(track_image, currentTrack.Title(), track_album.Title(), track_artist.Name(), database_context);

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


void MainWindow::on_state_Button_ld_clicked()
{
    this->on_play_pause_pa_clicked();
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        ui->state_Button_ld->setText("Pause");
    }
    else {
        ui->state_Button_ld->setText("Play");
    }
}


void MainWindow::on_englishButton_clicked()
{
    this->switchToEnglish();
}


void MainWindow::on_spanishButton_clicked()
{
    this->switchToSpanish();
}


void MainWindow::on_AddGenre_clicked()
{
    this->addGenreFromTrackManagement();
}


void MainWindow::on_DeleteGenre_clicked()
{
    this->deleteGenreFromTrackManagement();
}


void MainWindow::on_DeleteTrack_clicked()
{
    this->deleteTrackFromTrackManagement();
}


void MainWindow::on_AddTrack_clicked()
{
    this->addTrackFromTrackManagement();
}

void MainWindow::on_DeleteAlbum_clicked()
{
    this->deleteAlbumFromTrackManagement();
}

void MainWindow::on_AddAlbum_clicked()
{
    this->addAlbumFromTrackManagement();
}

void MainWindow::on_editTrackSourceFile_clicked()
{
    this->addTrackFromFolder();
}

void MainWindow::on_addplaylisttrack_clicked()
{
    this->addTrackfromPlaylistFromTrackManagement();
}


void MainWindow::on_deleteplaylisttrack_clicked()
{
    // call the delete function
    this->deleteTrackfromPlaylistFromTrackManagement();
}


void MainWindow::on_CreateNewPlaylist_clicked()
{
    // Create a new playlist
    this->createPlaylistFromTrackManagement();
}

