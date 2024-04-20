#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    LoadResources();
    ui->setupUi(this);
}

void MainWindow::LoadResources(){
    // Load resources, fonts, etc.
    // Load the fonts
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

    // Load the external CSS file
    QFile styleFile(":/otherfiles/assets/css/finalproject.css");  // Replace with the actual path
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet);  // Apply stylesheet to the entire application
        styleFile.close();
    } else {
        qDebug() << "Failed to open CSS file:" << styleFile.errorString();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::qMain(){
    // Main entry point
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

