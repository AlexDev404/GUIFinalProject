#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QFile>
#include <QFileDialog>
#include <QDirIterator>

// For populating the default playlist
#include "database.hpp"

// Schemas
#include "Track.hpp"
#include "Albums.hpp""
#include "Artists.hpp"
#include "Genres.hpp"
#include "Playlist.hpp"
#include "Track_Playlist.hpp"

// Mappings
#include "Track-odb.hxx"
#include "Albums-odb.hxx"
#include "Artists-odb.hxx"
#include "Genres-odb.hxx"
#include "Playlist-odb.hxx"
#include "Track_Playlist-odb.hxx"

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
    QIcon searchIcon(":/otherfiles/assets/images/searchImg.png");
    QIcon settingIcon(":/otherfiles/assets/images/settings.png");
    QIcon userIcon(":/otherfiles/assets/images/user.png");
    QIcon headphonesIcon(":/otherfiles/assets/images/headphones.png");
    QIcon backIcon(":/otherfiles/assets/images/backward.png");
    QIcon playIcon(":/otherfiles/assets/images/play.png");
    QIcon forwardIcon(":/otherfiles/assets/images/forward.png");
    QPixmap playlistImage(":/otherfiles/assets/images/playlist.jpg");

    QIcon trackIcon(":/otherfiles/assets/images/tracks.png");
    QIcon albumIcon(":/otherfiles/assets/images/album.png");
    QIcon playlistIcon(":/otherfiles/assets/images/playlist.png");
    QIcon languageIcon(":/otherfiles/assets/images/lang.png");
    QIcon usermanIcon(":/otherfiles/assets/images/user_big.png");
    QIcon trackmanIcon(":/otherfiles/assets/images/trackman.png");
    QIcon reportsIcon(":/otherfiles/assets/images/reports.png");

    QIcon englishIcon(":/otherfiles/assets/images/englishflag.png");
    QIcon spanishIcon(":/otherfiles/assets/images/spanishflag.png");

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
    ui->play_pause_pa->setIcon(QIcon(playIcon));
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
    ui->mainStackedWidget->setCurrentIndex(3);
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
	// Open a file dialog to select a folder
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Open Folder"), "C:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(folderPath.isEmpty()){
		return;
	}

    // Let's try to get the default playlist
    db = *new database();
    db.setDatabase("userdata");
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());
    odb::result<Playlist> playlists = database_context.query<Playlist>(odb::query<Playlist>::name == "DEFAULT");

    Playlist defaultPlaylist("DEFAULT", "2022");
    if(playlists.begin() == playlists.end()){
        database_context.persist(defaultPlaylist);
	}
	else{
		defaultPlaylist = *playlists.begin();
	}

    // Walk through the folder and get all MP3 files in it
    QDirIterator it(folderPath, QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        // Get the next file
        it.next();

        // Parse the ID3 tags
        std::string filename = it.filePath().toStdString();
        TagLib::FileRef f(filename.c_str());
        if (!f.tag()) {
			//qDebug() << "No ID3 tag found";
			continue;
		}

        // Query to see if the album already exists
        odb::result<Albums> albums = database_context.query<Albums>(odb::query<Albums>::title == f.tag()->album().toCString());
        Albums album(f.tag()->album().toCString(), std::to_string(f.tag()->year()));
        if(albums.begin() == albums.end()){
            database_context.persist(album);
        }
        else {
            album = *albums.begin();
        }

        // Query to see if the artist already exists
        odb::result<Artists> artists = database_context.query<Artists>(odb::query<Artists>::name == f.tag()->artist().toCString());
        Artists artist(f.tag()->artist().toCString());
        if(artists.begin() == artists.end()){
            database_context.persist(artist);
		}
		else {
            artist = *artists.begin();
		}

        // Query to see if the genre already exists
        odb::result<Genres> genres = database_context.query<Genres>(odb::query<Genres>::title == f.tag()->genre().toCString());
        Genres genre(f.tag()->genre().toCString());
        if(genres.begin() == genres.end()){
            database_context.persist(genre);
        }
        else {
            genre = *genres.begin();
		}

        // Get the duration
        int duration = f.audioProperties()->lengthInMilliseconds() / 1000;

        // Get the file location
        std::string fileLocation = it.filePath().toStdString();

        // Query to see if the genre already exists
        odb::result<Track> tracks = database_context.query<Track>(odb::query<Track>::file_location == fileLocation);
        // Create a new track object
        Track track(f.tag()->title().toCString(), &artist, &album, &genre, "", std::to_string(f.tag()->year()), duration, fileLocation);
        if (tracks.begin() == tracks.end()) {
            database_context.persist(track);
        }
        else {
            track = *tracks.begin();
        }

        
        // Add the track to the default playlist
        Track_Playlist playlist_map(&track, &defaultPlaylist);
        try {
            database_context.persist(playlist_map);
        }
		catch(odb::exception &e){
			qDebug() << e.what();
		}

    }
    database_context.update(defaultPlaylist);
	t.commit();

}

