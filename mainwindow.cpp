#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::qMain(){
    // Main entry point
}
