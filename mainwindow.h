#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QListView>

#include "database.hpp"
#include "Track.hpp"
#include "TrackImage.hpp"
#include "Playlist.hpp"
#include "Albums.hpp"
#include "Artists.hpp"
#include "WindowsAccount.hpp"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioDevice>
#include <QMediaDevices>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void LoadResources();
    void qMain();
    void LoadAllTracksPage(QListView* listView, QSize size, QSize icon_size);
	void LoadPlayListDisplayPage(const QModelIndex& index);
    void LoadAllPlaylistPage();
    void LoadUserManagementPage();
    void LoadTrackManagementPage();
	void LoadReportPage();
    void LoadAllAlbumsPage();
    void LoadSearchResultPage();
    void PlayTrack(const QModelIndex& index);
    /// <summary>
    /// Expects database to be open. Sets the play area data. This is the data that is displayed when a track is played.
    /// </summary>
    /// <param name="track_image"></param>
    /// <param name="track_title"></param>
    /// <param name="album_name"></param>
    /// <param name="artist_name"></param>
    void SetPlayAreaData(TrackImage& track_image, std::string track_title, std::string album_name, std::string artist_name, odb::sqlite::database& database_context);
    void UIAddTrack();
    void deleteGenreFromTrackManagement();
    void addGenreFromTrackManagement();
    void addTrackFromTrackManagement();
    void deleteTrackFromTrackManagement();
    void addAlbumFromTrackManagement();
    void deleteAlbumFromTrackManagement();
    void addTrackFromFolder();

    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();


private slots:

    // Context menus
    void ShowUserContextMenu(QPoint pos);
    void ShowAlbumContextMenu(QPoint pos);
    void ShowTracksContextMenu(QPoint pos);
    void ShowPlaylistContextMenu(QPoint pos);

    // Other stuff
    void on_settings_clicked();

    void on_search_submit_clicked();

    void on_reportButton_clicked();

    void on_your_playlists_clicked();

    void on_trackManagementButton_clicked();

    void on_userManagementButton_clicked();

    void on_all_tracks_clicked();

    void on_all_albums_clicked();

    void on_viewSongsButton_clicked();

    void on_viewAlbumsButton_clicked();

    void on_viewPlaylistButton_clicked();

    void on_languageButton_clicked();

    void on_addButton_atp_clicked();

    void on_actionOpen_Folder_triggered();

    void on_play_pause_pa_clicked();

    void on_back_pa_clicked();

    void on_forward_pa_clicked();

    void on_state_Button_ld_clicked();

    void switchToEnglish();

    void switchToSpanish();

    void on_englishButton_clicked();

    void on_spanishButton_clicked();

    void on_AddGenre_clicked();

    void on_DeleteGenre_clicked();

    void on_DeleteTrack_clicked();

    void on_DeleteAlbum_clicked();

    void on_AddAlbum_clicked();

    void on_AddTrack_clicked();

    void on_editTrackSourceFile_clicked();

    void on_search_widget_returnPressed();

private:
    Ui::MainWindow* ui;
    // The database
    database db; // Format: SQLite

    // The only media player we need
    QMediaPlayer* player = new QMediaPlayer();
    QAudioOutput* device = new QAudioOutput(QMediaDevices::defaultAudioOutput());
    QImage image;
    QPixmap pixmap;
    QStandardItem* trackView;
    Playlist defaultPlaylist = *(new Playlist("DEFAULT", "2022"));
    QUrl* track_url;
    QString folderPath;
    QString filePath;
    Track currentTrack = *(new Track("NO_NAME"));
    Windows_Account currentUser = *(new Windows_Account("NULL_USER"));

    // Icons
    QIcon searchIcon;
    QIcon settingIcon;
    QIcon userIcon;
    QIcon headphonesIcon;
    QIcon backIcon;
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon stopIcon;
    QIcon forwardIcon;
    QPixmap playlistImage;
    QIcon trackIcon;
    QIcon albumIcon;
    QIcon playlistIcon;
    QIcon languageIcon;
    QIcon usermanIcon;
    QIcon trackmanIcon;
    QIcon reportsIcon;
    QIcon englishIcon;
    QIcon spanishIcon;
};
#endif // MAINWINDOW_H
