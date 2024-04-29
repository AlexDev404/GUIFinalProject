#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QFile>
#include <QFileDialog>
#include <QDirIterator>

// For populating the default playlist
#include "database.hpp"
#include "TrackManagement.hpp"

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
        // Clean up (This actually does something)
        folderPath.clear();
        folderPath.~QString();
        folderPath = nullptr;
		return;
	}

    // Let's try to get the default playlist
    db = *new database();
    db.setDatabase("userdata");
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    // Query for the default playlist
    odb::result<Playlist> playlists = database_context.query<Playlist>(odb::query<Playlist>::name == "DEFAULT");

    // The default playlist
    // What it is: A playlist that contains all the tracks in the folder
    // Basically, this is the user's library. 
    // It is the default playlist that is created when the user opens the application for the first time
    // (but of course, the user doesn't know this)
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
        //TagLib::FileRef f(filename.c_str());
        //if (!f.tag()) {
            //qDebug() << "No ID3 tag found";
          //  continue; // Skip the file if there is no ID3 tag
        //}

        // Get the file location
        std::string fileLocation = it.filePath().toStdString();

        // Add the track to the default playlist
        int exists = TrackManagement::addTrack(fileLocation, defaultPlaylist, database_context);
        if (exists == 1) {
            continue;
		}
    }

    // Update the default playlist and commit the transaction
    database_context.update(defaultPlaylist);
	t.commit();

    // Destroy all our objects
    // Clean up (to be safe)
    folderPath.clear();
    folderPath.~QString();
    folderPath = nullptr;
    // Last time, the memory was spiking up to nearly ~800MB
    // This time, it's just a mere ~43MB

    // Update the UI
    StateHasChanged(ui->allTracksListView);
}

void MainWindow::StateHasChanged(QListView* listView) {
    // Update the UI
    db = *new database();
    db.setDatabase("userdata");
    odb::sqlite::database database_context = db.getDatabase();
    odb::transaction t(database_context.begin());

    auto model = new QStandardItemModel(this);
    listView->setModel(model);


    // Query for the default playlist
    odb::result<Playlist> playlists = database_context.query<Playlist>(odb::query<Playlist>::name == "DEFAULT");

    // The default playlist
    // What it is: A playlist that contains all the tracks in the folder
    // Basically, this is the user's library. 
    // It is the default playlist that is created when the user opens the application for the first time
    // (but of course, the user doesn't know this)
    Playlist defaultPlaylist("DEFAULT", "2022");
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
        TrackImage track_image = track.Image();
        QImage image;
        // Check to see if it's 16 bytes long. If it is, it's an empty image (probably a bug)
        if (!track_image.Data() || track_image.Size() == 16) {
			image.load(":/otherfiles/assets/images/album.png");
		}
        else {
            image.loadFromData(QByteArray::fromRawData(track_image.Data(), track_image.Size() == 16 ? 0 : track_image.Size()), "JPG"); // Pretty much all of the images are JPGs
        }
        QPixmap pixmap = QPixmap::fromImage(image);
        QStandardItem* trackView = new QStandardItem(QIcon(pixmap), QString::fromLatin1(track.Title()));
        // We can add more information to the trackView if we want
        // For example, we can add the artist, album, etc.
        trackView->setEditable(false);
        // Set the album and artist below the title
        trackView->appendRow(new QStandardItem(QString::fromLatin1("test")));
        // Resize the trackView to around 500 pixels
        trackView->setSizeHint(QSize(125, 175));
        // Have the image fit the trackView
        listView->setIconSize(QSize(100, 100));


        model->appendRow(trackView);
        //ui->track_list_fp->addItem(QString::fromStdString(it->getTitle()));
    }

    // Commit the transaction
    t.commit();
}