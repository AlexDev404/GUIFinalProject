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


#include <qDebug>

/// <summary>
/// Track Management class
/// </summary>
class TrackManagement {
public:
	/// <summary>
    /// ******!!! EXPECTS THE DATABASE TO BE OPEN !!!******
    /// 
    /// Adds a track to the database. 
    /// Takes into account that if the album, artist, or genre do not exist, they need to be added to the database.
    /// </summary>
    /// <param name="f"></param>
    /// <param name="defaultPlaylist"></param>
    /// <param name="database_context"></param>
    /// <param name="fileLocation"></param>
	static void addTrack(TagLib::FileRef f, Playlist defaultPlaylist, odb::sqlite::database& database_context, std::string fileLocation);

};