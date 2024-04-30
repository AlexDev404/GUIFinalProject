QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Albums-odb.cxx \
    Albums.cpp \
    AllTracksPage.cpp \
    Artists-odb.cxx \
    Artists.cpp \
    Genres-odb.cxx \
    Genres.cpp \
    Playlist-odb.cxx \
    Playlist.cpp \
    Roles-odb.cxx \
    Roles.cpp \
    Track-odb.cxx \
    Track.cpp \
    TrackImage.cpp \
    TrackManagement.cpp \
    Track_Playcount-odb.cxx \
    Track_Playcount.cpp \
    Track_Playlist-odb.cxx \
    Track_Playlist.cpp \
    WindowsAccount-odb.cxx \
    WindowsAccount.cpp \
    database.cpp \
    main.cpp \
    mainwindow.cpp \
    qMain.cpp

HEADERS += \
    Albums-odb.hxx \
    Albums.hpp \
    Artists-odb.hxx \
    Artists.hpp \
    Genres-odb.hxx \
    Genres.hpp \
    Playlist-odb.hxx \
    Playlist.hpp \
    Roles-odb.hxx \
    Roles.hpp \
    Track-odb.hxx \
    Track.hpp \
    TrackImage.hpp \
    TrackManagement.hpp \
    Track_Playcount-odb.hxx \
    Track_Playcount.hpp \
    Track_Playlist-odb.hxx \
    Track_Playlist.hpp \
    WindowsAccount-odb.hxx \
    WindowsAccount.hpp \
    database.hpp \
    mainwindow.h \
    ui_mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc \
    resources.qrc

DISTFILES += \
    README.md \
    assets/css/finalproject.css

INCLUDEPATH += \
    ./include

LIBS += \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/odb-sqlite.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/odb-mssql.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/odb-mysql.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/odb-pgsql.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/odb-boost.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/odb-qt.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/odb.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/crypto.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/mysqlclient.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/pq.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/sqlite3.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/ssl.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/odb-debug/z.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/Qt6EntryPointd.lib \
    E:\Data\PROGRAMMING\Qt\OOPFinalProject\include\lib\tag.lib \
    E:\Data\PROGRAMMING\Qt\OOPFinalProject\include\lib\tag_c.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/Qt6Multimedia.lib \
    C:/Qt/6.6.2/msvc2019_64/lib/Qt6MultimediaWidgets.lib \
    shell32.lib \
    kernel32.lib \
    user32.lib \
    gdi32.lib \
    winspool.lib \
    comdlg32.lib \
    advapi32.lib \
    shell32.lib \
    ole32.lib \
    oleaut32.lib \
    uuid.lib \
    odbc32.lib \
    odbccp32.lib \
