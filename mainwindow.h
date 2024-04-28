#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.hpp"
#include <QStandardItemModel>
#include <QListView>

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
    void StateHasChanged(QListView* listView);
    MainWindow(QWidget *parent = nullptr);
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

private:
    Ui::MainWindow *ui;
    // The database
    database db; // Format: SQLite
};
#endif // MAINWINDOW_H
