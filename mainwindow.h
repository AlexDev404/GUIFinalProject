#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QListView>

#include "database.hpp"
#include "Track.hpp"
#include "Playlist.hpp"

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
    void StateHasChanged(QListView* listView, QSize size, QSize icon_size);
    void PlayTrack(const QModelIndex& index);
    void UIAddTrack();
    void LoadAllAlbumsPage();
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
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
